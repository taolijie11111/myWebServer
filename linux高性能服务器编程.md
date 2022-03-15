# linux高性能服务器编程————读书笔记

## 5-Linux网络基础编程API

### socket地址API

1. 大小端字节序转换
```C++
#include <netinet/in.h>
unsigned long int htonl(unsigned long int hostlong);
unsigned short int htos(unsigned short int hostshort);

```