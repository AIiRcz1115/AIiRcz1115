#include <iostream>
//#include <conio.h>
//#include <windows.h>
#include <math.h>
#include <time.h>

#include "polynomial.h"

using namespace std;
long r, q, x;


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
        cout << "test #1 a^b=n => " << a << "^" << b << "=" << n <<" => liczba zlozona\n";
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

int aks(long n)
{

  if( isformofa2b(n) ) return -1;

  r = 2;

  double logn = log(n)/log(2);

  while ( r < n )
  {
    if( gcd( n, r ) != 1 )
    {
        cout << "test #2 NWD: Liczba n="<< n <<" ma dzielnik r="<< r <<", "<< n <<"/"<< r <<"="<< n/r <<" => liczba zlozona\n";
        return -1;
    }

    if( isprime( r ) )
    {
      q = largestprimefactor( r - 1 );
      if ( (q >= 4 * sqrt(r) * logn) && ( ((long)pow ( n, ( r - 1 ) / q ) - 1) % r ) ) break;
      //q >= 4 sqrt(r) * log(n) i ( n^( (r-1)/q ) - 1 ) % r != 0
    }
    r++;
  }

  cout <<"r="<<r<<" | q="<<q<<"\n";

  Polynomial* p=Polynomial::createXa(r,-1);

  long s=(long)(2 * sqrt(r) * logn);
  for (long a = 1 ; a <= s ; a++)
  {

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

      cout << "test #3: n=" << n << " to liczba zlozona\n";

      return -1;
    }

    delete leftpoly;
    delete rightpoly;
  }

  delete p;

  cout << "n=" << n << " to liczba pierwsza\n";
  return 0;
  }

int main()
{
    long liczba;
    char c;
    do
    {
    cout << "Podaj liczbe: ";
    cin >> liczba;

    if( liczba < 2 )
    {
      cout << liczba <<": ani liczba pierwsza, ani zlozona\n";
      continue;
    }
    else
      if( liczba  == 2 || liczba  == 3)
      {
        cout << liczba << " - liczba pierwsza\n";
        continue;
      }

    clock_t t1=clock();
//    t1 = GetTickCount();
    aks(liczba);
    clock_t t2=clock();
//    t2 = GetTickCount();
    double czas =(t2 - t1)/ (double)CLOCKS_PER_SEC;
    cout << "Czas: " << czas << " s = " << czas/(double)60 << " min. Kontunuowac? (t/n)\n";
    cin >> c;
    } while (c == 't');
    cout << "Wcisnij enter by zakonczyc";
//    getch();
    return 0;
}
