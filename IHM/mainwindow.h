#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpserveur.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    TCPServeur tcp;

private slots:
    void on_shotBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
