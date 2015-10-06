/*
 * wavelettree.h
 *
 *  Created on: Dec 15, 2014
 *      Author: kodly
 */

#ifndef WAVELETTREE_H_
#define WAVELETTREE_H_

/**
 * Pretrazuje i vraca rank za odredeni znak u prefiksu promatranog niza.
 * Prefiks se odreduje indeksom u nizu.
 * Rank je broj ponavljanja znaka do u prefiksu oredenom preko indeksa.
 *
 * node - korijen stabla valica
 * znak - znak za kojeg se izracunava rank
 * indeks - do kuda u nizu se izracunava rank (prefiks)
 * abeceda - abeceda koja vrijedu u nixu
 * duljinaAbecede - velicina abecede
 */
int rank(node * tree, char znak, int indeks, char abeceda[], int duljinaAbecede);

/**
 * Stvara novi cvor u stablu valica.
 * Cvori u stablu valica su bit stringovi.
 *
 * bitovi - bit string
 */
node *NoviCvor(char bitovi[]);

/**
 * Gradi stablo valica tako da mu se predaje korijen. Dijeli abecedu na dva dijela
 * i sukladno tome kodira znakova abecede. 0 idu lijevo, a 1 desno.
 * I tako rekurzivno dok djeca ne budu samo jedan znak abecede.
 * Znakovi se ne spremaju nego samo njihove bitne reprezentacije (kodovi)
 *
 * node - korijen stabla valica
 * duljinaNiza - duljina niza nad kojim se radi stablo valica
 * duljinaAbecede - duzina abecede koja vrijedi za niz
 * abeceda - abeceda niza
 * niz - niz nad kojim se gradi stablo valica
 */
void insert(node ** tree, int duljinaNiz, int duljinaAbecede, char *abeceda,
		char *niz);

/**
 * Gradi stablo valica tako da mu se predaje korijen. Dijeli abecedu na dva dijela
 * i sukladno tome kodira znakova abecede. 0 idu lijevo, a 1 desno.
 * I tako rekurzivno dok djeca ne budu samo jedan znak abecede.
 * Znakovi se ne spremaju nego samo njihove bitne reprezentacije (kodovi).
 * Bitovi su spremljeni u bitove, tj. 8 bitoca je utrpano u char za najvecu korisnost.
 *
 * node - korijen stabla valica
 * duljinaNiza - duljina niza nad kojim se radi stablo valica
 * duljinaAbecede - duzina abecede koja vrijedi za niz
 * abeceda - abeceda niza
 * niz - niz nad kojim se gradi stablo valica
 */
void insertBit(node ** tree, int duljinaNiz, int duljinaAbecede, char *abeceda,
		char *niz);

/**
 * Pretrazuje i vraca rank za odredeni znak u prefiksu promatranog niza.
 * Prefiks se odreduje indeksom u nizu.
 * Rank je broj ponavljanja znaka do u prefiksu oredenom preko indeksa.
 *
 * node - korijen stabla valica
 * znak - znak za kojeg se izracunava rank
 * indeks - do kuda u nizu se izracunava rank (prefiks)
 * abeceda - abeceda koja vrijedu u nixu
 * duljinaAbecede - velicina abecede
 * duljinaNiz - duzina ulaznog niza (u glavnom programu)
 */
int rankBit(node * tree, char znak, int indeks, char abeceda[],
		int duljinaAbecede, int duljinaNiz);

#endif /* WAVELETTREE_H_ */
