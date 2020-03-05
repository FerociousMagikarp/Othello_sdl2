#include "UIBoardOthello.h"
#include "settings.h"

using namespace std;

UIBoardOthello::UIBoardOthello(SDL_Renderer* renderer) :
    UIWidget(SDL_Rect(), renderer)
{
    // Rect of board
    _pos.x = _pos.y = BOARD_MARGIN;
    _pos.w = _pos.h = WINDOW_HEIGHT - BOARD_MARGIN * 2;

    // Rect of cell
    m_cell_rect.x = m_cell_rect.y = BOARD_MARGIN + BOARD_CELL_MARGIN;
    m_cell_rect.w = m_cell_rect.h = WINDOW_HEIGHT - m_cell_rect.x * 2;

    // Rect of chess
    int start_pos = m_cell_rect.x + 1;  // border width
    int cell_side_length = m_cell_rect.w / 8;
    int chess_margin = 8;
    int chess_side_length = cell_side_length - chess_margin * 2;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            SDL_Rect rect;
            rect.x = start_pos + j * cell_side_length + chess_margin;
            rect.y = start_pos + i * cell_side_length + chess_margin;
            rect.w = rect.h = chess_side_length;
            m_chess_pos_rect[i][j] = rect;
        }
    }

    players[0] = players[1] = HUMAN;
    _is_active = false;
    m_show_message = NULL;

    m_board_bg = load_texture("pictures/board_bg.png");
    m_board_cell = load_texture("pictures/board_cell.png");
    m_chess_black = load_texture("pictures/black.png");
    m_chess_white = load_texture("pictures/white.png");
    m_black_selected = load_texture("pictures/black_selected.png");
    m_white_selected = load_texture("pictures/white_selected.png");
}

UIBoardOthello::~UIBoardOthello()
{
    SDL_DestroyTexture(m_board_bg);
    SDL_DestroyTexture(m_board_cell);
    SDL_DestroyTexture(m_chess_black);
    SDL_DestroyTexture(m_chess_white);
    SDL_DestroyTexture(m_black_selected);
    SDL_DestroyTexture(m_white_selected);
}

void UIBoardOthello::update()
{
    SDL_RenderCopy(_renderer, m_board_bg, NULL, &_pos);
    SDL_RenderCopy(_renderer, m_board_cell, NULL, &m_cell_rect);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch(m_board.get_chess_type(i, j))
            {
            case BLACK:
                SDL_RenderCopy(_renderer, m_chess_black, NULL, &m_chess_pos_rect[i][j]);
                break;
            case WHITE:
                SDL_RenderCopy(_renderer, m_chess_white, NULL, &m_chess_pos_rect[i][j]);
                break;
            default:
                if (_is_active && m_board.get_legal_pos(i, j))
                {
                    if (m_board.get_current_player_is_black())
                        SDL_RenderCopy(_renderer, m_black_selected, NULL, &m_chess_pos_rect[i][j]);
                    else
                        SDL_RenderCopy(_renderer, m_white_selected, NULL, &m_chess_pos_rect[i][j]);
                }
                break;
            }
        }
    }
}

void UIBoardOthello::mouse_click(int x, int y)
{
    if ((m_board.get_current_player_is_black() && players[0] == HUMAN) ||
        (!m_board.get_current_player_is_black() && players[1] == HUMAN))
    {
        if (x > m_cell_rect.x && y > m_cell_rect.y &&
            x < m_cell_rect.x + m_cell_rect.w && y < m_cell_rect.y + m_cell_rect.h)
        {
            // Transform click position to board position
            int cell_side_length = m_cell_rect.w / 8;
            int board_y = (x - m_cell_rect.x) / cell_side_length;  // reverse x and y
            int board_x = (y - m_cell_rect.y) / cell_side_length;

            action_move(board_x, board_y);
        }
    }
}

void UIBoardOthello::set_show_command(SceneGame* scene, MessageFuncP message_command, MoveFuncP human_move_command)
{
    m_scene_game = scene;
    m_show_message = message_command;
    m_human_move = human_move_command;
}

void UIBoardOthello::start(Player player1, Player player2)
{
    players[0] = player1;
    players[1] = player2;
    m_board.init_board();
    _is_active = true;
}

void UIBoardOthello::action_move(int x, int y)
{
    if (m_board.get_legal_pos(x, y))
    {
        m_board.action_move(x, y);
        (m_scene_game->*m_human_move)(x, y);
    }
    Result result = m_board.get_result();
    switch (result)
    {
        case CONTINUE:
            break;
        case PASS:
            if (m_board.get_current_player_is_black())
                (m_scene_game->*m_show_message)("black pass!");
            else
                (m_scene_game->*m_show_message)("white pass!");
            m_board.pass_move();
            break;
        case BLACK_WIN:
            (m_scene_game->*m_show_message)("black win!");
            _is_active = false;
            break;
        case WHITE_WIN:
            (m_scene_game->*m_show_message)("white win!");
            _is_active = false;
            break;
        case DRAW:
            (m_scene_game->*m_show_message)("draw!");
            _is_active = false;
            break;
    }
}
