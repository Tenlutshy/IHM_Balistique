#include "tcpserveur.h"
#include "mainwindow.h"
#include "settingsmanager.h"
#include "logger.h"

TCPServeur::TCPServeur(QObject *parent):QObject(parent){

    this->TCPServer = new QTcpServer();

    SettingsManager::loadSettings();
    if(TCPServer->listen(QHostAddress::AnyIPv4, SettingsManager::port.toInt())){
        connect(TCPServer, &QTcpServer::newConnection,this, &TCPServeur::newConnection);
        qDebug() << "Serveur start on port" << SettingsManager::port;
        Logger::writeLog("INFO | Serveur start on port" + SettingsManager::port);
    }
    else {
        Logger::writeLog("ERROR | Serveur Error : " + TCPServer->errorString());
    }

}

void TCPServeur::newConnection()
{
    while(TCPServer->hasPendingConnections()){
        AddClient(TCPServer->nextPendingConnection());
        qobject_cast<MainWindow *>(parent())->ModifInfoLabel("Nouveau canon connecté");
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
        Logger::writeLog("INFO | Fermeture de connexion " + QString::number(socket->socketDescriptor()));
        TCPClients.removeAt(idx);
    }

    socket->deleteLater();
};

void TCPServeur::AddClient(QTcpSocket *socket){
    this->TCPClients.append(socket);
    Logger::writeLog("INFO | Nouvelle connexion " + QString::number(socket->socketDescriptor()));
    connect(socket, &QTcpSocket::readyRead, this, &TCPServeur::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &TCPServeur::discardSocket);
}

void TCPServeur::Send_Message(int type, int dtype, int data)
{
    QTcpSocket *socket = this->TCPClients.value(0);

    if(socket){
        if(socket->isOpen()){

            QByteArray bPayload;
            QByteArray bType;
            bType.append(static_cast<char>(type));
            bType.resize(1);

            QByteArray bDType;
            QByteArray bData;

            QDataStream streamData(&bData, QIODevice::WriteOnly);

            bPayload.append(bType);

            bDType.append(static_cast<char>(dtype));
            bDType.resize(1);
            bPayload.append(bDType);


            streamData << data;
            bPayload.append(bData);


            QByteArray bHeader;
            bHeader.resize(4);
            QDataStream streamHeader(&bHeader, QIODevice::WriteOnly);
            streamHeader << static_cast<quint32>(bPayload.length());

            bPayload.prepend(bHeader);
            Logger::writeLog("INFO | Envoie " + bPayload);
            socket->write(bPayload);
        }else {
            Logger::writeLog("WARNING | Envoie impossible connexion fermé");
        }
    }else {
        Logger::writeLog("WARNING | Envoie impossible aucune connexion");
    }
};
