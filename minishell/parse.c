#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<linux/limits.h>
#include<ulimit.h>
#include<signal.h>
#include"parse.h"
#include"def.h"
#include"init.h"
#include"externs.h"
#include "excute.h"
#include "buildin.h"


void print_command();


/*
    shell主循环
*/
void shell_loop()
{
    while(1)
    {   
        /*初始化环境*/
        init();
        /*读取命令*/
        if(read_command()==-1)
            break;
        /*解析命令*/
        parse_command();
        /*打印命令状态*/
        //print_command();
        /*执行命令*/
        excute_command();
    }
    printf("\nexit\n");
}

/*
    读取命令
    成功返回0，失败或者读取到文件结束符：-1
*/
int read_command()
{
    if(fgets(cmdline,MAXLINE,stdin)==NULL)
        return -1;
    return 0;
}

/*
    解析命令
    成功返回解析到的命令数量，失败：-1
    */
int parse_command()
{
    /*复杂命令解析*/
    
    if(check("\n"))
        return 0;
    /*判断是否是内部命令并执行它*/
    if(buildin())
        return 0;
    /* cat < test.txt | grep -n public > test2.txt & */
    //1.解析第一条简单命令
    int i=0;
    get_command(i++);
    //2.判断是否有输入重定向符
    if(check("<"))
        getname(infile);
    //3.判定是否有管道  
    

    for(i=1;i<PIPELINE;++i)
    {
        if(check("|"))
            get_command(i);
        else
            break;
    }
    //4.判定是否有输出重定向符
    if(check(">"))
    {
        if(check(">"))  /*以追加的方式重定向*/
            append=1;
        getname(outfile);
    }    
    //5.判定是否有后台作业符
    if(check("&"))
        backgnd=1;
    //6.判定命令结束
    if(check("\n"))
    {
        cmd_count=i;
        return cmd_count;
    }
    else
    {
        fprintf(stderr,"Command line syntax error\n");
        return -1;
    }
}

/*
    执行命令
    成功返回：0，失败返回：-1
*/
int excute_command()
{
    /*先判定是否是内部命令*/
    excute_command_external();
    return 0;
    
}

void print_command()
{
    int i;
    int j;
    printf("cmd_count=%d\n",cmd_count);
    if(infile[0]!='\0')
        printf("infile=[%s]\n",infile);
    if(outfile[0]!='\0')
        printf("outfile=[%s]\n",outfile);
    for(i=0;i<cmd_count;++i)
    {
        j=0;
        while(cmd[i].args[j]!=NULL)
        {
            printf("[%s] ",cmd[i].args[j]);
            j++;
        }
        printf("\n");
    }
}

/*
    解析简单命令至cmd[i]中
    提取cmdline命令参数到avline数组中
    并且将COMMAND结构中的args[]中的每个指针指向这些字符串
*/
void get_command(int i)
{
    /* cat < test.txt | grep -n public > test2.txt & */
    int j=0;
    int inword=0;
    while(*lineptr!='\0')
    {   
        /*去除空格*/
        while(*lineptr==' '|| *lineptr=='\t')
            lineptr++;
        /*将第i条命令第j个参数指向avptr*/
        cmd[i].args[j]=avptr;
        /*提取参数*/
        while(*lineptr!='\0' && *lineptr!=' ' && *lineptr!='\t' && *lineptr!='>' && *lineptr!='<' && *lineptr!='&' && *lineptr!='|' && *lineptr!='\n')
        {
            /*参数提取之avptr指针所指向的数组avline*/
            *avptr++=*lineptr++;
            inword=1;
        }    
        *avptr++='\0';
        switch (*lineptr)
        {
            case ' ' :
            case '\t':  j++; inword=0; break;
            case '<' :
            case '>' :
            case '|' :
            case '&' :
            case '\n': if(inword==0){cmd[i].args[j]=NULL;} return;
            default  : break;
        }
    }
}

/*
    将lineptr中的字符串与str进行匹配
    成功返回:1,同时lineptr移过所匹配的字符串,失败返回:-1,lineptr保持不变
*/
int check(const char* str)
{
    char* p;
    /*去除空格*/
    while(*lineptr==' ' || *lineptr=='\t')
    {
        lineptr++;
    }
    p=lineptr;
    while(*str!='\0' && *str==*p)
    {
        str++;
        p++;
    }
    if(*str=='\0')
    {
        lineptr=p;  /*lineptr移过所匹配的字符串*/
        return 1;
    }
    /*lineptr保持不变*/
    return 0;
}

void getname(char* name)
{
    while(*lineptr==' ' || *lineptr=='\t')
        lineptr++;
    while(*lineptr!='\0' && *lineptr!=' ' && *lineptr!='\t' && *lineptr!='>' && *lineptr!='<' && *lineptr!='&' && *lineptr!='|' && *lineptr!='\n')
        *name++=*lineptr++;
    *name='\0';
}

