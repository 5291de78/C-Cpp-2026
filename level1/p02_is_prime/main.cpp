#include <iostream>
#include <chrono>
#include "miller.h"
using namespace std;


int main()
{   
    cout<<"=========================\nWelcome to the prime number checker!\nThis checker supports detecting positive integers with up to 19 digits.\n ========================="<<endl;
    while(true){
        config();
        if (!greeting()) {          // 输入非法则跳过本次检测
            if (cin.eof()) break;   // 读到 EOF 则退出循环
            continue;               // 非数字输入则继续下一轮
        }
        auto start = chrono::high_resolution_clock::now();
        is_prime();
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli>elapsed = end - start;
        cout << "Time taken: " << elapsed.count() << " ms" << endl;
    };

}
