#include "headers/tilemap.h"
#include "headers/player.h"
#include "headers/input.h"
#include "headers/weapon.h"
#include "headers/ui.h"
#include "headers/camera.h"
#include <SDL2/SDL.h>

SDL_Texture *singleTexture;

int main(int argc, char *argv[]) {
    srand(0);
    srand(rand());
    SDL_Init(SDL_INIT_EVERYTHING);
    Graphics *graphicsInstance = Graphics::GetInstance();
    UI *uiInstance = UI::GetInstance();
    uiInstance->LoadFont(graphicsInstance);
    uiInstance->SetState(STATE_ALIVE);

    graphicsInstance->SetTargetFPS(60);
    graphicsInstance->SetTitle("Game", "../../res/icon.bmp");

    singleTexture = graphicsInstance->LoadImage("../../res/0x72_DungeonTilesetII_v1.4/0x72_DungeonTilesetII_v1.4.png");
    Player player(0, 0);
    Tilemap floor(singleTexture);
    floor.AddTile(16, 64);
    floor.AddTile(32, 64);
    floor.AddTile(48, 64);
    floor.AddTile(16, 80);
    floor.AddTile(32, 80);
    floor.AddTile(48, 80);
    floor.AddTile(16, 96);
    floor.AddTile(32, 96);
    Vec2f mouse;

    SDL_Event e;

    std::map<SDL_Scancode, bool> key_held;

    int tsize = 16;
    SDL_Rect tile = {
        0, 0,
        tsize,
        tsize
    };
    SDL_Rect maprect = {
        0, 0,
        tsize * 70,
        tsize * 32
    };
    SDL_FRect dmaprect = {
        0, 0,
        float(tsize * 3 * 70),
        float(tsize * 3 * 32)
    };

    uint32_t *indices = new uint32_t [maprect.w / tsize * maprect.h / tsize];
    SDL_Texture *map = graphicsInstance->CreateTexture(maprect.w, maprect.h);
    for (int x = 0; x < maprect.w / tsize; x ++) {
        for (int y = 0; y < maprect.h / tsize; y ++) {
            indices[x * maprect.h / tsize + y] = rand() & 7;
        }
    }
    graphicsInstance->BindTexture(map);
    for (int x = 0; x < maprect.w / tsize; x ++) {
        tile.x = tsize * x;
        for (int y = 0; y < maprect.h / tsize; y ++) {
            tile.y = tsize * y;
            floor.Draw(graphicsInstance, indices[x * maprect.h / tsize + y], tile);
        }
    }
    graphicsInstance->BindTexture(NULL);
    delete []indices;

    Vec2f ini, pcur, offset(0, 0);
    ini = player.GetCenter();

    Input *inputInstance = Input::GetInstance();
    inputInstance->BindActionToKey(SDL_SCANCODE_W, std::bind(&Player::MoveUp   , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_A, std::bind(&Player::MoveLeft , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_S, std::bind(&Player::MoveDown , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_D, std::bind(&Player::MoveRight, &player), true);

    Camera cam(&player, {0, 0, 1280, 720});

    while(1) {
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return 0;
            } else if (e.type == SDL_KEYDOWN) {
                inputInstance->KeyDown(e.key.keysym.scancode);
            } else if (e.type == SDL_KEYUP) {
                inputInstance->KeyUp(e.key.keysym.scancode);
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                inputInstance->BtnDown(e.button.button, e.button.x, e.button.y);
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                inputInstance->BtnUp(e.button.button);
            }
        }
        inputInstance->Handle();

        if(uiInstance->GetCurrentState() == STATE_ALIVE){
            mouse = cam.GetCursorPosition();
            player.FaceTowards(mouse);
            player.Update(graphicsInstance->GetDeltaTime());
            cam.Update();
            offset = cam.GetOffset();
        }

        SDL_FRect dmaprect = {
            -offset.x, -offset.y,
            float(tsize * 3 * 70),
            float(tsize * 3 * 32)
        };

        graphicsInstance->Clear();
        graphicsInstance->DrawTexture(map, maprect, dmaprect);
        if(uiInstance->GetCurrentState() == STATE_ALIVE)
            cam.Render(player);
        uiInstance->Draw(graphicsInstance, player);

        graphicsInstance->Update();
    }

    return 0;
}
