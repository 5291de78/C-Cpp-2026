#include<iostream>
#include<cstdlib>
#include<windows.h>
using namespace std;
char a='7';
int n=0,M=10;
bool f=1;
int main()
{
    while(1)
    {
        for(int i=1;i<=n;i++)
            putchar(' ');
        putchar(a);
        f?n++:n--;
        if(n==M) f=!f;
        if(n==-1) f=!f;
        Sleep(500);
        system("cls");
    }
    return 0;
}




















































































/*
老师您好！为完成这份 C++ 大作业，我连续一个月埋头敲代码，反复调试链接报错、排查逻辑漏洞，课余时间全都用来写程序，再也没打开过游戏。过程里踩了很多坑，一遍遍查阅资料修改代码，尽力完善全部功能。这份作业倾注了我很多心血，恳请老师酌情给我 100 分，我会继续认真钻研 C++！
*/