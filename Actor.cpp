#include "Actor.h"

#include <limits>
#include <string>
#include <vector>

using namespace std;

Actor::Actor(string name) {
  this->name = name;
  this->prevA = nullptr;
  this->prevM = "";
  this->dist = numeric_limits<int>::max();
  this->size = 1;
  this->done = false;
}
