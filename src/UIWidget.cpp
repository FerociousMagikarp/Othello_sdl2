#include "UIWidget.h"
#include <SDL_image.h>

UIWidget::UIWidget(const SDL_Rect& pos, SDL_Renderer* renderer)
{
    this->_pos.x = pos.x;
    this->_pos.y = pos.y;
    this->_pos.w = pos.w;
    this->_pos.h = pos.h;
    this->_renderer = renderer;
    this->_is_active = true;
}

UIWidget::~UIWidget()
{
}

bool UIWidget::is_in(int x, int y)
{
    if (_is_active)
        if (x > _pos.x && x < _pos.x + _pos.w && y > _pos.y && y < _pos.y + _pos.h)
            return true;
    return false;
}

SDL_Texture* UIWidget::load_texture(const std::string& filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
