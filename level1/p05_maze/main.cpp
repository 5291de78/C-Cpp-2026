#include<iostream>
#include<windows.h>
#pragma comment(lib, "User32.lib")
#include<conio.h>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
#include<random>
#include<ctime>
#include<limits>
using namespace std;

//https://blog.csdn.net/imred/article/details/105329806
int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}


int size_c = 0;

//============显示+============================

class screen{
    private:
        int cols = 0;
        int lines = 0;
    public:
        static void config_();
        static void greetingg();
        static void greeting_();
};

void screen::config_(){
    SetProcessDPIAware();
    DEVMODE dm{};
    dm.dmSize = sizeof(DEVMODE);
    if (EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm))
    {
        int w = dm.dmPelsWidth;
        int h = dm.dmPelsHeight;
        int g = gcd(w, h);
        int ratioW = w / g;
        int ratioH = h / g;
        int cols = size_c *3* ratioW;
        int lines = size_c *3* ratioH;
        string DOS = "mode con cols=" + to_string(cols) + " lines=" + to_string(lines);
        system(DOS.c_str());
    }
};
void screen::greetingg(){
    cout<<string(50 , '=')<<string(2 , '\n');
    cout<<"Welcome ! This is a maze game"<<string(2 , '\n');
    cout<<"Rules:"<<'\n';
    cout<<"1,press W,A,S,D to move as char e\n";
    cout<<"2,find $ in limited time to score\n";
    cout<<"3,press E to check the correct route\n";
    cout<<"4,press X to end game and check your final score\n";
    cout<<string(2 , '\n');
    cout<<"Last but not the least:HAPPY BRAINSTORMING:)"<<string(3 , '\n');
};
void screen::greeting_(){
    greetingg();
    cout<<"ENTER your expected difficulty level(1~10) and START GAME\n";
    cout<<'>';
    while(true){
        if (cin >> size_c && size_c >= 1 && size_c <= 10) break;
        system("cls");
        greetingg();
        cout << "Invalid input!pls enter 1~10 to start game\n>";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
};

//============数据结构============================

class AdjacencyList
{
public:
    AdjacencyList(int row = 0, int column = 0);

    void connect(int i, int j);
    void disconnect(int i, int j);

    int row() const { return m_row; }
    int column() const { return m_column; }

    std::vector<int> &neighbor(int i);
    std::vector<int> &surround(int i);
    const std::vector<int> &neighbor(int i) const;
    const std::vector<int> &surround(int i) const;

private:
    int m_row;
    int m_column;

    std::vector<std::vector<int>> m_index2neighbor;
    std::vector<std::vector<int>> m_index2surround;
};

AdjacencyList::AdjacencyList(int row, int column)
    : m_row(row), m_column(column),
      m_index2neighbor((size_t)row * column),
      m_index2surround((size_t)row * column)
{
    auto id = [&](int r, int c) { return r * m_column + c; };
    for (int r = 0; r < row; r++)
        for (int c = 0; c < column; c++) {
            if (r > 0)          m_index2surround[id(r,c)].push_back(id(r-1,c));
            if (r < row - 1)    m_index2surround[id(r,c)].push_back(id(r+1,c));
            if (c > 0)          m_index2surround[id(r,c)].push_back(id(r,c-1));
            if (c < column - 1) m_index2surround[id(r,c)].push_back(id(r,c+1));
        }
}

void AdjacencyList::connect(int i, int j)
{
    m_index2neighbor[i].push_back(j);
    m_index2neighbor[j].push_back(i);
}
void AdjacencyList::disconnect(int i, int j)
{
    auto& a = m_index2neighbor[i];
    a.erase(remove(a.begin(), a.end(), j), a.end());
    auto& b = m_index2neighbor[j];
    b.erase(remove(b.begin(), b.end(), i), b.end());
}
std::vector<int>& AdjacencyList::neighbor(int i) { return m_index2neighbor[i]; }
std::vector<int>& AdjacencyList::surround(int i) { return m_index2surround[i]; }
const std::vector<int>& AdjacencyList::neighbor(int i) const { return m_index2neighbor[i]; }
const std::vector<int>& AdjacencyList::surround(int i) const { return m_index2surround[i]; }

//============生成器============================

class DeepFirstSearch
{
public:
    DeepFirstSearch(int row, int column) : m_row(row), m_column(column) {}
    AdjacencyList generate();
private:
    int m_row, m_column;
};

AdjacencyList DeepFirstSearch::generate()
{
    enum Color { White, Gray, Black };

    AdjacencyList result(m_row, m_column);
    vector<int> color((size_t)m_row * m_column, White);
    vector<int> current;
    current.reserve((size_t)m_row * m_column);

    std::mt19937 rng(std::random_device{}());          
    color[0] = Gray;
    current.push_back(0);

    while (current.size())
    {
        int last = current.back();
        std::shuffle(result.surround(last).begin(), result.surround(last).end(), rng);

        auto iter = result.surround(last).cbegin();
        for (; iter != result.surround(last).cend(); iter++)
        {
            if (color[(size_t)*iter] == White)
            {
                color[(size_t)*iter] = Gray;
                result.connect(last, *iter);
                current.push_back(*iter);
                break;
            }
        }

        if (iter == result.surround(last).cend())
        {
            current.pop_back();
            color[(size_t)last] = Black;
        }
    }
    return result;
}

//============工具函数============================

int g_id(int r, int c, int C) { return r * C + c; }

// 两格之间是否连通
bool linked(const AdjacencyList& g, int a, int b) {
    for (int x : g.neighbor(a)) if (x == b) return true;
    return false;
}

// BFS 求从 start 到 goal 的路线（E 键功能），返回格子编号序列
vector<int> bfsPath(const AdjacencyList& g, int start, int goal) {
    int n = g.row() * g.column();
    vector<int> prev((size_t)n, -1);
    queue<int> q;
    q.push(start); prev[start] = start;
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        if (cur == goal) break;
        for (int nxt : g.neighbor(cur))
            if (prev[nxt] == -1) { prev[nxt] = cur; q.push(nxt); }
    }
    vector<int> path;
    if (prev[goal] == -1) return path;
    for (int p = goal; p != start; p = prev[p]) path.push_back(p);
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

// 读一个按键：
// 输入被重定向时退回 cin，读到文件尾视为按 X 退出
char readKey() {
    DWORD mode;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn != INVALID_HANDLE_VALUE && GetConsoleMode(hIn, &mode))
        return (char)_getch();
    char c;
    return (cin.get(c)) ? c : 'X';
}

//============游戏层============================

class maze {
private:
    AdjacencyList g;         // 迷宫地图（邻接表）
    int R, C;                // 行、列
    int pr, pc;              // 玩家(e)位置
    int tr, tc;              // 宝藏($)位置
    time_t t0;               // 开始时间
    int score;               // 得分
    int limit;               // 时间限制(秒)
    std::mt19937 rng;

    int leftTime() const {
        int used = (int)(time(nullptr) - t0);
        return limit - used;
    }

public:
    maze(int difficulty) : rng(std::random_device{}()) {
        R = difficulty + 2;           // 难度决定迷宫大小
        C = difficulty * 2 + 2;
        DeepFirstSearch gen(R, C);
        g = gen.generate();
        pr = pc = 0;                  // 玩家从左上出发
        tr = R - 1; tc = C - 1;       // $ 在右下角
        t0 = time(nullptr);
        score = 0;
        limit = 60 + difficulty * 15; // 难度越高时间越多，奖励也更高
    }

    bool timeUp() const { return leftTime() <= 0; }

    // 按方向移动：目标格在 neighbor 列表里(没墙)才走
    void move(char key) {
        int nr = pr, nc = pc;
        if (key == 'W' || key == 'w') nr--;
        else if (key == 'S' || key == 's') nr++;
        else if (key == 'A' || key == 'a') nc--;
        else if (key == 'D' || key == 'd') nc++;
        else return;
        if (nr < 0 || nr >= R || nc < 0 || nc >= C) return;                 // 越界
        if (!linked(g, g_id(pr, pc, C), g_id(nr, nc, C))) return;           // 有墙
        pr = nr; pc = nc;
        checkTreasure();
    }

    // 吃到 $：得分 = 剩余秒数 * 难度，把 $ 随机挪到别处
    void checkTreasure() {
        if (pr == tr && pc == tc) {
            score += leftTime() * (C / 2);   // C/2 == 难度
            uniform_int_distribution<int> rr(0, R - 1), rc(0, C - 1);
            do { tr = rr(rng); tc = rc(rng); }
            while (tr == pr && tc == pc);
        }
    }

    // 画一帧：showPath=true 时叠加 BFS 正确路线
    void draw(bool showPath) const {
        system("cls");
        cout << "Time: " << leftTime() << "s   Score: " << score
             << "   [WASD]=move  [E]=route  [X]=exit\n";

        vector<bool> onPath((size_t)R * C, false);
        if (showPath)
            for (int p : bfsPath(g, g_id(pr, pc, C), g_id(tr, tc, C)))
                onPath[(size_t)p] = true;

        auto cellCh = [&](int r, int c) -> char {
            if (r == pr && c == pc) return 'e';
            if (r == tr && c == tc) return '$';
            if (onPath[(size_t)g_id(r, c, C)]) return '*';
            return ' ';
        };

        cout << '+';
        for (int c = 0; c < C; c++) cout << "---+";
        cout << '\n';
        for (int r = 0; r < R; r++) {
            cout << '|';
            for (int c = 0; c < C; c++) {
                cout << ' ' << cellCh(r, c) << ' ';
                cout << (c < C - 1 && !linked(g, g_id(r,c,C), g_id(r,c+1,C)) ? '|' : ' ');
            }
            cout << '\n';
            if (r < R - 1) {
                cout << '+';
                for (int c = 0; c < C; c++)
                    cout << (!linked(g, g_id(r,c,C), g_id(r+1,c,C)) ? "---+" : "   +");
                cout << '\n';
            }
        }
        cout << '+';
        for (int c = 0; c < C; c++) cout << "---+";
        cout << '\n';
    }

    void gameOver(const string& reason) const {
        draw(false);
        cout << reason << "\nFINAL SCORE: " << score << '\n';
    }
};

//============主函数============================

int main() {
    screen::greeting_();     // 1. 欢迎 
    screen::config_();       // 2. 难度
    system("cls");

    maze game(size_c);       // 3. 生成迷宫

    while (true) {
        game.draw(false);                    // 4. 每帧重画
        if (game.timeUp()) {                 // 5. 超时判负
            game.gameOver("TIME UP!");
            break;
        }
        char key = readKey();                
        if (key == 'X' || key == 'x') {      
            game.gameOver("Game ended by player.");
            break;
        }
        if (key == 'E' || key == 'e') {      
            game.draw(true);
            readKey();
            continue;
        }
        game.move(key);                      // 9. 移动
    }
    return 0;
}
