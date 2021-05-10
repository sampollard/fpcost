/* Errors of floating-point, x^2 for small x
 * Author: Samuel D. Pollard
 * largest subnormal = 1.1754942107 * 10^-38.
 * sqrt(1.1754942107 * 10^-38)
 *    = .0000000000000000001084202107865503091228960505549503450839328292536
 *    = 1.08420210786e-19
 * smallest normal = 2^-126
 * sqrt(2^-126)
 *    = 2^-63
 *    = .0000000000000000001084202172485504434007452800869941711425781250000
 *    = 1.08420217248e-19
 * 2*smallest normal = 2^-125
 * sqrt(2^-125)
 *    = .0000000000000000001533293416683374081438849972620667472093186834755
 *    = 1.53329341668e-19
 */
#include <cmath>
#include <cstdio>
#include <limits>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/number.hpp>

using namespace boost::multiprecision;
using namespace boost::math;

const long ITERS = 30000000;

int main(int argc, char *argv[])
{
	mpfr_float_1000 xm, exact, approx, relerr;
	float xs, result;
	float inf = std::numeric_limits<float>::infinity();
	// xs = 1.08420217248e-19; // 2^-63, sqrt(smallest binary32 normal)
	xs = 1.53329341668e-19; // sqrt(2^-125)

	printf("Type\tOperation\tx\ty\tResult\tRelErr\n");
	for (long i = 0; i < ITERS; i++) {
		result = xs * xs;
		xm = xs;
		exact = xm * xm;
		approx = result;
		relerr = abs((exact - approx)/exact);
		printf("float\t*\t%a\t%a\t%a\t", xs, xs, result);
		mpfr_printf("%.10RNe\n", relerr);
		xs = std::nextafter(xs, -inf);
	}
	return 0;
}
