/*
 * waveletTree.c
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

// Maska za rad nad bitovima
unsigned char maska[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

/* Wavelet tree */
void insert(node ** tree, int duljinaNiz, int duljinaAbecede, char *abeceda,
		char *niz) {

	//printf("%s   %d\n", niz, duljinaAbecede);

	int imaNula = 0;
	int i = 0;
	int j = 0;
	int znakPrveAbecede = 0;
	int brojac = 0;
	int brojacNula = 0;
	char pomocni[2];
	char *bitovi = (char *) malloc(duljinaNiz + 1);
	//char *i+1] = '\0'pointerNaNePointovano = bitovi;
	//char bitovi[duljinaniz+1];

	char *sljedeciPodniz1 = NULL, *sljedeciPodniz2 = NULL;

	char *prvaAbeceda = (char *) malloc(duljinaAbecede / 2);
	char *drugaAbeceda = (char *) malloc(duljinaAbecede - duljinaAbecede / 2);

	// Inicijaliziraj
	//memset(bitovi, '\0', sizeof(bitovi) +1);
	memset(prvaAbeceda, '\0', sizeof(char) * ((duljinaAbecede / 2) - 1));
	memset(drugaAbeceda, '\0',
			sizeof(char) * ((duljinaAbecede - duljinaAbecede / 2) - 1));

	/* Izlucivanje dvije abecede */
	// Napravi prvu abecedu
	for (i = 0; i < duljinaAbecede / 2; i++) {
		prvaAbeceda[i] = abeceda[i];
		brojac++;
	}

	// Napravi drugu abecedu
	for (i = 0; i < duljinaAbecede - duljinaAbecede / 2; i++) {
		drugaAbeceda[i] = abeceda[i + duljinaAbecede / 2];
	}

	/* Petlja za stvaranje podatka unutar cvora i znakova sljedeceg cvora s pripadnim abecedama */
	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				imaNula++;
			}
		}
	}

	sljedeciPodniz1 = (char *) malloc(imaNula + 1);
	memset(sljedeciPodniz1, '\0', sizeof(char) * (imaNula));

	sljedeciPodniz2 = (char *) malloc(duljinaNiz - imaNula + 1);
	memset(sljedeciPodniz2, '\0', sizeof(char) * (duljinaNiz - imaNula));

	/* Petlja za stvaranje podatka unutar cvora i znakova sljedeceg cvora s pripadnim abecedama */
	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				znakPrveAbecede = 1;
			}
		}
		pomocni[0] = *(niz + i);
		// Znak pripada prvoj abecedi
		if (znakPrveAbecede == 1) {
			//strncat(bitovi, "0", 1);
			bitovi[i] = '0';
			//bitovi[i+1] = '\0';

			strncat(sljedeciPodniz1, pomocni, 1);

			brojacNula++; /* brojac nula mi je duljina sljedeceg podniza */
			// Znak prpada drugoj abecedi
		} else {
			//strncat(bitovi, "1", 1);
			bitovi[i] = '1';
			//bitovi[i+1] = '\0';

			strncat(sljedeciPodniz2, pomocni, 1);
		}
		znakPrveAbecede = 0;
	}

	// Stvori novi cvor, pazi da to nije dijete
	if (*tree == NULL) {
		//puts(bitovi);
		*tree = NoviCvor(bitovi);
	}
	// Pozovi rekurziju
	if (duljinaAbecede > 2) {
		if (duljinaAbecede - duljinaAbecede / 2 > 1) {
			insert(&((*tree)->right), duljinaNiz - brojacNula,
					duljinaAbecede - duljinaAbecede / 2, drugaAbeceda,
					sljedeciPodniz2);
		}
		if (duljinaAbecede / 2 > 1) {
			insert(&((*tree)->left), brojacNula, duljinaAbecede / 2,
					prvaAbeceda, sljedeciPodniz1);
		}
	}

	// Oslobodi svu zauzetu memoriju
	free(prvaAbeceda);
	free(drugaAbeceda);
	free(sljedeciPodniz1);
	free(sljedeciPodniz2);
}

// Rekurzivno obidi stablo i obrisi ga
void deallocateTree(node ** tree) {

	// Ako dodes do kraja vrati se razinu gore
	if (*tree == NULL) {
		return;
	}
	// Stablo je binarno, tako ga i obidi
	deallocateTree(&((*tree)->left));
	deallocateTree(&((*tree)->right));
	// Izbrisi sve podatke
	free((*tree)->val);
	free(*tree);
}

// Stvori novi cvor
node *NoviCvor(char bitovi[]) {
	node *novi = (node *) malloc(sizeof(node));
	novi->val = bitovi;
	novi->left = NULL;
	novi->right = NULL;
	return novi;
}




// Pokusaj rekurzivnog ranka
int rank(node * cvor, char znak, int indeks, char abeceda[], int duljinaAbecede) {

	if( strlen(cvor->val) < indeks)
		return 0;

	int znakPripradaLijevoAbecedi = 0, znakPripadaDesnojAbecedi = 0;
	char *trenutnaAbeceda = (char *) malloc(duljinaAbecede + 1);
	char *sljedecaAbceda = (char *)malloc(duljinaAbecede + 1);
	int i = 0, noviIndeks = 0;
	// Prepisi abecedu
	for (i = 0; i < duljinaAbecede; i++) {
		trenutnaAbeceda[i] = abeceda[i];
		sljedecaAbceda[i] = abeceda[i];
	}
	trenutnaAbeceda[i] = '\0';
	// Saznaj o kojem dijelu abecede se radi (lijevom ili desnom)
	// Ovo vrijedi samo za korjen
	indeks = indeks + 1;
	do {
		//puts(trenutnaAbeceda);
		//printf("%d\n",indeks);
		znakPripradaLijevoAbecedi = 0;
		znakPripadaDesnojAbecedi = 0;
		// Provjeri za lijevu stranu
		for (i = 0; i < duljinaAbecede / 2; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripradaLijevoAbecedi = 1;
				break;
			}
		}
		// Provjeri za desnu stranu
		for (i = duljinaAbecede / 2; i < duljinaAbecede ; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripadaDesnojAbecedi = 1;
				break;
			}
		}
		// Prebroji 0 ili 1 ovisno o abecedi i znaku
		// Ocito je da se radi o nekoj nebulozi
		if (znakPripradaLijevoAbecedi == 0 && znakPripadaDesnojAbecedi == 0)
			return 0;
		// Radi se o znaku koji pripada lijevoj strani abecede
		else if (znakPripradaLijevoAbecedi == 1
				&& znakPripadaDesnojAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				// izracunaj novi indeks za sljedeci cvor
				if (cvor->val[i] == '0')
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcije
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede / 2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede / 2; i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->left;
		}
		// Radi se o znaku koji pripada desnoj strani abecede
		else if (znakPripadaDesnojAbecedi == 1
				&& znakPripradaLijevoAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				// Izracunaj indeks za novi cvor
				if (cvor->val[i] == '1')
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcijebr
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede - duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede - duljinaAbecede/2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede - (duljinaAbecede / 2); i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i + (duljinaAbecede / 2)];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->right;
		}
		// Namjesti da u sljedecm cvor ide se do onog znaka do kojeg se upravo doslo
		indeks = noviIndeks;
		noviIndeks = 0;
		// Namjesti abecede za sljedece izvodenje
		for( i = 0; i < duljinaAbecede; i++) {
			sljedecaAbceda[i] = trenutnaAbeceda[i];
		}
		sljedecaAbceda[i] = '\0';
	} while (cvor != NULL || duljinaAbecede > 2);
	free(sljedecaAbceda);
	free(trenutnaAbeceda);
	return indeks;
}



//// Vraca rank tj. Occ
//int rank(node * tree, char znak, int indeks, char abeceda[], int duljinaAbecede) {
//
////	printf("Znak %c\nIndeks %d\nAbeceda %s\nduljina abecede %d\n", znak, indeks,
////			abeceda, duljinaAbecede);
//
//	int prviPomocniIndeks = 1;
//
//	// Provjeri da netko ne pokusava pitati za indeks duzi od ulaznog niza
//	if (strlen(tree->val) < indeks)
//		return 0;
//
//	// Inicijaliziraj
//	int zastavica = 1;
//	int i = 0;
//	int j = 0;
//	int pomocniIndeks = 0;
//	int brojac = 0;
//	char znakJeNula = '0';
//	char *pomocnaAbeceda = (char *) malloc(duljinaAbecede + 1);
//
//	// resetiraj abecedu
//	memset(pomocnaAbeceda, '\0', duljinaAbecede + 1);
//
//	char *pointerNaNePointovano = pomocnaAbeceda;
//
//	// Napravi pomocnu abecedu od dane
//	for (i = 0; i < duljinaAbecede; i++) {
//		pomocnaAbeceda[i] = abeceda[i];
//	}
//	// Obavezno ocisti sta si koristio
//	i = 0;
//
//	// Idi do pola niza ili ako nemas gdje prestani!
//	while (duljinaAbecede > 2 && tree != NULL) {
//		pomocniIndeks = 0;
//		if (duljinaAbecede / 2 > 1) {
//			for (i = 0; i < duljinaAbecede / 2; i++) {
//				// Znak priprada lijevoj polovici abecede
//				if (pomocnaAbeceda[i] == znak) {
//					zastavica = 0;
//					break;
//				}
//			}
//		}
//		// Mora biti manje jednak jer je tako po definiciji
//		for (i = 0; i <= indeks; i++) {
//			// Broji 0 jer je znak na lijevoj strani
//			if (('0' == (char) (tree)->val[i]) && zastavica == 0) {
//				pomocniIndeks++;
//			// Broji 1 jer je znak na desnoj strani
//			} else if (('1' == (char) (tree)->val[i]) && zastavica == 1)
//				pomocniIndeks++;
//		}
//		// Obradi lijevu stranu
//		if (zastavica == 0) {
//			// Odi u lijevi cvor i tamo izracunaj novu abecedu
//			tree = (tree)->left;
//			// Izracunaj novu abeccedu
//			for (j = 0; j < duljinaAbecede / 2; j++) {
//				pomocnaAbeceda[j] = abeceda[j];
//			}
//			// Dodaj terminator na kraju nove abecede
//			pomocnaAbeceda[duljinaAbecede / 2] = '\0';
//			abeceda = pomocnaAbeceda;
//			duljinaAbecede /= 2;
//		// Obradi desnu stranu
//		} else {
//			// Odi u desni cvor i tamo izracunaj novu abecedu
//			tree = (tree)->right;
//			// Izracunaj novu abecedu
//			for (j = 0; j < duljinaAbecede - duljinaAbecede / 2; j++) {
//				pomocnaAbeceda[j] = abeceda[j + duljinaAbecede / 2];
//			}
//			// Dodaj terminator na kraj
//			pomocnaAbeceda[duljinaAbecede - duljinaAbecede / 2] = '\0';
//			abeceda = pomocnaAbeceda;
//			// Mora napraviti desnu abecedu
//			//duljinaAbecede -= duljinaAbecede / 2;
//			duljinaAbecede = duljinaAbecede - (duljinaAbecede / 2);
//		}
//		zastavica = 1;
//		// Namjesti indeks za sljedecu razinu stabla
//		if( prviPomocniIndeks ) {
//			indeks = pomocniIndeks - 1;
//		} else {
//			indeks = pomocniIndeks;
//		}
//		prviPomocniIndeks = 0;
//	}
//
//	if (abeceda[0] == znak) {
//		znakJeNula = '0';
//	} else {
//		znakJeNula = '1';
//	}
//	// Prodi po listu i prebroji OCC
//	for (i = 0; i < pomocniIndeks; i++) {
//		if (znakJeNula == (tree)->val[i]) {
//			brojac++;
//		}
//	}
//
//	// Oslobodi zauzetu memoriju
//	free(pointerNaNePointovano);
//	return brojac;
//}

//// Zoranov rank
//int rank(node * tree, char znak, int indeks, char abeceda[],
//		int duljinaAbecede) {
//
//	int zastavica = 0;
//	int i = 0;
//	int j = 0;
//	int pomocniIndeks = 0;
//	int brojac = 0;
//	char znakJeNula = '0';
//	char *pomocnaAbeceda = (char *) malloc(duljinaAbecede + 1);
//
//	memset(pomocnaAbeceda, '\0', duljinaAbecede + 1);
//
//	char *pointerNaNePointovano = pomocnaAbeceda;
//
//	for(i = 0; i < duljinaAbecede; i++) {
//		pomocnaAbeceda[i] = abeceda[i];
//	}
//	while (duljinaAbecede > 2 && tree != NULL) {
//		pomocniIndeks = 0;
//		for ( i = 0 ; i < duljinaAbecede ;i++ ) {
//			if ( pomocnaAbeceda[i] == znak && i < duljinaAbecede/2 ){
//				if ( duljinaAbecede /2 > 1 ){
//					zastavica = 1;
//					break;
//				}
//			} else if ( pomocnaAbeceda[i] == znak && i >= duljinaAbecede/2 ){
//				if ( duljinaAbecede - duljinaAbecede/2 > 1) {
//					zastavica = 2;
//					break;
//				}
//			}
//		}
//		for (i = 0; i <= indeks; i++) {
//			if (('0' == (char) (tree)->val[i]) && zastavica == 1) {
//				pomocniIndeks++;
//			} else if (('1' == (char) (tree)->val[i]) && zastavica == 2)
//				pomocniIndeks++;
//		}
//		if (zastavica == 1) {
//			tree = (tree)->left;
//			for (j = 0; j < duljinaAbecede / 2; j++) {
//				pomocnaAbeceda[j] = abeceda[j];
//			}
//			pomocnaAbeceda[duljinaAbecede / 2] = '\0';
//			abeceda = pomocnaAbeceda;
//			duljinaAbecede /= 2;
//		} else if ( zastavica == 2 ) {
//			tree = (tree)->right;
//			for (j = 0; j < duljinaAbecede - duljinaAbecede / 2; j++) {
//				pomocnaAbeceda[j] = abeceda[j + duljinaAbecede / 2];
//			}
//			pomocnaAbeceda[duljinaAbecede - duljinaAbecede / 2] = '\0';
//			abeceda = pomocnaAbeceda;
//			duljinaAbecede -= duljinaAbecede / 2;
//		}
//		if ( pomocniIndeks == 0 ) {
//			return 0;
//		} else {
//			zastavica = 0;
//			indeks = pomocniIndeks - 1;
//		}
//	}
//
//
//	if (abeceda[0] == znak) {
//		znakJeNula = '0';
//	} else {
//		znakJeNula = '1';
//	}
//	for (i = 0; i < pomocniIndeks; i++) {
//		if (znakJeNula == (tree)->val[i]) {
//			brojac++;
//		}
//	}
//	free(pointerNaNePointovano);
//	return brojac;
//}

//// Pokusaj rekurzivnog ranka
//int rank(node * cvor, char znak, int indeks, char abeceda[], int duljinaAbecede) {
//
//	int znakPripradaLijevoAbecedi = 0, znakPripadaDesnojAbecedi = 0;
//	char *trenutnaAbeceda = (char *) malloc(duljinaAbecede + 1);
//	char *sljedecaAbceda = NULL;
//	int i = 0, noviIndeks = 0, br = 0;
//	// Prepisi abecedu
//	for (i = 0; i < duljinaAbecede; i++) {
//		trenutnaAbeceda[i] = abeceda[i];
//	}
//	trenutnaAbeceda[i] = '\0';
//	// Saznaj o kojem dijelu abecede se radi (lijevom ili desnom)
//	// Provjeri za lijevu stranu
//	for (i = 0; i < duljinaAbecede / 2; i++) {
//		if (znak == trenutnaAbeceda[i]) {
//			znakPripradaLijevoAbecedi = 1;
//			break;
//		}
//	}
//	// Provjeri za desnu stranu
//	for (i = duljinaAbecede / 2; i < duljinaAbecede - duljinaAbecede / 2; i++) {
//		if (znak == trenutnaAbeceda[i]) {
//			znakPripadaDesnojAbecedi = 1;
//			break;
//		}
//	}
//	// Prebroji 0 ili 1 ovisno o abecedi i znaku
//	if (znakPripradaLijevoAbecedi == 0 && znakPripadaDesnojAbecedi == 0)
//		return 0;
//	// Radi se o znaku koji pripada desnoj strani abecede
//	else if (znakPripradaLijevoAbecedi == 1 && znakPripadaDesnojAbecedi == 0) {
//		for (i = 0; i < indeks; i++) {
//			if (cvor->val[i] == '0')
//				noviIndeks++;
//		}
//		// Izracunaj novu lijevu abecedu za novi poziv funkcije
//		sljedecaAbceda = (char *) malloc(duljinaAbecede / 2 + 1);
//		int duzinaNoveAbeede = 0;
//		for (i = 0; i < duljinaAbecede / 2; i++) {
//			sljedecaAbceda[i] = trenutnaAbeceda[i];
//			duzinaNoveAbeede++;
//		}
//		sljedecaAbceda[i] = '\0';
//		// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
//		return br + rank(cvor->left,znak, noviIndeks,sljedecaAbceda,duzinaNoveAbeede);
//	}
//	// Radi se o znaku koji pripada lijevoj strani abecede
//	else if (znakPripadaDesnojAbecedi == 1 && znakPripradaLijevoAbecedi == 0) {
//		for (i = 0; i < indeks; i++) {
//			if (cvor->val[i] == '1')
//				noviIndeks++;
//		}
//		// Izracunaj novu lijevu abecedu za novi poziv funkcije
//		sljedecaAbceda = (char *) malloc(duljinaAbecede / 2 + 1);
//		int duzinaNoveAbeede = 0;
//		for (i = 0; i < duljinaAbecede - (duljinaAbecede / 2); i++) {
//			sljedecaAbceda[i] = trenutnaAbeceda[i +( duljinaAbecede/2)];
//			duzinaNoveAbeede++;
//		}
//		sljedecaAbceda[i] = '\0';
//		// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
//		return br + rank(cvor->right,znak, noviIndeks,sljedecaAbceda,duzinaNoveAbeede);
//	}
//	free(sljedecaAbceda);
//	free(trenutnaAbeceda);
//}

// Stvara stablo valica s bitovima
void insertBit(node ** tree, int duljinaNiz, int duljinaAbecede, char *abeceda,
		char *niz) {

	int imaNula = 0;
	int i = 0;
	int j = 0;
	int znakPrveAbecede = 0;
	int brojac = 0;
	int brojacNula = 0;
	char pomocni[2];
	char *bitovi =
			duljinaNiz % 8 ?
					(char *) malloc(duljinaNiz / 8 + 1) :
					(char *) malloc(duljinaNiz / 8);
	char *sljedeciPodniz1 = NULL, *sljedeciPodniz2 = NULL;
	char *prvaAbeceda = (char *) malloc(duljinaAbecede / 2);
	char *drugaAbeceda = (char *) malloc(duljinaAbecede - duljinaAbecede / 2);

	// Inicijaliziraj
	memset(prvaAbeceda, '\0', sizeof(char) * ((duljinaAbecede / 2) - 1));
	memset(drugaAbeceda, '\0',
			sizeof(char) * ((duljinaAbecede - duljinaAbecede / 2) - 1));

	// Napravi prvu abecedu
	for (i = 0; i < duljinaAbecede / 2; i++) {
		prvaAbeceda[i] = abeceda[i];
		brojac++;
	}

	// Napravi drugu abecedu
	for (i = 0; i < duljinaAbecede - duljinaAbecede / 2; i++) {
		drugaAbeceda[i] = abeceda[i + duljinaAbecede / 2];
	}

	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				imaNula++;
			}
		}
	}

	// Napravi dva podniza i oba inicijaliziraj
	sljedeciPodniz1 = (char *) malloc(imaNula + 1);
	memset(sljedeciPodniz1, '\0', sizeof(char) * (imaNula));

	sljedeciPodniz2 = (char *) malloc(duljinaNiz - imaNula + 1);
	memset(sljedeciPodniz2, '\0', sizeof(char) * (duljinaNiz - imaNula));

	// Napravi bitove za cvor
	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				znakPrveAbecede = 1;
			}
		}
		pomocni[0] = *(niz + i);
		// Znak pripada prvoj abecedi
		if (znakPrveAbecede == 1) {
			*(bitovi + i / 8) = setBit(i, 0);
			strncat(sljedeciPodniz1, pomocni, 1);
			brojacNula++;
		}
		// Znak pripada drugoj abecedi
		else {
			*(bitovi + i / 8) = setBit(i, 1);
			strncat(sljedeciPodniz2, pomocni, 1);
		}
		znakPrveAbecede = 0;
	}
	// Napravi novi cvor
	if (*tree == NULL) {
		*tree = NoviCvor(bitovi);
	}
	// Pozovi rekurziju
	if (duljinaAbecede > 2) {
		if (duljinaAbecede - duljinaAbecede / 2 > 1) {
			insertBit(&((*tree)->right), duljinaNiz - brojacNula,
					duljinaAbecede - duljinaAbecede / 2, drugaAbeceda,
					sljedeciPodniz2);
		}
		if (duljinaAbecede / 2 > 1) {
			insertBit(&((*tree)->left), brojacNula, duljinaAbecede / 2,
					prvaAbeceda, sljedeciPodniz1);
		}
	}
	// Oslobodi memoriju
	free(prvaAbeceda);
	free(drugaAbeceda);
	free(sljedeciPodniz1);
	free(sljedeciPodniz2);
}

//// Racuna rank za bitovno stablo valica
//int rankBit(node * tree, char znak, int indeks, char abeceda[],
//		int duljinaAbecede, int duljinaNiz) {
//
//	// Pazi da indeks ne preleti niz
//	if (duljinaNiz < indeks)
//		return 0;
//
//	// Definicija
//	int zastavica = 1;
//	int i = 0;
//	int j = 0;
//	int pomocniIndeks = 0;
//	int brojac = 0;
//	int znakJeNula = 0;
//	char bitovi[1];
//	char *pomocnaAbeceda = (char *) malloc(duljinaAbecede + 1);
//
//	// Inicijalizacija
//	memset(pomocnaAbeceda, '\0', duljinaAbecede + 1);
//
//	// Prekopiraj pomocnu abecedu
//	char *pointerNaNePointovano = pomocnaAbeceda;
//	for (i = 0; i < duljinaAbecede; i++) {
//		pomocnaAbeceda[i] = abeceda[i];
//	}
//	while (duljinaAbecede > 2 && tree != NULL) {
//
//		pomocniIndeks = 0;
//		if (duljinaAbecede / 2 > 1) {
//			for (i = 0; i < duljinaAbecede / 2; i++) {
//				if (pomocnaAbeceda[i] == znak) {
//					zastavica = 0;
//					break;
//				}
//			}
//		}
//		for (i = 0; i <= indeks; i++) {
//			bitovi[0] = (tree)->val[i / 8];
//			if ((0 == getRankBit(i)) && zastavica == 0) {
//				pomocniIndeks++;
//			} else if ((1 == getRankBit(i)) && zastavica == 1)
//				pomocniIndeks++;
//		}
//		if (zastavica == 0) {
//			tree = (tree)->left;
//			for (j = 0; j < duljinaAbecede / 2; j++) {
//				pomocnaAbeceda[j] = abeceda[j];
//			}
//			pomocnaAbeceda[duljinaAbecede / 2] = '\0';
//			abeceda = pomocnaAbeceda;
//			duljinaAbecede /= 2;
//		} else {
//			tree = (tree)->right;
//			for (j = 0; j < duljinaAbecede - duljinaAbecede / 2; j++) {
//				pomocnaAbeceda[j] = abeceda[j + duljinaAbecede / 2];
//			}
//			pomocnaAbeceda[duljinaAbecede - duljinaAbecede / 2] = '\0';
//			abeceda = pomocnaAbeceda;
//			duljinaAbecede -= duljinaAbecede / 2;
//		}
//		zastavica = 1;
//		indeks = pomocniIndeks - 1;
//	}
//	if (abeceda[0] == znak) {
//		znakJeNula = 0;
//	} else {
//		znakJeNula = 1;
//	}
//	for (i = 0; i < pomocniIndeks; i++) {
//		bitovi[0] = (tree)->val[i / 8];
//		if (znakJeNula == getRankBit(i)) {
//			brojac++;
//		}
//	}
//	free(pointerNaNePointovano);
//	return brojac;
//}

// Pokusaj rekurzivnog ranka
int rankBit(node * cvor, char znak, int indeks, char abeceda[], int duljinaAbecede, int duljinaNiz) {

 	//Pazi da indeks ne preleti niz
	if (duljinaNiz < indeks)
		return 0;

	int znakPripradaLijevoAbecedi = 0, znakPripadaDesnojAbecedi = 0;
	char *trenutnaAbeceda = (char *) malloc(duljinaAbecede + 1);
	char *sljedecaAbceda = (char *)malloc(duljinaAbecede + 1);
	char bitovi[1];
	int i = 0, noviIndeks = 0;
	// Prepisi abecedu
	for (i = 0; i < duljinaAbecede; i++) {
		trenutnaAbeceda[i] = abeceda[i];
		sljedecaAbceda[i] = abeceda[i];
	}
	trenutnaAbeceda[i] = '\0';
	// Saznaj o kojem dijelu abecede se radi (lijevom ili desnom)
	// Ovo vrijedi samo za korjen
	indeks = indeks + 1;
	do {
		//puts(trenutnaAbeceda);
		//printf("%d\n",indeks);
		znakPripradaLijevoAbecedi = 0;
		znakPripadaDesnojAbecedi = 0;
		// Provjeri za lijevu stranu
		for (i = 0; i < duljinaAbecede / 2; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripradaLijevoAbecedi = 1;
				break;
			}
		}
		// Provjeri za desnu stranu
		for (i = duljinaAbecede / 2; i < duljinaAbecede ; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripadaDesnojAbecedi = 1;
				break;
			}
		}
		// Prebroji 0 ili 1 ovisno o abecedi i znaku
		// Ocito je da se radi o nekoj nebulozi
		if (znakPripradaLijevoAbecedi == 0 && znakPripadaDesnojAbecedi == 0)
			return 0;
		// Radi se o znaku koji pripada lijevoj strani abecede
		else if (znakPripradaLijevoAbecedi == 1
				&& znakPripadaDesnojAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				bitovi[0] = (cvor)->val[i / 8];
				// izracunaj novi indeks za sljedeci cvor
				if (getRankBit(i) == 0)
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcije
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede / 2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede / 2; i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->left;
		}
		// Radi se o znaku koji pripada desnoj strani abecede
		else if (znakPripadaDesnojAbecedi == 1
				&& znakPripradaLijevoAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				bitovi[0] = (cvor)->val[i / 8];
				// Izracunaj indeks za novi cvor
				if (getRankBit(i) == 1)
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcijebr
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede - duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede - duljinaAbecede/2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede - (duljinaAbecede / 2); i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i + (duljinaAbecede / 2)];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->right;
		}
		// Namjesti da u sljedecm cvor ide se do onog znaka do kojeg se upravo doslo
		indeks = noviIndeks;
		noviIndeks = 0;
		// Namjesti abecede za sljedece izvodenje
		for( i = 0; i < duljinaAbecede; i++) {
			sljedecaAbceda[i] = trenutnaAbeceda[i];
		}
		sljedecaAbceda[i] = '\0';
	} while (cvor != NULL || duljinaAbecede > 2);
	free(sljedecaAbceda);
	free(trenutnaAbeceda);
	return indeks;
}
