C++ 文件操作

检查文件夹

假设有文件结构

```plain text
p10_zoo_cpp/
├─ bark/        ←要扫描的文件夹
│  ├─ bird.m4a
│  ├─ cat.mp3
│  └─ ...
└─ main.cpp
```

两个方案

1. C++17 std::filesystem（推荐！现代 C++，跨平台，代码干净）
2. Windows 原生 `FindFirstFile`（旧方案，仅 Windows）

（一）c++ 17 filesystem

```c++
#include <iostream>
#include <filesystem>
#include <string>
using namespace std;
namespace fs = std::filesystem;

// 扫描文件夹，打印文件名 + 相对路径
void listFiles(const string& folderPath)
{
    try{
        // 遍历目录里所有条目
        for (const auto& entry : fs::directory_iterator(folderPath))
        {
            if (entry.is_regular_file()) // 只打印文件，跳过子文件夹
            {
                // 相对路径 bark/cat.mp3
                fs::path rel_path = entry.path();
                cout << "文件名：" << rel_path.filename().string()
                     << "    相对地址：" << rel_path.string() << endl;
            }
        }
    }
    catch(const fs::filesystem_error& e)
    {
        cout << "扫描文件夹失败: " << e.what() << endl;
    }
}

int main()
{
    listFiles("bark"); // bark文件夹和main.cpp同目录
    return 0;
}

```

