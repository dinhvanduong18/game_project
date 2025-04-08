#include "gamemechanic.h"

void restartGame(SDL_Renderer* renderer, SDL_Texture* background, vector<Tile>& tiles, Mix_Chunk* mgTile, Mix_Chunk* gameOver, Mix_Chunk* youWin) {
    cleanUpTiles(tiles);
    spawnTile(tiles, renderer);
    spawnTile(tiles, renderer);
    render(renderer, background, tiles);
    Mix_ResumeMusic();
    gameLoop(tiles, background, renderer, mgTile, gameOver, youWin);
}

void Endgame(SDL_Renderer* renderer, SDL_Texture* background, vector<Tile>& tiles, bool isWin, Mix_Chunk* mgTile, Mix_Chunk* gameOver, Mix_Chunk* youWin) {
    Mix_PauseMusic();
    SDL_Rect endRect = {77, 100, 600, 300};
    SDL_Texture* endTexture = loadTexture(isWin ? "image/youwin1.png" : "image/gameover1.png", renderer);
    SDL_Rect playRect = {102, 500, 250, 100};
    SDL_Texture* playTexture = loadTexture("image/play.png", renderer);
    SDL_Rect exitRect = {402, 500, 250, 100};
    SDL_Texture* exitTexture = loadTexture("image/exit.png", renderer);
    SDL_RenderClear(renderer);
    SDL_SetTextureAlphaMod(background, 30);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    for (auto& tile : tiles) {
        SDL_SetTextureAlphaMod(tile.texture, 30);
        SDL_RenderCopy(renderer, tile.texture, NULL, &tile.rect);
    }
    SDL_RenderCopy(renderer, endTexture, NULL, &endRect);
    SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
    SDL_RenderPresent(renderer);
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                exit(0);
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int x = e.button.x;
            int y = e.button.y;
            if (x >= playRect.x && x <= playRect.x + playRect.w && y >= playRect.y && y <= playRect.y + playRect.h) {
                restartGame(renderer, background, tiles, mgTile, gameOver, youWin);
            }
            if (x >= exitRect.x && x <= exitRect.x + exitRect.w && y >= exitRect.y && y <= exitRect.y + exitRect.h) {
                exit(0);
            }
        }
        SDL_Delay(100);
    }
    SDL_DestroyTexture(endTexture);
    SDL_DestroyTexture(playTexture);
    SDL_DestroyTexture(exitTexture);
}

void gameLoop(vector<Tile>& tiles,SDL_Texture* background, SDL_Renderer* renderer, Mix_Chunk* mgTile, Mix_Chunk* gameOver, Mix_Chunk* youWin) {
    SDL_Event e;
    bool running = true;
    bool moved = false;
    bool allTilesStopped = true;
    bool isWin = false;
    bool isEnd = false;
    int vx = 0, vy = 0;
    int dx = 0, dy = 0;
    vector<int> moveDistances;
    while (running) {
        bool hasMoved = false;
        bool canMerge = false;
        isEnd = false;
        if (allTilesStopped) {
            SDL_WaitEvent(&e);
            if (e.type == SDL_QUIT) {
                running = false;
                isEnd = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
                handleEvent(tiles, moveDistances, dx, dy, vx, vy, e);
                for (int i = 0; i < (int)moveDistances.size(); i++) {
                    if (moveDistances[i] > 0) {
                        hasMoved = true;
                        break;
                    }
                }
                if (!hasMoved) {
                    canMerge = checkMergePossible(tiles, vx, vy);
                    if (canMerge) {
                        mergeTiles(tiles, vx, vy, renderer, mgTile);
                        spawnTile(tiles, renderer);
                    }
                }
            }
        }
        allTilesStopped = true;
        for (int i = 0; i < (int)tiles.size(); i++) {
            if (moveDistances.size() > 0 && moveDistances[i] > 0 && tiles[i].moving) {
                if (moveDistances[i] < SPEED) {
                    if (vx > 0) tiles[i].rect.x += moveDistances[i];
                    else if (vx < 0) tiles[i].rect.x -= moveDistances[i];
                    else if (vy > 0) tiles[i].rect.y += moveDistances[i];
                    else if (vy < 0) tiles[i].rect.y -= moveDistances[i];
                }
                else {
                    tiles[i].rect.x += vx;
                    tiles[i].rect.y += vy;
                }
                moveDistances[i] -= SPEED;
                hasMoved = true;
                allTilesStopped = false;
            }
        }
        if (!hasMoved && moved && (int)tiles.size() < 16) {
            if (checkMergePossible(tiles, vx, vy)) {
                mergeTiles(tiles, vx, vy, renderer, mgTile);
            }
            spawnTile(tiles, renderer);
        }
        moved = hasMoved;
        for (int i = 0; i < (int)tiles.size(); i++) {
            if (tiles[i].value == 32) {
                play(youWin);
                running = false;
                isWin = true;
                break;
            }
        }
        if (!checkMergePossible(tiles, vx, vy) && !checkMergePossible(tiles, vy, vx) && (int)tiles.size() == 16) {
            play(gameOver);
            running = false;
            isWin = false;
            break;
        }
        render(renderer, background, tiles);
        SDL_Delay(16);
    }
    if (!isEnd) {
        for (int i = 500; i >= 0; i -= 5) {
            render(renderer, background, tiles);
            SDL_Delay(16);
        }
        Endgame(renderer, background, tiles, isWin, mgTile, gameOver, youWin);
    }
}

