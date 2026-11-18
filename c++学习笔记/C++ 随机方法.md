# C++ 随机方法

## 一、C++生成随机数过程

计算机所产生的随机数其实并不是真正随机的，而是伪随机的。当**我们在使用rand()生成随机数时得到的实际上是从由随机数种子生成的一个数字序列中生成的一个数**，该数字序列的长度一般是**65535**。也就是说一个固定的随机数种子生成的数字序列长度一般是65535，当我们在这个数字序列中生成随机数时，如果已经得到了65535个随机数那么接下来生成的随机数会和之前的随机数重复。简单点说就是如果不改变随机数种子的话，那么生成随机数的过程就是每次在一个固定的数字序列中拿出一个数(而且经过我多次试验发现每次还都是按顺序生成的，即有规律可循)。

## 二，常用方法

（零）.比赛用

```c
#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    // 生成 [l, r] 内的一个整数
    long long l = 1, r = 1000;
    long long x = uniform_int_distribution<long long>(l, r)(rng);

    // 生成 [0, 1] 的实数
    double p = uniform_real_distribution<double>(0.0, 1.0)(rng);
    return 0;
}
```



### （一）.cstdlib库（zhen de shi)

```c++
#include<cstdlib>
#include<ctime>
using namespace std;

int main(){
    srand(time(0));
    
    cout << "【0~9】：" << rand()%10 << endl;
    cout << "【1~10】：" << rand()%(10-1+1)+1 << endl; // [1,10]
    cout << "浮点数0~1：" << rand() / double(RAND_MAX) << endl;
    
    return 0;
}
```

### (二)，random库

在C++11以前，C和C++都使用相同的方式来生成随机数。而C++11提供了随机数库，其中包括**随机数引擎类**和**随机数分布类**。

#### （1）随机数引擎类

```

```

