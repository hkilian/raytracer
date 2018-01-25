
// System
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    // Logging
    #define LOG_INFO(TEXT) std::cout << "INFO: " << TEXT << std::endl;
    #define LOG_VEC3(TEXT, vec3) std::cout << TEXT << " = " << vec3.x_ << ", " << vec3.y_ << ", " << vec3.z_ << std::endl;
#else
    // Logging
    #include "easylogging++.h"
    INITIALIZE_EASYLOGGINGPP
    #define LOG_INFO(TEXT) LOG(INFO) << TEXT;
    #define LOG_VEC3(TEXT, vec3) LOG(INFO) << TEXT << " = " << vec3.x_ << ", " << vec3.y_ << ", " << vec3.z_;
#endif

// Raytracer
#include "Vec3.hpp"
#include "Ray.hpp"
#include "TextureImage.hpp"
#include "Camera.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;

// Starts up SDL and creates window
bool init();

// Shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window* window_ = NULL;

// The window renderer
SDL_Renderer* renderer_ = NULL;

// SDL Event handler
SDL_Event e;

// Screen texture
TextureImage screenTexture_;

// Timer
float timer = 0;

//Main loop flag
bool quit = false;

// Main camera
Camera camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

int pixelsDoneX = 0;
int pixelsDoneY = 0;
bool finishedRender = false;

bool init() {
    
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        //Create window
        window_ = SDL_CreateWindow( "Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        
        if( window_ == NULL ) {
            printf( "Window could not be created! %s\n", SDL_GetError() );
            success = false;
        } else {
            //Create renderer for window
            renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_ACCELERATED );
            if( renderer_ == NULL ) {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            } else {
                
                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer_, 0xFF, 0xFF, 0xFF, 0xFF );
                
            }
        }
    }
    
    return success;
}

void close() {
    
    //Free loaded images
    screenTexture_.free();
    
    //Destroy window
    SDL_DestroyRenderer( renderer_ );
    SDL_DestroyWindow( window_ );
    window_ = NULL;
    renderer_ = NULL;
    
    //Quit SDL subsystems
    SDL_Quit();
}

Vec3 randInUnitSphere() {
    
    Vec3 p = Vec3(99,99,99);
    //LOG_INFO(drand48());
    do {
        p = 1.0 * Vec3(drand48(), drand48(), drand48()) - Vec3(1,1,1);
    } while(p.SquaredLength() >= 1.0 );
    
    return p;
}

struct HitData {
    float t;
    Vec3 p;
    Vec3 normal;
};

bool hitSphere(Vec3 center, float t_min, float t_max, float radius, const Ray ray, HitData &data) {
    
    // Distance between center of sphere and ray origin
    Vec3 dis = ray.Origin() - center;
    float a = Dot(ray.Direction(), ray.Direction());
    float b = Dot(dis, ray.Direction());
    float c = Dot(dis, dis) - radius * radius;
    float discriminant = b*b - a*c;
    
    if(discriminant > 0) {
        
        float temp = (-b - sqrt(b*b-a*c)) / (a);
        
        if(temp < t_max && temp > t_min) {
            
            // Set data
            data.t = temp;
            data.p = ray.PointAtParameter(data.t);
        
            data.normal = Vec3(data.p - center).Normalized();
            data.normal = 0.5*Vec3(data.normal.x_+1.0f, data.normal.y_+1.0f, data.normal.z_+1.0f);
            
            return true;
            
        }
        
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            data.t = temp;
            data.p = ray.PointAtParameter(data.t);
            data.normal = (data.p - center) / radius;
            return true;
        }
        
    }
    
    return false;
    
}

bool hit(const Ray &ray, float t_min, float t_max, HitData &data) {
    
    // Main sphere
    if(hitSphere(Vec3(0.4, 0.0, -1), t_min, t_max, 0.4, ray, data)) {
        return true;
    }
    
    // Main sphere
    if(hitSphere(Vec3(-0.4, 0.0, -1), t_min, t_max, 0.4, ray, data)) {
        return true;
    }
    
    // Ground
    if(hitSphere(Vec3(0.0, 100.4, -1), t_min, t_max, 100.0, ray, data)) {
        return true;
    }
    
    return false;
    
}

Vec3 color(const Ray &ray, int depth) {
    
    Vec3 out = Vec3(0,0,0);
    HitData data;
    if(hit(ray, 0.001, 9999999999.0, data)) {
        
        Vec3 target = data.p + data.normal + randInUnitSphere();
        out = 0.5 * color(Ray(data.p, target - data.p), depth+1);
        //out = data.normal;
        
    } else {
    
        Vec3 direction = ray.Direction().Normalized();
        float t = 0.5f * (direction.y_ + 1.0f);
        out = (1.0-t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
        
    }
    
    return out;
    
}

void loop(void) {
    
    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ) {
        //User requests quit
        if( e.type == SDL_QUIT ) {
            quit = true;
        } else if( e.type == SDL_KEYDOWN ) {
            // Any other key down
        }
    }
    
    if(finishedRender) {
        return;
    }
    
    if( !screenTexture_.lockTexture() ) {
        printf( "Unable to lock Foo' texture!\n" );
    } else {
        // Edit pixels
        Uint32 format = SDL_GetWindowPixelFormat( window_ );
        SDL_PixelFormat* mappingFormat = SDL_AllocFormat( format );
        
        // Get pixel data
        Uint32* pixels = (Uint32*)screenTexture_.getPixels();
        int pixelCount = ( screenTexture_.getPitch() / 4 ) * screenTexture_.getHeight();
        
        if(pixels == nullptr) {
            printf( "No pixels returned. \n" );
        }
        
        timer += 1;
        int pixelsPerFrame = 500;
        int pixelsRenderedThisFrame = 0;
        bool finishedFrame = false;
        
        // Fill screen
        for( int y = pixelsDoneY; y < SCREEN_HEIGHT; ++y ) {
            if(finishedFrame) break;
            for( int x = pixelsDoneX; x < SCREEN_WIDTH; ++x ) {
                
                if(finishedFrame) break;
                int samplesPerRay = 50;
                Vec3 col(0,0,0);
                
                // Samples
                for( int s = 0; s < samplesPerRay; ++s ) {
                    
                    float u = (float)(x + drand48()) / float(SCREEN_WIDTH);
                    float v = (float)(y + drand48()) / float(SCREEN_HEIGHT);
                    
                    Ray ray = Ray(camera.origin, camera.lowerLeft + (u*camera.horizontal) + (v*camera.vertical));
                    col += color(ray, 0);
                    
                }
                
                col /= samplesPerRay;
                
                // Bring into correct gamma
                col = Vec3(sqrt(col.x_), sqrt(col.y_), sqrt(col.z_));
                int ir = int(255.99*col.x_);
                int ig = int(255.99*col.y_);
                int ib = int(255.99*col.z_);
                
                int i = x + SCREEN_WIDTH * y;
                Uint32 pixel = SDL_MapRGBA( mappingFormat, ir, ig, ib, 255);
                pixels[i] = pixel;
                
                pixelsRenderedThisFrame += 1;
                pixelsDoneX = x;
                pixelsDoneY = y;
                
                if(pixelsRenderedThisFrame >= pixelsPerFrame) {
                    finishedFrame = true;
                }
                
            }
            if(finishedFrame == false) {
                pixelsDoneX = 0;
            }
        }
        
        // See if finished
        if(pixelsDoneY >= SCREEN_HEIGHT && pixelsDoneX >= SCREEN_WIDTH) {
            //finishedRender = true;
        }
        
        //Unlock texture
        screenTexture_.unlockTexture();
        
        //Free format
        SDL_FreeFormat( mappingFormat );
    }
    
    
    //Clear screen
    SDL_SetRenderDrawColor( renderer_, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( renderer_ );
    
    //Modulate and render texture
    screenTexture_.render(renderer_, 0, 0 );
    
    //Update screen
    SDL_RenderPresent( renderer_ );
    
}


int main( int argc, char* args[] ) {
    
    #ifndef __EMSCRIPTEN__
        // Configure logging
        el::Configurations conf("./logging.conf");
        el::Loggers::reconfigureAllLoggers(conf);
    #endif
    
    LOG_INFO("Raytracer application start");
    
    //Start up SDL and create window
    if( !init() ) {
        LOG_INFO("Failed to initialize!");
    } else {
        
        // Create blank surface to render onto
        if( !screenTexture_.createBlank(renderer_, window_, SCREEN_WIDTH, SCREEN_HEIGHT) ) {
            LOG_INFO("Failed to create texture for screen");
        } else {
            
            #ifdef __EMSCRIPTEN__
                emscripten_set_main_loop(loop, 0, 1);
            #else
                while( !quit ) {
                    loop();
                }
            #endif
        }
    }
    
    //Free resources and close SDL
    close();
    
    return 0;
}

