//
//  main.cpp
//  Raytracer
//
//  Created by Harry on 17/01/2018.
//  Copyright © 2018 Harry. All rights reserved.
//

// System
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

// Raytracer
#include "TextureImage.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* window_ = NULL;

//The window renderer
SDL_Renderer* renderer_ = NULL;

//Event handler
SDL_Event e;

//Scene texture
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

bool loadMedia() {
    //Loading success flag
    bool success = true;
    
    //Load texture
    if( !screenTexture_.createBlank(renderer_, window_, SCREEN_WIDTH, SCREEN_HEIGHT) ) {
        printf( "Failed to load colors texture!\n" );
        success = false;
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
        
        //Map colors
        Uint32 colorKey = SDL_MapRGB( mappingFormat, 0, 0xFF, 0xFF );
        Uint32 transparent = SDL_MapRGBA( mappingFormat, 0xFF, 0xFF, 0xFF, 0x00 );
        Uint32 red = SDL_MapRGBA( mappingFormat, 255, 0, 0, 255 );
        Uint32 black = SDL_MapRGBA( mappingFormat, 0, 0, 0, 255 );
        
        timer += 1;
        
        //Color key pixels
        for( int y = 0; y < SCREEN_HEIGHT; ++y ) {
            for( int x = 0; x < SCREEN_WIDTH; ++x ) {
                
                int i = x + SCREEN_WIDTH * y;
                int r = x * (255.0f / SCREEN_WIDTH);
                int g = y * (255.0f / SCREEN_HEIGHT);
                int b = (M_PI/2+sin(timer*0.01)/2) * 255;
                
                if(rand() % 20 == 0) {
                    r = 0;
                    g = 0;
                    b = 0;
                }
                
                Uint32 col = SDL_MapRGBA( mappingFormat, r, g, b, 255);
                pixels[i] = col;
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
    
    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    } else {
        //Load media
        if( !loadMedia() ) {
            printf( "Failed to load media!\n" );
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

