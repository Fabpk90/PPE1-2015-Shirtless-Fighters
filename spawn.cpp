#include "spawn.h"

Vec2 getRandomSpawnPoint(std::vector<Vec2> respawnPoint, std::vector<unsigned int> *respawnPointTimer)
{
    //choose a random spawn point, add a controller to spawn in a free point

    /*
        TODO: find a way to make the player wait for a free spawn point without while
    */

    //bool ok = false;

    //first i get the freepoints, then occupy the spawnpoint and then spawn

    std::vector<int> freepoints = getFreeSpawnPoints(respawnPointTimer);

    int index = Rand(0, freepoints.size());

    respawnPointTimer->at(index) = Time() + 500;

    //int index = Rand(0, respawnPoint.size());
    return respawnPoint.at(freepoints.at(index));
    /*while(!ok)
    {
        //if the point is free
        if(respawnPointTimer->at(index) == 0)
        {
            respawnPointTimer->at(index) = Time() + 500;
            return respawnPoint.at(index);
        }

        index = Rand(0, respawnPoint.size());
    }*/
}

std::vector<int> getFreeSpawnPoints(std::vector<unsigned int> *respawnPointTimer)
{
    std::vector<int> freepoints;
    for(int i = 0; i < respawnPointTimer->size(); i++)
    {
        if(respawnPointTimer->at(i) == 0)
            freepoints.push_back(i);
    }

    return freepoints;
}

void checkSpawnPoints(std::vector<unsigned int> *respawnPointTimer)
{
    for(int i = 0; i < respawnPointTimer->size(); i++)
    {
        //if the point is free(nobody is there)
        if(respawnPointTimer->at(i) <= Time())
        {
            respawnPointTimer->at(i) = 0;
        }
    }
}
