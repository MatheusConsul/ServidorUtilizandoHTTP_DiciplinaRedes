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

#define numMaxClientes 10
int numClientesOnline = -1;
pthread_t thredClientes[numMaxClientes];
pthread_t p12;

int lerRequisicao(char requisicao[], int cliente);
void responderCliente (int cliente, int x);
void *novoCliente(void *  client);



void iniciarServidor (){
  
    struct sockaddr_in caddr;
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(5000)
    };


    int server = socket(AF_INET, SOCK_STREAM, 0);
    int client;
    int csize  = sizeof caddr;

    bind(server, (struct sockaddr *) &saddr, sizeof saddr);
    listen(server, 5);


    while(1){
        puts(" Inicio do while de escuta \n");

        client = accept(server, (struct sockaddr *) &caddr, &csize);
        printf("\n NUMERO DO CLIENT DO ACCEPT: %d",client);
        numClientesOnline++;
        
        pthread_create(&thredClientes[numClientesOnline],NULL, novoCliente,&client);
       // pthread_create(&p12,NULL, novoCliente,&client);
       

            // cria thread para tratar o recebimento de nova conexão 
            // lerRequisição pode ir para dentro da thread 
            // enviar a resposta de dentro da thread 


            
        fflush(stdout);
                 
        
       // memset(&client, 0, sizeof client);  

        puts(" Fim do while  \n");
    }
}



void * novoCliente(void * client ){  // para tratar uma nova conexão 

    int * cliente = ((int*)client);
    
    char buff[1000];

    printf(" Recebeu um cliente %d e vai ler o buffer \n",numClientesOnline); // não sei porque parece que o estado de espera esta sendo na leitura do buffer

    //recv(*cliente, buff, sizeof buff, 0);
   
    //printf(" FEZ a leitura do buffer do cliente numero %d \n",numClientesOnline);
    printf("\n Vai entrar no loop de leitura com o cliente");
    puts(buff);
    int x = 1;
   
    do{

    recv(*cliente, buff, sizeof buff, 0);
    x = lerRequisicao(buff,*cliente);  // fazer a leitura da requisição do navegar


    }while (x == 1);

     
    memset(&buff, 0, sizeof buff);
}




void responderCliente (int cliente, int x){

    char resposta[500] = "HTTP/1.0 200 OK\nDate: Tue, 09 Aug 2011 15:44:04 GMT\nServer: Apache/2.2.3(CentOS)\nLast-Modified: Tue, 09 Aug 2020 15:11:03 GMT\nContent-Length: 6821\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Esta e a resposta do servidor cliente nº: !!!<h1><body></html>";


    send(cliente,resposta,500, 0); // endereço socket cliente, char de resposta, tamanho da resposta e 0 nao lembro.

    puts(" Apos enviar resposta dentro da função de enviar resposta\n \n");

    if (x == 0){
        close(cliente);
        puts("\n Fechando a conexão dentro da função de resposta\n \n");
    }else{
        printf("\n Conexão não foi fechada!!!");
    }
    
    

    
}



int lerRequisicao(char requisicao[], int cliente ){


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
    metado[2]= '\0';  // apenas para teste no terminal 
    metado[3]= '\0';
    printf("========================\n");
    printf("metado: %s",metado);
    printf("protocolo: %s",protocolo);
    printf("versao: %s",versao);
    printf("========================\n");

    

    int x;

    if (strcmp(metado, "nn") == 0){    
        x=0;
        printf(" verdadeiro no if de sair x=0");
    }else {
        printf("nao identificou comando de sair x = 1");
        x=1;
    }

    responderCliente(cliente,x);    

    return x;
}


