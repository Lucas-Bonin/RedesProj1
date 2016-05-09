//
//  server.c
//  newServer
//
//  Created by Lucas Bonin on 4/7/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include "server.h"


//funcoes do tcp
static void sendMessage(TcpManager *self,  package pkg){
    printf("Comecou a Enviar Mensagem Para o Cliente\n");
    
    /*
     Envia a mensagem no buffer de envio para o servidor
     */
    
    if (send(self->ns, (void *) &pkg, sizeof( package), 0) < 0)
    {
        perror("Send()");
        exit(5);
    }
    
}

static  package recvMessage(TcpManager *self){
    /* Recebe a mensagem do servidor no buffer de recepção */
    package pkg;
    
    if (recv(self->ns,(void *) &pkg, sizeof( package), 0) < 0)
    {
        perror("Recv()");
        exit(6);
    }
    
    return pkg;
}

static void closeClient(TcpManager *self){
    close(self->ns);
}

static void closeServer(TcpManager *self){
    close(self->s);
}

static int acceptConnection(TcpManager *self){
    int namelen = sizeof(self->client);
    if ((self->ns = accept(self->s, (struct sockaddr *)&self->client, (socklen_t *)&namelen)) == -1) // **Foi necessario fazer o cast de namelen para socklen_t *
    {
        perror("Accept()");
        exit(5);
    }
    printf("Endereco IP %s da porta %d\n",inet_ntoa(self->client.sin_addr),ntohs(self->client.sin_port));
    return 1;
}

void initServer(TcpManager *self, unsigned short port){
    
    self->sendMessage = &sendMessage;
    self->recvMessage = &recvMessage;
    self->closeClient = &closeClient;
    self->closeServer = &closeServer;
    self->acceptConnection = &acceptConnection;
    
    /*
     * Cria um socket TCP (stream) para aguardar conexıes
     */
    if ((self->s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
        exit(2);
    }
    
    /*
     * Define a qual endereÁo IP e porta o servidor estar· ligado.
     * IP = INADDDR_ANY -> faz com que o servidor se ligue em todos
     * os endereÁos IP
     */
    self->server.sin_family = AF_INET;
    self->server.sin_port   = htons(port);
    self->server.sin_addr.s_addr = INADDR_ANY;
    
    /*
     * Liga o servidor ‡ porta definida anteriormente.
     */
    if (bind(self->s, (struct sockaddr *)&self->server, sizeof(self->server)) < 0)
    {
        perror("Bind()");
        exit(3);
    }
    
    /*
     * Prepara o socket para aguardar por conexıes e
     * cria uma fila de conexıes pendentes.
     */
    if (listen(self->s, 10) != 0)
    {
        perror("Listen()");
        exit(4);
    }
}

//funcoes

int verifyFreeSpace(dbController dataBase[]){
    int i;
    for(i = 0; i < MAX_LENGTH_DATA; i++){
        if(dataBase[i].free == 1){
            return i;
        }
    }
    return -1;
}

int messagesCount(dbController dataBase[]){
    int count = 0;
    int i;
    for (i = 0; i < MAX_LENGTH_DATA; i++) {
        if (dataBase[i].free == 0) {
            count++;
        }
    }
    
    return count;
}

Operations analiseData(package pkg){
    if(pkg.temperature > 19.0){
        printf("Servidor pedindo para ligar ventilador\n");
        return TURN_ON;
    }else if(pkg.temperature < 13.0){
        printf("Servidor pedindo para desligar ventilador\n");
        return TURN_OFF;
    }
    return NONE;
}

void storeMessage(TcpManager server, package pkg, dbController dataBase[]){
    int index = verifyFreeSpace(dataBase);
    package answ;
    printf("Operacao de Cadastrar Mensagem!\n");
    if(index != -1){
        dataBase[index].pkg = pkg;
        dataBase[index].free = 0;
        answ.control = ACCEPTED;
        printf("Operacao Bem Sucedida\n");
    }
    else{
        answ.control = FAILED;
        printf("Operacao Mal Sucedida\n");
    }
    
    //chamar funcao que analiza dados e retorna o comando que será executado da beagle
    
    answ.operation = analiseData(pkg);
    
    server.sendMessage(&server,answ);
    
}


void readMessage(TcpManager server, dbController dataBase[]){
    int count = messagesCount(dataBase);
    int i;
    package answ;
    answ.amountData = count;
    answ.control = ACCEPTED;
    printf("Operacao de Ler Mensagem !\n");
    server.sendMessage(&server,answ);
    
    for (i = 0; i < MAX_LENGTH_DATA; i++) {
        if(dataBase[i].free == 0){
            answ = dataBase[i].pkg;
            server.sendMessage(&server,answ);
        }
    }
    if(count > 0)
        printf("Leitura Bem Sucedida\n");
    else
        printf("Leitura Mal Sucedida\n");
    
}


void closeConnection(TcpManager server){
    package answ;
    answ.control = ACCEPTED;
    printf("Saindo da Aplicacao...\n");
    server.sendMessage(&server,answ);
    
    server.closeClient(&server);
}

//struct para mandar parametros para a thread
typedef struct {
    TcpManager server;
    dbController* dataBase;
    
}threadParam;
