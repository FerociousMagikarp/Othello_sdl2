#include "AIUct.h"
#include <math.h>
#include <iostream>

using namespace std;

TreeNode::TreeNode(BoardOthello* board, TreeNode* parent) : N(0), Q(0), s(board), a(-1), parent(parent)
{
    calculate_value();
    init_actions();
}

TreeNode::~TreeNode()
{
    delete s;
    for (unsigned int i = 0; i < children.size(); i++)
    {
        delete children[i];
    }
}

void TreeNode::calculate_value()
{
    result = s->get_result();
    Chess current_player_chess_type = s->get_current_player_is_black() ? BLACK : WHITE;
    switch (result)
    {
        case CONTINUE:
            value = s->get_legal_move_num() * 20;
            if (s->get_chess_type(0, 0) == current_player_chess_type)
            {
                value += 200;
            }
            else
            {
                if (s->get_chess_type(0, 0) != EMPTY) value -= 200;
            }
            if (s->get_chess_type(0, 7) == current_player_chess_type)
            {
                value += 200;
            }
            else
            {
                if (s->get_chess_type(0, 7) != EMPTY) value -= 200;
            }
            if (s->get_chess_type(7, 0) == current_player_chess_type)
            {
                value = 200;
            }
            else
            {
                if (s->get_chess_type(7, 0) != EMPTY) value -= 200;
            }
            if (s->get_chess_type(7, 7) == current_player_chess_type)
            {
                value += 200;
            }
            else
            {
                if (s->get_chess_type(7, 7) != EMPTY) value -= 200;
            }
            break;
        case BLACK_WIN:
            if (s->get_current_player_is_black())
                value = 1000;
            else
                value = -1000;
            break;
        case WHITE_WIN:
            if (s->get_current_player_is_black())
                value = -1000;
            else
                value = 1000;
            break;
        case PASS:
            value = -600;
        case DRAW:
            value = 0;
            break;
        default:
            break;
    }
}

void TreeNode::init_actions()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (s->get_legal_pos(i, j))
                actions.push_back(i * 8 + j);
        }
    }
}

UctTree::UctTree(BoardOthello* board)
{
    m_root = new TreeNode(board, 0);
}

UctTree::~UctTree()
{
    if (m_root != 0)
        delete m_root;
}

void UctTree::uct_search()
{
    TreeNode* node = tree_policy(m_root);
    back_up(node);
}

TreeNode* UctTree::expand(TreeNode* node)
{
    int children_size = node->children.size();
    int action_size = node->actions.size();
    if (action_size == 0)
    {
        BoardOthello* board = new BoardOthello(*(node->s));
        board->pass_move();
        if (board->get_legal_move_num() == 0)
        {
            delete board;
            return 0;
        }
        TreeNode* child = new TreeNode(board, node);
        node->children.push_back(child);
        return child;
    }
    if (children_size < action_size)
    {
        BoardOthello* board = new BoardOthello(*(node->s));
        int a = node->actions[children_size];
        board->action_move(a / 8, a % 8);
        TreeNode* child = new TreeNode(board, node);
        child->a = a;
        node->children.push_back(child);
        return child;
    }
    return 0;
}

TreeNode* UctTree::tree_policy(TreeNode* node)
{
    while (node->result == CONTINUE || node->result == PASS)
    {
        if (node->result == PASS)
        {
            if (node->children.size() == 0)
            {
                expand(node);
            }
            node = node->children[0];
            continue;
        }
        if (node->children.size() < node->actions.size())
            return expand(node);
        else
            node = get_best_child(node);
    }
    return node;
}

void UctTree::back_up(TreeNode* node)
{
    int value = node->value;
    while(node)
    {
        node->N++;
        node->Q -= value;
        value = -value;
        node = node->parent;
    }
}

TreeNode* UctTree::get_best_child(TreeNode* node)
{
    TreeNode* best_child = 0;
    double max_value = -100000;
    for (unsigned int i = 0; i < node->children.size(); i++)
    {
        TreeNode* child = node->children[i];
        double value = (double)child->Q / child->N / 1000 + 1.4 * sqrt(log(node->N) / child->N);
        if (value > max_value)
        {
            max_value = value;
            best_child = child;
        }
    }
    return best_child;
}

TreeNode* UctTree::get_best_child()
{
    if (m_root->result == PASS)
    {
        if (m_root->children.size() == 0)
            expand(m_root);
        m_root = m_root->children[0];
        m_root->parent->children.clear();
        delete m_root->parent;
        m_root->parent = 0;
    }
    TreeNode* best_child = 0;
    double max_value = -100000;
    for (unsigned int i = 0; i < m_root->children.size(); i++)
    {
        TreeNode* child = m_root->children[i];
        double value = (double)child->Q / child->N;
        if (value > max_value)
        {
            max_value = value;
            best_child = child;
        }
    }
    cout << (double)m_root->Q / m_root->N << endl;
    cout << m_root->value << endl;
    return best_child;
}

void UctTree::next_move(int x, int y)
{
    while (m_root->result == PASS)
    {
        if (m_root->children.size() == 0)
            expand(m_root);
        m_root = m_root->children[0];
        m_root->parent->children.clear();
        delete m_root->parent;
        m_root->parent = 0;
    }
    while(m_root->children.size() < m_root->actions.size())
    {
        expand(m_root);
    }
    int a = x * 8 + y;
    for (vector<TreeNode*>::iterator iter = m_root->children.begin(); iter != m_root->children.end(); iter++)
    {
        if ((*iter)->a == a)
        {
            m_root = (*iter);
            m_root->parent->children.erase(iter);
            delete m_root->parent;
            m_root->parent = 0;
            break;
        }
    }
}

void AIUct::init()
{
    if (uct != 0)
        delete uct;
    BoardOthello* board = new BoardOthello();
    uct = new UctTree(board);
}

void AIUct::think()
{
    for (int i = 0; i < 100; i++)
    {
        uct->uct_search();
    }
    m_uct_num += 100;
    if (m_uct_num >= 3200)
        _think_enough = true;
}

int AIUct::get_action()
{
    _think_enough = false;
    TreeNode* node = uct->get_best_child();
    int a = node->a;
    next_move(a / 8, a % 8);
    return a;
}

void AIUct::next_move(int x, int y)
{
    m_uct_num = 0;
    uct->next_move(x, y);
}
