#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <stdint.h>

#include "./entity.h"

typedef struct KinematicsManager KinematicsManager;

struct KinematicsManager
{
    EntityPool *pool;
    float *speed;
    float *rot;
    float *accel;
    float *angularVelocity;
    float *rotCacheX;
    float *rotCacheY;
    uint8_t *isUpdateRot;
};

KinematicsManager *newKinematicsManager(EntityPool *pool);
void updateKinematicsManager(KinematicsManager *mgr);
void deleteKinematicsManager(KinematicsManager *mgr);

#endif
