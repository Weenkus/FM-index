/*
 * stat.h
 *
 *  Created on: Dec 15, 2014
 *      Author: kodly
 */

#ifndef STAT_H_
#define STAT_H_

/**
 * Funkcija vraca substring stringa string s pozicije position i duzine length
 *
 * string - uzimamo njegov podniz
 * position - krecemo od ove pozicije
 * length - duzina podniza
 */
char * substring(char* string, int position, int length);

/**
 *Funkcija generira random podnizove ulaza i ubacuje ih u VEC NAPRAVLJEN FM index te mjeri njihovo vrijeme izvodenja.
 *Vraca aritmeticku sredinu vremena izvodenja. Plus ispisuje pojedina vremena radi kontrole.
 *FMindex je generiran pomocu Occ tablica.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *C[] - prefix sum table
 */
double vrijemeZaRandomPodnizoveUlazaOcc(int lengthOfinput, int duzinaAbecede,
		char *S, char *B, char *abeceda, int brojTestnihPrimjerka, int C[]);

/**
 *Funkcija generira random nizove iz abecede (bez znaka $) i ubacuje ih u VEC NAPRAVLJEN FM index
 *funkcija te mjeri njihovo vrijeme izvodenja.Vraca aritmeticku sredinu vremena izvodenja.
 *Plus ispisuje pojedina vremena radi kontrole.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *C[] - prefix sum table
 */
double vrijemeZaRandomGeneriranePodnizoveOcc(int brojTestnihPrimjerka,
		int lengthOfInput, char *S, char *B, int duzinaAbecede, char *abeceda,
		int C[]);

/**
 * Generira string za zadano duzinu niz i abecedu
 */
char *generirajRandomStringNadAbecedom(char *abeceda,
		int duzinaRandomGeneriranogNiza, int duzinaAbecede);

/**
 *Funkcija generira random nizove iz abecede (bez znaka $) ZADANE DULJINE i ubacuje ih u VEC NAPRAVLJEN FM index
 *funkcija mjeri njihovo vrijeme izvodenja.Vraca aritmeticku sredinu vremena izvodenja.
 *Plus ispisuje pojedina vremena radi kontrole.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *duzinaPodnizova - duzina random generiranih podnizova nad danom abecedom
 *tablicaVrijednosti - sprema brzine izvodenja svakog niza
 *C[] - prefix sum table
 */
double vrijemeZaRandomGeneriranePodnizoveZadaneDuzineOcc(
		int brojTestnihPrimjerka, int lengthOfInput, char *S, char *B,
		int duzinaAbecede, char *abeceda, int duzinaPodnizova,
		double *tablicaVrijednosti, int C[]);

/**
 *Funkcija generira random podnizove (zadane duzine) ulaza i ubacuje ih u VEC NAPRAVLJEN FM index te mjeri njihovo vrijeme izvodenja.
 *Vraca aritmeticku sredinu vremena izvodenja. Plus ispisuje pojedina vremena radi kontrole.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *zadanaDuzinaPodnizaUlaznogNiza - duzina podniza ulaznog niza
 *ablicaVrijednosti - sprema brzine izvodenja svakog niza
 *C[] - prefix sum table
 */
double vrijemeZaRandomPodnizoveUlazaZadaneDuljineOcc(int lengthOfInput,
		int duzinaAbecede, char *S, char *B, char *abeceda,
		int brojTestnihPrimjerka, int zadanaDuzinaPodnizaUlaznogNiza,
		double *tablicaVrijednosti, int C[]);

/**
 * Racuna i vraca standradno deviaciju polje data duzine lengthOfArray
 *
 * data - polje sa vrijednostima od kojih zelimo izracunati standardnu deviaciju
 * lengthOfArray - duzina polja sa vrijednostima od kojih zelimo izracunati standardnu deviaciju
 * aritmetickaSredina - aritemticka sredina od potadaka od kojih zelimo izracunati standardnu deviaciju
 * 						koristimo je jer olaksava racun i skracuje vrijeme izvodenja (imamo ju vec u programu)
 */
double standardDeviation(double *data, int lengthOfArray,
		double aritmetickaSredina);

/**
 *Funkcija generira random podnizove ulaza i ubacuje ih u VEC NAPRAVLJEN FM index te mjeri njihovo vrijeme izvodenja.
 *Vraca aritmeticku sredinu vremena izvodenja. Plus ispisuje pojedina vremena radi kontrole.
 *Radi se o FMindexu ostavrenom pomocu stabla valica.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *C[] - prefix sum table
 **cvor - pokazivac na korijen stablo valica
 */
double vrijemeZaRandomPodnizoveUlazaZadaneDuljineWaveletTree(int lengthOfInput,
		int duzinaAbecede, char *S, char *B, char *abeceda,
		int brojTestnihPrimjerka, int zadanaDuzinaPodnizaUlaznogNiza,
		double *tablicaVrijednosti, int C[], node *cvor);

/**
 *Funkcija generira random nizove iz abecede (bez znaka $) ZADANE DULJINE i ubacuje ih u VEC NAPRAVLJEN FM index
 *funkcija mjeri njihovo vrijeme izvodenja.Vraca aritmeticku sredinu vremena izvodenja.
 *Plus ispisuje pojedina vremena radi kontrole.
 *Radi se o FM indexu ostvarenom pomocu stabla valica.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *duzinaPodnizova - duzina random generiranih podnizova nad danom abecedom
 *tablicaVrijednosti - sprema brzine izvodenja svakog niza
 *C[] - prefix sum table
 **cvor - pokazivac na korijen stabla valica
 */
double vrijemeZaRandomGeneriranePodnizoveZadaneDuzineWaveletTree(
		int brojTestnihPrimjerka, int lengthOfInput, char *S, char *B,
		int duzinaAbecede, char *abeceda, int duzinaPodnizova,
		double *tablicaVrijednosti, int C[], node *cvor);

/**
 *Funkcija generira random podnizove ulaza i ubacuje ih u VEC NAPRAVLJEN FM index te mjeri njihovo vrijeme izvodenja.
 *Vraca aritmeticku sredinu vremena izvodenja. Plus ispisuje pojedina vremena radi kontrole.
 *Koristi stablo valica napravljeno iskljucivo od bitova.
 *Radi se na FM indexu ostavrenom pomocu bitovnog stabla valica.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *C[] - prefix sum table
 **cvor - pokazivac na korijen bitovno stablo valica
 */
double vrijemeZaRandomPodnizoveUlazaZadaneDuljineWaveletTreeBit(
		int lengthOfInput, int duzinaAbecede, char *S, char *B, char *abeceda,
		int brojTestnihPrimjerka, int zadanaDuzinaPodnizaUlaznogNiza,
		double *tablicaVrijednosti, int C[], node *cvor);

/**
 *Funkcija generira random nizove iz abecede (bez znaka $) ZADANE DULJINE i ubacuje ih u VEC NAPRAVLJEN FM index
 *funkcija mjeri njihovo vrijeme izvodenja.Vraca aritmeticku sredinu vremena izvodenja.
 *Plus ispisuje pojedina vremena radi kontrole.
 *Radi se na FM indexu ostavrenom pomocu bitovnog stabla valica.
 *
 *lengthOfInput - duzina ulaznog niza
 *duzinaAbecede - duzina abecede ulaznog niza
 *S - ulazni niz
 *B - Burrows Wheerlerova transofrmacija na ulaznim nizom S
 *abeceda - abeceda ulaznog niza S
 *brojTestnihPrimjerka - broj random podnizova kojih cemo stvoriti (veciji broj veca preciznost!)
 *duzinaPodnizova - duzina random generiranih podnizova nad danom abecedom
 *tablicaVrijednosti - sprema brzine izvodenja svakog niza
 *C[] - prefix sum table
 **cvor - pokazivac na korijen stabla valica
 */
double vrijemeZaRandomGeneriranePodnizoveZadaneDuzineWaveletTreeBit(
		int brojTestnihPrimjerka, int lengthOfInput, char *S, char *B,
		int duzinaAbecede, char *abeceda, int duzinaPodnizova,
		double *tablicaVrijednosti, int C[], node *cvor);

#endif /* STAT_H_ */
