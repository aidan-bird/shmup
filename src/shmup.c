#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <string.h>

#include "./sdlutils.h" 
#include "./config.h" 
#include "./content.h" 
#include "./assets.h" 

#define TICK_TIME (1000 / TICK_RATE)
#define FPS_CAP_FRAME_TIME (1000 / FPS_CAP)

static int isGameRunning;

void
termhandler(int signum)
{
    puts("Got signal; terminating!");
    isGameRunning = 0;
}

void
gameLoop()
{
    int frames;
    int ticks;
    int lastTime;
    int skipedFrames;
#if LIMIT_FPS == TRUE
    int updateStartTime;
    int updateEndTime;
#endif

    frames = 0;
    ticks = 0;
    skipedFrames = 0;
    lastTime = SDL_GetTicks();
#if LIMIT_FPS == TRUE
    updateStartTime = 0;
    updateEndTime = 0;
#endif
    while (isGameRunning) {
        skipedFrames = 0;
#if LIMIT_FPS == TRUE
        updateStartTime = SDL_GetTicks();
#endif
        while (SDL_GetTicks() > lastTime && skipedFrames < FRAME_SKIP) {
            // update();
            lastTime += TICK_TIME;
            skipedFrames++;
            ticks++;
            if (ticks > TICK_RATE) {
#if SHOW_FPS == TRUE
                printf("fps %d, tps %d\n", frames, ticks);
#endif
                ticks = 0;
                frames = 0;
            }
        }
        frames++;
        // drawGame(renderer);
#if LIMIT_FPS == TRUE
        updateEndTime = SDL_GetTicks();
        if (updateEndTime - updateStartTime < FPS_CAP_FRAME_TIME)
            SDL_Delay(FPS_CAP_FRAME_TIME - (updateEndTime - updateStartTime));
#endif
    }
}

int
main(int argc, char **argv)
{
    SDL_Renderer *renderer;
    SDL_Window *window;

    isGameRunning = 1;
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
    if (!(window = windowOpen(TITLE, WIDTH, HEIGHT, SDL_FLAGS)))
        goto error0;
    if (!(renderer = rendererOpen(window)))
        goto error1;
    // gameLoop();

    AssetTable test;

    test = (AssetTable) {
        .init = initSpriteAssetTable,
        .destroy = destroySpriteAssetTable,
        .loader = (AssetLoader) {
            .load = spriteLoaderFunc,
            .sprite = {
                .renderer = renderer,
            },
        }
    };
    loadAllFromAssetDefTab(&test, sprites, spriteCount);

    SDL_Rect src = { .w = 32, .h = 32, .x = 0, .y = 0 };
    SDL_Rect dest = { .w = 32, .h = 32, .x = WIDTH / 2, .y = HEIGHT / 2 };

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderCopy(renderer, test.assets[debugred], &src, &dest);
    SDL_RenderPresent(renderer);

    test.destroy(&test);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
error3:;
    destroySpriteTable(&test);
error2:;
    SDL_DestroyRenderer(renderer);
error1:;
    SDL_DestroyWindow(window);
error0:;
    SDL_Quit();
    return -1;
}
