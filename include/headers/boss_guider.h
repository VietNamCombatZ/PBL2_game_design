#pragma once
#include "../sdl2/SDL_image.h"
class BossGuider
{
public:
    BossGuider();
    ~BossGuider();
    void Update();
    void Render();
    bool active;
private:
    SDL_Texture *circle;
    SDL_Rect dest;
};

extern BossGuider *boss_guider;