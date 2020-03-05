#ifndef AIUCT_H
#define AIUCT_H

#include <AI.h>
#include <vector>

#include "BoardOthello.h"

class TreeNode
{
    public:
        TreeNode(BoardOthello* board, TreeNode* parent);
        virtual ~TreeNode();

        void calculate_value();
        void init_actions();

        int N;
        int Q;
        BoardOthello* s;
        int a;
        int value;
        TreeNode* parent;
        std::vector<TreeNode*> children;
        std::vector<int> actions;
        Result result;
};

class UctTree
{
    public:
        UctTree(BoardOthello* board);
        virtual ~UctTree();

        void uct_search();
        TreeNode* expand(TreeNode* node);
        TreeNode* tree_policy(TreeNode* node);
        void back_up(TreeNode* node);
        TreeNode* get_best_child(TreeNode* node);
        TreeNode* get_best_child();
        void next_move(int x, int y);

    private:
        TreeNode* m_root;
};

class AIUct : public AI
{
    public:
        AIUct() : AI() { m_uct_num = 0; uct = 0; }
        virtual ~AIUct() { if (uct != 0) delete uct; }

        virtual void init();
        virtual void think();
        virtual int get_action();
        virtual void next_move(int x, int y);

    protected:

    private:
        int m_uct_num;
        UctTree* uct;
};

#endif // AIUCT_H
