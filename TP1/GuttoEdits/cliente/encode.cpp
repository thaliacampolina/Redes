#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "encode.h"
#include <iostream>
#include <fstream>

using namespace std;

const char tab_B64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string codifica(string nome)
{
    string saida;
    int i,j,k=0;
    unsigned char byte[3]; //armazena os bytes
    int index[4]; //recebe o index de 6 bits
	int length; //tamanho do arquivo
	int opcao; //quantos caracteres ficaram no ultimo trio
  
	ifstream anexo(nome.c_str(), ios::in | ios::binary); //abre arquivo "anexo" no modo binario
	
	anexo.seekg (0, ios::end); //bota o ponteiro no fim do arquivo
	length = anexo.tellg(); //fala o tamanho do inicio até onde o ponteiro está
	length=length-1; //remove o \n que tem no final, pois nao vamos converter isso
	anexo.seekg (0, ios::beg); //volta o ponteiro pro inicio do arquivo
	opcao=(length)%3; //quantos caracteres ficaram no ultimo trio

//roda o tamanho do arquivo, tirando os finais (se nao for 3 bytes) e dividindo por 3, ja q eu faço de 3 em 3 no for
	for (i=0; i<((length-opcao)/3); i++)
	{
		byte[0]=anexo.get();//le o primeiro byte
		byte[1]=anexo.get();//le o segundo byte
		byte[2]=anexo.get();//le o terceiro byte
			
		index[0] = byte[0]>>2; //shifta o primeiro byte dois a direita
        index[1] = (((byte[1]>>4) | (byte[0]<<4)) & 63); //byte 2 shift 4 direita, byte 1 shift 4 esquerda, and 00111111
        index[2] = (((byte[1]<<2) | (byte[2]>>6)) & 63);
        index[3] = byte[2] & 63;
        
		//condição para tirar os tres primeiros bytes de arquivos word
		if ((index[0] != 59) && (index[1] != 59) && (index[2] != 46) && (index[3] != 63))
		{
			for(j=0;j<4;j++)
	        saida+=tab_B64[index[j]]; //pega o correspondente e salva na string
			k++;
		}	
        if ((k%19) == 0) //Condição de quebra de página, 76 caracteres maximo por linha
        saida+="\r\n";
	}

	//se faltar um caracter no final
	if (opcao==2)
	{
		byte[0]=anexo.get();//le o primeiro byte
		byte[1]=anexo.get();//le o segundo byte

		index[0] = byte[0]>>2;
        index[1] = (((byte[1]>>4) | (byte[0]<<4)) & 63);
        index[2] = ((byte[1]<<2) & 63);
        
		for(j=0;j<3;j++)
        saida+=tab_B64[index[j]]; //atualiza a string com a conversão
		
		saida+="="; //mostra que terminava com 2 bytes

	}
	
	//se faltar dois caracteres no final
	if (opcao==1)
	{
		byte[0]=anexo.get();//le o primeiro byte
		index[0] = byte[0]>>2;
        index[1] = (byte[0]&3)<<4;
        
		for(j=0;j<2;j++)
        saida+=tab_B64[index[j]]; //atualiza a string com a conversão
		
        saida+="=="; //mostra que terminava com 1 byte
        
	} 

    anexo.close(); //fecha o arquivo
    return saida;
}


