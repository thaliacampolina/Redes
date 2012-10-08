/////////////////////
//     main        //
////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "crc.h"


int main (int argc, char* argv[]) {
    
    FILE *input;
    int polinomio, opt;
    
    if (argc<3){
        puts("ARGUMENT MISSING");
        return 0;
    } else {
        input = fopen(argv[1], "rb");
        if (input == NULL){
            puts("THE FILE COULD NOT BE OPENED");
            return 0;
        }
    }
    opt = atoi(argv[2]);
//DEBUG
printf("filename = %s \n", argv[1]);
printf("opcao = %d\n",opt);
    if (opt==1){
        polinomio = 1;
    } else if (opt ==0){
        polinomio = 0;
    } else {
        puts("WRONG ARGUMENTS");
    }

    ReadFile(input, polinomio);

    fclose (input);
    return 0;
}


