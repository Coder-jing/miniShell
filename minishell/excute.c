#include "excute.h"
#include "def.h"
#include "externs.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<linux/limits.h>
#include<ulimit.h>
#include<signal.h>

int excute_command_external()
{
    if(cmd_count==0)
        return 0;
    /*ls | grep init | wc -w*/
    
    if(infile[0]!='\0')
    {
        cmd[0].infd=open(infile,O_RDONLY);
    }
    if(outfile[0]!='\0')
    {
        if(append)
            cmd[cmd_count-1].outfd=open(outfile,O_WRONLY|O_CREAT|O_APPEND,0666);
        else
            cmd[cmd_count-1].outfd=open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0666);
    }
    /*因为后台作业不会调用wait等待子进程退出,为避免僵尸进程,可以忽略SIGCHID信号*/
    if(backgnd)
        signal(SIGCHLD,SIG_IGN);
    else
        signal(SIGCHLD,SIG_DFL);
    int i;
    int fds[2];
    int fd;
    for(i=0;i<cmd_count;i++)
    {
        /*如果不是最后一条命令,则需要创建管道*/
        if(i<cmd_count-1)
        {
            pipe(fds);
            cmd[i].outfd=fds[1];
            cmd[i+1].infd=fds[0];
        }
        forkexec(&cmd[i]);
        if((fd=cmd[i].infd)!=0)
            close(fd);
        if((fd=cmd[i].outfd)!=1)
            close(fd);
    }
    if(backgnd==0)
    {
        /*前台作业,等待最后一个进程退出*/
        while(wait(NULL)!=lastpid);
    }
    return 0;
}

void forkexec(COMMAND* pcmd)
{
    int i=0;
    pid_t pid;
    pid=fork();
    if(pid<0)
        ERR_EXIT("fork");
    if(pid==0)
    {
        /* backgnd=1时,将第一条简单命令的infd重定向至/dev/null */
        /* 当地一条命令试图从标准输入获取数据时立即返回EOF*/
        if(pcmd->infd==0 && backgnd==1)
        {
            pcmd->infd=open("/dev/null",O_RDONLY);
        }
        /*将第一个简单命令进程作为进程组组长*/
        if(i==0)
            setpgid(0,0);
        if(pcmd->infd!=0)
        {
            close(0);
            dup(pcmd->infd);
        }
        if(pcmd->outfd!=1)
        {
            close(1);
            dup(pcmd->outfd);
        }
        for(i=3;i<_SC_OPEN_MAX;i++)
        {
            close(i);
        }
        /*前台作业能够结束SIGINT,SIGQUIT信号
        这两个信号恢复默认操作*/
        if(backgnd==0)
        {
            signal(SIGINT,SIG_DFL);
            signal(SIGQUIT,SIG_DFL);
        }
        execvp(pcmd->args[0],pcmd->args);
        ERR_EXIT("execvp");
    }
    else if(pid>0)
    {
        if(backgnd==1)
            printf("%d\n",pid);
        lastpid=pid;
    }
}