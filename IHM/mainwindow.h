#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpserveur.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QtSql>

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

    QLabel* target = nullptr;

    QMap<int,QPushButton*> impactButtons;

    QSqlDatabase db;

    void receiveImpact(QString t);
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void on_shotBtn_clicked();

    void on_shotPower_valueChanged(int value);

    void on_canonInclinaison_valueChanged(int value);

    void on_canonRotation_valueChanged(int value);

    void on_windPower_valueChanged(int value);

    void on_windDirection_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    int touchHeight;
    int touchWidth;
    int InsertEnv(float wDir, float wPow);
    int InsertCanon(float dir, float inc, float pow);
    int InsertImpact(float x, float y, float z);
    int InsertImpactConfiguration(int canon, int env, int impact);
    QList<int> GetImpactConfiguration(int impact_conf_id);
    void UpdateImpact();
};
#endif // MAINWINDOW_H
