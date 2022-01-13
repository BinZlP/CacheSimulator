#pragma once

#include <vector>
#include <queue>
#include <algorithm>

#include "common_include.h"
#include "log_reader.h"

#ifdef DEBUG_LOG
#include <iostream>
#endif

class NextAccessMap {
private:
  vector<queue<long long>*> map;

public:
  NextAccessMap(LogReader *access_log) {

    offset_t max_val = access_log->max_value();
    for(index_t i=0; i <= cast_to_map_index(max_val); i++)
      map.push_back(new queue<long long>());
#ifdef DEBUG_LOG
    cout << "AccessMap construction completed." << endl;
#endif
  }

  ~NextAccessMap() {
    for(index_t i=0; i<map.size(); i++) delete map[i];
  }


  void add_num(index_t idx, long long acceess_num);
  void pop_idx(index_t idx);
  void pop_until_cursor(index_t idx, index_t cursor);
  long long get_head_idx(index_t idx);

  index_t cast_to_map_index(index_t idx) { return idx/PAGE_SIZE; }

  void init(LogReader *access_log);

};