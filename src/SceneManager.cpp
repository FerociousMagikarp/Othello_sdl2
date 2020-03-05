#include "SceneManager.h"
#include "settings.h"
#include "SceneGame.h"
#include <math.h>
#include <iostream>

using namespace std;

SceneManager::SceneManager()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    m_window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    m_run = true;
    scene = new SceneGame(m_window, m_renderer);
}

SceneManager::~SceneManager()
{
    quit();
    delete scene;
}

SceneManager* SceneManager::get_instance()
{
    static SceneManager instance;
    return &instance;
}

void SceneManager::run()
{
    SDL_Event event;
    while (m_run)
    {
        Uint64 start_time = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit();
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    scene->mouse_click(event.motion.x, event.motion.y);
                }
            }
        }
        SDL_RenderClear(m_renderer);
        scene->update(start_time);
        SDL_RenderPresent(m_renderer);

        Uint64 end_time = SDL_GetPerformanceCounter();
        float elapsedMS = (end_time - start_time) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        float rest_time = floor(1000.0f / GAME_FPS - elapsedMS);
        SDL_Delay(rest_time > 0 ? rest_time : 0);
    }
}

void SceneManager::quit()
{
    m_run = false;
}
