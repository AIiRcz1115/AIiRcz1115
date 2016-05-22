#include "stdafx.h"

using namespace std;


class MillerRabin
{

protected:


	int pow2(int pow) {
		if (pow == 0) {
			return 1;
		}
		else return 2 * pow2(pow - 1);
	}


	//a^b mod m
	int powAtoBmodM(int a, int b, int m)
	{
		int i;
		int result = 1;
		long int x = a%m;

		for (i = 1; i <= b; i <<= 1)
		{
			x %= m;
			if ((b&i) != 0)
			{
				result *= x;
				result %= m;
			}
			x *= x;
		}

		return result;
	}


	// obliczenie S
	int calcS(int liczba,int s) {
		if (liczba < pow2(s)) {
			return s - 1;
		}
		else calcS(liczba, s + 1);
	}


public:


	bool executeAlgorithm(long n, int k, int id, int procs) {
		long s = 0;
		long a, d, prime;
		bool result = false;

		srand(time(NULL));

		if (n<4)
		{
			return true;
		}
		if (n % 2 == 0)
		{
			return false;
		}

		// calculate s and d
		s = calcS(n, s);
		d = n / pow2(s);

		// try k times
		for (int i = 0; i<k; i++)
		{
			a = 1 + (int)((n - 1)*rand() / (RAND_MAX + 1.0));
			if (powAtoBmodM(a, d, n) != 1)
			{
				result = false;
				for (int r = 0; r <= s - 1; r++)
				{
					if (powAtoBmodM(a, pow2(r) * d, n) == n - 1)
					{
						result = true;
						break;
					}
				}
				if (!result)
				{
					return 0;
				}
			}
		}

		return 1;
	}


};
