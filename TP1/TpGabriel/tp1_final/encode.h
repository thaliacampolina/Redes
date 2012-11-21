#ifndef ENCODE_H
#define ENCODE_H
#include <stdio.h>
#include <math.h>
#include <string.h>

// Funcao principal de codificar. Nela estao localizadas todas as outras funcoes
// de codificacao.
void Encode(unsigned char entrada[], FILE *f);

// Funcao que converte o texto de entrada em um numero binario.
void Textobinario(unsigned char entrada[], int binario[], int tamanho);

// Funcao que converte o binario de entrada em decimal, juntando de 6 em 6 bits
// (ja para a base64).
void BinariotoDecimal(int binario[], int convertido64[], int tamanho);

// Funcao que define quantos bytes sobrarão na codificação para o preenchimento 
// com os "=".
void Setflag(int binario[], int tamanho);

// Funcao para transforma a cadeia de inteiros convertidos para a palavra 
// codificada.
void Converte(int convertido64[], FILE *f);

// Funcao que inicializa a string com zeros.
void Inicializa(unsigned char entrada[]);

// Funcao que inicializa o vetor de binarios com zero
void InicializaBinario(int entrada[]);


#endif // ENCODE_H
