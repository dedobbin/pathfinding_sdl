#ifndef __GRAPHICS__
#define __GRAPHICS__

#include <SDL.h>
#include <string>
#include <vector>

#include "graph.h"

namespace Graphics {
  class Controller {
  public:
    const int WIDTH;
    const int HEIGHT;
    Controller(int w = 700, int h = 700);
    ~Controller();
    int addContent(Drawable* drawable);
    void draw();
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::string last_error;
    std::vector<Drawable*> content;
  };
}
#endif