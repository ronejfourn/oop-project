#include "SDL2/SDL_timer.h"
#include "tilemap.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <iostream>

SDL_Texture *singleTexture;

int main(int argc, char *argv[]) {
    srand(0);
    srand(rand());
    SDL_Init(SDL_INIT_EVERYTHING);
    Graphics context;

    context.SetTargetFPS(60);
    context.SetTitle("Game", "../../res/icon.bmp");

    singleTexture = context.LoadImage("../../res/0x72_DungeonTilesetII_v1.4/0x72_DungeonTilesetII_v1.4.png");
    Player player(1280 / 2, 720 / 2);
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

    SDL_Event e;

    std::map<SDL_Scancode, bool> key_held;

    int tsize = 16;
    SDL_Rect tile = {
        0,
        0,
        tsize,
        tsize
    };
    SDL_Rect maprect = {
        0,
        0,
        tsize * 70,
        tsize * 32
    };
    SDL_Rect dmaprect = {
        0,
        0,
        static_cast<int>(tsize * 2.5 * 70),
        static_cast<int>(tsize * 2.5 * 32)
    };
    uint32_t *indices = new uint32_t [maprect.w / tsize * maprect.h / tsize];
    SDL_Texture *map = context.CreateTexture(maprect.w, maprect.h);
    for (int x = 0; x < maprect.w / tsize; x ++) {
        for (int y = 0; y < maprect.h / tsize; y ++) {
            indices[x * maprect.h / tsize + y] = rand() & 7;
        }
    }
    context.BindTexture(map);
    for (int x = 0; x < maprect.w / tsize; x ++) {
        tile.x = tsize * x;
        for (int y = 0; y < maprect.h / tsize; y ++) {
            tile.y = tsize * y;
            floor.Draw(context, indices[x * maprect.h / tsize + y], tile);
        }
    }
    context.BindTexture(NULL);
    delete []indices;

    SDL_Point ini = player.GetCenter();
    int a;

    while(1) {
        a = SDL_GetTicks();
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

        mouse = context.GetCursorPosition(true);
        player.FaceTowards(mouse.x, mouse.y);
        player.Update(context.GetDeltaTime());
        SDL_Point pcur = player.GetCenter();
        context.SetOffset(pcur.x - ini.x, pcur.y - ini.y);

        context.Clear();
        context.DrawTexture(map, maprect, dmaprect);
        player.Draw(context);

        context.Update();
    }

    return 0;
}
