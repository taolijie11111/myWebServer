#pragma once
#include "locker.h"
#include <list>
#include <pthread.h>
#include <exception>
#include <iostream>
#include <cstdio>
//线程池类，为了代码的复用
template<typename T>
class threadpool{
public:
    threadpool(int thread_number=8,int max_requests=10000);
    ~threadpool();

    bool append(T* request);

private:
    static void* worker(void* arg);

    void run();

private:
    //线程的数量
    int m_thread_number;
    //线程池数组，大小为数量
    pthread_t* m_threads;
    //请求队列,最多允许的等待处理的请求数量
    int m_max_requests;
    std::list<T*> m_workqueue;
    //互斥锁
    locker  m_queuelocker;
    //信号量用来判断是否有人物需要处理
    sem m_queuestat;
    //是否结束线程
    bool m_stop;
};

//构造函数
template<typename T>
threadpool<T>::threadpool(int thread_number,int max_requests):
m_thread_number(thread_number),m_max_requests(max_requests),m_stop(false),m_threads(NULL){
    if((thread_number<=0)||(max_requests<=0)){
        throw exception();
    }
    m_threads= new pthread_t[m_thread_number];
    if(!m_threads){
        throw exception();
    }

    //创建thread_number个线程，并将他们设置为线程脱离
    for(int i=0;i<thread_number;++i){
        cout<< "create the %d thread"<<endl;
        
        if((pthread_create(m_threads+i,NULL,worker,this))!=0){
            delete[] m_threads;
            throw exception();
        }
        if(pthread_detach(m_threads[i])){
            delete[] m_threads;
            throw exception();
        }
        
    }
}

//xigouhanshu
template<typename T>
threadpool<T>::~threadpool(){
    delete[] m_threads;
    m_stop=true;
}

template<typename T>
bool threadpool<T>::append(T* request){
    m_quuelocker.lock();
    if(m_workqueue.size()>m_max_requests){
        m_quuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_quuelocker.unlock();
    m_queuestat.post();//信号量判断需不需要人来执行
    return true;
}

template<typename T>
void* threadpool<T>::worker(void* arg){
    threadpool* pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run(){
    while (!m_stop)
    {
        m_queuestat.wait();//如果信号量有值就会阻塞
        m_queuelocker.lock();
        if(m_workqueue.unlock()){
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();//取出第一个
        m_queuelocker.unlock();

        if(!request){
            continue;
        }
        request->process();
    }
    
}