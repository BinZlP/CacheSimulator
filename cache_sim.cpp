#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include "common_include.h"
#include "access_map.h"
#include "eviction_policy.h"
#include "cache.h"
#include "log_reader.h"


// *************************
/* ---- main functions ---- */
// *************************

// void calculate_optimal_hit_ratio(vector<offset_t> *access_log) {
void calculate_optimal_hit_ratio(LogReader *access_log) {
  offset_t max_value = access_log->max_value();
  cout << "Offset max value: " << max_value << endl;

  OptimalPolicy my_predictor(access_log, CACHE_SIZE/PAGE_SIZE);
  Cache my_cache(CACHE_SIZE, PAGE_SIZE, (EvictionPolicy *)&my_predictor, max_value);

  for(index_t i=0; i < access_log->size(); i++, access_log->cursor_next())
    my_cache.access(access_log->at(i));

  my_cache.print_statistics();
}

// unsigned long long extract_access_log(ifstream &log_file, vector<offset_t> &access_log) {
//   unsigned long long count = 0;
//   string line;
//   while(getline(log_file, line)) {
//     access_log.push_back(stoull(line));
//     count++;
//   }
//   return count;
// }


int main(int argc, char** argv) {
  /*if(argc < 4) {
    cout << "Usage: " << argv[0] << " [access log file]" << " [cache size]" << " [page size]" << endl;
    return -1;
  }*/

#ifdef CACHE_SIZE
#ifdef PAGE_SIZE

  if(argc < 2) {
    cout << "Usage: " << argv[0] << " [access log file]" << endl;
    return -1;
  }

  LogReader access_log(argv[1]);

  // vector<offset_t> access_log;
  // ifstream log_file(argv[1]);
  // if(log_file.is_open()) cout << "Total access count: " << extract_access_log(log_file, access_log) << endl;
  // else {
  //   cout << "Unable to open file: " << argv[1] << endl;
  //   return -1;
  // }
  // log_file.close();

  cout << "Simulator starts." << endl;
  auto start = chrono::high_resolution_clock::now();
  // calculate_optimal_hit_ratio(&access_log);
  calculate_optimal_hit_ratio(&access_log);
  auto elapsed = chrono::high_resolution_clock::now() - start;

  cout << "Elapsed time for getting optimal hit ratio: " << chrono::duration_cast<chrono::seconds>(elapsed).count() << " sec." << endl;

  cout << endl << "Simulation completed." << endl;
  return 0;

#endif
#endif

  cout << "Error: Must define CACHE_SIZE and PAGE_SIZE" << endl;
  return -1;
}

