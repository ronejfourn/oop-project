#include "headers/player.h"
#include "headers/input.h"
#include "headers/weapon.h"
#include "headers/ui.h"
#include "headers/camera.h"
#include <vector>
#include <SDL2/SDL.h>

SDL_Texture *singleTexture;

const uint32_t FPS = 60;
const float fixdt  = 1000.0f / FPS;

int main(int argc, char *argv[]) {
    srand(0);
    srand(rand());
    SDL_Init(SDL_INIT_EVERYTHING);
    Graphics *graphicsInstance = Graphics::GetInstance();
    UI *uiInstance = UI::GetInstance();
    uiInstance->LoadFont(graphicsInstance);
    uiInstance->SetState(STATE_MENU);

    graphicsInstance->SetTargetFPS(FPS);
    graphicsInstance->SetTitle("Game", "../../res/icon.bmp");

    singleTexture = graphicsInstance->LoadImage("../../res/0x72_DungeonTilesetII_v1.4/0x72_DungeonTilesetII_v1.4.png");
    Player player(0, 0);
    std::vector<Vec2i> tilemap;
    tilemap.push_back({16, 64});
    tilemap.push_back({32, 64});
    tilemap.push_back({48, 64});
    tilemap.push_back({16, 80});
    tilemap.push_back({32, 80});
    tilemap.push_back({48, 80});
    tilemap.push_back({16, 96});
    tilemap.push_back({32, 96});
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

    uint32_t *indices = new uint32_t [maprect.w / tsize * maprect.h / tsize];
    SDL_Texture *map = graphicsInstance->CreateTexture(maprect.w, maprect.h);
    for (int x = 0; x < maprect.w / tsize; x ++) {
        for (int y = 0; y < maprect.h / tsize; y ++) {
            indices[x * maprect.h / tsize + y] = rand() & 7;
        }
    }
    graphicsInstance->BindTexture(map);
    SDL_Rect tilesrc = {
        0, 0,
        tsize, tsize
    };
    for (int x = 0; x < maprect.w / tsize; x ++) {
        tile.x = tsize * x;
        for (int y = 0; y < maprect.h / tsize; y ++) {
            tile.y = tsize * y;
            tilesrc.x = tilemap[indices[x * maprect.h / tsize + y]].x;
            tilesrc.y = tilemap[indices[x * maprect.h / tsize + y]].y;
            graphicsInstance->DrawTexture(singleTexture, tilesrc, tile);
        }
    }
    graphicsInstance->BindTexture(NULL);
    delete []indices;

    Vec2f offset;

    Input *inputInstance = Input::GetInstance();
    inputInstance->BindActionToKey(SDL_SCANCODE_W, std::bind(&Player::MoveUp   , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_A, std::bind(&Player::MoveLeft , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_S, std::bind(&Player::MoveDown , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_D, std::bind(&Player::MoveRight, &player), true);
    inputInstance->BindActionToBtn(0, std::bind(&Player::Attack, &player), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_DOWN, std::bind(&UI::ChangeOption, uiInstance, false), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_UP  , std::bind(&UI::ChangeOption, uiInstance, true ), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_RETURN, std::bind(&UI::ChooseOption, uiInstance), false);

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
            player.Update(graphicsInstance->GetDeltaTime());
            cam.Update();
            mouse = cam.GetCursorPosition();
            player.FaceTowards(mouse);
            offset = cam.GetOffset();
        }

        SDL_Rect dmaprect = {
            static_cast<int>(-offset.x),
            static_cast<int>(-offset.y),
            maprect.w * 2,
            maprect.h * 2
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
