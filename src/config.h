#ifndef CONFIG_H
#define CONFIG_H
#define TRUE 1
#define FALSE 0
/* Tweak the settings below and recompile (make clean; make) to apply them. */ 
#define TICK_RATE 60 /* affects number of updates per second (game speed); 60 is default */
#define FRAME_SKIP 5
#define LIMIT_FPS TRUE
#define FPS_CAP 60 /* affects how many frames will be drawn per second; independent of game speed. */
#define WIDTH 600 /* width of the window in pixels */
#define HEIGHT 800 /* height of the window in pixels */
#define SDL_FLAGS (0 | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN)
#define TITLE "Bullet Hell Game"
#define SHOW_HIT_BOXES TRUE
#define SHOW_FPS TRUE /* print fps to stdout */
#define POOL_SIZE 256
#endif
