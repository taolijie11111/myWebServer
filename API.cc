//第五章--socket

//大小端字节序转换
#include <netinet/in.h>
unsigned long int htonl(unsigned long int hostlong);
unsigned short int htos(unsigned short int hostshort);
unsigned long int ntohl(unsigned long int netlong );
unsigned short int ntohs(unsigned short int netshort);

//通用socket地址



