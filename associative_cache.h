#pragma once

#include "common_include.h"
#include "cache.h"
#include "hasher.h"
#include "clock_policy.h"

#include <cmath>

class AssociativeCacheHasher : Hasher<index_t, offset_t> {
private:
  const static unsigned long long CONST_A = 27644437;
  const static unsigned long long CONST_P = 68718952447;

  index_t num_cells;
  unsigned long long pg_size;
  unsigned int level = 0;
  unsigned int split = 0;
  unsigned int offset_factor = 1;

  index_t universal_hash(unsigned long long v, unsigned long long modulo);
  index_t do_hash(offset_t offset);
  index_t do_hash1(offset_t offset);

public:
  AssociativeCacheHasher(): num_cells(0), pg_size(0) {}
  AssociativeCacheHasher(index_t _num_cells, unsigned long long _pg_size) 
    : num_cells(_num_cells), pg_size(_pg_size) {}
  ~AssociativeCacheHasher() {}

  index_t hash(offset_t offset) override;
};

class AssociativeCache {
private:
  unsigned long long total_cache_size;
  index_t way_cache_block_num;
  index_t num_way;

  Cache **caches;
  AssociativeCacheHasher *hasher;

public:
  AssociativeCache(unsigned long long _total_cache_size, unsigned long long _block_size,
    index_t _num_way, offset_t _max_value)
    : total_cache_size(_total_cache_size), way_cache_block_num((_total_cache_size / _num_way) / _block_size),
      num_way(_num_way)
  {
    hasher = new AssociativeCacheHasher(_num_way, _block_size);
    caches = new Cache*[num_way];
    for(index_t i=0; i<num_way; i++) {
      caches[i] = new Cache(way_cache_block_num * _block_size, _block_size, (EvictionPolicy *)(new ClockPolicy(way_cache_block_num)), _max_value);
    }
  }

  ~AssociativeCache() {
    delete hasher;
    for(index_t i=0; i<num_way; i++)
      delete caches[i]->get_policy();
    delete caches;
  }

  int access(offset_t offset);

  void print_statistics();
};