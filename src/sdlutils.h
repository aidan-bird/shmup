#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <stdint.h>

#include <SDL.h>
#include <stddef.h>

SDL_Texture *loadImage(SDL_Renderer *renderer, const char *path);
SDL_Window *windowOpen(const char *name, size_t width, size_t height, int flags);
SDL_Renderer *rendererOpen(SDL_Window *window);
void drawCircle(SDL_Renderer *renderer, int x, int y, int r);
void drawCircles(SDL_Renderer *renderer, const int *x, const int *y, int r,
    size_t n);

#endif
