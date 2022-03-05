#include <iostream>
using namespace std;
// 联合体占用内存的空间为每个成员字节长度的最大值
union endian
{
    int a;
    char b;
};
int main()
{
    endian e;
    e.a = 0x1234;
    // a 和 b 共用 4 字节的内存空间
    if (e.b == 0x12)
        cout << "big endian"<<endl;
    else if (e.b == 0x34)
        cout << "little endian"<<endl;
}
