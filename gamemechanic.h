#ifndef GAMEMECHANIC_H
#define GAMEMECHANIC_H

#include "graphics.h"
#include "logic.h"

void restartGame(SDL_Renderer* renderer, SDL_Texture* background, vector<Tile>& tiles, Mix_Chunk* mgTile, Mix_Chunk* gameOver, Mix_Chunk* youWin);
void Endgame(SDL_Renderer* renderer, SDL_Texture* background, vector<Tile>& tiles, bool isWin, Mix_Chunk* mgTile, Mix_Chunk* gameOver, Mix_Chunk* youWin);
void gameLoop(vector<Tile>& tiles,SDL_Texture* background, SDL_Renderer* renderer, Mix_Chunk* mgTile, Mix_Chunk* gameOver, Mix_Chunk* youWin);

#endif // GAMEMECHANIC_H
