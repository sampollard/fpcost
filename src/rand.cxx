#include "rand.hxx"
using namespace std;

Generator::Generator(int s, double m, double n){
	seed = s;
	min = m;
	max = n;
	srand(seed);
}

double Generator::gen(){
	double randomDouble = min + (double)(rand()) / ((double)(RAND_MAX/(max - min)));
	return randomDouble;
}

vector<double> Generator::gen_vector(int length){
	vector<double> randomVector;
	randomVector.reserve(length);
	for (int i=0; i < length; i++){
		randomVector.push_back(gen());
	}
	return randomVector;
}

void normalize(vector<double> x){
	double norm2x = 0.0;
	for (int i = 0; i< x.size(); i++){
		norm2x += (x[i] * x[i]);
	}
	
	for (int i = 0; i < x.size(); i++){
		x[i] *= (1/sqrt(norm2x));
	}
}

void normalize_r(vector<double> x){
	double norm2x = 0.0;
	for (int i = 0; i< x.size(); i++){
		norm2x += (x[i] * x[i]);
	}

	double rsqrt = 1/sqrt(norm2x);
	for (int i = 0; i < x.size(); i++){
		x[i] *= rsqrt;
	}
}

void normalize_xmm(vector<double> x){
	double norm2x = 0.0;
	double y;
	for (int i = 0; i< x.size(); i++){
		norm2x += (x[i] * x[i]);
	}
	
	__m128 y0 = _mm_set1_ps((float) norm2x);
	y0 = _mm_rsqrt_ps(y0);
	y = (double) _mm_cvtss_f32(y0);
	
	for (int i = 0; i < x.size(); i++){
		x[i] *= y;
	}
}