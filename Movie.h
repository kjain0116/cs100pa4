#ifndef MOVIE_H
#define MOVIE_H

#include "Actor.h"
#include <string>
#include <vector>

using namespace std;

class Movie {

public:

  string title;
  int year;
  vector<Actor*> cast;

  Movie(string year, string title);

};

#endif // MOVIE_H
