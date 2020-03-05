#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>
#include <vector>
#include <string>
#include <map>

class UIWidget;

class Scene
{
    public:
        Scene(SDL_Window* window, SDL_Renderer* renderer);
        virtual ~Scene();

        virtual void start();
        virtual void update(Uint64 start_time);

        virtual void mouse_click(int x, int y);

        void add_ui(const std::string& name, UIWidget* ui);
        UIWidget* find_ui_by_name(const std::string& name) { return _widgets[_widgets_name[name]]; }

    protected:
        SDL_Window* _window;
        SDL_Renderer* _renderer;

        std::vector<UIWidget*> _widgets;
        std::map<std::string, int> _widgets_name;

    private:

};

#endif // SCENE_H
