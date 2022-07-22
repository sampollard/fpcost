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

const long ITERS = 2000000;

int main(int argc, char *argv[])
{
	mpfr_float_1000 xm, ym, exact, approx, relerr;
	float xs, ys, result;
	float inf = std::numeric_limits<float>::infinity();
	ys = 1.0e-4;
    xs = 1.0e-34;

	printf("Type\tOperation\tx\ty\tResult\tRelErr\n");
	for (long i = 0; i < ITERS; i++) {
        result = xs * ys;
        xm = xs;
        ym = ys;
        exact = xm * ym;
        approx = result;
        relerr = abs((exact - approx)/exact);
        printf("float\t*\t%a\t%a\t%a\t", xs, ys, result);
        mpfr_printf("%.10RNe\n", relerr);
        xs = std::nextafter(xs, inf);
		// ys += 1e-5;
	}
	return 0;
}
