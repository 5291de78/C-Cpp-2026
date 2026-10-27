#include "miller.h"
#include <cctype>
#include <iostream>
#include <string>
using namespace std;

string input = "";                  // 当前待检测的数字串（greeting 校验通过后才有效）
long long smaller_num = 0;          // 10 位以内：数值较小，用试除法
unsigned long long big_num = 0;     // 11~20 位：数值很大，用 Miller-Rabin

void config()
{
    input.clear();
    smaller_num = 0;
    big_num = 0;
}

bool greeting()
{
    cout << "pls enter a positive integer number: ";
    if (!(cin >> input))
    {
        // 读到 EOF 或输入流错误
        return false;
    }
    // 逐字符检查是否全为数字，避免 stoll/stoull 解析非数字时抛异常崩溃
    for (char c : input)
    {
        // 注意：char 可能为负（如 UTF-8 字节），必须强转为 unsigned char 再交给 isdigit
        if (!isdigit(static_cast<unsigned char>(c)))
        {
            cout << "error: input is not a positive integer ):" << endl;
            return false;
        }
    }
    return true;
}

// ---------- 无溢出快速乘 / 快速幂 ----------

#if !defined(__SIZEOF_INT128__)    // 仅 MSVC 等不支持 128 位整数的编译器需要这个辅助函数
// 计算 (x + y) % m，其中 x、y 均小于 m。
// 若直接写 (x + y) % m，当 x + y >= 2^64 时会溢出（无符号数回绕，结果错误），
// 因此改用减法判断：x + y >= m 时结果为 x - (m - y)，全程最大值不超过 m。
static unsigned long long addmod(unsigned long long x, unsigned long long y, unsigned long long m)
{
    return (x >= m - y) ? x - (m - y) : x + y;
}
#endif

// 快速乘：返回 (a * b) % n
unsigned long long qmul(unsigned long long a, unsigned long long b, unsigned long long n)
{
#if defined(__SIZEOF_INT128__)            // GCC / Clang 支持 128 位整数，直接乘取模最快
    return static_cast<unsigned long long>(
        (static_cast<unsigned __int128>(a % n) * (b % n)) % n);
#else                                     // 其他编译器（如 MSVC）：退回二进制快速加
    a %= n;
    unsigned long long res = 0;
    while (b > 0)
    {
        if (b & 1) res = addmod(res, a, n);
        a = addmod(a, a, n);
        b >>= 1;
    }
    return res;
#endif
}

// 快速幂：返回 a^b % n，把指数 b 按二进制位拆分，乘法会调用无溢出的 qmul
unsigned long long qpow(unsigned long long a, unsigned long long b, unsigned long long n)
{
    unsigned long long res = 1 % n;
    a %= n;
    while (b > 0)
    {
        if (b & 1) res = qmul(res, a, n);
        a = qmul(a, a, n);
        b >>= 1;
    }
    return res;
}

// ---------- Miller-Rabin 素性检测 ----------

// 确定性 Miller-Rabin：用前 12 个素数作底数，
// 对 n < 3,317,044,064,679,887,385,961,981（约 3.3e24）以内的数判定绝对正确，
// 完全覆盖 unsigned long long 的范围（最大约 1.8e19）。
void Miller_Rabin(unsigned long long n)
{
    if (n < 2)
    {
        cout << "The number is not a prime." << endl;
        return;
    }
    if (n == 2 || n == 3)
    {
        cout << "The number is a prime." << endl;
        return;
    }

    // 把 n-1 分解为 d * 2^s（n 为奇数，所以 d 是奇数）
    unsigned long long d = n - 1;
    int s = 0;
    while (d % 2 == 0)
    {
        d /= 2;
        s += 1;
    }

    static const unsigned long long base_list[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(base_list) / sizeof(base_list[0]); i++)
    {
        unsigned long long a = base_list[i];
        if (a >= n) continue;                    // 底数不小于 n 时跳过该底数

        unsigned long long x = qpow(a, d, n);
        if (x == 1 || x == n - 1) continue;      // 通过本底数的检验

        // 反复平方，只要中途出现 n-1 即通过本底数
        for (int r = 1; r < s; r++)
        {
            x = qmul(x, x, n);
            if (x == n - 1) break;
        }
        if (x != n - 1)                          // 平方到最后仍不是 n-1：确定是合数
        {
            cout << "The number is composite." << endl;
            return;
        }
    }
    cout << "The number is a prime." << endl;
}

// ---------- 主入口：按位数分流 ----------

void is_prime()
{
    // 先去掉前导零，再按位数分流。
    // 否则 "0007" 会走大数路径，"00000000000000000000007"（23 位）会被误判为"太大"。
    size_t pos = input.find_first_not_of('0');
    string digits = (pos == string::npos) ? "0" : input.substr(pos);

    // 1) 单个数字：直接查表（0、1 不是素数；2、3、5、7 是素数；其余偶数/9 不是）
    if (digits.length() == 1)
    {
        switch (digits[0])
        {
        case '2': case '3': case '5': case '7':
            cout << "The number is a prime." << endl;
            break;
        default:
            cout << "The number is not a prime." << endl;
        }
        return;
    }

    // 2) 末位是偶数：大于 2 的偶数必为合数（O(1) 直接排除，约省一半工作量）
    if ((digits.back() - '0') % 2 == 0)
    {
        cout << "The number is not a prime." << endl;
        return;
    }

    // 3) 10 位以内：试除法即可。
    //    奇数只需试除奇因子，且循环条件用 d <= n/d 代替 d*d <= n 防止乘法溢出
    if (digits.length() <= 10)
    {
        smaller_num = stoll(digits);
        bool prime = true;
        for (long long d = 3; d <= smaller_num / d; d += 2)
        {
            if (smaller_num % d == 0)
            {
                prime = false;
                break;
            }
        }
        cout << (prime ? "The number is a prime." : "The number is composite.") << endl;
        return;
    }

    // 4) 11~20 位：尝试读入 unsigned long long。
    //    20 位数字可能超过 ULLONG_MAX（18446744073709551615），解析失败即说明超出检测范围
    try
    {
        big_num = stoull(digits);
    }
    catch (const out_of_range&)
    {
        cout << "The number is too large to check." << endl;
        return;
    }
    Miller_Rabin(big_num);
}
