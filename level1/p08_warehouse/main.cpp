// 简单进销存 - p08_warehouse
// 功能：数字菜单选择；显示存货列表 / 入库 / 出库 / 修改 / 删除 / 退出
//       启动时从 warehouse.txt 读取库存，退出时保存
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <limits>
#include <windows.h>
using namespace std;


class warehouse {
private:
    vector<string> Items;
    vector<unsigned int> Quantity;

public:
    size_t size() const { return Items.size(); }
    static bool match_func(const string& a, const string& b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++) {
            if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
                return false;
        }
        return true;
    }
    int findIndex(const string& name) const {
        for (size_t i = 0; i < Items.size(); i++)
            if (match_func(Items[i], name)) return (int)i;
        return -1;
    }
    const string& itemAt(size_t i) const { return Items[i]; }
    unsigned int qtyAt(size_t i) const { return Quantity[i]; }
    bool load_(const string& filename = "warehouse.txt") {
        ifstream infile(filename);
        if (!infile) return false;                  // 文件不存在（首次运行），不算损坏
        string line;
        int lineNo = 0, badLines = 0;
        while (getline(infile, line)) {
            lineNo++;
            if (lineNo == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
                line = line.substr(3);      // 去掉 UTF-8 BOM（记事本手工编辑可能产生）
            bool bad = false;
            // 行格式：型号 数量。型号允许含空格，数量固定为行尾最后一个字段
            size_t pos = line.find_last_of(" \t");
            if (pos == string::npos) bad = true;    // 没有数量字段
            string name, numStr;
            if (!bad) {
                name = line.substr(0, pos);
                numStr = line.substr(pos + 1);
                if (name.empty() || numStr.empty()) bad = true;
            }
            // 数量必须是纯数字且不超范围（保护手工改坏的文件）
            if (!bad)
                for (char ch : numStr)
                    if (!isdigit((unsigned char)ch)) { bad = true; break; }
            if (!bad) {
                try {
                    size_t used = 0;
                    unsigned long long v = stoull(numStr, &used);
                    if (used != numStr.size() ||
                        v > numeric_limits<unsigned int>::max()) bad = true;
                    else {
                        Items.push_back(name);
                        Quantity.push_back((unsigned int)v);
                    }
                } catch (...) { bad = true; }
            }
            if (bad) {                              // 损坏行必须明确提示，不能静默跳过
                cout << "  ! 存档损坏：第 " << lineNo << " 行无法解析，已跳过 -> \""
                     << line << "\"\n";
                badLines++;
            }
        }
        if (infile.bad()) {
            cout << "  ! 存档损坏：读取 " << filename << " 时发生 I/O 错误\n";
            badLines++;
        }
        if (badLines > 0)
            cout << "  ! 共 " << badLines << " 行损坏数据未载入，请检查 "
                 << filename << " 是否被改坏\n";
        return true;
    }
    bool save_func(const string& filename = "warehouse.txt") const {
        ofstream outfile(filename, ios::trunc);
        if (!outfile) return false;
        for (size_t i = 0; i < Items.size(); i++)
            outfile << Items[i] << ' ' << Quantity[i] << '\n';
        return (bool)outfile;
    }
    // 入库：返回 0 成功；1 数量溢出（拒绝）；2 保存失败
    int stock_in_(const string& name, unsigned int num) {
        int idx = findIndex(name);
        if (idx >= 0) {
            if (num > numeric_limits<unsigned int>::max() - Quantity[idx])
                return 1;                                   // 入库后总量将超过 unsigned int 上限
            Quantity[idx] += num;
        } else {
            Items.push_back(name);
            Quantity.push_back(num);
        }
        return save_func() ? 0 : 2;
    }
    // 出库：返回 0 成功；1 无此货品；2 库存不足；3 保存失败
    int stock_out_(const string& name, unsigned int num) {
        int idx = findIndex(name);
        if (idx < 0) return 1;
        if (Quantity[idx] < num) return 2;
        Quantity[idx] -= num;
        return save_func() ? 0 : 3;                 // 出库必须检查保存结果
    }
    bool revise_(size_t i, const string& name, unsigned int num) {
        if (i >= Items.size()) return false;
        Items[i] = name;
        Quantity[i] = num;
        return save_func();
    }
    bool delete_(size_t i) {
        if (i >= Items.size()) return false;
        Items.erase(Items.begin() + i);
        Quantity.erase(Quantity.begin() + i);
        return save_func();
    }
};
class screen {
private:
    static const size_t MAX_length = 56;

    void line() const { cout << string(MAX_length, '=') << '\n'; }
    void center(const string& s) const {
        size_t l = (MAX_length > s.size()) ? (MAX_length - s.size()) / 2 : 0;
        cout << string(l, ' ') << s << '\n';
    }

public:
    void greeting() const {
        line();
        center("WELCOME TO warehouse manager sys:)");
        line();
    }

    void menu() const {
        cout << "\n";
        line();
        center("MENU");
        cout << "  1. 显示存货列表\n"
                "  2. 入库\n"
                "  3. 出库\n"
                "  4. 修改条目\n"
                "  5. 删除条目\n"
                "  0. 退出程序\n";
        line();
    }

    void showItems(const warehouse& w) const {
        if (w.size() == 0) {
            center("(仓库空空如也)");
            return;
        }
        cout << "  编号   型号        数量\n";
        for (size_t i = 0; i < w.size(); i++) {
            // 用 signed 计算列宽，避免名称过长时 (常量 - size_t) 无符号下溢导致崩溃
            string idx  = to_string(i);
            string name = w.itemAt(i);
            int pad_idx  = 6  - (int)idx.size();   // 编号列右对齐
            int pad_name = 12 - (int)name.size();  // 名称列填充空格
            if (pad_idx  < 1) pad_idx  = 1;
            if (pad_name < 1) pad_name = 1;        // 名称过长时至少留 1 个空格
            cout << "  " << i
                 << string(pad_idx, ' ')
                 << name
                 << string(pad_name, ' ')
                 << w.qtyAt(i) << '\n';
        }
    }
};
string trimStr(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

// 严格读入一个整数：整行读取，拒绝空输入、负号、非数字字符与超范围数值
// （cin >> unsigned 会把 "-5" 静默转成超大正数，必须避免）
// 合法范围 [minV, maxV]；读到 EOF 返回 false
bool readNumber(const string& prompt, long long minV, long long maxV, long long& out) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) return false;
        string s = trimStr(line);
        if (s.empty())           { cout << "  ! 输入不能为空\n"; continue; }
        if (s[0] == '-')         { cout << "  ! 不能输入负数\n"; continue; }
        bool ok = true;
        for (char ch : s) if (!isdigit((unsigned char)ch)) { ok = false; break; }
        if (!ok)                 { cout << "  ! 请输入非负整数\n"; continue; }
        try {
            unsigned long long v = stoull(s);
            if ((long long)v < minV || (long long)v > maxV) {
                cout << "  ! 超出允许范围（" << minV << " ~ " << maxV << "）\n";
                continue;
            }
            out = (long long)v;
            return true;
        } catch (...)            { cout << "  ! 数字太大，超出可表示范围\n"; continue; }
    }
}

bool readLine(const string& prompt, string& out) {
    cout << prompt;
    if (!getline(cin, out)) return false;
    out = trimStr(out);          // 去掉首尾空白，避免型号带不可见空格导致匹配失败
    return true;
}
warehouse Warehouse;
screen Screen;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (!Warehouse.load_())
        cout << "  提示：未找到 warehouse.txt，从空仓库开始（首次运行属正常）。\n";
    Screen.greeting();

    while (true) {
        Screen.menu();
        long long choice;
        if (!readNumber("请选择: ", 0, 9, choice)) break;

        if (choice == 1) {
            system("cls");                          
            Screen.showItems(Warehouse);
        }
        else if (choice == 2) {
            string name; long long num;
            if (!readLine("货品名称: ", name)) break;
            if (name.empty()) { system("cls"); cout << "  ! 货品名称不能为空\n"; continue; }
            if (!readNumber("入库数量: ", 1, numeric_limits<unsigned int>::max(), num)) break;
            int r = Warehouse.stock_in_(name, (unsigned int)num);
            system("cls");
            if (r == 0)      cout << "  -> 入库成功，已保存\n";
            else if (r == 1) cout << "  ! 入库被拒绝：库存总量将超过最大值（溢出）\n";
            else             cout << "  ! 入库成功但写入文件失败\n";
        }
        else if (choice == 3) {
            string name; long long num;
            if (!readLine("货品名称: ", name)) break;
            if (name.empty()) { system("cls"); cout << "  ! 货品名称不能为空\n"; continue; }
            if (!readNumber("出库数量: ", 1, numeric_limits<unsigned int>::max(), num)) break;
            int r = Warehouse.stock_out_(name, (unsigned int)num);
            system("cls");
            if (r == 0)      cout << "  -> 出库成功，已保存\n";
            else if (r == 1) cout << "  ! 无此货品\n";
            else if (r == 2) cout << "  ! 库存不足，出库被拒绝\n";
            else             cout << "  ! 出库成功但保存失败，请检查 warehouse.txt 是否可写\n";
        }
        else if (choice == 4) {
            system("cls");
            Screen.showItems(Warehouse);
            long long i;
            if (!readNumber("要修改的编号: ", 0, numeric_limits<int>::max(), i)) break;
            string name; long long num;
            if (!readLine("新名称: ", name)) break;
            if (name.empty()) { cout << "  ! 新名称不能为空\n"; continue; }
            if (!readNumber("新数量: ", 0, numeric_limits<unsigned int>::max(), num)) break;
            if (Warehouse.revise_((size_t)i, name, (unsigned int)num)) cout << "  -> 修改成功\n";
            else cout << "  ! 编号越界\n";
        }
        else if (choice == 5) {
            system("cls");
            Screen.showItems(Warehouse);
            long long i;
            if (!readNumber("要删除的编号: ", 0, numeric_limits<int>::max(), i)) break;
            if (Warehouse.delete_((size_t)i)) cout << "  -> 删除成功\n";
            else cout << "  ! 编号越界\n";
        }
        else if (choice == 0) {                     
            break;
        }
        else {
            cout << "  ! 没有这个功能，请重新选择\n";
        }
    }
    Warehouse.save_func();      
    cout << "库存已保存，再见 :)\n";
    return 0;
}
