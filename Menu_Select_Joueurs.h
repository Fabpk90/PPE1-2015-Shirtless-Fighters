#ifndef MENU_SELECT_JOUEURS_H_INCLUDED
#define MENU_SELECT_JOUEURS_H_INCLUDED

#include <vector>

#include "Perso.h"
#include "Map.h"
#include "Menu.h"
#include "2players.h"
#include "3players.h"
#include "4players.h"

struct MenuSelectNumberOfPlayers{

    Image i_Background;
    Image i_Cursor;
    int noPerso; //variable pour selection des pseudos
    Vec2 pos;   //position cursor
    int tableau_Pos_Y[3];
    std::vector<Perso> players;
    int no_perso;
    Text nom_Joueurs[4];

    void Init()
    {
        tableau_Pos_Y[0] = 394;     //2
        tableau_Pos_Y[1] = 297;     //3
        tableau_Pos_Y[2] = 190;     //4
        i_Background = CreateImage("data/backgrounds_menu/2.png");

        i_Cursor = CreateImage("data/cursor.ico");
        ImgSetRotation(i_Cursor, 224);
        pos = CreateVec2(990, tableau_Pos_Y[0]); //pos du curseur sur 2
        ImgSetPosition(i_Cursor, pos);
    }

    int Draw()
    {
        while (!KeyGetOneShot(SDLK_ESCAPE) && !StickGetClick(0, 7))
        {
            //gestion de la position de Cursor
            if (KeyGetOneShot(SDLK_UP))
            {
                if(pos.y == tableau_Pos_Y[0])
                {pos.y = tableau_Pos_Y[2];}
                else if (pos.y == tableau_Pos_Y[1])
                {pos.y = tableau_Pos_Y[0];}
                else if (pos.y == tableau_Pos_Y[2])
                {pos.y = tableau_Pos_Y[1];}
            }
            if (KeyGetOneShot(SDLK_DOWN))
            {
                if(pos.y == tableau_Pos_Y[0])
                {pos.y = tableau_Pos_Y[1];}
                else if (pos.y == tableau_Pos_Y[1])
                {pos.y = tableau_Pos_Y[2];}
                else if (pos.y == tableau_Pos_Y[2])
                {pos.y = tableau_Pos_Y[0];}
            }
            ImgSetPosition(i_Cursor, pos);

            if (KeyGetOneShot(SDLK_RETURN))
            {
                if(pos.y == tableau_Pos_Y[0])
                {
                    TwoPlayers a;
                    a.Init();
                    a.Draw();

                    Perso perso;
                    perso.Init("data/", 0);
                    perso.name = TextGetText(a.pseudo[0]);
                    players.push_back(perso);

                    perso.Init("data/", 1);
                    perso.name = TextGetText(a.pseudo[1]);
                    players.push_back(perso);

                    SetMap(Rand(1, 3), players);

                    RefreshScreen();
                }
                else if(pos.y == tableau_Pos_Y[1])
                {
                    ThreePlayers b;
                    b.Init();
                    b.Draw();

                    Perso perso;
                    perso.Init("data/", 0);
                    perso.name = TextGetText(b.pseudo[0]);
                    players.push_back(perso);

                    perso.Init("data/", 1);
                    perso.name = TextGetText(b.pseudo[1]);
                    players.push_back(perso);

                    perso.Init("data/", 2);
                    perso.name = TextGetText(b.pseudo[2]);
                    players.push_back(perso);

                    SetMap(Rand(1, 3), players);

                    RefreshScreen();
                }
                else if (pos.y == tableau_Pos_Y[2])
                {
                    FourPlayers c;
                    c.Init();
                    c.Draw();

                    Perso perso;
                    perso.Init("data/", 0);
                    perso.name = TextGetText(c.pseudo[0]);
                    players.push_back(perso);

                    perso.Init("data/", 1);
                    perso.name = TextGetText(c.pseudo[1]);
                    players.push_back(perso);

                    perso.Init("data/", 2);
                    perso.name = TextGetText(c.pseudo[2]);
                    players.push_back(perso);

                    perso.Init("data/", 3);
                    perso.name = TextGetText(c.pseudo[3]);
                    players.push_back(perso);

                    SetMap(Rand(1, 3), players);

                    RefreshScreen();
                }
            }

            if (KeyGetOneShot(SDLK_BACKSPACE))
            {return 0;}

            DrawImage(i_Background);
            DrawImage(i_Cursor);
            RefreshScreen();
        }

        return 0;

        RefreshScreen();

    }


};

#endif // MENU_SELECT_JOUEURS_H_INCLUDED
