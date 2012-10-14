#ifndef _crc_h
#define _crc_h


void ReadFile(FILE* input, int polinomio);
//calculates mod for 8-bit polynomial
int shift8(unsigned int i);
//shifts 8 bits to the left
long int shift16(unsigned int i);
//shitfs 16 bits to the left
void printHex(unsigned int mod);
//prints the remainder of the polynomial division in hexadecimal format
void Mod8(unsigned int i);
//calculates mod for 16-bit polynomial
void Mod16(unsigned int i);
//calculates mod
void Mod(unsigned int i, int polinomio);


#endif
