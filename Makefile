CC = gcc
CXX = g++

CXXFLAGS += -g -Wall -std=c++14
LIBS += -lmpfr -lm

TARGETS = testbed

all : build

build : $(TARGETS)

testbed: rsqrt.o testbed.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $^

.PHONY: run clean
run: $(TARGETS)
	./testbed > testbed.tsv

clean:
	$(RM) $(TARGETS) *.o *.hxx.gch

# Dependency Lists
rsqrt.o : rsqrt.hxx
