#include "stdafx.h"

using namespace std;


/*
Opis zwracanych wartosci:
1 - sprawdzana liczba to liczba pierwsza
0 - sprawdzana liczba jest liczba zlozona
-1 - nie ma takiego algorytmu
-2 - zla liczba parametrow
*/

int main ( int argc , char *argv[] )
{
	// sprawdzenie liczby parametrow
	if ( argc != 3 && argc != 4 ) {
		cout << -2 << endl;
		return -2;
	}
	// sprawdzenie nazwy algorytmu
	if ( strcmp ( argv[1] , "aks" ) && strcmp ( argv[1] , "mr" ) ) {
		cout << -1 << endl;
		return -1;
	}

	// zmienne
	char *end;
	int myid, numprocs, namelen;
	char processor_name [ MPI_MAX_PROCESSOR_NAME ];
	double startwtime = 0.0, endwtime;
	unsigned long long number = strtoull ( argv[2], &end, 10 );
	unsigned long long attempts;
	unsigned short int result, all_results;

	// podstawowe sprawdzenie
	if ( number < 2 || number % 2 == 0) {
		cout << 0 << endl;
		return 0;
	}
	else if ( number == 2 || number == 3) {
		cout << 1 << endl;
		return 1;
	}

	// inicjalizacja MPI
	MPI_Init ( &argc , &argv );
	MPI_Comm_size ( MPI_COMM_WORLD , &numprocs );
	MPI_Comm_rank ( MPI_COMM_WORLD , &myid );
	MPI_Get_processor_name ( processor_name , &namelen );

	//cout << endl << "Proces " << myid << " / " << numprocs << " na " << processor_name << endl;

	// wywolanie algorytmu
	if ( !strcmp ( argv[1] , "aks" ) ) {

		AKS aks;

		result = aks.executeAlgorithm ( (long)number , myid , numprocs );
	}
	else if ( !strcmp(argv[1], "mr" ) ) {

		MillerRabin m_r;

		attempts = strtoull ( argv[3], &end, 10 ) / numprocs;

		result = m_r.executeAlgorithm ( number , attempts , myid , numprocs );
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Reduce(&result, &all_results, 1, MPI_UNSIGNED_SHORT, MPI_MIN, 0, MPI_COMM_WORLD);

	// koniec pomiaru czasu
	if ( myid == 0 )
		endwtime = MPI_Wtime();

	fflush(stdout);
	if ( myid == 0 ) {
		if ( all_results == 1 ) {
			if ( !strcmp ( argv[1] , "aks" ) )
				cout << endl << "Liczba " << number << " to liczba pierwsza." << endl;
			else if ( !strcmp(argv[1], "mr" ) )
				cout << endl << "Liczba " << number << " to prawdopodobnie liczba pierwsza." << endl;
		}
		else if ( all_results == 0 )
	    		cout << endl << "Liczba " << number << " to liczba zlozona." << endl;
		else
			cout << endl << "Cos sie popsulo!!!!! all_results: " << all_results << endl;
	}

	MPI_Finalize();
	return all_results;
}
