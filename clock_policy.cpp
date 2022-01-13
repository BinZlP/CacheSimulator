#include "clock_policy.h"
#include <iostream>

void ClockPolicy::init() { 
  res = -1;
}

void ClockPolicy::find(int thread_n, offset_t *offset_buf) {
  index_t clock_cursor = clock_head % buf_sz;
  do {
    if(clock_data[clock_cursor].offset != offset_buf[clock_cursor]) {
      cout << "[FATAL] Clock's data is mismatch with cache's data" << endl;
      exit(-1);
    }

    if(clock_data[clock_cursor].chance == false)
      res = clock_cursor;
  } while(res == -1);
  clock_head = (clock_cursor + 1) % buf_sz;
}

index_t ClockPolicy::get_result() {
  return res;
}

void ClockPolicy::notify_replace(index_t slot, offset_t new_off) {
  clock_data[slot].offset = new_off;
  clock_data[slot].chance = false;
}

void ClockPolicy::update_score(index_t slot) {
  clock_data[slot].chance = true;
}

int ClockPolicy::update_score_by_offset(offset_t offset) {
  for(index_t i=0; i<buf_sz; i++) {
    if(clock_data[i].offset == offset) {
      clock_data[i].chance = true;
      return 0;
    }
  }
#ifdef DEBUG_LOG
  cout << "[CLOCK] No such offset exist: " << offset << endl;
#endif
  return -1;
}

index_t ClockPolicy::evict(offset_t *offset_buf) {
  init();

  find(0, offset_buf);

  return get_result();
}