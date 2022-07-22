/*
*   Matheus Barbosa Consul
*   https://github.com/MatheusConsul/EC-Servidor-HTTP 
*
*   Codigo consegue receber uma requisição do cliente identificar o inicio do cabeçalho e enviar uma resposta.
*   Como cliente foi usado o terminal e o navegador acessando o http://localhost:5000/
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>




int main(){

    struct sockaddr_in caddr;
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(4000)
    };


    int server = socket(AF_INET, SOCK_STREAM, 0);
    int client;
    int csize  = sizeof caddr;

     char buff[500];

    bind(server, (struct sockaddr *) &saddr, sizeof saddr);
    listen(server, 5); //especifica o numero de conexoes que podem estar em espera num socket de um servidor (por exemplo, chamadas telefonicas em espera)


    while(1){
        puts("\n >>>>> Inicio do while de escuta da porta \n");

        client = accept(server, (struct sockaddr *) &caddr, &csize);

        recv(client, buff, sizeof buff, 0); // tinha ponteiro *cliente //Realiza a leitura do buffer de entrada do cliente
        printf("\n >>>>> Cliente %d LEU O BUFFER \n",client);
       
        fflush(stdout);

        char resposta[50] = "\n\n Resposta do servior \n Hello word!\n\n";

        send(client,resposta,50, 0); // endereço socket cliente, char de resposta, tamanho da resposta e 0 nao lembro  

        close(client); // fechando a conexão com o cliente 

        memset(&buff, 0, sizeof buff);
        
        puts("\n >>>>> Fim do while de escuta \n");

    }

}