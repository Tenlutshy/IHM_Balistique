#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpserveur.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>

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

    void receiveImpact(QString t);
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void on_shotBtn_clicked();

private:
    Ui::MainWindow *ui;
    int touchHeight;
    int touchWidth;
};
#endif // MAINWINDOW_H
