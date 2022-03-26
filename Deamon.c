#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>

//没有写完 

void work(int num){
    time_t tm=time(NULL);
}

int main(){
    pid_t pid=fork();

    if(pid>0){
        _exit(0);
    }

    //将子进程重新创建一个会话

    setsid();

    umask(022);

    chdir("/");

    int fd=open("/dev/null",O_RDWR);
    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);

    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler=work;
    act.sa_flags=0;
    act.sa_flags=0;
    

    sigaction(SIGALRM,)


    //业务逻辑
    struct itimerval val;
    val.it_value.tv_sec=2;
    val.it_value.tv_usec=0;
    val.it_interval.tv_sec=2;
    val.it_interval.tv_usec=0;



    //创建定时器
    setitmer(ITIMER_REAL,&val,NULL)

    return 0;
}