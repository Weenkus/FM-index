/*
 * FMindex.c
 *
 *  Created on: Oct 23, 2014
 *      Author: kodly
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <math.h>

// Daje sve funkcije koje nam trebaju da ostavarimo FMindex, ukljucujuci i njega
#include "../includes/utils.h"
// Ukljucuje sve defineove koje koristimo
#include "../includes/defs.h"
// Sadrzi sve strukture podataka koje koristimo i statice varijable (treba ih izbaciti)
#include "../includes/types.h"
// Sadrzava funkcije za izdradu SA, jos ima BWT i neke druge koje u ovoj implementaciji FMindexa ne koristimo
#include "divsufsort.h"
// Sadrzava sve funkcije za mjerenje vremena izgradnje, pretrazivanja... standardna deviacija, aritemticka sredina itd.
#include "../includes/stat.h"
// Sadrzi sve funkcije za stvaranje, pretrazivanje i dealokaciju stabla valica
#include "../includes/waveletTree.h"

// Occurrence tablica
int **OccTablica;

int main(void) {

	// Spremanje podniza
	char podNiz[PATTER_SIZE + 1];

	// Cijela abeceda ulaznog niza
	char *abeceda = NULL;

	// Ulazni niz
	char *S = NULL;

	// BWT
	char *B = NULL;

	// Pomocne varijable, uglavnom za for i while petlje
	int i = 0;

	// Polje rezultata izvodenja testnih nizova, sluzi nam za racunanje standardne deviacije
	double *tablicaVremenaIzvodenja;

	// Sandardna deviacija
	double standardnaDeviacija = 0;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Duzina ulaznog niza i duzina abecede
	int lengthOfInput = 0, duzinaAbecede = 1;

	// Sufiksnog polje
	int *SA = NULL;

	// Metoda OCCa
	int koristiWaveLetTree = 0, korisnikovaOdluka = 0;

	// Prefix sum table
	int *C = NULL;

	// Korisnika se pita za statistiku
	char zeliStatistiku = 'n';

	// Vrijeme cijelog programa
	clock_t beginProgram, endProgram;

	// Vrijeme za citanje ulaza
	clock_t beginInput, endInput;

	// Vrijeme za izgradnju sufiksnog polja
	clock_t beginSA, endSA;

	// Vrijeme za izgradnju BWT-a
	clock_t beginBWT, endBWT;

	// Vrijeme za izgradnju Abecede
	clock_t beginABC, endABC;

	// Vrijeme za izgradnju OCC tablice
	clock_t beginOCC, endOCC;

	// Vrijeme za ptretrano FM indexa
	clock_t beginSearch, endSearch;

	// Vrijeme za izgradnju prefix sum table
	clock_t beginC, endC;

	// Inicijaliziraj vrijednosti
	node *root = NULL;

	// Omogucuje upis bez se prvo sve ispise
	setbuf(stdout, NULL);

	// pocni brojati vrijeme
	beginProgram = clock();

	// Pocni racunati vrijeme za citanje ulazne datoteke
	beginInput = clock();

	// Seed za statistiku
	srand((unsigned) time(NULL));

	// procitaj ulaz u S
	S = parsirajUlaz(&lengthOfInput);

	// Zavrsni brojanje vremena za citanje ulazne datoteke
	endInput = clock();
	printf("It took %f seconds to read the input file of %d length.",
			(((double) endInput - beginInput) / CLOCKS_PER_SEC), lengthOfInput);

	// Pocni brojati vrijeme izgradnje sufiksnog polja
	beginSA = clock();

	// Izgradi sufiksno polje
	SA = (int *) malloc(lengthOfInput * sizeof(int));
	divsufsort(S, SA, lengthOfInput);

	endSA = clock();
	printf("\nIt took %f seconds to construct the suffix array.",
			(((double) endSA - beginSA) / CLOCKS_PER_SEC));

	// Pocni racunati vrijeme izgradnje BWT-a
	beginBWT = clock();

	//napravi BWT iz SA i S i ispisia ga
	B = (char *) malloc(lengthOfInput * sizeof(char));
	BWT(S, SA, B, lengthOfInput);

	// Ispisi vrijeme izgradnje BWT-a
	endBWT = clock();
	printf("\nIt took %f seconds to construct the BWT.\n",
			(((double) endBWT - beginBWT) / CLOCKS_PER_SEC));

	// Vrijeme izrade abecede
	beginABC = clock();

	// Napravi abecedu preko SA (koji je vec soritran), vrati abecedu i preko adrese njenu duzinu
	abeceda = stvoriAbecedu(S, lengthOfInput, &duzinaAbecede, SA);

	// Ispis trajanje konstrukcije abecede
	endABC = clock();
	printf("It took %f seconds to construct the alphabet.\n",
			((double) (endABC - beginABC) / CLOCKS_PER_SEC));

	// Izgradi prefix sum table, tj. C
	beginC = clock();
	C = createPrefixSumTable(S, lengthOfInput, duzinaAbecede, abeceda);

	// Ispisi vrijeme izgradnje C tablice
	endC = clock();
	printf("It took %f seconds to construct the prefix sum table(C).\n",
			(((double) endC - beginC) / CLOCKS_PER_SEC));

	// Pitaj korisnika zeli li koristit OCC matricu ili stablo valica
	do {
		fflush(stdin);
		printf(
				"Enter 1 for Occ table, 2 for wavelet tree or 3 for bitWavelet tree: ");
		scanf("%d", &korisnikovaOdluka);
	} while (!((korisnikovaOdluka == 1) || (korisnikovaOdluka == 2)
			|| (korisnikovaOdluka == 3)));

	// Sta je korisnik odlucio?
	// Koristi Occ table
	if (korisnikovaOdluka == 1)
		koristiWaveLetTree = 0;
	// Koristi stablo valica
	else if (korisnikovaOdluka == 2)
		koristiWaveLetTree = 1;
	// Koristi bitovno stablo valica
	else
		koristiWaveLetTree = 2;

	// Ovisno o korisnikovom odabiru sagradi sta je potrebno
	// Vrijeme potrebno iz izgraditi matricu OCC
	beginOCC = clock();

	// Korisnik se odlucio za OCC table
	if (koristiWaveLetTree == 0) {
		// stvari 2D matricu sa vrijednostima Occ-a
		stvoriOcc(B, abeceda, lengthOfInput, duzinaAbecede);

		// Ispisi vrijeme potrebno za stvoriti cijelu occ tablicu
		endOCC = clock();
		printf("\nIt took %f seconds to create the full OCC table.",
				((double) (endOCC - beginOCC) / CLOCKS_PER_SEC));

		// Zatrazi od korisnika podniz kojeg zeli provjeriti
		printf("\nEnter the sequence you want to check: ");
		fflush(stdin);
		scanf("%s", &podNiz);

		// Vrijeme potrebno za pretragu FM indexa
		beginSearch = clock();

		// Odradi FMindex (za occ table) i ispisi broj pogodaka
		printf("\nGiven sequence was repeated %d times in the given file. \n ",
				FMindex(&podNiz, S, B, abeceda, lengthOfInput, duzinaAbecede,
						C));
	}
	// Korisnik se odlucio ostvratiti OCC preko wavelet treea
	else if (koristiWaveLetTree == 1) {
		// stvori wavelet tree nad BWTom!!! (vazno)
		insert(&root, lengthOfInput, duzinaAbecede, abeceda, B);

		// Ispisi vrijeme potrebno za napraviti stablo valica
		endOCC = clock();
		printf("\nIt took %f seconds to create the wavelet tree.",
				((double) (endOCC - beginOCC) / CLOCKS_PER_SEC));

		// Zatrazi od korisnika podniz kojeg zeli provjeriti
		printf("\nEnter the sequence you want to check: ");
		fflush(stdin);
		scanf("%s", &podNiz);

		// Vrijeme potrebno za pretragu FM indexa
		beginSearch = clock();

		// Odradi FMindex (za stablo valica) i ispisi broj pogodaka
		printf("\nGiven sequence was repeated %d times in the given file. \n ",
				FMindexWaveletTree(&podNiz, S, B, abeceda, lengthOfInput,
						duzinaAbecede, root, C));
	}
	// Korisnik se odlucio ostvratiti OCC preko BITOVNOG wavelet treea
	else if (koristiWaveLetTree == 2) {
		// stvori wavelet tree nad BWTom!!! (vazno)
		insertBit(&root, lengthOfInput, duzinaAbecede, abeceda, B);

		// Ispisi vrijeme potrebno za napraviti stablo valica
		endOCC = clock();
		printf("\nIt took %f seconds to create the wavelet tree with bits.",
				((double) (endOCC - beginOCC) / CLOCKS_PER_SEC));

		// Zatrazi od korisnika podniz kojeg zeli provjeriti
		printf("\nEnter the sequence you want to check: ");
		fflush(stdin);
		scanf("%s", &podNiz);

		// Vrijeme potrebno za pretragu FM indexa
		beginSearch = clock();

		// Odradi FMindex (za stablo valica) i ispisi broj pogodaka
		printf("\nGiven sequence was repeated %d times in the given file. \n ",
				FMindexWaveletTreeBit(&podNiz, S, B, abeceda, lengthOfInput,
						duzinaAbecede, root, C));
	}
	// Doslo je do greske, ovo se ne bi smijelo nikada doC[] - prefix sum tablegoditi
	else {
		printf(
				"Grska nastala u odabiru metoda izrade OCCa!\n(kontaktirajte svog lokalnog programera)\n");
		exit(0);
	}

	// Vrijeme za trazenje podniza unutar FM indexa
	endSearch = clock();

	// Ispisi vrijeme izvodenja
	endProgram = clock();
	printf(
			"\nIt took %f seconds to SEARCH and BUILD the FMindex including all the data needed to make the FMindex.",
			((double) (endProgram - beginProgram) / CLOCKS_PER_SEC));
	printf("\nIt took %f second to SEARCH the FMindex.",
			(((double) endSearch - beginSearch) / CLOCKS_PER_SEC));

	// Pitaj korisnika ze li vidjeti statistku
	getchar();		// ovo je da ocistim stream, inace scanf ce proletiti
	fflush(stdin);
	printf(
			"\nDo you want to see statistics for the chosen method of FMindex construction(y/n): ");
	scanf("%c", &zeliStatistiku);

	// Korisnik zeli statistiku, napravi ju i ispisi na stdout
	if (zeliStatistiku == 'y') {

		// Korisnik se odlucio za OCC table, porekni statistiku za OCC tablicu
		if (koristiWaveLetTree == 0) {

			// Napravi tablicu izvodenja
			tablicaVremenaIzvodenja = (double *) malloc(
			NUMBER_OF_SAMPLES * sizeof(double));

			// Funkcija vraca prosjecno vrijeme potrebno za pretrazivanje FM indexa s random podnizovima ulaza
			aritmetickaSredina = vrijemeZaRandomPodnizoveUlazaZadaneDuljineOcc(
					lengthOfInput, duzinaAbecede, S, B, abeceda,
					NUMBER_OF_SAMPLES,
					STRING_TESTING_LENGTH, tablicaVremenaIzvodenja, C);

			standardnaDeviacija = standardDeviation(tablicaVremenaIzvodenja,
			NUMBER_OF_SAMPLES, aritmetickaSredina);

			printf(
					"\n\nZa %d uzoraka, ulaza duljine %d, prosjecno vrijeme pretrazivanja je: %f, a standardna deviacija %f",
					NUMBER_OF_SAMPLES, lengthOfInput, aritmetickaSredina,
					standardnaDeviacija);

			// Izbrisi vrijednosti tablice vrijednosti
			memset(tablicaVremenaIzvodenja, 0,
					sizeof(double) * NUMBER_OF_SAMPLES);

			// Funkcija vraca prosjecno vrijeme random nizova za pretrazivanje FM indexa s random abecedom
			aritmetickaSredina =
					vrijemeZaRandomGeneriranePodnizoveZadaneDuzineOcc(
					NUMBER_OF_SAMPLES, lengthOfInput, S, B, duzinaAbecede,
							abeceda,
							STRING_TESTING_LENGTH, tablicaVremenaIzvodenja, C);

			standardnaDeviacija = standardDeviation(tablicaVremenaIzvodenja,
			NUMBER_OF_SAMPLES, aritmetickaSredina);

			printf(
					"\n\nZa %d random generiranih nizova nad abecedom ulaza duljine %d, prosjecno vrijeme pretrazivanja je: %f, \n"
							"a standardna deviacija %f", NUMBER_OF_SAMPLES,
					lengthOfInput, aritmetickaSredina, standardnaDeviacija);

			printf("\n\nThis is for a input file of %d length and testing "
					"strings of %d length in %d samples.\n", lengthOfInput,
			STRING_TESTING_LENGTH, NUMBER_OF_SAMPLES);
		}

		// Korisnik se odlucio ostvratiti OCC preko wavelet treea
		else if (koristiWaveLetTree == 1) {
			// Napravi tablicu izvodenja
			tablicaVremenaIzvodenja = (double *) malloc(
			NUMBER_OF_SAMPLES * sizeof(double));

			// Funkcija vraca prosjecno vrijeme potrebno za pretrazivanje FM indexa s random podnizovima ulaza
			aritmetickaSredina =
					vrijemeZaRandomPodnizoveUlazaZadaneDuljineWaveletTree(
							lengthOfInput, duzinaAbecede, S, B, abeceda,
							NUMBER_OF_SAMPLES,
							STRING_TESTING_LENGTH, tablicaVremenaIzvodenja, C,
							root);

			standardnaDeviacija = standardDeviation(tablicaVremenaIzvodenja,
			NUMBER_OF_SAMPLES, aritmetickaSredina);

			printf(
					"\n\nZa %d uzoraka, ulaza duljine %d, prosjecno vrijeme pretrazivanja je: %f, a standardna deviacija %f",
					NUMBER_OF_SAMPLES, lengthOfInput, aritmetickaSredina,
					standardnaDeviacija);

			// Izbrisi vrijednosti tablice vrijednosti
			memset(tablicaVremenaIzvodenja, 0,
					sizeof(double) * NUMBER_OF_SAMPLES);

			// Funkcija vraca prosjecno vrijeme random nizova za pretrazivanje FM indexa s random abecedom
			aritmetickaSredina =
					vrijemeZaRandomGeneriranePodnizoveZadaneDuzineWaveletTree(
					NUMBER_OF_SAMPLES, lengthOfInput, S, B, duzinaAbecede,
							abeceda,
							STRING_TESTING_LENGTH, tablicaVremenaIzvodenja, C,
							root);

			standardnaDeviacija = standardDeviation(tablicaVremenaIzvodenja,
			NUMBER_OF_SAMPLES, aritmetickaSredina);

			printf(
					"\n\nZa %d random generiranih nizova nad abecedom ulaza duljine %d, prosjecno vrijeme pretrazivanja je: %f, \n"
							"a standardna deviacija %f", NUMBER_OF_SAMPLES,
					lengthOfInput, aritmetickaSredina, standardnaDeviacija);

			printf("\n\nThis is for a input file of %d length and testing "
					"strings of %d length in %d samples.\n", lengthOfInput,
			STRING_TESTING_LENGTH, NUMBER_OF_SAMPLES);
		}

		// Korisnik se odlucio ostvratiti OCC preko BITOVNOG wavelet treea
		else if (koristiWaveLetTree == 2) {
			// Napravi tablicu izvodenja
			tablicaVremenaIzvodenja = (double *) malloc(
			NUMBER_OF_SAMPLES * sizeof(double));

			// Funkcija vraca prosjecno vrijeme potrebno za pretrazivanje FM indexa s random podnizovima ulaza
			aritmetickaSredina =
					vrijemeZaRandomPodnizoveUlazaZadaneDuljineWaveletTreeBit(
							lengthOfInput, duzinaAbecede, S, B, abeceda,
							NUMBER_OF_SAMPLES,
							STRING_TESTING_LENGTH, tablicaVremenaIzvodenja, C,
							root);

			standardnaDeviacija = standardDeviation(tablicaVremenaIzvodenja,
			NUMBER_OF_SAMPLES, aritmetickaSredina);

			printf(
					"\n\nZa %d uzoraka, ulaza duljine %d, prosjecno vrijeme pretrazivanja je: %f, a standardna deviacija %f",
					NUMBER_OF_SAMPLES, lengthOfInput, aritmetickaSredina,
					standardnaDeviacija);

			// Izbrisi vrijednosti tablice vrijednosti
			memset(tablicaVremenaIzvodenja, 0,
					sizeof(double) * NUMBER_OF_SAMPLES);

			// Funkcija vraca prosjecno vrijeme random nizova za pretrazivanje FM indexa s random abecedom
			aritmetickaSredina =
					vrijemeZaRandomGeneriranePodnizoveZadaneDuzineWaveletTreeBit(
					NUMBER_OF_SAMPLES, lengthOfInput, S, B, duzinaAbecede,
							abeceda,
							STRING_TESTING_LENGTH, tablicaVremenaIzvodenja, C,
							root);

			standardnaDeviacija = standardDeviation(tablicaVremenaIzvodenja,
			NUMBER_OF_SAMPLES, aritmetickaSredina);

			printf(
					"\n\nZa %d random generiranih nizova nad abecedom ulaza duljine %d, prosjecno vrijeme pretrazivanja je: %f, \n"
							"a standardna deviacija %f", NUMBER_OF_SAMPLES,
					lengthOfInput, aritmetickaSredina, standardnaDeviacija);

			printf("\n\nThis is for a input file of %d length and testing "
					"strings of %d length in %d samples.\n", lengthOfInput,
			STRING_TESTING_LENGTH, NUMBER_OF_SAMPLES);
		}
	}

	// Dealociraj svu memoriju
	if (koristiWaveLetTree == 0) {
		//provjeri dva puta da nema sve pogasis
		for (i = 0; i < duzinaAbecede; i++) {
			free(OccTablica[i]);
		}
		free(OccTablica);
	}
	// Korsnik je koristio stablo
	else {
		// Izbrsi wavelet tree
		deallocateTree(&root);
	}

	// Izbrisi sve ostalo sto se uvijek koristi
	free(tablicaVremenaIzvodenja);
	free(S);
	free(SA);
	free(B);
	free(abeceda);
	free(C);

	return 0;
}
