#include <iostream>
#include <chrono>
#include "miller.h"
using namespace std;

int main()
{
    cout << "=========================\n"
            "Welcome to the prime number checker!\n"
            "Supports positive integers up to 18446744073709551615 (20 digits).\n"
            "=========================" << endl;

    while (true)
    {
        config();                       // 清空上一轮的全局状态
        if (!greeting())                // 读取并校验输入
        {
            if (cin.eof()) break;       // 读到 EOF：正常结束程序
            continue;                   // 输入非法：提示后进入下一轮
        }

        auto start = chrono::high_resolution_clock::now();
        is_prime();                     // 分流到试除法 / Miller-Rabin
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "Time taken: " << elapsed.count() << " ms" << endl;
    }
    return 0;
}
