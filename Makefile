override FLAGS += -O3
GDB_FLAG=
INCLUDE_FLAG= -lpthread 
OBJS=cache_sim.o access_map.o eviction_policy.o cache.o log_reader.o clock_policy.o associative_cache.o
TARGET=cache_sim

$(TARGET): $(OBJS)
	g++ $(GDB_FLAG) -o $(TARGET) $(FLAGS) $(OBJS) $(INCLUDE_FLAG)

access_map.o: access_map.h access_map.cpp common_include.h log_reader.h
	g++ $(GDB_FLAG) -c -o access_map.o $(FLAGS) access_map.cpp

eviction_policy.o: eviction_policy.h eviction_policy.cpp access_map.h log_reader.h
	g++ $(GDB_FLAG) -c -o eviction_policy.o $(FLAGS) eviction_policy.cpp

cache.o: cache.h cache.cpp common_include.h eviction_policy.h
	g++ $(GDB_FLAG) -c -o cache.o $(FLAGS) cache.cpp

cache_sim.o: cache_sim.cpp common_include.h access_map.h eviction_policy.h cache.h
	g++ $(GDB_FLAG) -c -o cache_sim.o $(FLAGS) cache_sim.cpp

log_reader.o: log_reader.cpp log_reader.h common_include.h
	g++ $(GDB_FLAG) -c -o log_reader.o $(FLAGS) log_reader.cpp 

clock_policy.o: clock_policy.cpp clock_policy.h common_include.h eviction_policy.h
	g++ $(GDB_FLAG) -c -o clock_policy.o $(FLAGS) clock_policy.cpp

associative_cache.o: associative_cache.cpp associative_cache.h common_include.h cache.h hasher.h clock_policy.h
	g++ $(GDB_FLAG) -c -o associative_cache.o $(FLAGS) associative_cache.cpp

clean:
	rm -rf $(OBJS)
	rm -rf $(TARGET)
