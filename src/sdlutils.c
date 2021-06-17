#include <SDL_image.h>

#include "./sdlutils.h"


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
