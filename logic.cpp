#include "logic.h"

void cleanUpTiles(vector<Tile>& tiles) {
    for (auto& tile : tiles) {
        SDL_DestroyTexture(tile.texture);
    }
    tiles.clear();
}

bool isPositionEmpty(int i, int j, const vector<Tile>& tiles) {
    for (const auto& tile : tiles) {
        if (tile.rect.x == i && tile.rect.y == j) return false;
    }
    return true;
}

void spawnTile(vector<Tile>& tiles, SDL_Renderer* renderer) {
    while (true) {
        int i = (rand() % 4) * 183 + 22;
        int j = (rand() % 4) * 183 + 22;
        if (isPositionEmpty(i, j, tiles)) {
            int randomValue = (rand() % 20 > 15) ? 4 : 2;
            const char* textureFile = (randomValue == 4) ? "image/tile4.png" : "image/tile2.png";
            SDL_Texture* tileTexture = loadTexture(textureFile, renderer);
            tiles.push_back({ {i, j, 161, 161}, tileTexture, randomValue, false });
            return;
        }
    }
}

bool checkMergePossible(const vector<Tile>& tiles, const int& vx, const int& vy) {
    vector<vector<int>> arr(4, vector<int>(4, 0));
    for (int i = 0; i < (int)tiles.size(); i++) {
        int row = (tiles[i].rect.y - 22) / 183;
        int col = (tiles[i].rect.x - 22) / 183;
        if (tiles[i].value > 0) {
            arr[row][col] = tiles[i].value;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (vx != 0) {
                if (arr[i][j] == arr[i][j + 1] && arr[i][j] != 0) {
                    return true;
                }
            }
            if (vy != 0) {
                if (arr[j][i] == arr[j + 1][i] && arr[j][i] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void mergeTiles(vector<Tile>& tiles, int& vx, int& vy, SDL_Renderer* renderer, Mix_Chunk* mgTile) {
    vector<vector<int>> arr(4, vector<int>(4, 0));
    for (int i = 0; i < (int)tiles.size(); i++) {
        int row = (tiles[i].rect.y - 22) / 183;
        int col = (tiles[i].rect.x - 22) / 183;
        if (tiles[i].value > 0) {
            arr[row][col] = tiles[i].value;
        }
    }
    for (int i = 0; i < 4; i++) {
        vector<int> temp;
        for (int j = 0; j < 4; j++) {
            int value = (vx != 0) ? arr[i][j] : arr[j][i];
            if (value) temp.push_back(value);
        }
        if (vx > 0 || vy > 0) reverse(temp.begin(), temp.end());
        for (int j = 0; j < (int)temp.size() - 1; j++) {
            if (temp[j] && temp[j] == temp[j + 1]) {
                temp[j] *= 2;
                temp[j + 1] = 0;
            }
        }
        vector<int> newTemp;
        for (int value : temp) {
            if (value) newTemp.push_back(value);
        }
        while (newTemp.size() < 4) newTemp.push_back(0);
        if (vx > 0 || vy > 0) reverse(newTemp.begin(), newTemp.end());
        for (int j = 0; j < 4; j++) {
            if (vx != 0) arr[i][j] = newTemp[j];
            if (vy != 0) arr[j][i] = newTemp[j];
        }
    }
    play(mgTile);
    cleanUpTiles(tiles);
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (arr[row][col] != 0) {
                string tileImage = getTileImagePath(arr[row][col]);
                SDL_Texture* tileTexture = loadTexture(tileImage.c_str(), renderer);
                tiles.push_back({col * 183 + 22, row * 183 + 22, TILE_SIZE, TILE_SIZE, tileTexture, arr[row][col], false});
            }
        }
    }
}

void calculateMoveDistances(int& dx, int& dy, vector<int>& moveDistances, vector<Tile>& tiles) {
    moveDistances.clear();
    moveDistances.resize(tiles.size(), 0);
    vector<Tile> temptiles = tiles;
    bool moved = true;
    while (moved) {
        moved = false;
        for (int i = 0; i < (int)temptiles.size(); i++) {
            int newX = temptiles[i].rect.x + dx;
            int newY = temptiles[i].rect.y + dy;
            if (newX >= 22 && newX <= 571 && newY >= 22 && newY <= 571 && isPositionEmpty(newX, newY, temptiles)) {
                temptiles[i].rect.x = newX;
                temptiles[i].rect.y = newY;
                moveDistances[i] += 183;
                moved = true;
            }
        }
    }
}

void handleEvent(vector<Tile>& tiles, vector<int>& moveDistances, int& dx, int& dy, int& vx, int& vy, SDL_Event& e) {
    switch (e.key.keysym.sym) {
        case SDLK_a: case SDLK_LEFT:
            dx = -183;
            vx = -SPEED;
            dy = 0;
            vy = 0;
            break;
        case SDLK_d: case SDLK_RIGHT:
            dx = 183;
            vx = SPEED;
            dy = 0;
            vy = 0;
            break;
        case SDLK_w: case SDLK_UP:
            dx = 0;
            vx = 0;
            dy = -183;
            vy = -SPEED;
            break;
        case SDLK_s: case SDLK_DOWN:
            dx = 0;
            vx = 0;
            dy = 183;
            vy = SPEED;
            break;
    }
    for (int i = 0; i < (int)tiles.size(); i++) {
        tiles[i].moving = true;
    }
    if (dx != 0 || dy != 0) calculateMoveDistances(dx, dy, moveDistances, tiles);
}



