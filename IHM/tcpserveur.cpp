#include "tcpserveur.h"
#include "mainwindow.h"


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

    qobject_cast<MainWindow *>(parent())->receiveImpact(socket->readAll());
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

void TCPServeur::Send_Message(/*QTcpSocket *socket, */int type, QString msg)
{
    QTcpSocket *socket = this->TCPClients.value(0);

    if(socket){
        if(socket->isOpen()){

            QByteArray bPayload;
            QByteArray bType;
            bType.append(static_cast<char>(type));
            bType.resize(1);

            bPayload.append(bType);

            switch (type) {
            case 1:
                break;
            default:
                break;
            }

            QByteArray bHeader;
            bHeader.resize(4);
            QDataStream streamHeader(&bHeader, QIODevice::WriteOnly);
            streamHeader << static_cast<quint32>(bPayload.length());

            bPayload.prepend(bHeader);
            qDebug() << bPayload;
            socket->write(bPayload);
        }else {
            qDebug() << "Socket closed";
        }
    }else {
        qDebug() << "No socket";
    }
};
