#ifndef _crc_h
#define _crc_h


void ReadFile(FILE* input, int polinomio);
//calculates mod for 8-bit polynomial
int shift8(int i);
//shifts 8 bits to the left
long int shift16(int i);
//shitfs 16 bits to the left
void printHex(int mod);
//prints the remainder of the polynomial division in hexadecimal format
void Mod8(int i);
//calculates mod for 16-bit polynomial
void Mod16(int i);
//calculates mod
void Mod(int i, int polinomio);
//prints a hexadecimal value on screen*********************EDIT
void Hexadecimal(int mod);



#endif
