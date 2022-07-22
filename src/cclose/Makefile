CC = gcc
CXX = g++

CXXFLAGS += -g -Wall -std=c++14
LIBS += -lmpfr -lm

TARGETS = subn van_der_waals nextafter nasty sqrt notsonasty nearly-subn squared prod

all : build

build : $(TARGETS)

%: %.cxx
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

.PHONY: run clean
run: $(TARGETS)
	./subn
	./van_der_waals
	./nextafter > nextafter.tsv
	./nasty > nasty.tsv
	./sqrt > sqrt.tsv
	./notsonasty > notsonasty.tsv
	./squared > squared.tsv
	./prod > prod.tsv
	./nearly-subn > nearly-subn.tsv

clean:
	$(RM) $(TARGETS)
