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

    singleTexture = graphicsInstance->LoadImage("../../res/0x72_DungeonTilesetII_v1.4/0x72_DungeonTilesetII_v1.4.png");
    Player player(100, 100);
    Map map;
    map.texture = singleTexture;
    map.tilemap = new Vec2i[8];
    map.tilemap[0] = {16, 64};
    map.tilemap[1] = {32, 64};
    map.tilemap[2] = {48, 64};
    map.tilemap[3] = {16, 80};
    map.tilemap[4] = {32, 80};
    map.tilemap[5] = {48, 80};
    map.tilemap[6] = {16, 96};
    map.tilemap[7] = {32, 96};
    Vec2f mouse;

    SDL_Event e;

    map.tilesize = 16;
    map.drawsize = 2 * map.tilesize;
    map.dim = {73, 30};

    map.indices = new int [map.dim.x * map.dim.y];
    for (int x = 0; x < map.dim.x; x ++)
        for (int y = 0; y < map.dim.y; y ++)
            map.indices[x * map.dim.y + y] = (x == 0 || x == map.dim.x - 1 || y == 0 || y == map.dim.y - 1 ? -1 : 1) * ((rand() & 3) + 1);

    Vec2f offset;
    Vec2f f = {0.1, 0};

    Input *inputInstance = Input::GetInstance();
    inputInstance->BindActionToKey(SDL_SCANCODE_W, std::bind(&Player::MoveUp   , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_A, std::bind(&Player::MoveLeft , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_S, std::bind(&Player::MoveDown , &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_D, std::bind(&Player::MoveRight, &player), true);
    inputInstance->BindActionToKey(SDL_SCANCODE_L, std::bind(&Player::AddForce , &player, f), true);
    inputInstance->BindActionToBtn(MouseButton::Left, std::bind(&Player::Attack, &player), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_DOWN, std::bind(&UI::ChangeOption, uiInstance, false), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_UP  , std::bind(&UI::ChangeOption, uiInstance, true ), false);
    inputInstance->BindActionToKey(SDL_SCANCODE_RETURN, std::bind(&UI::ChooseOption, uiInstance), false);

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

        if(uiInstance->GetCurrentState() == GameState::ALIVE){
            player.Update(graphicsInstance->GetDeltaTime());
            player.Collision(nullptr, map, graphicsInstance->GetDeltaTime());
            cam.Update();
            mouse = cam.GetCursorPosition();
            player.FaceTowards(mouse);
            offset = cam.GetOffset();
        }

        graphicsInstance->Clear();
        if(uiInstance->GetCurrentState() == GameState::ALIVE) {
            cam.Render(player, map);
        }
        uiInstance->Draw(graphicsInstance, player);

        graphicsInstance->Update();
    }

    return 0;
}
