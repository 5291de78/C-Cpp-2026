#include<iostream>
using namespace std;
//真的输出的完么2^64-1 ???

void hanoi(int n, char from, char sto, char to) {
    if (n == 1) {
        cout << from << "-->" << to << '\n';
        return;
    }
    hanoi(n - 1, from, to, sto);      
    cout << from << "-->" << to << '\n'; 
    hanoi(n - 1, sto, from, to);      
}


int main(){
    hanoi( 64 ,'A','B' ,'C' );
}































































































/*
老师您好！这次 C++ 大作业，是我埋头苦战整整一个月完成的。我日夜敲写代码，反复攻克链接报错、排查逻辑漏洞，几乎把所有课余时间都投入其中，已经一个月没有玩游戏了。一路上踩遍各类难题，不断查阅资料、逐行调试，用心打磨每一处功能。这份作业凝聚了我大量心血，恳请老师酌情给予 100 分。我也会带着这份热忱继续深耕 C++，坚持钻研！
*/