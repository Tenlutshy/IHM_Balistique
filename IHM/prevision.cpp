#include "prevision.h"

Prevision::Prevision() {}

float Prevision::degreesToRadians(float degrees) {
    return degrees * M_PI / 180.0f;
}

QVector3D Prevision::calculateWindForce(float windAngle, float windForce) {
    float radians = degreesToRadians(windAngle);

    float windForceX = windForce * std::cos(radians);
    float windForceZ = windForce * std::sin(radians);

    return QVector3D(windForceX, 0.0f, windForceZ);
}

QVector3D Prevision::calculateCannonForce(float cannonAngle, float directionAngle, float cannonPower) {
    float radiansCannon = degreesToRadians(cannonAngle);
    float radiansDirection = degreesToRadians(directionAngle);

    float cannonForceX = cannonPower * std::cos(radiansCannon) * std::cos(radiansDirection);
    float cannonForceY = cannonPower * std::sin(radiansCannon);
    float cannonForceZ = cannonPower * std::cos(radiansCannon) * std::sin(radiansDirection);

    return QVector3D(cannonForceX, cannonForceY, cannonForceZ);
}

QVector3D Prevision::calculateImpactCoordinates(const QVector3D& cannonForce, const QVector3D& windForce, float projectileMass)
{
    const float gravity = 9.81f;

    QVector3D initialVelocity = cannonForce / projectileMass;

    float timeOfFlight = (2.0f * initialVelocity.y()) / gravity;   // Prise en compte de la taille a realisé

    QVector3D adjustedWindForce = windForce / gravity;

    QVector3D position = (initialVelocity * timeOfFlight) + adjustedWindForce;

    position.setY(0.0f);

    return position;
}


QList<int> Prevision::adjusteCanon(float x, float z, QVector3D& windForce, float projectileMass){
    int max_inclinaison = 90;
    int max_direction = 359;
    int max_puissance = 30;

    for (int d=0; d <= max_direction; d++){
        int direction = d;

        for (int i=0; i <= max_inclinaison; i++){
            int inclinaison = i <= 45 ? i+45 : 45-i+45;

            for (int p=0; p<= max_puissance; p++){
                int puissance = p;

                QVector3D previsionImpact = calculateImpactCoordinates(calculateCannonForce(inclinaison, direction, puissance),windForce,projectileMass);

                double epsilon = 1;

                if (fabs(previsionImpact.x() - z) < epsilon && fabs(previsionImpact.z() - x) < epsilon){
                    return QList{direction,inclinaison, puissance};
                }

            }


        }

    }
    return QList{1000,1000,1000};

}
