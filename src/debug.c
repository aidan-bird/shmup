#include <stdio.h>

#include "./debug.h"

int spawnedObjectsCount = 0;
int despawnedObjectsCount = 0;

void
printDebugStatus()
{
    printf("spawned %d\ndespawned: %d\n", spawnedObjectsCount,
        despawnedObjectsCount);
}
