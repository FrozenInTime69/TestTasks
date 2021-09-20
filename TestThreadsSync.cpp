// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
mutex mu;
condition_variable cond;
bool ready = true;

void print_first()
{
	while (true)
	{
		unique_lock<mutex> locker(mu);
		cond.wait(locker, []{return ready;});
		cout << "1" << "\n";
		//std::this_thread::sleep_for(std::chrono::seconds(3));
		locker.unlock();
		cond.notify_one();
		ready = false;
	}
}

void print_second()
{
	while (true)
	{
		unique_lock<mutex> locker(mu);
		cond.wait(locker, [] {return !ready; });
		cout << "2" << "\n";
		//std::this_thread::sleep_for(std::chrono::seconds(2));
		locker.unlock();
		cond.notify_one();
		ready = true;
	}
}
int main()
{
	thread t1(print_first);
	thread t2(print_second);
	t1.join();
	t2.join();
	return 0;
}
