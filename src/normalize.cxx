#include <iostream>
#include <vector>
#include <chrono>
#include "rand.hxx"
#define NUM_TRIALS 5
using namespace std;

long VECTOR_SIZE = 5000000000;

void print_header()
{
	printf("Type\tOperation\tLength\t\tTime(s)\n");
}

void print_timer(std::string header, std::chrono::duration<double> time){
	printf("%s\t %ld\t %f\t\n", header.c_str(), VECTOR_SIZE, time.count());
}

int main() {
	print_header();
	auto start = std::chrono::steady_clock::now();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds;
	Generator obj(1, 1.0, 100.0);  // Create an object of MyClass
	vector<double> randomVector = obj.gen_vector(VECTOR_SIZE);


	for (int i = 0; i < NUM_TRIALS; i++){
		start = std::chrono::steady_clock::now();
		normalize(randomVector);
		end = std::chrono::steady_clock::now();
		elapsed_seconds = end-start;
		print_timer("double\tnormalize", elapsed_seconds);
	}
 	
	for (int i = 0; i < NUM_TRIALS; i++){
		start = std::chrono::steady_clock::now();
		normalize_r(randomVector);
		end = std::chrono::steady_clock::now();
		elapsed_seconds = end-start;
		print_timer("double\tnormalize_r", elapsed_seconds);
	}

	for (int i = 0; i < NUM_TRIALS; i++){
		start = std::chrono::steady_clock::now();
		normalize_xmm(randomVector);
		end = std::chrono::steady_clock::now();
		elapsed_seconds = end-start;
		print_timer("double\tnormalize_xmm", elapsed_seconds);
	}
}