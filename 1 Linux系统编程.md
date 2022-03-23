# Linux系统编程
## 1.1 Linux开发环境搭建
* Linux--需要远程连接服务端，还有弄一个openssn-server，但是我这边已经在有了，就不用作那个了
* 生成公钥和私钥就可以直接远程连接虚拟机
* 远程连接为了方便文件传输
  
## GCC

* GCC编译流程：
  源代码->预处理器（过滤注释，展开头文件等）.i-> 编译器 ->汇编代码.s->汇编器->.o->链接器->.out
  ```
  gcc test.c -E -o test.i
  gcc test.i -S -o test.s
  gcc test.s -s -o test.o
  gcc test.s -S -o test.s
  ```

* 误区：gcc和g++的主要区别在于gcc不能自动和C++程序使用的库连接，所以通常g++来完成编译连接cpp文件，其实gcc和g++都可以用来编译cpp/c文件
* g++在编译阶段会使用gcc，但是在后面连接库的阶段就只能是g++
* gcc不会定义__cplusplus宏，而g++会（错误）-------后缀为.c且采用gcc编译，宏才是未定义的

```
gcc test.c -o test -d DEBUG //表示运行宏DEBUG里面的内容，  #ifdef DEBUG  ……   #enif 
```

## 制作静态库
gcc 获得 .o 文件
ar rcs libxxx.a xxx.o xxx.o
r -将文件插入被存文件中
c -建立被存文件
s -索引

## 文件描述符
文件描述符是I/O函数的一个返回指针值---FTLE