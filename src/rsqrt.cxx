#ifndef RSQRT_CXX
#define RSQRT_CXX

#include "rsqrt.hxx"

float Q_rsqrt(float x, int n)
{
	float const x2 = x * 0.5f;
	float const threehalfs = 1.5f;
	/* Initial Guess */
	auto i = reinterpret_cast<std::uint32_t&>(x);
	i  = 0x5f3759df - (i >> 1);
	float y = *reinterpret_cast<float*>(&i);
	/* Newton's Method */
	for (int j = 0; j < n; j++) {
		y *= threehalfs - (x2 * y * y);
	}
	return y;
}

double Q_rsqrt_d(double x, int n)
{
	/* Integer constant derived by https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf */
	double const x2 = x * 0.5f;
	double const threehalfs = 1.5f;
	/* Initial Guess */
	auto i = reinterpret_cast<std::uint64_t&>(x);
	i  = 0x5fe6eb50c7b537a9 - (i >> 1);
	double y = *reinterpret_cast<double*>(&i);
	/* Newton's Method */
	for (int j = 0; j < n; j++) {
		y *= threehalfs - (x2 * y * y);
	}
	return y;
}

double S_rsqrt_d(double x, int n)
{
	double const x2 = x * 0.5f;
	double const threehalfs = 1.5f;
	/* Not a good intitial guess, but better than nothing */
	double y = 0.5 / x;
	for (int j = 0; j < n; j++) {
		y *= threehalfs - (x2 * y * y);
	}
	return y;
}

float S_rsqrt_s(float x, int n)
{
	float const x2 = x * 0.5f;
	float const threehalfs = 1.5f;
	/* Not a good intitial guess, but better than nothing */
	float y = 0.5 / x;
	for (int j = 0; j < n; j++) {
		y *= threehalfs - (x2 * y * y);
	}
	return y;
}

float xmm_rsqrt_s(float x, int n)
{
	float const x2 = x * 0.5f;
	float const threehalfs = 1.5f;
	float y;
	/* A very good initial guess */
	__m128 y0 = _mm_set1_ps(x);
	y0 = _mm_rsqrt_ps(y0);
	/* free operation, just interpret low bits of vector as float */
	y = _mm_cvtss_f32(y0);
	for (int j = 0; j < n; j++) {
		y *= threehalfs - (x2 * y * y);
	}
	return y;
}

/* Inverse square root but with intel intrinsic rsqrt as initial guess */
double xmm_rsqrt_d(double x, int n)
{
	double const x2 = x * 0.5f;
	double const threehalfs = 1.5f;
	double y;
	/* A very good initial guess, but slow because conversion from single/double */
	__m128 y0 = _mm_set1_ps((float) x);
	y0 = _mm_rsqrt_ps(y0);
	y = (double) _mm_cvtss_f32(y0);
	for (int j = 0; j < n; j++) {
		y *= threehalfs - (x2 * y * y);
	}
	return y;
}

void orthonormalize_unstable(double *A)
{
	return;
}

void orthonormalize_stable(double *A)
{
	return;
}

real_e_t random_norm2(long n, double a, double b)
{
	static std::default_random_engine generator;
	if (b < a) {
		throw RAND_EXCEPTION;
	}
	std::uniform_real_distribution<double> runif(a,b);

	real_e_t rv;
	double acc, ele;
	mpfr_float_1000 acc_m, ele_m;
	acc_m = 0.0;
	acc = 0.0;
	for (long i = 1; i < n; i++) {
		ele = runif(generator);
		ele_m = ele;
		acc = acc + ele * ele;
		acc_m = acc_m + ele_m * ele_m;
	}
	rv.approx = acc;
	rv.exact = acc_m;
	return rv;
}
#endif

