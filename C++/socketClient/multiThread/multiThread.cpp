// multiThread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <ctime>
#include <mutex>

using namespace std;

void func1()
{
	for (int i = 0; i < 10; i++)
	{
		cout << "Threa1 Working.." << endl;
	}
}

void func2()
{
	for (int i = 0; i < 10; i++)
	{
		cout << "Threa2 Working.." << endl;
	}
}

void add1(int* val, int min, int max, mutex* m1, mutex* m2) {
	for (int i = min; i <= max; i++) {
		m1->lock();
		m2->lock();
		cout << "Thread 1" << endl;
		//*val += i;
		m2->unlock();
		m1->unlock();
	}
}

void add2(int* val, int min, int max, mutex* m1, mutex* m2) {
	for (int i = min; i <= max; i++) {
		while (true) {
			m2->lock();
			if (m1->try_lock()) {
				m2->unlock();
				continue;
			}
			cout << "Thread 2" << endl;
			//*val += i;
			m1->unlock();
			m2->unlock();

			break;
		}
	}
}

int main()
{
	/*thread t1(func1);
	thread t2(func2);
	thread t3([] {
		for (int i = 0; i < 10; i++) {
			cout << "Thread3 Working.." << endl;
		}
		});

	t1.join();
	t2.join();
	t3.join();*/

	mutex m1, m2;

	int num = 0;

	float start = clock();

	//thread t1(add, &num, 1, 3333, &m1);
	//thread t2(add, &num, 3334, 6666, &m1);
	//thread t3(add, &num, 6667, 10000, &m1);
	thread t1(add1, &num, 1, 10, &m1, &m2);
	thread t2(add2, &num, 11, 20, &m1, &m2);

	t1.join();
	t2.join();
	//t3.join();

	float end = clock();

	cout << "Result : " << num << endl;
	cout << "Working Time : " << (double)(end - start) / CLOCKS_PER_SEC << endl;

	//cout << "Main End" << endl;

	return 0;
}
