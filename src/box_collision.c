#include <stdlib.h>
#include <stdio.h>

#include "./box_collision.h"
#include "./entity.h"

BoxCollider *
newBoxCollider(EntityPool *pool)
{
    size_t vecSize;
    size_t n;
    BoxCollider *ret;

    n = pool->count;
    vecSize = 2 * sizeof(uint8_t);
    ret = malloc(sizeof(BoxCollider) + vecSize * n);
    if (!ret)
        return NULL;
    ret->pool = pool;
    ret->width = (uint8_t *)ret + sizeof(BoxCollider);
    ret->height = (uint8_t *)ret->width + n * sizeof(uint8_t);
    return ret;
}

void
deleteBoxCollider(BoxCollider *collider)
{
    free(collider);
}

int32_t
testBoxCollider(float x, float y, const BoxCollider *collider)
{
    size_t j;
    float x2;
    float y2;
    float width2;
    float height2;
    EntityPoolIndexList list;

    list = getEntityPoolActiveIndexList(collider->pool);
    for (size_t i = 0; i < list.n; i++) {
        j = list.keys[i];
        x2 = collider->pool->x[j];
        y2 = collider->pool->y[j];
        width2 = collider->width[j];
        height2 = collider->height[j];
        if (x >= x2 && x <= x2 + width2 && y >= y2 && y <= y2 + height2) {
            puts("box collision detected!");
            return j;
        }
    }
    return -1;
}

void
drawBoxCollider(SDL_Renderer *renderer, const BoxCollider *collider)
{
    SDL_Rect rect;
    size_t j;
    EntityPoolIndexList list;

    list = getEntityPoolActiveIndexList(collider->pool);
    for (size_t i = 0; i < list.n; i++) {
        j = list.keys[i];
        rect = (SDL_Rect) {
            .x = collider->pool->x[j],
            .y = collider->pool->y[j],
            .w = collider->width[j],
            .h = collider->height[j],
        };
        SDL_RenderDrawRect(renderer, &rect);
    }
}
