//////////////////////
//      crc         //
//////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "crc.h"

void ReadFile(FILE* input, int polinomio){
    char first;
    char second;
    char str[2];
    int f, s, sixteen;
    int mod;
printf("conteudo arquivo:  ");
    while ((fscanf(input, "%c", &first) > 0)){
        //if 16-bit polynomial, two strings are concatenated
printf(" first =  %c %d",first, first);
        if (polinomio == 1) {
            fscanf(input, "%c", &second);
            str[0]=first;
	    str[1]=second;
printf("second =  %c  dec %d   ",second, second);
printf("string = %c%c ",str[0],str[1]);
        }
        f = (int) first;
        s = (int) second;
        sixteen = first <<8;
        sixteen = first|second;        
printf("%i   ",f);
	if (polinomio == 0) mod = Mod(f,polinomio);
	else mod = Mod(sixteen, polinomio);
printf("mod dentro do read = %i \n", mod);
   //     Hexadecimal(mod);
   }
    //free(e);
printf("\n ta fechando o arquivo! \n");
    printf("\n");

}

int Mod8(int i){
    //8-bit polynomial = x⁸ + x² + x + 1
    //binary value: 100000111
    //decimal: 263
    int mod = i%263;
    return mod;
printf("mod de 8 bits = %d \n", mod);
}

int Mod16(int i){
    //16-bit polynomial =x¹⁶+X¹⁵+x²+1 
    //binary value: 11000000000000101
    //decimal: 98309
    int mod = i%98309;
    return mod;
printf("mod de 16 bits = %d \n", mod);
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
