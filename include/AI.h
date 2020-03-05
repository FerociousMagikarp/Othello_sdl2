#ifndef AI_H
#define AI_H

class AI
{
    public:
        AI();
        virtual ~AI();

        virtual void init() = 0;
        virtual void think() = 0;
        virtual int get_action() = 0;
        virtual void next_move(int x, int y) = 0;

        bool get_think_enough() const { return _think_enough; }

    protected:
        bool _think_enough;

    private:
};

#endif // AI_H
