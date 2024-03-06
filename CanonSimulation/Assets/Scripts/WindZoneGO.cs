using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WindZoneGO : MonoBehaviour
{
    public Vector3 direction = Vector3.zero;
    public float power = 0;

    private void OnTriggerStay(Collider other)
    {
        if (!other.gameObject.CompareTag("Bullet") || power == 0) return;

        other.gameObject.GetComponent<Rigidbody>().AddForce(direction * power, ForceMode.Force);
    }
}
