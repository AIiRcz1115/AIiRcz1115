#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "mpi.h"
#include "polynomial.h"
#include "aks_functions.cpp"

using namespace std;

int aks(long n, int id, int procs)
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

//  printf("Proces: %d, test1: %d, test2: %d, logn %f\n", id, test1, test2, logn);
//  fflush(stdout);
//  MPI_Barrier(MPI_COMM_WORLD);

  if (test1)
    return -1;

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

//  printf("Proces: %d, test1: %d, test2: %d, logn %f\n", id, test1, test2, logn);
//  fflush(stdout);
//  MPI_Barrier(MPI_COMM_WORLD);

  if (test2)
    return -1;

//Najwieksze r ze wszystkich procesow
  MPI_Allreduce(&r1, &r2, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

  Polynomial* p=Polynomial::createXa(r2,-1);

//s obliczamy tylko na roocie
  if (id == 0)
    s = (long)(2 * sqrt(r2) * logn);
  MPI_Bcast(&s, 1, MPI_LONG, 0, MPI_COMM_WORLD);

//  MPI_Barrier(MPI_COMM_WORLD);
//  printf("Proces: %d, r2=%ld | q=%ld | s=%ld \n", id, r2, q, s);
//  fflush(stdout);
//  MPI_Barrier(MPI_COMM_WORLD);

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
    return -1;

  printf("Proces: %d, n=%ld to liczba pierwsza\n", id, n);
  fflush(stdout);
  return 0;
}

int main(int argc, char *argv[])
{
  int myid, numprocs, namelen, result = 1, all_results;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  double startwtime = 0.0, endwtime;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  MPI_Get_processor_name(processor_name,&namelen);

  if (argc != 2) {
    if (myid == 0)
      cout << "Zła liczba parametrow. Podaj jako parametr sprawdzana liczbe. \n";
    return -1;
  }
  long number = atol(argv[1]);

  printf("Process %d of %d is on %s\n", myid, numprocs, processor_name);
  fflush(stdout);

  if( number < 2 )
  {
    if (myid == 0) {
      printf("%ld: ani liczba pierwsza, ani zlozona\n", number);
      fflush(stdout);
    }
  }
  else {
    if( number == 2 || number == 3)
    {
      if (myid == 0) {
        printf("%ld: liczba pierwsza\n", number);
        fflush(stdout);
      }
    }
    else {
      if (myid == 0)
        startwtime = MPI_Wtime();


      MPI_Barrier(MPI_COMM_WORLD);
      result = aks(number, myid, numprocs);
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Reduce(&result, &all_results, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

      if (myid == 0) {
        endwtime = MPI_Wtime();
        if (all_results == 0)
          printf("Liczba %ld jest liczba pierwsza.", number);
        else if (all_results == -1)
          printf("Liczba %ld jest liczba zlozona.", number);
        else 
          printf("Cos sie popsulo.");
      	printf("wall clock time = %f\n", endwtime-startwtime);
      	fflush(stdout);
      }
    }
  }

  MPI_Finalize();
  return 0;
}
