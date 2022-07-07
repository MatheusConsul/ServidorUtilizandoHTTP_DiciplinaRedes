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
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "servidor.h"
#include <pthread.h>

int numMaxClientes = 10;
int numClientesOnline = 0;

void lerRequisicao(char requisicao[], int cliente);
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
    listen(server, 5);
    pthread_t thredClientes[numMaxClientes];
    pthread_t p1;


    while(1){
        puts(" Inicio do while de escuta \n");

        client = accept(server, (struct sockaddr *) &caddr, &csize);

        //pthread_create(&thredClientes[numClientesOnline],NULL, novoCliente,(void*)(&client));
        pthread_create(&p1,NULL, novoCliente,(void*)(&client));
       

            // cria thread para tratar o recebimento de nova conexão 
            // lerRequisição pode ir para dentro da thread 
            // enviar a resposta de dentro da thread 


            
        fflush(stdout);
                 
        
       // memset(&client, 0, sizeof client);  

        puts(" Fim do while  \n");
    }
}



void * novoCliente(void * client ){  // para tratar uma nova conexão 

    int *cliente = (int*)(client);


    numClientesOnline++;
    
    char buff[1000];

    printf(" Recebeu um cliente %d e vai ler o buffer \n",numClientesOnline); // não sei porque parece que o estado de espera esta sendo na leitura do buffer

    recv(*cliente, buff, sizeof buff, 0);
   
    printf(" FEZ a leitura do buffer do cliente numero %d \n",numClientesOnline);
   
    puts(buff);

    lerRequisicao(buff,*cliente);  // fazer a leitura da requisição do navegar 
    memset(&buff, 0, sizeof buff);
}




void responderCliente (int cliente){

    char resposta[500] = "HTTP/1.0 200 OK\nDate: Tue, 09 Aug 2011 15:44:04 GMT\nServer: Apache/2.2.3(CentOS)\nLast-Modified: Tue, 09 Aug 2020 15:11:03 GMT\nContent-Length: 6821\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Esta e a resposta do servidor!<h1><body></html>";


    send(cliente,resposta,500, 0); // endereço socket cliente, char de resposta, tamanho da resposta e 0 nao lembro.

    puts(" Apos enviar resposta dentro da função de enviar resposta\n \n");

    close(cliente);

    puts(" Fechando a conexão dentro da função de resposta\n \n");
}



void lerRequisicao(char requisicao[], int cliente ){


    char metado[10] = "\0";
    char protocolo[10] = "\0";
    char versao[10] = "\0";
    char endCliente[30];
    

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
    printf("========================\n");
    printf("metado: %s",metado);
    printf("protocolo: %s",protocolo);
    printf("versao: %s",versao);
    printf("========================\n");

    responderCliente(cliente);

    //return 0;
}


