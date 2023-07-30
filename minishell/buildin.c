#include "buildin.h"
#include"parse.h"
#include"externs.h"
#include"def.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void do_exit();
void do_cd();
void do_type();

typedef void(*CMD_HANDLEER)(void);
typedef struct buildin_cmd
{
    char* name;
    CMD_HANDLEER handler;
}BUILDIN_CMD;

BUILDIN_CMD buildins[]={
    {"exit",do_exit},
    {"cd",do_cd},
    {"type",do_type},
    {NULL,NULL}
};


/*
    内部命令解析
    返回1表示内部命令,返回表示不是内部命令
*/
int buildin()
{
    /* if(check("exit"))
        do_exit();
    else if(check("cd"))
        do_cd();
    else 
        return 0; */
    int i=0;
    int found=0;
    int count=sizeof(buildins)/sizeof(buildins[0]);
    while(buildins[i].name)
    {
        if(check(buildins[i].name))
        {
            buildins[i].handler();
            found=1;
            break;
        }
        i++;
    }
    return found;
}

void do_exit()
{
    printf("exit");
    exit(EXIT_SUCCESS);
}

void do_cd()
{
    printf("do_cd...\n");
}

void do_type()
{
    printf("do_type...\n");
}