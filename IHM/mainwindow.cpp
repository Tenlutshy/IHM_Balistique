#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tcp(this), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->touchableZone->installEventFilter(this);
    this->touchHeight = ui->touchableZone->size().height();
    this->touchWidth = ui->touchableZone->size().width();
    QLabel *canonVisualisation = new QLabel(ui->touchableZone);
    canonVisualisation->setFixedSize(10, 10);
    canonVisualisation->setStyleSheet("background-color: black; border-radius: 5px;");
    canonVisualisation->move(QPoint(touchHeight/2, touchWidth/2) - QPoint(5, 5));
    canonVisualisation->show();

    QDir databasePath;
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(databasePath.currentPath()+"/database.db");

    if(db.open()){
        qDebug() << "DB Connected";
        qDebug() << this->db.tables();
    } else {
        qDebug() << "Error in DB Connection";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::InsertImpact(float x, float y, float z){
    QSqlQuery qrySelectImpact;
    qrySelectImpact.prepare("SELECT COUNT(*) FROM Impact WHERE x=:x AND z=:z");
    qrySelectImpact.bindValue(":x",x);
    qrySelectImpact.bindValue(":y",y);
    qrySelectImpact.bindValue(":z",z);
    qrySelectImpact.exec();

    if(qrySelectImpact.next()){
        if (qrySelectImpact.value(0).toInt() == 0){
            QSqlQuery qryInsertImpact;
            qryInsertImpact.clear();
            qryInsertImpact.prepare("INSERT INTO 'Impact' ('x','y','z') "
                                    "VALUES(:x, :y, :z)");
            qryInsertImpact.bindValue(":x",x);
            qryInsertImpact.bindValue(":y",y);
            qryInsertImpact.bindValue(":z",z);

            if(!qryInsertImpact.exec()){
                qDebug() << qryInsertImpact.lastError().text();
            }
            qDebug()<< "Impact write";
            return qryInsertImpact.lastInsertId().toInt();
        }

    }
    return 0;
}

int MainWindow::InsertEnv(float wDir, float wPow){
    QSqlQuery qrySelectEnv;
    qrySelectEnv.prepare("SELECT COUNT(*) FROM Environnement WHERE wind_direction=:wd AND wind_power=:wp");
    qrySelectEnv.bindValue(":wd",wDir);
    qrySelectEnv.bindValue(":wp",wPow);
    qrySelectEnv.exec();
    if(qrySelectEnv.next()){
        if (qrySelectEnv.value(0).toInt() == 0){
            QSqlQuery qryInsertEnv;
            qryInsertEnv.clear();
            qryInsertEnv.prepare("INSERT INTO Environnement ('wind_direction','wind_power') "
                                 "VALUES(:wd, :wp)");
            qryInsertEnv.bindValue(":wd",wDir);
            qryInsertEnv.bindValue(":wp",wPow);

            if(!qryInsertEnv.exec()){
                qDebug() << qryInsertEnv.lastError().text();
            }
            qDebug()<< "Env write";
            return qryInsertEnv.lastInsertId().toInt();
        }
    }
    return 0;
}

int MainWindow::InsertCanon(float dir, float inc, float pow){
    QSqlQuery qrySelectCanon;
    qrySelectCanon.prepare("SELECT COUNT(*) FROM Canon WHERE direction=:dir AND inclinaison=:inc AND puissance=:pow");
    qrySelectCanon.bindValue(":dir",dir);
    qrySelectCanon.bindValue(":inc",inc);
    qrySelectCanon.bindValue(":pow",pow);
    qrySelectCanon.exec();
    if(qrySelectCanon.next()){
        if (qrySelectCanon.value(0).toInt() == 0){
            QSqlQuery qryInsertCanon;
            qryInsertCanon.prepare("INSERT INTO Canon (direction, inclinaison, puissance) "
                                   "VALUES (:dir, :inc, :pow)");
            qryInsertCanon.bindValue(":dir", dir); // Conversion en entier
            qryInsertCanon.bindValue(":inc", inc);
            qryInsertCanon.bindValue(":pow", pow);
            if(!qryInsertCanon.exec()){
                qDebug() << qryInsertCanon.lastError().text();
            }
            qDebug()<< "Canon write";
            return qryInsertCanon.lastInsertId().toInt();
        }
    }
    return 0;
}

int MainWindow::InsertImpactConfiguration(int canon, int env, int impact){
    QSqlQuery qrySelectImpactConfiguration;
    qrySelectImpactConfiguration.prepare("SELECT COUNT(*) FROM ImpactConfiguration WHERE env_id=:env AND impact_id=:impact AND canon_id=:canon");
    qrySelectImpactConfiguration.bindValue(":canon",canon);
    qrySelectImpactConfiguration.bindValue(":impact",impact);
    qrySelectImpactConfiguration.bindValue(":env",env);
    qrySelectImpactConfiguration.exec();
    if(qrySelectImpactConfiguration.next()){
        if (qrySelectImpactConfiguration.value(0).toInt() == 0){
            QSqlQuery qryInsertImpactConfiguration;
            qryInsertImpactConfiguration.prepare("INSERT INTO ImpactConfiguration (env_id, impact_id, canon_id) "
                                   "VALUES (:env, :impact, :canon)");
            qryInsertImpactConfiguration.bindValue(":canon", canon); // Conversion en entier
            qryInsertImpactConfiguration.bindValue(":impact", impact);
            qryInsertImpactConfiguration.bindValue(":env", env);
            if(!qryInsertImpactConfiguration.exec()){
                qDebug() << qryInsertImpactConfiguration.lastError().text();
            }
            qDebug()<< "config write";
            return qryInsertImpactConfiguration.lastInsertId().toInt();
        }
    }
    return 0;
}




void MainWindow::receiveImpact(QString t)
{
    QStringList coord = t.split(' ');
    float x = coord.value(0).trimmed().replace(',', '.').toFloat();
    float y = coord.value(1).trimmed().replace(',', '.').toFloat();
    float z = coord.value(2).trimmed().replace(',', '.').toFloat();

    QPoint impact = QPoint(x+(this->touchWidth/2),(z-(this->touchHeight/2))*-1);
    qDebug() << impact;


    QLabel *pointLabel = new QLabel(ui->touchableZone);
    pointLabel->setFixedSize(5, 5);
    pointLabel->setStyleSheet("background-color: red; border-radius: 3px;");
    pointLabel->move(impact - QPoint(2, 2));
    pointLabel->show();


    int imp_id = this->InsertImpact(x, y, z);
    int env_id = this->InsertEnv(ui->windDirection->value(), ui->windPower->value());
    int can_id = this->InsertCanon(ui->canonRotation->value(), ui->canonInclinaison->value(), ui->shotPower->value());
    int conf_id = this->InsertImpactConfiguration(can_id,env_id,imp_id);

    this->impactLabels.insert(conf_id, *pointLabel);
}

void MainWindow::on_shotBtn_clicked()
{
    tcp.Send_Message(1);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->touchableZone && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            qDebug() << "Coordonnées du clic : " << mouseEvent->pos().x()-(this->touchWidth/2) << ", " << (mouseEvent->pos().y()-(this->touchHeight/2))*-1;
            if (this->target != nullptr) {
                // Supprimer l'ancien QLabel
                this->target->deleteLater();
            }
            this->target = new QLabel(ui->touchableZone);
            this->target->setFixedSize(5, 5);
            this->target->setStyleSheet("background-color: lightgreen; border-radius: 3px;");
            this->target->move(mouseEvent->pos() - QPoint(2, 2));
            this->target->show();

        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_shotPower_valueChanged(int value)
{
    this->tcp.Send_Message(4,3,value);
}


void MainWindow::on_canonInclinaison_valueChanged(int value)
{
    this->tcp.Send_Message(4,2,value);
}


void MainWindow::on_canonRotation_valueChanged(int value)
{
    this->tcp.Send_Message(4,1,value);
}


void MainWindow::on_windPower_valueChanged(int value)
{
    this->tcp.Send_Message(2,1,value);
}


void MainWindow::on_windDirection_valueChanged(int value)
{
    this->tcp.Send_Message(2,2,value);
}

