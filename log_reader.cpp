#include <iostream>
#include <fstream>
#include <string>

#include "log_reader.h"

unsigned long long LogReader::extract_access_log(ifstream &log_file) {
  unsigned long long count = 0;
  string line;
  while(getline(log_file, line)) {
    access_log->push_back(stoull(line));
    count++;
  }
  return count;
}

LogReader::LogReader(char *file) {
    access_log = new vector<offset_t>();
    ifstream log_file(file);
    if(log_file.is_open()) cout << "Total access count: " << extract_access_log(log_file) << endl;
    else {
        cout << "Unable to open file: " << file << endl;
        log_file.close();
        exit(-1);
    }
    log_file.close();
}
