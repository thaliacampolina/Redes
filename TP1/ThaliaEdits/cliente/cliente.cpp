/*
=========================================================================
							TP1 Redes
=========================================================================

Autores:	Augusto Oliveira Paes
		Thalia Magalh�es de Almeida Campolina
		 				


Observa��o:
>>>>>>>>>>>	Importante: A porta n�o est� configurada para 25 e sim 9999, para testes
>>>>>>>>>>>>>>>>>>>>>>>	Exce��es:	<<<<<<<<<<<<<<<<<<<<<<<<<<
1. Anexar arquivo e ele n�o est� no mesmo diret�rio do cliente;
		Resultado: envia o e-mail mas na anvia o arquivo. N�o ocorre nenhum aviso de erro.
*/

//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <limits.h>
// Include BASE64
#include "encode.h"
//#include <fstream>


#define PORTA 9999		//Porta SMTP: 25

using namespace std;

int main(int argc, char* argv[])
{
/////////////////////////////////////////////////////////////
//Pega dados usu�rio:

    	char[16] host;			//ip servidor
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
//---
	//Pegar data e colocar no formato e-mail
	string aux_str; 
	string date;	
	char buffer [3];
	time_t rawtime;
  	struct tm * timeinfo;
 	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
	strftime (buffer,10," %Z",timeinfo);  		//FUSO Hor�rio do sistema
	aux_str = asctime(timeinfo);	
	// Armazena data formato mensagem	
	date = aux_str.substr(0,3) +", "+ aux_str.substr(8,2) +" "+ aux_str.substr(4,3) +" "+ aux_str.substr(20,4) +" "+ aux_str.substr(11,8) +" "+ buffer;

	// Pega IP local ou dominio para completar mensagem HELO
	//>>>>>>>>>>>> FAZER  <<<<<<<<<<<<<<<<<<

	//Acrescentando SUBJECT e DATE na mensagem
	mensagem = "Date: " + date + "\n" + "Subject: " + assunto + "\n" + "\n" + mensagem;


///////////////////////////////////////////////////////////////////
// Codifica arquivo Base64 --> Fun��o "codifica" e monta mesagem 

	if (argc == 5)
		mensagem = mensagem + "\n\n" + "--" + nome_arquivo + "\n" + "Content-Disposition: attachment; filename=\"" + nome_arquivo + "\"\n" +  "Content-Transfer-Encoding: base64\n\n" + codifica(nome_arquivo) + "\n--" + nome_arquivo + "--" + "\n.\r\n\0";
	else mensagem = mensagem + "\r\n.\r\n\0";

///////////////////////////////////////////////////////////////////
// Montando strings mensagens

	//HELO
	string helo;
	helo = "HELO localhost\r\n\0";   //<<<<<<<<<<<<<<<< Modificar
	
	//MAIL FROM
	mail_de = "MAIL FROM:" + mail_de + "\r\n\0";

	//RCPT TO
	mail_para = "RCPT TO:" + mail_para+ "\r\n\0";

	//DATA
	string data;
	data = "DATA\r\n\0";

	//QUIT
	string quit;
	quit = "QUIT\r\n\0";

//////////////////////////////////////////////////////////////////
// Variaveis SOCKET

    int sock; //inteiro que recebe o socket

	string envia, recebe; //strings de envio e recebimento de msgs

	char char_recebe[200]; //usado como parametro no recebimento

    const char *char_envia; //usado como parametro para envio

    struct sockaddr_in server; //estrutura de comunica��o

	/*A struct usada para conex�es na internet � a sockaddr_in, e possui a seguinte declara��o:

	struct sockaddr_in {
		    short int sin_family;          //Familia do endereco:
		                                   // AF_INET      (ARPA INTERNET PROTOCOLS) - "A mais usada"
		                                   // AF_UNIX      (UNIX INTERNET PROTOCOLS)
		                                   // AF_ISO       (ISO PROTOCOLS)
		                                   // AF_NS        (XEROX NETWORK SYSTEM PROTOCOLS)
		    unsigned short int sin_port;  // Numero da porta
		    struct in_addr sin_addr;      // IP do Host
		    unsigned char sin_zero[8];    // Zera a estrutura, algum espaco como struct sockaddr */

///////////////////////////////////////////////////////////////////
// Inicia conex�o servidor

	sock = socket(AF_INET, SOCK_STREAM, 0);

	/* sock = socket(sin_family, tipo_do_socket_desejado, Numero_protocolo);

	tipo_do_socket_desejado: no nosso caso SOCK_STREAM ou SOCK_DGRAM (para UDP);
	Numero_protocolo: � o n�mero correspondente do protocolo q se vai trabalhar, ex:
	  0 - IP - INTERNET PROTOCOL
	  1 - ICMP - INTERNET CONTROL MESSAGE PROTOCOL
	  2 - IGMP - INTERNET GROUP MULTICAST PROTOCOL
	  3 - GGP - GATEWAY-GATEWAY PROTOCOL
	  6 - TCP - TRANSMISSION CONTROL PROTOCOL
	  17 - UDP - USER DATAGRAMA PROTOCOL */

	if (sock==-1) //ou <0
	{
	 perror("opening stream socket"); //erro ao abrir o socket
	 exit(1);
	}

	bzero(&server, sizeof(server)); //zera o resto da estrutura
	server.sin_family = AF_INET; //declara��o da familia arpa net

	server.sin_addr.s_addr = inet_addr(host.c_str());//ou inet_aton("127.0.0.1", &dest.sin_addr); q eh o endere�o destino
	server.sin_port=htons(PORTA); //25 eh a SMTP PORT //

	/* Aqui o esquema � o seguinte: htons significa "host to network short", como � short
	trabalha com 2 bytes,mas ha ainda outros tipos como: htonl("host to network long",4 bytes),
	nt�s("network to host short",2 bytes), ntol("network to host long", 4 bytes) */

	if (connect(sock, (struct sockaddr *) &server, sizeof (server))==-1) //ou <0
	{
	 perror("connecting stream socket"); //erro ao conectar no socket
	 exit(1);
	}

//////////////////////////////////////////////////////////////////
//TROCA DE MENSAGENS

	int passo = 1;
	int finaliza = 0;
	
	//cout << endl << "Enviando E-MAIL..." << endl;
	printf(" E-mail esta sendo enviado \n " );

	do {//Trocas de msgs cliente-servidor

		switch (passo){
			case 1: 
				//HELO
				envia = helo;
				break;
			case 2: 
				//MAIL FROM
				envia = mail_de;
				break;
			case 3: 
				//RCPT TO:
				envia = mail_para;
				break;
			case 4: 
				//DATA
				envia = data;
				break;
			case 5: 
				//Mensagem
				envia = mensagem;
				break;
			case 6:
				//QUIT
				envia = quit;
				break;
			default:
				break;
		}

	//Recebendo resposta do servidor;
	read(sock, char_recebe, 100); //recebe a msg do servidor em char
	recebe = string(char_recebe);

   // cout << "MENSAGEM RECEBIDA DO SERVIDOR: " << recebe[0] << recebe[1] << recebe[1] << endl;
    printf("\n MENSAGEM RECEBIDA DO SERVIDOR: %s %s %s \n", recebe[0], recebe[1],recebe[1]);

	// Testa resposta		
	if (char_recebe[0] == '2' || char_recebe[0] == '3')		//Servidor pronto?
		{
		// Envia proxima mensagem
		char_envia = envia.c_str(); //converte a string para char
		//write (sock, char_envia, envia.length()-1); //envia a msg digitada em char
		send(sock,char_envia,envia.length(),0);
		if (passo == 7) // QUIT
			{
			//cout << "E-MAIL enviado com sucesso!" << endl;
			printf("\n E-MAIL enviado! \n");
			finaliza = 1;	//sai do while
			}
		}
	else
		{
		//Erro:
		//cout << "Ocorreu um erro na comunicacao com o servidor, tente novamente!" << endl;
		printf("\n OOps! Algum Erro Aconteceu!\n");
		finaliza = 1;		//sai do while
		}	
		
	passo++;
	}while (finaliza != 1);

//////////////////////////////////////////////////////////////////
// FECHA CONEXAO
	close (sock);

//////////////////////////////////////////////////////////////////

	return(0);
}


