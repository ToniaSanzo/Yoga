/*
* authors: Wyatt Matt & Tonia Sanzo 
* date: 10/23/2020
* file: Yoga main.cpp
* 
* Special Shoutout to Young Kartz, Les Cartes Postales Sonores, and Ketsa for the music they
* shared under the Creative Commons license.
*/
#include "LTexture.h"
#include "ArrowInfo.h"
#include <stdio.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
using namespace std;

// The window we will be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// The music
Mix_Music *gIntroMusic = NULL;
Mix_Music *gRollieMusic = NULL;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

// The score of the player
int current_combo = 0;
int max_combo = 0;


// The intro screen sprite sheet and information for animation
const int INTRO_ANIMATION_FRAMES = 6, INTRO_WIDTH = 320, INTRO_HEIGHT = 240;
LTexture gIntroTexture;
SDL_Rect gIntroClips[INTRO_ANIMATION_FRAMES];

// The studio_t sprite sheet and information for animation
const int STUDIO_ANIMATE_FRAMES = 9, STUDIO_T_WIDTH = 160, STUDIO_T_HEIGHT = 120;
LTexture gStudioTTexture;
SDL_Rect gStudioTClips[STUDIO_ANIMATE_FRAMES];

// The character sprite sheet and information for animation
const int CHARACTERS = 3, CHARACTER_ANIMATE_FRAMES = 12, CHARACTER_WIDTH = 64, CHARACTER_HEIGHT = 64;
LTexture gCharacterTexture;
SDL_Rect gCharacterClips[CHARACTER_ANIMATE_FRAMES * CHARACTERS];
int gSenseiIndex = 0, gStudentIndex = 12;

// The arrow textures
const int ARROWS = 4, ARROW_STATES = 3, ARROW_WIDTH = 32, ARROW_HEIGHT = 32;
LTexture gArrowTexture[ARROWS];
SDL_Rect gArrowClips[ARROW_STATES];

// The goal texture
const int GOALS = 2, GOAL_WIDTH = 32, GOAL_HEIGHT = 32;
LTexture gGoalTexture;
SDL_Rect gGoalClips[GOALS];

bool init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // Create window
        gWindow = SDL_CreateWindow("Yoga", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!gWindow) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            // Create renderer for the window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (!gRenderer) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                // Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

// Pushes an event to the SDL_Event Queue to end the play loop
Uint32 my_callbackfunc(Uint32 interval, void *params) {
    SDL_Event e;
    e.type = SDL_KEYDOWN;

    e.key.keysym.sym = SDLK_0;
    SDL_PushEvent(&e);
    return interval;
}

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load intro texture
    gIntroTexture.initRenderer(gRenderer);
    if (!gIntroTexture.loadFromFile("assets/intro.png")) {
        printf("Failed to load intro texture!\n");
        success = false;
    }
    else {
        // Set background texture scale
        gIntroTexture.scale = 4;
        // Set standard alpha blending
        gIntroTexture.setBlendMode(SDL_BLENDMODE_BLEND);
        gIntroTexture.setAlpha(255);

        // Set the background texture rectangles
        for (int i = 0; i < INTRO_ANIMATION_FRAMES; ++i) {
            gIntroClips[i].x = i * INTRO_WIDTH;
            gIntroClips[i].y = 0;
            gIntroClips[i].w = INTRO_WIDTH;
            gIntroClips[i].h = INTRO_HEIGHT;
        }
    }

    // Load arrow textures
    gArrowTexture[0].initRenderer(gRenderer);
    if (!gArrowTexture[0].loadFromFile("assets/left_arrow.png")) {
        printf("Failed to load left arrow texture!\n");
        success = false;
    }

    gArrowTexture[1].initRenderer(gRenderer);
    if (!gArrowTexture[1].loadFromFile("assets/up_arrow.png")) {
        printf("Failed to load up arrow texture!\n");
        success = false;
    }

    gArrowTexture[2].initRenderer(gRenderer);
    if (!gArrowTexture[2].loadFromFile("assets/right_arrow.png")) {
        printf("Failed to load right arrow texture!\n");
        success = false;
    }

    gArrowTexture[3].initRenderer(gRenderer);
    if (!gArrowTexture[3].loadFromFile("assets/down_arrow.png")) {
        printf("Failed to load down arrow texture!\n");
        success = false;
    }
    else {
        // Set the scale for each arrow
        for (int i = 0; i < ARROWS; ++i) { gArrowTexture[i].scale = 2; }

        // Load the arrow rectangles  
        for (int i = 0; i < ARROW_STATES; ++i) {
            gArrowClips[i].x = i * ARROW_WIDTH;
            gArrowClips[i].y = i * 0;
            gArrowClips[i].w = ARROW_WIDTH;
            gArrowClips[i].h = ARROW_HEIGHT;
        }
    }

    // Load goal texture
    gGoalTexture.initRenderer(gRenderer);
    if (!gGoalTexture.loadFromFile("assets/box.png")) {
        printf("Failed to load box texture!\n");
        success = false;
    }
    else {
        // Set the goal texture scale
        gGoalTexture.scale = 4;
    }

    // Load studio t texture
    gStudioTTexture.initRenderer(gRenderer);
    if (!gStudioTTexture.loadFromFile("assets/studio_sheet_t.png")) {
        printf("Failed to load studio t texture!\n");
        success = false;
    }
    else {
        // Set the goal texture scale
        gStudioTTexture.scale = 8;

        // Load the studio t rectangles
        for(int i = 0; i < STUDIO_ANIMATE_FRAMES; ++i){
            gStudioTClips[i].x = i * STUDIO_T_WIDTH;
            gStudioTClips[i].y = 0;
            gStudioTClips[i].w = STUDIO_T_WIDTH;
            gStudioTClips[i].h = STUDIO_T_HEIGHT;
        }
    }

    // Load character texture
    gCharacterTexture.initRenderer(gRenderer);
    if (!gCharacterTexture.loadFromFile("assets/monk_sheet.png")) {
        printf("Failed to load studio t texture!\n");
        success = false;
    }
    else {
        // Set the character texture scale
        gCharacterTexture.scale = 4;

        // Load the character texture
        for (int i = 0; i < CHARACTER_ANIMATE_FRAMES; ++i) {
            gCharacterClips[i].x = i * CHARACTER_WIDTH;
            gCharacterClips[i].y = 0;
            gCharacterClips[i].w = CHARACTER_WIDTH;
            gCharacterClips[i].h = CHARACTER_HEIGHT;

            gCharacterClips[i + CHARACTER_ANIMATE_FRAMES].x = i * CHARACTER_WIDTH;
            gCharacterClips[i + CHARACTER_ANIMATE_FRAMES].y = 64;
            gCharacterClips[i + CHARACTER_ANIMATE_FRAMES].w = CHARACTER_WIDTH;
            gCharacterClips[i + CHARACTER_ANIMATE_FRAMES].h = CHARACTER_HEIGHT;

            gCharacterClips[i + (2 * CHARACTER_ANIMATE_FRAMES)].x = i * CHARACTER_WIDTH;
            gCharacterClips[i + (2 * CHARACTER_ANIMATE_FRAMES)].y = 128;
            gCharacterClips[i + (2 * CHARACTER_ANIMATE_FRAMES)].w = CHARACTER_WIDTH;
            gCharacterClips[i + (2 * CHARACTER_ANIMATE_FRAMES)].h = CHARACTER_HEIGHT;
        }
    }

    // Load music
    gIntroMusic = Mix_LoadMUS("assets/intro.wav");
    if (gIntroMusic == NULL) {
        printf("Failed to load intro music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gRollieMusic = Mix_LoadMUS("assets/rollie.wav");
    if (gRollieMusic == NULL) {
        printf("Failed to load intro music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}

// Fade the intro screen out
void dissolveIntro() {
    for (Uint8 a = 255; a > 0; a -= 15) {
        SDL_Delay(50);
        
        // Clear screen, set alpha and render
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        // Render the studio, and characters inthe background
        gStudioTTexture.render(0, 0, &gStudioTClips[0]);
        gCharacterTexture.render(138, 110, &gCharacterClips[gSenseiIndex]);
        gCharacterTexture.render(765, 580, &gCharacterClips[gStudentIndex]);

        // Render the intro screen
        gIntroTexture.setAlpha(a);
        gIntroTexture.render(0, 0, &gIntroClips[0]);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }
}

void bow() {
    for (Uint8 act = 0; act < 8; ++act) {
        SDL_Delay(180);

        // Clear screen, set alpha and render
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        // Render the studio, and characters inthe background
        gStudioTTexture.render(0, 0, &gStudioTClips[act]);
        gCharacterTexture.render(138, 110, &gCharacterClips[act]);
        gCharacterTexture.render(765, 580, &gCharacterClips[act + 12]);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }
}

// play's the game
bool play() {
    // Collection of arrows
    vector<ArrowInfo> arrows;
    vector<ArrowInfo>::iterator outArrow;

    // State of game, determines when the game ends
    bool over = false;
    
    // Target frames per second, max time we have between delays
    const int FPS = 60; const int frameDelay = 1000 / FPS;

    // Used to make the game clock
    Uint32 delay = 180000;
    SDL_TimerID mytimerId = SDL_AddTimer(delay, my_callbackfunc, NULL);
    Uint32 frameStart;
    int frameTime;

    // Used to generate arrows
    int randCount = 0, direction, arrowCnt;

    // Used to animate the textures
    int backgroundFrame = 0, characterFrame = 0;

    // Event handler
    SDL_Event e;

    // While application is running
    Mix_PlayMusic(gRollieMusic, -1);
    while (!over) {
        frameStart = SDL_GetTicks();

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                return true;
            }
            // On keypress start the game
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                // up arrow
                case SDLK_UP:
                    // Update the student action
                    gStudentIndex = 23;

                    // Check to see if the key matches an arrow in the box
                    for (auto &arrow : arrows) {
                        if (arrow.inGoal) {
                            // ignore already defined variables
                            if (arrow.state == 1 || arrow.state == 2) {
                                continue;
                            }

                            if (arrow.direction == 1) {
                                arrow.state = 1;
                                ++current_combo;
                                if ((current_combo % 5) == 0)
                                    printf("Combo: %d\n", current_combo);
                            }
                            else {
                                arrow.state = 2;
                                if (current_combo > max_combo)
                                    max_combo = current_combo;
                                current_combo = 0;
                                printf("Combo Reset!\n");
                            }
                            break;
                        }
                    }
                    break;
                // down arrow
                case SDLK_DOWN:
                    // Update the student action
                    gStudentIndex = 22;

                    // Check to see if the key matches an arrow in the box
                    for (auto &arrow : arrows) {
                        if (arrow.inGoal) {
                            // ignore already defined variables
                            if (arrow.state == 1 || arrow.state == 2) {
                                continue;
                            }
                            
                            if (arrow.direction == 3) {
                                arrow.state = 1;
                                ++current_combo;
                                if ((current_combo % 5) == 0)
                                    printf("Combo: %d\n", current_combo);
                            }
                            else {
                                arrow.state = 2;
                                if (current_combo > max_combo)
                                    max_combo = current_combo;
                                current_combo = 0;
                                printf("Combo Reset!\n");
                            }
                            break;
                        }
                    }
                    break;
                // left arrow
                case SDLK_LEFT:
                    // Update the student action
                    gStudentIndex = 21;

                    // Check to see if the key matches an arrow in the box
                    for (auto &arrow : arrows) {
                        if (arrow.inGoal) {

                            // ignore already defined variables
                            if (arrow.state == 1 || arrow.state == 2) {
                                continue;
                            }

                            if (arrow.direction == 0) {
                                arrow.state = 1;
                                ++current_combo;
                                if ((current_combo % 5) == 0)
                                    printf("Combo: %d\n", current_combo);
                            }
                            else {
                                arrow.state = 2;
                                if (current_combo > max_combo)
                                    max_combo = current_combo;
                                current_combo = 0;
                                printf("Combo Reset!\n");
                            }
                            break;
                        }
                    }
                    break;
                // right arrow
                case SDLK_RIGHT:
                    // Update the students action
                    gStudentIndex = 20;

                    // Check to see if the key matches an arrow in the box
                    for (auto &arrow : arrows) {
                        if (arrow.inGoal) {
                            
                            // ignore already defined variables
                            if (arrow.state == 1 || arrow.state == 2) {
                                continue;
                            }

                            if (arrow.direction == 2) {
                                arrow.state = 1;
                                ++current_combo;
                                if ((current_combo % 5) == 0)
                                    printf("Combo: %d\n", current_combo);
                            }
                            else {
                                arrow.state = 2;
                                if (current_combo > max_combo)
                                    max_combo = current_combo;
                                current_combo = 0;
                                printf("Combo Reset!\n");
                            }
                            break;
                        }
                    }
                    break;
                // q key
                case SDLK_q:
                    over = true;
                    break;
                // Sensei left event
                case SDLK_1:
                    gSenseiIndex = 9;
                    break;
                // Sensei up event
                case SDLK_2:
                    gSenseiIndex = 11;
                    break;
                // Sensei right event
                case SDLK_3:
                    gSenseiIndex = 8;
                    break;
                // Sensei down event
                case SDLK_4:
                    gSenseiIndex = 10;
                    break;
                // Timer end
                case SDLK_0:
                    over = true;
                    break;
                // Escape key
                case SDLK_ESCAPE:
                    return true;
                default:; /* null */
                }
            }
        }

        // Update game world
        // Randomly generate arrow commands
        ++randCount;
        if (randCount == 110) { 
            // Generate a random number between 0 and 2
            arrowCnt = rand() % 3;

            switch (arrowCnt) {
            // Generate one arrow
            case 0:
                arrows.push_back(ArrowInfo((rand() % 4), -70));
                break;

            // Generate two arrows
            case 1:
                arrows.push_back(ArrowInfo((rand() % 4), -70));
                arrows.push_back(ArrowInfo((rand() % 4), -140));
                break;

            // Generate three arrows
            case 2:
                arrows.push_back(ArrowInfo((rand() % 4), -70));
                arrows.push_back(ArrowInfo((rand() % 4), -140));
                arrows.push_back(ArrowInfo((rand() % 4), -210));
                break;
            default:; /* null */
            }

            randCount = 0;
        }

        // Used to animate the background
        ++backgroundFrame;
        if (backgroundFrame == STUDIO_ANIMATE_FRAMES * 25) {
            backgroundFrame = 0;
        }

        // update the arrows
        outArrow = arrows.end();
        for (auto arrow = arrows.begin(); arrow != arrows.end(); ++arrow) {
            arrow->update();

            // If the arrow has moved off the screen
            if (arrow->out) {
                outArrow = arrow;
            }
        }

        // Remove an arrow if necessary
        if (outArrow != arrows.end()) {
            arrows.erase(outArrow);
        }

        // Clear screen to black
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render the game stage
        gStudioTTexture.render(0, 0, &gStudioTClips[backgroundFrame / 25]);
        
        // Render the character texture
        gCharacterTexture.render(138, 110, &gCharacterClips[gSenseiIndex]);
        gCharacterTexture.render(765, 580, &gCharacterClips[gStudentIndex]);

        // Render every arrow
        for (auto arrow : arrows) {
            gArrowTexture[arrow.direction].render(arrow.xpos, 460, &gArrowClips[arrow.state]);
        }
        // Render the goal
        gGoalTexture.render(785, 420);

        // Update screen
        SDL_RenderPresent(gRenderer);

        frameTime = SDL_GetTicks() - frameStart;

        // This keeps us from displaying more frames than 60
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    Mix_HaltMusic();
    bow();
    return false;
}


void close() {
    // Free loaded images
    printf("Setting the texture's free: \"Freedom is the power to choose our own chains\" - Jean Jacques Rousseau\n\n");
    gIntroTexture.free();
    gGoalTexture.free();
    for (LTexture &arrow : gArrowTexture) {
        arrow.free();
    }
    gCharacterTexture.free();
    gStudioTTexture.free();

    // Free the music
    printf("Cleaning up the music: \"Music gives a soul to the universe wings to the mind, flight to the imagination\n and life to everything.\" - Plato\n\n");
    Mix_FreeMusic(gIntroMusic);
    Mix_FreeMusic(gRollieMusic);
    gIntroMusic = NULL;
    gRollieMusic = NULL;

    // Destroy window and renderer
    printf("Destroying the window and renderer: \"Death is nothing else but going home to God, the bond of love will be unbroken\n for all eternity.\" - Mother Teresa\n\n");
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);

    // Print current and max combo
    if (max_combo < current_combo)
        max_combo = current_combo;
    printf("Combo: %d, Max Combo: %d", current_combo, max_combo);

    // Quit SDL subsystems
    printf("Quiting the SDL subsystems\n\n");
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *args[]) {
    // Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        // Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        }
        else {
            // Main loop flag
            bool quit = false, start = false;

            // Event handler
            SDL_Event e;

            // Animation frame variables
            int introFrame = 0;

            // While application is running
            while (!quit) {

                // Intro screen
                Mix_PlayMusic(gIntroMusic, -1);
                while (!start) {
                    // Handle events on queue
                    while (SDL_PollEvent(&e) != 0) {
                        // User requests quit
                        if (e.type == SDL_QUIT) {
                            quit = true;
                            start = true;
                        }
                        // On keypress start the game
                        else if (e.type == SDL_KEYDOWN) {
                            if (e.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                                start = true;
                            }
                            else if (e.key.keysym.sym != 0) {
                                start = true;
                            }
                        }
                    }

                    // Clear screen
                    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
                    SDL_RenderClear(gRenderer);

                    // Render intro screen
                    gIntroTexture.render(0, 0, &gIntroClips[introFrame / 3000]);

                    // Update screen
                    SDL_RenderPresent(gRenderer);

                    ++introFrame;
                    if (introFrame == INTRO_ANIMATION_FRAMES * 3000) {
                        introFrame = 0;
                    }
                }

                if (!quit) {
                    start = false;
                    Mix_HaltMusic();
                    dissolveIntro();
                    bow();
                    gIntroTexture.setAlpha(255);
                    quit = play();
                }
            }
        }
    }

    // Free resources and close SDL
    close();
    system("pause");
    return 0;
}
