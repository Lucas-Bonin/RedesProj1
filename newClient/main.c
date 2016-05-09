//
//  main.c
//  newClient
//
//  Created by Lucas Bonin on 4/13/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include <stdio.h>
#include "newClient.h"

int main(int argc, const char * argv[]) {
    
    unsigned short port;
    
    /*
     * O primeiro argumento (argv[1]) é o hostname do servidor.
     * O segundo argumento (argv[2]) é a porta do servidor.
     */
    
    argc = 3;
    argv[1] = "localhost";
    argv[2] = "5000";
    
    if (argc != 3)
    {
        fprintf(stderr, "Use: %s hostname porta\n", argv[0]);
        exit(1);
    }
    
    // Transforma o valor da porta de string para int
    //  e depois faz um cast para unsigned short
    port = (unsigned short) atoi(argv[2]);
    
    
    TcpManager client;
    initClient(&client, argv[1], port);
    
    int answ,ret;
    
    do{
        printf ("1 - cadastrar mensagem\n2 - ler mensagens cadastradas\n4 - sair da aplicacao\n" );
        scanf("%d", &answ);
        getchar();
        switch (answ){
            case 1:
                ret = storeMessage(client,9,0.456);
                
                switch(ret){
                    case TURN_ON:
                        printf("Comando de TURN_ON\n");
                        break;
                        
                    case TURN_OFF:
                        printf("Comando de TURN_OFF\n");
                        break;
                        
                    case NONE:
                        printf("Comando de NONE\n");
                        break;
                        
                    default:
                        printf("Retorno de storeMessage() invalido\n");
                }
                break;
                case 2:
                reciveMessage(client);
                break;
            case 4:
                closeMessage(client);
                break;
            default:
                printf("Opcao invalida\n");
        }
    }while(answ != 4);
    
    
    return 0;
}

