#ifndef _PARSE_H__
#define _PARSE_H__

/*
    1.语法处理
    2.别名处理
    3.路径扩展
    4.通配处理
    5.算术处理
    6.变量处理
    7.作业控制
*/

void shell_loop();
int read_command();
int parse_command();
int excute_command();

void get_command(int);
int check(const char*);
void getname(char* name);


#endif