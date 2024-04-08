using System;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class uTCPClient : MonoBehaviour
{
    public string host = "localhost";
    public int port = 8080;
    public GameObject TrameCoderObject;
    public TrameCoder trameCoder;

    private TcpClient tcpClient;
    private NetworkStream stream;
    private Thread tClientReceive;

    void Start()
    {
        trameCoder = TrameCoderObject.GetComponent<TrameCoder>();
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
        }
        catch (SocketException e)
        {
            Debug.Log("SocketException" + e.ToString());
        }
    }

    private async void Listen()
    {
        try
        {
            while (true)
            {
                if (stream.DataAvailable)
                {
                    int length;

                    // Get header
                    byte[] headerBytes = new byte[4];
                    int dataLength = 0;
                    
                    length = stream.ReadAsync(headerBytes, 0, headerBytes.Length).Result;
                
                    var data = new byte[length];
                    Array.Copy(headerBytes, 0, data, 0, length);

                    if (BitConverter.IsLittleEndian)
                    {
                        Array.Reverse(headerBytes);
                    }

                    dataLength = BitConverter.ToInt32(headerBytes, 0);
                    Debug.Log("Header : " + dataLength);




                    byte[] dataBytes = new byte[dataLength];
                    int totalBytesRead = 0;
                    int bytesRead;




                    while (totalBytesRead < dataLength)
                    {
                        bytesRead = await stream.ReadAsync(dataBytes, totalBytesRead, dataLength - totalBytesRead);

                        if (bytesRead == 0)
                        {
                            // La connexion a été fermée
                            break;
                        }

                        totalBytesRead += bytesRead;
                    }

                    trameCoder.Decode(dataBytes);

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
