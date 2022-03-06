# chenleng

```C++
inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
    /*
    AF_INET
            src points to a character  string  containing  an  IPv4  network
            address  in  dotted-decimal format, "ddd.ddd.ddd.ddd", where ddd
            is a decimal number of up to three digits in the range 0 to 255.
            The  address is converted to a struct in_addr and copied to dst,
            which must be sizeof(struct in_addr) (4) bytes (32 bits) long.
    */
    cout<<"serv.sin_addr.sin_addr:"<<serv.sin_addr.s_addr<<endl;
```

result:
```shell
serv.sin_addr.sin_addr:16777343
socket-CRT: socket-CRT.cpp:52: int main(): Assertion `ret>=0' failed.
Aborted (core dumped)
```

reason:
covert is success but it is DEC not HEX