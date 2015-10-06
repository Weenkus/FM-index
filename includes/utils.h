/*
 * utils.h
 *
 *  Created on: Nov 4, 2014
 *      Author: kodly
 */

#ifndef UTILS_H_
#define UTILS_H_

// Daje sve funkcije koje nam trebaju da ostavarimo FMindex, ukljucujuci i njega
#include "../includes/utils.h"
// Ukljucuje sve defineove koje koristimo
#include "../includes/defs.h"
// Sadrzi sve strukture podataka koje koristimo i statice varijable (treba ih izbaciti)
#include "../includes/types.h"

/**
 *Gradi abecedu iz sufksnog polja koji je vec soriran. Razlog tome je sto vec imamo SA, pa
 *nema razloga da ga ne iskoristimo. Pogotovo jer je njegov algoritam stvaranja JAKO optimiziran.
 *
 *S[] - ulaz niz vec prije izparsiran, znaci ima samo potrebne informacije unutra
 *lengthOfInput - duzina izparsiranog ulaznog niza
 **duzinaAbecede - duzina abecede, cilj je izracunati u funkciji i vratiti to glavnom programu preko adrese
 *SA[] - sufiksno polje koje koristimo da bi napravili abecedu
 */
char *stvoriAbecedu(char S[], int lengthOfInput, int *duzinaAbecede, int SA[]);

/**
 * Cita ulaz i sprema ga u S kojeg vraca preko adrese.
 * Odbacuje sve znakove novog reda za Windows i Linux.
 *
 * Jos nije makar trebalo bi ju napraviti da radi za fasta format.
 *
 * lengthOfInput - duzina ulaznog niza
 * return - parsirani ulazni niz
 */
char *parsirajUlaz(int *lengthOfInput);

/**
 *Prima polje znakova S(input), i vraca integer tj. sumu
 *svih znakova leksigorafski manjih od znaka poslanog u funkciju
 *
 ***treba optimizaciju
 ***CAKA mozemo C izracunati preko sufiksnog polja, interval sufiksnog polja
 *
 *znak - znak za kojeg racunamo C
 *input[] - ulazni niz
 *lengthofInput - duzina ulaznog niza input[]
 */
int Cfunk(char znak, const char input[], int lengthOfInput);

/**
 * Racun vrijednost C tablica preko znaka. Radimo to ovako jer C je polje
 * intova i samo u sebi nema pohranjenu vrijednost znaka abecede.
 *
 * znak - znak za kojeg nas zanima vrijednost prefix sum table-a
 * abeceda[] - abeceda ulaznog znaka
 * duzinaAbecede - duzina abecede kreirane nad ulaznim nizom
 * C[] - prefix sum table
 * return - vrijednost polja C[] za dan ulazni znak
 */
int Ccalc(char znak, char abeceda[], int duzinaAbecede, int C[]);

/**
 *Gradi prefix sum table nad zadanim ulazom. C($) = 0, a ostali se racunau
 *tako da se broji pojavljivanje leksikografskih znakova manjih od promatranog znaka.
 *I tako nad cijelim ulazom.
 *
 *input[] - ulazni niz
 *lengthOfInput - duzina ulaza
 *duzinaAbecede - duzina abecede
 *abeceda[] - abeceda za ulazni znak
 *return - prefix sum table, tj. C tablicu
 */
int *createPrefixSumTable(const char input[], int lengthOfInput,
		int duzinaAbecede, char abeceda[]);

/**
 * funkcija gradi BWT od SA i inputa, dakle gradimo BWT na prethodnicima
 *
 * input[] - ulaz (string)
 * SA[] - sufkisno polje
 * B[] - Burrowse Wheeler transformacija
 * length - duzina ulazanog niza
 * */
void BWT(const char input[], int SA[], char B[], int length);

/**
 *Optimiziranija funkcija od funkcija Occ, cilj je napraviti tabblicu sa svim vrijednostima
 *Preuzeo sam algoritam sa: http://stackoverflow.com/questions/13023739/creating-an-2d-array-using-pointer-malloc-and-then-print-it-out
 *
 *BWT - Burrows Wheelerova transformacija na ulaznim nizom
 *abeceda[] - abeceda ulaznog niza
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede
 */
void stvoriOcc(char BWT[], char abeceda[], int lengthOfInput, int duzinaAbecede);

/**
 * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke/
 * Racuna FM index koristeci C i OCC, gdje je OCC ostvaren preko matrica.
 * Za vise informacija vidi funkciju stvoriOcc.
 *
 * input - string koji se pretrazuje
 * B - BWT string
 * duzinaAbecede - velicina abecede
 * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
 * lengthOfInput - duzina ulaznog niza kojeg pretrazujemo
 * duzinaAbecede - duzina abecede
 * C[] - prefix sum table
 * return - broj pojavljivanja podniza u nizu input
 * */
int FMindex(char podniz[], const char input[], char B[], char abeceda[],
		int lengthOfInput, int duzinaAbecede, int C[]);

/**
 * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke.
 * Racuna FM index koristeci C i OCC, gdje je OCC ostvaren preko stabla valica.
 * Za vise informacija pogledaj waveletTree.c/.h
 * Kodirane vrijednosti se spremaju u charove sta nije memorijski ucinkovito.
 *
 * input - string koji se pretrazuje
 * B - BWT string
 * duzinaAbecede - velicina abecede
 * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
 * lengthOfInput - duzina ulaznog niza kojeg pretrazujemo
 * duzinaAbecede - duzina abecede
 * node - korijen stabla valica
 * C[] - prefix sum table
 * */
int FMindexWaveletTree(char podniz[], const char input[], char B[],
		char abeceda[], int lengthOfInput, int duzinaAbecede, node *root, int C[]);

/**
 * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke.
 * Racuna FM index koristeci C i OCC, gdje je OCC ostvaren preko stabla valica.
 * Za vise informacija pogledaj waveletTree.c/.h
 * Ovdje se vrijednosti (kodirane spremaju u bitove).
 *
 * input - string koji se pretrazuje
 * B - BWT string
 * duzinaAbecede - velicina abecede
 * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
 * lengthOfInput - duzina ulaznog niza kojeg pretrazujemo
 * duzinaAbecede - duzina abecede
 * node - korijen stabla valica
 * C[] - prefix sum table
 * */
int FMindexWaveletTreeBit(char podniz[], const char input[], char B[],
		char abeceda[], int lengthOfInput, int duzinaAbecede, node *root, int C[]);

#endif /* UTILS_H_ */
