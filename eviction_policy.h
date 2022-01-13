#pragma once

#include <mutex>
#include "access_map.h"
#include "log_reader.h"

class EvictionPolicy {
private:
  virtual void init() = 0;
  virtual void find(int thread_n, offset_t *offset_buf, index_t buf_sz) = 0;
  virtual index_t get_result() = 0;

protected:
  // vector<offset_t> *access_log;
  // index_t cursor;
  LogReader *access_log;

public:
  // EvictionPolicy(vector<offset_t> *_access_log) 
  //   : cursor(0), access_log(_access_log) {}
  EvictionPolicy(LogReader *_access_log)
    : access_log(_access_log) {}
  ~EvictionPolicy() {}

  index_t evict(offset_t *offset_buf, index_t buf_sz);
};

class OptimalPolicy : EvictionPolicy {
private:
  NextAccessMap *access_map;

  mutex write_lock;
  pair<index_t, bool> wont_use_block;
  index_t res, farthest;

  void init() override;
  void find(int thread_n, offset_t *offset_buf, index_t buf_sz) override;

  void init_access_map();

  index_t get_result() override;

public:
  OptimalPolicy(LogReader *_access_log)
             : EvictionPolicy(_access_log), write_lock() {
    init_access_map();
#ifdef DEBUG_LOG
    cout << "OptimalPolicy init. completed." << endl;
#endif
  }
  ~OptimalPolicy() { delete access_map; }
};