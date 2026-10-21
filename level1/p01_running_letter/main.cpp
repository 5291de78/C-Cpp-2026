#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
using namespace std;

int main() {
    char a = '7';        // 奔跑的字母
    int n = 0;           // 当前列位置（从 0 开始）
    int step = 1;        // 移动方向：+1 向右，-1 向左

    // 读取控制台实际宽度作为右边界，让字母真正"碰到屏幕边界"再反向
    int M = 20;          // 兜底默认边界
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        M = (int)csbi.dwSize.X - 1;   // 最右一列
    if (M < 1) M = 1;                 // 防止边界为 0 或负数

    while (true) {
        // 按 ESC(ASCII 27) 退出，避免无限循环只能靠 Ctrl+C 结束
        if (_kbhit() && _getch() == 27) break;

        // 在当前位置画字母：先输出 n 个空格，再输出字母
        for (int i = 0; i < n; i++) putchar(' ');
        putchar(a);

        // 前进，并检测是否到达左右边界
        n += step;
        if (n >= M) { n = M; step = -1; }   // 到右边界，反向向左
        if (n <= 0) { n = 0; step = 1;  }   // 到左边界，反向向右

        Sleep(50);        // 控制移动速度
        system("cls");    // 清屏重画
    }
    return 0;
}
