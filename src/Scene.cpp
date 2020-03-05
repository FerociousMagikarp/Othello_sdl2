#include "Scene.h"
#include <SDL_image.h>
#include "UIWidget.h"

using namespace std;

Scene::Scene(SDL_Window* window, SDL_Renderer* renderer)
{
    this->_window = window;
    this->_renderer = renderer;
}

Scene::~Scene()
{
    for (vector<UIWidget*>::iterator iter=_widgets.begin(); iter != _widgets.end(); iter++)
        delete (*iter);
}

void Scene::start()
{
}

void Scene::update(Uint64 start_time)
{
    for (unsigned int i = 0; i < _widgets.size(); i++)
    {
        _widgets[i]->update();
    }
}

void Scene::mouse_click(int x, int y)
{
    for (unsigned int i = 0; i < _widgets.size(); i++)
    {
        if (_widgets[i]->is_in(x, y))
        {
            _widgets[i]->mouse_click(x, y);
            break;
        }
    }
}

void Scene::add_ui(const string& name, UIWidget* ui)
{
    _widgets_name[name] = _widgets.size();
    _widgets.push_back(ui);
}
