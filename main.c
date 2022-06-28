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


void lerRequisicao(char requisicao[] ){


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
                            versao[v+1] = '\n\n';
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

    //return 0;
}






int main(){
    char buff[1000];

    char resposta[500] = "HTTP/1.1 301 Not found\r\nConnection: close\r\nDate: Tue, 09 Aug 2011 15:44:04 GMT\r\nServer: Apache/2.2.3(CentOS)\r\nLast-Modified: Tue, 09 Aug 2020 15:11:03 GMT\r\nContent-Length: 6821\r\nContent-Type: text/html\r\n";



    struct sockaddr_in caddr;
    struct sockaddr_in saddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(5000)
    };

    int server = socket(AF_INET, SOCK_STREAM, 0);
    int client, x;
    int csize  = sizeof caddr;

    bind(server, (struct sockaddr *) &saddr, sizeof saddr);
    listen(server, 5);
    

    while(1){


        puts(" Inicio do while de escuta \n");

        client = accept(server, (struct sockaddr *) &caddr, &csize);
        
        puts(" Recebeu um cliente e vai ler o buffer \n");

        x = recv(client, buff, sizeof buff, 0);

       // printf(" Já leu o buffer\n");

        lerRequisicao(buff);  // fazer a leitura da requisição do navegar 
            
            
            if (strcmp(buff, "ssss") == 0){
                send(client, "SIMMM\n",6, 0);
            }

            if (strcmp(buff, "nnnn") == 0){    
                send(client, "NAOOO\n",6, 0);
            }

            //send(client, buff,x, 0);

            send(client,resposta,500, 0);

            puts(" Apos os ifs \n \n");

            puts(buff);
            fflush(stdout);

            if (strcmp(buff, "sair") != 0){
                close(client);
                puts(" fechou a conexão \n");
            }
        puts(" FIm do while  \n");
    }

    return 0;
}

