#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h> 
#define PORTA 1234

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
	char host[16];		//ip servidor
	char username[50];		//username
	char password[50];
	if (argc != 4)
		{
		printf("ERRO: Argumentos errados\nReinicie com os argumentos corretos: <username> <password> <IP do servidor>");
		return 0;
		}

	// armazena argumentos
	strcpy(username,argv[1]);
	strcpy(password,argv[2]);
	strcpy(host,argv[3]);
    int sockfd, portno, n=0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char hostname[1024];
    hostname[1023]='\0';
    gethostname(hostname, 1023);
    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket"); //erro na abertura do socket
    server = gethostbyname(hostname);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORTA);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting"); //erro na conexão
    while(1){
	    bzero(buffer,256);
	    fgets(buffer, 255, stdin);
	    n = write(sockfd,buffer,strlen(buffer));
	    if (n < 0) error("ERROR writing to socket");
	    if (strncmp(buffer, "QUIT", 4) == 0) break;
	    else if (strncmp(buffer, "STAT", 4) == 0) printf("type enter again");
	    else if (strncmp(buffer, "LIST", 4) == 0) printf("type enter again");	    
	    bzero(buffer,256);
//	    sleep(10);
//	    while(n==0){
	    	n = read(sockfd,buffer,255);
//	    	sleep(1);
//	    }	    	
	    if (n < 0) error("ERROR reading from socket");
	    printf("%s\n",buffer);
    }
    close(sockfd);
    shutdown(sockfd, 2);
    return 0;
}
