#ifndef MILLER_H       // 防止重复包含的头文件保护
#define MILLER_H

void config();
bool greeting();  // 读取并校验输入，合法返回 true，非法或 EOF 返回 false
void is_prime();
void Miller_Rabin(unsigned long long n);
unsigned long long qmul ( unsigned long long a, unsigned long long b, unsigned long long n) ; // 快速乘 a*b mod n 
unsigned long long qpow ( unsigned long long a, unsigned long long b, unsigned long long n) ; // 快速幂 a^b mod n

#endif