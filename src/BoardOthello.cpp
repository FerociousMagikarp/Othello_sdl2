#include "BoardOthello.h"

BoardOthello::BoardOthello()
{
    init_board();
}

BoardOthello::BoardOthello(const BoardOthello& b)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            m_board[i][j] = b.get_chess_type(i, j);
        }
    }
    m_current_player_is_black = b.get_current_player_is_black();
    refresh_legal_move();
}

BoardOthello::~BoardOthello()
{

}

void BoardOthello::init_board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            m_board[i][j] = EMPTY;
        }
    }
    m_board[3][3] = m_board[4][4] = WHITE;
    m_board[3][4] = m_board[4][3] = BLACK;
    m_current_player_is_black = true;
    refresh_legal_move();
}

bool BoardOthello::is_legal(int x, int y)
{
    if (m_board[x][y] == EMPTY)
    {
        int dx[8] = { 0, 0, 1,-1, 1, 1,-1,-1};
        int dy[8] = { 1,-1, 0, 0, 1,-1, 1,-1};
        for (int i=0; i < 8; i++)
        {
            int n = 0;
            int x_ = x + dx[i];
            int y_ = y + dy[i];
            while(x_ >= 0 && x_ < 8 && y_ >= 0 && y_ < 8)
            {
                if (m_board[x_][y_] == EMPTY)
                    break;
                if ((m_current_player_is_black && m_board[x_][y_] == WHITE) ||
                    (!m_current_player_is_black && m_board[x_][y_] == BLACK))
                        n++;
                else
                {
                    if (n > 0)
                        return true;
                    break;
                }
                x_ += dx[i];
                y_ += dy[i];
            }
        }
    }
    return false;
}

bool BoardOthello::action_move(int x, int y)
{
    if (m_legal_move[x][y] == true)
    {
        if (m_current_player_is_black)
            m_board[x][y] = BLACK;
        else
            m_board[x][y] = WHITE;
        int dx[8] = { 0, 0, 1,-1, 1, 1,-1,-1};
        int dy[8] = { 1,-1, 0, 0, 1,-1, 1,-1};

        for (int i=0; i < 8; i++)
        {
            int n = 0;
            bool flag = false;
            int x_ = x + dx[i];
            int y_ = y + dy[i];
            while(x_ >= 0 && x_ < 8 && y_ >= 0 && y_ < 8)
            {
                if (m_board[x_][y_] == EMPTY)
                    break;
                if ((m_current_player_is_black && m_board[x_][y_] == WHITE) ||
                    (!m_current_player_is_black && m_board[x_][y_] == BLACK))
                        n++;
                else
                {
                    if (n > 0)
                    {
                        flag = true;
                        x_ -= dx[i];
                        y_ -= dy[i];
                    }
                    break;
                }
                x_ += dx[i];
                y_ += dy[i];
            }
            if (flag)
            {
                while ((m_current_player_is_black && m_board[x_][y_] == WHITE) ||
                    (!m_current_player_is_black && m_board[x_][y_] == BLACK))
                {
                    if (m_current_player_is_black)
                        m_board[x_][y_] = BLACK;
                    else
                        m_board[x_][y_] = WHITE;
                    x_ -= dx[i];
                    y_ -= dy[i];
                }
            }
        }

        m_current_player_is_black = !m_current_player_is_black;
        refresh_legal_move();
        return true;
    }
    return false;
}

void BoardOthello::refresh_legal_move()
{
    m_legal_move_num = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (is_legal(i, j))
            {

                m_legal_move[i][j] = true;
                m_legal_move_num++;
            }
            else
            {
                m_legal_move[i][j] = false;
            }
        }
    }
}

void BoardOthello::pass_move()
{
    m_current_player_is_black = !m_current_player_is_black;
    refresh_legal_move();
}

int BoardOthello::count_pieces()
{
    int black = 0, white = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j=0; j < 8; j++)
        {
            if (m_board[i][j] == BLACK)
                black++;
            else if (m_board[i][j] == WHITE)
                white++;
        }
    }
    if (black > white)
        return 1;
    else if (black < white)
        return -1;
    else
        return 0;
}

Result BoardOthello::get_result()
{
    if (m_legal_move_num != 0)
        return CONTINUE;
    m_current_player_is_black = !m_current_player_is_black;
    int legal_move_num = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (is_legal(i, j))
            {

                legal_move_num++;
            }
        }
    }
    if (legal_move_num != 0)
    {
        m_current_player_is_black = !m_current_player_is_black;
        return PASS;
    }
    int result = count_pieces();
    if (result == 1)
        return BLACK_WIN;
    else if (result == -1)
        return WHITE_WIN;
    else
        return DRAW;
}
