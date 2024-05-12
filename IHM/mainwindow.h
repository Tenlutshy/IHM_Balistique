#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpserveur.h"
#include "databasemanager.h"
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

    QMap<int,QPushButton*> impactButtons = QMap<int,QPushButton*>();

    DatabaseManager db_manager;

    void receiveImpact(QString t);
    bool eventFilter(QObject *obj, QEvent *event);
    void UpdateImpact();
    void UpdateTarget();
    void ModifInfoLabel(QString msg);
private slots:
    void on_shotBtn_clicked();

    void on_shotPower_valueChanged(int value);

    void on_canonInclinaison_valueChanged(int value);

    void on_canonRotation_valueChanged(int value);

    void on_windPower_valueChanged(int value);

    void on_windDirection_valueChanged(int value);

    void on_cb_impact_stateChanged(int arg1);

    void on_btn_sprimp_clicked();

    void on_projPoids_valueChanged(int value);

    void on_te_cpow_valueChanged(int arg1);

    void on_te_cdir_valueChanged(int arg1);

    void on_te_cinc_valueChanged(int arg1);

    void on_te_projPoids_valueChanged(int arg1);

    void on_te_wpow_valueChanged(int arg1);

    void on_te_wdir_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    int touchHeight;
    int touchWidth;
    int currentImpactSelect;
};
#endif // MAINWINDOW_H
