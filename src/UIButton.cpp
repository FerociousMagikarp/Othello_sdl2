#include "UIButton.h"

using namespace std;

UIButton::UIButton(int x, int y, int w, int h, SDL_Renderer* renderer) : UIWidget(SDL_Rect(), renderer)
{
    _pos.x = x;
    _pos.y = y;
    _pos.w = w;
    _pos.h = h;
    m_button_command = NULL;
}

UIButton::~UIButton()
{
    SDL_DestroyTexture(m_origin_texture);
    SDL_DestroyTexture(m_pressed_texture);
}

void UIButton::update()
{
    SDL_RenderCopy(_renderer, m_origin_texture, NULL, &_pos);
}

void UIButton::mouse_click(int x, int y)
{
    if (m_button_command != NULL)
        (m_scene->*m_button_command)();
}

void UIButton::set_textures(const string& origin, const string& pressed)
{
    m_origin_texture = load_texture(origin);
    m_pressed_texture = load_texture(pressed);
}

void UIButton::set_command(Scene* scene, BtnFuncP f)
{
    m_button_command = f;
    m_scene = scene;
}
