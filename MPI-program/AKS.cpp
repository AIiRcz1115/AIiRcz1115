#include "stdafx.h"

using namespace std;


class AKS
{

protected:


	bool isformofa2b(long n)
	{
		long b;

		for(long a = 2 ; a <= floor( sqrt( n ) + 1 ) ; a++) {
			b = 2;

			while( pow( a, b ) <= n) {
				if( pow ( a, b) == n)
					return true;
				b++;
			}
		}
		return false;
	}


	long gcd(long a, long b)
	{
		while (a != b) {
			if (b > a)
				b -= a;
			else
				a -= b;
		}
		return a;
	}


	bool isprime(long n)
	{
		long l = (long)sqrt(n) + 1;
		for ( long i = 5 ; i <= l ; i += 2 ) //liczby nieparzyste do sqrt(n)
			if( !(n % i) )
				return false;

		return true;
	}


	long largestprimefactor(long n)
	{
		long i;
		if (n < 2)
			return 1;

		for (i = 2 ; i < n ; i++) {
			while ( !(n % i) )
				n /= i;
			if ( n==1 )
				break;
		}

		return i;
	}


public:


	bool executeAlgorithm(long n, int id, int procs)
		{
		unsigned short int result = 1, all_results = 1;
		long r = 2 + id, all_r, f_r = 0, all_f_r, q, s;
		double logn = log(n)/log(2);

		if (id == 0)
			cerr << 0 << " ";

		// Test 1

		if (id == 0)
			if( isformofa2b(n) )
				result = 0;

		// Wynik testu 1 z mastera do wszystkich procesow
		MPI_Bcast(&result, 1, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);

    	if (result == 0)
    		return false;

		// Test 2

		while ( r < n && all_results == 1)
		{
			if ( gcd( n, r ) != 1 )
				result = 0;

			if ( isprime( r ) ) {
				q = largestprimefactor( r - 1 );
				// q >= 4 sqrt(r) * log(n) i ( n^( (r-1)/q ) - 1 ) % r != 0
				if ( (q >= 4 * sqrt(r) * logn) && ( ((long)pow ( n, ( r - 1 ) / q ) - 1) % r ) ) {
					f_r = r;
					break;
				}
			}
			r += procs;
		}

		// Wynik testu 2 do wszystkich procesow
		MPI_Allreduce(&result, &all_results, 1, MPI_UNSIGNED_SHORT, MPI_MIN, MPI_COMM_WORLD);

		if ( all_results == 0 )
			return false;

		// Najwieksze r ze wszystkich procesow
		MPI_Allreduce(&r, &all_r, 1, MPI_LONG, MPI_MAX, MPI_COMM_WORLD);

		// Najmniejsze znalezione r
		MPI_Allreduce(&f_r, &all_f_r, 1, MPI_LONG, MPI_MAX, MPI_COMM_WORLD);
		if ( f_r == 0 )
			f_r = all_f_r;
		MPI_Allreduce(&f_r, &all_f_r, 1, MPI_LONG, MPI_MIN, MPI_COMM_WORLD);

		if ( all_f_r != 0 )
			all_r = all_f_r;

		Polynomial* p=Polynomial::createXa(all_r,-1);
		s = (long)(2 * sqrt(all_r) * logn);

		// test 3

		for (long a = 1+id ; a <= s && all_results == 1; a+=procs)
		{
			// procenty
			if (id == 0)
				cerr << (a*100)/s << endl;

			Polynomial* leftpoly=Polynomial::createXa(1,-a);

			leftpoly->powmod(n, n);
			leftpoly->mod(p); // ( (x-a)^n ) (mod x^r - 1, n)

			Polynomial* rightpoly=Polynomial::createXa(n, -a);

			rightpoly->mod((long)n);
			rightpoly->mod(p); //(x^n - a) (mod x^r - 1, n)

			if (leftpoly->compareTo(rightpoly) != 0)
				result = 0;

			// wynik testu 3 do wszystkich procesow
			if (a < s-((s-1)%procs)) {
				MPI_Allreduce(&result, &all_results, 1, MPI_UNSIGNED_SHORT, MPI_MIN, MPI_COMM_WORLD);
			}

			delete leftpoly;
			delete rightpoly;
		}

		delete p;

		// wynik testu 3 do wszystkich procesow
		MPI_Allreduce(&result, &all_results, 1, MPI_UNSIGNED_SHORT, MPI_MIN, MPI_COMM_WORLD);

		if ( all_results == 0 )
			return false;

		return true;
	}


};
