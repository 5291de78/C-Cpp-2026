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
        if (!infile) return false;
        string name;
        unsigned int qty;
        while (infile >> name >> qty) {  
            Items.push_back(name);
            Quantity.push_back(qty);
        }
        return true;
    }
    bool save_func(const string& filename = "warehouse.txt") const {
        ofstream outfile(filename, ios::trunc);
        if (!outfile) return false;
        for (size_t i = 0; i < Items.size(); i++)
            outfile << Items[i] << ' ' << Quantity[i] << '\n';
        return (bool)outfile;
    }
    bool stock_in_(const string& name, unsigned int num) {
        int idx = findIndex(name);
        if (idx >= 0) Quantity[idx] += num;
        else { Items.push_back(name); Quantity.push_back(num); }
        return save_func();
    }
    int stock_out_(const string& name, unsigned int num) {
        int idx = findIndex(name);
        if (idx < 0) return 1;
        if (Quantity[idx] < num) return 2;
        Quantity[idx] -= num;
        save_func();
        return 0;
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
bool readUInt(const string& prompt, unsigned int& out) {
    while (true) {
        cout << prompt;
        if (cin >> out) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }
        if (cin.eof()) return false;
        cin.clear();                                           
        cin.ignore(numeric_limits<streamsize>::max(), '\n');    
        cout << "  ! 输入无效，请输入非负整数\n";
    }
}

bool readLine(const string& prompt, string& out) {
    cout << prompt;
    return (bool)getline(cin, out);
}
warehouse Warehouse;
screen Screen;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Warehouse.load_();          
    Screen.greeting();

    while (true) {
        Screen.menu();
        unsigned int choice;
        if (!readUInt("请选择: ", choice)) break;   

        if (choice == 1) {
            system("cls");                          
            Screen.showItems(Warehouse);
        }
        else if (choice == 2) {                     
            string name; unsigned int num;
            if (!readLine("货品名称: ", name)) break;
            if (!readUInt("入库数量: ", num)) break;
            Warehouse.stock_in_(name, num);
            system("cls");
            cout << "  -> 入库成功，已保存\n";
        }
        else if (choice == 3) {                    
            string name; unsigned int num;
            if (!readLine("货品名称: ", name)) break;
            if (!readUInt("出库数量: ", num)) break;
            int r = Warehouse.stock_out_(name, num);
            if (r == 0){
                cout << "  -> 出库成功，已保存\n";
                system("cls");
            }      
            else if (r == 1) {
                cout << "  ! No matching entries found（无此货品）\n";
                system("cls");
            }else{
                cout << "  ! The quantity to be shipped is greater than the current stock!（库存不足）\n";
                system("cls");
            }             
        }
        else if (choice == 4) {
            system("cls");                     
            Screen.showItems(Warehouse);
            unsigned int i;
            if (!readUInt("要修改的编号: ", i)) break;
            string name; unsigned int num;
            if (!readLine("新名称: ", name)) break;
            if (!readUInt("新数量: ", num)) break;
            if (Warehouse.revise_(i, name, num)) cout << "  -> 修改成功\n";
            else cout << "  ! index out of range（编号越界）\n";
        }
        else if (choice == 5) {
            system("cls");                     
            Screen.showItems(Warehouse);
            unsigned int i;
            if (!readUInt("要删除的编号: ", i)) break;
            if (Warehouse.delete_(i)) cout << "  -> 删除成功\n";
            else cout << "  ! index out of range（编号越界）\n";
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
