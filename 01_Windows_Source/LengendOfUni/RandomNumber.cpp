#include "stdafx.h"
#include "RandomNumber.h"
const long CRandomNumber::A = 48271;
const long CRandomNumber::M = 2147483647;
const long CRandomNumber::Q = M / A;
const long CRandomNumber::R = M % A;

long CRandomNumber::lCounter = 0;

CRandomNumber::CRandomNumber(long s)
{
	if (s < 0)
		s = 0;

	if (s == 0)
	{
		// get time of day in seconds since 12:00 AM,
		// January 1, 1970
		long t_time = time(NULL);

		// mix-up bits by squaring
		t_time *= t_time;
		// result can overflow. handle cases
		// > 0, < 0, = 0
		if (t_time > 0)
			s = t_time ^ 0x5EECE66DL;
		else if (t_time < 0)
			s = (t_time & 0x7fffffff) ^ 0x5EECE66DL;
		else
			s = 0x5EECE66DL;
	}

	seed = s + lCounter;
//	TRACE(_T("seed =  %d\n"), seed);
}

long CRandomNumber::random()
{
	long tmpSeed = A * ( seed % Q ) - R * ( seed / Q );

//	TRACE(_T("seed =  %d, tmpSeed = %d\n"), seed, tmpSeed);
	if( tmpSeed >= 0 )
		 seed = tmpSeed;
	else
		 seed = tmpSeed + M;

	lCounter = seed;
	return seed;
}

long CRandomNumber::random(long n)
{
	double fraction = double(random())/double(M);

	return int(fraction * n);
}

double CRandomNumber::frandom()
{
	return double(random())/double(M);
}