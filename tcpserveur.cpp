#include "tcpserveur.h"


TCPServeur::TCPServeur(QObject *parent):QObject(parent){

    this->TCPServer = new QTcpServer();

    connect(TCPServer, SIGNAL(newConnection()),this,SLOT(newConnection()));

    if (this->TCPServer->isListening()){

    }
}



void TCPServeur::newConnection()
{

}
void readSocket(){};
    void discardSocket(){};

void AddClient(QTcpSocket *socket){};
