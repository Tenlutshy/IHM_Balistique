#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector3D>
#include <cmath>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tcp(this), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->touchableZone->installEventFilter(this);
    this->touchHeight = ui->touchableZone->size().height();
    this->touchWidth = ui->touchableZone->size().width();
    QLabel *canonVisualisation = new QLabel(ui->touchableZone);
    canonVisualisation->setObjectName("canonVisualisation");
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
        this->UpdateImpact();
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
    qrySelectImpact.prepare("SELECT COUNT(*),id FROM Impact WHERE x=:x AND z=:z");
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
            return qryInsertImpact.lastInsertId().toInt();
        }else {
            return qrySelectImpact.value(1).toInt();
        }
    }
    return 0;
}

int MainWindow::InsertEnv(float wDir, float wPow){
    QSqlQuery qrySelectEnv;
    qrySelectEnv.prepare("SELECT COUNT(*),id FROM Environnement WHERE wind_direction=:wd AND wind_power=:wp");
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
            return qryInsertEnv.lastInsertId().toInt();
        }else {
            return qrySelectEnv.value(1).toInt();
        }
    }
    return 0;
}

int MainWindow::InsertCanon(float dir, float inc, float pow){
    QSqlQuery qrySelectCanon;
    qrySelectCanon.prepare("SELECT COUNT(*),id FROM Canon WHERE direction=:dir AND inclinaison=:inc AND puissance=:pow");
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
            return qryInsertCanon.lastInsertId().toInt();
        }else {
            return qrySelectCanon.value(1).toInt();
        }
    }
    return 0;
}

int MainWindow::InsertImpactConfiguration(int canon, int env, int impact){
    QSqlQuery qrySelectImpactConfiguration;
    qrySelectImpactConfiguration.prepare("SELECT COUNT(*),id FROM ImpactConfiguration WHERE env_id=:env AND impact_id=:impact AND canon_id=:canon");
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
            return qryInsertImpactConfiguration.lastInsertId().toInt();
        }else {
            return qrySelectImpactConfiguration.value(1).toInt();
        }
    }
    return 0;
}

QList<int> MainWindow::GetImpactConfiguration(int impact_conf_id){

    int c_dir = 0;
    int c_inc = 0;
    int c_pow = 0;

    int w_dir = 0;
    int w_pow = 0;

    QSqlQuery qrySelectImpactConfiguration;
    qrySelectImpactConfiguration.prepare("SELECT env_id, canon_id FROM ImpactConfiguration WHERE id=:ici");
    qrySelectImpactConfiguration.bindValue(":ici",impact_conf_id);
    qrySelectImpactConfiguration.exec();

    if(qrySelectImpactConfiguration.next()){
        QSqlQuery qrySelectCanon;
        qrySelectCanon.prepare("SELECT direction, inclinaison, puissance FROM Canon WHERE id=:ci");
        qrySelectCanon.bindValue(":ci",qrySelectImpactConfiguration.value(1).toInt());
        qrySelectCanon.exec();
        if(qrySelectCanon.next()){
            c_dir = qrySelectCanon.value(0).toInt();
            c_inc = qrySelectCanon.value(1).toInt();
            c_pow = qrySelectCanon.value(2).toInt();
        }


        QSqlQuery qrySelectEnvironnement;
        qrySelectEnvironnement.prepare("SELECT wind_power, wind_direction FROM Environnement WHERE id=:ei");
        qrySelectEnvironnement.bindValue(":ei",qrySelectImpactConfiguration.value(0).toInt());
        qrySelectEnvironnement.exec();
        if(qrySelectEnvironnement.next()){
            w_pow = qrySelectEnvironnement.value(0).toInt();
            w_dir = qrySelectEnvironnement.value(1).toInt();
        }

    }

    return QList{c_dir,c_inc,c_pow,w_dir,w_pow};
}






float degreesToRadians(float degrees) {
    return degrees * M_PI / 180.0f;
}

QVector3D calculateWindForce(float windAngle, float windForce) {
    // Convertir l'angle du vent en radians
    float radians = degreesToRadians(windAngle);

    // Calculer les composantes x et z du vecteur de force du vent en fonction de l'angle
    float windForceX = windForce * std::cos(radians);
    float windForceZ = windForce * std::sin(radians);

    return QVector3D(windForceX, 0.0f, windForceZ);
}

QVector3D calculateCannonForce(float cannonAngle, float directionAngle, float cannonPower) {
    // Convertir les angles en radians
    float radiansCannon = degreesToRadians(cannonAngle);
    float radiansDirection = degreesToRadians(directionAngle);

    // Calculer les composantes x, y et z du vecteur de force du canon
    float cannonForceX = cannonPower * std::cos(radiansCannon) * std::cos(radiansDirection);
    float cannonForceY = cannonPower * std::sin(radiansCannon);
    float cannonForceZ = cannonPower * std::cos(radiansCannon) * std::sin(radiansDirection);

    return QVector3D(cannonForceX, cannonForceY, cannonForceZ);
}

QVector3D calculateImpactCoordinates(const QVector3D& cannonForce, const QVector3D& windForce, float projectileMass)
{
    const float gravity = 9.81f;

    QVector3D initialVelocity = cannonForce / projectileMass;

    float timeOfFlight = (2.0f * initialVelocity.y()) / gravity;   // Prise en compte de la taille a realisé

    QVector3D adjustedWindForce = windForce / gravity;

    QVector3D position = (initialVelocity * timeOfFlight) + adjustedWindForce;

    position.setY(0.0f);

    return position;
}






QList<int> adjusteCanon(float x, float z, QVector3D& windForce, float projectileMass){
    qDebug() << x << " -- " << z;
    int max_inclinaison = 90;
    int max_direction = 359;
    int max_puissance = 30;

    // Boucle direction
    for (int d=0; d <= max_direction; d++){
        int direction = d;

        // Boucle inclinaison
        for (int i=0; i <= max_inclinaison; i++){
            int inclinaison = i <= 45 ? i+45 : 45-i+45;

            // Boucle puissance
            for (int p=0; p<= max_puissance; p++){
                int puissance = p;

                QVector3D previsionImpact = calculateImpactCoordinates(calculateCannonForce(inclinaison, direction, puissance),windForce,projectileMass);

                double epsilon = 1; // Définissez une petite valeur pour la tolérance

                if (fabs(previsionImpact.x() - z) < epsilon && fabs(previsionImpact.z() - x) < epsilon){
                    qDebug() << direction << " | " << inclinaison << " | " << puissance << " | " << previsionImpact;
                    return QList{direction,inclinaison, puissance};
                }

            }


        }

    }

    qDebug() << "Pas de solutions trouvé";
    return QList{0,0,0};

}








void MainWindow::UpdateTarget(){

    float rotation = this->ui->canonRotation->value(); // Example rotation angle in degrees
    float inclinaison = this->ui->canonInclinaison->value(); // Example inclination angle in degrees
    float power = this->ui->shotPower->value(); // Example power of the shot
    float wind_direction = this->ui->windDirection->value(); // Example wind direction in degrees
    float wind_power = this->ui->windPower->value(); // Example wind power
    float projectile_mass = 1.0f; // Example projectile mass in kg



    QVector3D fCanon = calculateCannonForce(inclinaison,rotation,power);
    QVector3D fVent = calculateWindForce(wind_direction,wind_power);

    QVector3D previsionImpact = calculateImpactCoordinates(fCanon,fVent,projectile_mass);
    qDebug() << previsionImpact;

    float z = previsionImpact.x();
    float x = previsionImpact.z();
    qDebug()<<x<<" | "<<z;

    QPoint pImpactCoord = QPoint(x+(this->touchWidth/2),(z-(this->touchHeight/2))*-1);
    qDebug()<<pImpactCoord;

    if (this->target != nullptr) {
        // Supprimer l'ancien QLabel
        this->target->deleteLater();
    }
    this->target = new QLabel(ui->touchableZone);
    this->target->setObjectName("target");
    this->target->setFixedSize(5, 5);
    this->target->setStyleSheet("background-color: lightgreen; border-radius: 3px;");
    this->target->move(pImpactCoord - QPoint(2, 2));
    this->target->show();
}

void MainWindow::UpdateImpact(){
    // Get environnement configuration
    int w_pow = this->ui->windPower->value();
    int w_direction = this->ui->windDirection->value();


    foreach (QPushButton *btn, this->impactButtons.values()) {
        btn->deleteLater();
    }

    this->impactButtons.clear();

    QSqlQuery qrySelectEnv;
    qrySelectEnv.prepare("SELECT id FROM Environnement WHERE wind_direction=:wd AND wind_power=:wp");
    qrySelectEnv.bindValue(":wd",w_direction);
    qrySelectEnv.bindValue(":wp",w_pow);
    qrySelectEnv.exec();

    if(qrySelectEnv.next()){
        QSqlQuery qrySelectImpactConfig;
        qrySelectImpactConfig.prepare("SELECT id, impact_id FROM ImpactConfiguration WHERE env_id=:ei");
        qrySelectImpactConfig.bindValue(":ei", qrySelectEnv.value(0).toInt());
        qrySelectImpactConfig.exec();

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
                    QList<int> configuration = this->GetImpactConfiguration(conf_id);
                    this->ui->canonInclinaison->setValue(configuration.value(1));
                    this->ui->canonRotation->setValue(configuration.value(0));
                    this->ui->shotPower->setValue(configuration.value(2));

                    this->ui->windPower->setValue(configuration.value(4));
                    this->ui->windDirection->setValue(configuration.value(3));
                });

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
    qDebug() << "("<<x<<","<<z<<")";

    int imp_id = this->InsertImpact(x, y, z);
    int env_id = this->InsertEnv(ui->windDirection->value(), ui->windPower->value());
    int can_id = this->InsertCanon(ui->canonRotation->value(), ui->canonInclinaison->value(), ui->shotPower->value());
    int conf_id = this->InsertImpactConfiguration(can_id,env_id,imp_id);

    this->UpdateImpact();
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


            float wind_direction = this->ui->windDirection->value(); // Example wind direction in degrees
            float wind_power = this->ui->windPower->value(); // Example wind power
            float projectile_mass = 1.0f; // Example projectile mass in kg
            float x = mouseEvent->pos().x()-(this->touchWidth/2);
            float z = (mouseEvent->pos().y()-(this->touchHeight/2))*-1;

            QVector3D fVent = calculateWindForce(wind_direction,wind_power);

            QList<int> newCanonConf= adjusteCanon(x,z, fVent, projectile_mass);
            this->ui->canonRotation->setValue(newCanonConf.value(0));
            this->ui->canonInclinaison->setValue(newCanonConf.value(1));
            this->ui->shotPower->setValue(newCanonConf.value(2));

            qDebug() << "New Conf" << newCanonConf;

            //return QList{direction,inclinaison, puissance};



        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
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

