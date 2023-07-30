#ifndef _EXTERNS_H__
#define _EXTERNS_H__

#include"def.h"

extern char cmdline[MAXLINE+1];
extern char avline[MAXLINE+1];
extern char* lineptr;
extern char* avptr;
extern COMMAND cmd[PIPELINE];

extern char infile[MAXNAME+1];
extern char outfile[MAXNAME+1];

extern int cmd_count;
extern int backgnd;
extern int append;
extern int lastpid;

#endif