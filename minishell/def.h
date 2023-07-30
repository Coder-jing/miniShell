#ifndef _DEF_H__
#define _DEF_H__

#include<stdlib.h>

#define MAXLINE 1024    /*输入行的最大长度*/
#define MAXARG 20       /*每个简单命令的最多参数个数*/
#define PIPELINE 5      /*一个管道中简单命令的最大个数*/
#define MAXNAME 100     /*IO重定向文件名的最大长度*/
#define ERR_EXIT(m) \
    do \
    {   \
        perror(m);   \
        exit(EXIT_FAILURE); \
    }   \
    while(0)

typedef struct command
{
    char* args[MAXLINE];  /*解析出的简单命令参数列表*/
    int infd;
    int outfd;
}COMMAND;

#endif