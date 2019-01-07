#include "graphics.h"


Graphics::Controller::Controller(int w, int h) : WIDTH(w), HEIGHT(h) {
  char error_message[100];
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    sprintf_s(error_message, "SDL could not initialize, SDL Error: %s\n", SDL_GetError());
    throw std::runtime_error(error_message);
  }
  this->window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  if (this->window == NULL) {
    sprintf_s(error_message, "Window could not be created, SDL Error: %s\n", SDL_GetError());
    throw std::runtime_error(error_message);
  }
  this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
  if (this->renderer == NULL) {
    sprintf_s(error_message, "Renderer could not be created SDL Error: %s\n", SDL_GetError());
    throw std::runtime_error(error_message);
  }
}

Graphics::Controller::~Controller() {	
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
  for (auto it = content.begin(); it < content.end(); it++) {
    delete(*it);
  }
}

//returns idx of added content
int Graphics::Controller::addContent(Drawable* drawable) {
  content.push_back(drawable);
  return content.size() - 1;
}

void Graphics::Controller::draw() {
  //Clear screen
  SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(this->renderer);

  for (auto drawablePtr = this->content.begin(); drawablePtr < this->content.end(); drawablePtr++) {
    //todo: probably better to call drawLineS etc rather than call drawLine etc in loops
    switch ((*drawablePtr)->type) {
      case graphType:
      {
        Graph * g = (Graph*)*drawablePtr;
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0xFF);
        for (auto pointIt = g->points.begin(); pointIt != g->points.end(); pointIt++) {
          SDL_Rect rect;
          rect.x = pointIt->second->x - 2;
          rect.y = pointIt->second->y - 2;
          rect.w = 4;
          rect.h = 4;
          SDL_RenderFillRect(this->renderer, &rect);

          std::vector<Point*> connections = pointIt->second->connections;
          for (auto it = connections.begin(); it != connections.end(); it++) {
            SDL_RenderDrawLine(this->renderer, pointIt->second->x, pointIt->second->y, (*it)->x, (*it)->y);
          }
        }
        break;
      }
      case pathType:
      {
        Path * path = (Path*)*drawablePtr;
        auto it = path->list.begin();
        SDL_Rect startRect = { path->list.at(0)->x - 2, path->list.at(0)->y - 2, 4, 4 };
        SDL_Rect endRect = { path->list.at(path->list.size()-1)->x - 2, path->list.at(path->list.size()-1)->y - 2, 4, 4 };
        SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x00, 0xFF, 0xFF);
        SDL_RenderFillRect(this->renderer, &endRect);
        SDL_RenderFillRect(this->renderer, &startRect);
        for (auto it = path->list.begin(); it != path->list.end() - 1; it++){
          auto p1 = *it;
          auto p2 = *(std::next(it, 1));
          SDL_RenderDrawLine(this->renderer, p1->x, p1->y, p2->x, p2->y);
        }
        break;
      }
    }
  }
  SDL_RenderPresent(this->renderer);
}
