/*
=========================================================================
							TP1 Redes
=========================================================================

Autores:	Marcelo Augusto Andrade Penna (prof Luiz Felipe)		marceloaapenna@gmail.com
			Alysson Alves de Oliveira (prof. Daniel)				alysson_alves@yahoo.com.br
		 				


Observação:
>>>>>>>>>>>	Importante: A porta não está configurada para 25 e sim 9999, para testes
1. Os comandos neste servidor podem ser repetidos e os dados serão sobrescritos;
2. A ordem dos comandos não é levado em conta;
*/

#include <iostream>
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

using namespace std;

//Manipulacao de Arquivo
#include <cstdlib>
#include <fstream>
using std::ofstream;
using std::ios;
using std::cerr;
using std::cout;
using std::endl;
using std::exit;

//Tamanho Max da mensagem recebida do cliente (5MBytes)
#define TAMANHO_BUFFER_RECEBE 5000000
//Porta que os servidor está escutando
#define PORTA 9999
//Tamanho max e-mail
#define max_email 200

bool	continua,
		recebeu_mail,
		recebeu_rcpt,
		recebeu_msg,
		recebeu_data,
		recebeu_helo,
		recebeu_rset,
		recebeu_noop,
		recebeu_quit,
		loop2;

int main(int argc, char* argv[])
{
	//Tabela consulta comando
		//Armazena comando HELO
		char helo[6] = "helo ";
		char H_E_L_O[6] = "HELO ";
		//Armazena comando MAIL FROM
		char mail[11] = "mail from:";
		char M_A_I_L[11] = "MAIL FROM:";
		//Armazena comando RCPT
		char rcpt[9] = "rcpt to:";
		char R_C_P_T[9] = "RCPT TO:";
		//Fim de comando
		//char fim[3] = "\r\n";
		char at = '@';
		//Armazena comando DATA
		char D_A_T_A[7] = "DATA\r\n";
		char data[7] = "data\r\n";
		//Armazena comando RSET
		char R_S_E_T[7] = "RSTE\r\n";
		char rset[7] = "rste\r\n";
		//Armazena comando NOOP
		char N_O_O_P[7] = "NOOP\r\n";
		char noop[7] = "noop\r\n";
		//Armazena comando QUIT
		char Q_U_I_T[7] = "QUIT\r\n";
		char quit[7] = "quit\r\n";


    int sock, cliente_sock;
    struct sockaddr_in sock_param, cliente;
    socklen_t addrlen= sizeof(cliente);

	string recebe, envia; //strings dos textos
	const char *char_envia; //usado como parametro no write
	char char_recebe[TAMANHO_BUFFER_RECEBE];
	//char *char_recebe; //usado como parametro no read

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock==-1) //ou <0
		{
	 	perror("opening stream socket"); //erro ao abrir o socket
	 	exit(1);
		}

	bzero(&sock_param, sizeof(sock_param)); //zera o resto da estrutura
	sock_param.sin_family = AF_INET; //declaração da familia arpa net
	sock_param.sin_port=htons(PORTA); /*25 eh a SMTP PORT */
	sock_param.sin_addr.s_addr = INADDR_ANY; //aceita conexão de qq ip, eh setado em 0.0.0.0

	bind (sock, (struct sockaddr*)&sock_param, sizeof(sock_param)); //vincula o socket a um endereço ip
	

	// Escutando Porta
	listen(sock, 5); //"escuta" o socket esperando conexao, numero de conexções q ele guenta ate a certa aparecer (5 maximo)

	
	while (1) //o servidor nunca para
	{
	//strings para formar arquivo
	//Armazena e-mail from
	string str_mail = "";
	//Armazena e-mail to
	string str_rcpt = "";
	//Armazena mensagem
	string mensagem = "";
	//Nome do arquivo
	string nome_arquivo = "";

	//O cliente conecta
	cliente_sock = accept(sock, (struct sockaddr*)&cliente, &addrlen);

	//Quando o servidor aceita uma comunicacao ele envia uma mensage (Primeira Mensagem)
	envia = "220 SERVIDOR TP1 SMTP\n";
	char_envia=envia.c_str(); //converte a string
	send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente

	//inicializando variáveis a cada nova conexão
	recebeu_helo = false;
	recebeu_mail = false;
	recebeu_rcpt = false;
	recebeu_msg = false;
	recebeu_data = false;
	recebeu_rset = false;
	recebeu_noop = false;
	recebeu_quit = false;
	loop2 = true;

	do {//Loop2
		recebe.clear();
		
		//Esvazia char_recebe
		bzero(char_recebe,TAMANHO_BUFFER_RECEBE);
		//Escuta mensagens e se recebe armazena em "recebe"
		read(cliente_sock, char_recebe, TAMANHO_BUFFER_RECEBE);//;recv (, char_recebe, ,MSG_WAITALL);								
		recebe=string(char_recebe); //converte para string										

		if ((recebeu_data == true) && (recebeu_msg == false))
			{
			//Envia mensagem de confirmação por ter recebido a mensagem
			envia = "250 OK\n";
			char_envia=envia.c_str(); //converte a string
			send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
			//carrega string para armazenamento
			mensagem.clear();
			if (recebe [recebe.length()-2] == '\r')
				mensagem = recebe.substr(0 , (recebe.size()-5)) + "\n";		//Retira<CRLF>.<CRLF> = \r\n.\r\n
			else 															//Retira<CR>.<CR> = Termina com "\n.\n" 	
				mensagem = recebe.substr(0 , (recebe.size()-3)) + "\n";
			recebeu_msg = true;
			}

		//Testa mensagem HELO
		int i = 0;
		do{
			if (recebe[i] == helo[i] || recebe[i] == H_E_L_O[i])
				{
				continua = true;
				if (i == 4) //compara o ultimo caracter e esta certo e o comando certo
					{
					//Envia mensagem de confirmação do comando helo
					envia = "250 Hello\n";
					char_envia=envia.c_str(); //converte a string
					send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
					continua = false;
					//Recebeu comando helo
					recebeu_helo = true;
					}
				}
			else
				{
				//Nao e o comando helo
				continua = false;
				}
			i++;
			} while (continua);

		//Testa mensagem MAIL
		i = 0;
		do{
			if (recebe[i] == mail[i] || recebe[i] == M_A_I_L[i])
				{
				continua = true;
				if (i == 9) //compara o ultimo caracter e esta certo -> "MAIL FROM:"
					{
					unsigned int num_at = 0;
					//Procura pelo @
					for (unsigned int aux = i+1; aux <= recebe.length()-2; aux++)
						{
						if (recebe[aux] == at)			//o caracter é um @?
							num_at++;
						str_mail = str_mail + recebe[aux];
						}
					if (num_at == 1 ) //&& recebe[recebe.length()-2] == '\r' && recebe[recebe.length()-1] == '\n' )		//tem 1 @ e finaliza certo?
						{
						//Envia mensagem de confirmação do comando MAIL
						envia = "250 OK\n";
						char_envia=envia.c_str(); //converte a string
						send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
						//MAIL FROM preenchido
						recebeu_mail = true;
						}
					continua = false;
					}
				}
			else
				{
				//Nao e o comando MAIL
				continua = false;
				}
		i++;
		} while (continua);

		//Testa mensagem RCPT
		i = 0;
		do{
			if (recebe[i] == rcpt[i] || recebe[i] == R_C_P_T[i])
				{
				continua = true;
				if (i == 7) //compara o ultimo caracter e esta certo -> "RCPT TO:"
					{
					unsigned int num_at = 0;
					//Procura pelo @ e armazena o nome do arquivo RCPT TO:<nome do arquivo>@domain
					for (unsigned int aux = i+1; aux <= recebe.length()-2; aux++)
						{
						if (recebe[aux] == at)
							{
							//Nome do arquivo
							nome_arquivo = str_rcpt;
							num_at++;
							}
						str_rcpt = str_rcpt + recebe[aux];
						}
					if (num_at == 1 ) //&& recebe[recebe.length()-2] == '\r' && recebe[recebe.length()-1] == '\n' )		//tem 1 @ e finaliza certo?
						{
						//Envia mensagem de confirmação do comando RCPT
						envia = "250 Accepted\n";
						char_envia=envia.c_str(); //converte a string
						send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
						//Se entrou aqui, "nome_arquivo" contém o nome do arquivo certo
						//MAIL FROM preenchido
						recebeu_rcpt = true;
						}
					continua = false;
					}
				}
			else
				{
				//Nao e o comando RCPT
				continua = false;
				}
			i++;
			} while (continua);

			//Testa mensagem DATA
			i = 0;
			do{
				if (recebe[i] == data[i] || recebe[i] == D_A_T_A[i])
					{
					continua = true;
					if (i == 5) //compara o ultimo caracter e esta certo e o comando certo
						{
						//Envia mensagem de confirmação do comando data
						envia = "354 Enter message\n";
						char_envia=envia.c_str(); //converte a string
						send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
						//Flags de recebeu data e recebeu mensagem
						recebeu_msg = false;
						recebeu_data = true;
						continua = false;
						}
					}
				else
					{
					//Nao e o comando data
					continua = false;
					}
				i++;
				} while (continua);

			//Testa mensagem RSET
			i = 0;
			do{
				if (recebe[i] == rset[i] || recebe[i] == R_S_E_T[i])
					{
					continua = true;
					if (i == 5) //compara o ultimo caracter e esta certo e o comando certo
						{
						//Envia mensagem de confirmação do comando rset
						envia = "250 Reset OK\n";
						char_envia=envia.c_str(); //converte a string
						send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
						//"Zera" Flags: Inicia o server novamente
						recebeu_helo = false;
						recebeu_mail = false;
						recebeu_rcpt = false;
						recebeu_msg = false;
						recebeu_data = false;
						recebeu_rset = false;
						recebeu_noop = false;
						recebeu_quit = false;
						}
					}
				else
					{
					//Nao e o comando data
					continua = false;
					}
				i++;
				} while (continua);
			//Testa mensagem NOOP
			i = 0;
			do{
				if (recebe[i] == noop[i] || recebe[i] == N_O_O_P[i])
					{
					continua = true;
					if (i == 5) //compara o ultimo caracter e esta certo e o comando certo
						{
						//Envia mensagem de confirmação do comando noop
						envia = "250 OK\n";
						char_envia=envia.c_str(); //converte a string
						send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
						continua = false;
						//Recebeu comando noop
						recebeu_noop = true;
						}
					}
				else
					{
					//Nao e o comando data
					continua = false;
					}
				i++;
				} while (continua);

			//Testa mensagem QUIT
			i = 0;
			do{
				if (recebe[i] == quit[i] || recebe[i] == Q_U_I_T[i])
					{
					continua = true;
					if (i == 5) //compara o ultimo caracter e esta certo e o comando certo
						{
						//Envia mensagem de confirmação do comando quit
						envia = "221 FECHANDO CONEXAO\n";
						char_envia=envia.c_str(); //converte a string
						send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
						//Fecha conexao com o cliente
						close (cliente_sock);
						//Recebeu comando quit
						recebeu_quit = true;
						if ((recebeu_mail == true && recebeu_rcpt == true) && (recebeu_msg == true && recebeu_data == true))		//e-mail completo?
							{
							cout << "Cria arquivo" << endl;
							//pegar nome do arquivo
							nome_arquivo = nome_arquivo + ".mbox";
							//Montar arquivo
							//Funcoes abrir arquivo para gravacao
							ofstream outClientFile(nome_arquivo.c_str(), ios::app);
							if (!outClientFile)
								{
								cerr << "Arquivo nao pode ser aberto" << endl;
								exit(1);
								}
							//Grava arquivo mensagem
							mensagem = "\nDelivered-to: " + str_rcpt + "From: " + str_mail + "\n" + mensagem + "====================";
							outClientFile << mensagem;
							}
						continua = false;
						loop2 = false;
						}						
					}
				else
					{
					//Nao e o comando data
					continua = false;
					}
				i++;
				} while (continua);

		//Comando errado
		if (recebeu_helo == false &&
		recebeu_mail == false &&
		recebeu_rcpt == false &&
		recebeu_msg == false &&
		recebeu_data == false &&
		recebeu_rset == false &&
		recebeu_noop == false &&
		recebeu_quit == false)
			{
			//Envia mensagem de erro de comando
			envia = "500 Comando nao reconhecido\n";
			char_envia=envia.c_str(); //converte a string
			send(cliente_sock,char_envia,envia.length(),0);	//envia a msg pro cliente
			}
		} while (loop2);
	}
	return(0);
}


