#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <limits>
#include <filesystem>
#include <conio.h>
#pragma comment(lib, "winmm.lib")

using namespace std;
namespace fs = std::filesystem;

// mciSendString 的错误检查封装：失败时打印原因，不再"静默无声"
static void mciTry(const char* cmd) {
    DWORD r = mciSendStringA(cmd, NULL, 0, NULL);
    if (r != 0) {
        char buf[256] = {0};
        mciGetErrorStringA(r, buf, 256);
        cerr << "[mci 错误] " << cmd << " -> " << buf << '\n';
    }
}

class zoo{
    private:
        int i = 0;
        inline static string sheep = R"(
      )--------(
     /  o    o  \
    |     ^      |
     \__________/
    /            \
   /              \
  /________________\
)";
        inline static string dog = R"(
             
       _.'__    `.
        .--($)($$)---/#\
      .' @          /###\
      :         ,   #####
       `-..__.-' _.-\###/
             `;_:    `"'
           .'"""""`.
          /,     ,\\
         //  dog   \\
         `-._______.-'
         ___`. | .'___
        (______|______)       
        )";
        inline static string cow = R"(
          ^__^
         (oo)\_______
         (__)\       )\/\
              ||----w  |
              ||     ||
             ~~~     ~~~
)" ;
        inline static string chick = R"(
         _._
       .'   '.
      ( o   o )
       \  v  /        
        '---'
        (  o )         
         '-'-'
         _| |_
        ~~   ~~
)" ;    
        inline static string cat = R"(
        /\_/\
       ( o.o )        
        > ^ <         
      /       \
     (  ~  ~   )       
      '-.._..-'

)" ; 
        inline static string bird = R"(
        ___
    ___( o)>
    \   <_  )
     '-----'
       | |
       | |
      ~~~~~
)" ; 
        inline static vector<string> pic = {sheep , dog , cow , chick , cat ,bird};
        inline static vector<string> names ={"sheep" , "dog" , "cow" , "chick","cat" };
        string name = names.at(i);
        inline static vector<string> paths = {"bark/sheep.mp3","bark/dog.wav","bark/cow.mp3","bark/chicken.wav","bark/cat.mp3"};

    public:
        zoo(int i_ ) : i(i_) {}
        void bark() {
            string mp3_path = paths.at(i);
            string cmd = "open \"" + mp3_path + "\" alias asound";   // 修正：open "路径" alias 名字
            mciTry(cmd.c_str());
            mciTry("play asound");
            Sleep(6000);
            mciTry("stop asound");
            mciTry("close asound");
        }
        void draw(){
            cout << string(25 , '=')<<'\n';
            cout << pic.at(i) << '\n';
            cout << string(25 , '=')<<'\n';
            cout << "Look! that is a " + name + " !\n";
        }
         
        inline static string sound_match(const string& folderPath , const string& name)
        {
            try{
                // 遍历目录里所有条目
                for (const auto& entry : fs::directory_iterator(folderPath))
                {
                    if (entry.is_regular_file()) // 只打印文件，跳过子文件夹
                    {
                        // 相对路径 bark/cat.mp3
                        fs::path rel_path = entry.path();
                        string rel_path_str = rel_path.filename().string();
                        if(rel_path_str.substr(0 , rel_path_str.find('.')) == name){
                            return rel_path.string();
                        }

                    }
                }
                return "";
            }
            catch(const fs::filesystem_error& e)
            {
                cout << "扫描文件夹失败: " << e.what() << endl;
            }
            return "";   // 捕获异常后也必须返回，否则未定义行为
        }

        inline static void add(){
            system("cls");
            string new_name;
            cout<<string(10 , '=')<<'\n';
            cout<<"WHat is it?\n>";
            while(!(cin>>new_name)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                system("cls");
                cout <<string(10 , '=')<<'\n';
                cout <<"That is not a name!\n";
                cout <<"WHat is it?\n>";
            }
            string address = sound_match("bark" , new_name);
            cout<<"OH,it's " + new_name + " ,I like it!\n";
            if(address.empty()){cout<<"It's a pity, it can't make a sound..\n";
            }else{paths.push_back(address);
            names.push_back(new_name);
            cout<<"it makes sound like this!\n";
            string cmd = "open \"" + address + "\" alias asound";   // 修正 MCI 命令
            mciTry(cmd.c_str());
            mciTry("play asound");
            Sleep(5000);
            mciTry("stop asound");
            mciTry("close asound");}
            system("cls");
    
        }
    };

char readKey() {
    DWORD mode;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn != INVALID_HANDLE_VALUE && GetConsoleMode(hIn, &mode))
        return (char)_getch();
    char c;
    return (cin.get(c)) ? c : 'X';
}

int main(){
    int i = 0;
    zoo sheep(0);
    zoo dog(1);
    zoo cow(2);
    zoo cat(3);
    zoo chicken(4);
    vector<zoo> Zoo ={sheep , dog , cow , cat , chicken};
    cout << string(25 , '=')<<'\n'; 
    cout<<"what a nice day,let's visit the zoo!\n";
    cout<<"press A and D to visit\n";
    cout<<"press E to add new animal into your zoo!\n";
    cout<<"press W to let them sing a song for you!\n";
    cout<<"press X to EXIT\n";
    cout<<"press any key to start!\n";
    if(_kbhit()){system("cls");}
    while(1){
        char key = readKey();
        if( key == 'w' || key == 'W' ){
            for(int j = 0; j < (int)Zoo.size(); j++){   // 修正：原来是逗号运算符 i<Zoo.size(), j++ 会死循环崩溃
                cout << string(25 , '=')<<'\n'; 
                cout << "[A/D]=move on  [E]=add animal [W]=let'em sing [x]=EXIT\n";
                Zoo.at(j).draw();
                Zoo.at(j).bark();
                system("cls");
            }
        }else if( key == 'a' || key == 'A'){
            if(i>0){
                i--;
                cout << string(25 , '=')<<'\n'; 
                cout << "[A/D]=move on  [E]=add animal [W]=let'em sing [x]=EXIT\n";
                Zoo.at(i).draw();
                Zoo.at(i).bark();
            }
        }else if( key == 'e' || key == 'E'){
            zoo::add();
        }else if( key == 'd' || key == 'D'){
            if(i<Zoo.size() - 1){
                i++;
                cout << string(25 , '=')<<'\n'; 
                cout << "[A/D]=move on  [E]=add animal [W]=let'em sing [x]=EXIT\n";
                Zoo.at(i).draw();
                Zoo.at(i).bark();
            }
        }else if( key == 'x' || key == 'X'){
            break;
        }
    }
}




























































/*
老师您好，这份 C++ 大作业，耗费了我整整一个月的精力。我日夜敲写代码，反复攻克链接报错、排查逻辑漏洞，几乎把所有课余时间都投入其中，已经一个月没有玩游戏了。一路上踩遍各类难题，不断查阅资料、逐行调试，用心打磨每一处功能。这份作业凝聚了我大量心血，恳请老师酌情给予 100 分。我也会带着这份热忱继续深耕 C++，坚持钻研*/