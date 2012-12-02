/*IMPLEMENTACAO DO CLIENTE*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <limits.h>
// Include BASE64

int main(int argc, char* argv[]) {
/*
/////////////////////////////////////////////////////////////
//Pega dados usuário:

    	char host[16] ;			//ip servidor
	char* username = "";		//username
	char* password = "";
	
	if (argc <> 4)
		{
		printf("ERRO: Argumentos errados\nReinicie com os argumentos corretos: <username> <password> <IP do servidor>");
		return 0;
		}

	// armazena argumentos
	username = argv[1];
	password = argv[2];
	host = argv[3];
*/
//////////////////////////////////////////////////////////////////
// Variaveis SOCKET

    int sock; //inteiro que recebe o socket

	//string envia, recebe; //strings de envio e recebimento de msgs

	char char_recebe[200]; //usado como parametro no recebimento

    const char *char_envia; //usado como parametro para envio

    struct sockaddr_in server; //estrutura de comunicação


///////////////////////////////////////////////////////////////////
// Inicia conexão servidor

	sock = socket(PF_INET, SOCK_DGRAM, 0);

	read(sock, char_recebe, 100); //recebe a msg do servidor em char
    
    printf("MENSAGEM RECEBIDA DO SERVIDOR: %s", char_recebe);





//////////////////////////////////////////////////////////////////
// FECHA CONEXAO
	close (sock);
        return 0;
}
