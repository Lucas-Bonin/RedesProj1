//
//  server.h
//  newServer
//
//  Created by Lucas Bonin on 4/7/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#ifndef server_h
#define server_h

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LENGTH_DATA 20

//sinais de controle
enum Signals{
    ACCEPTED = 1,
    STACK_OVERFLOW,
    STORE,
    DELETE,
    READ,
    CLOSE,
    FAILED,
};


//sinais para controle do sensor
enum Operations{
    NONE = 0,
    TURN_ON,
    TURN_OFF,
};


//estrutura para mandar arquivos
typedef struct {
    int amountData;
    int control;
    int operation;
    double temperature;
    double humidity;
}package;


typedef struct{
    int free;
    package pkg;
}dbController;


// estrutura TCP
typedef struct _TcpManager{
    struct sockaddr_in client;
    struct sockaddr_in server;
    int s;                     /* Socket para aceitar conexıes       */
    int ns;                    /* Socket conectado ao cliente        */
    
    void (*sendMessage)(struct _TcpManager *self,  package pkg);
    package (*recvMessage)(struct _TcpManager *self);
    int (*acceptConnection)(struct _TcpManager *self);
    void (*closeClient)(struct _TcpManager *self);
    void (*closeServer)(struct _TcpManager *self);
    
    
}TcpManager;

void storeMessage(TcpManager server, package pkg, dbController dataBase[]);
void readMessage(TcpManager server, dbController dataBase[]);
void closeConnection(TcpManager server);

void initServer(TcpManager *self, unsigned short port);





#endif /* server_h */
