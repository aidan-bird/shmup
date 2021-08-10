#include <SDL_image.h>

#include "./sdlutils.h"
#include "./utils.h"

#define MAX_RADIUS (1 << 8)

/*
 * REQUIRES
 * renderer is valid
 * path is valid
 *
 * MODIFIES
 * renderer
 *
 * EFFECTS
 * loads a texture at path and returns it
 */
SDL_Texture *
loadImage(SDL_Renderer *renderer, const char *path)
{
    SDL_Texture *tex;
    SDL_Surface *tmpSurf;
    
    tmpSurf = IMG_Load(path);
    if (!tmpSurf)
        return NULL;
    tex = SDL_CreateTextureFromSurface(renderer, tmpSurf);
    if (!tex)
        return NULL;
    SDL_FreeSurface(tmpSurf);
    return tex;
}

/*
 * REQUIRES
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * opens a new window and returns it
 */
SDL_Window *
windowOpen(const char *name, size_t width, size_t height, int flags)
{
    if (!SDL_Init(SDL_INIT_EVERYTHING))
        return SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    return NULL;
}

/*
 * REQUIRES
 * window is valid
 *
 * MODIFIES
 * window
 *
 * EFFECTS
 * attaches a new render to window and returns it
 */
SDL_Renderer *
rendererOpen(SDL_Window *window)
{
    return SDL_CreateRenderer(window, -1, 0);
}

/*
 * REQUIRES
 *
 * MODIFIES
 *
 * EFFECTS
 *
 */
void
drawCircle(SDL_Renderer *renderer, int x, int y, int r)
{
    size_t l;
    int endpt;
    int offsetY;
    static SDL_Point points[4 * MAX_RADIUS];
    /* 
     * midpoint circle algorithm
     * based on "https://web.archive.org/web/20120422045142/https://banu.com/blog/7/drawing-circles/"
     */
    endpt = r * ROOT1_2F;
    l = 0;
    for (int offsetX = 0; offsetX <= endpt; offsetX++) {
        offsetY = sqrtf(SQUARE(r) - SQUARE(offsetX));
        points[l] = (SDL_Point) {
            .x = x + offsetX,
            .y = y + offsetY,
        };
        points[l + 1] = (SDL_Point) {
            .x = x + offsetX,
            .y = y + -offsetY,
        };
        points[l + 2] = (SDL_Point) {
            .x = x + -offsetX,
            .y = y + offsetY,
        };
        points[l + 3] = (SDL_Point) {
            .x = x + -offsetX,
            .y = y + -offsetY,
        };
        points[l + 4] = (SDL_Point) {
            .x = x + offsetY,
            .y = y + offsetX,
        };
        points[l + 5] = (SDL_Point) {
            .x = x + offsetY,
            .y = y + -offsetX,
        };
        points[l + 6] = (SDL_Point) {
            .x = x + -offsetY,
            .y = y + offsetX,
        };
        points[l + 7] = (SDL_Point) {
            .x = x + -offsetY,
            .y = y + -offsetX,
        };
        l += 8;
    }
    SDL_RenderDrawPoints(renderer, points, l);
}
