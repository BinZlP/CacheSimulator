#include <thread>
#include <vector>
#include "eviction_policy.h"

// ********************************************
/* ---- Member functions of OptimalPolicy ---- */
// ********************************************

// Thread task function
void OptimalPolicy::find(int thread_n, offset_t *offset_buf) {
  index_t start = (buf_sz/MAX_THREAD) * thread_n;
  index_t end = (thread_n+1) == MAX_THREAD ? buf_sz : ((buf_sz/MAX_THREAD) * (thread_n+1));

#ifdef DEBUG_LOG
  cout << "[PREDICT] (" << thread_n << ") " << start << ", " << end << endl;
#endif

  long long am_ret;
  for(index_t i = start; i<end && !wont_use_block.second; i++) {
    access_map->pop_until_cursor(offset_buf[i], access_log->get_cursor());
    am_ret = access_map->get_head_idx(offset_buf[i]);
#ifdef DEBUG_LOG
    cout << "[PREDICT] (" << thread_n << ") Page " << offset_buf[i] << "'s head return: " << am_ret << endl;
    cout << "Cursor: " << access_log->get_cursor() << endl;
#endif
    if(am_ret == -1) {
      // found the block that will not be used in future
      wont_use_block.second = true;
      wont_use_block.first = i;
      return;
    }
    if(am_ret > farthest) {
      write_lock.lock();
      if(am_ret > farthest) {
        farthest = am_ret;
        res = i;
      }
      write_lock.unlock();
    }
  }
}

void OptimalPolicy::init() {
  res = -1;
  farthest = access_log->get_cursor();
  wont_use_block = pair<index_t, bool>(-1, false);
}

// Find out page which will not be used in future
index_t OptimalPolicy::evict(offset_t *offset_buf){
  init();

  vector<thread> threads;
  if(buf_sz > (MAX_THREAD << 12)) {
    for(int i=0; i<MAX_THREAD; i++){
      threads.push_back(thread([=]{this->find(i, offset_buf);}));
    }
    for(int i=0; i<MAX_THREAD; i++){
      // If some thread found the block that will not be used in future,
      // other threads will return as soon as they finish checking currently looking block.
      threads[i].join();
    }
  } else {
    find(0, offset_buf);
  }

  return get_result();
}

void OptimalPolicy::init_access_map() {
  access_map = new NextAccessMap(access_log);
  access_map->init(access_log);
}

index_t OptimalPolicy::get_result() {
  if(wont_use_block.second) return wont_use_block.first;
  else return (res == -1) ? 0 : res;
}