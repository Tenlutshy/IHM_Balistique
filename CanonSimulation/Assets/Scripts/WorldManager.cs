using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldManager : MonoBehaviour
{
    public GameObject canon;
    public GameObject tcpClient;
    public GameObject windZone;
    public GameObject target;

    public static Vector3 CalculateImpactCoordinates(Vector3 cannonForce, Vector3 windForce, float projectileMass)
    {
        const float gravity = 9.81f;

        Vector3 initialVelocity = cannonForce / projectileMass;

        float timeOfFlight = (2.0f * initialVelocity.y) / gravity;

        windForce = windForce/gravity;


        Vector3 position = (initialVelocity * timeOfFlight) + windForce;

        position.y = 0.0f;

        return position;
    }

    private void Update()
    {
        var rot = Quaternion.AngleAxis(canon.GetComponentInChildren<Pulser>().rotation, Vector3.up);
        var lDirection = rot * Vector3.forward;


        Vector3 directionVent = windZone.GetComponent<WindZoneGO>().direction;

        float puissanceVent = windZone.GetComponent<WindZoneGO>().power;

        target.transform.position = CalculateImpactCoordinates(canon.GetComponentInChildren<Pulser>().force, directionVent*puissanceVent, 1.0f);

        Debug.DrawLine(Vector3.zero, target.transform.position, Color.green);

    }
}
