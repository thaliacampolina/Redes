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
#include <limits.h>


int main(int argc, char* argv[]) {

    int sock, cliente_sock;
    struct sockaddr_in sock_param, cliente;
    socklen_t addrlen= sizeof(cliente);

    sock = socket(PF_INET, SOCK_DGRAM, 0);


    bind (sock, (struct sockaddr*)&sock_param, sizeof(sock_param)); 
    // /\ vincula o socket a um endereço ip

    listen(sock, 5); //"escuta" o socket esperando conexao,
    // numero de conexções q ele guenta ate a certa aparecer (5 maximo)

    puts("conexao estabelecida");
    while (1) { //o servidor nunca para
    //O cliente conecta
        cliente_sock = accept(sock, (struct sockaddr*)&cliente,
                                                          &addrlen);
        send(cliente_sock, "OLA", 4, 0);
        write(cliente_sock,"I got your message",18);


    }
    return(0);
}
