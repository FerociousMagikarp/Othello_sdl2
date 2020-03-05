#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <UIWidget.h>
#include <string>

class Scene;

typedef void(Scene::*BtnFuncP)(void);

class UIButton : public UIWidget
{
    public:
        UIButton(const SDL_Rect& pos, SDL_Renderer* renderer) : UIWidget(pos, renderer) { m_button_command = NULL; }
        UIButton(int x, int y, int w, int h, SDL_Renderer* renderer);
        virtual ~UIButton();

        virtual void update();
        virtual void mouse_click(int x, int y);

        void set_textures(const std::string& origin, const std::string& pressed);
        void set_command(Scene* scene, BtnFuncP f);

    protected:

    private:
        SDL_Texture* m_origin_texture;
        SDL_Texture* m_pressed_texture;

        BtnFuncP m_button_command;
        Scene* m_scene;

};

#endif // UIBUTTON_H
