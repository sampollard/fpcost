/* Slow Inverse Square Root
 * Author: Samuel D. Pollard
 */

#ifndef RSQRT_HXX
#define RSQRT_HXX

#include <math.h>
#include <xmmintrin.h>
#include <smmintrin.h>

#include <random>

#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/number.hpp>

using namespace boost::multiprecision;
using namespace boost::math;

#define RAND_EXCEPTION 3

/* A floating-point approximation with its "exact" mpfr value */
typedef struct real_e {
	double approx;
	mpfr_float_1000 exact;
} real_e_t;

/* The famous "fast inverse square root" */
float Q_rsqrt(float x, int n = 1);
/* fast inverse square root but for double precision */
double Q_rsqrt_d(double x, int n = 1);

/* Inverse square root but with operations supported by Satire */
double S_rsqrt_d(double x, int n = 1);
float S_rsqrt_s(float x, int n = 1);

/* Guess with y = 0.5/x */
template <typename FLOAT_T>
FLOAT_T S_rsqrt(FLOAT_T x, int n=1);

/* Guess with y = 1.0 */
template <typename FLOAT_T>
FLOAT_T S_rsqrt_one(FLOAT_T x, int n=1);

/* Guess with y = x */
template <typename FLOAT_T>
FLOAT_T S_rsqrt_x(FLOAT_T x, int n=1);

/* Inverse square root but with intel intrinsic rsqrt as initial guess */
float xmm_rsqrt_s(float x, int n = 1);

/* Inverse square root but with intel intrinsic rsqrt as initial guess */
double xmm_rsqrt_d(double x, int n = 1);

/* Gram Schmidt */
void orthonormalize_unstable(double *A);
/* Modified Gram Schmidt */
void orthonormalize_stable(double *A);

/* Dot product of random values between a and b */
real_e_t random_norm2(long n, double a, double b);


/* Implementations of templated functions */
template <typename FLOAT_T>
FLOAT_T S_rsqrt(FLOAT_T x, int n)
{
	FLOAT_T const x2 = x * 0.5f;
	FLOAT_T const threehalfs = 1.5f;
	/* Not a good intitial guess, but better than nothing */
	FLOAT_T y = 0.5 / x;
	for (int j = 0; j < n; j++) {
		y = y * (threehalfs - (x2 * y * y));
	}
	return y;
}

template <typename FLOAT_T>
FLOAT_T S_rsqrt_one(FLOAT_T x, int n)
{
	FLOAT_T const x2 = x * 0.5f;
	FLOAT_T const threehalfs = 1.5f;
	FLOAT_T y = 1.0;
	for (int j = 0; j < n; j++) {
		y = y * (threehalfs - (x2 * y * y));
	}
	return y;
}

template <typename FLOAT_T>
FLOAT_T S_rsqrt_x(FLOAT_T x, int n)
{
	FLOAT_T const x2 = x * 0.5f;
	FLOAT_T const threehalfs = 1.5f;
	FLOAT_T y = x;
	for (int j = 0; j < n; j++) {
		y = y * (threehalfs - (x2 * y * y));
	}
	return y;
}

#endif
