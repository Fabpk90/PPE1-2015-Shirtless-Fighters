#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED
#include "graphic.h"
#include "util/gameConst.h"
#include "Perso.h"

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <vector>


void updatePseudo(std::vector<Perso> players);
void SetMap(int numMap, std::vector<Perso> players);
void endMatch(std::vector<Perso> players);
int checkForWinner(std::vector<Perso> players);


#endif // MAP_H_INCLUDED
