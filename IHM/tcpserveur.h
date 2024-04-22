#ifndef TCPSERVEUR_H
#define TCPSERVEUR_H

#include <QtNetwork>
#include <QObject>
#include <QDebug>

class TCPServeur: public QObject
{
public:
    TCPServeur(QObject *parent);
    void Send_Message(/*QTcpSocket* socket, */int type, int dType = 0, int data = 0);
private slots:
    void readSocket();
    void discardSocket();

    void newConnection();
    void AddClient(QTcpSocket *socket);
private:
    QTcpServer *TCPServer;
    QList<QTcpSocket*> TCPClients;
};

#endif // TCPSERVEUR_H
