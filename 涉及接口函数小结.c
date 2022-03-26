// 网络编程中涉及到的接口函数小结

// 线程
#include <pthread.h>
int pthread_create(pthread_t * thread,const pthread_attr_t *attr,void *(*start_routine)(void*),void*arg);
    -功能：创建一个子线程
    -参数：
        pthread_t* thread：保存线程ID 
        const pthread_attr_t* attr：设置线程的属性，一般使用默认值NULL
        void *(*start_routine)(void*)：函数指针，农一个回调函数，子线程逻辑代码
        void*arg：给第三个参数使用，传参
void pthread_exit(void* retval);
    -功能：终止线程
    -参数：
        void* retval：传递一个指针，作为一个返回值，可以在pthread_join()中，常用NULL

pthread_t pthread_self(void);
    -功能：获取当前线程的线程ID

int pthread_equal(pthread_t t1,pthread_t t2);
    -功能：比较t1和t2之间的线程ID是否相等

int pthread_join(pthread_t thread, void **retval);
    -功能：和一个已经终止的线程进行连接;
        任何一个线程都可以回收其他线程，进程只能由他父亲回收
    这个函数是阻塞函数，调用一次只能回收一个子线程;
    一般在主线程中使用
    pthread_t thread：要回收的ID
    void **retval：接收子线程退出时的返回值，可以返回NULL
    这个是一个二级指针，对指针变量的地址进行改变
    return 0：成功

    用的时候这么用：
    int* thread_retval;
    ret = pthread_join(tid,(void**)&thread_retval);
    想要对thread_retval进行改变，就需要对其两个指针进行引用;

int pthread_detach(pthread_t thread);
    功能：分离一个线程，被分离的线程在终止的时候，会自动释放资源返回给系统;
    不能去连接一个已经分离的线程，不能多次分离，会产生不可预料的行为;

int pthread_cancel(pthread_t thread);
    功能：取消线程（让线程终止）;
    取消点，一些系统规定的取消点;
    并不是立刻终止，而是要在一个取消点终止;

//线程属性
设置线程属性的方法;
int pthread_attr_init(pthread_attr_t *attr);
    初始化线程属性的变量;
int pthread_attr_destory(pthread_attr_t* attr);
    释放线程属性资源;
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
    设置属性;
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);


