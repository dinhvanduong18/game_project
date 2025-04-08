#ifndef DEFS_H
#define DEFS_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <algorithm>

using namespace std;

struct Tile {
    SDL_Rect rect;
    SDL_Texture* texture;
    int value;
    bool moving;
};

const int SCREEN_WIDTH = 754;
const int SCREEN_HEIGHT = 754;
#define WINDOW_TITLE "Game 2048"
const int SPEED = 80;
const int TILE_SIZE = 161;


#endif // DEFS_H
