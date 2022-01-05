#include "Movie.h"

using namespace std;
Movie::Movie(string year, string title) {
  this->year = stoi(year);
  this->title = title;
}
