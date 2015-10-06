/*
 * defs.h
 *
 *  Created on: Nov 3, 2014
 *      Author: kodly
 */

#ifndef DEFS_H_
#define DEFS_H_


// Max char velicina
#define CHAR_SIZE 255

// Maksimalna duljina podniza
#define PATTER_SIZE 500000

// Najveca velicina abecede
#define MAX_SIZE_ABECEDA 255

// Broj uzoraka kod proracuna statistike
#define NUMBER_OF_SAMPLES 100

// Duzina testnih nizova
#define STRING_TESTING_LENGTH 1000

// Dohvati bit na zadanoj poziciji
#define getBit(position) (bitovi[(position)/8]&maska[(position)%8] ? 1 : 0)

// Postavi bit na zadanoj poziciji
#define setBit(position, bit) ((bit)== 1 ? bitovi[(position)/8]|maska[(position)%8] : bitovi[(position)/8]&(~(maska[(position)%8])))

// Dobi rank odgovarajuceg bita
#define getRankBit(position) (bitovi[0]&maska[(position)%8] ? 1 : 0)


#endif /* DEFS_H_ */
