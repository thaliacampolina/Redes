/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#define PORTA 1234

//vetor que guarda os tamanhos de cada arquivo (e-mail)
static vetor_tamanhos[100];

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int conta_emails(char nome[50]){
	int contador=0;
	FILE *input;
	char nova[60], number;
	strcpy(nova,nome);
	strcat(nova, ".1.txt");
	int size=strlen(nova);
	while(1){
		input = fopen(nova, "r");
		if(input==NULL) break;
		fclose(input);
		contador++;
		number = (contador+48);
		nova[size-5]=number;
		}
	contador--;
	return contador;
}

void tamanho_emails(char nome[50]/*, int* tamanho[]*/){
	int contador=1, x[50];
	FILE *input;
	char nova[60], number;
	strcpy(nova,nome);
	strcat(nova, ".1.txt");
	int size=strlen(nova);
	while(1){
		input = fopen(nova, "r");
		if(input==NULL) break;
		fseek (input, 1 , SEEK_END);
		vetor_tamanhos[contador] = (ftell(input)-1);
		fclose(input);
		contador++;
		number = (contador+48);
		nova[size-5]=number;
		}
}

int tamanho_total(){
	int i, total=0;
	for (i=0; i<50; i++){
		total+= (vetor_tamanhos[i]);
		}
	return total;
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, i, tamanho_nome, j, n, *tamanho;
     FILE *retrieve;
     socklen_t clilen;
     char buffer[256], nome[50], nomedoarquivo[60], caractere, mensagem[100], msg_curta[10], buffer_retr[10*1024];
     struct sockaddr_in serv_addr, cli_addr;
     //declaração das variaveis
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) error("ERROR opening socket"); //erro na abertura do socket
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORTA);
     //conexão
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding"); //erro na vinculação
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //aceitar a conexão
     if (newsockfd < 0) error("ERROR on accept"); //erro na aceitação
     while(1){
	     bzero(buffer,256);
	     n = read(newsockfd,buffer,255);
	     if (n < 0) error("ERROR reading from socket"); //erro na leitura do socket do cliente
	     switch (buffer[0]){
		     case 'U':	j=5;
		     		for (i=0; i<50; i++){
					buffer[i]=buffer[j];
					j++;
					}
				strcpy(nome, buffer);
				tamanho_nome=strlen(nome);
				nome[tamanho_nome-1]='\0';
				break;
		     case 'P':  tamanho_emails(nome);
		     		sprintf(mensagem, "+OK user %s has %i messages (%i bytes)\n", nome, conta_emails(nome), tamanho_total());
		     		write(newsockfd, mensagem, strlen(mensagem));
		     		break;
		     case 'S':  tamanho_emails(nome);
		     		sprintf(mensagem, "+STATOK %i %i", conta_emails(nome), tamanho_total());
		     		fflush(stdin);
		     		write(newsockfd, mensagem, strlen(mensagem));
		     		break;
		     case 'L':	tamanho_emails(nome);
		     		sprintf(mensagem, "+LISTOK %i messages (%i bytes)\n", conta_emails(nome), tamanho_total());
		     		fflush(stdin);
		     		write(newsockfd, mensagem, strlen(mensagem));
		     		for (i=1; i<=(conta_emails(nome)); i++){
		     			sprintf(msg_curta, "%i %i\n", i, vetor_tamanhos[i]);
		     			write(newsockfd, msg_curta, strlen(msg_curta));
		     			}
		     		break;
		     case 'R':  strcpy(nomedoarquivo, nome);
		     		strcat(nomedoarquivo, ".1.txt");
		     		caractere=buffer[5];
     				nomedoarquivo[(strlen(nomedoarquivo))-5]=caractere;
		     		retrieve=fopen(nomedoarquivo, "r");
		     		while(!EOF) fscanf(retrieve, "%s", &buffer_retr);
		     		puts(buffer_retr);
		     case 'Q':	close(newsockfd);
		     		close(sockfd);
		     		return 0;
		     default: 	break;
		     }
	     n = write(newsockfd," ",1);
	     if (n < 0) error("ERROR writing to socket"); //erro na escrita no socket do cliente
     }
     return 0; 
}
