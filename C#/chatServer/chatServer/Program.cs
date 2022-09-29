using System;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections;

namespace ConsoleApplication
{
    class Program
    {
        public static Hashtable clientList = new Hashtable();
        private static int userCnt = 0;
        private static Mutex mut = new Mutex();

        static void Main()
        {
            try
            {
                TcpListener serverSocket = new TcpListener(IPAddress.Any, 8282);
                TcpClient clientSocket = default(TcpClient);
                int counter = 0;
                byte[] bytesFrom = new byte[1024];
                string dataFromClient = "";

                serverSocket.Start();
                Console.WriteLine("C# Server Started...");
                counter = 0;
                while (true)
                {
                    counter += 1;
                    clientSocket = serverSocket.AcceptTcpClient();
                    dataFromClient = "";

                    counter = userCnt;
                    userCnt += 1;

                    HandleClient client = new HandleClient();
                    clientList.Add(counter, client);

                    client.startClient(clientSocket, clientList, counter);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }
    }

    public class HandleClient
    {
        const string COMMAND_ENTER = "#ENTER#";
        const string COMMAND_HISTORY = "#HISTORY#";

        public TcpClient clientSocket;
        public int userID;
        public string clientID;

        private Hashtable clientList;
        private bool noConnection = false;

        public void startClient(TcpClient inClientSocket, Hashtable cList, int userSerial)
        {
            userID = userSerial;
            clientList = cList;
            clientSocket = inClientSocket;

            Thread ctThread = new Thread(doChat);
            ctThread.Start();
        }
    }
}