#include <iostream>
#include <vector>
#include <thread>
#include<random>
#include<set>
#include<mutex>
#include<queue>
#include <condition_variable>

using namespace std;
using namespace this_thread;

condition_variable cv;

mt19937 mt;
uniform_int_distribution<int> r(0, 100'000);

mutex m;

queue<int> objects;
bool work1 = true, work2 = true;
vector<int> v1, v2;

bool f() 
{
	return !(objects.empty());
}

void factory()
{
	while (work1 == true)
	{
		int object = r(mt);
		objects.push(object);
		cout << object << "produced" << endl;
		v1.push_back(object);
	}
}

void consumer()
{
	while (work2 == true)
	{
		if (objects.empty())
			sleep_for(100ms);
		else
		{
			v2.push_back(objects.front());
			cout << objects.front() << "consumed" << endl;
			objects.pop();
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

	if (!objects.empty())
	{
		work2 = true;
		thread c1(consumer);
		thread c2(consumer);
		thread c3(consumer);
		sleep_for(500ms);
		work2 = false;
		c1.join();
		c2.join();
		c3.join();
	}

	multiset<int> s1, s2;

	for (int i : v1)
		s1.insert(i);
	for (int i : v2)
		s2.insert(i);


	return s1 == s2;

}

void factory_m()
{
	while (work1 == true)
	{
		m.lock();
		int object = r(mt);
		objects.push(object);
		cout << object << "produced" << " ";
		v1.push_back(object);
		m.unlock();
	}
}

void consumer_m()
{
	while (work2 == true)
	{
		m.lock();
		if (objects.empty())
			sleep_for(100ms);
		else
		{
			v2.push_back(objects.front());
			cout << objects.front() << "consumed" << " ";
			objects.pop();
		}
		m.unlock();
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

	if (!objects.empty())
	{
		work2 = true;
		thread c1(consumer_m);
		thread c2(consumer_m);
		thread c3(consumer_m);
		sleep_for(500ms);
		work2 = false;
		c1.join();
		c2.join();
		c3.join();
	}

	multiset<int> s1, s2;

	for (int i : v1)
		s1.insert(i);
	for (int i : v2)
		s2.insert(i);

	return s1 == s2;

}

void factory_cond()
{
	while (work1 == true)
	{
		lock_guard<mutex> l(m);
		int object = r(mt);
		objects.push(object);
		cout << object << "produced" << endl;
		v1.push_back(object);
		cv.notify_one();
	}
}

void consumer_cond()
{
	while (work2 == true)
	{
		unique_lock<mutex> l(m);
		cv.wait(l, f);
		int object = objects.front();
		cout << object << "consumed" << endl;
		v2.push_back(object);
		objects.pop();
	}
}

bool check_all_cond()
{
	thread p1(factory_cond);
	thread p2(factory_cond);
	thread c1(consumer_cond);
	thread c2(consumer_cond);
	thread c3(consumer_cond);

	sleep_for(1000ms);

	work1 = false;
	work2 = false;

	c1.join();
	c2.join();
	c3.join();

	if (!objects.empty())
	{
		work2 = true;
		thread c1(consumer_cond);
		thread c2(consumer_cond);
		thread c3(consumer_cond);
		sleep_for(500ms);
		work2 = false;
		c1.detach();
		c2.detach();
		c3.detach();
	}

	p1.join();
	p2.join();

	cout << "size" << objects.size() << endl;

	multiset<int> s1, s2;

	for (int i : v1)
		s1.insert(i);
	for (int i : v2)
		s2.insert(i);

	return s1 == s2;

}


int main()
{
	cout << check_all_cond() << endl;

	return 0;
}
