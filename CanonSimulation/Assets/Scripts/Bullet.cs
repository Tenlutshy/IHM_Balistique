using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    public GameObject impact;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }


    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Pulser")) return;
        Destroy(this.gameObject);
        Debug.Log(collision.contacts[0].point.x + " " + collision.contacts[0].point.y + " " + collision.contacts[0].point.z);
        if (collision.gameObject.CompareTag("Target"))
        {
            Debug.Log("Target Touch");
        }else
        {
            Instantiate(impact, collision.contacts[0].point, Quaternion.identity);
        }
    }
}
