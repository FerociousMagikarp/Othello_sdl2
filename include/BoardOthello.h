#ifndef BOARDOTHELLO_H
#define BOARDOTHELLO_H

enum Chess { EMPTY, BLACK, WHITE };
enum Result { CONTINUE, BLACK_WIN, WHITE_WIN, DRAW, PASS };

class BoardOthello
{
    public:
        BoardOthello();
        BoardOthello(const BoardOthello&);
        virtual ~BoardOthello();

        void init_board();
        Chess get_chess_type(int x, int y) const { return m_board[x][y]; }
        bool get_current_player_is_black() const { return m_current_player_is_black; }
        bool get_legal_pos(int x, int y) const { return m_legal_move[x][y]; }
        int get_legal_move_num() const { return m_legal_move_num; }

        bool is_legal(int x, int y);
        bool action_move(int x, int y);
        void refresh_legal_move();
        void pass_move();

        /** 0: black = white;
            1: black > white;
           -1: black < white; */
        int count_pieces();
        Result get_result();

    protected:

    private:
        Chess m_board[8][8];
        bool m_current_player_is_black;
        bool m_legal_move[8][8];
        int m_legal_move_num;
};

#endif // BOARDOTHELLO_H
