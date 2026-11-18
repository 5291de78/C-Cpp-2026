# C++ 函数与结构体的构造/析构函数

## 一、C++ 函数基础

函数是程序的基本执行单元，由**返回类型、函数名、参数列表、函数体**组成。

```cpp
#include <iostream>

// 函数声明（ prototyping ），可以放在头文件
int add(int a, int b);

int main() {
    std::cout << add(3, 5) << std::endl;  // 调用
    return 0;
}

// 函数定义
int add(int a, int b) {
    return a + b;
}
```

### 1. 几个重要特性

- **默认参数**：从右往左给默认值
  ```cpp
  void print(int x, int y = 10);   // ✅
  // void print(int x = 1, int y); // ❌ 错误：默认参数后不能跟非默认参数
  ```

- **函数重载（Overload）**：同名函数，参数**个数或类型不同**（仅返回值不同不算重载）
  ```cpp
  int add(int a, int b);
  double add(double a, double b);
  int add(int a, int b, int c);
  ```

- **内联函数 `inline`**：建议编译器展开，适合短小函数
  ```cpp
  inline int square(int x) { return x * x; }
  ```

- **引用传参**：避免拷贝、可修改实参
  ```cpp
  void swap(int& a, int& b) {
      int t = a; a = b; b = t;
  }

  void print(const std::string& s); // 只读大对象，避免拷贝
  ```

- **函数模板**：泛型编程
  ```cpp
  template <typename T>
  T add(T a, T b) { return a + b; }
  ```

---

## 二、结构体与类

C++ 中 `struct` 和 `class` **几乎完全一样**，唯一区别是默认访问权限：

|          | 默认访问权限              |
| -------- | ------------------------- |
| `struct` | `public`（成员对外可见）  |
| `class`  | `private`（成员对外隐藏） |

结构体不仅可以有数据成员，还可以有**成员函数、构造函数、析构函数**——这就是 C++ 的"面向对象"。

```cpp
struct Point {
    int x, y;                 // 默认 public

    Point(int x, int y) : x(x), y(y) {}   // 构造函数
    ~Point() { /* 析构逻辑 */ }            // 析构函数

    double length() const {   // 成员函数
        return std::sqrt(x*x + y*y);
    }
};
```

---

## 三、构造函数（Constructor）

### 1. 基本概念

- 函数名与结构体名**完全相同**，**没有返回类型**（连 `void` 都不能写）
- 在创建对象时**自动调用**，用于初始化成员
- 不写的话，编译器自动生成一个"默认构造函数"（啥也不做，或按规则初始化成员）

```cpp
struct Student {
    std::string name;
    int age;

    // 无参构造函数
    Student() {
        name = "unknown";
        age = 0;
    }

    // 带参构造函数
    Student(const std::string& n, int a) {
        name = n;
        age = a;
    }
};

Student s1;                  // 调用 Student()
Student s2("Alice", 20);     // 调用 Student("Alice", 20)
Student s3{"Bob", 18};       // 列表初始化（推荐，C++11）
Student s4 = {"Carol", 22};  // 拷贝列表初始化
```

### 2. 成员初始化列表（强烈推荐）

在冒号后初始化成员，比在函数体内赋值**更高效**（直接初始化而非先默认构造再赋值），并且是**某些成员的唯一初始化方式**：

```cpp
struct Person {
    const int id;      // const 成员只能在初始化列表中初始化
    int& alias;        // 引用成员也是
    std::string name;

    Person(int id, int& a, const std::string& n)
        : id(id), alias(a), name(n) {}   // 初始化列表
};
```

### 3. 构造函数的重载与默认参数

```cpp
struct Rect {
    int w, h;
    Rect() : w(0), h(0) {}
    Rect(int side) : w(side), h(side) {}   // 正方形
    Rect(int w, int h) : w(w), h(h) {}
};

Rect r1;          // 0, 0
Rect r2(5);       // 5, 5
Rect r3(3, 4);    // 3, 4
```

### 4. 委托构造函数（C++11）

一个构造函数调用本类的另一个构造函数：

```cpp
struct Rect {
    int w, h;
    Rect(int w, int h) : w(w), h(h) {}
    Rect() : Rect(0, 0) {}         // 委托给上面那个
};
```

### 5. 特殊构造函数

| 类型                           | 作用                           |
| ------------------------------ | ------------------------------ |
| 拷贝构造函数 `T(const T&)`     | 用一个对象初始化新对象         |
| 移动构造函数 `T(T&&)`（C++11） | 窃取右值对象的资源，避免深拷贝 |
| `explicit` 构造函数            | 禁止隐式类型转换               |

```cpp
struct String {
    char* data;

    explicit String(size_t len) { data = new char[len]; }
    String(const String& other) { /* 深拷贝 */ }
    String(String&& other) noexcept {  // 移动构造
        data = other.data;
        other.data = nullptr;
    }
    ~String() { delete[] data; }
};
```

---

## 四、析构函数（Destructor）

### 1. 基本概念

- 函数名是 `~` 加结构体名，**无参数、无返回值**（因此不能重载，**一个类只有一个析构函数**）
- 对象**生命周期结束**时自动调用：
  - 局部对象：离开作用域时
  - `new` 出来的对象：`delete` 时
  - 静态/全局对象：程序结束时

```cpp
struct Timer {
    std::chrono::steady_clock::time_point start;
    Timer() : start(std::chrono::steady_clock::now()) {}
    ~Timer() {
        auto end = std::chrono::steady_clock::now();
        std::cout << "耗时: "
                  << std::chrono::duration<double>(end - start).count()
                  << " 秒\n";
    }
};

void work() {
    Timer t;        // 开始计时
    // ... 做一些事 ...
}   // t 离开作用域，自动输出耗时 —— 这就是 RAII 思想
```

### 2. 核心用途：资源释放（RAII）

析构函数最常见的用途是**释放资源**：内存、文件句柄、网络连接、锁等。这是 C++ 最重要的惯用法 **RAII**（Resource Acquisition Is Initialization，资源获取即初始化）的基础。

```cpp
struct File {
    FILE* fp;
    File(const char* path) { fp = fopen(path, "r"); }
    ~File() { if (fp) fclose(fp); }   // 保证文件一定被关闭
};
```

有了析构函数，就**不用记得手动 `fclose`**——无论函数正常返回还是中途抛异常，析构函数都会执行。

### 3. 不写析构函数会怎样？

编译器会生成一个默认析构函数，逐个调用成员的析构函数。大多数情况下这就够了。但如果你**手动 `new` 了资源**，通常需要自己写析构函数，或者直接用智能指针：

```cpp
struct Node {
    std::string name;
    std::unique_ptr<Node> next;  // 用智能指针，连析构函数都不用写
};
```

---

## 五、一个完整的综合示例

```cpp
#include <iostream>
#include <string>

struct Account {
    std::string owner;
    double balance;
    static int count;                 // 静态成员：统计账户数

    Account(const std::string& name, double money)
        : owner(name), balance(money)   // 初始化列表
    {
        ++count;
        std::cout << owner << " 开户，余额 " << balance << "\n";
    }

    ~Account() {
        --count;
        std::cout << owner << " 销户\n";
    }

    void deposit(double m) { balance += m; }
    bool withdraw(double m) {
        if (m > balance) return false;
        balance -= m;
        return true;
    }
};

int Account::count = 0;

int main() {
    {
        Account a("张三", 1000);
        Account b("李四", 500);
        a.deposit(200);
        b.withdraw(100);
        std::cout << "当前账户数: " << Account::count << "\n";
    }   // ← a 和 b 在这里析构，输出"销户"
    std::cout << "当前账户数: " << Account::count << "\n";
}
```

输出：

```
张三 开户，余额 1000
李四 开户，余额 500
当前账户数: 2
李四 销户
张三 销户
当前账户数: 0
```

注意析构顺序与构造顺序**相反**（后构造的先析构，像栈一样）。

---

## 六、要点总结

- **构造函数**：对象诞生时自动调用，负责初始化；名字同类名、无返回值；可用初始化列表提高效率；可以重载、可委托。
- **析构函数**：对象销毁时自动调用，负责清理；名为 `~类名`、无参无返回值、不能重载。
- 不写时编译器自动生成默认版本，多数情况下够用；涉及裸资源（`new`、文件句柄）时需自己管理，或改用智能指针/`fstream` 等 RAII 包装。
- `struct` 与 `class` 的唯一区别是默认访问权限（`public` vs `private`）。
- 构造/析构保证了**"资源随对象生命周期自动管理"**，这是 C++ 相比 C 最核心的进步。

如果你想动手练习，可以在工作目录的 `cpp_demo/` 里写一个演示文件编译跑一下（目录里已有 `g++` 编译出的 `.exe`，说明环境可用）。需要我帮你写一个带构造/析构日志的完整练习程序吗？