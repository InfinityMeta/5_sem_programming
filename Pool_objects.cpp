#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <cassert>
#include <stdexcept>


using namespace std;

template <class T>
class ObjectPool {
public:
	T* Allocate()
	{
		if (!deallocated.empty())
		{
			T* ptr;
			ptr = deallocated.front();
			allocated.insert(ptr);
			deallocated.pop();
			return ptr;
		}
		else
		{
			T* object;
			object = new T;
			allocated.insert(object);
			return object;
		}
	}

	T* TryAllocate()
	{
		if (!deallocated.empty())
		{
			T* ptr;
			ptr = deallocated.front();
			allocated.insert(ptr);
			deallocated.pop();
			return ptr;
		}
		else
			return nullptr;
	}

	void Deallocate(T* object)
	{
		if (allocated.find(object) == allocated.end())
			throw invalid_argument("object is not allocated");
		else
		{
			deallocated.push(object);
			allocated.erase(object);
		}
	}

	~ObjectPool()
	{
		for (auto i : allocated)
		{
			delete i;
		}
		allocated.clear();
		while (!deallocated.empty())
		{
			delete deallocated.front();
			deallocated.pop();
		}
	}

private:

	set <T*> allocated; // выделенные
	queue <T*> deallocated; // освобожденные
};




int main()
{
	ObjectPool <string> pool;

	auto p1 = pool.Allocate(); // заводим строчки
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();
	
	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);

	assert(*pool.Allocate() == "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);

	assert(*pool.Allocate() == "third");
	assert(*pool.Allocate() == "first");

	pool.Deallocate(p1);
	pool.Deallocate(p1);
	
	return 0;
}