/* Notes
 * Don't change what's between PRINT_ERROR unless it breaks compatibility somehow.
 */
#ifndef TESTBED_TIME_CXX
#define TESTBED_TIME_CXX
#include "rsqrt.hxx"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <chrono>
#define MPFR_T mpfr_float_1000

#define XMM_ITERS 5
#define SATIRE_RSQRT_ITERS 9
#define N_RAND 100

/* TODO: Tweak these as needed */
// #define PRINT_ERROR 1
#define NUM_TRIALS 700000
#define DIM 10

/* A good starting point for distributions */
#define UNIF_A (0.001)
#define UNIF_B (1000.)


void print_header()
{
#ifdef PRINT_ERROR
	printf("Type\tOperation\tHexadecimal\tDecimal\tRelErr\tAbsErr\n");
#else
	/* TODO: Print your header for timing with fields separated by tabs */
	printf("No timing information\n");
#endif
}

void print_exact(std::string header, MPFR_T exact)
{
	MPFR_T zero;
	zero = 0.0;
	/* MPFR 1000 digit */
	/* XXX: To declutter, don't print %a */
	mpfr_printf("%s\t% 20RNa\t%.20RNe\t% 1RNf\t% 1RNe\n",
			header.c_str(), zero, exact, zero, zero);
}

template <typename FLOAT_T>
void print_error(std::string header, FLOAT_T approx_f, MPFR_T exact)
{
	MPFR_T approx, relerr, abserr;
	approx = approx_f;
	relerr = abs((exact - approx)/exact);
	abserr = abs(exact - approx);
	printf("%s\t% 24a\t% 20.17e\t", header.c_str(), approx_f, approx_f);
	mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);
}

int main(int argc, char *argv[])
{
	float  xs, approx_s;
	double xd, approx_d;
	long iters; /* Newton iterations */
	long j;
	xd = 3.0;
	xs = (float) xd;
	MPFR_T exact, approx, relerr, abserr, zero, one;
	zero = 0.0;
	one = 1.0;
	print_header();

	for (iters = 0; iters < XMM_ITERS; iters++) {
		/* Single precision Q_sqrt */
		/* TODO: Run many times for different inputs (vary xs) and print out the time */
		
		auto start = std::chrono::steady_clock::now();
		for (j = 0; j < NUM_TRIALS; j++) {
			approx_s = Q_rsqrt(xs, iters);
#ifdef PRINT_ERROR
			exact = one/sqrt((double) xs);
			print_exact("MPFR(" +
			            std::to_string(std::numeric_limits<MPFR_T>::digits) +
			            ")\tQ_rsqrt(x,n=" + std::to_string(iters) + ")",
			            exact);
			print_error<float>(
					"float\tQ_rsqrt(x,n=" + std::to_string(iters)+")",
					approx_s, exact);
#endif
		}
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
    		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
		
		/* Double precision Qsqrt */
		/* TODO: Run many times (xd) use the number generated as xs */
		
		start = std::chrono::steady_clock::now();
		for (j = 0; j < NUM_TRIALS; j++) {
			approx_d = Q_rsqrt_d(xd, iters);
#ifdef PRINT_ERROR
			exact = one/sqrt(xd);
			print_exact("MPFR(" +
			            std::to_string(std::numeric_limits<MPFR_T>::digits) +
			            ")\tQ_rsqrt(x,n=" + std::to_string(iters) + ")",
			            exact);
			print_error<double>(
					"double\tQ_rsqrt(x,n="+std::to_string(iters)+")",
					approx_d, exact);
#endif
		}
		end = std::chrono::steady_clock::now();
    		elapsed_seconds = end-start;
    		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	}

	/* XMM approximations */
	/* Iterations change the number of newton's iterations */
	for (iters = 0; iters <= XMM_ITERS; iters++) {
		/* Single Precision */
		/* TODO: Run many times for different inputs and print out the time */
		
		auto start = std::chrono::steady_clock::now();
		for (j = 0; j < NUM_TRIALS; j++) {
			approx_s = xmm_rsqrt_s(xs, iters);
#ifdef PRINT_ERROR
			exact = one/sqrt((double) xs);
			print_error<float>(
					"float\txmm_rsqrt(x,n="+std::to_string(iters)+")",
					approx_s, exact);
#endif
		}
		auto end = std::chrono::steady_clock::now();
    		std::chrono::duration<double> elapsed_seconds = end-start;
    		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

		/* Double Precision */
		/* TODO: Run many times, use the number generated as xd */
		
		start = std::chrono::steady_clock::now();
		for (j = 0; j <= NUM_TRIALS; j++) {
			approx_d = xmm_rsqrt_d(xd, iters);
#ifdef PRINT_ERROR
			exact = one/sqrt(xd);
			print_error<double>(
					"double\txmm_rsqrt(x,n="+std::to_string(iters)+")",
					approx_s, exact);
#endif
		}
		end = std::chrono::steady_clock::now();
    		elapsed_seconds = end-start;
    		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	}

	/* Satire Double precision */
	for (iters = 0; iters <= SATIRE_RSQRT_ITERS; iters++) {
		/* TODO: Run many times */
		for (j = 0; j <= NUM_TRIALS; j++) {
			approx_s = S_rsqrt_s(xs, iters);
			approx_d = S_rsqrt_d(xd, iters);
			exact = one/sqrt(xd);
#ifdef PRINT_ERROR
			print_error<float>(
					"float\tS_rsqrt(x,n="+std::to_string(iters)+")",
					approx_s, exact);
			print_error<double>(
					"double\tS_rsqrt(x,n="+std::to_string(iters)+")",
					approx_d, exact);
#endif
		}
	}

	/* Dot product */
	/* TODO: (Sam) Fix this part up. It should do ||x|| and rsqrt in
	 * same loop */
	real_e_t t;
	for (iters = 0; iters < N_RAND; iters++) {
		/* Norm squared */
		/* TODO: Run many times and time */
		
		auto start = std::chrono::steady_clock::now();
		for (j = 0; j <= NUM_TRIALS; j++) {
			t = random_norm2(DIM, UNIF_A, UNIF_B);
		}
#ifdef PRINT_ERROR
		print_error<double>(
				"double\t||x||_2^2(dim=" + std::to_string(DIM)+")",
				t.approx, t.exact);
#endif
		auto end = std::chrono::steady_clock::now();
    		std::chrono::duration<double> elapsed_seconds = end-start;
    		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
		
		/* 1/sqrt(norm^2) */
		/* TODO: Run many times and time */
		start = std::chrono::steady_clock::now();
		for (j = 0; j <= NUM_TRIALS; j++) {
			approx_d = 1.0/sqrt(t.approx);
#ifdef PRINT_ERROR
			exact = one/sqrt(t.exact);
			print_error<double>(
					"double\trsqrt(||x||_2^2)(dim=" + std::to_string(DIM)+")",
					approx_d, exact);
#endif
		}
		end = std::chrono::steady_clock::now();
    		elapsed_seconds = end-start;
    		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
		
		approx = approx_d;
		relerr = abs((exact - approx)/exact);
		abserr = abs(exact - approx);
		printf("double\trsqrt(||x||_2^2)(dim=%d)\t% 24a\t% 20.17e\t", DIM, approx_d, approx_d);
		mpfr_printf("%.20RNe\t%.20RNe\n", relerr, abserr);
	}

	return 0;
}
#endif

