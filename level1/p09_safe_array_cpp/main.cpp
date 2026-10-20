#include <iostream>
#include <limits>
using namespace std;

int main()
{
    int num;
    cout << "请输入整数：";
    while (!(cin >> num))  // 如果读取失败进入循环
    {
        cin.clear(); // 清除错误标记
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //清缓冲区
        cout << "输入不是整数！重新输入：";
    }
    cout << "成功读取：" << num << endl;
    return 0;
}
