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

//funcao que retorna um erro
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

//funcao que conta o numero de emails
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

// retorna o tamanho de cada email
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

//retorna o tamanho total de todos os emails
int tamanho_total(){
	int i, total=0;
	for (i=0; i<50; i++){
		total+= (vetor_tamanhos[i]);
		}
	return total;
}

int main(int argc, char *argv[]) {
     //inicia a declaracao de variaveis
     int sock_connect, sock_client, portno, i, tamanho_nome, j, n, *tamanho;
     FILE *retrieve;
     socklen_t clilen;
     char buffer[10*1024], nome[50], nomedoarquivo[60], caractere, mensagem[100], msg_curta[10], buffer_retr[10*1024];
     struct sockaddr_in serv_addr, cli_addr;
     //fim da declaração das variaveis

     //inicio da conexão
     sock_connect = socket(AF_INET, SOCK_STREAM, 0);
     if (sock_connect < 0) error("ERROR opening socket"); //erro na abertura do socket
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORTA);
     //fim da conexão

     if (bind(sock_connect, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding"); //erro na vinculação
     listen(sock_connect,5);
     clilen = sizeof(cli_addr);
     sock_client = accept(sock_connect, (struct sockaddr *) &cli_addr, &clilen); //aceitar a conexão
     if (sock_client < 0) error("ERROR on accept"); //erro na aceitação
     while(1){
	     bzero(buffer,10*1024);
	     n = read(sock_client,buffer,10*1024);
	     if (n < 0) error("ERROR reading from socket"); //erro na leitura do socket do cliente

             //comandos POP, sendo reconhecidos pela primeira letra dos mesmos (que é sempre diferente)
	     switch (buffer[0]){
                    //USER
		     case 'U':	j=5;
		     		for (i=0; i<50; i++){
					buffer[i]=buffer[j];
					j++;
					}
				strcpy(nome, buffer);
				tamanho_nome=strlen(nome);
				nome[tamanho_nome-1]='\0';
				break;
                     //PASS
		     case 'P':  tamanho_emails(nome);
		     		sprintf(mensagem, "+OK user %s has %i messages (%i bytes)\n", nome, conta_emails(nome), tamanho_total());
		     		write(sock_client, mensagem, strlen(mensagem));
		     		break;
                     //STAT
		     case 'S':  tamanho_emails(nome);
		     		sprintf(mensagem, "+OK %i %i", conta_emails(nome), tamanho_total());
		     		fflush(stdin);
		     		write(sock_client, mensagem, strlen(mensagem));
		     		break;
                     //LIST
		     case 'L':	tamanho_emails(nome);
		     		sprintf(mensagem, "+OK %i messages (%i bytes)\n", conta_emails(nome), tamanho_total());
		     		fflush(stdin);
		     		write(sock_client, mensagem, strlen(mensagem));
		     		for (i=1; i<=(conta_emails(nome)); i++){
		     			sprintf(msg_curta, "%i %i\n", i, vetor_tamanhos[i]);
		     			write(sock_client, msg_curta, strlen(msg_curta));
		     			}
		     		break;
                     //RETR
		     case 'R':  strcpy(nomedoarquivo, nome);
		     		strcat(nomedoarquivo, ".1.txt");
		     		caractere=buffer[5];
     				nomedoarquivo[(strlen(nomedoarquivo))-5]=caractere; //coloca o numero do email passado como parametro no RETR
		     		retrieve=fopen(nomedoarquivo, "r");
                                i = 0;
                                buffer_retr[i]=fgetc(retrieve);
		     		while(buffer_retr[i] != EOF) {
                                	i++;
                                        puts("entro no while");
                                	/*fscanf(retrieve, "%s", &buffer_retr);*/ 
                                        buffer_retr[i]=fgetc(retrieve);
                                        printf("buf: %c\n", buffer_retr[i]);
                                }
		     		write(sock_client,buffer_retr, strlen(buffer_retr)-1);
	                        bzero(buffer_retr,10*1024);
                                break;
                     //DELETE
		     case 'D':  strcpy(nomedoarquivo, nome);
                                strcat(nomedoarquivo, ".1.txt");
                                caractere=buffer[5];
                                nomedoarquivo[(strlen(nomedoarquivo))-5]=caractere; //coloca o numero do email passado como parametro no RETR
                                remove(nomedoarquivo);
		     		sprintf(mensagem, "+OK message %c will be deleted\n", caractere);
		     		write(sock_client, mensagem, strlen(mensagem));
                                break;
 
                     //QUIT
		     case 'Q':	close(sock_client);
		     		close(sock_connect);
		     		return 0;
		     default: 	break;
		     }
	     n = write(sock_client," ",1);
	     if (n < 0) error("ERROR writing to socket"); //erro na escrita no socket do cliente
     }
     return 0; 
}
