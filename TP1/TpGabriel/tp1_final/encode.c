#include <stdio.h>
#include "encode.h"

// Numero de bytes do vetor string.
#define MAX 4

// Variavel global que conta o numero de caracteres impressos para imprimir
// os \n a cada 64 caracteres.
int formatacao = 0;

// Funcao principal que chama todas as outras funcoes.
void Encode(unsigned char entrada[], FILE *f){
	// O tamanho sera indicado pela ultima posicao do vetor de entrada.
	int tamanho = ((int)entrada[MAX-1]);
	int binario[8*MAX], convertido64[MAX];
	InicializaBinario(binario);
	Textobinario(entrada, binario, tamanho);
	BinariotoDecimal(binario, convertido64, tamanho);
	Converte(convertido64,f);
}

// Dependendo do tamanho da string, podemos obter quantos bytes irão sobrar no 
// final da codificacao. A logica funciona da seguinte maneira: O tamanho, e o 
// comprimento total da entrada (numero de bits). O resto da divisao deste
// tamanho por 6, ira indicar um valor. Este valor mostra se está faltando um, 
// dois, ou nenhum byte para codificação. Colocamos uma flag no final do vetor 
// para fazer esta indicação.
void Setflag(int binario[], int tamanho){
	if (tamanho == 1){
		// Flag para dois bytes faltando.
		binario[8*tamanho + 4] = -1;
	} else if (tamanho == 2){
		// Flag para um byte faltando.
		binario[8*tamanho + 2] = -2;
	} else {
		// Flag para zero bytes faltando.
		binario[8*tamanho] = -3;
	}
}

// Funcao que procura na tabela de conversão o valor decimal para o valor bin64.
// Se o valor decimal for um valor negativo (flag), atribui-se os devidos "=".
void Converte(int convertido64[], FILE *f){
	char tableconvert[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
	'N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
	'g','h','i','j','k','l','m', 'n','o','p','q','r','s','t','u','v','w','x','y',
	'z','0','1','2','3','4','5','6','7','8','9','+','/'};
	for (int i = 0 ; ;i++){
		// Caso chegue no limite das colunas, imprime os caracteres de terminação de
		// linha CR+LF.
		if (formatacao % 64 == 0 && formatacao != 0 && i != 4){
			fprintf(f,"\r\n");
		}
		// Se o valor no vetor de convertidos é válido, imprime o caractere codifi
		// cado.
		if (convertido64[i] >= 0){
			fprintf(f,"%c",tableconvert[convertido64[i]]);
		// Se o valor no vetor de convertidos é negativo, imprime os "=" para cada
		// situação, pré-informadas pela flag no final dos números válidos.
		} else if (convertido64[i] == -2){
			fprintf(f,"=");
			return;
		} else if (convertido64[i] == -1){
			fprintf(f,"==");
			return;
		} else {
			return;
		}
		// Soma para cada caractere imprimido.
		formatacao++;
	}
}

void Textobinario(unsigned char entrada[], int binario[], int tamanho){
	int decimal, i = 0;
	// Este for percorre a frase a ser codificada.
	for (int j = 0 ; i < tamanho ; i++, j += 8){
		decimal = (int)entrada[i];
		// Preenche o vetor binario de 8 em 8, partindo do bit mais significativo 
		// (j+7) para o menos significativo (j). Preenche com 0 os bits restantes.
		for (int k = j + 7 ; k >= j ; k--){
			binario[k] = decimal % 2;
			decimal = decimal / 2;
		}		
	}
	// Coloca a flag, dependendo do tamanho.
	Setflag(binario, tamanho);
}

void BinariotoDecimal(int binario[], int convertido64[], int tamanho){
	for (int i = 0, j = 0 ;  ; i++, j += 6){
		int decimal = 0;
		// Percorre o vetor de 6 em 6 (bits), transformando em decimal a partir dos
		// valores obtidos pela potencia de 2. (j+5)-k oferecerá o valor da potencia 
		// para o determinado bit. Ex.: bit menos significativo (j+5)-k = 0.
		for (int k = j ; k < j + 6 ; k++){
			// Se for um numero menor que zero, quer dizer que chegou na nossa flag 
			// (que define o final da conversao).
			if (binario[k] >= 0){
				decimal += binario[k]*pow(2, (j + 5) - k);
			} else {
				convertido64[i] = binario[k];
				return;
			}
		}
		convertido64[i] = decimal;
	}
}

// Garantimos que a string tera os valores zerados, para que nao apareca lixo.
void Inicializa (unsigned char entrada[]){
	unsigned char zero = (char)0;	
	for (int i = 0 ; i < 4 ; i++){
		entrada[i] = (int)zero;
	}
}

// Garantimos que o vetor tera os valores zerados, para que nao apareca lixo.
void InicializaBinario(int entrada[]){
	for (int i = 0 ; i < 8*MAX ; i++){
		entrada[i] = 0;
	}
}

