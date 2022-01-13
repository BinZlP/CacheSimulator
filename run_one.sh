#!/bin/bash

if [ $# -lt 3 ]; then
  echo "$0: Missing arguments"
  echo "* Usage: $0 [access log] [cache size] [page size]"
  exit
fi


make clean
# g++ -o cache_sim.out cache_sim.cpp -lpthread -DCACHE_SIZE=$2 -DPAGE_SIZE=$3 -O3
make FLAGS+=" -DCACHE_SIZE=$2 -DPAGE_SIZE=$3"
sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'
./cache_sim $1
