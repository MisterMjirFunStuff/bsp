#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_PERCENT .20
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

/*
 * Globals
 */
int running = 1;

/*
 * SDL stuff
 */
SDL_Window   *window;
SDL_Renderer *renderer;

/*
 * Procedurally generate rooms with BSP
 */
void bsp(int min_x, int min_y, int max_x, int max_y, int depth)
{
  if (depth == 0) {
    return;
  }

  int w, h, mid_x, mid_y, x, yl, yr;
  int offset_x, offset_y;
  
  w = max_x - min_x;
  h = max_y - min_y;

  mid_x = min_x + (w / 2);
  mid_y = min_y + (h / 2);

  offset_x = w * RANDOM_PERCENT;
  offset_y = h * RANDOM_PERCENT;

  offset_x = offset_x > 0 ? offset_x : 1 ;
  offset_y = offset_y > 0 ? offset_y : 1 ;

  x = (mid_x - offset_x) + (rand() % offset_x);
  yl = (mid_y - offset_y) + (rand() % offset_y);
  yr = (mid_y - offset_y) + (rand() % offset_y);

  /* Draw the sections */
  SDL_RenderDrawLine(renderer, x,     min_y, x,     max_y);
  SDL_RenderDrawLine(renderer, min_x, yl,    x,     yl);
  SDL_RenderDrawLine(renderer, x,     yr,    max_x, yr);

  /* tl, tr, br, bl */
  bsp(min_x, min_y, x,     yl,    depth - 1);
  bsp(x,     min_y, max_x, yr,    depth - 1);
  bsp(x,     yr,    max_x, max_y, depth - 1);
  bsp(min_x, yl,    x,     max_y, depth - 1);
}

/* Input */
void input(void)
{
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        running = 0;
        break;
    }
  }
}

int main(int argc, char *args[])
{
  SDL_Init(SDL_INIT_EVERYTHING);

  srand(time(NULL));

  window = SDL_CreateWindow("BSP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 24, 64, 255, 255);
  bsp(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  
  SDL_RenderPresent(renderer);

  while (running) {
    input();
  }

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
