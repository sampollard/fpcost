/* Newton-Ralphson Iteration for the Van der Waals equation
 * Source: Numerical Analysis, Timothy Sauer, 2nd, Edition, pg. 60.
 * Author: Samuel D. Pollard
 * usage: ./van_der_waals
 */

/* There is some nastiness with mpfr. You usually can't do something where
 * you allocate a temporary expression with MPFR. e.g. printing something
 * like abs(x-y) will cause an unhelpful segfault. You need to instead do
 * mpfr_float_1000 val = abs(x-y); mpfr_printf("...", val);
 */
#include <mpfr.h>
#include <cstdio>
#include <boost/multiprecision/mpfr.hpp>

#define MPFR_T mpfr_float_1000

using namespace boost::multiprecision;

/* This constant is exact in decimal, units J/(K * mol) */
#define GAS_CONST           (8.31446261815324)
const char* GAS_CONST_STR = "8.31446261815324";
mpfr_float_1000 GAS_CONST_M;
/* 1 atm = ATM_PA Pascals */
#define ATM_PA              (101325.0)
/* 1 bar = 100,000 Pascals */
#define PA_BAR              (100000.)

/* Initial guess of Van der Waals calculating Volume based on PV = nRT */
inline double ideal_gas_V(double P, double n, double T)
{
	return n * GAS_CONST * T / P;
}

MPFR_T ideal_gas_V_m(MPFR_T P, MPFR_T n, MPFR_T T)
{
	return n * GAS_CONST * T / P;
}

/* Solves for V in (P + (an^2) / V^2)(V - nb) = nRT
 * by using an initial guess from the ideal gas law,
 * then solving
 * f(V) = PV^3 - (nbP + nRT)V^2 + n^2aV - n^3ab = 0
 * using Newton-Ralphson iteration
 * f'(V) = 3PV^2 - 2(nbP + nRT)V + n^2a
 */
double van_der_waals_newton_V(
	double P, double n, double a, double b, double T, int iters)
{
	static const double R = GAS_CONST;
	double V = ideal_gas_V(P, n, T);
	for (int i = 0; i < iters; i++) {
		V = V - (P*V*V*V - (n*b*P + n*R*T)*V*V + n*n*a*V - n*n*n*a*b) /
		        (3.*P*V*V - 2.*(n*b*P + n*R*T)*V + n*n*a);
	}
	return V;
}

MPFR_T van_der_waals_newton_V_m(
	MPFR_T P, MPFR_T n, MPFR_T a, MPFR_T b, MPFR_T T, int iters)
{
	MPFR_T R = GAS_CONST_M; // Global variable, must be set with mpfr_set_str
	MPFR_T V = ideal_gas_V_m(P, n, T);
	for (int i = 0; i < iters; i++) {
		V = V - (P*V*V*V - (n*b*P + n*R*T)*V*V + n*n*a*V - n*n*n*a*b) /
		        (3.*P*V*V - 2.*(n*b*P + n*R*T)*V + n*n*a);
	}
	return V;
}

int main(int argc, char *argv[])
{
	double approx_d;
	/* Solve for Volume */
	double P = 15.0*ATM_PA; // Pressure in kPa
	double T = 320.0;       // Temperature (K)
	double n = 1.0;         // number of moles
	/* Constants for Oxygen */
	double a = 1.36;        // L^2 * atm/mole^2
	double b = 0.003183;    // L/mole
	mpfr_float_1000 exact, approx, relerr, abserr, systemic, syserr, one, zero;
	one = 1.0;
	zero = 0.0;
	printf("Type\tOperation\tHexadecimal\tDecimal\tRelErr\tAbsErr\tSystemic AbsErr\n");

	/* Gas Constant */
	/* This is a workaround, since the default operator converts to double,
	 * then sets the MPFR with the (incorrect) value. Therefore, mpfr.h C
	 * bindings are required until newer versions of boost (maybe 1.75)
	 * which have operator=(const char*) support. Looking at
	 * https://www.exploringbinary.com/floating-point-converter, we see
	 * that GAS_CONST isn't exactly representable as a double, so there
	 * will be some error. We want to calculate exactly how much. */
	mpfr_t tmp;
	mpfr_init2(tmp, 3324); // 1000 digits
	if (mpfr_set_str(tmp, GAS_CONST_STR, 10, MPFR_RNDN)) {
		std::cerr << "Error setting MPFR string" << std::endl;
		return 1;
	}
	exact = tmp;
	GAS_CONST_M = tmp;
	approx_d = GAS_CONST;
	approx = approx_d;
	relerr = abs((exact - approx)/exact);
	abserr = abs(exact - approx);
	printf("double\tGas Constant\t% 24a\t% 20.15e\t", approx_d, approx_d);
	mpfr_printf("%.20RNf\t%.20RNe\t%.20RNe\n", relerr, abserr, one);

	/* Van der Waals */
	/* Solve the problem to some large number of digits, treat as exact */
	systemic = van_der_waals_newton_V_m(P, n, a, b, T, 100);
	mpfr_printf(
		"mpfr(3324)\tVan der Waals Newton(100)\t"\
	    "%RNa\t%.17RNe\t%.1RNe\t%.1RNe\t%.1RNe\n",
	    systemic, systemic, zero, zero, zero);

	/* Initial Guess */
	approx_d = ideal_gas_V(P, n, T);
	approx = approx_d;
	exact = ideal_gas_V_m(P, n, T);
	relerr = abs((exact - approx)/exact);
	abserr = abs(exact - approx);
	syserr = abs(systemic - approx);
	printf("double\tInitial Guess V\t% 24a\t% 20.15e\t", approx_d, approx_d);
	mpfr_printf("%.20RNf\t%.20RNe\t%.20RNe\n",
	            relerr, abserr, syserr);

	/* Newton's iteration */
	for (int iters = 1; iters < 10; iters++) {
		approx_d = van_der_waals_newton_V(P, n, a, b, T, iters);
		approx = approx_d;
		exact = van_der_waals_newton_V_m(P, n, a, b, T, iters);
		relerr = abs((exact - approx)/exact);
		abserr = abs(exact - approx);
		syserr = abs(systemic - approx);
		printf("double\tVan der Waals Newton(%d)\t% 24a\t% 20.15e\t",
		       iters, approx_d, approx_d);
		mpfr_printf("%.20RNf\t%.20RNe\t%.20RNe\n",
		            relerr, abserr, syserr);
	}

	return 0;
}
