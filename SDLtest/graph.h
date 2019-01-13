#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <map>

enum DrawableType {
  graphType,
  pathType
};

//interface
class Drawable {
public:
  const DrawableType type;
protected:
  Drawable(DrawableType type);
};

class Point {
public:
  Point(int _id, int x, int y);
  const int id;
  const int x;
  const int y;
  std::vector<Point*> connections;
};

class Graph : public Drawable {
public:
  Graph(int nPoints, bool debug = false);
  ~Graph();
  //Connects 2 points
  bool connect(Point* start, Point *end, bool ignoreIntersections = false);
  std::map<int, Point*> points;
};

class Path : public Drawable {
public:
  Path(std::vector<const Point*> pointList);
  const std::vector<const Point*> list;
};

Path* AStar(const Graph* g, const Point* start, const Point* end);

#endif __GRAPH_H__