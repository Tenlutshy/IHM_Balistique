using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using Unity.VisualScripting.Antlr3.Runtime;
using UnityEngine;

public class Pulser : MonoBehaviour
{
    private readonly object _lock = new object();
    private readonly Queue<Action> _mainThreadActions = new Queue<Action>();

    public GameObject bullet;
    public Vector3 force;
    public int power = 12;
    private int _orientation = 0;
    public int orientation
    {
        get
        {
            return _orientation;
        }
        set
        {
            _orientation = value;

            lock (_lock)
            {
                _mainThreadActions.Enqueue(() =>
                {

                    //transform.rotation = Quaternion.AngleAxis(value, Vector3.right);

                    transform.Rotate(value-transform.rotation.eulerAngles.x, 0, 0, Space.Self);
                });
            }
        }
    }
    private int _rotation = 0;
    public int rotation
    {
        get
        {
            return _rotation;
        }
        set
        {
            _rotation = value;
            lock (_lock)
            {
                _mainThreadActions.Enqueue(() =>
                {
                    this.transform.parent.transform.rotation = Quaternion.AngleAxis(value, Vector3.up);
                });
            }
        }
    }


    void Start()
    {
    }

    void Update()
    {
        lock (_lock)
        {
            while (_mainThreadActions.Count > 0)
            {
                var action = _mainThreadActions.Dequeue();

                action?.Invoke();
            }
        }

        var rot = Quaternion.AngleAxis(rotation, Vector3.up);
        var lDirection = rot * Vector3.forward;

        force = transform.rotation * Vector3.up * power;

        Vector3 point_C = Vector3.zero + (lDirection.normalized * power);
        Debug.DrawLine(Vector3.zero, point_C, Color.blue);


    }

    public void Shot()
    {
        lock (_lock)
        {
            _mainThreadActions.Enqueue(() =>
            {
                Instantiate(bullet).GetComponent<Rigidbody>().AddForce(transform.rotation * Vector3.up * power, ForceMode.Impulse);
            });
        }

    }
}
