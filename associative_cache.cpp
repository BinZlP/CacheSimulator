#include "associative_cache.h"

#include <iostream>

// ***********************************************
/* ---- Member functions of AssociativeCache ---- */
// ***********************************************

int AssociativeCache::access(offset_t offset) {
  index_t way = hasher->hash(offset);

#ifdef DEBUG_LOG
  cout << "[ACache] access to offset " << offset << ", way: " << way << endl;
#endif

  ClockPolicy *policy = (ClockPolicy *)caches[way]->get_policy();
  index_t ret = caches[way]->access(offset);
  index_t slot = caches[way]->search(offset);


  if(ret >= 0) {
    policy->update_score(slot);
  } else {
    policy->notify_replace(slot, offset);
  }
  return ret;
}

void AssociativeCache::print_statistics() {
  unsigned long long total_cache_hit = 0, total_cache_miss = 0, total_cold_miss = 0;

  for(index_t i=0; i<num_way; i++) {
    total_cache_hit += caches[i]->get_hit();
    total_cache_miss += caches[i]->get_miss();
    total_cold_miss += caches[i]->get_cold_miss();
  }

  cout << "[Cache statistics]"<< endl;
  cout << "  Total cache access: " << total_cache_hit + total_cache_miss + total_cold_miss << endl;
  cout << "  Cache hit: " << total_cache_hit << endl;
  cout << "  Cache miss(w/o cold miss): " << total_cache_miss << endl;
  cout << "  Cache hit ratio: " << (double)total_cache_hit / (double)(total_cache_hit + total_cache_miss) << endl << endl;
}

// *****************************************************
/* ---- Member functions of AssociativeCacheHasher ---- */
// *****************************************************

index_t AssociativeCacheHasher::hash(offset_t offset) {
  index_t global_idx = do_hash(offset);
  if(global_idx < split) 
    global_idx = do_hash1(offset);
  return global_idx;
}

index_t AssociativeCacheHasher::do_hash(offset_t offset) {
  index_t n_cells = (num_cells * (1 << level));
  return universal_hash(offset / pg_size / offset_factor + 1, n_cells); 
}

index_t AssociativeCacheHasher::do_hash1(offset_t offset) {
  index_t n_cells = (num_cells * (1 << (level + 1)));
  return universal_hash(offset / pg_size / offset_factor + 1, n_cells); 
}

index_t AssociativeCacheHasher::universal_hash(unsigned long long v, unsigned long long modulo) {
  return (v * CONST_A) % CONST_P % modulo;
}


