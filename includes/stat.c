/*
 * stat.c
 *
 *  Created on: Dec 15, 2014
 *      Author: kodly
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Daje sve funkcije koje nam trebaju da ostavarimo FMindex, ukljucujuci i njega
#include "../includes/utils.h"
// Ukljucuje sve defineove koje koristimo
#include "../includes/defs.h"
// Sadrzi sve strukture podataka koje koristimo i statice varijable (treba ih izbaciti)
#include "../includes/types.h"
// Sadrzava sve funkcije za mjerenje vremena izgradnje, pretrazivanja... standardna deviacija, aritemticka sredina itd.
#include "../includes/stat.h"
// Sadrzi sve funkcije za stvaranje, pretrazivanje i dealokaciju stabla valica
#include "../includes/waveletTree.h"

/**
 * Funkcija vraca substring stringa string s poziyicije position i duzine length
 */
char *substring(char *string, int position, int length) {
	char *pointer;
	int c;

	pointer = (char *) malloc((length + 1) * sizeof(char));

	if (pointer == NULL) {
		printf("Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	for (c = 0; c < position - 1; c++)
		string++;

	for (c = 0; c < length; c++) {
		*(pointer + c) = *string;
		string++;
	}

	*(pointer + c) = '\0';

	return pointer;
}

/**
 * Funkcija generira random podnizove ulaznog niza i ispituje njihovo vrijeme izvodenja u vec kreiranom FM indexu.
 */
double vrijemeZaRandomPodnizoveUlazaOcc(int lengthOfInput, int duzinaAbecede,
		char *S, char *B, char *abeceda, int brojTestnihPrimjerka, int C[]) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja slucajnu duzinu ulznog niza u proracunu statistike (kraj programa)
	int randPocetak = 0, randKraj = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {
		// Uzmi slucajnu duzinu niza
		randPocetak = rand() % lengthOfInput;
		randKraj = randPocetak + rand() % (lengthOfInput - randPocetak);

		// randomPodNiz pointer na random podniz ulaznog niza S
		randomPodNiz = substring(S, randPocetak, randKraj - randPocetak);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindex(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede, C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Oslobdi pointer na novo nastali random podniz
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}

/**
 * Funkcija stvara podnizove ulaza random duzine
 */
double vrijemeZaRandomGeneriranePodnizoveOcc(int brojTestnihPrimjerka,
		int lengthOfInput, char *S, char *B, int duzinaAbecede, char *abeceda, int C[]) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja slucajnu duzinu niza u proracunu statistike (kraj programa)
	int duzinaRandomGeneriranogNiza = 0;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {

		// Uzmi slucajnu duzinu niza
		duzinaRandomGeneriranogNiza = rand() % lengthOfInput;

		randomPodNiz = generirajRandomStringNadAbecedom(abeceda,
				duzinaRandomGeneriranogNiza, duzinaAbecede);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindex(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede, C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Oslobodi zauzetu memoriju
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}

/**
 * Funkcija generira random stringove nad danom abecedom.
 */
char *generirajRandomStringNadAbecedom(char *abeceda,
		int duzinaRandomGeneriranogNiza, int duzinaAbecede) {
	char *niz;
	int i = 0, randomBroj = 0;

	// Pointer na novo nastali niz
	niz = (char *) malloc(sizeof(char) * (duzinaRandomGeneriranogNiza + 1));

	// Jesi li uspio alocirati memoriju?
	if (niz == NULL) {
		printf("Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	// Stvaraj niz, ali tako da ne uzimas znak $ posto nema smisla
	for (i = 0; i < duzinaRandomGeneriranogNiza; i++) {
		// Bez znaka $ jer inace nikada nece biti pogodak posto ulazni niz ima samo jedan taj i to na kraju
		// Inace $ je leksigorafski gledano najmanji u cijeloj abecedi
		randomBroj = rand() % (duzinaAbecede - 1) + 1;
		*(niz + i) = *(abeceda + randomBroj);
	}

	// Zavrsni string i varti ga u glavni program
	*(niz + i) = '\0';

	return niz;
}

/**
 * Funkcija stvara podnizove ulaza random duzine
 */
double vrijemeZaRandomGeneriranePodnizoveZadaneDuzineOcc(int brojTestnihPrimjerka,
		int lengthOfInput, char *S, char *B, int duzinaAbecede, char *abeceda,
		int duzinaPodnizova, double *tablicaVrijednosti, int C[]) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja ZADANU duzinu niza u proracunu statistike (kraj programa)
	int duzinaRandomGeneriranogNiza = duzinaPodnizova;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {

		randomPodNiz = generirajRandomStringNadAbecedom(abeceda,
				duzinaRandomGeneriranogNiza, duzinaAbecede);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindex(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede, C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Dodaj vrijednost u tablicu vrijednosti (vrijeme izvodenja)
		*(tablicaVrijednosti + i) = (((double) endStatTime - beginStatTime)
				/ CLOCKS_PER_SEC);

		// Oslobodi zauzetu memoriju
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}

/**
 * Funkcija generira random podnizove ulaznog niza i ispituje njihovo vrijeme izvodenja u vec kreiranom FM indexu.
 */
double vrijemeZaRandomPodnizoveUlazaZadaneDuljineOcc(int lengthOfInput,
		int duzinaAbecede, char *S, char *B, char *abeceda,
		int brojTestnihPrimjerka, int zadanaDuzinaPodnizaUlaznogNiza,
		double *tablicaVrijednosti, int C[]) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja slucajnu duzinu ulznog niza u proracunu statistike (kraj programa)
	int randPocetak = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {
		// Uzmi slucajnu pocetak niza u ulaznom nizu, pazi da ne premasis cijeli niz
		randPocetak = rand()
				% (lengthOfInput - zadanaDuzinaPodnizaUlaznogNiza - 2);

		// randomPodNiz pointer na random podniz ulaznog niza S
		randomPodNiz = substring(S, randPocetak,
				zadanaDuzinaPodnizaUlaznogNiza);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindex(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede,C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Dodaj vrijednost u tablicu vrijednosti (vrijeme izvodenja)
		*(tablicaVrijednosti + i) = (((double) endStatTime - beginStatTime)
				/ CLOCKS_PER_SEC);

		// Oslobdi pointer na novo nastali random podniz
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}

/**
 * Racuna i vraca standradno deviaciju polje data duzine lengthOfArray
 */
double standardDeviation(double *data, int lengthOfArray,
		double aritmetickaSredina) {
	double sumDeviation = 0.0, kor = 0;
	int i = 0;

	// Izracunaj standardnu deviaciju bez korrjena i podjele
	for (i = 0; i < lengthOfArray; i++) {
		sumDeviation += (data[i] - aritmetickaSredina)
				* (data[i] - aritmetickaSredina);
	}

	kor = sumDeviation / lengthOfArray;

	return sqrt(kor);
}



/**
 * Funkcija generira random podnizove ulaznog niza i ispituje njihovo vrijeme izvodenja u vec kreiranom FM indexu.
 * Pomocu wavelet tree-a.
 */
double vrijemeZaRandomPodnizoveUlazaZadaneDuljineWaveletTree(int lengthOfInput,
		int duzinaAbecede, char *S, char *B, char *abeceda,
		int brojTestnihPrimjerka, int zadanaDuzinaPodnizaUlaznogNiza,
		double *tablicaVrijednosti, int C[], node *cvor) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja slucajnu duzinu ulznog niza u proracunu statistike (kraj programa)
	int randPocetak = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {
		// Uzmi slucajnu pocetak niza u ulaznom nizu, pazi da ne premasis cijeli niz
		randPocetak = rand()
				% (lengthOfInput - zadanaDuzinaPodnizaUlaznogNiza - 2);

		// randomPodNiz pointer na random podniz ulaznog niza S
		randomPodNiz = substring(S, randPocetak,
				zadanaDuzinaPodnizaUlaznogNiza);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindexWaveletTree(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede,cvor,C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Dodaj vrijednost u tablicu vrijednosti (vrijeme izvodenja)
		*(tablicaVrijednosti + i) = (((double) endStatTime - beginStatTime)
				/ CLOCKS_PER_SEC);

		// Oslobdi pointer na novo nastali random podniz
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}

/**
 * Funkcija stvara podnizove ulaza random duzine, pomocu stabla valica
 */
double vrijemeZaRandomGeneriranePodnizoveZadaneDuzineWaveletTree(int brojTestnihPrimjerka,
		int lengthOfInput, char *S, char *B, int duzinaAbecede, char *abeceda,
		int duzinaPodnizova, double *tablicaVrijednosti, int C[], node *cvor) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja ZADANU duzinu niza u proracunu statistike (kraj programa)
	int duzinaRandomGeneriranogNiza = duzinaPodnizova;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {

		randomPodNiz = generirajRandomStringNadAbecedom(abeceda,
				duzinaRandomGeneriranogNiza, duzinaAbecede);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindexWaveletTree(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede, cvor, C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		fflush(stdout);
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		fflush(stdout);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Dodaj vrijednost u tablicu vrijednosti (vrijeme izvodenja)
		*(tablicaVrijednosti + i) = (((double) endStatTime - beginStatTime)
				/ CLOCKS_PER_SEC);

		// Oslobodi zauzetu memoriju
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}


/**
 * Funkcija generira random podnizove ulaznog niza i ispituje njihovo vrijeme izvodenja u vec kreiranom FM indexu.
 * Pomocu wavelet tree-a koji koristi bitove.
 */
double vrijemeZaRandomPodnizoveUlazaZadaneDuljineWaveletTreeBit(int lengthOfInput,
		int duzinaAbecede, char *S, char *B, char *abeceda,
		int brojTestnihPrimjerka, int zadanaDuzinaPodnizaUlaznogNiza,
		double *tablicaVrijednosti, int C[], node *cvor) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja slucajnu duzinu ulznog niza u proracunu statistike (kraj programa)
	int randPocetak = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {
		// Uzmi slucajnu pocetak niza u ulaznom nizu, pazi da ne premasis cijeli niz
		randPocetak = rand()
				% (lengthOfInput - zadanaDuzinaPodnizaUlaznogNiza - 2);

		// randomPodNiz pointer na random podniz ulaznog niza S
		randomPodNiz = substring(S, randPocetak,
				zadanaDuzinaPodnizaUlaznogNiza);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindexWaveletTreeBit(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede,cvor,C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Dodaj vrijednost u tablicu vrijednosti (vrijeme izvodenja)
		*(tablicaVrijednosti + i) = (((double) endStatTime - beginStatTime)
				/ CLOCKS_PER_SEC);

		// Oslobdi pointer na novo nastali random podniz
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}


/**
 * Funkcija stvara podnizove ulaza random duzine, pomocu BITOVNOG stabla valica
 */
double vrijemeZaRandomGeneriranePodnizoveZadaneDuzineWaveletTreeBit(int brojTestnihPrimjerka,
		int lengthOfInput, char *S, char *B, int duzinaAbecede, char *abeceda,
		int duzinaPodnizova, double *tablicaVrijednosti, int C[], node *cvor) {

	int i = 0;

	// Vrijeme koje se koristi u proracunu statistike
	clock_t beginStatTime, endStatTime;

	// Predstavlja ZADANU duzinu niza u proracunu statistike (kraj programa)
	int duzinaRandomGeneriranogNiza = duzinaPodnizova;

	// Aritmeticka sredina, koristi se u proracunu statistike (kraj programa)
	double aritmetickaSredina = 0;

	// Suma koja se koristi u proracunu statistike (kraj programa)
	double sum = 0;

	// Podniz kojeg koristimo u statistici
	char *randomPodNiz;

	// Sada racunamo statistiku sa brojTestnihPrimjerka (minimalno 100 ili vise) podniza ulaza slucajne duzine
	for (i = 0; i < brojTestnihPrimjerka; i++) {

		randomPodNiz = generirajRandomStringNadAbecedom(abeceda,
				duzinaRandomGeneriranogNiza, duzinaAbecede);

		// Izmjeri vrijeme trazenja podniza
		beginStatTime = clock();
		FMindexWaveletTreeBit(randomPodNiz, S, B, abeceda, lengthOfInput, duzinaAbecede, cvor, C);
		endStatTime = clock();

		//printf("\n%2d. %.10f, %s",i+1,((double)endStatTime-beginStatTime) / CLOCKS_PER_SEC, randomPodNiz );
		fflush(stdout);
		printf("\n%4d. %.10f", i + 1,
				((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);
		fflush(stdout);
		sum = sum + (((double) endStatTime - beginStatTime) / CLOCKS_PER_SEC);

		// Dodaj vrijednost u tablicu vrijednosti (vrijeme izvodenja)
		*(tablicaVrijednosti + i) = (((double) endStatTime - beginStatTime)
				/ CLOCKS_PER_SEC);

		// Oslobodi zauzetu memoriju
		free(randomPodNiz);
	}

	// Ispisi statistiku
	aritmetickaSredina = (double) sum / i;

	return aritmetickaSredina;
}

