#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "Menu_Select_Joueurs.h"
#include "Credits.h"
#include "controls.h"
#include "graphic.h"


struct MainMenu{

    Image i_Background;
    Image i_Cursor;
    Vec2 pos;   //position cursor

    int tableau_Pos_Y[5];

    FILE* file_NbMorts = NULL;
    Text txt_Morts;
    Color red;
    Vec2 pos_TxtMorts;
    int nbKill;

    void Init()
    {
        tableau_Pos_Y[0] = 473;     //jouer
        tableau_Pos_Y[1] = 365;     //controls
        tableau_Pos_Y[2] = 260;     //score
        tableau_Pos_Y[3] = 170;     //credits
        tableau_Pos_Y[4] = 67;      //quitter

        i_Background = CreateImage("data/backgrounds_menu/1.png");

        i_Cursor = CreateImage("data/cursor.ico");
        ImgSetRotation(i_Cursor, 224);
        pos = CreateVec2(990, tableau_Pos_Y[0]);
        ImgSetPosition(i_Cursor, pos);

        //ouverture & lecture du fichier
        file_NbMorts = fopen("data/nbMorts.txt", "r");
        pos_TxtMorts = CreateVec2(460,200);
        red = CreateColor(255,0,0);
        txt_Morts = CreateText("", red, 55);
        fscanf(file_NbMorts,"%d", &nbKill);


        if (file_NbMorts == NULL)
        {
            TextSetText(txt_Morts, "ERREUR: fichier inéxistant");
            TextSetPosition(txt_Morts, pos_TxtMorts);
            TextSetSize(txt_Morts, 20);
        }
        else
        {
            TextSetText(txt_Morts,"%d", nbKill);
            TextSetPosition(txt_Morts, pos_TxtMorts);
        }
    }

    int Draw()
    {
        while (!KeyGetOneShot(SDLK_ESCAPE) && !StickGetClick(0, 7))
        {
            //gestion de la position de Cursor
            if (KeyGetOneShot(SDLK_UP))
            {
                if(pos.y == tableau_Pos_Y[0])
                {pos.y = tableau_Pos_Y[4];}
                else if (pos.y == tableau_Pos_Y[1])
                {pos.y = tableau_Pos_Y[0];}
                else if (pos.y == tableau_Pos_Y[2])
                {pos.y = tableau_Pos_Y[1];}
                else if (pos.y == tableau_Pos_Y[3])
                {pos.y = tableau_Pos_Y[2];}
                else if (pos.y == tableau_Pos_Y[4])
                {pos.y = tableau_Pos_Y[3];}
            }
            if (KeyGetOneShot(SDLK_DOWN))
            {
                if(pos.y == tableau_Pos_Y[0])
                {pos.y = tableau_Pos_Y[1];}
                else if (pos.y == tableau_Pos_Y[1])
                {pos.y = tableau_Pos_Y[2];}
                else if (pos.y == tableau_Pos_Y[2])
                {pos.y = tableau_Pos_Y[3];}
                else if (pos.y == tableau_Pos_Y[3])
                {pos.y = tableau_Pos_Y[4];}
                else if (pos.y == tableau_Pos_Y[4])
                {pos.y = tableau_Pos_Y[0];}
            }

            ImgSetPosition(i_Cursor, pos);

            if (KeyGetOneShot(SDLK_RETURN))
            {
                //enter sur PLAY
                if(pos.y == tableau_Pos_Y[0])
                {
                    MenuSelectNumberOfPlayers m;
                    m.Init();
                    RefreshScreen();
                    m.Draw();
                }
                //enter sur CONTROLS
                else if(pos.y == tableau_Pos_Y[1])
                {
                    Controls ctrl;
                    ctrl.Init();
                    ctrl.Draw();
                }
                //enter sur SCORE
                else if (pos.y == tableau_Pos_Y[2])
                {system("start http://www.google.com");}
                //enter sur CREDITS
                else if(pos.y == tableau_Pos_Y[3])
                {
                    MenuCredits mc;
                    mc.Init();
                    RefreshScreen();
                    mc.Draw();
                }
                //entrer sur QUITTER
                else if(pos.y == tableau_Pos_Y[4])
                {return 0;}
            }

            DrawImage(i_Background);
            DrawText(txt_Morts);
            DrawImage(i_Cursor);
            RefreshScreen();
        }
    }
};



#endif // MENU_H_INCLUDED




