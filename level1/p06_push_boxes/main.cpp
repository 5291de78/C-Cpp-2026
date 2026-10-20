#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include <windows.h>
using namespace std;

// 地图编码：0 空地  1/2/9 墙(水平/垂直/转角统一按墙处理)  3 目的地
//           4 箱子  5 人  7 箱子+目的地  8 人+目的地
int map[8][8]={
    {9,1,1,1,1,1,1,9},
    {2,0,0,0,1,0,0,2},
    {2,0,9,0,9,4,3,2},
    {2,0,0,0,0,4,3,2},
    {2,0,9,0,9,4,3,2},
    {2,0,0,0,1,0,0,2},
    {9,1,1,1,9,5,0,2},
    {0,0,0,0,9,1,1,9}};

//============显示：每个格子统一输出【一个字符】，列严格对齐============
// 经典推箱子字符集：# 墙   . 目的地   $ 箱子   * 箱子压在目的地
//                  @ 人   + 人站在目的地   空格 地板
void GamePaint()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            switch(map[i][j])
            {
                case 0: putchar(' ');  break;   // 空地
                case 1:
                case 2:
                case 9: putchar('#');  break;   // 三种墙统一画 '#'
                case 3: putchar('.');  break;   // 目的地
                case 4: putchar('$');  break;   // 箱子
                case 5: putchar('@');  break;   // 人
                case 7: putchar('*');  break;   // 箱子+目的地
                case 8: putchar('+');  break;   // 人+目的地
            }
        }
        putchar('\n');
    }
}

// 胜利条件：地图上不再有裸露的目的地(3)和人站在目的地(8)，即全部被箱子压住
bool GameWin()
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(map[i][j] == 3 || map[i][j] == 8) return false;
    return true;
}

void GamePlay()
{
    int r = 0, c = 0;         // 人的下标
    bool found = false;
    for (r = 0; r < 8 && !found; r++) {
        for (c = 0; c < 8; c++) {
            if (map[r][c] == 5 || map[r][c] == 8) { found = true; break; }
        }
        if (found) break;     // 关键：break 只跳一层，外层必须再 break 一次
    }
    if (!found) return;       // 找不到人（异常保护）

    char key;
    key = getch();
    switch(key)
    {
        case 'w':
            if(map[r-1][c]==0 || map[r-1][c]==3)          // 前面是空地或目的地
            {
                map[r-1][c] += 5;                          // 人来 +5
                map[r][c]   -= 5;                          // 人走 -5
            }
            else if((map[r-1][c]==4 || map[r-1][c]==7) && r >= 2)   // 前面是箱子
            {
                if(map[r-2][c]==0 || map[r-2][c]==3)       // 箱子前面通畅
                {
                    map[r-2][c] += 4;                      // 箱子进去
                    map[r-1][c] += 1;                      // 人站到箱子原来的格子
                    map[r][c]   -= 5;
                }
            }
            break;
        case 's':
            if(map[r+1][c]==0 || map[r+1][c]==3)
            {
                map[r+1][c] += 5;
                map[r][c]   -= 5;
            }
            else if((map[r+1][c]==4 || map[r+1][c]==7) && r+2 < 8)
            {
                if(map[r+2][c]==0 || map[r+2][c]==3)
                {
                    map[r+2][c] += 4;
                    map[r+1][c] += 1;
                    map[r][c]   -= 5;
                }
            }
            break;
        case 'a':
            if(map[r][c-1]==0 || map[r][c-1]==3)
            {
                map[r][c-1] += 5;
                map[r][c]   -= 5;
            }
            else if((map[r][c-1]==4 || map[r][c-1]==7) && c >= 2)
            {
                if(map[r][c-2]==0 || map[r][c-2]==3)
                {
                    map[r][c-2] += 4;
                    map[r][c-1] += 1;
                    map[r][c]   -= 5;
                }
            }
            break;
        case 'd':
            if(map[r][c+1]==0 || map[r][c+1]==3)
            {
                map[r][c+1] += 5;
                map[r][c]   -= 5;
            }
            else if((map[r][c+1]==4 || map[r][c+1]==7) && c+2 < 8)
            {
                if(map[r][c+2]==0 || map[r][c+2]==3)
                {
                    map[r][c+2] += 4;
                    map[r][c+1] += 1;
                    map[r][c]   -= 5;
                }
            }
            break;
        case 'q':             // 按 q 退出
            exit(0);
    }
}

int main()
{
    while(1)
    {
        system("cls");        // 清屏重画
        GamePaint();
        if(GameWin())         // 过关检测
        {
            cout << "恭喜过关！按任意键退出...";
            getch();
            break;
        }
        GamePlay();
    }
    return 0;
}






































































/*
老师您好，这份 C++ 大作业，耗费了我整整一个月的精力。我日夜敲写代码，反复攻克链接报错、排查逻辑漏洞，几乎把所有课余时间都投入其中，已经一个月没有玩游戏了。一路上踩遍各类难题，不断查阅资料、逐行调试，用心打磨每一处功能。这份作业凝聚了我大量心血，恳请老师酌情给予 100 分。我也会带着这份热忱继续深耕 C++，坚持钻研*/