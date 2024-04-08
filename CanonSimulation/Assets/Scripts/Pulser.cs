using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;

public class Pulser : MonoBehaviour
{
    private readonly object _lock = new object();
    private readonly Queue<Action> _mainThreadActions = new Queue<Action>();

    public GameObject bullet;
    public int power = 12;

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
