using System;
using UnityEngine;
using UnityEngine.XR;
using Debug = UnityEngine.Debug;

public class TrameCoder : MonoBehaviour
{

    public WorldManager WorldManager;

    private Pulser pulser;
    private WindZoneGO windZone;

    void Start() {
        WorldManager = GameObject.FindWithTag("WorldManager").GetComponent<WorldManager>();
        pulser = WorldManager.canon.GetComponentInChildren<Pulser>();
        windZone = WorldManager.windZone.GetComponent<WindZoneGO>();
    }

    public void Decode(byte[] data)
    {

        byte firstByte = data[0];

        byte[] newDataBytes = new byte[data.Length - 1];
        Array.Copy(data, 1, newDataBytes, 0, newDataBytes.Length);

        data = newDataBytes;

        int type = firstByte;
        Debug.Log("Type : " + type);

        int dtype;

        switch (type)
        {
            case 1:
                pulser.Shot();
                break;
            case 2:
                Debug.Log("Env");
                firstByte = data[0];

                newDataBytes = new byte[data.Length - 1];
                Array.Copy(data, 1, newDataBytes, 0, newDataBytes.Length);

                data = newDataBytes;

                dtype = firstByte;
                Debug.Log("Data Type : " + dtype);

                switch (dtype)
                {
                    case 1:

                        Debug.Log("Wind power");

                        if (BitConverter.IsLittleEndian)
                            Array.Reverse(data);

                        try
                        {

                            windZone.power = BitConverter.ToInt32(data);
                        }
                        catch (Exception ex)
                        {
                            Debug.Log(ex);
                        }

                        break;
                    case 2:
                        Debug.Log("Wind Dir");

                        if (BitConverter.IsLittleEndian)
                            Array.Reverse(data);

                        var rot = Quaternion.AngleAxis(BitConverter.ToInt32(data), Vector3.up);
                        // that's a local direction vector that points in forward direction but also 45 upwards.
                        var lDirection = rot * Vector3.forward;
                        windZone.direction = lDirection;

                        //pulser.orientation = BitConverter.ToInt32(data);

                        break;

                    default: break;
                }


                break;
            case 3:
                Debug.Log("Proj");
                break;
            case 4:
                Debug.Log("Canon");
                firstByte = data[0];

                newDataBytes = new byte[data.Length - 1];
                Array.Copy(data, 1, newDataBytes, 0, newDataBytes.Length);

                data = newDataBytes;

                dtype = firstByte;
                Debug.Log("Data Type : " + dtype);

                switch (dtype)
                {
                    case 1:

                        if (BitConverter.IsLittleEndian)
                            Array.Reverse(data);

                        try
                        {

                            pulser.rotation = BitConverter.ToInt32(data);
                        }
                        catch (Exception ex)
                        {
                            Debug.Log(ex);
                        }

                        break;
                    case 2:

                        if (BitConverter.IsLittleEndian)
                            Array.Reverse(data);

                        pulser.orientation = BitConverter.ToInt32(data);

                        break;
                    case 3:


                        if (BitConverter.IsLittleEndian)
                            Array.Reverse(data);
                        pulser.power = BitConverter.ToInt32(data);
                        break;

                    default: break;
                }




                break;
            default:
                Debug.Log("default");
                break;
        }
        Debug.Log(newDataBytes);

        /*string msg = Encoding.UTF8.GetString(data);
        Debug.Log(msg);*/
    }

    public byte[] getNBytes()
    {
        return new byte[0];
    }
}
