#include "threadpool.h"
#include <iostream>
#include  <memory>
using namespace std;

ThreadPool* thrPool=nullptr;
int beginnum=1000;

void* thrRun(void *arg)
{
    ThreadPool *pool=(ThreadPool* )arg;
    int taskpos=0;
    PoolTask *task=(PoolTask*)malloc(sizeof(PoolTask));

    while(1)
    {
        pthread_mutex_lock(&thrPool->pool_lock);

        while (thrPool->job_num<=0 && !thrPool->shutdown)
        {
            /* code */
            
        }
        
    }
}

void addtask(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->pool_lock);
    while(pool->max_job_num<=pool->job_num){
        pthread_cond_wait(&pool->empty_task,&pool->pool_lock);
    }
    int taskpos=(pool->job_push++)%pool->max_job_num;
    pool->tasks[taskpos].tasknum=beginnum++;
    pool->tasks[taskpos].arg=(void*)&pool->tasks[taskpos];
    pool->tasks[taskpos].task_func=taskRun;
    pool->job_num++;

    pthread_mutex_unlock(&pool->pool_lock);

    
}
void create_threadpool(int thrnum,int maxtasknum)
{
    cout<<"begin call "<<__FUNCTION__<<endl;
    thrPool=(ThreadPool*)malloc(sizeof(ThreadPool));

    thrPool->thr_num=thrnum;
    thrPool->max_job_num=maxtasknum;
    thrPool->shutdown=0;
    thrPool->job_push=0;
    thrPool->job_pop=0;
    thrPool->job_num=0;

    thrPool->tasks=(PoolTask*)malloc((sizeof(PoolTask)*maxtasknum));

    pthread_mutex_init(&thrPool->pool_lock,NULL);
    pthread_cond_init(&thrPool->empty_task,NULL);
    pthread_cond_init(&thrPool->not_empty_task,NULL);

    int i=0;
    thrPool->threads=(pthread_t *)malloc(sizeof(pthread_t)*thrnum);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    for(i=0;i<thrnum;i++){
        pthread_create(&thrPool->threads[i],&attr,thrRun,(void*)thrPool);
    }

}




int main()
{
    create_threadpool(3,20);
    int i=0;
    for(i=0;i<50;i++)
    {
        addtask(thrPool);
    }
    sleep(20);
    destory_threadpool(thrPool);
    
    return 0;
}