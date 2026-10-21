#include "miller.h"
#include <cstdint>
#include <cctype>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
using namespace std;

string input = "";
long long smaller_num = 0;
unsigned long long big_num = 0;

void config()
{
    input.clear();
    smaller_num = 0;
    big_num = 0;
}


bool greeting()
{
    cout << "pls enter a positive integer number: ";
    if (cin >> input)
    {
        // 逐个字符检查是否为数字，避免 stoll 解析非数字时抛异常崩溃
        for (char c : input)
        {
            if (!isdigit(c))
            {
                cout << "error:input is not a positive integer ):" << endl;
                return false;
            }
        }
        return true;   // 输入合法
    }
    else
    {
        cout << "error:cin failed ):" << endl;
        return false;  // 读到 EOF 或输入流错误
    }
}

//函数太复杂拿出来写
void Miller_Rabin(unsigned long long n)
{   
    unsigned long long d = n - 1; //一定是偶数
    int s = 0;
    while(d%2 == 0){
        d /= 2 ;
        s += 1;
    }
    unsigned long long base_list[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for ( int i = 0; i < 12 ; i++)
    {
        unsigned long long a = base_list[i];
        if (a >= n) continue;
        unsigned long long x = qpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        for (int r = 1; r < s; r++)
        {
            x = qmul(x, x, n);
            if (x == n - 1) break;
        }
        if (x != n - 1)
        {
            cout << "The number is composite." << endl;
            return;
        }
    }
    cout << "The number is a prime." << endl;
    
}


//a^b (mod n) 快速幂
unsigned long long qpow(unsigned long long a, unsigned long long b, unsigned long long n)
{
    unsigned long long res = 1;
    a %= n;
    while (b > 0)
    {
        if (b & 1) res = qmul(res, a, n);
        a = qmul(a, a, n);
        b >>= 1;
    }
    return res;
}

unsigned long long qmul(unsigned long long a, unsigned long long b, unsigned long long n)
{
    unsigned long long res = 0;
    a %= n;
    while (b > 0)
    {
        if (b & 1) res = (res + a) % n;
        a = (a + a) % n;
        b >>= 1;
    }
    return res;
}

void is_prime()
{   
    //位数很小呢？
    if(input.length() == 1)
    {
        smaller_num = stoll(input); 
        switch (smaller_num)
        {
        case 2:
        case 3:
        case 5:
        case 7:
            cout << "The number is a prime." << endl;
            return;
        default:
            cout << "The number is not a prime." << endl;
            return;
        }
    //先看看是不是偶数
    }else if((input.at(input.length()-1) - '0')%2 == 0){   
        cout<<"The number is not a prime."<<endl;
        return;
    }
    //如果不算很大呢？
    else if(input.length() <= 10)
    {   
        smaller_num = stoll(input);
        int adx = 2;
        while(smaller_num % adx != 0 && (long long)adx * adx <= smaller_num)
        {
            adx++;
        }
        if(smaller_num % adx == 0){

            cout << "The number is composite." << endl;
        }
        else
        {
            cout << "The number is a prime." << endl;   
        }
        return;
    }
    //如果很大呢
    else if(input.length() <= 19)
    {
        big_num = stoull(input);
        Miller_Rabin(big_num);
        return;
    }

    //要是太大了呢 Miller-Rabin?
    else
    {
        cout << "The number is too large to check." << endl;
    }
}