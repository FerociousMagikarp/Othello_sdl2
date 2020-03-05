#ifndef SCENEGAME_H
#define SCENEGAME_H

#include <Scene.h>
#include <SDL.h>
#include <string>

class AI;
class UIBoardOthello;

class SceneGame : public Scene
{
    public:
        SceneGame(SDL_Window* window, SDL_Renderer* renderer);
        virtual ~SceneGame();
        virtual void start();
        virtual void update(Uint64 start_time);

        void start_game();
        void exit();

        // get variables from ui_board
        void show_message(std::string message);
        void human_last_move(int x, int y);

    protected:

    private:
        AI* m_ai;
        UIBoardOthello* m_ui_board;

};

#endif // SCENEGAME_H
