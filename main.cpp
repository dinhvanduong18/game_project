#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "graphics.h"

using namespace std;

void clearEventQueue() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {

    }
}

int main(int argc, char *argv[]) {
    srand(time(0));
    SDL_Window* window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer* renderer = createRenderer(window);
    Mix_Music* bgMusic = loadMusic("music/backgroundmusic.mp3");
    play(bgMusic);
    Mix_Chunk* mgTile = loadSound("music/mergeTile.wav");
    Mix_Chunk* gameOver = loadSound("music/gameOver.wav");
    Mix_Chunk* youWin = loadSound("music/win.wav");
    SDL_Texture* background = loadTexture("image/game2048.jpg", renderer);
    vector<Tile> tiles;
    spawnTile(tiles, renderer);
    spawnTile(tiles, renderer);
    clearEventQueue();
    render(renderer, background, tiles);
    gameLoop(tiles, background, renderer, mgTile, gameOver, youWin);
    cleanUpTiles(tiles);
    SDL_DestroyTexture(background);
    background = NULL;
    quitSDL(window, renderer, bgMusic, mgTile, gameOver, youWin);
    return 0;
}
