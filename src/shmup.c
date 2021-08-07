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
#include "./utils.h" 
#include "./collision.h" 
#include "./stage.h" 
#include "./animation.h" 
#include "./behaviour.h"

typedef void (*GameUpdateFunc)();
typedef void (*GameDrawFunc)();

/* time in milliseconds that each tick lasts */
#define TICK_TIME (1000 / (TICK_RATE))

/* time in milliseconds that each frame lasts */
#define FPS_CAP_FRAME_TIME (1000 / (FPS_CAP))

static int isGameRunning;
static AssetTable spriteTest;
static SDL_Renderer *renderer;
static SDL_Window *window;
static SDL_Event events;
static char isRight;
static char isLeft;
static char isDown;
static char isUp;
static char isX;
static char isEnter;
static char isSpace;
static char isEscape;

static float playerX;
static float playerY;
static float playerVelocityX;
static float playerVelocityY;
static char playerVelocityXScale;
static char playerVelocityYScale;
static float playerVelocityCorrection;
static SpriteAssetKeys playerSprite;

static GameUpdateFunc update;
static GameDrawFunc draw;


/* XXX testing the behaviour system */
static EntityBehaviourManager *testBeh;

/* XXX testing the animation system */
static AssetTable animTab;
static Animator *testAnimator;

/* XXX testing the collision detection system */
static CircleCollider *testCollider;

/* XXX testing the event system */
static EventManager *onCollisionTestEvent;

/*
 * REQUIRES
 * events are handled before calling this.
 *
 * MODIFIES
 *
 * EFFECTS
 * updates the game every tick.
 */
void
updateGame()
{
    /* XXX testing the collision detection system + the event system */
    short colliderKey;

    /* update player */
    playerVelocityXScale = 0;
    playerVelocityYScale = 0;
    if (isUp)
        playerVelocityYScale--;
    if (isDown)
        playerVelocityYScale++;
    if (isLeft)
        playerVelocityXScale--;
    if (isRight)
        playerVelocityXScale++;
    playerVelocityCorrection = (playerVelocityXScale & 1) 
        + (playerVelocityYScale & 1) > 1 ?  ROOT1_2F : 1;
    playerVelocityX = PLAYER_MAX_SPEED * playerVelocityXScale 
        * playerVelocityCorrection;
    playerVelocityY = PLAYER_MAX_SPEED * playerVelocityYScale 
        * playerVelocityCorrection;
    playerX += playerVelocityX;
    playerY += playerVelocityY;

    /* update entity pools */
    updateEntityPool(&playerBullets);

    /* XXX testing the collision detection system + the event system */
    colliderKey = testCircCollider(playerX, playerY, 32, testCollider);
    if (colliderKey >= 0) {
        /* raise onCollision Event */
        unsigned short j;

        j = colliderKey;
        raiseEvent(onCollisionTestEvent, &j);
    }

    /* XXX test behaviour system */
    updateEntityBehaviourManager(testBeh);

    /* XXX test animation system */
    updateAnimator(testAnimator);
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * draws the game every frame.
 *
 */
void
drawGame()
{
    drawSprite(renderer, &spriteTest, debugred, playerX, playerY, 0, 0);
    drawAnimator(renderer, testAnimator);
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
            /* handle events */
            if (SDL_PollEvent(&events)) {
                switch (events.type) {
                    case SDL_QUIT:
                        return;
                    case SDL_KEYDOWN:
                        switch (events.key.keysym.sym) {
                            case SDLK_RIGHT:
                            case SDLK_d:
                                isRight = 1;
                                break;
                            case SDLK_LEFT:
                            case SDLK_a:
                                isLeft = 1;
                                break;
                            case SDLK_DOWN:
                            case SDLK_s:
                                isDown = 1;
                                break;
                            case SDLK_UP:
                            case SDLK_w:
                                isUp = 1;
                                break;
                            case SDLK_x:
                                isX = 1;
                                break;
                            case SDLK_RETURN:
                                isEnter = 1;
                                break;
                            case SDLK_SPACE:
                                isSpace = 1;
                                break;
                            case SDLK_ESCAPE:
                                isEscape = 1;
                                break;
                        }
                        break;
                    case SDL_KEYUP:
                        switch (events.key.keysym.sym) {
                            case SDLK_RIGHT:
                            case SDLK_d:
                                isRight = 0;
                                break;
                            case SDLK_LEFT:
                            case SDLK_a:
                                isLeft = 0;
                                break;
                            case SDLK_DOWN:
                            case SDLK_s:
                                isDown = 0;
                                break;
                            case SDLK_UP:
                            case SDLK_w:
                                isUp = 0;
                                break;
                            case SDLK_x:
                                isX = 0;
                                break;
                            case SDLK_RETURN:
                                isEnter = 0;
                                break;
                            case SDLK_SPACE:
                                isSpace = 0;
                                break;
                            case SDLK_ESCAPE:
                                isEscape = 0;
                                break;
                        }
                        break;
                }
            }
            update();
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
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        draw();
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
        .destroy = destroySpriteAssetTable,
        .loader = (AssetLoader) {
            .load = spriteLoaderFunc,
            .sprite = {
                .renderer = renderer,
            },
        }
    };
    if (loadAllFromAssetDefTab(&spriteTest, &spritesheet))
        goto error2;
    /* XXX testing the event system */
    addOnSpawnEntityEventManager(&playerBullets);

    /* XXX testing the animation system */
    animTab = (AssetTable) {
        .destroy = destroyAnimAssetTable,
        .loader = (AssetLoader) {
            .load = animLoaderFunc,
            .sprite = {
                .renderer = renderer,
            },
        }
    };
    loadAllFromAssetDefTab(&animTab, &animSpriteSheet);
    testAnimator = newAnimator(&playerBullets, &animTab);
    testCollider = newCircCollider(&playerBullets);
    /* XXX testing the behaviour system */
    SubsystemsList subsystemsTest = (SubsystemsList) {
        .debug = {
            .animator = testAnimator,
            .collider = testCollider,
        },
    };
    testBeh = newEntityBehaviourManager(&playerBullets, &subsystemsTest);
    unsigned short key;
    key = spawnEntity(&playerBullets);
    playerBullets.x[key] = 256;
    playerBullets.y[key] = 256;
    testBeh->behaviourKey[key] = debugbotStart;
    onCollisionTestEvent = newEventManager(NULL, "onCollisionTestEvent");
    subscribeToEventManager(onCollisionTestEvent, testBeh,
            onCollisionTestEvent_EntityBehaviourManager,
            "onCollisionTestEvent_EntityBehaviourManager");
    updateActiveIndexMap(&playerBullets);

    /* start game */
    isGameRunning = 1;
    update = updateGame;
    draw = drawGame;
    playerSprite = debugred;
    gameLoop();
    /* cleanup */

    /* XXX */
    animTab.destroy(&animTab);
    deleteAnimator(testAnimator);
    deleteEntityBehaviourManager(testBeh);
    deleteEventManager(onCollisionTestEvent);
    deleteCircCollider(testCollider);

    spriteTest.destroy(&spriteTest);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
error3:;
    spriteTest.destroy(&spriteTest);
error2:;
    SDL_DestroyRenderer(renderer);
error1:;
    SDL_DestroyWindow(window);
error0:;
    SDL_Quit();
    return -1;
}
