/* Errors of floating-point
 * Author: Samuel D. Pollard
 */
#include <cmath>
#include <cstdio>
#include <limits>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/number.hpp>

using namespace boost::multiprecision;
using namespace boost::math;

const long ITERS = 10000;

int main(int argc, char *argv[])
{
	mpfr_float_1000 xm, exact, approx, relerr;
	double xd, result;
	// double inf = std::numeric_limits<double>::infinity(); // used for std::nextafter
	xd = 1.;

	printf("Type\tOperation\tx\ty\tResult\tRelErr\n");
	for (long i = 0; i < ITERS; i++) {
        result = sqrt(xd);
        xm = xd;
        exact = sqrt(xm);
        approx = result;
        relerr = abs((exact - approx)/exact);
        printf("double\tsqrt\t%a\t%a\t%a\t", xd, nan(""), result);
        mpfr_printf("%.20RNe\n", relerr);
		xd += 0.5;
	}
	return 0;
}
