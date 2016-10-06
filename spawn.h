#ifndef SPAWN_H_INCLUDED
#define SPAWN_H_INCLUDED

#include "../graphic.h"
#include <vector>

Vec2 getRandomSpawnPoint(std::vector<Vec2> respawnPoint, std::vector<unsigned int> *respawnPointTimer);
std::vector<int> getFreeSpawnPoints(std::vector<unsigned int> *respawnPointTimer);

void checkSpawnPoints(std::vector<unsigned int> *respawnPointTimer);

#endif // SPAWN_H_INCLUDED
