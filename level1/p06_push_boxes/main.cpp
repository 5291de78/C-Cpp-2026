#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <conio.h>
using namespace std;

// ============ 地图编码（每个格子存一个数字，用加减法原地更新状态） ============
//   0 空地    1 / 2 / 9 墙    3 目的地    4 箱子    5 人    7 箱子+目的地    8 人+目的地
// 移动技巧：人进入某格 +5（0->5、3->8），人离开 -5（5->0、8->3）；
//          箱子进入某格 +4（0->4、3->7），人走进箱子原格 +1（4->5、7->8）。
const int MAP_SIZE = 8;
int map[MAP_SIZE][MAP_SIZE];

// 从文件读取 8x8 地图（每个格子是一个 0~9 的数字，允许空格/换行分隔）
bool loadMap(const string& filename) {
    ifstream f(filename);
    if (!f) return false;
    char ch;
    for (int i = 0; i < MAP_SIZE; i++)
        for (int j = 0; j < MAP_SIZE; j++) {
            if (!(f >> ch)) return false;          // 数据不足
            map[i][j] = ch - '0';
            if (map[i][j] < 0 || map[i][j] > 9) return false;   // 非法字符
        }
    return true;
}

// 校验地图合法性：必须有且仅有 1 个玩家，箱子数 >= 1，且箱子数与目的地数相等，
// 否则该关不可能/不明确获胜，直接跳过（边界处理）
bool validateMap(const string& filename) {
    int players = 0, boxes = 0, goals = 0;
    for (int i = 0; i < MAP_SIZE; i++)
        for (int j = 0; j < MAP_SIZE; j++)
            switch (map[i][j]) {
                case 5: case 8: players++; break;   // 人 / 人+目的地
                case 4: boxes++;  break;            // 箱子
                case 7: boxes++;  goals++; break;   // 箱子+目的地（两者都计入）
                case 3: goals++;  break;            // 目的地
            }
    if (players != 1) {
        cout << filename << " 非法：玩家数量应为 1，实际为 " << players << "，跳过该关。" << endl;
        return false;
    }
    if (boxes == 0 || boxes != goals) {
        cout << filename << " 非法：箱子数(" << boxes << ")应与目的地数("
             << goals << ")相等且不为 0，跳过该关。" << endl;
        return false;
    }
    return true;
}

// 统计尚未归位的箱子数（裸露的 4），用于状态栏显示
int countLeftBoxes() {
    int cnt = 0;
    for (int i = 0; i < MAP_SIZE; i++)
        for (int j = 0; j < MAP_SIZE; j++)
            if (map[i][j] == 4) cnt++;
    return cnt;
}

// 显示：每格输出一个字符，列严格对齐
void GamePaint() {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
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

// 胜利条件：所有目的地都被箱子压住（不再有裸露的目的地 3 或 人站目的地 8）
bool GameWin() {
    for (int i = 0; i < MAP_SIZE; i++)
        for (int j = 0; j < MAP_SIZE; j++)
            if (map[i][j] == 3 || map[i][j] == 8) return false;
    return true;
}

// 找到玩家位置（5 或 8）
bool findPlayer(int& r, int& c) {
    for (r = 0; r < MAP_SIZE; r++)
        for (c = 0; c < MAP_SIZE; c++)
            if (map[r][c] == 5 || map[r][c] == 8) return true;
    return false;
}

// 判断某格是否可通行（空地 0 或 目的地 3），带边界保护
bool isPassable(int r, int c) {
    if (r < 0 || r >= MAP_SIZE || c < 0 || c >= MAP_SIZE) return false;
    return map[r][c] == 0 || map[r][c] == 3;
}

// 判断某格是否是箱子（普通 4 或 压在目的地 7），带边界保护
bool isBox(int r, int c) {
    if (r < 0 || r >= MAP_SIZE || c < 0 || c >= MAP_SIZE) return false;
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
    // 关卡文件按顺序依次挑战（从文件中读取地图，满足"多关卡"要求）
    vector<string> levels = {"level1.txt", "level2.txt", "level3.txt"};
    ofstream scoreFile("score.txt", ios::app);   // 分数以追加方式记录到文件（满足"存档"要求）
    if (!scoreFile)
        cout << "警告：无法打开 score.txt，本局分数将不会被保存。" << endl;

    for (size_t lv = 0; lv < levels.size(); lv++) {
        // 读图 + 校验，任一失败都跳过该关
        if (!loadMap(levels[lv]) || !validateMap(levels[lv])) {
            cout << "读取 " << levels[lv] << " 失败或地图非法，跳过该关卡。" << endl;
            system("pause");
            continue;
        }

        int steps = 0;
        system("cls");
        cout << "===== 第 " << (lv + 1) << " 关 =====" << endl;

        while (true) {
            GamePaint();
            cout << "  [W/A/S/D]移动   [R]重开本关   [Q]退出   "
                 << "步数: " << steps
                 << "   未归位箱子: " << countLeftBoxes() << endl;
            if (GameWin()) break;

            char key = getch();          // 读键盘，无需回车
            if (key == 'q' || key == 'Q') {
                system("cls");
                cout << "已退出游戏，本关未完成。" << endl;
                scoreFile.close();
                return 0;
            }
            if (key == 'r' || key == 'R') {          // 重开本关：重新读档、步数清零
                loadMap(levels[lv]);
                steps = 0;
                system("cls");
                continue;
            }

            bool moved = false;
            switch (key) {
                case 'w': case 'W': moved = tryMove(-1, 0); break;
                case 's': case 'S': moved = tryMove(1, 0);  break;
                case 'a': case 'A': moved = tryMove(0, -1); break;
                case 'd': case 'D': moved = tryMove(0, 1);  break;
            }
            if (moved) steps++;          // 只有真正移动才计步（撞墙不计步）
            system("cls");
        }

        // 过关：步数越少分数越高
        int score = 1000 - steps * 10;
        if (score < 0) score = 0;

        system("cls");
        cout << "===== 第 " << (lv + 1) << " 关通过！=====" << endl;
        cout << "本关步数: " << steps << "，本关得分: " << score << endl;
        if (scoreFile)                           // 每关分数写入存档文件
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
