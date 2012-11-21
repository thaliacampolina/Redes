#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include "encode.h"

#define PORT 24007

// Funcao que encoda o arquivo a ser anexado. E quase uma copia do main do TP0.
int encodeb64(char nome_arquivo[]){ 
	FILE *read = fopen(nome_arquivo, "rb");
	FILE *write= fopen("temp.b64", "wb");
	unsigned char str[4]; 
	unsigned char buf, c;
	int controle = 1, i;
	while (controle){
		Inicializa(str);
		for (i = 0 ; i < 3 ; i++){
			if (fscanf(read, "%c", &c) == -1){
				controle = 0;
				break;
			}
		str[i] = c;
		str[3] = (char)((int)str[3] + 1);
		}
		Encode(str, write);
	}
	fclose(read);
	fclose(write);
	return 0;
}

char *hora (){
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  return asctime(timeinfo);
}

void getAssunto(char assunto[]){
  printf("Assunto: ");
  fgets(assunto, 255, stdin);
}

void getMensagem(char mensagem[], int client_sock, int argc, char nome_arquivo[]){
  char linha[255];
  int controle = 0;
  linha[0] = '\0';
  printf("Digite sua mensagem. Termine a escrita com uma linha terminada por FIM\n");
  while(1){
    fgets(linha, 255, stdin);
    if (strcmp(linha,"\n") == 0){
      fgets(linha, 255, stdin);
      if ((strcmp(linha, "FIM\n") == 0)){
				if (nome_arquivo != NULL){
					encodeb64(nome_arquivo);
				}
        return;
      }else{
        mensagem = strcat(mensagem, "\n");
      }
    }
    mensagem = strcat(mensagem, linha);
  }
}

int MsgOk(char msg[]){
	char status = msg[0];
	if (status == '2' || status == '3'){
		return 1;
	} else {
		return 0;
	}
}

int main(int argc, char **argv){
	
	// Socket do cliente.
	int client_sock;

	// Tamanho da string recebida pelo recv.
	int tamanho, controle;

	// Variavel que guardara as strings de passagem.
	char buf[255], msg[1024], assunto[255];
	unsigned char buf2[80];

	// Declara a estrutura do servidor ao qual o cliente ira se conectar.
	struct hostent *server;

	// Declara a estrutura do socket do cliente.
	struct sockaddr_in client;

	server = gethostbyname(argv[1]); // argv[1]

	client_sock = socket(AF_INET, SOCK_STREAM,0);

	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	client.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(client.sin_zero), 8);

	// Conecta o cliente ao servidor.
	connect(client_sock, (struct sockaddr *)&client, sizeof(struct sockaddr));

	// O cliente envia as perguntas e recebe as respostas do servidor.

  // Recebe do servidor a mensagem que eles podem trabalhar.
  tamanho = recv(client_sock, buf, 255, 0);
  buf[tamanho] = '\0';
  printf("S:%s", buf);

  // O cliente envia o Helo para o servidor.
  send(client_sock, "HELO kurunfinwe.arda\n", strlen("HELO kurunfinwe.arda\n"), 0);

  // O cliente recebe a confirmacao do servidor.
  tamanho = recv(client_sock, buf, 255, 0);
  buf[tamanho] = '\0';
  printf("S:%s", buf);

  // O cliente envia o email "from".
  sprintf(buf, "MAIL FROM: <%s >\n", argv[2]); //argv[2]
  send(client_sock, buf, strlen(buf), 0);

  // O cliente recebe a confirmacao que email "from" esta OK.
  tamanho = recv(client_sock, buf, 255, 0);
  buf[tamanho] = '\0';
	if (!MsgOk(buf)){
		printf("Mensagem do Sistema: O campo e-mail FROM o foi digitado incorretamente.\nFechando o cliente.\n");
		close (client_sock);
		return 0;
	}
  printf("S:%s", buf);

  // O cliente envia o email "to".
  sprintf(buf, "RCPT TO: <%s >\n", argv[3]); //argv[3]
  send(client_sock, buf, strlen(buf), 0);

  // O cliente recebe a confirmacao que email "to" esta OK.
  tamanho = recv(client_sock, buf, 255, 0);
  buf[tamanho] = '\0';
	if (!MsgOk(buf)){
		printf("Mensagem do Sistema: O campo e-mail to o foi digitado incorretamente.\nFechando o cliente.\n");
		close (client_sock);
		return 0;
	}
  printf("S:%s", buf);

  // Cliente escreve o Assunto e a Mensagem.
  getAssunto(assunto);
	sprintf(msg, "Date: %sSubject: %s\n", hora(), assunto);
  getMensagem(msg, client_sock, argc, argv[4]);

  // O cliente envia que vai mandar o email.
  send(client_sock, "DATA\n", 5, 0);

  // O cliente recebe a confirmacao que pode enviar o email.
  tamanho = recv(client_sock, buf, 255, 0);
  buf[tamanho] = '\0';
  printf("S:%s", buf);

  // O cliente envia o email. 	
 	send(client_sock, msg, strlen(msg), 0);

	// Caso haja anexo:
	
	if (argv[4] != NULL){
		// O cliente envia o cabecalho do email.
		char cabecalho[200];

		sprintf(cabecalho, "\n\n--1afdb9e0b0083ce1d4c302fb633c5082\nContent-Type: application/octet-stream; name=\"%s\"\nContent-Disposition: attachment; filename=\"%s\"\nContent-Tranfer-Encoding: base64\n\n", argv[4], argv[4]);
		send(client_sock, cabecalho, strlen(cabecalho), 0); 

		// O cliente envia o anexo.
		FILE *read = fopen("temp.b64", "rb");
		while(fgets(buf2, 78, read) != NULL){
			send(client_sock, buf2, strlen(buf2) , 0);
		}
		send(client_sock, "\n--1afdb9e0b0083ce1d4c302fb633c5082--\n", 38, 0);
		fclose(read);
	}

	send (client_sock, "\r\n.\r\n", 5, 0);

  // O cliente recebe que o servidor esta com a mensagem.
  tamanho = recv(client_sock, buf, 255, 0);
  buf[tamanho] = '\0';
  printf("Mensagem enviada com sucesso!\n");

  // O cliente envia a requisicao de saida.
  send(client_sock, "QUIT\n", 5, 0);

  // O cliente recebe o final da conexão e desliga.
  tamanho = recv(client_sock, buf, 255, 0);
  buf[tamanho] = '\0';
  printf("S:%s", buf);

	close(client_sock);
	return 0;
}
