#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <conio.h>
using namespace std;

// 地图编码：
//   0 空地   1/2/9 墙   3 目的地   4 箱子   5 人   7 箱子+目的地   8 人+目的地
// 移动数值技巧：人进入 +5，人离开 -5，箱子进入 +4，箱子位移到人 +1
int map[8][8];

// 从文件读取 8x8 地图（每个格子是一个 0~9 的数字，允许空格/换行分隔）
bool loadMap(const string& filename) {
    ifstream f(filename);
    if (!f) return false;
    char ch;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (!(f >> ch)) return false;      // 数据不足
            map[i][j] = ch - '0';
            if (map[i][j] < 0 || map[i][j] > 9) return false;
        }
    return true;
}

// 显示：每格输出一个字符，列严格对齐
void GamePaint() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch (map[i][j]) {
                case 0: putchar(' '); break;                 // 空地
                case 1: case 2: case 9: putchar('#'); break; // 墙
                case 3: putchar('.'); break;                 // 目的地
                case 4: putchar('$'); break;                 // 箱子
                case 5: putchar('@'); break;                 // 人
                case 7: putchar('*'); break;                 // 箱子+目的地
                case 8: putchar('+'); break;                 // 人+目的地
            }
        }
        putchar('\n');
    }
}

// 胜利条件：所有箱子都压在目的地上（不再有裸露的目的地 3 或 人站目的地 8）
bool GameWin() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (map[i][j] == 3 || map[i][j] == 8) return false;
    return true;
}

// 找到玩家位置
bool findPlayer(int& r, int& c) {
    for (r = 0; r < 8; r++)
        for (c = 0; c < 8; c++)
            if (map[r][c] == 5 || map[r][c] == 8) return true;
    return false;
}

// 判断某格是否可通行（空地 0 或 目的地 3），带边界保护
bool isPassable(int r, int c) {
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;
    return map[r][c] == 0 || map[r][c] == 3;
}

// 判断某格是否是箱子（普通 4 或 压在目的地 7），带边界保护
bool isBox(int r, int c) {
    if (r < 0 || r >= 8 || c < 0 || c >= 8) return false;
    return map[r][c] == 4 || map[r][c] == 7;
}

// 朝 (dr, dc) 方向尝试移动一次，返回是否真正发生了移动（用于计步）
bool tryMove(int dr, int dc) {
    int r, c;
    if (!findPlayer(r, c)) return false;
    int nr = r + dr, nc = c + dc;        // 人要走到的格子

    if (isPassable(nr, nc)) {            // 前面是空地或目的地
        map[nr][nc] += 5;                // 人进入
        map[r][c]  -= 5;                 // 人离开
        return true;
    }
    if (isBox(nr, nc)) {                 // 前面是箱子
        int br = nr + dr, bc = nc + dc;  // 箱子要推到的格子
        if (isPassable(br, bc)) {        // 箱子前方通畅
            map[br][bc] += 4;            // 箱子进入（0->4 或 3->7）
            map[nr][nc] += 1;            // 箱子原格变人（4->5 或 7->8）
            map[r][c]  -= 5;             // 人离开
            return true;
        }
    }
    return false;                        // 撞墙 / 推不动，不动
}

int main() {
    // 关卡文件按顺序依次挑战
    vector<string> levels = {"level1.txt", "level2.txt", "level3.txt"};
    ofstream scoreFile("score.txt", ios::app);   // 分数追加记录

    for (size_t lv = 0; lv < levels.size(); lv++) {
        if (!loadMap(levels[lv])) {
            cout << "读取 " << levels[lv] << " 失败，跳过该关卡。" << endl;
            continue;
        }

        int steps = 0;
        system("cls");
        cout << "===== 第 " << (lv + 1) << " 关 =====" << endl;

        while (true) {
            GamePaint();
            cout << "  [W/A/S/D]移动   [Q]退出   步数: " << steps << endl;
            if (GameWin()) break;

            char key = getch();          // 读键盘，无需回车
            if (key == 'q' || key == 'Q') {
                system("cls");
                cout << "已退出游戏，本关未完成。" << endl;
                scoreFile.close();
                return 0;
            }

            bool moved = false;
            switch (key) {
                case 'w': case 'W': moved = tryMove(-1, 0); break;
                case 's': case 'S': moved = tryMove(1, 0);  break;
                case 'a': case 'A': moved = tryMove(0, -1); break;
                case 'd': case 'D': moved = tryMove(0, 1);  break;
            }
            if (moved) steps++;          // 只有真正移动才计步
            system("cls");
        }

        // 过关：步数越少分数越高
        int score = 1000 - steps * 10;
        if (score < 0) score = 0;

        system("cls");
        cout << "===== 第 " << (lv + 1) << " 关通过！=====" << endl;
        cout << "本关步数: " << steps << "，本关得分: " << score << endl;
        if (scoreFile)
            scoreFile << "Level " << (lv + 1) << ": " << score
                      << " 分 (步数 " << steps << ")" << endl;
        cout << "按任意键进入下一关...";
        getch();
    }

    scoreFile.close();
    system("cls");
    cout << "全部关卡完成！分数已保存到 score.txt" << endl;
    return 0;
}
