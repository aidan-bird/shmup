#ifndef CONFIG_H
#define CONFIG_H
#define TRUE 1
#define FALSE 0
/* Tweak the settings below and recompile (make clean; make) to apply them. */ 

/* affects number of updates per second (game speed); 60 is default */
#define TICK_RATE 60 

#define FRAME_SKIP 5

#define LIMIT_FPS TRUE

 /* affects how many frames will be drawn per second; independent of game speed. */
#define FPS_CAP 60

/* width of the window in pixels */
#define WIDTH 600 

/* height of the window in pixels */
#define HEIGHT 800 

#define SDL_FLAGS (0 | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN)

#define TITLE "Bullet Hell Game"

#define SHOW_HIT_BOXES TRUE

/* print fps to stdout */
#define SHOW_FPS TRUE 

/* the capacity of the object pools */
#define POOL_SIZE 256

/* max speed of player in pixels/tick */
#define PLAYER_MAX_SPEED 8

#endif
