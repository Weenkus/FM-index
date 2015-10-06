/*
 * utils.c
 *
 *  Created on: Nov 3, 2014
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
 *Gradi abecedu iz sufksnog polja koji je vec soriran. Razlog tome je sto vec imamo SA, pa
 *nema razloga da ga ne iskoristimo. Pogotovo jer je njegov algoritam stvaranja JAKO optimiziran.
 */
char *stvoriAbecedu(char S[], int lengthOfInput, int *duzinaAbecede, int SA[]) {
	char prethodniZnak, *abeceda = NULL;
	int i = 0;
	// Napravi abecedu preko sufiksnog polja
	abeceda = (char *) malloc(sizeof(char) * MAX_SIZE_ABECEDA);
	// Po dogvoru $ je leksikografski najmanji element u abecedi UVIJEK
	abeceda[0] = '$';
	*duzinaAbecede = 1;
	prethodniZnak = '$';
	// Predi cijeli ulaz i dodaj sva nova slova u abecedu
	for (i = 0; i < lengthOfInput; i++) {
		// Znak je vec u abecedi preskoci ili je $ koji smo odmah stavili
		if (S[SA[i]] == prethodniZnak || S[SA[i]] == '$') {
			continue;
		}
		// Znak treba staviti u abecedu, povecati njenu duzinu i azurirati varijablu prethodniZnak
		else {
			abeceda[*duzinaAbecede] = S[SA[i]];
			(*duzinaAbecede)++;
			prethodniZnak = S[SA[i]];
		}
	}
	// Radi i bez ovoga, makar od kada smo stavili stablo valica odluka je ovo vratiti
	abeceda[*duzinaAbecede] = '\0';

	return abeceda;
}

/**
 * Cita ulaz i sprema ga u S kojeg vraca preko adrese.
 * Odbacuje sve znakove novog reda za Windows i Linux.
 *
 * Jos nije makar trebalo bi ju napraviti da radi za fasta format.
 */
char *parsirajUlaz(int *lengthOfInput) {
	char *S;
	// Ulazna datoteka
	FILE *fp;

	// Otvori ulaznu datoteku
	fp = fopen("ulaz.txt", "r");
	if (fp == NULL) {
		printf("ERROR reading the file!(ovo nije kompajler)");
		exit(1);
	}

	char znak = fgetc(fp);
	S = (char *) malloc(sizeof(char));
	int i = 0;
	while (znak != EOF) {
		// Procitan je znak \n, preskoci na sljedeci znak, NEMOJ dodati \n u polje procitanih znakova
		if (znak == '\n' || znak == '\r' || znak == '\0') {
			znak = fgetc(fp);
			continue;
		}
		// Dodaj novi procitani znak u procitani niz
		S[i] = znak;
		i++;
		znak = fgetc(fp);
		S = (char *) realloc(S, sizeof(char) * (i + 1));
	}
	// Dodaj na kraju niza $
	*lengthOfInput = i + 1;
	S[*lengthOfInput - 1] = '$';

	// Obavezno zatvori file nakon sto si ga procitao
	fclose(fp);

	return S;
}

/**
 * Prima polje znakova S(input), i vraca integer tj. sumu
 *svih znakova leksigorafski manjih od znaka poslanog u funkciju
 *-- treba optimizaciju
 *---CAKA mozemo C izracunati preko sufiksnog polja, interval sufiksnog polja
 */
int Cfunk(char znak, const char input[], int lengthOfInput) {
	int i, sum = 0;
	// C('$') = 0 po definiciji!
	if (znak == '$')
		return 0;
	// trcimo preko cijelog inputa tu mozemo izracunati abeceda*/
	for (i = 0; i < lengthOfInput; i++) {
		if (input[i] < znak)
			sum++;
	}
	//printf("C('%c')=%d\n",znak,sum);
	return sum;
}

/**
 * Nadi za zadani znak odgovorajucu vrijednost C-a
 */
int Ccalc(char znak, char abeceda[], int duzinaAbecede, int C[]) {
	// Po definiciji $ je leksikografski najmanji znak
	if( znak == '$')
		return 0;
	int i = 0;
	// Pronadi vrijednost od C-a za odgovarajuci znak
	for( i = 1; i < duzinaAbecede; i++) {
		if( znak == abeceda[i] ) {
			//printf("C(%c) = %d\n",znak,C[i]);
			return C[i];
		}
	}
	// Netko je poslao znak koji nij u abecedi
	return 0;
}

/**
 * Izgradi prefix sum table(C tablicu) nad zadanim ulazom
 **/
int *createPrefixSumTable(const char input[], int lengthOfInput,
		int duzinaAbecede, char abeceda[]) {
	int *C = (int *) malloc(duzinaAbecede * sizeof(int));
	char znak;
	int i = 0, j = 0, sum = 0;
	// Po definiciji C($) = 0, jer je najmanji leksikografski znak
	C[0] = 0;
	//Izracunaj C za svaki znak abecede
	for (i = 1; i < duzinaAbecede; i++) {
		znak = abeceda[i];
		sum = 0;
		// Izracunaj za zadano slovo abecede, C
		for (j = 0; j < lengthOfInput; j++) {
			if (input[j] < znak)
				sum++;
		}
		// Dodaj izracunati znak u prefix sum tablica
		C[i] = sum;
	}
	return C;
}

// funkcija gradi BWT od SA i inputa, dakle gradimo BWT na prethodnicima */
void BWT(const char input[], int SA[], char B[], int lengthOfInput) {
	int i;
	for (i = 0; i < lengthOfInput; i++) {
		if (SA[i] != 0)
			B[i] = input[SA[i] - 1];
		else
			B[i] = '$';
	}
}

/**
 *Optimiziranija funkcija od funkcija Occ, cilj je napraviti tabblicu sa svim vrijednostima
 *Preuzeo sam algoritam sa: http://stackoverflow.com/questions/13023739/creating-an-2d-array-using-pointer-malloc-and-then-print-it-out
 */
void stvoriOcc(char BWT[], char abeceda[], int lengthOfInput, int duzinaAbecede) {
	// stvori jedan stupac sa svim slovima abecede, sada ga treba prosiriti s duzinom BWT-a (naknadno dodao - 1)
	OccTablica = (int **) malloc(sizeof(int *) * duzinaAbecede);
	int i, j, k, brojac;
	// stvori redovi u Occ tablici
	for (i = 0; i < duzinaAbecede; i++)
		//OccTablica[i] = (int *) malloc(sizeof(int) * strlen(BWT));
		// naknadno dodao length - 1
		OccTablica[i] = (int *) malloc(sizeof(int) * lengthOfInput);

	// napuni ju vrijednostima
	for (i = 0; i < duzinaAbecede; i++) {
		for (j = 0; j < lengthOfInput; j++) {
			brojac = 0;
			// tu moramo izracunati vrijednost Occa prije nego ga upisemo u tablicu
			for (k = 0; k <= j; k++) {
				if (BWT[k] == abeceda[i])
					brojac++;
			}
			OccTablica[i][j] = brojac;
		}
	}
	return;
}

/**
 *Funkcija koja racuna Occ(broj pojavljivanja) znaka u BWTu, broj je mjesto do kojeg se gleda u BWTu, treba optimizaciju.
 *trebamo probati s matricom, trebalo bi biti brze of funkcije, preko BWTa, obzvezno napraviti MATRICU
 */
//int Occ(char znak, int broj, char B[]){
//	int i, ponavljanje = 0;
//	for( i = 0; i <= broj; i++){
//		if( B[i] == znak )
//			ponavljanje++;
//	}
//	return ponavljanje;
//}
/**
 * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke
 * input - string koji se pretrazuje
 * B - BWT string
 * duzinaAbecede - velicina abecede
 * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
 * */
int FMindex(char podniz[], const char input[], char B[], char abeceda[],
		int lengthOfInput, int duzinaAbecede, int C[]) {
	int Lp, Rp, i, boolean = 0, br = 1, zastavica = 0, k = 0, falseNiz = 0,
			duzinaPodNiza = 0;
	char znakZaRp;

	// Vrati 0 cim skuzis da prvo slovo nije dio abecede
	for (i = 0; i < duzinaAbecede; i++) {
		if (abeceda[i] == podniz[strlen(podniz) - 1]) {
			falseNiz++;
			break;
		}
	}
	// Izadi jer podniz nije dio abecede
	if (falseNiz == 0)
		return 0;

	duzinaPodNiza = strlen(podniz);
	for (i = (duzinaPodNiza - 1); i >= 0; i--) {
		// posto ima posebna formula za zadnji znak to stavljamo u zaseban if
		if (i == duzinaPodNiza - 1) {

			Lp = Ccalc(podniz[duzinaPodNiza - 1], abeceda, duzinaAbecede, C);
			// nadi slovo koje se nalazi iza slova koji se koristi gore u Lp
			for (k = 0; k < duzinaAbecede; k++) {
				if (abeceda[k] == podniz[duzinaPodNiza - 1])
					boolean = 1;
				if (boolean == 1 && abeceda[k] != podniz[duzinaPodNiza - 1]) {
					znakZaRp = abeceda[k];
					// GRANICNI SLUCAJ U slucaju da nema sljedeceg znaka u abecedi, program nikada tu nece uci
					zastavica = 1;
					// getOut ne da da se opet ude u ovaj if
					break;
				}
			}
			// pomocu gore nadenog sljedeceg znaka racunamo Rp
			if (zastavica == 1)
				Rp = Ccalc(znakZaRp, abeceda, duzinaAbecede, C) - 1;
			//Ako nema sljedeceg znaka u abecedi, onda je Rp jednak duzini nizia-1
			else
				Rp = lengthOfInput - 1;
		} else {
			// uvjet algoritma
			if (Lp > Rp && i < 1)
				break;
			// nase slovo u Occu, u kojojem se mjestu nalazi u abecedi?
			for (k = 0; k < duzinaAbecede; k++) {
				if (abeceda[k] == podniz[duzinaPodNiza - br])
					break;
			}
			// k - mjesto u abecedi si se pojavljuje nase slovu koje promatramo unutar podniza
			//printf("Lp = C(%c) = %d + occ(%c,%d) = %d\n",
			//		podniz[duzinaPodNiza - br],
			//		C(podniz[duzinaPodNiza - br], input, lengthOfInput),
			//		abeceda[k], Lp - 1, OccTablica[k][Lp - 1]);
			Lp = Ccalc(podniz[duzinaPodNiza - br], abeceda, duzinaAbecede, C)
					+ OccTablica[k][Lp - 1];
			//printf("Rp = C(%c) = %d + occ(%c,%d) = %d + 1\n",
			//		podniz[duzinaPodNiza - br],
			//		C(podniz[duzinaPodNiza - br], input, lengthOfInput),
			//		abeceda[k], Lp - 1, OccTablica[k][Rp] - 1);
			Rp = Ccalc(podniz[duzinaPodNiza - br], abeceda, duzinaAbecede, C)
					+ OccTablica[k][Rp] - 1;
			//printf("\n");
		}
		// sluzi mi nesto kao i u for petlji samo posto petlja ide odozada ovo ce mi ici otpocetka
		br++;
	}
	if (Rp < Lp)
		return 0;
	else
		return Rp - Lp + 1;
}

///**
// * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke
// * input - string koji se pretrazuje
// * B - BWT string
// * duzinaAbecede - velicina abecede
// * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
// * */
//int FMindex(char podniz[], const char input[], char B[], char abeceda[],
//		int lengthOfInput, int duzinaAbecede) {
//	int i, c, falseNiz = 0, Lp, Rp, k = 0, boolean;
//	char csljed;
//	// Vrati 0 cim skuzis da prvo slovo nije dio abecede
//	for (i = 0; i < duzinaAbecede; i++) {
//		if (abeceda[i] == podniz[strlen(podniz) - 1]) {
//			falseNiz++;
//			break;
//		}
//	}
//	// Izadi jer podniz nije dio abecede
//	if (falseNiz == 0)
//		return 0;
//
//	i = strlen(podniz) - 1;
//	c = podniz[i - 1];
//	//printf("Lp = C('%c')\n",c);
//	Lp = C(c, input, lengthOfInput);
//
//	for (k = 0; k < duzinaAbecede; k++) {
//		if (abeceda[k] == podniz[i])
//			boolean = 1;
//		if (boolean == 1 && abeceda[k] != podniz[i]) {
//			csljed = abeceda[k];
//			// GRANICNI SLUCAJ U slucaju da nema sljedeceg znaka u abecedi, program nikada tu nece uci
//			// getOut ne da da se opet ude u ovaj if
//			break;
//		}
//	}
//	//printf("Rp = C('%c')\n",csljed);
//	Rp = C(csljed, input, lengthOfInput) - 1;
//	while( (Lp <= Rp) && (i >= 1)) {
//		i = i - 1;
//		c = podniz[i]; // sljedeci znak za obradu u podnizu
//		for (k = 0; k <= duzinaAbecede; k++) {
//				if (abeceda[k] == c)
//					break;
//		}
//		//printf("Lp = C('%c') + Occ[%d][%d]\n",c,);
//		Lp = C(c, input, lengthOfInput) + OccTablica[k][Lp-1];
//		//printf("Rp = C('%c')\n",csljed);
//		Rp = C(c, input, lengthOfInput) + OccTablica[k][Rp] - 1;
//
//	}
//	if( Rp < Lp)
//		return 0;
//	else
//		return Rp - Lp + 1;
//}

///**
// * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke
// * input - string koji se pretrazuje
// * B - BWT string
// * duzinaAbecede - velicina abecede
// * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
// * */
//int FMindexWaveletTree(char podniz[], const char input[], char B[],
//		char abeceda[], int lengthOfInput, int duzinaAbecede, node *root) {
//	int Lp, Rp, i, boolean = 0, br = 1, zastavica = 0, k = 0, falseNiz = 0, duzinaPodNiza;
//	char znakZaRp;
//
//
//	duzinaPodNiza = strlen(podniz);
//	// Vrati 0 cim skuzis da prvo slovo nije dio abecede
//	for (i = 0; i < duzinaAbecede; i++) {
//		if (abeceda[i] == podniz[duzinaPodNiza - 1]) {
//			falseNiz++;
//			break;
//		}
//	}
//	// Izadi jer podniz nije dio abecede
//	if (falseNiz == 0)
//		return 0;
//
//	for (i = duzinaPodNiza - 1; i >= 0; i--) {
//		// posto ima posebna formula za zadnji znak to stavljamo u zaseban if
//		if (i == duzinaPodNiza - 1) {
//
//			Lp = C(podniz[strlen(podniz) - 1], input, lengthOfInput);
//			// nadi slovo koje se nalazi iza slova koji se koristi gore u Lp
//			for (k = 0; k < duzinaAbecede; k++) {
//				if (abeceda[k] == podniz[duzinaPodNiza - 1])
//					boolean = 1;
//				if (boolean == 1 && abeceda[k] != podniz[duzinaPodNiza - 1]) {
//					znakZaRp = abeceda[k];
//					// GRANICNI SLUCAJ U slucaju da nema sljedeceg znaka u abecedi, program nikada tu nece uci
//					zastavica = 1;
//					// getOut ne da da se opet ude u ovaj if
//					break;
//				}
//			}
//			// pomocu gore nadenog sljedeceg znaka racunamo Rp
//			if (zastavica == 1)
//				Rp = C(znakZaRp, input, lengthOfInput) - 1;
//			//Ako nema sljedeceg znaka u abecedi, onda je Rp jednak duzini nizia-1
//			else
//				Rp = lengthOfInput - 1;
//		} else {
//			// uvjet algoritma
//			if (Lp > Rp && i < 1)
//				break;
//			// nase slovo u Occu, u kojojem se mjestu nalazi u abecedi?
//			for (k = 0; k <= duzinaAbecede; k++) {
//				if (abeceda[k] == podniz[duzinaPodNiza - br])
//					break;
//			}
//			// k - mjesto u abecedi si se pojavljuje nase slovu koje promatramo unutar podniza
////			Lp = C(podniz[strlen(podniz) - br], input, lengthOfInput) + OccTablica[k][Lp - 1];
////			Rp = C(podniz[strlen(podniz) - br], input, lengthOfInput) + OccTablica[k][Rp] - 1;
//
//			printf("Znak %c\nIndeks %d\n", abeceda[k], Lp - 1);
//			Lp = C(podniz[duzinaPodNiza - br], input, lengthOfInput)
//					+ rank(root, abeceda[k], Lp - 1, abeceda, duzinaAbecede);
//			printf("Znak %c\nIndeks %d\n", abeceda[k], Rp);
//			Rp = C(podniz[duzinaPodNiza - br], input, lengthOfInput)
//					+ rank(root, abeceda[k], Rp, abeceda, duzinaAbecede) - 1;
//
//		}
//		// sluzi mi nesto kao i u for petlji samo posto petlja ide odozada ovo ce mi ici otpocetka
//		br++;
//	}
//	if( Rp < Lp)
//		return 0;
//	else
//		return Rp - Lp + 1;
//}

/**
 * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke
 * input - string koji se pretrazuje
 * B - BWT string
 * duzinaAbecede - velicina abecede
 * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
 * */
int FMindexWaveletTree(char podniz[], const char input[], char B[],
		char abeceda[], int lengthOfInput, int duzinaAbecede, node *root, int C[]) {
	int Lp, Rp, i, boolean = 0, br = 1, zastavica = 0, k = 0, falseNiz = 0,
			duzinaPodNiza = 0;
	char znakZaRp;

// Vrati 0 cim skuzis da prvo slovo nije dio abecede
	for (i = 0; i < duzinaAbecede; i++) {
		if (abeceda[i] == podniz[strlen(podniz) - 1]) {
			falseNiz++;
			break;
		}
	}
// Izadi jer podniz nije dio abecede
	if (falseNiz == 0)
		return 0;

	duzinaPodNiza = strlen(podniz);
	for (i = (duzinaPodNiza - 1); i >= 0; i--) {
		// posto ima posebna formula za zadnji znak to stavljamo u zaseban if
		if (i == duzinaPodNiza - 1) {
			Lp = Ccalc(podniz[duzinaPodNiza - 1], abeceda, duzinaAbecede, C);
			// nadi slovo koje se nalazi iza slova koji se koristi gore u Lp
			for (k = 0; k < duzinaAbecede; k++) {
				if (abeceda[k] == podniz[duzinaPodNiza - 1])
					boolean = 1;
				if (boolean == 1 && abeceda[k] != podniz[duzinaPodNiza - 1]) {
					znakZaRp = abeceda[k];
					// GRANICNI SLUCAJ U slucaju da nema sljedeceg znaka u abecedi, program nikada tu nece uci
					zastavica = 1;
					// getOut ne da da se opet ude u ovaj if
					break;
				}
			}
			// pomocu gore nadenog sljedeceg znaka racunamo Rp
			if (zastavica == 1)
				Rp =  Ccalc(znakZaRp, abeceda, duzinaAbecede, C) - 1;
			//Ako nema sljedeceg znaka u abecedi, onda je Rp jednak duzini nizia-1
			else
				Rp = lengthOfInput - 1;
		} else {
			// uvjet algoritma
			if (Lp > Rp && i < 1)
				break;
			// nase slovo u Occu, u kojojem se mjestu nalazi u abecedi?
			for (k = 0; k < duzinaAbecede; k++) {
				if (abeceda[k] == podniz[duzinaPodNiza - br])
					break;
			}
			// k - mjesto u abecedi si se pojavljuje nase slovu koje promatramo unutar podniza
//			printf("Lp = C(%c) = %d + rank(%c,%d) = %d\n",
//					podniz[duzinaPodNiza - br],
//					C(podniz[duzinaPodNiza - br], input, lengthOfInput),
//					abeceda[k], Lp - 1,
//					rank(root, abeceda[k], Lp - 1, abeceda, duzinaAbecede));
			Lp =  Ccalc(podniz[duzinaPodNiza - br], abeceda, duzinaAbecede, C)
					+ rank(root, abeceda[k], Lp - 1, abeceda, duzinaAbecede);
//			printf("Rp = C(%c) = %d + rank(%c,%d) = %d  + 1\n",
//					podniz[duzinaPodNiza - br],
//					C(podniz[duzinaPodNiza - br], input, lengthOfInput),
//					abeceda[k], Lp - 1,
//					rank(root, abeceda[k], Rp, abeceda, duzinaAbecede));
			Rp =  Ccalc(podniz[duzinaPodNiza - br], abeceda, duzinaAbecede, C)
					+ rank(root, abeceda[k], Rp, abeceda, duzinaAbecede) - 1;
			//printf("\n");
		}
		// sluzi mi nesto kao i u for petlji samo posto petlja ide odozada ovo ce mi ici otpocetka
		br++;
	}
	if (Rp < Lp)
		return 0;
	else
		return Rp - Lp + 1;
}

/**
 * Glavna funkcija, vraca broj ponavljanja podniza u nizu koji je zadan preko datoteke
 * input - string koji se pretrazuje
 * B - BWT string
 * duzinaAbecede - velicina abecede
 * podniz - podniz koji provjeravamo, je li se nalazi unutar vece niza(inputa)
 * */
int FMindexWaveletTreeBit(char podniz[], const char input[], char B[],
		char abeceda[], int lengthOfInput, int duzinaAbecede, node *root, int C[]) {
	int Lp, Rp, i, boolean = 0, br = 1, zastavica = 0, k = 0, falseNiz = 0,
			duzinaPodNiza = 0;
	char znakZaRp;

// Vrati 0 cim skuzis da prvo slovo nije dio abecede
	for (i = 0; i < duzinaAbecede; i++) {
		if (abeceda[i] == podniz[strlen(podniz) - 1]) {
			falseNiz++;
			break;
		}
	}
// Izadi jer podniz nije dio abecede
	if (falseNiz == 0)
		return 0;

	duzinaPodNiza = strlen(podniz);
	for (i = (duzinaPodNiza - 1); i >= 0; i--) {
		// posto ima posebna formula za zadnji znak to stavljamo u zaseban if
		if (i == duzinaPodNiza - 1) {

			Lp =  Ccalc(podniz[duzinaPodNiza - 1], abeceda, duzinaAbecede, C);
			// nadi slovo koje se nalazi iza slova koji se koristi gore u Lp
			for (k = 0; k < duzinaAbecede; k++) {
				if (abeceda[k] == podniz[duzinaPodNiza - 1])
					boolean = 1;
				if (boolean == 1 && abeceda[k] != podniz[duzinaPodNiza - 1]) {
					znakZaRp = abeceda[k];
					// GRANICNI SLUCAJ U slucaju da nema sljedeceg znaka u abecedi, program nikada tu nece uci
					zastavica = 1;
					// getOut ne da da se opet ude u ovaj if
					break;
				}
			}
			// pomocu gore nadenog sljedeceg znaka racunamo Rp
			if (zastavica == 1)
				Rp =  Ccalc(znakZaRp, abeceda, duzinaAbecede, C) - 1;
			//Ako nema sljedeceg znaka u abecedi, onda je Rp jednak duzini nizia-1
			else
				Rp = lengthOfInput - 1;
		} else {
			// uvjet algoritma
			if (Lp > Rp && i < 1)
				break;
			// nase slovo u Occu, u kojojem se mjestu nalazi u abecedi?
			for (k = 0; k < duzinaAbecede; k++) {
				if (abeceda[k] == podniz[duzinaPodNiza - br])
					break;
			}
			// k - mjesto u abecedi si se pojavljuje nase slovu koje promatramo unutar podniza
			//printf("Znak %c\nIndeks %d\n", abeceda[k], Lp - 1);
			Lp =  Ccalc(podniz[duzinaPodNiza - br], abeceda, duzinaAbecede, C)
					+ rankBit(root, abeceda[k], Lp - 1, abeceda, duzinaAbecede,
							lengthOfInput);
			//OccTablica[k][Lp - 1];
			//printf("Znak %c\nIndeks %d\n", abeceda[k], Rp);
			Rp =  Ccalc(podniz[duzinaPodNiza - br], abeceda, duzinaAbecede, C)
					+ rankBit(root, abeceda[k], Rp, abeceda, duzinaAbecede,
							lengthOfInput) - 1;
			//OccTablica[k][Rp] - 1;
		}
		// sluzi mi nesto kao i u for petlji samo posto petlja ide odozada ovo ce mi ici otpocetka
		br++;
	}
	if (Rp < Lp)
		return 0;
	else
		return Rp - Lp + 1;
}
