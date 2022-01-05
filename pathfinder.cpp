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
  string year;
  string title;
  string start;
  string end;
  string actorName;

  ActorGraph* graph = new ActorGraph();

  // input_file = argv[1]
  // type of path = *argv[2]
  // query_file = argv[3]
  // output_file = argv[4]

  ifstream ifs(argv[3]);
  ofstream ofs(argv[4]);

  graph->loadFromFile(argv[1], -1);
  graph->makeGraph(argv[1], -1);
  graph->findNeighbors();

  bool weighted = false;
  if(*argv[2]=='w') {
    weighted = true;
  }
  else if(*argv[2]=='u') {
    weighted = false;
  }
  // Reads first line of the file
  getline(ifs,line);
  // write header info
  ofs << "(actor)--[movie#@year]-->(actor)--..." << endl;
  // reads rest of lines
  while (getline(ifs, line)) {
    graph->resetNodes();
      istringstream iss(line);
      getline(iss, start, '\t');
      getline(iss, end, '\t');
      stack<pair<Actor*, string>> path = graph->getPath(weighted, start, end);
      ofs << "(" << start << ")";
      // Outputs the path from start to end by popping from stack
      while (false == path.empty()) {
        actorName = path.top().first->name;
        year = path.top().second.substr(0, 4);
        title = path.top().second.substr(4, path.top().second.length() - 4);
        ofs << "--[" << title << "#@" << year << "]-->";
        ofs << "(" << actorName << ")";
        path.pop();
      }
      ofs << endl;

  }
  ifs.close();
  ofs.close();
  delete graph;
}
