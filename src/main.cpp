#include "tilemap.h"
#include "player.h"
#include <SDL2/SDL.h>

SDL_Texture *singleTexture;

int main() {
    srand(0);
    srand(rand());
    SDL_Init(SDL_INIT_EVERYTHING);
    Graphics context;

    context.SetTargetFPS(60);
    context.SetTitle("Game", "../res/icon.bmp");

    singleTexture = context.LoadImage("../res/0x72_DungeonTilesetII_v1.4/0x72_DungeonTilesetII_v1.4.png");
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
    SDL_Rect maprect = {
        .x = 0,
        .y = 0,
        .w = 1280,
        .h = 720
    };

    SDL_Event e;

    std::map<SDL_Scancode, bool> key_held;

    int tsize = 48;
    SDL_Rect tile = {
        .x = 0,
        .y = 0,
        .w = tsize,
        .h = tsize
    };
    SDL_Texture *map = context.CreateTexture(1280, 720);
    uint32_t indices [1280 / tsize * 720 / tsize];
    for (int x = 0; x < 1280 / tsize; x ++) {
        for (int y = 0; y < 720 / tsize; y ++) {
            indices[x * 720 / tsize + y] = rand() & 7;
        }
    }
    context.BindTexture(map);
    for (int x = 0; x < 1280 / tsize; x ++) {
        tile.x = tsize * x;
        for (int y = 0; y < 720 / tsize; y ++) {
            tile.y = tsize * y;
            floor.Draw(context, indices[x * 720 / tsize + y], tile);
        }
    }
    context.BindTexture(NULL);

    SDL_Point ini = player.GetCenter();

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

        mouse = context.GetCursorPosition(true);
        player.FaceTowards(mouse.x, mouse.y);
        player.Update(context.GetDeltaTime());
        SDL_Point pcur = player.GetCenter();
        context.SetOffset(pcur.x - ini.x, pcur.y - ini.y);

        context.Clear();
        context.DrawTexture(map, maprect, maprect);
        player.Draw(context);
        context.Update();
    }

    return 0;
}
