#include "ActorGraph.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

using namespace std;

int main(int argc, char* argv[]) {
  string line;
  string type;
  string start;
  string finish;
  bool notFound = false;
  bool widestp = true;
  bool connected = false;
  ifstream ifs(argv[2]);
  ofstream ofs(argv[3]);
  type = argv[4];

  ActorGraph* graph = new ActorGraph();
  graph->loadFromFile(argv[1], -1);

  if(type == "widestp") {
    widestp = true;
  }
  else if(type == "ufind") {
    widestp = false;
  }
  // Reads first line of the file
  getline(ifs,line);
  // write header info
  ofs << "Actor1\tActor2\tYear" << endl;
  while (getline(ifs, line)) {
    graph->eraseMovies();
    graph->resetNodes();
    istringstream iss(line);
    getline(iss, start, '\t');
    getline(iss, finish, '\t');
    ofs << start << '\t' << finish << '\t';
    int year = 1941;
    while(year < 2016){
      if (widestp == true) {
          graph->makeGraph(argv[1], year);
          graph->findNeighbors();
          graph->resetNodes();
          notFound = graph->getPath(false, start, finish).empty();
          if (notFound == false) {
            connected = true;
            ofs << year << endl;
            break;
          }
      }
      else {
        vector<pair<Actor*, string>> newlyAdded = graph->makeGraph(argv[1], year);
        if (newlyAdded.empty() == false) {
          graph->Union(newlyAdded);
          connected = graph->connected(start, finish);
          if (connected == true) {
            ofs << year << endl;
            break;
          }
        }
      }
     year++;
    }
    if (connected == false) {
      ofs << 9999 << endl;
    }
  }
  ifs.close();
  ofs.close();

  delete(graph);
}
