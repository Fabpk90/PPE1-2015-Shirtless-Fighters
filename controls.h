#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#include "Menu.h"

struct Controls{
    Image i_Background;

    void Init()
    {
        i_Background = CreateImage("data/backgrounds_menu/controls.png");
    }

    void Draw()
    {
        while (!KeyGet(SDLK_ESCAPE) && !StickGetClick(0, 7))
        {
            DrawImage(i_Background);
            RefreshScreen();
        }
    }

};

#endif // CONTROLS_H_INCLUDED
