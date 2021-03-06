//////////////////////
//      crc         //
//////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "crc.h"

void ReadFile(FILE* input, int polinomio){
    char first, second, str[2];
    unsigned int f, s, sixteen;
    printf("0x");
    while ((fscanf(input, "%c", &first) > 0)){
	f = (unsigned int) first;
        if (polinomio == 1) {
	//if 16-bit polynomial, two strings are concatenated
            fscanf(input, "%c", &second);
            str[0]=first;
	    str[1]=second;
	    s = (unsigned int) second;
	    sixteen = f << 8;
	    sixteen = sixteen|s;
        }
	if (polinomio == 0) Mod(f,polinomio);
	else Mod(sixteen, polinomio);
   }
   printf("\n\n");
}

int shift8(unsigned int i){
	i = i<<8;
	return i;
	}
	
long int shift16(unsigned int i){
	long int a = i<<16;
	return a;
	}
	
void printHex(unsigned int mod){
    char s[256];
    int k=0;
    sprintf(s, "%x", mod);
    while (s[k]){
    	putchar(toupper(s[k]));
    	k++;
    }
}

void Mod8(unsigned int i){
    //8-bit polynomial = x⁸ + x² + x + 1
    //binary value: 100000111
    //decimal: 263
    i = shift8(i);
    unsigned int mod = i%263;
    printHex(mod);
}

void Mod16(unsigned int i){
    //16-bit polynomial =x¹⁶+X¹⁵+x²+1 
    //binary value: 11000000000000101
    //decimal: 98309
    long int a = shift16(i);
    unsigned int mod = a%98309;
    printHex(mod);
}

void Mod(unsigned int i, int polinomio){
    if (polinomio == 0) Mod8(i);
    else Mod16(i);
}
