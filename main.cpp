#include <iostream>
#include <ctime>
#include <string>   

#include "graphics.h"

int main(int argc, char* argv[]) {
  int screenWidth, screenHeight, nNodes;
  if (argc < 4){
	  std::cout << "Args are screen_width, screen_height, num_nodes" << std::endl;
	  screenWidth = 640;
	  screenHeight = 480;
	  nNodes = 100;
  } else {
	screenWidth = std::stoi(argv[1]);
	screenHeight = std::stoi(argv[2]);
	nNodes = std::stoi(argv[3]);
  }

  std::cout << "Window size: " << screenWidth << "x" << screenHeight << ", " << nNodes << " nodes" << std::endl;

  Graphics::Controller * controller = NULL;
  controller = new Graphics::Controller(screenWidth, screenHeight);
  clock_t begin = clock();
  Graph* g = new Graph(nNodes, screenWidth, screenHeight);
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