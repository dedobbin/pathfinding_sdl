#include <iostream>
#include "graphics.h"
#include <ctime>

int main(int argv, char* argc[]) {
  Graphics::Controller * controller = NULL;
  controller = new Graphics::Controller(640, 480);
  clock_t begin = clock();
  Graph* g = new Graph(20);
  double graphTime = double(clock() - begin) / CLOCKS_PER_SEC;
  std::cout << "Took " << graphTime << " second to create graph." << std::endl;;

  begin = clock();
  Path* p = AStar(g, g->points.at(0), g->points.at(g->points.size() - 1));
  double calcTime = double(clock() - begin) / CLOCKS_PER_SEC;
  std::cout << "Took " << calcTime << " seconds to find path." << std::endl;
    
  controller->addContent(g);
  controller->addContent(p);
  

  SDL_Event e;
  bool quit = false;
  while (!quit){
    clock_t begin = clock();
    while (SDL_PollEvent(&e) != 0){
      if (e.type == SDL_QUIT){
        quit = true;
      }
    }
    controller->draw();
    double drawTime = double(clock() - begin) / CLOCKS_PER_SEC;
  }
  
  delete(controller);
  return 0;

}