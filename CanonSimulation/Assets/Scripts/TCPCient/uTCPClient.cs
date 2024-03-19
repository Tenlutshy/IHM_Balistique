using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEditor.PackageManager;
using UnityEngine;

public class uTCPClient : MonoBehaviour
{
    public string host = "localhost";
    public int port = 8080;

    private TcpClient tcpClient;
    private NetworkStream stream;
    private Thread tClientReceive;

    void Start()
    {
        ConnectToServer();
    }

    void Update()
    {
        
    }

    void ConnectToServer()
    {
        try
        {
            tcpClient = new TcpClient(host, port);
            stream = tcpClient.GetStream();

            Debug.Log("Connected to server");

            tClientReceive = new Thread(new ThreadStart(Listen));
            tClientReceive.IsBackground = true;
            tClientReceive.Start();

            SendMessage("yop");
        }
        catch (SocketException e)
        {
            Debug.Log("SocketException" + e.ToString());
        }
    }

    private void Listen()
    {
        try
        {
            byte[] bytes = new byte[1024];
            while(true)
            {
                if (stream.DataAvailable)
                {
                    int length;
                    while ((length = stream.ReadAsync(bytes,0, bytes.Length).Result) != 0)
                    {
                        var data = new byte[length];
                        Debug.Log(data.GetValue(0));

                        Array.Copy(bytes, 0, data, 0, length);

                        string msg = Encoding.UTF8.GetString(data);
                        Debug.Log(data.Count());
                        Debug.Log("Message received : " + msg);
                    }
                }
            }
        }
        catch (SocketException e)
        {
            Debug.Log("SocketException" + e.ToString());
        }
    }

    public void SendMessage(string msg)
    {
        if (tcpClient == null || !tcpClient.Connected)
        {
            Debug.Log("Client not connected to server");
            return;
        }

        byte[] data = Encoding.UTF8.GetBytes(msg);
        stream.Write(data, 0, data.Length);
        Debug.Log("Message send : " + msg);
    }

    private void OnApplicationQuit()
    {
        if (stream != null)
        {
            stream.Close();
        }
        if (tcpClient != null)
        {
            tcpClient.Close();
        }
        if (tClientReceive != null)
        {
            tClientReceive.Abort();
        }
    }
}
