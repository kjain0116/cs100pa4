#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include "Actor.h"
#include "Movie.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class ActorHash {
  public:
    size_t operator()(const Actor* actor) const;
};

class ActorEquals {
  public:
    bool operator()(const Actor* actor1, const Actor* actor2) const;
};

class ActorPriority {
  public:
    bool operator()(const pair<Actor*, string> p1, const pair<Actor*, string> p2) const;
};

class ActorGraph {

public:

  ActorGraph();

  unordered_map<string, vector<string>>* moviesToActors;
  unordered_map<Actor*, vector<Movie*>, ActorHash, ActorEquals>* actorsToMovies;
  
  bool loadFromFile(const char* in_filename, int yr);

  vector<pair<Actor*, string>> makeGraph(const char* in_file, int year);

  void findNeighbors();

  stack<pair<Actor*, string>> getPath(bool w, string start, string finish);

  bool checkPath(bool w, string start, string finish);

  void Union(vector<pair<Actor*, string>> newLinks);

  bool connected(string start, string finish);

  Actor* Find(Actor* a);

  void compressPath(Actor* currA);

  void resetNodes();

  void eraseMovies();

  void deleteGraph();

  ~ActorGraph();

};

#endif // ACTORGRAPH_H
