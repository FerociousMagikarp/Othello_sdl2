#ifndef UIBOARDOTHELLO_H
#define UIBOARDOTHELLO_H

#include <UIWidget.h>
#include <string>
#include "BoardOthello.h"

class SceneGame;

enum Player { HUMAN, AI };
typedef void(SceneGame::*MessageFuncP)(std::string message);
typedef void(SceneGame::*MoveFuncP)(int x, int y);

class UIBoardOthello : public UIWidget
{
    public:
        UIBoardOthello(SDL_Renderer* renderer);
        virtual ~UIBoardOthello();
        virtual void update();
        virtual void mouse_click(int x, int y);

        void set_show_command(SceneGame* scene, MessageFuncP message_command, MoveFuncP human_move_command);

        void start(Player player1, Player player2);
        BoardOthello get_board() const { return m_board; }
        Player get_current_player() { return m_board.get_current_player_is_black() ? players[0] : players[1];}
        void action_move(int x, int y);

    protected:

    private:
        SDL_Texture* m_board_bg;
        SDL_Texture* m_board_cell;
        SDL_Texture* m_chess_black;
        SDL_Texture* m_chess_white;
        SDL_Texture* m_black_selected;
        SDL_Texture* m_white_selected;

        SDL_Rect m_cell_rect;
        SDL_Rect m_chess_pos_rect[8][8];

        BoardOthello m_board;
        Player players[2];

        MessageFuncP m_show_message;
        MoveFuncP m_human_move;
        SceneGame* m_scene_game;
};

#endif // UIBOARDOTHELLO_H
