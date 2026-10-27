// 题目：SafeArray —— 用 class 封装"安全数组"
// 功能要求与实现对应关系：
//   一) int 型数组，下标越界时报错        -> SafeArray<int>，operator[] 越界抛 std::out_of_range
//   二) 重载下标运算符（学完运算符重载）   -> operator[] 提供可读可写 / 只读两个版本
//   三) 模板实现类型无关（学完模板）       -> template <typename T>，int / double / string 均可
// 附加：深拷贝（规则五）、移动语义、越界信息中包含实际下标与数组长度

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <cstddef>
using namespace std;

template <typename T>
class SafeArray {
private:
    T* data;      // 指向堆上分配的数组
    size_t len;   // 数组长度

public:
    // 构造函数：分配 n 个元素并零初始化
    explicit SafeArray(size_t n = 0)
        : data(new T[n]{}), len(n) {}

    // 拷贝构造函数：深拷贝，避免悬挂指针 / 重复释放
    SafeArray(const SafeArray& other)
        : data(new T[other.len]{}), len(other.len) {
        for (size_t i = 0; i < len; ++i)
            data[i] = other.data[i];
    }

    // 拷贝赋值运算符：深拷贝 + 自赋值保护（三/五法则）
    // 先把新内存备好再释放旧内存，即使拷贝过程抛异常也不会破坏原对象
    SafeArray& operator=(const SafeArray& other) {
        if (this != &other) {
            T* tmp = new T[other.len]{};
            for (size_t i = 0; i < other.len; ++i)
                tmp[i] = other.data[i];
            delete[] data;
            data = tmp;
            len = other.len;
        }
        return *this;
    }

    // 移动构造函数：直接接管源对象的内存，标记 noexcept
    SafeArray(SafeArray&& other) noexcept
        : data(other.data), len(other.len) {
        other.data = nullptr;      // 置空源对象，防止其析构时误删内存
        other.len = 0;
    }

    // 移动赋值运算符：释放自身内存后接管源对象内存
    SafeArray& operator=(SafeArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            len = other.len;
            other.data = nullptr;
            other.len = 0;
        }
        return *this;
    }

    // 析构函数：释放堆内存
    ~SafeArray() { delete[] data; }

    // 返回数组长度
    size_t size() const { return len; }

    // 下标运算符（可读可写）：越界抛 out_of_range，报错信息包含实际下标和长度
    T& operator[](size_t i) {
        if (i >= len)
            throw out_of_range("SafeArray 下标越界: index=" + to_string(i)
                             + ", size=" + to_string(len));
        return data[i];
    }

    // const 重载：供 const 对象只读访问
    const T& operator[](size_t i) const {
        if (i >= len)
            throw out_of_range("SafeArray 下标越界: index=" + to_string(i)
                             + ", size=" + to_string(len));
        return data[i];
    }
};

int main() {
    // ---- 功能一 + 二：int 数组 + operator[]，越界报错 ----
    SafeArray<int> arr(5);
    for (size_t i = 0; i < arr.size(); ++i)
        arr[i] = static_cast<int>(i * 10);       // 通过 operator[] 写入

    cout << "int 数组内容: ";
    for (size_t i = 0; i < arr.size(); ++i)
        cout << arr[i] << ' ';                   // 通过 operator[] 读取
    cout << '\n';

    // 边界 1：下标等于 size（合法下标是 0 ~ size-1），越界访问被捕获，程序不崩溃
    try {
        cout << arr[5] << '\n';
    } catch (const out_of_range& e) {
        cout << "捕获越界异常: " << e.what() << '\n';
    }

    // 边界 2：负下标。int 的 -1 会隐式转成极大的 size_t，同样被越界检查拦下
    try {
        arr[-1] = 100;
    } catch (const out_of_range& e) {
        cout << "捕获负下标异常: " << e.what() << '\n';
    }

    // ---- 拷贝语义验证：修改副本不影响原数组 ----
    SafeArray<int> copy = arr;                   // 拷贝构造（深拷贝）
    copy[0] = 999;
    cout << "深拷贝验证 —— 副本 copy[0]=" << copy[0]
         << "，原数组 arr[0]=" << arr[0] << '\n';

    // ---- 功能三：模板实现类型无关，用 double 与 const 数组验证 ----
    SafeArray<double> darr(3);
    darr[0] = 1.5;
    darr[1] = 2.5;
    darr[2] = 3.5;
    cout << "double 数组内容: ";
    for (size_t i = 0; i < darr.size(); ++i)
        cout << darr[i] << ' ';
    cout << '\n';

    const SafeArray<int>& cref = arr;            // const 对象只能调用 const 版 operator[]
    cout << "const 数组只读访问 arr[4]=" << cref[4] << '\n';

    return 0;
}
