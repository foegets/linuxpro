#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#define PATH_MAX 20
#define CMD_MAX 30
const int cnum=11; //number of cmd
char cwd[PATH_MAX];
char* cmd_argv[CMD_MAX];
char allcmd[cnum][CMD_MAX]={"pwd2","cd2","ls2","touch2","echo2","cat2",
"cp2","rm2","rename2","history2","quit"};
void mygetcwd()
{
    if(getcwd(cwd,sizeof(cwd))==NULL)
    {
        perror("getcwd error\n");
    }
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

void cd2(const char * new_path)
{
    if(new_path==NULL||strcmp(new_path,""))chdir("home");
    else if(strcmp(new_path,"-"))chdir("..");
    else if(chdir(new_path)==0);
    else {
        printf("cd: %s: No such file or directory\n",new_path);
    }
    mygetcwd();
}

void ls2(const char* dirname)
{
    DIR *dirp;
    struct dirent* entry;
    if((dirp=opendir(dirname))==NULL)
    {
        perror("ls: NO such file or directory\n");
        return;
    }
}
void touch(){}
void echo2(){}
void cat2(){}
void cp2(){}
void rm2(){}
void rename2(){}
void history2(){}
void quit(){}
void err(){}

int main(int argc,char** argv)
{
    char cmd[CMD_MAX];
    printf("####### Welcome to shelldemo #########\n");
    while(1){
    printf("[%s]>>",cwd);
    memset(cmd,'\0',CMD_MAX);
    fgets(cmd,sizeof(cmd),stdin);
    cmd_argv[0] = strtok(cmd," ");
    int index=1;
    while((cmd_argv[index++]=strtok(NULL," "))!=NULL);
    int ci=0;
    while(ci<cnum&&strcmp(allcmd[ci],cmd_argv[0])!=0)ci++;
    switch(ci){
        case 0:pwd2();break;
        case 1:cd2();break;
        case 2:break;
        case 3:break;
        case 4:break;
        case 5:break;
        case 6:break;
        case 7:break;
        case 8:break;
        case 9:break;
        case 10:break;
        case 11:break;
    }
    fflush(stdout);
    }|
    printf("####### Quiting shelldemo #########\n");
    return 0;
}