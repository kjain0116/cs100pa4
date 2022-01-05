#include "ActorGraph.h"

using namespace std;

static const int inf = numeric_limits<int>::max();


size_t ActorHash::operator()(const Actor* actor) const {
   return hash<string>()(actor->name);
}


bool ActorEquals::operator()(const Actor* actor1, const Actor* actor2) const {
   return actor1->name == actor2->name;
}


bool ActorPriority::operator()(const pair<Actor*, string> p1, const pair<Actor*, string> p2) const {
   return p1.first->dist > p2.first->dist;
}


ActorGraph::ActorGraph() {
  actorsToMovies = new unordered_map<Actor*, vector<Movie*>, ActorHash, ActorEquals>();
  moviesToActors = new unordered_map<string, vector<string>>();
}

bool ActorGraph::loadFromFile(const char* in_filename, int yr) {
    ifstream infile(in_filename);
    bool have_header = false;
    while (infile) {
        string s;
        if (!getline( infile, s )) break;
        if (!have_header) {
            have_header = true;
            continue;
        }
        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string next;
            if (!getline( ss, next, '\t' )) break;
            record.push_back( next );
        }
        if (record.size() != 3) {
            continue;
        }
        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        Actor* actor = new Actor(actor_name);
        if (actorsToMovies->find(actor) == actorsToMovies->end()) {
          actorsToMovies->emplace(actor, vector<Movie*>());
        }
        else {
          delete(actor);
        }
         string movie = record[2] + movie_title;
         if (moviesToActors->find(movie) == moviesToActors->end()) {
           moviesToActors->emplace(movie, vector<string>());
         }
         moviesToActors->at(movie).push_back(actor_name);
    }
    if (!infile.eof()) {
        return false;
    }
    infile.close();
    return true;
}

vector<pair<Actor*, string>> ActorGraph::makeGraph(const char* in_file, int year) {
  Actor* tempAct = new Actor("");
  vector<pair<Actor*, string>> newlyAdded;
  unordered_set<string> alreadyAdded;
  string line;
  string actor;
  string name;
  string title;
  string y;
  string movie;
  string movieTitle;
  string movieYear;
  vector<string> vec;
  ifstream ifs(in_file);
  getline(ifs, line);
  while (getline(ifs, line)) {
    istringstream iss(line);
    getline(iss, actor, '\t');
    getline(iss, title, '\t');
    getline(iss, y, '\t');
    movie = y + title;
    if ((actor.empty()==false) && (movie.empty()==false)) {
      if (year == -1 || year == stoi(movie.substr(0, 4))) {
        tempAct->name = actor;
        Actor* curr = actorsToMovies->find(tempAct)->first;
        Movie* mov = new Movie(y, title);
        Actor* neighbor;
        vec = moviesToActors->at(movie);
        for(int i = 0; i < vec.size(); i++){
          name = vec.at(i);
          tempAct->name = name;
          neighbor = actorsToMovies->find(tempAct)->first;
          if (!(curr->name == neighbor->name)) {
            mov->cast.push_back(neighbor);
          }
          if (!(alreadyAdded.find(movie) != alreadyAdded.end())) {
            newlyAdded.push_back(pair<Actor*, string>(neighbor, movie));
          }
        }
        actorsToMovies->at(curr).push_back(mov);
        alreadyAdded.insert(movie);
      }
    }
  }
  ifs.close();
  delete(tempAct);
  return newlyAdded;
}

void ActorGraph::findNeighbors() {
  Actor* act;
  int castSize = 0;
  string movieTitle;
  string movieYear;
  Actor* tempAct = new Actor("");
  auto a = actorsToMovies->begin();
  while(a != actorsToMovies->end()) {
    tempAct->name = a->first->name;
    vector<pair<Actor*, string>> neighbors;
    if (actorsToMovies->find(tempAct) != actorsToMovies->end()) {
      act = actorsToMovies->find(tempAct)->first;
      vector<Movie*> movies = actorsToMovies->at(act);
      int i = 0;
      while(i < movies.size()){
        castSize = movies.at(i)->cast.size();
        int j =0;
        while(j < castSize) {
          Actor* neighbor = movies.at(i)->cast.at(j);
          movieYear = to_string(movies.at(i)->year);
          movieTitle = movies.at(i)->title;
          string movieInfo = movieYear + movieTitle;
          neighbors.push_back(pair<Actor*, string>(neighbor, movieInfo));
          j++;
        }
       i++;
      }
    }
    a->first->neighbors = neighbors;
    a++;
  }
  delete(tempAct);
}

stack<pair<Actor*, string>> ActorGraph::getPath(bool w, string start, string finish) {
  Actor* tempAct = new Actor("");
  Actor* a;
  string movie;
  stack<pair<Actor*, string>> path;
  if (checkPath(w, start, finish)) {
    tempAct->name = finish;
    Actor* destination = actorsToMovies->find(tempAct)->first;
    pair<Actor*, string> curr = pair<Actor*, string>(destination, destination->prevM);
    for(curr.first->prevA; curr.first->prevA;) {
      path.push(curr);
      a = curr.first->prevA;
      movie = curr.first->prevA->prevM;
      curr = pair<Actor*, string>(a, movie);
    }
  }
  delete(tempAct);
  return path;
}

bool ActorGraph::checkPath(bool w, string start, string finish) {
  Actor* tempAct = new Actor("");
  int year = 0;
  pair<Actor*, string> neighbor;
  vector<pair<Actor*, string>> vec;
  bool exists = false;
  int edgeWeight = 1;
  int currDist = 0;
  pair<Actor*, string> curr;
  priority_queue<pair<Actor*, string>, vector<pair<Actor*, string>>, ActorPriority> frontier;
  tempAct->name = start;
  Actor* source = actorsToMovies->find(tempAct)->first;
  curr = pair<Actor*, string>(source, "");
  curr.first->dist = 0;
  frontier.push(curr);
  while (frontier.empty() == false) {
    curr = frontier.top();
    frontier.pop();
    if (curr.first->name.compare(finish) == 0) {
      exists = true;
      break;
    }
    if ((curr.first->done) == false) {
      curr.first->done = true;
      vec = curr.first->neighbors;
      int i = 0;
      while(i < vec.size()) {
        neighbor = vec.at(i);
        edgeWeight = 1;
        if (w == true) {
         year = stoi(neighbor.second.substr(0, 4));
         edgeWeight += (2015 - year);
        }
        currDist = curr.first->dist + edgeWeight;
        if (!(neighbor.first->dist <= currDist)) {
          neighbor.first->prevA = curr.first;
          neighbor.first->prevM = neighbor.second;
          neighbor.first->dist = currDist;
          frontier.push(neighbor);
        }
        i++;
      }
    }
  }
  delete(tempAct);
  return exists;
}

void ActorGraph::Union(vector<pair<Actor*, string>> newLinks) {
  Actor* currA;
  Actor* sent;
  string currM = newLinks.front().second;
  int ind = 0;
  int startInd = 0;
  int endInd = 0;
  int max =  0;
  while(ind < newLinks.size()) {
    startInd = ind;
    currA = newLinks.at(ind).first;
    currM = newLinks.at(ind).second;
    sent = nullptr;
    max = 0;
    while ( !(ind >= newLinks.size()) && (newLinks.at(ind).second == currM)) {
      ind++;
    }
    endInd = ind;
    for(ind=startInd;ind < endInd; ind++) {
      currA = newLinks.at(ind).first;
      while (currA->prevA) {
        currA = currA->prevA;
      }
      if (max < currA->size) {
        sent = currA;
        max = currA->size;
      }
    }
    for(ind = startInd; ind < endInd; ind++) {
      currA = newLinks.at(ind).first;
      while (currA->prevA){
        currA = currA->prevA;
      }
      if (currA != sent) {
        currA->prevA = sent;
      }
    }
  }
}

bool ActorGraph::connected(string start, string finish) {
  Actor* tempA = new Actor("");
  tempA->name = start;
  Actor* a1 = actorsToMovies->find(tempA)->first;
  tempA->name = finish;
  Actor* a2 = actorsToMovies->find(tempA)->first;
  compressPath(a1);
  compressPath(a2);
  Actor* a1sent = Find(a1);
  Actor* a2sent = Find(a2);
  if(a1sent == a2sent) {
    return true;
  }
  return false;

}

// returns sentinel
Actor* ActorGraph::Find(Actor* a) {
  if(a->prevA) {
    return a->prevA;
  }
  else {
    return a;
  }
}

void ActorGraph::compressPath(Actor* currA) {
  Actor* sent = currA;
  Actor* tempA = currA;
  while (sent->prevA) {
    sent = sent->prevA;
  }
  while (currA->prevA) {
    tempA = currA->prevA;
    if (currA->prevA != sent) {
      currA->prevA = sent;
    }
    currA = tempA;
  }
}

void ActorGraph::resetNodes() {
  auto a = actorsToMovies->begin();
  while(a != actorsToMovies->end()){
    auto actor = a->first;
    actor->done = false;
    actor->dist = inf;
    actor->size = 1;
    actor->prevA = nullptr;
    actor->prevM = "";
    a++;
  }
}

void ActorGraph::eraseMovies() {
  auto a = actorsToMovies->begin();
  while(a != actorsToMovies->end()){
    a->second.clear();
    a++;
  }
}

void ActorGraph::deleteGraph() {
  auto a = actorsToMovies->begin();
  while(a != actorsToMovies->end()){
    vector<Movie*> movs = a->second;
    for(int i = 0; i < movs.size(); i++){
      auto mov = movs.at(i);
      delete mov;
    }
    delete a->first;
    a++;
  }
  delete actorsToMovies;
  delete moviesToActors;

}

ActorGraph::~ActorGraph() {
  deleteGraph();
}
