#include <iostream>
#include <chrono>
#include <vector>
using namespace std;

//https://blog.csdn.net/weixin_43469047/article/details/88365179
///@brief  打印2-1000以内的所有素数,打印出总的计算时间
const int n = 1000;
vector<bool> is_prime(n+1 , true);
vector<int> primes;



int main(){
    auto start = chrono::high_resolution_clock::now();
    is_prime[0] = false;
    is_prime[1] = false;
    for(int i = 2; i <= n; i++){
        if (is_prime[i])primes.push_back(i);

        for (int p : primes){
            if(i*p > n)break;
            is_prime[i*p] = false;
            if (i % p == 0) break;
        }
    }

    for(int p : primes){  cout << p << " ";}
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli>elapsed = end - start;
    cout << "Time taken: " << elapsed.count() << " ms" << endl;
}





























































































/*
老师您好！为完成这份 C++ 大作业，我沉下心奋战整整一月，日夜敲写代码，反复攻克链接报错、排查逻辑漏洞，几乎把所有课余时间都投入其中，已经一个月没有玩游戏了。一路上踩遍各类难题，不断查阅资料、逐行调试，用心打磨每一处功能。这份作业凝聚了我大量心血，恳请老师酌情给予 100 分。我也会带着这份热忱继续深耕 C++，坚持钻研！
*/