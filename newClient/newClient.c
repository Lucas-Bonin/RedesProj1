//
//  newClient.c
//  
//
//  Created by Lucas Bonin on 4/13/16.
//
//

#include "newClient.h"


//funcoes do tcp
void sendMessage(TcpManager *self,  package pkg){
    printf("Comecou a Enviar Mensagem Para o Servidor\n");
    
    /*
     Envia a mensagem no buffer de envio para o servidor
     */
    
    if (send(self->s, (void *) &pkg, sizeof( package), 0) < 0)
    {
        perror("Send()");
        exit(5);
    }
    
}

package recvMessage(TcpManager *self){
    /* Recebe a mensagem do servidor no buffer de recepção */
    package pkg;
    
    if (recv(self->s,(void *) &pkg, sizeof( package), 0) < 0)
    {
        perror("Recv()");
        exit(6);
    }
    
    return pkg;
}

void closeClient(TcpManager *self){
    close(self->s);
}

void initClient(TcpManager *self, const char * hostName, unsigned short port){
    self->sendMessage = &sendMessage;
    self->recvMessage = &recvMessage;
    self->closeClient = &closeClient;
    
    /*
     * Obtendo o endereço IP do servidor
     * procura o endereco a partir do nome. Ex:
     * "merlin.cs.purdue.edu" ou "128.10.2.3"
     */
    self->hostnm = gethostbyname(hostName);
    if (self->hostnm == (struct hostent *) 0) // Compara valor recebido com o numero 0
    {
        fprintf(stderr, "Gethostbyname failed\n");
        exit(2);
    }
    
    /*
     * Define o endereço IP e a porta do servidor
     */
    self->server.sin_family      = AF_INET; //familia de protocolo
    self->server.sin_port        = htons(port);
    self->server.sin_addr.s_addr = *((unsigned int *)self->hostnm->h_addr);
    
    /*
     * Cria um socket TCP (stream)
     Familia de protocolos, servico, identificar o protocolo desejado
     */
    if ((self->s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket()");
        exit(3);
    }
    
    /*
     Estabelece conexão com o servidor
     3-ways handshake
     descritor socket, endereco struct do endereco remoto, compromento segundo
     argumento
     */
    if (connect(self->s, (struct sockaddr *)&self->server, sizeof(self->server)) < 0)
    {
        perror("Connect()");
        exit(4);
    }
}



//envia a temperatura e humidade para o servidor e espera a acao a tomar
int storeMessage(TcpManager tcp, double temp, double humidiy){
    package data,answ;
    data.control = STORE;
    data.temperature = temp;
    data.humidity = humidiy;
    
    tcp.sendMessage(&tcp,data);
    
    answ = tcp.recvMessage(&tcp);
    
    if(answ.control != ACCEPTED){
        printf("Servidor lotado!!!\n");
    }
    
    return answ.operation;
}



void closeMessage(TcpManager tcp){
    package controller;
    controller.control = CLOSE;
    tcp.sendMessage(&tcp,controller);
    controller = tcp.recvMessage(&tcp);
    if (controller.control == ACCEPTED) {
        tcp.closeClient(&tcp);
    }
    else{
        printf("Erro ao fechar conexao: %d\n",controller.control);
    }
}

// funcoes para cliente em C

void showMessages(package messages[], int amount){
    int i;
    
    for (i = 0; i < amount; ++i){
        printf("TEMPERATURA: %lf HUMIDADE: %lf\n\n", messages[i].temperature, messages[i].humidity);
        // printf("mensagem: %s \n", messages[i].message);
    }
}

void reciveMessage(TcpManager tcp){
    int i;
    package messages[MAX_LENGTH_DATA],controller;
    controller.control = READ;
    tcp.sendMessage(&tcp,controller);
    
    controller = tcp.recvMessage(&tcp);
    
    if(controller.control == ACCEPTED){
        for (i = 0; i< controller.amountData; i++) {
            messages[i] = tcp.recvMessage(&tcp);
        }
    }
    if(controller.amountData > 0){
        printf("\nNumero de mensagens cadastradas = %d\n\n", controller.amountData );
        showMessages(messages,controller.amountData);
    }
    else
        printf("Nenhuma mensagem cadastrada\n");
}


