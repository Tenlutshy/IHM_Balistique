#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QMap>
#include <QtSql>

class DatabaseManager: public QObject
{
public:
    DatabaseManager(QObject *parent);

    QSqlDatabase db;

    void init();
    int InsertImpact(float x, float y, float z);
    int InsertEnv(float wDir, float wPow);
    int InsertCanon(float dir, float inc, float pow);
    int InsertBullet(int weidth);
    int InsertImpactConfiguration(int canon, int env, int impact, int bullet);
    QList<int> GetImpactConfiguration(int impact_conf_id);
    void RemoveImpactConfig(int impactConfId);

private:
};

#endif // DATABASEMANAGER_H
