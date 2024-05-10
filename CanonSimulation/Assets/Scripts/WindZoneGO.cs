using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class WindZoneGO : MonoBehaviour
{
    public Vector3 direction = Vector3.zero;
    public float power = 0;

    private void OnTriggerEnter(Collider other)
    {
        if (!other.gameObject.CompareTag("Bullet") || power == 0) return;

        Debug.Log("yop");
        StartCoroutine(ApplyWind(other.gameObject));
    }

    private IEnumerator ApplyWind(GameObject rb)
    {

        if(rb.IsDestroyed()) yield break;
        
        rb.GetComponent<Rigidbody>().AddForce(direction * power);
        yield return new WaitForSeconds(1);
        StartCoroutine(ApplyWind(rb));
    }


    private void Update()
    {
        Vector3 point_C = Vector3.zero+ (direction.normalized * power);
        //Draw the line
        Debug.DrawLine(Vector3.zero, point_C, Color.red);
    }
}
