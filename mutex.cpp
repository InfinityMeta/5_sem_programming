#include <iostream>
#include <vector>
#include <thread>
#include<random>
#include<mutex>

using namespace std;
using namespace this_thread;

mt19937 mt;
uniform_int_distribution<int> r(0, 100'000);

mutex m;

int object; 
bool flag = false; 
bool work1 = true, work2 = true;
vector<int> v1, v2;

void factory()
{
	while (work1 == true)
	{
		if (flag == true)
			sleep_for(100ms);
		else
		{
			object = r(mt);
			v1.push_back(object);
			flag = true;
		}

	}
}

void consumer()
{
	while (work2 == true)
	{
		if (flag == false)
			sleep_for(100ms);
		else
		{
			v2.push_back(object);
			flag = false;
		}

	}
}

bool check_all()
{
	thread p1(factory);
	thread p2(factory);
	thread c1(consumer);
	thread c2(consumer);
	thread c3(consumer);

	
	sleep_for(1000ms);

	work1 = false;
	work2 = false;
	
	p1.join();
	p2.join();
	c1.join();
	c2.join();
	c3.join();

	if (v1.size() > v2.size())
	{
		work2 = true;
		thread c1(consumer);
		sleep_for(100ms);
		work2 = false;
		c1.join();
	}

	return v1 == v2;

}

void factory_m()
{
	while (work1 == true)
	{
		m.lock();
		if (flag == true)
		{
			m.unlock();
			sleep_for(100ms);
		}
		else
		{
			object = r(mt);
			cout << object << "produced" << endl;
			v1.push_back(object);
			flag = true;
			m.unlock();
		}

	}
}


void consumer_m()
{
	while (work2 == true)
	{
		m.lock();
		if (flag == false)
		{
			m.unlock();
			sleep_for(100ms);
		}
		else
		{
			v2.push_back(object);
			cout << object << "consumed" << endl;
			flag = false;
			m.unlock();
		}

	}
}


bool check_all_m()
{
	thread p1(factory_m);
	thread p2(factory_m);
	thread c1(consumer_m);
	thread c2(consumer_m);
	thread c3(consumer_m);

	sleep_for(1000ms);

	work1 = false;
	work2 = false;

	p1.join();
	p2.join();
	c1.join();
	c2.join();
	c3.join();

	if (v1.size() > v2.size())
	{
		work2 = true;
		thread c1(consumer_m);
		sleep_for(100ms);
		work2 = false;
		c1.join();
	}

	return v1 == v2;
}


int main()
{
	cout << check_all_m() << endl;
	return 0;
}
