#pragma once

#include <cstdlib>

#include "common_include.h"
#include "eviction_policy.h"

class Cache {
private:
  static const offset_t EMPTY = -1;

  index_t total_block;
  index_t used_block;

  unsigned long long cache_total_size;
  unsigned long long cache_block_size;

  unsigned long long cache_hit;
  unsigned long long cache_miss;
  unsigned long long cold_miss;

  offset_t *offset_buf;

  //map<offset_t, bool> reference_map;
  bool *reference_map;

  bool is_finished;

  EvictionPolicy *replace_policy;
  // friend class EvictionPolicy;

  void thread_search(int thread_num, offset_t offset, index_t *ret_buf);


public:
  // Cache(unsigned long long _cache_size, unsigned long long _block_size, EvictionPolicy *_predictor, offset_t max_value)
  //        : cache_hit(0), cache_miss(0), cold_miss(0), cache_total_size(_cache_size), cache_block_size(_block_size),
  //          total_block(_cache_size/_block_size), used_block(0), replace_policy(_predictor), reference_map(), is_finished(false) {
    Cache(unsigned long long _cache_size, unsigned long long _block_size, EvictionPolicy *_policy, offset_t max_value)
         : cache_hit(0), cache_miss(0), cold_miss(0), cache_total_size(_cache_size), cache_block_size(_block_size),
           total_block(_cache_size/_block_size), used_block(0), replace_policy(_policy), reference_map(), is_finished(false) {
    offset_buf = (offset_t *)calloc(total_block, sizeof(offset_t));
    for(index_t i=0; i<total_block; i++) offset_buf[i] = Cache::EMPTY;
    reference_map = (bool *)calloc((max_value/_block_size)+1, sizeof(bool));
#ifdef DEBUG_LOG
    cout << "Cache init. completed. " << endl;
#endif
  }
  ~Cache(){ free(offset_buf); free(reference_map); }

  index_t search(offset_t offset);
  index_t get_empty_block_index();
  offset_t replace(offset_t offset, index_t index);
  int access(offset_t offset);

  void hit() { cache_hit++; }
  void miss(bool is_cold) { if(is_cold) cold_miss++; else cache_miss++; }

  bool is_referenced_before(offset_t offset);

  void print_statistics();

  unsigned long long get_hit() { return cache_hit; }
  unsigned long long get_miss() { return cache_miss; }
  unsigned long long get_cold_miss() { return cold_miss; }
};