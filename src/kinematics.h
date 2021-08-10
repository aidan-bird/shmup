#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "./entity.h"

typedef struct KinematicsManager KinematicsManager;

struct KinematicsManager
{
    EntityPoolRef poolRef;
    float *speed;
    float *rot;
    float *accel;
    float *angularVelocity;
    float *rotCacheX;
    float *rotCacheY;
    /* TODO add an event manager that will set this to 1 on spawn event */
    char *isUpdateRot;
};

KinematicsManager *newKinematicsManager(EntityPool *pool);
void updateKinematicsManager(KinematicsManager *mgr);
void deleteKinematicsManager(KinematicsManager *mgr);

#endif
