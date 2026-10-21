#include <iostream>
#include <stdexcept>
#include <cstddef>
using namespace std;

// ============ 模板安全数组 SafeArray ============
// 功能：
//   1) 封装动态数组，下标越界时报错（抛 std::out_of_range）
//   2) 重载 operator[]，支持下标读写
//   3) 使用模板实现，数组元素类型无关（int / double / ... 均可）
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

    // 析构函数：释放堆内存
    ~SafeArray() { delete[] data; }

    // 返回数组长度
    size_t size() const { return len; }

    // 下标运算符（可读可写）：越界抛 out_of_range
    T& operator[](size_t i) {
        if (i >= len)
            throw out_of_range("SafeArray: 下标越界 (index >= size)");
        return data[i];
    }

    // const 重载：供 const 对象只读访问
    const T& operator[](size_t i) const {
        if (i >= len)
            throw out_of_range("SafeArray: 下标越界 (index >= size)");
        return data[i];
    }
};

int main() {
    // ---- 功能一 + 二：int 数组 + operator[]，越界报错 ----
    SafeArray<int> arr(5);
    for (size_t i = 0; i < arr.size(); ++i)
        arr[i] = (int)(i * 10);          // 通过 operator[] 写入

    cout << "int 数组内容: ";
    for (size_t i = 0; i < arr.size(); ++i)
        cout << arr[i] << ' ';           // 通过 operator[] 读取
    cout << '\n';

    // 越界访问：捕获异常并报错，程序不崩溃
    try {
        cout << arr[5] << '\n';          // 越界（合法下标 0~4）
    } catch (const out_of_range& e) {
        cout << "捕获越界异常: " << e.what() << '\n';
    }

    // ---- 功能三：模板 -> 用 double 验证类型无关 ----
    SafeArray<double> darr(3);
    darr[0] = 1.5;
    darr[1] = 2.5;
    darr[2] = 3.5;
    cout << "double 数组内容: ";
    for (size_t i = 0; i < darr.size(); ++i)
        cout << darr[i] << ' ';
    cout << '\n';

    return 0;
}
