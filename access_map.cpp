#include "access_map.h"

using namespace std;

// ********************************************
/* ---- Member functions of NextAccessMap ---- */
// ********************************************

void NextAccessMap::add_num(index_t idx, long long access_num) {
  map[cast_to_map_index(idx)]->push(access_num);
}

void NextAccessMap::pop_idx(index_t idx) {
  map[cast_to_map_index(idx)]->pop();
}

void NextAccessMap::pop_until_cursor(index_t idx, index_t cursor) {
  idx = cast_to_map_index(idx);
  if(map[idx]->empty()) return;
  long long cur = map[idx]->front();
  while(cur <= cursor) {
    map[idx]->pop();
    if(map[idx]->empty()) return;
    cur = map[idx]->front();
  }
}

long long NextAccessMap::get_head_idx(index_t idx) {
  idx = cast_to_map_index(idx);
  if(map[idx]->empty()) return -1;
  else return map[idx]->front();
}


void NextAccessMap::init(LogReader *access_log) {
  for(long long i = 0; i < access_log->size(); i++) {
    add_num(access_log->at(i), i);
  }
}