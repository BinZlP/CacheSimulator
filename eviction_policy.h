#pragma once

#include <mutex>
#include "access_map.h"
#include "log_reader.h"

class EvictionPolicy {
private:
  virtual void init() = 0;
  virtual void find(int thread_n, offset_t *offset_buf) = 0;
  virtual index_t get_result() = 0;

protected:
  // vector<offset_t> *access_log;
  // index_t cursor;
  index_t buf_sz;

public:
  EvictionPolicy() : buf_sz(CACHE_SIZE / PAGE_SIZE) {}
  EvictionPolicy(index_t buf_sz) {}
  ~EvictionPolicy() {}

  index_t evict(offset_t *offset_buf, index_t buf_sz);
};

class OptimalPolicy : EvictionPolicy {
private:
  LogReader *access_log;
  NextAccessMap *access_map;

  mutex write_lock;
  pair<index_t, bool> wont_use_block;
  index_t res, farthest;

  void init() override;
  void find(int thread_n, offset_t *offset_buf) override;

  void init_access_map();

  index_t get_result() override;

public:
  OptimalPolicy(LogReader *_access_log, index_t _buf_sz)
             : EvictionPolicy(_buf_sz), access_log(_access_log), write_lock() {
    init_access_map();
#ifdef DEBUG_LOG
    cout << "OptimalPolicy init. completed." << endl;
#endif
  }
  ~OptimalPolicy() { delete access_map; }
};