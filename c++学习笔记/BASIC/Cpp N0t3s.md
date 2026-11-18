# Cpp **N0t3s**

```c++
#include <iostream>
#include <cstdint>  //引入定长数字

using namespace std;
/// 遗忘是时间最大副作用
@brief 这是一份c++笔记
@
@
```

# 一，数据

# 数据类型

```c++
int main(){
	std::cout<<"size of kinds of ints"<<std::endl;
    //定长数字
    cout << sizeof(int8_t);
    cout << sizeof(uint64_t);
    /*
       short  2*8 -32768 ~32767  float  4*8  7位有效数字
       int    4*8 四个字节 32位   double 8*8  15位有效数字
       long   8*8 		   long double 16*8 18位有效数字
       unsigned 标志说明该数字以无符号形式存储 大小一致
    
    */
    cout << "size of various chars"
    cin>>Ready for char;
    /*bool  1  
      char  1   -128~127
      wchar     宽字符
      
    
	return 0
}
```

# 类型转换

## 	隐式转换

```c++
//算术转换
    int a = 3;
    double b = 2.5
    auto c = a + b 
//初始化转换
	doulbe d = 3.14;
	int n = d; (实际上n是3，被截断了)
//参数传递
	void func(int num){};
	func(3.14); 实际传入3
	
	int arr[] = {1,2,3,4};
	for(auto v : arr){} //C数组退化为指向arr[0]的指针
//条件转换
	if(3.14)  非0 --> true ; 0 --> flase;
	if("abc") 指针非空 --> true
	if(nullptr)    --> false
//有无符号
	int a = -1;
    unsigned int b = a;  
    // a = -1, b = 4294967295

```

## 	转型操作

```c++
int(var) var(int) //C风格数组转换,不检查
int i = 42
double d = static_cast<double>(i) //可读性高，编译器会检查

reinterpret __ 重新解释底层的位模式
    // 场景一：void* 和类型指针之间的转换
    int value = 100;
    void* pv = &value;
    int* pi = reinterpret_cast<int*>(pv);
    std::cout << *pi << std::endl;  // 100

    // 场景二：查看浮点数的底层位模式
    float f = 1.0f;
    uint32_t bits = reinterpret_cast<uint32_t&>(f);
    // 1.0f 的 IEEE 754 表示：0x3f800000
    std::cout << std::hex << bits << std::endl;

```

## 	数值精度

```c++
//数字运算
int a = 5, b = 2;
double result = a / b;           // 整数除法！结果是 2，不是 2.5
double correct = static_cast<double>(a) / b;  // 正确：5.0 / 2 = 2.5
//浮点数比较
#include <iostream>
#include <cmath>

int main()
{
    double a = 0.1 + 0.2;
    double b = 0.3;

    // 直接比较：false！因为 0.1+0.2 实际存储为 0.30000000000000004
    std::cout << std::boolalpha << (a == b) << std::endl;  // false

    // 正确做法：判断差值是否足够小
    double epsilon = 1e-9;
    bool approx_equal = std::abs(a - b) < epsilon;
    std::cout << approx_equal << std::endl;  // true
    return 0;
}
//整数溢出
#include <climits>

int max_int = INT_MAX;     // 2147483647
int overflow = max_int + 1;  // 未定义行为！通常是 -2147483648

unsigned char uc = 255;
uc = uc + 1;               // 明确定义的回绕，变成 0
```

## 列表

# CONST常量

## 常量的写法

```c++
@CONST
    const int var = 100; //不可再更改
    //与指针
    const int* p = & var; //锁数据  可以指向别的变量
    int* const p = & var //锁指向	可以通过指针改变数据
@宏定义
	//宏定义会在语义分析前对程序进行替换 非常危险 
	//注意 这不是c++的语法
	#define PI 3,14159
@CONSTEXPR
	//constexpr强大的地方在于一个函数的参数都是在编译器可以确定的数，那可以在编译器直接确定
	constexpr int square(int x){
		return x*x
	}
	constexpr int kResult = square(5)
```

# 别名

## 给类型取别名

## 1，typedef

```c
typedef int Integer;          // Integer 等价 int
typedef int* IntPtr;          // IntPtr = int*
typedef int Arr[10];          // Arr 是大小为10的int数组类型

Integer a = 10;

```

## 2，using

```c++
using Integer = int;
using IntPtr = int*;
using Arr = int[10];
```

## 3,#define

```c
#define INT int
typedef int Int;
```

## 4,auto/decltype

```c
double f();
using RetType = decltype(f()); // RetType = double

```

# 二，控制流

## 循环

```c
scores[] = {1,2,3,4,5};

for ( int i = 0; i < 5; ++i){  }
for ( int score : scpres){  }
while( situations ){ cycle }
do{     }while( situations)
    
///可遍历对象*只要对象支持 begin () /end ()，就可以用范围 for 遍历**，这类我们叫可遍历范围 (range)。
    /*C数组（注意退化）
     STL容器：
     vector string deque list forward_list map/mutimpa set/multset
     */
cout << scores.begin();
cout << scores.end();
cout << .cbegin //const只读迭代器
cout << .cend
cout << .rbegin
.rend
```

## 条件判断

```c
//switch
    switch (op) {
    case 1:
        cout << "add";
        break;   // 必须加!
    case 2:
        cout << "sub";
        break;
    case 3:
    case 4:      // 合并case
        cout << "mul";
        break;
    default:
        cout << "invalid";
    }
    //忘加 break 会贯穿（fall-through）执行下一个 case

//if-else
	int score = 85;

    if (score >= 90) {
        // 优秀
        cout << "A";
    } else if (score >= 80) {
        // 良好
        cout << "B";
    } else if (score >= 60) {
        cout << "C";
    } else {
        cout << "F";
    }

//三目判断
	int max = (a > b)? a:b;
	
	int sign = (x > 0) ? 1 :
            (x < 0) ? -1 : 0;


```

# 函数，结构与声明

## 函数重载

编译器根据传入函数的数据类型匹配最可能的函数，若没有，则发生隐式转换。

每次调用一个函数，程序要做的事情其实不少：保存当前执行位置、分配栈帧、跳转到函数体、执行完再跳回来、销毁栈帧、恢复现场。

## 结构

```c
#include <iostream>
#include <vector>
#include <string>

// 前向声明
struct SubData;

struct BigData
{
    // ========== 1. 嵌套定义：内部枚举、内部结构体 ==========
    enum class DataType
    {
        IntType,
        FloatType,
        StringType,
        COUNT
    };

    struct MetaInfo
    {
        std::string name;
        int version;
    };

    // ========== 2. 成员变量（普通变量 + 多个vector容器，就是你前面关心的） ==========
    MetaInfo meta;
    std::vector<int>         int_buf;
    std::vector<double>      float_buf;
    std::vector<std::string> str_buf;

    // ========== 3. 私有成员（struct也可以写private，只是默认是public） ==========
private:
    int secret_id = 0;
public:

    // ========== 4. 构造函数 ==========
    // 默认构造
    BigData() = default;

    // 带参构造
    BigData(std::string name, int ver)
    {
        meta.name = std::move(name);
        meta.version = ver;
    }

    // 拷贝构造函数
    BigData(const BigData& other)
    {
        std::cout << "[拷贝构造]\n";
        meta      = other.meta;
        int_buf   = other.int_buf;
        float_buf = other.float_buf;
        str_buf   = other.str_buf;
        secret_id = other.secret_id;
    }

    // 移动构造（避免拷贝大vector，性能更好）
    BigData(BigData&& other) noexcept
    {
        std::cout << "[移动构造]\n";
        meta      = std::move(other.meta);
        int_buf   = std::move(other.int_buf);
        float_buf = std::move(other.float_buf);
        str_buf   = std::move(other.str_buf);
        secret_id = other.secret_id;
    }

    // ========== 5. 析构函数 ==========
    ~BigData()
    {
        std::cout << "[析构] " << meta.name << "\n";
    }

    // ========== 6. 成员函数：普通函数 + const成员函数（承诺不修改对象） ==========
    void set_secret(int id)
    {
        secret_id = id;
    }

    // const成员函数，只能读取，不能修改成员
    void print_info() const
    {
        std::cout << "name:" << meta.name
                  << " | ver:" << meta.version
                  << " | secret_id:" << secret_id << "\n";
        std::cout << "int_buf size = " << int_buf.size() << "\n";
    }

    void clear_all()
    {
        int_buf.clear();
        float_buf.clear();
        str_buf.clear();
    }

    // ========== 7. 静态成员（属于类型本身，不属于单个实例） ==========
    static int instance_count;
    static void show_static()
    {
        std::cout << "静态函数：instance_count = " << instance_count << "\n";
    }

    // ========== 8. 运算符重载 ==========
    bool operator==(const BigData& other) const
    {
        return meta.name == other.meta.name;
    }
};
// 静态变量类外初始化
int BigData::instance_count = 0;


// ========== 9. struct 继承示例（struct可以继承，默认public继承） ==========
struct ExtData : BigData
{
    std::vector<long long> long_buf;
};


int main()
{
    // 方式1：调用带参构造
    BigData d1("test_data", 1);
    d1.int_buf = {1,2,3,4};
    d1.float_buf.push_back(3.14);
    d1.set_secret(1001);
    d1.print_info();

    // 方式2：列表初始化（C++11）
    BigData d2{"demo2", 2};
    d2.str_buf.emplace_back("hello struct");
    d2.print_info();

    // 方式3：拷贝构造
    BigData d3 = d1;
    d3.print_info();

    // 方式4：移动构造
    BigData d4 = std::move(d2);
    d4.print_info();

    // 使用内部enum
    BigData::DataType t = BigData::DataType::IntType;
    std::cout << "enum test\n";

    // 访问静态成员
    BigData::instance_count = 10;
    BigData::show_static();

    // 运算符重载 ==
    if (!(d1 == d4))
    {
        std::cout << "d1 != d4\n";
    }

    // 继承的结构体
    ExtData ed;
    ed.meta.name = "extend";
    ed.long_buf.push_back(123456LL);
    ed.print_info();

    ed.clear_all();
    return 0;
}
```



# STL容器

## 迭代器（Iterator)

迭代器iterator是C++ STL的组件之一，作用是**用来遍历容器**，而且是**通用的遍历容器元素的方式**，无论容器是基于什么数据结构实现的，**尽管不同的数据结构，遍历元素的方式不一样，但是用迭代器遍历不同容器的代码是完全一样的**。经典的迭代器遍历容器的代码如下：

```c++
vector<int>::iterator it = vec.begin();
for (; it != vec.end(); ++it)
{   
	cout << *it << " ";
}
cout << endl;

unordered_set<int>::iterator it = us.begin();
for (; it != us.end(); ++it)
{ 
	cout << *it << " ";
}
cout << endl;

```

迭代器的简单实现

```c++
class Iterator {
    int* ptr; // 指向元素
public:
    Iterator(int* p) : ptr(p) {}
    int& operator*() { return *ptr; }       // 解引用
    Iterator& operator++() { ++ptr; return *this; } // 前置递增
    Iterator& operator--() { --ptr; return *this; } // 前置递减
    bool operator!=(const Iterator& other) { return ptr != other.ptr; }
};

```



## string字符串的操作

```c
#include<string>
using namespace std;
s = "I give you my mercy!"
```

访问/查找

```c
var = s[2];  var = 2[] ; 'g'
    s[1] = 's'	//也可以直接改
var = s.length(); var = s.size();; "20"
var = s.back();	s.front(); 
s.find()	s.rfind
```

修改操作

```c
//追加
s += "append part"
s = a + b + c;
s.append("append part");
s.append(10,'=');
s.append("01234" , 1 , 3); //追加了123
//insert
s.insert(1,"xxxx")
//erase
s,erase(2,2)// 从下标2，删2个字符：删掉c,d → abef
s.erase(3) // 从下标3删到结尾
//修改
s.replace(0,5,"hi")
    
```

查找

```
find：正向查找
rfind：反向查找（从后往前）
find_first_of：找任意一个匹配字符
find_last_of
```

字串

```c
string s = "abcdef";
string sub = s.substr(1,3); // 从下标1，取3个字符 → "bcd"
string sub2 = s.substr(2);  // 从下标2一直到末尾
```

分割 split

```c
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> res;
    std::string item;
    size_t start = 0;
    size_t end;
    while ((end = s.find(delim, start)) != std::string::npos)
    {
        item = s.substr(start, end - start);
        res.push_back(item);
        start = end + 1;
    }
    // 最后一段
    res.push_back(s.substr(start));
    return res;
}

// 使用示例
// auto parts = split("apple,banana,orange", ',');
```



其他

```c
string s = "  test ";
s.empty();          // 判断是否为空
s.size() / s.length(); // 获取字符数量，两者完全一样
s.clear();          // 清空字符串
s.resize(5);        // 修改字符长度
```

## vector容器

```c++
#include <vector>
using namespace std; // 简易写法，项目中按需取舍
vector<string> Items = {"ass","assist","assassin"}
```

操作

```c++
//stack 特性
Items.push_back("apple");
Items.pop_back();
//modify
Items.insert(Items.begin(), "banana"); // 在开头插入
Items.erase(Items.begin()); // 删除第一个
//元素访问
Items[2]; 
[Items]4;
Items.at(67) // 带越界检查
Items[Items.size()-1]; // 最后一个元素
string last = Items.back();
    

// 迭代器使用
auto it = Items.begin(); 
cout << *it;      
++it;             
cout << *it;      

    

// 范围for
for(auto &s : Items){
    cout << s << endl;
}
//访问属性
Items.size();
Items.empty();
Items.capacity();
Items.resize(n);
Items.reverse(n)
```

vector没有length()成员函数！！！！！

vector没有length()成员函数！！！！！！

vector没有length()成员函数！！！！！！！

### **查找**

```c++
//STD::FIND
	// 原型：find(起始迭代器, 末尾迭代器, 要找的值)
auto it = find(Items.begin(), Items.end(), "要查找的货物名称");
vector<string> Items = {"apple", "banana", "orange"};
auto it = find(Items.begin(), Items.end(), "banana");
if(it != Items.end()){
    // 找到了
    cout << "找到：" << *it << endl;
    int idx = it - Items.begin(); // 获取下标
}else{
    // 未找到
    screen.err_();
}
//手写循环查找
	//迭代器查找
	for(auto it = Items.begin(); it != Items.end(); ++it){
    if(*it == "banana"){
        // 找到 *it
        break;
    }
}
	//范围for版本
	for(auto &s : Items){
    if(s == "banana"){
        //找到了
    }
}

```