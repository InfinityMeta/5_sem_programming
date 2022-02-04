#include <iostream>

using namespace std;

template <typename T>
class SimpleVector {
public:
    explicit SimpleVector(size_t size)
    {
        data = new T[size];
        end_ = data + size;
        last_ = end_;
    }
    ~SimpleVector()
    {
        delete[] data;
    }

    T& operator[](size_t index)
    {
        return data[index];
    }

     T* begin()  { return data; }
     T* end() { return end_; }
     T* last() { return last_; }

    const T* begin() const { return data; }
    const T* end() const { return end_; }
    const T* last() const { return last_; }


    template <typename T>
    void Print(const SimpleVector<T>& v)
    {
        for (const auto& x : v)
        {
            cout << x << ' ';
        }
    }

    size_t Size() const
    {
 
        return end() - begin();
    };
    size_t Capacity() const
    {
        return last() - begin();
    }
    void PushBack(const T& value)
    {
        size_t size = Size();
        if (size == last() - begin()) {
            T* data_new = new T[size*2];
            copy(data, data + size, data_new);
            data = data_new;
            data[size] = value;
            last_ = data + size * 2;
            end_ = data + size + 1;
        }
        else
        {
            data[size] = value;
            end_ = end_ + 1;
        }
    }

private:
    T* data;
    T* end_;
    T* last_;
};

int main()
{
    SimpleVector<int> vs(5);
    cout << "size: " << vs.Size() << " \t";
    cout << "capacity: " << vs.Capacity() << endl;
    for (int i = 0; i < 20; i++)
    {
        vs.PushBack(i);
        cout << "size: " << vs.Size() << " \t";
        cout << "capacity: " << vs.Capacity() << endl;
    }

    return 0;
}
