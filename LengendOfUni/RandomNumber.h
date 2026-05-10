#if !defined(D_RANDOM_H)
#define D_RANDOM_H
#include <iostream>
#include <time.h>

using namespace std;

// generate random numbers
class CRandomNumber
{
	public:
		// initialize the random number generator
		CRandomNumber(long s = 0);

		// return a 32-bit random integer m, 1 <= m <= 2^31-2
		long random();

		// return a 32-bit random integer m, 0 <= m <= n-1,
		// where n <= 2^31-1
		long random(long n);

		// return a real number x, 0 <= x < 1
		double frandom();

	private:
		static const long A;
		static const long M;
		static const long Q;
		static const long R;

		long seed;

		static long lCounter;
};
#endif

