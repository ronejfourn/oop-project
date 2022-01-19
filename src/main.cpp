#include "tilemap.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

SDL_Texture *singleTexture;

int main() {
    srand(0);
    srand(rand());
    SDL_Init(SDL_INIT_EVERYTHING);
    Graphics context;

    context.SetTargetFPS(30);
    context.SetTitle("Game", "../res/icon.bmp");

    singleTexture = context.LoadImage("../res/0x72_DungeonTilesetII_v1.4/0x72_DungeonTilesetII_v1.4.png");
    Player player;
    Tilemap floor(singleTexture);
    floor.AddTile(16, 64);
    floor.AddTile(32, 64);
    floor.AddTile(48, 64);
    floor.AddTile(16, 80);
    floor.AddTile(32, 80);
    floor.AddTile(48, 80);
    floor.AddTile(16, 96);
    floor.AddTile(32, 96);
    SDL_Point mouse;
    SDL_Rect tile = {
        .x = 0,
        .y = 0,
        .w = 48,
        .h = 48
    };

    SDL_Event e;

    std::map<SDL_Scancode, bool> key_held;

    uint32_t indices [1280 / 48 * 720 / 48];
    for (int x = 0; x < 1280 / 48; x ++) {
        for (int y = 0; y < 720 / 48; y ++) {
            indices[x * 720 / 48 + y] = rand() & 7;
        }
    }

    while(1) {
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return 0;
            } else if (e.type == SDL_KEYDOWN) {
                key_held[e.key.keysym.scancode] = true;
            } else if (e.type == SDL_KEYUP) {
                key_held[e.key.keysym.scancode] = false;
            }
        }

        if (key_held[SDL_SCANCODE_W]) {
            player.MoveUp();
        }
        if (key_held[SDL_SCANCODE_A]) {
            player.MoveLeft();
        }
        if (key_held[SDL_SCANCODE_S]) {
            player.MoveDown();
        }
        if (key_held[SDL_SCANCODE_D]) {
            player.MoveRight();
        }

        mouse = context.GetCursorPosition();

        context.Clear();
        for (int x = 0; x < 1280 / 48; x ++) {
            tile.x = 48 * x;
            for (int y = 0; y < 720 / 48; y ++) {
                tile.y = 48 * y;
                floor.Draw(context, indices[x * 720 / 48 + y], tile);
            }
        }
        player.FaceTowards(mouse.x, mouse.y);
        player.Update(context.GetDeltaTime());
        player.Draw(context);
        context.Update();
    }

    return 0;
}
