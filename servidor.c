/*
*   Matheus Barbosa Consul
*   https://github.com/MatheusConsul/EC-Servidor-HTTP 
*
*   Codigo consegue receber uma requisição do cliente identificar o inicio do cabeçalho e enviar uma resposta.
*   Como cliente foi usado o terminal e o navegador acessando o http://localhost:5000/
*
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


int lerRequisicao(char requisicao[], int cliente);
void responderCliente (int cliente);
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
    listen(server, 5); //especifica o n´umero de conex˜oes que podem estar em espera num socket de um servidor (por exemplo, chamadas telef´onicas em espera)


    while(1){
        puts("\n >>>>> Inicio do while de escuta da porta \n");

        client = accept(server, (struct sockaddr *) &caddr, &csize);

        numClientesOnline++;
        
        puts("\n >>>>> Criando uma thread para o cliente \n");

        pthread_create(&thredClientes[numClientesOnline],NULL, novoCliente,&client);
       
        fflush(stdout);
                 
        // PROBLEMA AO FECHAR O NAVEGAR SEM FECHAR A CONEXÃO ENCERRA A EXECUÇÃO DO SERVIDOR 
        
        puts("\n >>>>> Fim do while e zerando o conteudo de client \n");
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

    char resposta[500] = "HTTP/1.0 200 OK\nDate: Tue, 09 Aug 2011 15:44:04 GMT\nServer: Apache/2.2.3(CentOS)\nLast-Modified: Tue, 09 Aug 2020 15:11:03 GMT\nContent-Length: 6821\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Esta e a resposta do servidor cliente nº: !!!<h1><body></html>";

    send(cliente,resposta,500, 0); // endereço socket cliente, char de resposta, tamanho da resposta e 0 nao lembro    

}



int lerRequisicao(char requisicao[], int cliente ){

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
    printf("\n metado: %s \n",metado);
    printf("protocolo: %s",protocolo);
    printf("versao: %s",versao);
    printf("========================\n");

    

    int x;

    if (strcmp(metado, "nn") == 0){    
        x=0;
        printf("\n >>>>> CLIENTE SOLICITOU FECHAMENTO DA CONEXÃO 'nn' x=0 para sair do loop de conexao não havera resposta p/ cliente");
    }else {
        printf("\n >>>>> nao identificou comando de sair x = 1");
        x=1;
        responderCliente(cliente);
    }

        

    return x;
}


