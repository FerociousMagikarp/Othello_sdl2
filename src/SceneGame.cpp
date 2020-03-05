#include "SceneGame.h"
#include "settings.h"
#include "UIBoardOthello.h"
#include "UIButton.h"
#include "SceneManager.h"
#include "settings.h"
#include "AIUct.h"
#include <iostream>

using namespace std;

SceneGame::SceneGame(SDL_Window* window, SDL_Renderer* renderer) : Scene(window, renderer)
{
    start();
    m_ai = new AIUct();
}

SceneGame::~SceneGame()
{
    delete m_ai;
}

void SceneGame::start()
{
    Scene::start();
    m_ui_board = new UIBoardOthello(_renderer);
    m_ui_board->set_show_command(this, &show_message, &human_last_move);
    add_ui("board", m_ui_board);
    UIButton* start_game = new UIButton(900, 720, 130, 70, _renderer);
    start_game->set_textures("pictures/btn_startgame.png", "pictures/btn_startgame_pressed.png");
    start_game->set_command(this, (BtnFuncP)&SceneGame::start_game);
    add_ui("btn_start", start_game);
    UIButton* backmove = new UIButton(1050, 720, 130, 70, _renderer);
    backmove->set_textures("pictures/btn_backmove.png", "pictures/btn_backmove_pressed.png");
    // backmove->set_command(this, (BtnFuncP)&SceneGame::start_game);
    add_ui("btn_backmove", backmove);
    UIButton* giveup = new UIButton(900, 810, 130, 70, _renderer);
    giveup->set_textures("pictures/btn_giveup.png", "pictures/btn_giveup_pressed.png");
    // giveup->set_command(this, (BtnFuncP)&SceneGame::start_game);
    add_ui("btn_giveup", giveup);
    UIButton* exit = new UIButton(1050, 810, 130, 70, _renderer);
    exit->set_textures("pictures/btn_exit.png", "pictures/btn_exit_pressed.png");
    exit->set_command(this, (BtnFuncP)&SceneGame::exit);
    add_ui("btn_exit", exit);
}

void SceneGame::update(Uint64 start_time)
{
    Scene::update(start_time);
    float rest_time = 0.0f;
    do
    {
        if (m_ui_board->get_is_active())
        {
            m_ai->think();
            if (m_ai->get_think_enough() && m_ui_board->get_current_player() == AI)
            {
                int a = m_ai->get_action();
                m_ui_board->action_move(a / 8, a % 8);
            }
        }
        Uint64 end_time = SDL_GetPerformanceCounter();
        float elapsedMS = (end_time - start_time) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        rest_time = 1000.0f / GAME_FPS - elapsedMS;
    } while (rest_time > 10.0f);
}

void SceneGame::start_game()
{
    m_ui_board->start(AI, HUMAN);
    m_ai->init();
}

void SceneGame::exit()
{
    SceneManager::get_instance()->quit();
}

void SceneGame::show_message(string message)
{
    cout << message << endl;
}

void SceneGame::human_last_move(int x, int y)
{
    m_ai->next_move(x, y);
}
