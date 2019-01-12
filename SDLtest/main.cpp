#include <iostream>
#include "graphics.h"
#include <ctime>

int main(int argv, char* argc[]) {
  Graphics::Controller * controller = NULL;
  try {
    controller = new Graphics::Controller(700, 700);
    Graph* g = new Graph(100);
    Path* p = AStar(g, g->points.at(0), g->points.at(g->points.size() - 1));
    controller->addContent(g);
    controller->addContent(p);
  }
  catch (std::runtime_error e) {
    std::cout << e.what() << std::endl;
  }
  SDL_Event e;

  
  bool quit = false;
  while (!quit){
    clock_t begin = clock();
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
    }
    controller->draw();
    double timeSpend = double(clock() - begin) / CLOCKS_PER_SEC;
    std::cout << "Draw time:" << timeSpend << std::endl;
  }
  
  delete(controller);
  return 0;

}