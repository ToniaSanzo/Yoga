#pragma once
/*
* authors: Wyatt Matt & Tonia Sanzo 
* date: 10/23/2020
* file: Yoga LTexture.h
*/
#include <SDL.h>
#include <SDL_image.h>
#include <string>

// Texture wrapper class
class LTexture {
public:
    // Initialize variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(int xpos, int ypos, SDL_Rect *clip = NULL);

    // Initialize renderer
    void initRenderer(SDL_Renderer *rend);

    // Get image dimensions
    int getWidth();
    int getHeight();
    
    // Images scale, and pos
    int scale;

private:
    // The actual hardware texture
    SDL_Texture *mTexture;
    SDL_Renderer *mRenderer;

    // Image dimensions
    int mWidth;
    int mHeight;
};

