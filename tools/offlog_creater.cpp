#include <iostream>
#include <fstream>
#include <random>
#include <string>

using namespace std;

int main(void){
  unsigned long long nums;
  cout << "Size of log: ";
  cin >> nums;

  unsigned long long max_num;
  cout << "Max # of offset: ";
  cin >> max_num;

  string file_name;
  cout << "Output file name: ";
  cin >> file_name;

  ofstream fout(file_name.c_str());
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<unsigned long long> dis(1, max_num);

  for(unsigned long long i = 0; i<nums; i++)
    fout << dis(gen) << endl;

}
