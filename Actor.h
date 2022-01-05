#ifndef ACTOR_H
#define ACTOR_H

#include <limits>
#include <string>
#include <vector>

using namespace std;

class Actor {

public:
  string name;
  Actor* prevA;
  string prevM;
  bool done;
  int dist;
  int size;
  vector<pair<Actor*, string>> neighbors;

  Actor(string name);

};

#endif // ACTOR_H
