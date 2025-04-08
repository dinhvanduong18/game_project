#ifndef LOGIC_H
#define LOGIC_H

#include "input.h"
#include "graphics.h"
#include "gameMechanic.h"

void cleanUpTiles(vector<Tile>& tiles);
bool isPositionEmpty(int i, int j, const vector<Tile>& temptiles);
void spawnTile(vector<Tile>& tiles, SDL_Renderer* renderer);
void mergeTiles(vector<Tile>& tiles, int& vx, int& vy, SDL_Renderer* renderer, Mix_Chunk* mgTile);
bool checkMergePossible(const vector<Tile>& tiles, const int& vx, const int& vy);
void calculateMoveDistances(int& dx, int& dy, vector<int>& moveDistances, vector<Tile>& tiles);
void handleEvent(vector<Tile>& tiles, vector<int>& moveDistances, int& dx, int& dy, int& vx, int& vy, SDL_Event& e);

#endif // LOGIC_H
