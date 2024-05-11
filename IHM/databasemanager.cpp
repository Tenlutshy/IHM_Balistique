#include "databasemanager.h"
#include "mainwindow.h"

DatabaseManager::DatabaseManager(QObject* parent):QObject(parent) {
}

void DatabaseManager::init(){
    QDir databasePath;
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(databasePath.currentPath()+"/database.db");

    if(db.open()){
        qDebug() << "DB Connected";
        qDebug() << this->db.tables();
        qobject_cast<MainWindow *>(parent())->UpdateImpact();
    } else {
        qDebug() << "Error in DB Connection";
    }
}

int DatabaseManager::InsertImpact(float x, float y, float z){
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

int DatabaseManager::InsertEnv(float wDir, float wPow){
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

int DatabaseManager::InsertCanon(float dir, float inc, float pow){
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

int DatabaseManager::InsertImpactConfiguration(int canon, int env, int impact){
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

QList<int> DatabaseManager::GetImpactConfiguration(int impact_conf_id){

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

void DatabaseManager::RemoveImpactConfig(int impactConfId){
    QSqlQuery qryRemoveImpactConfig;
    qryRemoveImpactConfig.prepare("DELETE FROM ImpactConfiguration WHERE id=:ii");
    qryRemoveImpactConfig.bindValue(":ii",impactConfId);
    qryRemoveImpactConfig.exec();
};
