#include <iostream>
#include <chrono>
#include <vector>
using namespace std;

// 功能：打印 2~1000 以内的所有素数，并输出总计算时间。
// 算法：欧拉（线性）筛，每个合数只被它的最小质因子筛掉一次，时间复杂度 O(n)。

const int n = 1000;
vector<bool> is_prime(n + 1, true);   // true 表示当前仍认为是素数
vector<int> primes;                    // 收集已找到的素数

int main() {
    auto start = chrono::high_resolution_clock::now();   // 开始计时（只统计计算部分）

    is_prime[0] = false;   // 0 不是素数
    is_prime[1] = false;   // 1 不是素数

    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) primes.push_back(i);   // i 是素数，收集

        // 用已找到的素数去筛合数，保证每个合数只被最小质因子筛掉一次
        for (int p : primes) {
            if (i * p > n) break;       // 超出范围
            is_prime[i * p] = false;    // i*p 是合数
            if (i % p == 0) break;      // p 是 i 的最小质因子，防止重复筛
        }
    }

    auto end = chrono::high_resolution_clock::now();   // 结束计时（打印耗时不计入）

    // 打印所有素数
    for (int p : primes) { cout << p << " "; }
    cout << '\n';

    chrono::duration<double, milli> elapsed = end - start;
    cout << "共 " << primes.size() << " 个素数，Time taken: " << elapsed.count() << " ms" << endl;
    return 0;
}
