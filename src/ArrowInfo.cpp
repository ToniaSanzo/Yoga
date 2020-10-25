/*
* authors: Wyatt Matt & Tonia Sanzo
* date: 10/24/2020
* file: Yoga ArrowInfo.cpp
*/
#include "ArrowInfo.h"

ArrowInfo::ArrowInfo() {
    state = 0;
    direction = 0;
    xpos = -128;
    speed = 3.f;
    inGoal = out = false;
}

ArrowInfo::ArrowInfo(int d, int x) {
    state = 0;
    direction = d;
    xpos = x;
    speed = 3.f;
    inGoal = out = false;
}

// Destructor
ArrowInfo::~ArrowInfo() {}

// Update the ArrowInfo
void ArrowInfo::update() {
    xpos += speed;

    // If the arrow is in range update inGoal value
    if (!inGoal && (xpos > 721 && xpos < 913)) {
        SDL_Event e;
        e.type = SDL_KEYDOWN;
        
        switch (direction) {
        // Push left to the event queue
        case 0:
            e.key.keysym.sym = SDLK_1;
            SDL_PushEvent(&e);
            break;
        // Push up to the event queue
        case 1:
            e.key.keysym.sym = SDLK_2;
            SDL_PushEvent(&e);
            break;
        // Push right to the event queue
        case 2:
            e.key.keysym.sym = SDLK_3;
            SDL_PushEvent(&e);
            break;
            // down
        case 3:
            e.key.keysym.sym = SDLK_4;
            SDL_PushEvent(&e);
            break;
        default:;   /* null */
        }
        
        inGoal = true;
        return;
    }
    
    // If the arrow moves out of goal
    if (inGoal && xpos > 913) {
        inGoal = false;
        if (state == 0) {
            state = 2;
        }
        return;
    }

    // mark the node as out if the node moves off the screen
    if (xpos > 1260) {
        out = true;
    }
}
