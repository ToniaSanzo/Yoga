/*
* authors: Wyatt Matt & Tonia Sanzo
* date: 10/23/2020
* file: Yoga LTexture.cpp
*/
#include "LTexture.h"

// LTexture constructor
LTexture::LTexture() {
    // Initialize
    mRenderer = NULL;
    mTexture = NULL;
    mWidth = mHeight = 0;
    scale = 1;
}

// LTexture destructor
LTexture::~LTexture() {
    // Deallocate texture
    free();
    
    // Free renderer if necessary
    if (mRenderer) {
        mRenderer = NULL;
    }
}

// Load LTexture from a file
bool LTexture::loadFromFile(std::string path) {
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        printf("unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
        if (!newTexture) {
            printf("Unable to create texture from %s! SDL Error: %s\n:", path.c_str(), SDL_GetError());
        }
        else {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

// Deallocate the texture
void LTexture::free() {
    // Free texture if it exists
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}


// Define whether this texture can blend
void LTexture::setBlendMode(SDL_BlendMode blending) {
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

// Define the texture's opacity
void LTexture::setAlpha(Uint8 alpha) {
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

// Modulate the texture's color
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    // Modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

// Render texture
void LTexture::render(int xpos, int ypos, SDL_Rect *clip) {
    // Set Rendering space and render to screen
    SDL_Rect renderQuad = { xpos, ypos, mWidth * scale, mHeight * scale };

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w * scale;
        renderQuad.h = clip->h * scale;
    }

    // Render to screen
    SDL_RenderCopy(mRenderer, mTexture, clip, &renderQuad);
}

void LTexture::initRenderer(SDL_Renderer *rend)
{
    mRenderer = rend;
}
