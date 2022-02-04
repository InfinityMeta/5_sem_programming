#include <iostream>
#include <utility>

#define double_pair_type() pair<double, double>

#define struct_inner(type, var, begin, end)\
type var;\
begin pair <double, double> operator+ (const Number& x) const end;\
begin pair <double, double> operator+ (const Integer& x) const end;\
begin pair <double, double> operator+ (const Real& x) const end;\
begin pair <double, double> operator+ (const Complex& x) const end;

#define addition(first_left, first_right, second_left, second_right)\
pair <double, double> p;\
p.first = first_left + first_right;\
p.second = second_left + second_right;\
return p;

#define struct_addition(struct_name, struct_first, struct_second)\
pair <double, double> struct_name::operator+ (const Number& x) const\
{\
return x + *this;\
}\
pair <double, double> struct_name::operator+ (const Integer& x) const\
{\
    addition(struct_first, x.n, struct_second, 0);\
}\
pair <double, double> struct_name::operator+ (const Real& x) const\
{\
    addition(struct_first, x.a, struct_second, 0);\
}\
pair <double, double> struct_name::operator+ (const Complex & x) const\
{\
    addition(struct_first, x.c.first, struct_second, x.c.second);\
}


using namespace std;

struct Integer;
struct Real;
struct Complex;

struct Number
{
    struct_inner(virtual, ~Number() {}, virtual, = 0);
};


struct Integer : Number
{
    struct_inner(int, n);
};

struct Real : Number
{
    struct_inner(double, a);
};

struct Complex : Number
{
    struct_inner(double_pair_type(), c);
};

struct_addition(Integer, n, 0);

struct_addition(Real, a, 0);

struct_addition(Complex, c.first, c.second);

ostream& operator << (std::ostream& os, pair <double, double> num)
{
    if (num.second == 0)
        return os << num.first;
    else
        return os << num.first << "+i*" << num.second;
}

int main()
{
    Integer x; x.n = 6;
    Complex y; y.c.first = 2; y.c.second = 4;
    Number& p = x, & q = y;
    cout << p + q;
    return 0;
}
