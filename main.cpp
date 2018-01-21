
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

bool hitSphere(Vec3 center, float radius, const Ray ray) {
    
    Vec3 oc = ray.Origin() - center;
    float a = Dot(ray.Direction(), ray.Direction());
    float b = 2.0 * Dot(oc, ray.Direction());
    float c = Dot(oc, oc) - radius * radius;
    float discriminant = b*b - 4 *a*c;
    
    return discriminant>0;
    
}

Vec3 color(const Ray &ray) {
    
    if(hitSphere(Vec3(0,0,-1), 0.5, ray)) {
        return Vec3(1.0, 0.0, 0.0);
    }
    
    Vec3 direction = ray.Direction().Normalized();
    float t = 0.5f * (direction.y_ + 1.0f);
    return (1.0-t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    
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
        
        // Spaces
        float ratio = float(SCREEN_WIDTH) / SCREEN_HEIGHT;
        Vec3 origin = Vec3(0,0,0);
        Vec3 lowerLeft = Vec3(-ratio, -1, -1);
        Vec3 horizontal = Vec3(ratio * 2, 0, 0);
        Vec3 vertical = Vec3(0, 2, 0);
        
        // Fill screen
        for( int y = 0; y < SCREEN_HEIGHT; ++y ) {
            for( int x = 0; x < SCREEN_WIDTH; ++x ) {
                
                float u = (float)x / SCREEN_WIDTH;
                float v = (float)y / SCREEN_HEIGHT;
                
                Ray ray = Ray(origin, lowerLeft + (u*horizontal) + (v*vertical));
                Vec3 col = color(ray);
                Uint32 pixel = SDL_MapRGBA( mappingFormat, col.x_ * 255, col.y_ * 255, col.z_ * 255, 255);
                
                int i = x + SCREEN_WIDTH * y;
                pixels[i] = pixel;
            }
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
    
    Vec3 v1 = Vec3(5, 5, 5);
    Vec3 v2 = Vec3(10, 10, 10);
    
    LOG_VEC3("v1", v1);
    LOG_VEC3("v2", v2);
    
    v1 = v1 * 7.0f;
    Vec3 n = v1.Normalized();
    LOG_VEC3("v1", v1);
    LOG_VEC3("n", n);
    
    //Start up SDL and create window
    if( !init() ) {
        LOG_INFO("Failed to initialize!");
    } else {
        //Load media
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

