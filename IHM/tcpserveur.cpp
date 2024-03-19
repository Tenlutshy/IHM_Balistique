#include "tcpserveur.h"


TCPServeur::TCPServeur(QObject *parent):QObject(parent){

    this->TCPServer = new QTcpServer();

    if(TCPServer->listen(QHostAddress::AnyIPv4, 8080)){
        connect(TCPServer, &QTcpServer::newConnection,this, &TCPServeur::newConnection);
        qDebug() << "Serveur start on port 8080";
    }
    else {
        qDebug() << "Serveur Error" << TCPServer->errorString();
    }

}

void TCPServeur::newConnection()
{
    while(TCPServer->hasPendingConnections()){
        AddClient(TCPServer->nextPendingConnection());
    }
}
void TCPServeur::readSocket(){
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());

    qDebug() << socket->readAll();
    Send_Message(socket, "hello");
};

void TCPServeur::discardSocket(){
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    int idx = TCPClients.indexOf(socket);
    if (idx > -1){
        TCPClients.removeAt(idx);
    }

    socket->deleteLater();
};

void TCPServeur::AddClient(QTcpSocket *socket){
    this->TCPClients.append(socket);
    connect(socket, &QTcpSocket::readyRead, this, &TCPServeur::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &TCPServeur::discardSocket);
    qDebug() << "Client connected :" << QString::number(socket->socketDescriptor());
}

void TCPServeur::Send_Message(QTcpSocket *socket, QString msg)
{
    if(socket){
        if(socket->isOpen()){
            socket->write("hello");
        }else {
            qDebug() << "Socket closed";
        }
    }else {
        qDebug() << "No socket";
    }
};
