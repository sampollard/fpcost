
#include "rsqrt.hxx"

#include <cstdint>
#include <cstdio>

int main(int argc, char *argv[])
{
	float  xs, approx_s;
	double xd, approx_d;
	long iters;
	xd = 3.0;
	xs = (float) xd;
	mpfr_float_1000 exact, approx, relerr, abserr, zero, one;
	zero = 0.0;
	one = 1.0;
	printf("Type\tOperation\tHexadecimal\tDecimal\tRelErr\tAbsErr\n");
	exact = one/sqrt(xd);

	/* Single precision */
	iters = 1;
	approx_s = Q_rsqrt(xs, iters);
	approx = approx_s;
	relerr = abs((exact - approx)/exact);
	abserr = abs(exact - approx);
	printf("float\tQ_rsqrt(x,n=%ld)\t% 24a\t% 20.17e\t", iters, approx_s, approx_s);
	mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);

	/* XMM approximation */
	for (iters = 0; iters<5; iters++) {
		approx_s = xmm_rsqrt_s(xs, iters);
		approx = approx_s;
		relerr = abs((exact - approx)/exact);
		abserr = abs(exact - approx);
		printf("float\txmm_rsqrt(x,n=%ld)\t% 24a\t% 20.17e\t", iters, approx_s, approx_s);
		mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);
	}
	for (iters = 0; iters<5; iters++) {
		approx_d = xmm_rsqrt_d(xs, iters);
		approx = approx_d;
		relerr = abs((exact - approx)/exact);
		abserr = abs(exact - approx);
		printf("double\txmm_rsqrt(x,n=%ld)\t% 24a\t% 20.17e\t", iters, approx_d, approx_s);
		mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);
	}

	/* Double precision */
	iters = 1;
	approx_d = Q_rsqrt_d(xd, iters);
	approx = approx_d;
	relerr = abs((exact - approx)/exact);
	abserr = abs(exact - approx);
	printf("double\tQ_rsqrt(x,n=%ld)\t% 24a\t% 20.17e\t", iters, approx_d, approx_d);
	mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);

	/* Satire Double precision */
	for (iters=1; iters<=NEWTON_ITERS; iters++) {
		approx_d = S_rsqrt_d(xd, iters);
		approx = approx_d;
		relerr = abs((exact - approx)/exact);
		abserr = abs(exact - approx);
		printf("double\tS_rsqrt(x,n=%ld)\t% 24a\t% 20.17e\t", iters, approx_d, approx_d);
		mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);
	}
	/* MPFR 1000 digit */
	mpfr_printf("MPFR(%d)\trsqrt(x)\t% 20RNa\t%.20RNe\t% 1RNf\t% 1RNe\n",
			std::numeric_limits<mpfr_float_1000>::digits, // Precision in bits
			exact, exact, zero, zero);

	/* Dot product */
	real_e_t t;
	for (iters=0; iters < N_RAND; iters++) {
		/* Norm squared */
		t = random_norm2(DIM, UNIF_A, UNIF_B);
		exact = t.exact;
		approx_d = t.approx;
		approx = approx_d;
		relerr = abs((exact - approx)/exact);
		abserr = abs(exact - approx);
		printf("double\t||x||_2^2(dim=%d)\t% 24a\t% 20.17e\t", DIM, approx_d, approx_d);
		mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);
		/* 1/sqrt(norm^2) */
		exact = one/sqrt(t.exact);
		approx_d = 1.0/sqrt(t.approx);
		approx = approx_d;
		relerr = abs((exact - approx)/exact);
		abserr = abs(exact - approx);
		printf("double\trsqrt(||x||_2^2)(dim=%d)\t% 24a\t% 20.17e\t", DIM, approx_d, approx_d);
		mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);
		
	}

	return 0;
}

