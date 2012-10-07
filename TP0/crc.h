#ifndef _crc_h
#define _crc_h


void ReadFile(FILE* input, int polinomio);
//calculates mod for 8-bit polynomial
int Mod8(int i);
//calculates mod for 16-bit polynomial
int Mod16(int i);
//calculates mod
int Mod(int i, int polinomio);
//prints a hexadecimal value on screen
void Hexadecimal(int mod);



#endif
