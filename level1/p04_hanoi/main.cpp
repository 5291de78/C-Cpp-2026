#include <iostream>
using namespace std;

// 输出一次移动：把盘子从 from 移到 to
void move_disk(char from, char to) {
    cout << from << " --> " << to << '\n';
}

// 汉诺塔递归：把 n 个盘子从柱子 from，借助柱子 sto，移动到柱子 to
void hanoi(int n, char from, char sto, char to) {
    if (n == 1) {                        // 只剩一个盘子，直接移动
        move_disk(from, to);
        return;
    }
    hanoi(n - 1, from, to, sto);         // 先把上面 n-1 个移到辅助柱
    move_disk(from, to);                 // 再把最大的移过去
    hanoi(n - 1, sto, from, to);         // 最后把 n-1 个移到目标柱
}

int main() {
    // 用 3 个盘子演示（共 2^3 - 1 = 7 步，输出可完整打印）
    cout << "3 个盘子的移动步骤：\n";
    hanoi(3, 'A', 'B', 'C');

    // 传说中的 64 个盘子：步数是 2^64 - 1（约 1.8e19），是天文数字，
    // 无法在有限时间内逐一打印，这里只给出理论步数。
    cout << "\n64 个盘子共需 " << 18446744073709551615ULL
         << " 步（2^64 - 1），无法逐一打印。\n";
    return 0;
}
