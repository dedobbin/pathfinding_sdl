#include <random>
#include <ctime>
#include <numeric>
#include "util.h"

bool randomInit = false;

void Util::seedRandom(int seed) {
  randomInit = true;
  srand(seed);
}

int Util::generateRandom(int min, int max) {
  if (!randomInit)
    Util::seedRandom(time(NULL));
  return rand() % (max + 1 - min) + min;
}

Util::RandomUnique::RandomUnique(int min, int max) {
  if (max < min)
    std::swap(min, max);
  uniques = std::vector<int>(max - min + 1);
  std::iota(uniques.begin(), uniques.end(), min);
  std::random_device rd;
  std::mt19937 generator(rd());
  std::shuffle(uniques.begin(), uniques.end(), generator);
}

int Util::RandomUnique::generate() {
  if (uniques.empty()) 
    throw new std::runtime_error("No more unique ints to generate...");

  int result = uniques.back();
  uniques.pop_back();
  return result;
}