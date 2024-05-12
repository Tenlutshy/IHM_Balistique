#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "prevision.h"
#include "logger.h"
#include <QVector3D>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tcp(this), db_manager(this), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Logger::writeLog("INFO | Interface lancée");

    ui->btn_sprimp->setVisible(false);

    ui->touchableZone->installEventFilter(this);
    this->touchHeight = ui->touchableZone->size().height();
    this->touchWidth = ui->touchableZone->size().width();
    QLabel *canonVisualisation = new QLabel(ui->touchableZone);
    canonVisualisation->setObjectName("canonVisualisation");
    canonVisualisation->setFixedSize(10, 10);
    canonVisualisation->setStyleSheet("background-color: black; border-radius: 5px;");
    canonVisualisation->move(QPoint(touchHeight/2, touchWidth/2) - QPoint(5, 5));
    canonVisualisation->show();

    this->db_manager.init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateTarget(){

    this->ui->btn_sprimp->setEnabled(false);
    this->ui->btn_sprimp->setVisible(false);

    float rotation = this->ui->canonRotation->value();
    float inclinaison = this->ui->canonInclinaison->value();
    float power = this->ui->shotPower->value();
    float wind_direction = this->ui->windDirection->value();
    float wind_power = this->ui->windPower->value();
    float projectile_mass = this->ui->projPoids->value();


    QVector3D fCanon = Prevision::calculateCannonForce(inclinaison,rotation,power);
    QVector3D fVent = Prevision::calculateWindForce(wind_direction,wind_power);

    QVector3D previsionImpact = Prevision::calculateImpactCoordinates(fCanon,fVent,projectile_mass);
    qDebug() << previsionImpact;

    float z = previsionImpact.x();
    float x = previsionImpact.z();

    this->ui->lb_info->setText("Cible : " + QString::number(x) + "  |  " + QString::number(z));

    QPoint pImpactCoord = QPoint(x+(this->touchWidth/2),(z-(this->touchHeight/2))*-1);
    qDebug()<<pImpactCoord;

    if (this->target != nullptr) {
        this->target->deleteLater();
    }
    this->target = new QLabel(ui->touchableZone);
    this->target->setObjectName("target");
    this->target->setFixedSize(5, 5);
    this->target->setStyleSheet("background-color: lightgreen; border-radius: 3px;");
    this->target->move(pImpactCoord - QPoint(2, 2));
    this->target->show();

    Logger::writeLog("INFO | Position de la cible modifié " + QString::number(x) + "  ~  " + QString::number(z));
}

void MainWindow::UpdateImpact(){

    foreach (QPushButton *btn, this->impactButtons.values()) {
        btn->deleteLater();
    }
    this->impactButtons.clear();

    if(!this->ui->cb_impact->isChecked()) return;

    int w_pow = this->ui->windPower->value();
    int w_direction = this->ui->windDirection->value();

    int b_poids = this->ui->projPoids->value();

    QSqlQuery qrySelectEnv;
    qrySelectEnv.prepare("SELECT id FROM Environnement WHERE wind_direction=:wd AND wind_power=:wp");
    qrySelectEnv.bindValue(":wd",w_direction);
    qrySelectEnv.bindValue(":wp",w_pow);
    qrySelectEnv.exec();

    QSqlQuery qrySelectBullet;
    qrySelectBullet.prepare("SELECT id FROM Bullet WHERE poids=:pd");
    qrySelectBullet.bindValue(":pd",b_poids);

    if(qrySelectEnv.next()){
        int envId = qrySelectEnv.value(0).toInt();
        qrySelectBullet.exec();

        if(qrySelectBullet.next()){
            int bulletId = qrySelectBullet.value(0).toInt();
            QSqlQuery qrySelectImpactConfig;
            qrySelectImpactConfig.prepare("SELECT id, impact_id FROM ImpactConfiguration WHERE env_id=:ei AND bullet_id=:bi");
            qrySelectImpactConfig.bindValue(":ei", envId);
            qrySelectImpactConfig.bindValue(":bi", bulletId);
            if(!qrySelectImpactConfig.exec()){
                qDebug() << qrySelectImpactConfig.lastError().text();
            }

            while(qrySelectImpactConfig.next()){

                int conf_id = qrySelectImpactConfig.value(0).toInt();
                int impact_id = qrySelectImpactConfig.value(1).toInt();

                QSqlQuery qrySelectImpact;
                qrySelectImpact.prepare("SELECT x,z FROM Impact WHERE id=:ii");
                qrySelectImpact.bindValue(":ii",impact_id);
                qrySelectImpact.exec();

                if (qrySelectImpact.next()){

                    int x = qrySelectImpact.value(0).toInt();
                    int z = qrySelectImpact.value(1).toInt();

                    QPoint impact = QPoint(x+(this->touchWidth/2),(z-(this->touchHeight/2))*-1);
                    this->impactButtons.insert(conf_id, new QPushButton(ui->touchableZone));

                    QPushButton *button = this->impactButtons.value(conf_id);
                    button->setFixedSize(5, 5);
                    button->setStyleSheet("background-color: red; border-radius: 3px;");
                    button->move(impact - QPoint(2, 2));
                    button->show();
                    button->setStyleSheet("QPushButton { background-color: red; border-radius: 3px; }"
                                          "QPushButton:hover { background-color: blue; }");

                    connect(button, &QPushButton::clicked, [=]() {
                        QList<int> configuration = db_manager.GetImpactConfiguration(conf_id);
                        this->ui->canonInclinaison->setValue(configuration.value(1));
                        this->ui->canonRotation->setValue(configuration.value(0));
                        this->ui->shotPower->setValue(configuration.value(2));

                        this->ui->windPower->setValue(configuration.value(4));
                        this->ui->windDirection->setValue(configuration.value(3));

                        this->ui->projPoids->setValue(configuration.value(5));

                        this->currentImpactSelect = conf_id;


                        this->ui->btn_sprimp->setEnabled(true);
                        this->ui->btn_sprimp->setVisible(true);

                        Logger::writeLog("INFO | Impact séléctionné " + QString::number(conf_id));

                    });
                }
            }
        }
    }
}


void MainWindow::receiveImpact(QString t)
{
    QStringList coord = t.split(' ');
    float x = coord.value(0).trimmed().replace(',', '.').toFloat();
    float y = coord.value(1).trimmed().replace(',', '.').toFloat();
    float z = coord.value(2).trimmed().replace(',', '.').toFloat();

    int imp_id = db_manager.InsertImpact(x, y, z);
    int env_id = db_manager.InsertEnv(ui->windDirection->value(), ui->windPower->value());
    int can_id = db_manager.InsertCanon(ui->canonRotation->value(), ui->canonInclinaison->value(), ui->shotPower->value());
    int bul_id = db_manager.InsertBullet(ui->projPoids->value());
    db_manager.InsertImpactConfiguration(can_id,env_id,imp_id, bul_id);


    Logger::writeLog("INFO | Impact reçu " + QString::number(x) + "  ~  " + QString::number(z));
    this->UpdateImpact();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    if (obj == ui->touchableZone && event->type() == QEvent::MouseButtonPress && this->ui->cb_tts->isChecked()) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            qDebug() << "Coordonnées du clic : " << mouseEvent->pos().x()-(this->touchWidth/2) << ", " << (mouseEvent->pos().y()-(this->touchHeight/2))*-1;

            Logger::writeLog("INFO | Clic utilisateur " + QString::number(mouseEvent->pos().x()-(this->touchWidth/2)) + ", " + QString::number((mouseEvent->pos().y()-(this->touchHeight/2))*-1));

            float wind_direction = this->ui->windDirection->value();
            float wind_power = this->ui->windPower->value();
            float projectile_mass = this->ui->projPoids->value();
            float x = mouseEvent->pos().x()-(this->touchWidth/2);
            float z = (mouseEvent->pos().y()-(this->touchHeight/2))*-1;

            QVector3D fVent = Prevision::calculateWindForce(wind_direction,wind_power);

            QList<int> newCanonConf= Prevision::adjusteCanon(x,z, fVent, projectile_mass);

            if (newCanonConf.value(0) == 1000){

                Logger::writeLog("WARNING | Aucune configuration trouvé :\nCible hors de portée");
                this->ui->lb_info->setText("Aucune configuration trouvé : Cible hors de portée");
            }else {
                this->ui->canonRotation->setValue(newCanonConf.value(0));
                this->ui->canonInclinaison->setValue(newCanonConf.value(1));
                this->ui->shotPower->setValue(newCanonConf.value(2));
            }
        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::ModifInfoLabel(QString msg){
    this->ui->lb_info->setText(msg);
}





// -----------------   SLOTS

void MainWindow::on_shotBtn_clicked()
{
    tcp.Send_Message(1);
}


void MainWindow::on_shotPower_valueChanged(int value)
{
    this->tcp.Send_Message(4,3,value);
    this->ui->te_cpow->setValue(value);
    this->UpdateTarget();
}


void MainWindow::on_canonInclinaison_valueChanged(int value)
{
    this->tcp.Send_Message(4,2,value);
    this->ui->te_cinc->setValue(value);
    this->UpdateTarget();
}


void MainWindow::on_canonRotation_valueChanged(int value)
{
    this->tcp.Send_Message(4,1,value);
    this->ui->te_cdir->setValue(value);
    this->UpdateTarget();
}


void MainWindow::on_windPower_valueChanged(int value)
{
    this->tcp.Send_Message(2,1,value);
    this->UpdateImpact();
    this->ui->te_wpow->setValue(value);
    this->UpdateTarget();
}


void MainWindow::on_windDirection_valueChanged(int value)
{
    this->tcp.Send_Message(2,2,value);
    this->UpdateImpact();
    this->ui->te_wdir->setValue(value);
    this->UpdateTarget();
}


void MainWindow::on_cb_impact_stateChanged(int arg1)
{
    this->UpdateImpact();
}


void MainWindow::on_btn_sprimp_clicked()
{
    this->db_manager.RemoveImpactConfig(this->currentImpactSelect);
    UpdateTarget();
    UpdateImpact();
}


void MainWindow::on_projPoids_valueChanged(int value)
{
    this->tcp.Send_Message(3,1,value);
    this->ui->te_projPoids->setValue(value);
    this->UpdateTarget();
    this->UpdateImpact();
}


void MainWindow::on_te_cpow_valueChanged(int arg1)
{
    this->ui->shotPower->setValue(arg1);
}


void MainWindow::on_te_cdir_valueChanged(int arg1)
{
    this->ui->canonRotation->setValue(arg1);
}


void MainWindow::on_te_cinc_valueChanged(int arg1)
{
    this->ui->canonInclinaison->setValue(arg1);
}


void MainWindow::on_te_projPoids_valueChanged(int arg1)
{
    this->ui->projPoids->setValue(arg1);
}


void MainWindow::on_te_wpow_valueChanged(int arg1)
{
    this->ui->windPower->setValue(arg1);
}


void MainWindow::on_te_wdir_valueChanged(int arg1)
{
    this->ui->windDirection->setValue(arg1);
}

