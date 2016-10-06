#ifndef TWOPLAYERS_H_INCLUDED
#define TWOPLAYERS_H_INCLUDED

#include "Menu_Select_Joueurs.h"

struct TwoPlayers{
    Image i_Background;
    Text pseudo[2];
    Color textColor;
    Vec2 textPos1;
    Vec2 textPos2;

    void Init()
    {
        i_Background = CreateImage("data/backgrounds_menu/2players.png");

        textColor = CreateColor(255,0,0);

        pseudo[0] = CreateText("écrire ici", textColor, 32);
        pseudo[1] = CreateText("écrire ici", textColor, 32);
        TextSetAlterable(pseudo[0], true);
        TextSetAlterable(pseudo[1], true);
        TextSetLimitEditable(pseudo[0],10);
        TextSetLimitEditable(pseudo[1],10);
        textPos1 = CreateVec2(570, 293);
        textPos2 = CreateVec2(570, 126);
        TextSetPosition(pseudo[0], textPos1);
        TextSetPosition(pseudo[1], textPos2);
    }

    int Draw()
    {
        RefreshScreen();

        while (!KeyGetOneShot(SDLK_ESCAPE) && !StickGetClick(0, 7))
        {
            if (KeyGetOneShot(SDLK_RETURN))
                return 0;

            DrawImage(i_Background);
            DrawText(pseudo[0]);
            DrawText(pseudo[1]);
            RefreshScreen();
        }

    }

    Text GetNames()
    {
        return pseudo[0],pseudo[1];
    }
};

#endif // 2PLAYERS_H_INCLUDED
