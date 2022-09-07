// multiThread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <ctime>

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

void add(int* val, int min, int max) {
	for (int i = min; i <= max; i++) {
		*val += i;
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

	int num = 0;

	float start = clock();

	thread t1(add, &num, 1, 3333);
	thread t2(add, &num, 3334, 6666);
	thread t3(add, &num, 6667, 10000);

	t1.join();
	t2.join();
	t3.join();

	float end = clock();

	cout << "Result : " << num << endl;
	cout << "Working Time : " << (double)(end - start) / CLOCKS_PER_SEC << endl;

	//cout << "Main End" << endl;

	return 0;
}
