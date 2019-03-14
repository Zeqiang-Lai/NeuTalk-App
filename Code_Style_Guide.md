# C++&&C代码规范

*作者：赖泽强*

请务必保持自己的代码清晰美观易读！！！

下面是几个重点：

## 命名

### 文件命名

1. 文件名全部小写
2. 使用有意义的英文单词（简写）进行命名
3. 使用下划线 `_` 进行单词的分割

**例子：**

- `client.h`  `client.cpp`
- `main.cpp`

**Note：**

1. 类的头文件与实现文件同名

### 类型命名

1. 类型包括：类，结构体，枚举，模版等
2. 所有名字均以大写字母开始，然后每个单词首字母均大写，不包含下划线。

```c++
// 类和结构体
class UrlTable { ...
class UrlTableTester { ...
struct UrlTableProperties { ...

// 类型定义
typedef hash_map<UrlTableProperties *, string> PropertiesMap;

// using 别名
using PropertiesMap = hash_map<UrlTableProperties *, string>;

// 枚举
enum UrlTableErrors { ...
```

### 变量命名

1. 一律小写
2. 单词间用下划线连接

#### 普通变量命名

举例:

```c++
string table_name;  // 好 - 用下划线.
string tablename;   // 好 - 全小写.

string tableName;  // 差 - 混合大小写
```

#### 类数据成员

不管是静态的还是非静态的, 类数据成员都可以和普通变量一样, 但前面要加m_.

```c++
class TableInfo {
  ...
 private:
  string m_table_name;  // 好
  string m_tablename;   // 好.
  static Pool<TableInfo>* m_pool;  // 好.
};
```

#### 结构体变量

不管是静态的还是非静态的, 结构体数据成员都可以和普通变量一样, 不用像类那样接下划线:

```c++
struct UrlTableProperties {
  string name;
  int num_entries;
  static Pool<UrlTableProperties>* pool;
};
```

### 常量命名

1. 常量命名以“k”开头
2. 大小写混合

```c++
const int kDaysInAWeek = 7;
```

### 函数命名

1. 常规函数使用大小写混合
2. 取值和设值函数的命名与变量一致，使用全小写+下划线

```c++
// 常规
void AddTableEntry()
void DeleteUrl()
void OpenFileOrDie()
// 取值,设值
int count()
void set_count(int count)
```



## 缩进与括号

1. 使用tab进行缩进

**括号形式：**

短函数，短循环不换行。

```c++
// 类
class Test 
{
    // 第一个括号换行,与类名有一个空格
};
// 函数
void func(int a, int b) 
{
    // 同上
    // 参数之间逗号后有一个空格
}
// for循环
for() 
{
    
}
// while循环
while() 
{
    
}
```

**空格的使用：**

```c++
// 变量声明
int a, b, c; // 逗号后有一个空格
// 指针和引用
int *b; // *号跟在变量名前
int &c; // &号跟在变量名前
// for循环
for(int i=1; i<=n; ++i) // 注意空格
```



## 注释

请在必要的地方进行注释，如果你纠结要不要注释？请写上注释。

**统一使用英文标点！！**

统一使用 `//` 进行注释

参考: [注释](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/comments/)

> 新内容！！！往下翻直到看到“新内容结束” #2018-8-31

### 文件注释

文件开头著名作者信息。

格式如下：

```c++
//
// 文件名.cpp / 文件名.h
// NeuTalk
//
// 作者: 张三
// 创建日期: 2018-8-23
// 联系方式: 123456@qq.com
//
```

### 类注释

每个类的定义都要附带一份注释, 描述类的功能和用法, 除非它的功能相当明显.

```c++
// NeuTalk服务器端, 处理客户端间的通信...
// 作者: 张三
// 完成日期: 2018-8-23
class Server {
    
};
```

类成员变量也要注释，要求见变量注释。

### 函数注释

**函数声明处**的注释描述函数功能; **定义处的**注释描述函数实现。

很简单的函数可以不注释。

**函数声明**处注释的内容:

- 描述函数功能，输入输出
- 参数的含义，如果可以从名字看出来则可以不注释，

格式如下:

```c++
//
// 这个函数是个测试函数,测试加法是否正确
//   - num1: 第一个加数 
//   - num2: 第二个加数
// 
// 作者: 张三
// 日期: 2018-8-23
int add(int num1, int num2);
```

**函数定义**

如果函数的实现过程中用到了很巧妙的方式, 那么在函数定义处应当加上解释性的注释. 例如, 你所使用的编程技巧, 实现的大致步骤, 或解释如此实现的理由. 举个例子, 你可以说明为什么函数的前半部分要加锁而后半部分不需要.

*不要* 从 `.h` 文件或其他地方的函数声明处直接复制注释. 简要重述函数功能是可以的, 但注释重点要放在如何实现上.

### 变量注释

通常变量名本身足以很好说明变量用途. 某些情况下, 也需要额外的注释说明.

**类数据成员**

每个类数据成员 (也叫实例变量或成员变量) 都应该用注释说明用途. 参考内容如下：

- 特殊用法，特殊取值
- 无法直接看出含义的变量
- ...

特殊取值注释范例：

```c++
private:
 // Used to bounds-check table accesses. -1 means
 // that we don't yet know how many entries the table has.
 int num_total_entries_;
```

**全局变量**

和数据成员一样, 所有全局变量也要注释说明含义及用途, 以及作为全局变量的原因. 比如:

```
// The total number of tests cases that we run through in this regression test.
const int kNumTestCases = 6;
```

### 实现注释

对于代码中巧妙的, 晦涩的, 有趣的, 重要的地方加以注释.

**不要写一些显而易见的注释，如果犹豫了，选择注释。**

**1. 代码前注释**

巧妙或复杂的代码段前要加注释. 比如:

```c++
// Divide result by two, taking into account that x
// contains the carry from the add.
for (int i = 0; i < result->size(); i++) {
  x = (x << 8) + (*result)[i];
  (*result)[i] = x >> 1;
  x &= 1;
}
```

**2. 行注释**

比较隐晦的地方要在行尾加入注释. 在行尾空两格进行注释. 比如:

```c++
// If we have enough memory, mmap the data portion too.
mmap_budget = max<int64>(0, mmap_budget - index_->length());
if (mmap_budget >= data_size_ && !MmapData(mmap_chunk_bytes, mlock))
  return;  // Error already logged.
```

注意, 这里用了两段注释分别描述这段代码的作用, 和提示函数返回时错误已经被记入日志.

如果你需要连续进行多行注释, 可以使之对齐获得更好的可读性:

### TODO注释

**总述**

对那些临时的, 短期的解决方案, 或已经够好但仍不完美的代码使用 `TODO` 注释.

- `TODO` 注释要使用全大写的字符串 `TODO`,

- 在随后的圆括号里写上你的名字, 邮件地址, bug ID, 或其它身份标识和与这一 `TODO` 相关的 issue. 主要目的是让添加注释的人 (也是可以请求提供更多细节的人) 可根据规范的 `TODO` 格式进行查找. 添加 `TODO` 注释并不意味着你要自己来修正, 因此当你加上带有姓名的 `TODO` 时, 一般都是写上自己的名字.

```c++
// TODO(kl@gmail.com): Use a "*" here for concatenation operator.
// TODO(Zeke) change this to use relations.
// TODO(bug 12345): remove the "Last visitors" feature
```

> 新内容结束。#2018-8-31

## 头文件

### 使用#define进行保护

**目的：**避免多次include

```c++
#ifndef CLINET_H_
#define CLIENT_H_
...
#endif // CLIENT_H_
```

**说明：**

- CLINET_H_ 为头文件名称+下划线+H+下划线
- 全部大写

### include顺序

1. 先include系统头文件
2. 再include自己的头文件

```c++
#include <iostream>
#include <string>

#include "client.h"
```

## 最佳实践

1. 能不用全局变量就不用

## 参考文档

[Google 开源项目风格指南 (中文版)](https://zh-google-styleguide.readthedocs.io/en/latest/)

