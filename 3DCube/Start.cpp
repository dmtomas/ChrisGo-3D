#include "SDL.h"
#include <iostream>
#include <math.h>
#include "Objects.h"
#include "Terrain.h"

class Game {
public:
    int t = 0;
    Player* player = new Player;
    Terrain* terrain = new Terrain(10, 0);
    Game() {
        isRunning = false;
    };
    ~Game() {};
    void Init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
            std::cout << "The graphics are starting..." << std::endl;
            window = SDL_CreateWindow("SDL 3D engine.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,0);
            renderer = SDL_CreateRenderer(window, -1, 0);

            std::cout << "The proyect has already started." << std::endl;
            isRunning = true;

        }
        else {
            isRunning = false;
        }
    }
    void HandleEvents() {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_LEFT) {
                player->Move(20, 0, 0);
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                player->Move(-20, 0, 0);
            }
            if (event.key.keysym.sym == SDLK_UP) {
                player->Move(0, 0, -20);
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                player->Move(0, 0, 20);
            }
            if (event.key.keysym.sym == SDLK_SPACE) {
                player->Move(0, 20, 0);
            }
            if (event.key.keysym.sym == SDLK_LSHIFT) {
                player->Move(0, -20, 0);
            }
            break;
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;

        }
    }
    void Update() {
        int x, y;
        Uint32 buttons;
        t++;
        buttons = SDL_GetMouseState(&x, &y);
        (*player).Rotate(x, y);
    }
    void Render() {
        float absoluteCenter[3] = { 0, 0, 0 };

        // Points of the 3D rectangle.
        Point point1;
        point1.Fill(100, 200, 600);
        Point point2;
        point2.Fill(300, 300, 900);

        SDL_SetRenderDrawColor(renderer, 58, 225, 231, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        (*terrain).ManageChuncks(player, renderer);
        Cube cubo(point1, point2, player);

        //Rotate the cube acording to the players sight.
        cubo.RotateY(-player->angleX, absoluteCenter);
        cubo.RotateX(player->angleY, absoluteCenter);

        //Render only if the cube is in front of the player and not too far.
        if (cubo.pt1.position[2] > 1 && cubo.pt1.position[2] < 10000) {
            cubo.RenderCube(renderer, player);
        }
        SDL_RenderPresent(renderer);
    }
    void Clean() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        delete player;
        delete terrain;
        std::cout << "Game cleaned." << std::endl;

    }
    bool isRunning;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};


Game* game = nullptr;
int main(int argc, char* argv[]) {
    game = new Game();
    game->Init();
    while (game->isRunning) {
        game->HandleEvents();
        game->Update();
        game->Render();
    }
    game->Clean();
    return 0;
}