#include "stdafx.h"

using namespace std;


class AKS
{

protected:


  bool isformofa2b(long n)
  {
    long b;

    for(long a = 2 ; a <= floor( sqrt( n ) + 1 ) ; a++)
    {
      b=2;

      while( pow( a, b ) <= n)
      {
        if( pow ( a, b) == n)
        {
          printf("test #1 a^b=n => %ld^%ld=%ld => liczba zlozona\n", a, b, n);
          fflush(stdout);
          return true;
        }
        b++;
      }
    }
    return false;
  }


  long gcd(long a, long b)
  {
    while(a != b)
    {
      if (b > a)
      b -= a;
      else
      a -= b;
    }
    return a;
  }


  bool isprime(long n)
  {
    if( n == 1 ) return false; // 1 - ani zlozona, ani pierwsza
    if( n < 4 ) return true; // 2 i 3 - liczba pierwsza
    if( !(n % 2) ) return false; //liczby parzyste - zlozone

    long l = (long)sqrt(n)  + 1;
    for(long i = 5 ; i <= l ; i += 2 ) //liczby nieparzyste do sqrt(n)
    if( !(n % i) ) return false;

    return true;
  }


  long largestprimefactor(long n)
  {
    long i;
    if(n < 2) return 1;

    for(i = 2 ; i < n ; i++)
    {
      while( !(n % i) ) n /= i;
      if( n==1 ) break;
    }

    return i;
  }


public:


  bool executeAlgorithm(long n, int id, int procs)
  {
    int test1 = 0, test2 = 0, test3 = 0, one = 0;
    long r1, r2, q, s;
    double logn;

    if (id == 0) {
      if( isformofa2b(n) )
      test1 = 1;
      logn = log(n)/log(2);
    }

    // wynik testu w roota do wszystkich procesow;
    MPI_Bcast(&logn, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&test1, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (test1)
    	return false;

    r1 = 2 + id;

    while ( r1 < n && test2 == 0)
    {
      printf("Proces: %d, Stage one: %ld \n", id, (r1*100)/n);
      if ( gcd( n, r1 ) != 1 )
      {
        printf("Proces: %d, test #2 NWD: Liczba n=%ld ma dzielnik r=%ld, %ld/%ld=%ld => liczba zlozona\n", id, n, r1, n, r1, n/r1);
        fflush(stdout);
        one = 1;
      }

      if (r1 < n-((n-2)%procs)) {
        MPI_Allreduce(&one, &test2, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
      }

      if ( isprime( r1 ) )
      {
        q = largestprimefactor( r1 - 1 );
        if ( (q >= 4 * sqrt(r1) * logn) && ( ((long)pow ( n, ( r1 - 1 ) / q ) - 1) % r1 ) ) break;
        //q >= 4 sqrt(r) * log(n) i ( n^( (r-1)/q ) - 1 ) % r != 0
      }
      r1 += procs;
    }

    MPI_Allreduce(&one, &test2, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (test2)
    	return false;

    //Najwieksze r ze wszystkich procesow
    MPI_Allreduce(&r1, &r2, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    Polynomial* p=Polynomial::createXa(r2,-1);

    //s obliczamy tylko na roocie
    if (id == 0)
    s = (long)(2 * sqrt(r2) * logn);
    MPI_Bcast(&s, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    for (long a = 1+id ; a <= s && test3 == 0; a+=procs)
    {
      printf("Proces: %d, Stage two: %ld \n", id, (a*100)/s);
      fflush(stdout);
      Polynomial* leftpoly=Polynomial::createXa(1,-a);

      leftpoly->powmod(n, n);
      leftpoly->mod(p); // ( (x-a)^n ) (mod x^r - 1, n)

      Polynomial* rightpoly=Polynomial::createXa(n, -a);

      rightpoly->mod((long)n);
      rightpoly->mod(p); //(x^n - a) (mod x^r - 1, n)

      if (leftpoly->compareTo(rightpoly)!=0)
      {
        delete leftpoly;
        delete rightpoly;

        printf("Proces: %d, test #3: n=%ld to liczba zlozona\n", id, n);
        fflush(stdout);

        one = 1;
      }
      if (a < s-((s-1)%procs)) {
        MPI_Allreduce(&one, &test3, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
      }

      delete leftpoly;
      delete rightpoly;
    }

    delete p;

    MPI_Allreduce(&one, &test3, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (test3)
    return false;

    printf("Proces: %d, n=%ld to liczba pierwsza\n", id, n);
    fflush(stdout);
    return true;
  }


};
