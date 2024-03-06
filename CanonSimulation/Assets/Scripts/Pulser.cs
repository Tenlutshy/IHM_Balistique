using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pulser : MonoBehaviour
{

    public GameObject bullet;
    public int power = 12;

    // Start is called before the first frame update
    void Start()
    {

        bullet.GetComponent<Rigidbody>().AddForce(transform.rotation * Vector3.up * power, ForceMode.Impulse);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
