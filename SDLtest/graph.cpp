#include <iostream>
#include <sstream>
#include <algorithm>
#include <queue>
#include "graph.h"
#include "util.h"


typedef struct LineEquation {
  // actual equation: y=slope+b
  float b;
  float slope;
  bool slopeIsInfinite;
} LineEquation;

LineEquation getEquation(Point* start, Point* end) {
  float y1 = start->y;
  float y2 = end->y;
  float x1 = start->x;
  float x2 = end->x;
  LineEquation equation = {0, 0, false};
  if (x2 - x1 == 0) {
    equation.slopeIsInfinite = true;
  } else {
    equation.slope = (y2 - y1) / (x2 - x1);
    equation.b = y1 - 2 * x1;
  }
  return equation;
}

int getLineOrientation(const Point* p, const Point* q, const Point* r) {
  int val = (q->y - p->y) * (r->x - q->x) - (q->x - p->x) * (r->y - q->y);
  if (val == 0)
    return 0;
  return (val > 0) ? 1 : 2;
}

bool onSegment(const Point* p, const Point* q, const Point* r){
  if (q->x <= std::max(p->x, r->x) && q->x >= std::min(p->x, r->x) &&
    q->y <= std::max(p->y, r->y) && q->y >= std::min(p->y, r->y))
    return true;

  return false;
}

bool intersects(const Point* oneStart, const Point* oneEnd, const Point* twoStart, const Point* twoEnd) {
  int o1 = getLineOrientation(oneStart, oneEnd, twoStart);
  int o2 = getLineOrientation(oneStart, oneEnd, twoEnd);
  int o3 = getLineOrientation(twoStart, twoEnd, oneStart);
  int o4 = getLineOrientation(twoStart, twoEnd, oneEnd);

  if (o1 != o2 && o3 != o4)
    return true;
  if (o1 == 0 && onSegment(oneStart, twoStart, oneEnd))
    return true;
  if (o2 == 0 && onSegment(oneStart, twoEnd, oneEnd))
    return true;
  if (o3 == 0 && onSegment(twoStart, oneStart, twoEnd))
    return true;
  if (o4 == 0 && onSegment(twoStart, oneEnd, twoEnd))
    return true;
  return
    false;
}

//Checks if line from start to end will intersect with any connections
bool intersectsWithAnyConnection(const Point* start, const Point* end, std::map<int,Point*> points) {
  for (std::map<int, Point*>::iterator outerIt = points.begin(); outerIt != points.end(); outerIt++) {
    Point* start2 = outerIt->second;
    //todo: if i sort points on like, orientation compared to checkee, dont have to go through entire list
    std::vector<Point*> connections = start2->connections;
    std::sort(std::begin(connections), std::end(connections),
      [](const auto& a, const auto& b) {
      return a->x < b->x;
    });

    for (std::vector<Point*>::iterator it = connections.begin(); it != connections.end(); it++) {
      const Point* end2 = (*it);
      if (start == end2 || end == start2 || start == start2 || end == end2)
        continue;
      if (intersects(start, end, start2, end2))
        return true;
    }
  }
  return false;
}

//meassures distance between 2 points, does not take any connections in mind
int distance(const Point* one, const Point* two) {
  return std::abs(one->x - two->x) + std::abs(one->y - two->y);
}

//Finds amount nearests points to basepoint
std::vector<Point*> findNearestPoints(const Point * basePoint, std::map<int, Point*> points, int amount = 1) {
  std::map<int, Point*> foundPoints;
  if (amount >= points.size())
    amount = points.size() - 1;
  std::vector<Point*> result;
for (size_t i = 0; i < amount; i++) {
  int shortestDistance = -1;
  int currentPointDistance = -1;
  int currentClosestId = NULL;
  for (auto it = points.begin(); it != points.end(); it++) {
    if (it->second == basePoint)
      continue;
    currentPointDistance = abs(it->second->x - basePoint->x) + abs(it->second->y - basePoint->y);
    if (shortestDistance < 0 || currentPointDistance < shortestDistance) {
      //check if already found
      //todo: could do this with id map that stores found ones, so don't have to iterate over all results each time
      //measure time to check if is more efficient
      if (!(std::find(result.begin(), result.end(), it->second) != result.end())) {
        shortestDistance = currentPointDistance;
        currentClosestId = it->second->id;
      }
    }
  }
  result.push_back(points.at(currentClosestId));
}
return result;
}
/***** public ************************************/

Drawable::Drawable(DrawableType _type) : type(_type) {}

Point::Point(int _id, int _x, int _y)
  : id(_id), x(_x), y(_y) {}


bool Graph::connect(Point* start, Point* end, bool ignoreIntersections) {
  if (!ignoreIntersections && intersectsWithAnyConnection(start, end, points))
    return false;
  start->connections.push_back(end);
  //also connect backwards, maybe not needed but this way it's possible depeding on how graph is used, 
  //but this logic allows for one way paths
  end->connections.push_back(start);
  return true;
}

Graph::Graph(int nPoints, bool debug) : Drawable(graphType) {
  int x = 0, y = 0;
  if (!debug) {
    //Create points
    Util::RandomUnique * xGenerator = new Util::RandomUnique(0, 600);
    Util::RandomUnique * yGenerator = new Util::RandomUnique(0, 600);
    for (size_t i = 0; i < nPoints; i++) {
      Point* point = new Point(i, xGenerator->generate(), yGenerator->generate());
      this->points.insert(std::pair<int, Point*>(point->id, point));
    }
    //Make connections with nearest points
    for (auto pointsIt = points.begin(); pointsIt != this->points.end(); pointsIt++) {
      std::vector<Point*> nearests = findNearestPoints(pointsIt->second, points, 10);
      for (std::vector<Point*>::iterator nearestIt = nearests.begin(); nearestIt < nearests.end(); nearestIt++) {
        connect(pointsIt->second, *nearestIt);
      }
    }
  }
  else {
    
    points.insert(std::pair<int, Point*>(0, new Point(0, 100, 100)));
    points.insert(std::pair<int, Point*>(1, new Point(1, 50, 200)));
    points.insert(std::pair<int, Point*>(2, new Point(2, 150, 200)));
    //Make connections with nearest points
    for (std::map<int, Point*>::iterator pointsIt = this->points.begin(); pointsIt != this->points.end(); pointsIt++) {
      std::vector<Point*> nearests = findNearestPoints(pointsIt->second, points, 10);
      for (std::vector<Point*>::iterator nearestIt = nearests.begin(); nearestIt < nearests.end(); nearestIt++) {
        connect(pointsIt->second, *nearestIt);
      }
    }
  }
  return;
}

Graph::~Graph() {
  for (std::map<int, Point*>::iterator it = points.begin(); it != points.end(); it++)
    delete(it->second);
}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
  return os;
}

Path::Path(std::vector<const Point*> _pointsList) : Drawable(pathType), list(_pointsList)  {}

//Used for priority queue in a*
typedef struct PointWrapper {
  const Point* point;
  int priority;
} PointWrapper;

//Used for priority queue in a*
struct compareHeuristic {
  bool operator()(PointWrapper const l, PointWrapper const r) {
    return l.priority > r.priority;
  };
};

Path* AStar(const Graph* graph, const Point* start, const Point* end){
  std::map<int, int> costMap;
  std::map<int, const Point*> cameFrom;

  //todo: is there a way to define logic for compareHeuristic inline (lambda)
  std::priority_queue<PointWrapper, std::vector<PointWrapper>, compareHeuristic> frontier;
  PointWrapper* startWrapper = new PointWrapper{ start, 0 };
  frontier.push(*startWrapper);
  costMap.insert(std::pair<int, int>(startWrapper->point->id, 0));
  const PointWrapper current;


  while (!frontier.empty()) {
    const PointWrapper current = frontier.top();
    frontier.pop();
    //found end?
    if (current.point->id == end->id) {
      std::vector<const Point*> pathList;
      const Point* current = end;
      while (current->id != start->id) {
        pathList.push_back(current);
        current = cameFrom.at(current->id);
      }
      pathList.push_back(start);
      std::reverse(pathList.begin(), pathList.end());
      Path* path = new Path(pathList);
      return path;
    }

    std::vector<Point*> connections = current.point->connections;
    for (std::vector<Point*>::iterator conIt = connections.begin(); conIt != connections.end(); conIt++) {
      int cost = costMap.at(current.point->id) + distance(current.point, (*conIt));
      if (costMap.find((*conIt)->id) == costMap.end()||cost < costMap.at((*conIt)->id)) {
        costMap.insert(std::pair<int, int>((*conIt)->id, cost));
        frontier.push(*(new PointWrapper{ *conIt, cost }));
        cameFrom.insert(std::pair<int, const Point*>((*conIt)->id, current.point));
      }
    
    }
  }
  return new Path(std::vector<const Point*>());
}
