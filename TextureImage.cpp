//
//  texture.cpp
//  Raytracer
//
//  Created by Harry on 18/01/2018.
//  Copyright © 2018 Harry. All rights reserved.
//

#include "TextureImage.hpp"

TextureImage::TextureImage()
{
    //Initialize
    mTexture = NULL;
    width_ = 0;
    height_ = 0;
}

TextureImage::~TextureImage()
{
    //Deallocate
    free();
}

bool TextureImage::createBlank( SDL_Renderer* gRenderer, SDL_Window* gWindow, int width, int height )
{
    //Get rid of preexisting texture
    free();
    
    //The final texture
    SDL_Texture* newTexture = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    
    if( loadedSurface == NULL ) {
        printf( "Unable to create blank image! SDL_image Error\n");
    } else {
        //Convert surface to display format
        SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_GetWindowPixelFormat( gWindow ), NULL );
        if( formattedSurface == NULL )
        {
            printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Create blank streamable texture
            newTexture = SDL_CreateTexture( gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
            if( newTexture == NULL )
            {
                printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Lock texture for manipulation
                SDL_LockTexture( newTexture, NULL, &pixels_, &pitch_ );
                
                //Copy loaded/formatted surface pixels
                memcpy( pixels_, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );
                
                //Unlock texture to update
                SDL_UnlockTexture( newTexture );
                pixels_ = NULL;
                
                //Get image dimensions
                width_ = formattedSurface->w;
                height_ = formattedSurface->h;
            }
            
            //Get rid of old formatted surface
            SDL_FreeSurface( formattedSurface );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void TextureImage::free() {
    //Free texture if it exists
    if( mTexture != NULL ) {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        width_ = 0;
        height_ = 0;
    }
}

void TextureImage::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void TextureImage::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, width_, height_ };
    
    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    //Render to screen
    SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int TextureImage::getWidth()
{
    return width_;
}

int TextureImage::getHeight()
{
    return height_;
}

void* TextureImage::getPixels()
{
    return pixels_;
}

int TextureImage::getPitch()
{
    return pitch_;
}

bool TextureImage::lockTexture()
{
    bool success = true;
    
    //Texture is already locked
    if( pixels_ != NULL )
    {
        printf( "Texture is already locked!\n" );
        success = false;
    }
    //Lock texture
    else
    {
        if( SDL_LockTexture( mTexture, NULL, &pixels_, &pitch_ ) != 0 )
        {
            printf( "Unable to lock texture! %s\n", SDL_GetError() );
            success = false;
        }
    }
    
    return success;
}

bool TextureImage::unlockTexture()
{
    bool success = true;
    
    //Texture is not locked
    if( pixels_ == NULL )
    {
        printf( "Texture is not locked!\n" );
        success = false;
    }
    //Unlock texture
    else
    {
        SDL_UnlockTexture( mTexture );
        pixels_ = NULL;
        pitch_ = 0;
    }
    
    return success;
}


