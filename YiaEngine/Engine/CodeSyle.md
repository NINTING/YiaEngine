# 谷歌风格C++规范
https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide

## 命名空间

1. 禁止使用using namespace,特别是在头文件，会造成由于头文件的引入而带来的命名空间污染

2. 不在头文件使用命名空间别名，除非显示在内部命名空间使用

3. 命名空间末尾注释命名空间名字

4. 使用匿名空间将外部不会使用的变量和函数抱起来。

5. 对于无需依赖外部变量的函数，使用命名空间而非作为新类的静态函数

   ```c++
   namespace Gemo{
      mesh Triangle();
      mesh Sphere();
      mesh Cube();
   }//Gemo
   ```

   



## 静态变量

由于静态变量构造和析构顺序的未定义，因此

1. 禁止 声明除函数内部以外的静态类(非原生数据类型 POD: Plain Old Data)
2. 包括了全局变量，静态变量，静态类成员变量和函数静态变量，都必须是原生数据类型 (POD : Plain Old Data): 
  即 int, char 和 float, 以及 POD 类型的指针、数组和结构体。
3. 确实需要一个 `class` 类型的静态或全局变量，可以考虑在 `main()` 函数或 `pthread_once()` 内初始化一个指针且永不回收。注意只能用 raw 指针，别用智能指针，毕竟后者的析构函数涉及到上文指出的不定顺序问题。





## 类

1. 禁止在构造函数中使用虚函数

### 类型转换

1. 类型转换运算符和单参数构造函数都应当用 `explicit`
2. 多参数构造函数和列表初始化构造函数不需要 explicit

## 拷贝/移动

1. 只要存在拷贝行为和

2. 基类需要拷贝功能时，只能定义copy()或protect下的拷贝/移动函数

3. 数据成员皆为private对象

### 结构体(struct)

当只有数据成员时，使用struct


### 变量名

1. 私有成员变量 memberVar_
2. 静态成员变量 私有： s_member_ 公有 s_Member
3. 函数变量 funcVar
