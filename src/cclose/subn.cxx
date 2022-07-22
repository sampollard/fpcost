/* How good is Machine Epsilon?
 * Author: Samuel D. Pollard
 * usage:
 * 	$CC subn.c -o subn -lm
 * 	./subn
 */
#include <cmath>
#include <cstdio>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/number.hpp>

using namespace boost::multiprecision;
using namespace boost::math;

int main(int argc, char *argv[])
{
	float  xs = 1.0e-4;
	float  ys = 3.0e-40;
	double xd = 1.0e-4;
	double yd = 3.0e-40;
    mpfr_float_1000 exact = 1.0e-4 * 3.0e-40;
    mpfr_float_1000 sp = xs * ys;
    mpfr_float_1000 fp_e = 1.0;
    fp_e = sp / exact - fp_e;
    

	printf("Operation\tHexadecimal\tDecimal\tRelErr\n");
	printf("float(x)\t% 24a\t% 13e\t% 13e\n", xs, xs, nanf(""));
	printf("float(y)\t% 24a\t% 13e\t% 13e\n", ys, ys, nan(""));
	printf("float(x * y)\t% 24a\t% 13e\t% 13e\n", xs * ys, xs * ys, fabs((double) (xs * ys) - xd * yd) / (xd * yd));
	printf("double(x * y)\t% 24a\t% 13e\t% 13e\n", xd * yd, xd * yd, fabs((double) (xs * ys) - xd * yd) / (xd * yd));
	printf("(x * y)(1 + e), double(e) =\t% 13e\n", (xs * ys)/(xd * yd) - 1.0);
	printf("abs error(x * y)\t%.17e\n", fabs((double) (xd * yd) - (xs * ys)));
	printf("delta(float)\t%.17e\n", pow(2.,-149.0));
	mpfr_printf("(x * y)(1 + e), e =\t% 13RNe\n", fp_e);
	printf("nan*0\t%f\tNA\tNA\n", nan("")*0.0);
	return 0;
}
