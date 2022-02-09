#include "headers/player.h"
#include "headers/input.h"
#include "headers/weapon.h"
#include "headers/ui.h"
#include "headers/camera.h"
#include <SDL2/SDL.h>

SDL_Texture *singleTexture;

const uint32_t FPS = 60;
const float fixdt  = 1000.0f / FPS;

int main(int argc, char *argv[]) {
    srand(0);
    SDL_Init(SDL_INIT_EVERYTHING);
    Graphics *graphicsInstance = Graphics::GetInstance();
    UI *uiInstance = UI::GetInstance();
    uiInstance->LoadFont(graphicsInstance);
    uiInstance->SetState(GameState::MENU);

    graphicsInstance->SetTargetFPS(FPS);
    graphicsInstance->SetTitle("Game", "../../res/icon.bmp");

    singleTexture = graphicsInstance->LoadImage("../../res/tileset.png");
    Player player(100, 700);
    Map map;
    map.texture = singleTexture;
    map.tilemap = new Vec2i[34];
    map.tilemap[0] = {16, 64};
    map.tilemap[1] = {32, 64};
    map.tilemap[2] = {48, 64};
    map.tilemap[3] = {16, 80};
    map.tilemap[4] = {32, 80};
    map.tilemap[5] = {48, 80};
    map.tilemap[6] = {16, 96};
    map.tilemap[7] = {32, 96};

    map.tilemap[8] = {16, 0};
    map.tilemap[9] = {32, 0};
    map.tilemap[10] = {48, 0};

    map.tilemap[11] = {16, 16};
    map.tilemap[12] = {32, 16};
    map.tilemap[13] = {48, 16};

    map.tilemap[14] = {0 , 112};
    map.tilemap[15] = {16, 112};
    map.tilemap[16] = {0 , 128};
    map.tilemap[17] = {16, 128};
    map.tilemap[18] = {0 , 144};
    map.tilemap[19] = {16, 144};

    map.tilemap[20] = {32, 112};
    map.tilemap[21] = {48, 112};
    map.tilemap[22] = {32, 128};
    map.tilemap[23] = {48, 128};
    map.tilemap[24] = {32, 144};
    map.tilemap[25] = {48, 144};
    map.tilemap[26] = {32, 160};
    map.tilemap[27] = {48, 160};

    map.tilemap[28] = {80, 128};
    map.tilemap[29] = {64, 128};
    map.tilemap[30] = {80, 144};
    map.tilemap[31] = {64, 144};
    map.tilemap[32] = {80, 160};
    map.tilemap[33] = {64, 160};
    Vec2f mouse;

    SDL_Event e;

    map.tilesize = 16;
    map.drawsize = 2 * map.tilesize;
    map.dim = {73, 30};

    map.indices = new int [map.dim.x * map.dim.y];
    for (int x = 0; x < map.dim.x; x ++)
        for (int y = 0; y < map.dim.y; y ++)
            map.indices[x * map.dim.y + y] = (x == 0 || x == map.dim.x - 1 || y == 0 || y == map.dim.y - 1 ? -1 : 1) * ((rand() % 34) + 1);

    Vec2f offset;

    bool restart = false;
    Input *inputInstance = Input::GetInstance();
    inputInstance->BindActionToKey(SDL_SCANCODE_W, std::bind(&Player::MoveUp   , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_A, std::bind(&Player::MoveLeft , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_S, std::bind(&Player::MoveDown , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_D, std::bind(&Player::MoveRight, &player), true);
    inputInstance->BindActionToBtn(MouseButton::Left, std::bind(&Player::Attack, &player), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_DOWN, std::bind(&UI::ChangeOption, uiInstance, false), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_UP  , std::bind(&UI::ChangeOption, uiInstance, true ), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_RETURN, std::bind(&UI::ChooseOption, uiInstance, &restart), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_ESCAPE, std::bind(&UI::Pause, uiInstance), false);

    Camera cam(&player, {0, 0, 1280, 720});

    while(1) {
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                return 0;
            else if (e.type == SDL_KEYDOWN)
                inputInstance->KeyDown(e.key.keysym.scancode);
            else if (e.type == SDL_KEYUP)
                inputInstance->KeyUp(e.key.keysym.scancode);
            else if (e.type == SDL_MOUSEBUTTONDOWN)
                inputInstance->BtnDown(e.button.button, e.button.x, e.button.y);
            else if (e.type == SDL_MOUSEBUTTONUP)
                inputInstance->BtnUp(e.button.button);
        }
        inputInstance->Handle();

        GameState currentState = uiInstance->GetCurrentState();
        if(currentState == GameState::ALIVE){
            player.Update(graphicsInstance->GetDeltaTime());
            player.Collision(nullptr, map, graphicsInstance->GetDeltaTime());
            cam.Update();
            mouse = cam.GetCursorPosition();
            player.Seek(mouse);
            offset = cam.GetOffset();
        }

        if(restart){
            player.Restart(100, 700);
            restart = false;
        }

        graphicsInstance->Clear();
        if(currentState == GameState::ALIVE || currentState == GameState::PAUSE) {
            cam.Render(player, map);
        }
        uiInstance->Draw(graphicsInstance, player);

        graphicsInstance->Update();
    }

    return 0;
}
