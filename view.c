#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<string.h>
#include<stdlib.h>
#define PATH_MAX 20
#define CMD_MAX 30
char cwd[PATH_MAX];
void view()
{
    printf("####### Welcome to shelldemo #########\n");

}
void pwd2()
{
    //char cwd[PATH_MAX];
    if(getcwd(cwd,sizeof(cwd))!=NULL)
    {
        printf("current directory:[%s]\n",cwd);
    }else{
        perror("getcwd error\n");
    }
}
void cd2(){}
void ls2(){}
void touch(){}
void echo2(){}
void cat2(){}
void cp2(){}
void rm2(){}
void rename2(){}
void history2(){}
void quit(){}
int main(int argc,char** argv)
{
    char cmd[CMD_MAX];
    printf("####### Welcome to shelldemo #########\n");
    printf("[%s]>>",cwd);
    fflush(stdout);
    return 0;
}