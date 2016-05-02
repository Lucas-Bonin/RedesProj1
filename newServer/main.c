//
//  main.c
//  newServer
//
//  Created by Lucas Bonin on 4/7/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include <stdio.h>

// Biblioteca para conexao com o servidor
#include "server.h"

// Bibliotecas para thread
#include <pthread.h>


//struct para mandar parametros para a thread
typedef struct {
    TcpManager server;
    dbController* dataBase;
    
}threadParam;

pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER; //cria e inicializa mutex

//funcao para threads executarem
void* clientRequestManager(void* param){
    
    printf("Foi Criada uma Thread \n");
    
    threadParam* tP = (threadParam*) param;
    
    TcpManager server = tP->server;
    
    package pkg;
    do {
        pkg = server.recvMessage(&server);
        
        switch (pkg.control){
            case STORE:
                pthread_mutex_lock(&locker);
                storeMessage(server, pkg, tP->dataBase);
                pthread_mutex_unlock(&locker);
                break;
            case READ:
                pthread_mutex_lock(&locker);
                readMessage(server, tP->dataBase);
                pthread_mutex_unlock(&locker);
                break;
            case CLOSE:
                closeConnection(server);
                break;
        }
    }while (pkg.control != CLOSE);
    return NULL;
}


int main(int argc, const char * argv[]) {
    
    
    unsigned short port;
    dbController dataBase[MAX_LENGTH_DATA];
    int i;
    
    for (i = 0; i < MAX_LENGTH_DATA; ++i)
    {
        dataBase[i].free = 1;
    }
    
    /*
     * O primeiro argumento (argv[1]) È a porta
     * onde o servidor aguardar· por conexıes
     */
    
    argc = 2;
    argv[1] = "5000";
    
    if (argc != 2)
    {
        fprintf(stderr, "Use: %s porta\n", argv[0]);
        exit(1);
    }
    
    port = (unsigned short) atoi(argv[1]);
    
    
    TcpManager server;
    initServer(&server,port);
    
    //criando variavel para thread ID
    pthread_t threadID;
    
    //parametros que serao passados para a thread
    threadParam param;
    
    while(1){
        server.acceptConnection(&server);
        
        param.server = server;
        param.dataBase = dataBase;
        
        pthread_create(&threadID, NULL, &clientRequestManager,&param);
    }
    
    //printf("Servidor terminou com sucesso.\n");
    //exit(0);
    

}
