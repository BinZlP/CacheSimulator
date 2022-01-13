#pragma once

#include <vector>
#include <fstream>
#include <algorithm>
#include "common_include.h"

class LogReader {
private:
  vector<offset_t> *access_log;
  index_t cursor;

  unsigned long long extract_access_log(ifstream &log_file);

public:
  LogReader(char *file);
  ~LogReader() {}

  offset_t at(index_t i) { return access_log->at(i); }

  offset_t max_value() { return *max_element(access_log->begin(), access_log->end()); }
  index_t size() { return access_log->size(); }

  index_t get_cursor() { return cursor; }
  offset_t get_cur_offset() { return access_log->at(cursor); }
  void cursor_next() {cursor++;}

};