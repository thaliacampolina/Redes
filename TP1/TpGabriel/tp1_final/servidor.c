#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORT 20000

void IdentificaFinal(int sock, int *ctrl, int *ctrl_msg, char mensagem[]){
	int tam_buf = strlen(mensagem);
	if (*ctrl == 1 && tam_buf == 3 && mensagem[tam_buf-1] == '\n' && mensagem[tam_buf-2] == '\r' && mensagem[tam_buf-3] == '.'){
		send(sock, "250 Message accepted for delivery\n", 34, 0);
		*ctrl_msg = 1;		
		*ctrl = 2;
	}else{
		*ctrl = 0;
	}
	if(mensagem[tam_buf-1] == '\n'){
		if(mensagem[tam_buf-2] == '\r'){
			*ctrl = 1;
		}
	}
	if (FinalMensagem(mensagem, strlen(mensagem))){
		send(sock, "250 Message accepted for delivery\n", 34, 0 );
		*ctrl_msg = 1;
	} 
}

int NomeMail(char email_completo[], char nome[]){
	int dominioOK = 0, posicao;
	int i;
	for (i = 0 ; i < strlen(email_completo) ; i++){
		if (email_completo[i] == '@'){
			dominioOK = 1;
			posicao = i;
		}
		nome[i] = email_completo[i];
	}
	if (dominioOK == 1){
		nome[posicao] = '\0';
		strcat(nome,".mbox");
	}
	return dominioOK;
}

int FinalMensagem(char mensagem[], int tamanho){
	char *c = strstr(mensagem, "\r\n.\r\n");
	if (c != NULL) return 1;
	return 0;
}

int compara(char v1[], char v2[]){
	int i;
	for (i = 0 ; i < 4 ; i++){
		if (v1[i] != v2[i]) return 0;
	}
	return 1;
}

int Verifica(char buf[]){
	char *comandos[5] = {"HELO", "MAIL", "RCPT" , "DATA", "QUIT"};
	char aux[4];
	int i;
	for (i = 0 ; i < 4 ; i++){
		aux[i] = buf[i];
	}
	for (i = 0 ; i < 5 ; i++){
		if (strcasecmp(aux, comandos[i]) == 0) return i+1;
	}
	return -1;
}

struct mail{
  char mensagem[1024],
       nome_cliente[16],
       mail_from[32],
       mail_to[32],
			 nome_to[32];
};

int main(){
	// Declara socket do servidor.
	int server_sock, atual_sock;

  // O buffer
	int tam_buf;
	char buf[255];

	// Caractere de verificacao da informacao enviada pelo cliente.
	int verif,ctrl;

	// Variavel para controlar o fim da comunicação.
	int controle, controle_msg;

	// Definimos para o assunto um máximo de 100 Bytes e para a mensagem,
	// um máximo de 1MB.
	char assunto[100], mensagem[1024];

	// Declara estrutura dos sockets do servidor e cliente.
	struct sockaddr_in server_in;
	struct sockaddr_in client_in;

	// para escrever no arquivo.
	FILE *write;

  // Declara a estrutura que guarda as informacoes vindas do servidor.
  struct mail email;

	// Cria o socket do servidor.
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	// Atribuição da estrutura.
	memset(&server_in, 0, sizeof(server_in));
	server_in.sin_family = AF_INET;
	server_in.sin_addr.s_addr = INADDR_ANY;
	server_in.sin_port = htons(PORT);

	// Conecta a porta ao socket.
	bind(server_sock, (struct sockaddr *)&server_in, sizeof(server_in));

	// Seta o socket para escutar até 5 conexões.
	listen(server_sock, 5);

	while (1){

		// Espera a conexão do cliente.
		int tamanho = sizeof(client_in);
		atual_sock = accept(server_sock, (struct sockaddr *) &client_in, &tamanho);
	
		// Inicia a variavel de controle.
		controle = 0;	
		controle_msg = 0;
		
		// Inicia a mensagem.
		mensagem[0] = '\0';
	
  	// O servidor comeca a conversar com o cliente.

		// O servidor envia para o cliente que está disponível.
		send(atual_sock, "220 manwe.me\n", strlen("220 manwe.me\n"), 0);

		// O servidor comeca a receber os dados vindos do cliente. Dependendo das
		// requisicoes, o servidor responde de uma maneira diferente.
		while(controle != 1){
			// Recebe o dado do cliente.
			tam_buf = recv(atual_sock, buf, 255, 0);
			buf[tam_buf] = '\0';
			verif = Verifica(buf);
		
			switch (verif){
			// Trata o HELO.
			case 1:
				sscanf(buf, "%*s %s", email.nome_cliente);
				email.nome_cliente[tam_buf-5] = '\0';
				sprintf(buf, "250 Hello %s, pleased to meet you\n", email.nome_cliente);
				send(atual_sock, buf, strlen(buf), 0);
				break;
			// Trata o MAIL.
			case 2:
				sscanf(buf, "%*s %*s %*c%s%*c", email.mail_from);
				email.mail_from[tam_buf-15] = '\0';
				if ((NomeMail(email.mail_from, email.nome_to)) != 0){
					sprintf(buf, "250 %s... Sender ok\n", email.mail_from);
				} else {
					sprintf(buf, "553 Requested action not taken: mailbox name not allowed\n", 53, 0);
				}
				send(atual_sock, buf, strlen(buf), 0);
				break;
			// Trata o RCPT.
			case 3:
				sscanf(buf, "%*s %*s %*c%s%*c", email.mail_to);
				email.mail_to[tam_buf-13] = '\0';
				if ((NomeMail(email.mail_to, email.nome_to)) != 0){
					sprintf(buf, "250 %s... Recipient ok\n", email.mail_to);
				} else {
					sprintf(buf, "553 Requested action not taken: mailbox name not allowed\n", 53, 0);
				}
				send(atual_sock, buf, strlen(buf), 0);
				break;
			// Trata a DATA.
			case 4:
				// Primeiramente, recebe o assunto.
				write = fopen(email.nome_to, "a+b");
				fprintf(write, "Delivered-to: %s\nFrom: %s\n", email.mail_to, email.mail_from);
				controle_msg = 0;
				ctrl = 0;
				send(atual_sock, "354 Enter mail, end with \".\" on a line by itself\n", 50, 0);
				while(controle_msg != 1){
					tam_buf = recv(atual_sock, mensagem, 1024, 0);
					mensagem[tam_buf] = '\0';
					IdentificaFinal(atual_sock, &ctrl, &controle_msg, mensagem);
					if(controle_msg == 1 && ctrl != 2){
				 		if (mensagem[0] == '.'){ 
							mensagem[0] = '\0';
						} else {
							mensagem[tam_buf - 5] = '\0';
						}
					}
					if (ctrl != 2) fprintf(write, "%s", mensagem);
				}
				fprintf(write,"\n====================\n");
				fclose(write);
				break;
			// Trata o QUIT.
			case 5:
				controle = 1;
				send(atual_sock, "221 manwe.me closing connection\n", 32, 0);
				close(atual_sock);
				break;
			// Outros.
			default:
				send(atual_sock, "500 Syntax error, command unrecognized\n", 39, 0);
				break;
			}
		}
	}
	close(server_sock);
	return 0;
}
