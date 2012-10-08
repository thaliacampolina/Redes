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
    //char* e= (char*)malloc(sizeof (char));
    int i;
    float mod;
printf("conteudo arquivo:  ");
    while ((fscanf(input, "%c", &c) > 0)){
        //if 16-bit polynomial, two strings are concatenated
        if (polinomio == 1) {
            fscanf(input, "%c", &d);
       //     strcat(c,d);
        }
printf("%c = ",c);
        i = (int) c;
printf("%i   ",i);
        mod = Mod(i,polinomio);
printf("mod dentro do read = %f \n", mod);
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
    float mod = i%263;
    return mod;
printf("mod de 8 bits = %f \n", mod);
}

int Mod16(int i){
    //16-bit polynomial =x¹⁶+X¹⁵+x²+1 
    //binary value: 11000000000000101
    //decimal: 98309
    float mod = i%98309;
    return mod;
printf("mod de 16 bits = %f \n", mod);
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
