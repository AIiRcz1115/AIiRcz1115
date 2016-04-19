#include <iostream>

using namespace std;

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
