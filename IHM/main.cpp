#include "mainwindow.h"
#include "tcpserveur.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    TCPServeur tcp = new TCPServeur(&w);
    w.show();
    return a.exec();
}
