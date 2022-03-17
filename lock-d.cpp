#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include <iostream>

/**
 * 死锁原理：
 * 先在主线程中占有a锁，然后后操作被该锁保护的变量a，操作完毕后a和b一起释放
 * 连续两次调用lock
 * 导致两个线程先占有一个锁，然后期待另一个锁，最终两个相互僵持，变成死锁
 */

int a=0;
int b=0;
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;

void* another(void* arg){
    pthread_mutex_lock(&mutex_b);
    std::cout<<"in child thread,got mutex b,waiting for mutex a\n"<<std::endl;
    sleep(5);
    ++b;
    pthread_mutex_lock(&mutex_a);
    b+=a++;
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
    pthread_exit(NULL);
}

int main()
{
    pthread_t id;

    pthread_mutex_init(&mutex_a,NULL);
    pthread_mutex_init(&mutex_b,NULL);
    pthread_create(&id,NULL,another,NULL);

    pthread_mutex_lock(&mutex_a);
    std::cout<<"in parent thread,got mutex a,waiting for mutex b\n"<<std::endl;
    sleep(5);
    ++a;
    pthread_mutex_lock(&mutex_b);
    a+=b++;
    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);

    pthread_join(id,NULL);
    pthread_mutex_destroy(&mutex_b);
    pthread_mutex_destroy(&mutex_a);
    return 0;
}