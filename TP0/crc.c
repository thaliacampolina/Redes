//////////////////////
//      crc         //
//////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "crc.h"

void ReadFile(FILE* input, int polinomio){
    char c;
    char d;
    int i;
    int mod;
    while ((fscanf(input, "%c", &c) > 0)){
        if (polinomio == 1) {
            fscanf(input, "%c", &d);
            strcat(c,d);
        }
        i = (int) c;
        mod = Mod(i,polinomio);
   //     Hexadecimal(mod);
   }
    fclose (input);
    printf("\n");

}

int Mod8(int i){
    //8-bit polynomial = x⁸ + x² + x + 1
    //binary value: 100000111
    //decimal: 263
    int mod = i%263;
    return mod;
}

int Mod16(int i){
    //16-bit polynomial =x¹⁶+X¹⁵+x²+1 
    //binary value: 11000000000000101
    //decimal: 98309
    int mod = i%98309;
    return mod;
}

int Mod(int i, int polinomio){
    int mod;
    if (polinomio == 0) {
        mod = Mod8(i);
    } else {
        mod = Mod16(i);
    }
    return mod;
}

//void Hexadecimal(int mod){
//}
