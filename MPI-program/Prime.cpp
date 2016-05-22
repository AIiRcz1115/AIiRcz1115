#include "stdafx.h"

using namespace std;


int main(int argc, char *argv[])
{
	int myid, numprocs, namelen, result = 1, all_results;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double startwtime = 0.0, endwtime;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);

	if (argv[1] == "aks") {
		if (argc != 3) {
	      if (myid == 0)
	        cout << "Zła liczba parametrow. Podaj jako parametr sprawdzana liczbe. \n";
	      return -1;
	    }
	    long number = atol(argv[2]);

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
	else if (argv[1] == "mr") {
		if (argc != 4) {
	      if (myid == 0)
	        cout << "Zła liczba parametrow. Podaj jako parametr sprawdzana liczbe. \n";
	      return -1;
	    }
	    long number = atol(argv[2]);
		int attempts = atoi(argv[3]);

		attempts /= numprocs;

		MillerRabin rabin;

		result = rabin.executeAlgorithm(number, attempts);
		MPI_Reduce(&result, &all_results, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		if (all_results == 1) {
			cout << "liczba prawdopodobnie pierwsza";
		}
		else {
			cout << "liczba jest zlozona";
		}
		getchar();
		return 0;
	}
}
