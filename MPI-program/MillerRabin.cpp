#include "stdafx.h"

using namespace std;


class MillerRabin
{

protected:


	int pow2(int s) {
		if (s == 0) { 
			return 1; 
		}
		else {
			return 2 * pow2(s-1); 
		}
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
	int calcS(int liczba, int s) {
		int maxDzielnik =1;
		if (liczba < pow2(s)) {
			return maxDzielnik;
		}
		else {
			if (liczba-1%s == 0) maxDzielnik = s;
			calcS(liczba, s + 1);
		}
	}


public:


	bool executeAlgorithm(long n, int k, int id, int procs) {
		int s = 1;
		int a, d;
		unsigned short int result = 0, all_results = 0;
		srand(time(NULL));

		// calculate s and d
		s = calcS(n, s);
		d = n / pow2(s);

		// try k times
		for (int i = 0; i<k; i++)
		{
			// procenty
			if (id == 0)
				cerr << (i*100)/k << " ";
			
			a = 1 + (int)((n - 1)*(rand() / (RAND_MAX + 1.0)));
			if (powAtoBmodM(a, d, n) != 1)
			{
				result = 0;
				all_results = 0;
				for (int r = 0; r <= s - 1; r++)
				{
					if (powAtoBmodM(a, pow2(r) * d, n) == n - 1)
					{
						result = 1;
						break;
					}
				}
				
				MPI_Allreduce(&result, &all_results, 1, MPI_UNSIGNED_SHORT, MPI_MIN, MPI_COMM_WORLD);
				if (all_results == 0)
				{
					return false;
				}
			}
		}

		return true;
	}


};
