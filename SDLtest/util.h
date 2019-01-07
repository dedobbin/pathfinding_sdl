#ifndef __UTIL_H_
#define __UTIL_H_

namespace Util {
  void seedRandom(int seed);
  int generateRandom(int min = 0, int max = 500);
  class RandomUnique;
}

class Util::RandomUnique {
public:
  RandomUnique(int min, int max);
  int generate();
private:
  std::vector<int> uniques;
};

#endif