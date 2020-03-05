#ifndef UIWIDGET_H
#define UIWIDGET_H

#include <SDL.h>
#include <string>

enum WidgetState { IN, OUT };

class UIWidget
{
    public:
        UIWidget(const SDL_Rect& pos, SDL_Renderer* renderer);
        virtual ~UIWidget();

        virtual void update() = 0;
        virtual void mouse_click(int x, int y) = 0;

        SDL_Rect* get_pos_rect() { return &_pos; }
        virtual bool is_in(int x, int y);

        bool get_is_active() const { return _is_active; }
        // void set_is_active(bool value) { _is_active = value; }

        SDL_Texture* load_texture(const std::string& filename);

    protected:
        SDL_Rect _pos;
        SDL_Renderer* _renderer;
        bool _is_active;

    private:
};

#endif // UIWIDGET_H
