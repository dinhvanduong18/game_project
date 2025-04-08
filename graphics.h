#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "defs.h"
#include "logic.h"

void logErrorAndExit(const char* msg, const char* error);
SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT);
SDL_Renderer* createRenderer(SDL_Window* window);
SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, Mix_Music* bgMusic, Mix_Chunk* mgTile, Mix_Chunk* gameOver, Mix_Chunk* youWin);

Mix_Music *loadMusic(const char* path);
void play(Mix_Music *gMusic);
Mix_Chunk* loadSound(const char* path);
void play(Mix_Chunk* gChunk);

void renderBackground(SDL_Renderer* renderer, SDL_Texture* background);
void renderTiles(SDL_Renderer* renderer, vector<Tile>& tiles);
void render(SDL_Renderer* renderer, SDL_Texture* background, vector<Tile>& tiles);

#endif // GRAPHICS_H
