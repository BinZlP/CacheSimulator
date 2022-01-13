#include "cache.h"
#include <iostream>
#include <thread>
#include <vector>

// ************************************
/* ---- Member functions of Cache ---- */
// ************************************

void Cache::thread_search(int thread_num, offset_t offset, index_t *ret_buf){
  index_t i = (total_block/MAX_THREAD)*thread_num;
  index_t end = (thread_num + 1) == MAX_THREAD ? (total_block) : (total_block/MAX_THREAD) * (thread_num + 1);
  for(; i<end && !is_finished; i++)
    if(offset_buf[i] == offset) {
      *ret_buf = i;
      is_finished = true;
      return;
    }
}

// Check target page is in the cache
index_t Cache::search(offset_t offset){
  if(total_block > (MAX_THREAD << 12) ) {
    index_t *ret_val = (index_t *)calloc(MAX_THREAD, sizeof(index_t));
    for(int i=0; i<MAX_THREAD; i++)
      ret_val[i] = -1;

    index_t res;
    vector<thread> thread_v;

    for(int i=0; i<MAX_THREAD; i++)
      thread_v.push_back(thread(&Cache::thread_search, this, i, offset, &ret_val[i]));
    for(int i=0; i<MAX_THREAD; i++)
      thread_v[i].join();

    is_finished = false;
    for(int i=0; i<MAX_THREAD; i++)
      if(ret_val[i] != -1) {
        res = ret_val[i];
#ifdef DEBUG_LOG
        cout << "[SEARCH] found: " << offset << "," << res << endl;
#endif
        free(ret_val);
        return res;
      }
#ifdef DEBUG_LOG
    cout << "[SEARCH] not found: " << offset << endl;
#endif
    free(ret_val);
    return -1;
  } else {
    for(index_t i=0; i<total_block; i++) {
      if(offset_buf[i] == offset)
        return i;
    }
    return -1;
  }
}

// Get empty block's index
index_t Cache::get_empty_block_index() {
/*  for(index_t i=0; i<total_block; i++)
    if(offset_buf[i] == Cache::EMPTY) return i;*/
  if(used_block < total_block) return used_block;
  return -1;
}

// Replace buf[index]'s value with given offset
offset_t Cache::replace(offset_t offset, index_t index){
  offset_t old_val = offset_buf[index];
  offset_buf[index] = offset;
#ifdef EVICT_LOG
  if(old_val == Cache::EMPTY) cout << "Page " << offset << " has been inserted to cache block " << index << endl;
  else cout << "Page " << old_val << " has been eviced from cache block " << index << endl;
#endif
  return old_val;
}

// Try to access target data.
// If there's no data in cache, load data into cache.
int Cache::access(offset_t offset) {
  if(search(offset) < 0) { // If there's no target in the cache
    if(is_referenced_before(offset)) { // cache miss
#ifdef HIT_MISS_LOG
      cout << "CACHE MISS: " << offset << endl;
#endif
      miss(false);
    } else { // Counts cold miss seperatly
#ifdef HIT_MISS_LOG
      cout << "CACHE COLD MISS: " << offset << endl;
#endif
      miss(true);
      reference_map[offset/cache_block_size] = true;
    }

    // If there're empty blocks, insert page into empty block.
    if(used_block < total_block) {
      replace(offset, get_empty_block_index());
      used_block++;
    }
    // else, replace with the data which will not be used in future.
    else replace(offset, replace_policy->evict(offset_buf, total_block));

    return 0;

  } else { // If found target in the cache

#ifdef HIT_MISS_LOG
    cout << "CACHE HIT: " << offset << endl;
#endif
    hit();

    return 1;

  }
}

// Check whether target is referenced before
bool Cache::is_referenced_before(offset_t offset) {
  if(reference_map[offset/cache_block_size]) return true;
  else return false;
}

// Print cache statistics
void Cache::print_statistics() {
  cout << "[Cache statistics]"<< endl;
  cout << "  Total cache access: " << cache_hit + cache_miss + cold_miss << endl;
  cout << "  Cache hit: " << cache_hit << endl;
  cout << "  Cache miss(w/o cold miss): " << cache_miss << endl;
  cout << "  Cache hit ratio: " << (double)cache_hit / (double)(cache_hit + cache_miss) << endl << endl;
}
