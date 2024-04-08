using System;
using UnityEngine;
using Debug = UnityEngine.Debug;

public class TrameCoder : MonoBehaviour
{

    public WorldManager WorldManager;

    private Pulser pulser;
    void Start() {
        WorldManager = GameObject.FindWithTag("WorldManager").GetComponent<WorldManager>();
        pulser = WorldManager.canon.GetComponentInChildren<Pulser>();
    }

    public void Decode(byte[] data)
    {

        byte firstByte = data[0];

        byte[] newDataBytes = new byte[data.Length - 1];
        Array.Copy(data, 1, newDataBytes, 0, newDataBytes.Length);

        data = newDataBytes;

        /*if (BitConverter.IsLittleEndian)
        {
            Array.Reverse(firstByte);
        }*/

        int type = firstByte;
        Debug.Log("Type : " + type);

        switch (type)
        {
            case 1:
                pulser.Shot();
                break;
            case 2:
                Debug.Log("Env");
                break;
            case 3:
                Debug.Log("Proj");
                break;
            case 4:
                Debug.Log("Canon");
                break;
            default:
                Debug.Log("default");
                break;
        }

        /*string msg = Encoding.UTF8.GetString(data);
        Debug.Log(msg);*/
    }
}
