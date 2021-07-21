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
#include "./entity.h" 


/* time in milliseconds that each tick lasts */
#define TICK_TIME (1000 / (TICK_RATE))

/* time in milliseconds that each frame lasts */
#define FPS_CAP_FRAME_TIME (1000 / (FPS_CAP))

static int isGameRunning;
static int isSpriteTestLoaded;
static AssetTable spriteTest;
static SDL_Renderer *renderer;
static SDL_Window *window;

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * returns the debug asset table.
 * returns null on error.
 */
const AssetTable*
getDebugAssetTable()
{
    if (isSpriteTestLoaded) {
        return &spriteTest;
    } else {
        puts("debug sprite table is not loaded!");
        return NULL;
    }
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * isGameRunning
 *
 * EFFECTS
 * called when the program receives SIGTERM or SIGINT.
 * terminates the gameloop.
 */
void
termhandler(int signum)
{
    puts("Got signal; terminating!");
    isGameRunning = 0;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * calls update() every tick and draw() every frame when the game is running.
 * the gameloop terminates if isGameRunning is zero.
 */
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

        SDL_Rect src = { .w = 32, .h = 32, .x = 0, .y = 0 };
        SDL_Rect dest = { .w = 32, .h = 32, .x = WIDTH / 2, .y = HEIGHT / 2 };
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderCopy(renderer, spriteTest.assets[debugred], &src, &dest);


        /* XXX debug. test entity pool */
        drawEntityPool(renderer, &playerBullets);


        // drawGame(renderer);
        SDL_RenderPresent(renderer);
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
    /* setup signal handlers */
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
    /* setup sdl */
    if (!(window = windowOpen(TITLE, WIDTH, HEIGHT, SDL_FLAGS)))
        goto error0;
    if (!(renderer = rendererOpen(window)))
        goto error1;
    /* load assets */
    spriteTest = (AssetTable) {
        .init = initSpriteAssetTable,
        .destroy = destroySpriteAssetTable,
        .loader = (AssetLoader) {
            .load = spriteLoaderFunc,
            .sprite = {
                .renderer = renderer,
            },
        }
    };
    if (loadAllFromAssetDefTab(&spriteTest, sprites, spriteCount))
        goto error2;
    isSpriteTestLoaded = 1;


    /* XXX debug. test entity pool */
    unsigned short key;
    key = spawnEntity(&playerBullets);
    playerBullets.x[key] = WIDTH / 2;
    playerBullets.y[key] = 55;
    playerBullets.aabbWidth[key] = 32;
    playerBullets.aabbHeight[key] = 32;
    updateActiveIndexMap(&playerBullets);

    /* start game */
    isGameRunning = 1;
    gameLoop();
    /* cleanup */
    spriteTest.destroy(&spriteTest);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
error3:;
    spriteTest.destroy(&spriteTest);
    // destroySpriteTable(&spriteTest);
error2:;
    SDL_DestroyRenderer(renderer);
error1:;
    SDL_DestroyWindow(window);
error0:;
    SDL_Quit();
    return -1;
}
