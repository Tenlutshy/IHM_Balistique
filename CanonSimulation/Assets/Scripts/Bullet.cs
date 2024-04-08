using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    public GameObject impact;

    public WorldManager WorldManager;
    public uTCPClient uTCPClient;

    public float massKG {
        get {
            return this.gameObject.GetComponent<Rigidbody>().mass;
        }
        set
        {
            this.gameObject.GetComponent<Rigidbody>().mass = value;
        }
    }

    private float _radiusM = 0.25f;
    public float radiusM
    {
        get
        {
            return _radiusM;
        }
        set
        {
            _radiusM = value;
            this.transform.localScale = new Vector3(value * 2, value * 2, value * 2);
        }
    }


    void Start()
    {
        WorldManager = GameObject.FindWithTag("WorldManager").GetComponent<WorldManager>();
        uTCPClient = WorldManager.tcpClient.GetComponent<uTCPClient>();
    }

    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Pulser")) return;
        Debug.Log(collision.contacts[0].point.x + " " + collision.contacts[0].point.y + " " + collision.contacts[0].point.z);
        uTCPClient.SendMessage(collision.contacts[0].point.x + " " + collision.contacts[0].point.y + " " + collision.contacts[0].point.z);
        Destroy(this.gameObject);
        if (collision.gameObject.CompareTag("Target"))
        {
            Debug.Log("Target Touch");
        }else
        {
            Instantiate(impact, collision.contacts[0].point, Quaternion.identity);
        }
    }
}
