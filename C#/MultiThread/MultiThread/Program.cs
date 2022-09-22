using System;
using System.Threading;

//class ThreadExample
//{
//    public static void Func1(int num)
//    {
//        for (int i = 0; i < num; i++)
//        {
//            Console.WriteLine("Thread1 is running");
//        }
//    }

//    public static void Func2()
//    {
//        Thread.Sleep(1000 * 5);
//        Console.WriteLine("Thread2");
//    }


//    public static void Main()
//    {
//        Thread t1 = new Thread(() => Func1(100));
//        t1.Start();

//        Thread t2 = new Thread(Func2);
//        t2.Start();
//    }

//}

namespace Increase
{
    class Program
    {
        static void Main(string[] args)
        {
            Data myData = new Data();
            Thread myThread1 = new Thread(MyFunc);
            Thread myThread2 = new Thread(MyFunc);

            myThread1.Start(myData);
            myThread2.Start(myData);

            myThread1.Join();
            myThread2.Join();
        }

        private static void MyFunc(object obj)
        {
            Data targetData = obj as Data;
            for (int i = 0; i < 10; i++)
            {
                targetData.Increase();
                Console.WriteLine(targetData.num);
            }
        }

        class Data
        {
            private object obj = new object();
            public int num = 0;

            public void Increase()
            {
                lock (obj)
                {
                    this.num++;
                    Thread.Sleep(5);
                }
            }
        }

    }
}