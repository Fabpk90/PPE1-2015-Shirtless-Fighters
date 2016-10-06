#include <stdio.h>
#include <stdlib.h>


#include "graphic.h"
#include "Menu.h"



int main(int argc, char *argv[])
{
    //création de la fenetre et initialisation des variable
    CreateWindow(1280, 720, "Game", true);

    MainMenu menu;
    menu.Init();
    menu.Draw();

    return 0;
}
