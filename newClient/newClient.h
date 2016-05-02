//
//  newClient.h
//  
//
//  Created by Lucas Bonin on 4/13/16.
//
//

#ifndef newClient_h
#define newClient_h

#include <stdio.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



//constantes
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

// estrutura TCP
typedef struct _TcpManager{
    struct hostent *hostnm;    //representacao binaria do endereco de IP
    struct sockaddr_in server; // Especificar o endereco do servidor
    int s;
    
    void (*sendMessage)(struct _TcpManager *self,  package pkg);
    package (*recvMessage)(struct _TcpManager *self);
    void (*closeClient)(struct _TcpManager *self);
}TcpManager;


//funcoes do tcp

void initClient(TcpManager *self, const char * hostName, unsigned short port);

// funcoes do menu

int storeMessage(TcpManager tcp, double temp, double humidiy);

void closeMessage(TcpManager tcp);

//funcoes para cliente em C

void reciveMessage(TcpManager tcp);


#endif /* newClient_h */
