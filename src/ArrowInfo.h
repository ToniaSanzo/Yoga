#pragma once
/*
* authors: Wyatt Matt & Tonia Sanzo
* date: 10/24/2020
* file: Yoga ArrowInfo.h
*/
#include "SDL.h"

class ArrowInfo {
public:
    // Default constructor, sets the default values for the arrow
    ArrowInfo();
    // Constructor, that specifies the direction value for the arrow, and xpos
    ArrowInfo(int d, int x);

    // Destructor
    ~ArrowInfo();

    // Update the ArrowInfo
    void update();

    int state, direction, xpos;     // information about the arrow
    float speed;                    // speed the arrow travels
    bool inGoal;                    // whether the arrow is in the goal
    bool out;                       // If the arrow is out of range
};