CC = gcc
CXX = g++

CXXFLAGS += -g -Wall -std=c++14
LIBS += -lmpfr -lm

TARGETS = testbed_err testbed_time

all : testbed_time

time : testbed_time
error : testbed_err

normalize_time: normalize_testbed_time

testbed_err: DEBUG = -DPRINT_ERROR=1
testbed_err: rsqrt.o testbed.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

testbed_time: rsqrt.o testbed.o 
	$(CXX) $(CXXFLAGS) $(DEBUG) -o $@ $^ $(LIBS)

normalize_testbed_time: rand.o normalize.o 
	$(CXX) $(CXXFLAGS) $(DEBUG) -o $@ $^ $(LIBS)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $^

.PHONY: run clean error time run_error run_time
run_time: time
	./testbed_time > testbed_time.tsv

run_error: error
	./testbed_err > testbed_err.tsv

normalize_run_time: normalize_time
	./normalize_testbed_time > normalize_testbed_time.tsv

clean:
	$(RM) $(TARGETS) *.o *.hxx.gch

# Dependency Lists
rsqrt.o : rsqrt.hxx
rand.o : rand.hxx
