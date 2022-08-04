/*
*   Matheus Barbosa Consul
*   Gabriel Silveira Corrêa 
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "servidor.h"
#include <pthread.h>

#define numMaxClientes 10
int numClientesOnline = -1;
pthread_t thredClientes[numMaxClientes];


int lerRequisicao(char requisicao[], int cliente);
void responderCliente (int cliente);
void *novoCliente(void *  client);



void iniciarServidor (){
  
    struct sockaddr_in caddr;
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(4000)
    };


    int server = socket(AF_INET, SOCK_STREAM, 0);
    int client;
    int csize  = sizeof caddr;

    bind(server, (struct sockaddr *) &saddr, sizeof saddr);
    listen(server, 5); //especifica o numero de conexoes que podem estar em espera num socket de um servidor (por exemplo, chamadas telefonicas em espera)


    while(1){
        puts("\n >>>>> Inicio do while de escuta da porta \n");

        client = accept(server, (struct sockaddr *) &caddr, &csize);

        numClientesOnline++;
        
        puts("\n >>>>> Criando uma thread para o cliente \n");

        pthread_create(&thredClientes[numClientesOnline],NULL, novoCliente,&client);
       
        fflush(stdout);
        
        // PROBLEMA AO FECHAR O NAVEGAR SEM FECHAR A CONEXÃO ENCERRA A EXECUÇÃO DO SERVIDOR 
        
        puts("\n >>>>> Fim do while de escuta \n");
       // memset(&client, 0, sizeof client);  
    }

}



void * novoCliente(void * client ){  // para tratar uma nova conexão 

    int * cl= ((int*)client);
    int cliente = *cl;
    int x = 1;

    char buff[1000];

    printf("\n >>>>> Thread do cliente %d e vai ENTRAR NO LOOP DE CONEXÃO \n",cliente); 
    
    /* não sei porque parece que estado de espera esta sendo na leitura do buffer */

    do{
        printf("\n >>>>> Thread do cliente %d e INICIO DO LOOP DE CONEXÃO \n",cliente);

        recv(cliente, buff, sizeof buff, 0); // tinha ponteiro *cliente //Realiza a leitura do buffer de entrada do cliente

        printf("\n >>>>> Thread do cliente %d LEU O BUFFER VAI CHAMAR O lerRequisicao \n",cliente);
        x = lerRequisicao(buff,cliente);  // fazer a leitura da requisição do navegar tinha ponteiro *cliente

        memset(&buff, 0, sizeof buff);

    }while (x == 1);

    printf("\n >>>>> Thread do cliente %d APOS LOOP DE CONEXAO, FECHANDO A CONEXAO E A THREAD\n",cliente);
     
    close(cliente); // fechando a conexão com o cliente 
    pthread_exit(NULL); // fechando a thread referente ao cliente 
    
}


void responderCliente (int cliente){

    printf("\n >>>>> Thread do cliente %d DENTRO DO responderCliente \n",cliente);


    FILE *arquivo;
                             // caminho especifico do arquivo a ser enviado
    char caminho[100] = "/home/ec/Documentos/GitHub/EC-Servidor-HTTP/3ª Versão/imagem001.jpeg"; 


    if ((arquivo = open(caminho, O_RDONLY)) > 0){

            puts("Imagem encontrada.");
            int bytes;
            char buffer[1024];

            send(cliente, "HTTP/1.0 200 OK\r\nContent-Type: image/jpeg\r\n\r\n", 45, 0); 
            bytes = read(arquivo,buffer,1024);
            write(cliente,buffer,bytes);

	        while ( (bytes=read(arquivo, buffer, 1024))>0 ) {// lendo o arquivo do buffer
                    printf("\nDENTRO DO WHILE DO ENVIO DE ARQUIVO");
			      write (cliente, buffer, bytes); // enviando jpeg para cliente
              }

             

    }


    char resposta1[500] = "HTTP/1.1 200 OK\nDate: Tue, 09 Aug 2011 15:44:04 GMT\nServer: Apache/2.2.3(CentOS)\nLast-Modified: Tue, 09 Aug 2020 15:11:03 GMT\nContent-Length: 6821\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Esta e a resposta do servidor cliente nº: !!! <img src=\"http://www.lkmescritorios.com.br/wp-content/uploads/2016/04/5-D%C3%9AVIDAS-ESCRIT%C3%93RIO-VIRTUAL-720x720.jpg\"> <h1><body></html>";

    char resposta2[500] = "HTTP/1.1 200 OK\nDate: Tue, 09 Aug 2011 15:44:04 GMT\nServer: Apache/2.2.3(CentOS)\nLast-Modified: Tue, 09 Aug 2020 15:11:03 GMT\nContent-Length: 6821\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Esta e a resposta do servidor cliente nº: !!! <img src=\"/home/ec/Documentos/GitHub/EC-Servidor-HTTP/imagem001.jpg\"> <h1><body></html>";

    char resposta3[500] = "HTTP/1.1 200 OK\nDate: Tue, 09 Aug 2011 15:44:04 GMT\nServer: Apache/2.2.3(CentOS)\nLast-Modified: Tue, 09 Aug 2020 15:11:03 GMT\nContent-Length: 6821\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Esta e a resposta do servidor cliente nº: !!! <h1><body></html>";

    //send(cliente,resposta1,500, 0); // endereço socket cliente, char de resposta, tamanho da resposta e 0 nao lembro    

}



int lerRequisicao(char *requisicao, int cliente ){

    char metado[10] = "\0";
    char protocolo[10] = "\0";
    char versao[10] = "\0";
    char endCliente[30];
    
    printf("\n >>>>> Thread do cliente %d DENTRO DO lerRequisicao \n",cliente);

    for(int x = 0; x < 20; x++){
            
        metado[x] = requisicao[x];
       // printf("--> %s e x = %d \n",metado,x);

        if(requisicao[x] == '/'){
            metado[x]='\n';
            x++;

            for(int z=0;z<8;z++){
                x++;

               // printf("antes--> %s e z = %d \n",protocolo,z);
                protocolo[z] = requisicao[x];
               // printf("--> %s e z = %d \n",protocolo,z);

                
                if(requisicao[x] == '/'){
                    protocolo[z]='\n';
                    x++;

                    for(int v = 0; v<8;v++){

                        versao[v] = requisicao[x];
                        x++;
                        if(requisicao[x] == '\n'){
                            versao[v+1] = '\n';
                            x=10000;
                            z=10000;
                            v=10000;
                            

                        }
                    }


                }



            }

        }    


    }

    //strcpy(metado,requisicao);
    metado[2]= '\0';  // apenas para teste no terminal 
    metado[3]= '\0';
    printf("========================\n");
    printf("metado: %s \n",metado);
    printf("protocolo: %s",protocolo);
    printf("versao: %s",versao);
    printf("========================\n");
    printf("========================\n");
    puts(requisicao);
    printf("========================\n");
    printf("========================\n");

    

    int x;

    if (strcmp(metado, "nn") == 0){    
        x=0;
        printf("\n >>>>> CLIENTE SOLICITOU FECHAMENTO DA CONEXÃO 'nn' x=0 para sair do loop de conexao não havera resposta p/ cliente");
    }else {
        printf("\n >>>>> nao identificou comando de sair x = 1");
        x=1;
        memset(&requisicao, 0, sizeof requisicao);
        responderCliente(cliente);
    }

        

    return x;
}


