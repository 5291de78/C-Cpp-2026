# windos控制台(system_func)

函数名: system

功 能: 发出一个DOS命令

用 法: int system(char *command);

## 一，常用

```c++
system("cls")  		//清屏
system("mode con cols=80 lines=25")
system("title {windows name}")
system("pause")   //暂停，按任意键继续
```
## 二，更改颜色

### 1，整屏变色

```c
system("color 0A") //⚠️缺点：每次调用会新建子进程，频繁调用（动画）会卡顿
```

![1792377354392](../计算机科学/mind_pic/1792377354392.png)

### 2，指定字符串变色（维护简单）

```c++
#include <windows.h>
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// 用法：输出前设置，输出后恢复
cout << "普通白色，";
setColor(12);  cout << "亮红";   // 前景色 0~15
setColor(14);  cout << "亮黄";
setColor(7);   cout << "，恢复白色\n";

```

![1792377354392](../计算机科学/mind_pic/1792377354392.png)

### 3，ANSI 转义序列（简洁！

```c++
cout << "\033[32m绿色\033[0m"      // 32绿 31红 33黄 34蓝 35紫
     << "\033[1;31m加粗红\033[0m"   // 1; 表示加粗
     << "默认色\n";

```

### 4，改"已经打印出来"的某个字符串 

```c++
void recolorLastLine(const string& text, int color) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOut, &info);
    COORD pos = { 0, (SHORT)(info.dwCursorPosition.Y - 1) }; // 移到上一行开头
    SetConsoleCursorPosition(hOut, pos);
    DWORD written;
    FillConsoleOutputCharacter(hOut, ' ', info.dwSize.X, pos, &written); // 清行
    SetConsoleCursorPosition(hOut, pos);
    setColor(color);
    cout << text;
    setColor(7);
    cout << endl;
}

```



## 负一，常用DOS指令查询表

大小写均可以

ASSOC    显示或修改文件扩展名关联。
AT       计划在计算机上运行的命令和程序。
ATTRIB   显示或更改文件属性。
BREAK    设置或清除扩展式 CTRL+C 检查。
CACLS    显示或修改文件的访问控制列表(ACLs)。
CALL     从另一个批处理程序调用这一个。
CD       显示当前目录的名称或将其更改。
CHCP     显示或设置活动代码页数。
CHDIR    显示当前目录的名称或将其更改。
CHKDSK   检查磁盘并显示状态报告。
CHKNTFS  显示或修改启动时间磁盘检查。
CLS      清除屏幕。
CMD      打开另一个 Windows 命令解释程序窗口。
COLOR    设置默认控制台前景和背景颜色。
COMP     比较两个或两套文件的内容。
COMPACT  显示或更改 NTFS 分区上文件的压缩。
CONVERT  将 FAT 卷转换成 NTFS。您不能转换当前驱动器。
COPY     将至少一个文件复制到另一个位置。
DATE     显示或设置日期。
DEL      删除至少一个文件。
DIR      显示一个目录中的文件和子目录。
DISKCOMP 比较两个软盘的内容。
DISKCOPY 将一个软盘的内容复制到另一个软盘。
DOSKEY   编辑命令行、调用 Windows 命令并创建宏。
ECHO     显示消息，或将命令回显打开或关上。
ENDLOCAL 结束批文件中环境更改的本地化。
ERASE    删除至少一个文件。
EXIT     退出 CMD.EXE 程序(命令解释程序)。
FC       比较两个或两套文件，并显示不同处。
FIND     在文件中搜索文字字符串。
FINDSTR  在文件中搜索字符串。
FOR      为一套文件中的每个文件运行一个指定的命令
FORMAT   格式化磁盘，以便跟 Windows 使用。
FTYPE    显示或修改用于文件扩展名关联的文件类型。
GOTO     将 Windows 命令解释程序指向批处理程序中某个标明的行。
GRAFTABL 启用 Windows 来以图像模式显示扩展字符集。
HELP     提供 Windows 命令的帮助信息。
IF       执行批处理程序中的条件性处理。
LABEL    创建、更改或删除磁盘的卷标。
MD       创建目录。
MKDIR    创建目录。
MODE     配置系统设备。
MORE     一次显示一个结果屏幕。
MOVE     将文件从一个目录移到另一个目录。
PATH     显示或设置可执行文件的搜索路径。
PAUSE    暂停批文件的处理并显示消息。
POPD     还原 PUSHD 保存的当前目录的上一个值。
PRINT    打印文本文件。
PROMPT   更改 Windows 命令提示符。
PUSHD    保存当前目录，然后对其进行更改。
RD       删除目录。
RECOVER  从有问题的磁盘恢复可读信息。
REM      记录批文件或 CONFIG.SYS 中的注释。
REN      重命名文件。
RENAME   重命名文件。
REPLACE  替换文件。
RMDIR    删除目录。
SET      显示、设置或删除 Windows 环境变量。
SETLOCAL 开始批文件中环境更改的本地化。
SHIFT    更换批文件中可替换参数的位置。
SORT     对输入进行分类。
START    启动另一个窗口来运行指定的程序或命令。
SUBST    将路径跟一个驱动器号关联。
TIME     显示或设置系统时间。
TITLE    设置 CMD.EXE 会话的窗口标题。
TREE     以图形模式显示驱动器或路径的目录结构。
TYPE     显示文本文件的内容。
VER      显示 Windows 版本。
VERIFY   告诉 Windows 是否验证文件是否已正确写入磁盘。
VOL      显示磁盘卷标和序列号。
XCOPY    复制文件和目录树