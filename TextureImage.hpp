//
//  texture.hpp
//  Raytracer
//
//  Created by Harry on 18/01/2018.
//  Copyright © 2018 Harry. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

// System
#include <iostream>
#include <string>
#include <stdio.h>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#endif /* texture_hpp */

//Texture wrapper class
class TextureImage
{
public:
    //Initializes variables
    TextureImage();
    
    //Deallocates memory
    ~TextureImage();
    
    //Loads image at specified path
    bool loadFromFile(SDL_Renderer* gRenderer, SDL_Window* gWindow, std::string path );
    
    // Creates a blank texture
    bool createBlank( SDL_Renderer* gRenderer, SDL_Window* gWindow, int width, int height);
    
    //Deallocates texture
    void free();
    
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    
    //Renders texture at given point
    void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL );
    
    //Gets image dimensions
    int getWidth();
    int getHeight();
    
    //Pixel manipulators
    bool lockTexture();
    bool unlockTexture();
    void* getPixels();
    int getPitch();
    
private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    
    //Image dimensions
    int width_;
    int height_;
    
    //Pixels
    void* pixels_;
    int pitch_;
};
