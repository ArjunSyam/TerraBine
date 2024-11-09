#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

//screen sizes
#define SCREEN_WIDTH 680
#define SCREEN_HEIGHT 720

//text changes
#define TEXT_FONT "/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Regular.ttf"
#define TEXT_SIZE 12
#define FONT_COLOR {255,255,255,255}

//error macros
#define SDL2_ERROR printf("SDL2 Error: %s \n",SDL_GetError());
#define TTF_ERROR_SHOW printf("TTF Error: %s \n",TTF_GetError());

//error macro
//backlash is for line continuation
//best to wrap multiline macros in do { } while(0)
#define ERROR_CHECK(check_value,error_statment,error_type) \
do { \
  if (!check_value) { \
    printf("%s",error_statment); \
    error_type \
    SDL_Quit(); \
    TTF_Quit(); \
    return EXIT_FAILURE; \
 } \
} while(0);

int main(int argc, char *argv[]) {
  //initiatize SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Failed to initiatize the SDL2 library \n");
    SDL2_ERROR
    return EXIT_FAILURE;
  }

  //initiatize text
  if (TTF_Init() < 0) {
    printf("SDL_ttf could not be initialized \n");
    TTF_ERROR_SHOW 
    SDL_Quit();
    return EXIT_FAILURE;
  }
  
  //creates a window
  SDL_Window *window = SDL_CreateWindow(
    "TerraBine",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    680, 480,
    0);
  
  //check window presence
  ERROR_CHECK(window, "Failed to create window \n", SDL2_ERROR);

  //create a renderer
  //this is a struct that handles all rendering within the window
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  ERROR_CHECK(renderer,"Failed to create renderer \n",SDL2_ERROR)

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //black background
  SDL_RenderClear(renderer);

  TTF_Font *font = TTF_OpenFont(TEXT_FONT, TEXT_SIZE);
  ERROR_CHECK(font,"Failed to load font  \n",TTF_ERROR_SHOW)

  //creating a surface with rendered text
  //Surface is basically an image containing the rendered text 
  //Uses a CPU based Bit map
  SDL_Color textColor = FONT_COLOR;
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, ">>Skibbidi$ Hello World, Welcome to TerraBine", textColor);
  ERROR_CHECK(textSurface,"Failed to create text surface  \n",TTF_ERROR_SHOW)

  //create texture from surface
  //Surfaces are convereted into textures 
  //basically textures handle the images using the GPU which is faster
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  ERROR_CHECK(textTexture,"Failed to create text texture  \n",TTF_ERROR_SHOW)
  
  //creates a rectangle where the text will be drawn on
  SDL_Rect textRect = {0,20, textSurface->w, textSurface->h};
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  bool keep_window_open = true;
  while(keep_window_open) {
    
    //Gets all the events as a Queue
    SDL_Event e;
    //takes a event one at a time exits if there are no processes
    while(SDL_PollEvent(&e) > 0) {

      switch(e.type) {
        case SDL_QUIT:
          keep_window_open = false;
          break;
      }
    }

    // Clear screen and render text
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);  // Present the updated renderer
  }

  // Cleanup
  SDL_DestroyTexture(textTexture);
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
