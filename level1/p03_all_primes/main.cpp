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