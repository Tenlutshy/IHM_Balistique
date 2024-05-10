#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cmath>

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



struct Vector3D {
    float x;
    float y;
    float z;
};

Vector3D calculateImpactCoordinates(float rotation, float inclination, float power, float wind_direction, float wind_power, float projectile_mass) {
    const float gravity = 9.81f; // Gravitational acceleration in m/s^2
    const float drag_coefficient = 0.5f; // Drag coefficient of the projectile (for simplification)
    const float time_step = 1.0f; // Time step for simulation in seconds

    // Convert angles to radians
    float rotation_rad = rotation * M_PI / 180.0f;
    float inclination_rad = inclination * M_PI / 180.0f;
    float wind_direction_rad = wind_direction * M_PI / 180.0f;

    // Initialize variables
    float velocity_x = power * std::cos(inclination_rad) * std::cos(rotation_rad);
    float velocity_y = power * std::sin(inclination_rad);
    float velocity_z = power * std::cos(inclination_rad) * std::sin(rotation_rad);

    float position_x = 0.0f;
    float position_y = 0.0f;
    float position_z = 0.0f;

    // Simulate projectile motion
    for (float t = 0; position_y >= 0.0f; t += time_step) {
        // Calculate wind force at current time
        float wind_force_x = wind_power * std::cos(wind_direction_rad);
        float wind_force_z = wind_power * std::sin(wind_direction_rad);

        // Apply wind force to the projectile (impulse mode)
        float wind_acceleration_x = wind_force_x / projectile_mass;
        float wind_acceleration_z = wind_force_z / projectile_mass;

        // Update velocity with wind effect
        velocity_x += wind_acceleration_x * time_step;
        velocity_z += wind_acceleration_z * time_step;

        // Update position using current velocity
        position_x += velocity_x * time_step;
        position_y += velocity_y * time_step;
        position_z += velocity_z * time_step;

        // Update velocity with gravity effect
        velocity_y -= gravity * time_step;
    }

    return {position_x, position_y, position_z};
}



void MainWindow::on_shotBtn_clicked()
{
    tcp.Send_Message(1);
    float rotation = this->ui->canonRotation->value(); // Example rotation angle in degrees
    float inclination = this->ui->canonInclinaison->value(); // Example inclination angle in degrees
    float power = this->ui->shotPower->value(); // Example power of the shot
    float wind_direction = this->ui->windDirection->value(); // Example wind direction in degrees
    float wind_power = this->ui->windPower->value(); // Example wind power
    float projectile_mass = 1.0f; // Example projectile mass in kg

    Vector3D impact_coordinates = calculateImpactCoordinates(rotation, inclination, power, wind_direction, wind_power, projectile_mass);

    std::cout << "Impact coordinates (x, z): (" << impact_coordinates.x << ", " << impact_coordinates.z << ")" << std::endl;
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
    this->ui->te_cpow->setPlainText(QString::number(value));
}


void MainWindow::on_canonInclinaison_valueChanged(int value)
{
    this->tcp.Send_Message(4,2,value);
    this->ui->te_cinc->setPlainText(QString::number(value));
}


void MainWindow::on_canonRotation_valueChanged(int value)
{
    this->tcp.Send_Message(4,1,value);
    this->ui->te_cdir->setPlainText(QString::number(value));
}


void MainWindow::on_windPower_valueChanged(int value)
{
    this->tcp.Send_Message(2,1,value);
    this->UpdateImpact();
    this->ui->te_wpow->setPlainText(QString::number(value));
}


void MainWindow::on_windDirection_valueChanged(int value)
{
    this->tcp.Send_Message(2,2,value);
    this->UpdateImpact();
    this->ui->te_wdir->setPlainText(QString::number(value));
}

