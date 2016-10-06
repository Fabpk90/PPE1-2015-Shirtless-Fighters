#ifndef MENU_PSEUDO_2J_H_INCLUDED
#define MENU_PSEUDO_2J_H_INCLUDED
#include "Menu_Select_Joueurs.h"

#include <vector>

#include "MySQL.h"
#include "Map.h"
#include "Perso.h"


struct SelectionDeuxJoueurs{

    Image i_BackgroundJ1;
    Image i_BackgroundJ2;
    Text t_pseudoJ1;
    Text t_pseudoJ2;

    std::vector<Perso> players;

    void Init(std::vector<Perso> player)
    {
        players = player;

        i_BackgroundJ1 = CreateImage("data/backgrounds_menu/j1.png");
        i_BackgroundJ2 = CreateImage("data/backgrounds_menu/j2.png");

        Color textColor = CreateColor(255,0,0);

        t_pseudoJ1 = CreateText("écrire ici", textColor, 32);
        t_pseudoJ2 = t_pseudoJ1;

        TextSetAlterable(t_pseudoJ1, true);
        TextSetAlterable(t_pseudoJ2, true);

        Vec2 textPos = CreateVec2(460,200);
        TextSetPosition(t_pseudoJ1, textPos);
        TextSetPosition(t_pseudoJ2, textPos);
    }

    int Draw()
    {
        while (!KeyGetOneShot(SDLK_ESCAPE) && !StickGetClick(0, 7))
        {

                DrawImage(i_BackgroundJ1);
                DrawText(t_pseudoJ1);

                if(KeyGetOneShot(SDLK_RETURN))//enter pressed
                  {
                  // std::string str(TextGetText(t_pseudoJ1));
                    players[0].name = TextGetText(t_pseudoJ1);
                    players[1].name = TextGetText(t_pseudoJ1);


                    std::string query("INSERT INTO perso VALUES('");
                    query += players[0].name;
                    query += "')";

                    MySQLRequest("127.0.0.1", "root", "", "shirtless_fighters", (char*)query.c_str());

                    query = ("INSERT INTO perso VALUES('");
                    query += players[1].name;
                    query += "')";

                    MySQLRequest("127.0.0.1", "root", "", "shirtless_fighters", (char*)query.c_str());

                    SetMap(1, players);

                  }


            RefreshScreen();
        }
    }
};

#endif // MENU_PSEUDO_2J_H_INCLUDED
