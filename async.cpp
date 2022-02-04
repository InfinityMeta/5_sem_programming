#include <iostream>
#include <chrono>
#include <future>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;
using namespace chrono;

double pi_part_compution(double num_start, double num_finish)
{
    double pi = 0.0;
    for (double i = num_start; i < num_finish; i+=1.0)
    {
        pi += 2 / ((4 * i + 1) * (4 * i + 3));
    }
    pi = pi * 4;
    return pi;
}

double pi_parallel_сompution(int num_threads, int num_precise)
{
    double step = (double)num_precise / (double)num_threads;
    double i = 0.0;
    double result = 0.0;
    vector<future <double>> fut;
    while (i <= num_precise)
    {
        fut.push_back(async(pi_part_compution, i, i + step));
        i += step;
    }
    for (int j = 0; j < fut.size(); j++)
        result += fut[j].get();
    return result;
}


int rest_10(int size)
{
    int deg = size % 16;
    unsigned long long in_deg = pow(10, deg);
    return in_deg % 17;
}

int rest(string st, int threads, bool flag)
{
    if (st.length() <= 4)
    {
        unsigned long long num = stoull(st);
        return num % 17;
    }
    else
    {
        vector <string> str(threads);
        vector <future<int>> results1(threads);
        vector <int> results2(threads);
        int step = st.length() / threads;
        for (int i = 0; i < str.size() - 1; i++)
        {
            str[i].resize(step);
        }
        for (int i = 0; i < str.size() - 1; i++)
        {
            for (int j = i * step; j < (i + 1) * step; j++)
            {
                str[i][j - i * step] = st[j];
            }
            if (flag == false)
            {
                flag = true;
                results1[i] = async(rest, str[i], threads, flag);
                flag = false;
            }
            else
                results2[i] = rest(str[i], threads, flag);
        }
        str[str.size() - 1].resize(st.length() - (threads - 1) * step);
        for (int j = (threads - 1) * step; j < st.length(); j++)
        {
            str[threads - 1][j - (threads - 1) * step] = st[j];
        }
        if (flag == false)
        {
            flag = true;
            results1[threads - 1] = async(rest, str[threads - 1], threads, flag);
            flag = false;
        }
        else
            results2[threads - 1] = rest(str[threads - 1], threads, flag);

        int sum_rest = 0;

        if (flag == false)
        {
            for (int k = 0; k < results1.size() - 1; k++)
            {
                sum_rest += (results1[k].get() * rest_10(st.length() - step * (k + 1))) % 17;
            }
            sum_rest += results1[results1.size() - 1].get() % 17;
        }
        else
        {
            for (int k = 0; k < results2.size() - 1; k++)
            {
                sum_rest += (results2[k] * rest_10(st.length() - step * (k + 1))) % 17;
            }
            sum_rest += results2[results2.size() - 1] % 17;
        }

        return sum_rest % 17;
    }
}

int Find_thread(string what, string s, int threads, int first_index)
{
    int step = what.length() / threads;
    int count = 0;
    if (first_index == step * (threads-1))
    {
        for (int i = first_index; i <= what.length()-s.length(); i++)
        {
            string cand = what.substr(i, s.length());
            if (s == cand)
                count++;
        }
    }
    else
    {
        for (int i = first_index; i < first_index + step; i++)
        {
            string cand = what.substr(i, s.length());
            if (s == cand)
                count++;
        }
    }
    return count;
}

int MultiThreadFind(string what, string s, int threads)
{
    int count = 0;

    vector<future<int>> results(threads);

    int step = what.length() / threads;

    for (int i = 0; i < threads; i++)
        results[i] = async(Find_thread, what, s, threads, i * step);

    for (int i = 0; i < threads; i++)
        count += results[i].get();

    return count;
}


long long part_sum(string s)
{
    stringstream st;
    long long int sum = 0;
    int x;
    st << s; 
    while (st >> x) 
        sum = sum + x;
    return sum;
}

long long parallel_sum(string s, int threads)
{
    long long sum = 0;
    int step = s.length() / threads;
    vector<future<long long>> results(threads);
    string space = " "; string nothing = "";
    vector<string> sub_strings(threads);
    for (int i = 0; i < threads; i++)
        sub_strings[i] = nothing;
    int start = 0;
    int count = 0;
    for (int i = 0; i < threads; i++)
    {
        for (int j = start; j < (i + 1) * step; j++)
        {
            sub_strings[i] += s[j];
            count++;
        }

        if ((i + 1) * step != s.length() && s[(i + 1) * step] != space[0])
        {
            start = (i + 1) * step - 1;
            while (s[start] != space[0])
            {
                sub_strings[i].pop_back();
                count--;
                start--;
            }
            count--;
        }
        else
        {
            start = (i + 1) * step;
        }
    }

    if (count != s.length())
        for (int i = count; i < s.length(); i++)
            sub_strings[threads - 1] += s[i];

    for (int i = 0; i < threads; i++)
        cout << sub_strings[i] << endl;
    
    for (int i = 0; i < threads; i++)
        results[i] = async(part_sum, sub_strings[i]);

    for (int i = 0; i < threads; i++)
        sum += results[i].get();

    return sum;
}

int main()
{
    /*
    auto t1 = steady_clock::now();

    double p1 = pi_part_compution(0, 2500000.0);
    double p2 = pi_part_compution(2500000.0, 5000000.0);
    double p3 = pi_part_compution(5000000.0, 7500000.0);
    double p4 = pi_part_compution(7500000.0, 10000000.0);

    cout << p1 + p2 + p3 + p4 << endl;

    auto t2 = steady_clock::now();

    cout << duration_cast <milliseconds> (t2 - t1).count()
        << " ms " << endl;

    auto t3 = steady_clock::now();

    cout << pi_parallel_сompution(4, 10000000) << endl;

    auto t4 = steady_clock::now();

    cout << duration_cast <milliseconds> (t4 - t3).count()
        << " ms " << endl;
    */
    
    /*
    string st = "";
    for (int i = 0; i < 16; i++)
    {
        int x = rand() % 10;
        string s = to_string(x);
        st += s;
    }
    cout << stoull(st) % 17 << endl;
    cout << rest(st, 2, false) << endl;
    */
     
    /*
    cout << MultiThreadFind("aabbabaababbbaabrgdfababsadaababbab", "ab", 2) << endl;
    */

    /*
    string st = "";
    int count = 0;

    while (count <= 10000)
    {
        int x = rand() % 1000;
        string s = to_string(x);
        st += s;
        st += " ";
        count += s.length() + 1;
    }

    cout << parallel_sum(st, 2) << endl;
    */

    return 0;
}
