#ifndef PREVISION_H
#define PREVISION_H

#include <QVector3D>
#include <QList>

class Prevision
{
public:
    Prevision();
    static QVector3D calculateWindForce(float windAngle, float windForce);
    static QVector3D calculateCannonForce(float cannonAngle, float directionAngle, float cannonPower);
    static QVector3D calculateImpactCoordinates(const QVector3D& cannonForce, const QVector3D& windForce, float projectileMass);
    static QList<int> adjusteCanon(float x, float z, QVector3D& windForce, float projectileMass);
private:
    static float degreesToRadians(float degrees);
};

#endif // PREVISION_H
