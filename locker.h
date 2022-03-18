#pragma once


//实现互斥锁
//连接
//https://mp.weixin.qq.com/s?__biz=MzAxNzU2MzcwMw==&mid=2649274278&idx=3&sn=5840ff698e3f963c7855d702e842ec47&chksm=83ffbefeb48837e86fed9754986bca6db364a6fe2e2923549a378e8e5dec6e3cf732cdb198e2&scene=0&xtrack=1#rd
//实现RALL机制


#include <semaphore.h>
#include <exception>//for exception

#include <pthread.h>

/**
 * #include <semaphore.h>

       int sem_init(sem_t *sem, int pshared, unsigned int value);

       Link with -pthread.

 *  #include <semaphore.h>

       int sem_init(sem_t *sem, int pshared, unsigned int value);

       Link with -pthread.

 */

class sem{
public:
    //构造函数
    sem()
    {
        //信号量初始化
        //高性能服务器P275页
        //pshared为0表示信号量是当前进程的局部信号量
        if(sem_init(&m_sem,0,0)!=0)throw std::exception();//异常处理语句
    }
    sem(int num){//给一个进程初始值
        if(sem_init(&m_sem,0,num)!=0){
            throw std::exception();
        }
    }
    //析构函数
    ~sem(){
        //信号量摧毁
        sem_destroy(&m_sem);
    }
    bool wait(){
        //以原子操作的方式，将信号量-1，如果信号量为0，sem_wait 将被阻塞，直到这个信号量具有非0值
        return sem_wait(&m_sem);
    }
    bool post(){
        //以原子操作的方式将信号量+1，信号量>0时，其他正在调用sem_wait等待信号量的线程将被唤醒
        return sem_post(&m_sem);
    }
private:
    sem_t m_sem;
};


class locker
{
public:
    locker(){
        //初始化互斥锁
        if(pthread_mutex_init(&m_mutex,NULL)!=0)throw std::exception();
    }
    ~locker(){
        pthread_mutex_destroy(&m_mutex);//销毁互斥锁
    }
    //成功返回0，不成功返回错误码
    bool lock(){
        return pthread_mutex_lock(&m_mutex)==0;//以原子操作的方式给一个互斥锁加锁
    }
    bool unlock(){
        return pthread_mutex_unlock(&m_mutex)==0;//解锁
    }
    pthread_mutex_t *get(){
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};

/**
 * cond条件变量
 * 提供线程间的通讯机制，当某个共享数据达到某个值的时候，唤醒等待这个共享数据的线程
 * 
 */

class cond
{
public:
    cond(){
        //初始化
        if(pthread_cond_init(&m_cond,NULL)!=0)throw std::exception();
    }
    ~cond(){
        //删除
        pthread_cond_destroy(&m_cond);
    }
    bool wait(pthread_mutex_t *m_mutex){
        int ret=0;
        //等待条件目标变量，当其成功返回时，目标锁再次被锁上
        ret=pthread_cond_wait(&m_cond,m_mutex);

        return ret==0;
    }
    bool timewait(pthread_mutex_t *m_mutex,struct timespec t){
        int ret=0;
        // 线程等待一定的时间，如果超时或有信号触发，线程唤醒
        ret=pthread_cond_timedwait(&m_cond,m_mutex,&t);

        return ret==0;
    }
    bool signal(){
        return pthread_cond_signal(&m_cond)==0;
    }

    bool broadcast()
    {
        //以广播的形式唤醒所有等待目标条件变量的线程
        //线程等待信号触发，如果没有信号触发，无限期等待下去。
        return pthread_cond_broadcast(&m_cond)==0;
    }
private:
    pthread_cond_t m_cond;
};