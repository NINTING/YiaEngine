# 谷歌风格C++规范
https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide

## 静态变量
1. 禁止 声明除函数内部以外的静态类(非原生数据类型 POD: Plain Old Data)
2. 包括了全局变量，静态变量，静态类成员变量和函数静态变量，都必须是原生数据类型 (POD : Plain Old Data): 
	即 int, char 和 float, 以及 POD 类型的指针、数组和结构体。