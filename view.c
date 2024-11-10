#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#define PATH_MAX 255
#define CMD_MAX 30
#define SIZE 1024
const int cnum=11; //number of cmd
char cwd[PATH_MAX];
char cmd_argv[CMD_MAX][CMD_MAX];
char allcmd[11][CMD_MAX]={"pwd2","cd2","ls2","touch2","echo2","cat2","cp2","rm2","rename2","history2","quit"};
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
    //memset(cwd,0,sizeof(char)*PATH_MAX);
    if(getcwd(cwd,sizeof(cwd))!=NULL)
    {
        printf("current directory:[%s]\n",cwd);
    }else{
        perror("getcwd error\n");
    }
}

void cd2(const char* new_path)
{
    
    if(new_path==NULL||strcmp(new_path,"")==0){
        chdir("/home");
        printf("/home\n");
    }
    else if(strcmp(new_path,"-")==0)
    {
        chdir("..");
        printf("..\n");
    }
    else if(chdir(new_path)==0)printf("%s\n",new_path);
    else {
        printf("cd: %s: No such file or directory\n",new_path);
    }
    mygetcwd();
}

void ls2(const char* dirname)
{
    DIR *dirp;
    struct dirent* entry;
    printf("%s\n",dirname);
    if((dirp=opendir(dirname))==NULL)
    {
        perror("ls2: NO such file or directory\n");
        return;
    }else{
        printf("Dir:");
        while((entry=readdir(dirp))!=NULL)
        {
            if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0)continue;
            if(entry->d_type==DT_DIR)printf("%s   ",entry->d_name);
        }
        rewinddir(dirp);
        printf("\nFile:");
        while((entry=readdir(dirp))!=NULL)
        {
            if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0)continue;
            if(entry->d_type==DT_REG)printf("%s   ",entry->d_name);
        }
        printf("\n");
        closedir(dirp);
    }
}
void touch2(int fnum,const char fname[CMD_MAX][CMD_MAX])
{
    for(int i=1;i<fnum;++i)
    {  
        int fd=open(fname[i],O_CREAT,0777);
        if(fd==-1){
            fprintf(stderr,"file %s created fail\n",fname[i]);
            continue;
        }
        close(fd);
    }
    return;
}
void echo2(int strnum,char str[CMD_MAX][CMD_MAX])
{
    for(int i=1;i<strnum;++i)
    {
        fputs(str[i],stdout);
        if(i!=strnum-1)putchar(' ');
    }
    putchar('\n');
}
void cat2(const char* fname)
{
    FILE* fp;
    if((fp=fopen(fname,"r"))==NULL)
    {
        perror("can't open file\n");
        return;
    }else{
        char buf[SIZE];
        int linenum=1;
        while(fgets(buf,sizeof(buf),fp)!=NULL)
        {
            printf("%d %s",linenum++,buf);
        }
    }


}
void cp2(const char* src,const char* dest)
{
    int fd1,fd2;
    if(src==NULL||dest==NULL){
        perror("no enough argument\n");
    }else if((fd1=open(src,O_RDONLY))==-1){
        perror("source file can't open\n");
        return;
    }else{
        fd2=open(dest,O_CREAT|O_TRUNC|O_WRONLY,0664);
        ssize_t readbytes=0;
        char buf[SIZE];
        while((readbytes=read(fd1,buf,sizeof(buf)))>0)
        {
            write(fd2,buf,readbytes);
        }
        close(fd1);
        close(fd2);
    }
}
void rm2()
{

}
void rename2(const char* oldname,const char* newname)
{

    //struct stat buf;
    //if(oldname==NULL||oldname==NULL)
    //{
    //    perror("no oldname or no newname\n");
    //}else if(stat(oldname,&buf)==-1){
     //   perror("no such file or directory\n");
    ///}else{
        if(rename(oldname,newname)==-1){
            printf("Error: %s\n",strerror(errno));
        }else{
            printf("rename success\n");
        }
//  }

}
void history2(){}
void quit(){}
void err(){}

int main(int argc,char** argv)
{
    char cmd[CMD_MAX];
    printf("####### Welcome to shelldemo #########\n");
    while(1){
    fflush(stdout);
    mygetcwd();
    printf("[%s]>>",cwd);
    //char ch;
    //rsize_t strmax = sizeof cmd;
    memset(cmd,'\0',CMD_MAX);
    //while((ch=getchar()) != '\n'&& ch != EOF);
    fgets(cmd,sizeof(cmd),stdin);
    cmd[strlen(cmd)-1]='\0';
    char* ptr=cmd,*tmp=cmd;
    //cmd_argv[0] = strtok_r(cmd," ",&ptr);
    int index=0;
    //cmd_argv[index]=cmd;
    while(index<CMD_MAX&&(tmp=strtok_r(tmp," ",&ptr))!=NULL){strcpy(cmd_argv[index++],tmp);tmp=NULL;}
    int ci=0;
    while(ci<cnum&&strcmp(allcmd[ci],cmd_argv[0])!=0)ci++;
    for(int i=0;i<index;++i)printf("%s ",cmd_argv[i]);
    printf("%d\n",index);
    switch(ci){
        case 0:pwd2();break;
        case 1:cd2(index>1?cmd_argv[1]:NULL);break;
        case 2:ls2(index>1?cmd_argv[1]:".");break;
        case 3:touch2(index,cmd_argv);break;
        case 4:echo2(index,cmd_argv);break;
        case 5:cat2(cmd_argv[1]);break;
        case 6:cp2(index>1?cmd_argv[1]:NULL,index>2?cmd_argv[2]:NULL);break;
        case 7:break;
        case 8:rename(index>1?cmd_argv[1]:NULL,index>2?cmd_argv[2]:NULL);break;
        case 9:break;
        case 10:break;
        case 11:break;
    }
    
    }
    printf("####### Quiting shelldemo #########\n");
    return 0;
}