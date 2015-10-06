/*
 * types.h
 *
 *  Created on: Nov 4, 2014
 *      Author: kodly
 */

#ifndef TYPES_H_
#define TYPES_H_

// Occurrence tablica
int **OccTablica;

// Struktura za abecedu -> OCC tablica (sa svim mogucim kombinacijama)
typedef struct {
	char znak;
	int oznaka;
} occurrence;

// Cvor u stablu
struct element {
	char *val;
	struct element *right, *left;
};

// Preimenuj cvor radi lakseg koristenja
typedef struct element node;




#endif /* TYPES_H_ */
