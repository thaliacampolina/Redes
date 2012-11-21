#include <stdio.h>
#include "encode.h"

int main(int argc, char *argv[]){
	// Le o binario arquivo do primeiro argumento. 
	FILE *read = fopen(argv[1], "rb");
	// Abre o arquivo para escrita	
	FILE *write = fopen(argv[2], "wb");
 	unsigned char c, str[4], saida[4];
	// Dentro de while, se o fscanf achar EOF, muda o sinal de controle, saindo
	// do laço.
	int controle = 1;
	// Inicializa a string para garantir que nao aparecerao lixos. Pega caractere
	// por caractere e joga na string, formando grupos de no máximo 3 caracteres. 
	// A quarta posicao da string nos mostrara quantos caracteres foram pegos no 
	// grupo. Caso o EOF apareca antes do terceiro, saberemos qual flag colocar.
	while(controle) {
		Inicializa(str);
		for (int i = 0 ; i < 3 ; i++){
			if (fscanf(read, "%c", &c) == -1){
				controle = 0;				
				break;
			}
			str[i] = c;
			str[3] = (char)((int)str[3] + 1);
		}
		Encode(str,write);
	}
	fclose(write);
	fclose(read);
	return 0;
}
