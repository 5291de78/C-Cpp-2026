#ifndef MILLER_H            // 头文件保护宏：防止本文件被重复包含
#define MILLER_H

void config();              // 重置全局状态，开始新一轮检测
bool greeting();            // 读取并校验输入：合法返回 true；非法输入或 EOF 返回 false
void is_prime();            // 素数检测主入口：按位数把输入分流到不同算法
void Miller_Rabin(unsigned long long n);
// 快速乘：返回 (a*b) % n，全程不发生溢出（模幂的底层积木）
unsigned long long qmul(unsigned long long a, unsigned long long b, unsigned long long n);
// 快速幂：返回 a^b % n，全程不发生溢出
unsigned long long qpow(unsigned long long a, unsigned long long b, unsigned long long n);

#endif
