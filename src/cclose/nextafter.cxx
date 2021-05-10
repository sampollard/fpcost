/* Errors of floating-point
 * Author: Samuel D. Pollard
 * usage:
 * 	$CC -g -Wall -std=c++14 nextafter.c -o nextafter -lmpfr -lm
 * 	./nextafter
 */
#include <cmath>
#include <cstdio>
#include <limits>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/number.hpp>

using namespace boost::multiprecision;
using namespace boost::math;

const long ITERS_LARGE = 1000;
const long ITERS_SMALL = 100;

int main(int argc, char *argv[])
{
	mpfr_float_1000 xm, ym, exact, approx, relerr;
	double xd, yd, result;
	double inf = std::numeric_limits<double>::infinity();
	yd = 1000.;

	printf("Type\tOperation\tx\ty\tResult\tRelErr\n");
	for (long i = 0; i < ITERS_LARGE; i++) {
		xd = 1. / 1024.;
		for (long j = 0; j < ITERS_SMALL; j++) {
			result = xd * yd;
			xm = xd;
			ym = yd;
			exact = xm * ym;
			approx = result;
			relerr = abs((exact - approx)/exact);
			printf("double\t*\t%a\t%a\t%a\t", xd, yd, result);
			mpfr_printf("%.20RNe\n", relerr);
			for (long k = 0; k < 100; k++) {
				xd = std::nextafter(xd, inf);
			}
		}
		yd += 0.5;
	}
	return 0;
}
