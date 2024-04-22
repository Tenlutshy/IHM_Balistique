using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldManager : MonoBehaviour
{
    public GameObject canon;
    public GameObject tcpClient;
    public GameObject windZone;
    public GameObject target;


    public Vector3 CalculateImpactCoordinates(float power, float cannonAngle, Vector3 shootDirection, float gravity, float windForce, Vector3 windDirection)
    {
        // Convert the cannon angle from degrees to radians
        float cannonAngleRad = cannonAngle * Mathf.Deg2Rad;

        // Calculate the initial velocity components
        float initialVelocityX = power * Mathf.Cos(cannonAngleRad);
        float initialVelocityY = power * Mathf.Sin(cannonAngleRad);

        // Calculate the time of flight
        float timeOfFlight = 2f * initialVelocityY / gravity;

        // Calculate the horizontal distance
        float horizontalDistance = initialVelocityX * timeOfFlight;

        // Calculate the vertical distance
        float verticalDistance = (initialVelocityY * timeOfFlight) - (0.5f * gravity * timeOfFlight * timeOfFlight);

        // Apply wind effect
        horizontalDistance += (windForce * Mathf.Cos(Vector3.Angle(shootDirection, windDirection)) * timeOfFlight);

        // Calculate the impact coordinates
        Vector3 impactCoordinates = shootDirection.normalized * horizontalDistance + Vector3.up * verticalDistance;

        return impactCoordinates;
    }

    /*public Vector3 CalculerCoordonneesImpact(float puissanceDeTir, float inclinaisonCanon, Vector3 directionTir, float gravite, Vector3 directionVent, float puissanceVent)
    {
        // Calcul de la vitesse initiale en x et z
        float vitesseInitialeXZ = puissanceDeTir * Mathf.Cos(inclinaisonCanon * Mathf.Deg2Rad);

        // Calcul de la vitesse initiale en y
        float vitesseInitialeY = puissanceDeTir * Mathf.Sin(inclinaisonCanon * Mathf.Deg2Rad);

        // Correction de la direction du tir avec le vent
        Vector3 directionTirCorrigee = (directionTir + directionVent.normalized * puissanceVent).normalized;

        // Calcul de la durée de vol
        float dureeVol = (2 * vitesseInitialeY) / gravite;

        // Calcul de la distance horizontale
        float distanceHorizontale = vitesseInitialeXZ * dureeVol;

        // Calcul de la hauteur maximale atteinte
        float hauteurMaximale = (vitesseInitialeY * vitesseInitialeY) / (2 * gravite);

        // Calcul des coordonnées de l'impact
        float impactX = directionTirCorrigee.x * distanceHorizontale;
        float impactY = directionTirCorrigee.y * hauteurMaximale;
        float impactZ = directionTirCorrigee.z * distanceHorizontale;

        return new Vector3(impactX, impactY, impactZ);
    }*/

    /*public Vector3 CalculerCoordonneesImpact(float puissanceDeTir, float inclinaisonCanon, Vector3 directionTir, float gravite, Vector3 directionVent, float puissanceVent)
    {
        // Calcul de la vitesse initiale en x et z
        float vitesseInitialeXZ = puissanceDeTir * Mathf.Cos(inclinaisonCanon * Mathf.Deg2Rad);

        // Calcul de la vitesse initiale en y
        float vitesseInitialeY = puissanceDeTir * Mathf.Sin(inclinaisonCanon * Mathf.Deg2Rad);

        // Correction de la vitesse initiale avec la force du vent
        vitesseInitialeXZ += directionVent.x * puissanceVent;
        vitesseInitialeY += directionVent.y * puissanceVent;

        // Calcul de la durée de vol
        float dureeVol = (2 * vitesseInitialeY) / gravite;

        // Calcul de la distance horizontale
        float distanceHorizontale = vitesseInitialeXZ * dureeVol;

        // Calcul de la hauteur maximale atteinte
        float hauteurMaximale = (vitesseInitialeY * vitesseInitialeY) / (2 * gravite);

        // Calcul des coordonnées de l'impact
        float impactX = directionTir.x * distanceHorizontale;
        float impactY = hauteurMaximale; // La hauteur maximale est la hauteur de l'impact
        float impactZ = directionTir.z * distanceHorizontale;

        return new Vector3(impactX, impactY, impactZ);
    }*/


    private void Update()
    {
        var rot = Quaternion.AngleAxis(canon.GetComponentInChildren<Pulser>().rotation, Vector3.up);
        // that's a local direction vector that points in forward direction but also 45 upwards.
        var lDirection = rot * Vector3.forward;


        Vector3 directionVent = windZone.GetComponent<WindZoneGO>().direction;
        float puissanceVent = windZone.GetComponent<WindZoneGO>().power;

        // Calcul de l'impact en tenant compte de la force du vent
        target.transform.position = CalculateImpactCoordinates(
            canon.GetComponentInChildren<Pulser>().power,
            canon.GetComponentInChildren<Pulser>().orientation,
            lDirection, // Direction du tir depuis le canon
            9.81f, // Gravité terrestre standard
            puissanceVent,directionVent
        );
    }
}
