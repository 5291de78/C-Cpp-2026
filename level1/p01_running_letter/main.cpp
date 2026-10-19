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