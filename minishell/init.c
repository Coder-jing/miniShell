#include<signal.h>
#include<stdio.h>
#include<string.h>
#include "init.h"
#include "externs.h"
#include "def.h"


void sigint_handler(int sig);
/*
    初始化minishell,安装信号函数
*/
void setup()
{
    signal(SIGINT,sigint_handler);
    signal(SIGQUIT,SIG_IGN);
}

void sigint_handler(int sig)
{
    printf("\n[minishell]$ ");
    fflush(stdout);
}

void init()
{
    memset(cmd,0,sizeof(cmd));         /*初始化解析到的命令行*/
    for(int i=0;i<PIPELINE;i++)
    {
        cmd[i].infd=0;
        cmd[i].outfd=1;
    }
    memset(cmdline,0,sizeof(cmdline));  /*初始化读取到的命令行*/
    memset(avline,0,sizeof(avline));    /*初始化解析到的参数*/
    lineptr=cmdline;
    avptr=avline;
    memset(infile,0,sizeof(infile));
    memset(outfile,0,sizeof(outfile));
    cmd_count=0;
    backgnd=0;
    append=0;
    lastpid=0;
    
    printf("[minishell]$ ");
    fflush(stdout);
}

