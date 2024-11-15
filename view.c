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
void rename2(const char* oldname,const char* newname)
{

    //struct stat buf;
    //if(oldname==NULL||oldname==NULL)
    //{
    //    perror("no oldname or no newname\n");
    //}else if(stat(oldname,&buf)==-1){
     //   perror("no such file or directory\n");
    ///}else{
        int num=rename(oldname,newname);
        if(num==0){
            printf("rename success\n");
        }else{
            printf("Error: %s\n",strerror(errno));
        }
//  }

}
void history2(){}
void quit(){}
void err(){}

int rm_dir(const char* full_path)
{
    DIR* dirp=opendir(full_path);
    if(!dirp)
    {
        perror("this directory can't open");
        return -1;
    }
    struct dirent* dir;
    struct stat st;
    while((dir=readdir(dirp))!=NULL)
    {
	    if(strcmp(dir->d_name,".")==0||strcmp(dir->d_name,"..")==0)continue;
	    char* sub_path=(char*)malloc(sizeof(char)*(strlen(full_path)+strlen(dir->d_name)+2));
	strcpy(sub_path,full_path);
	strcat(sub_path,"/");
	strcat(sub_path,dir->d_name);
        if(lstat(sub_path,&st)==-1)
        {
            perror("rm_dir lstat error\n");
            continue;
        }
        //POSIX宏来使用st_mode字段检查文件类型,S_ISREG(mode_t st_mode)
        if(S_ISDIR(st.st_mode))
        {
            if(rm_dir(sub_path)==-1){
            
                closedir(dirp);
                return -1;
            }
        }else if(S_ISREG(st.st_mode))
        {
            unlink(sub_path);
        }else{
            perror("other types of file error\n");
            continue;
        }
	free(sub_path);
}
    if(rmdir(full_path)==-1)
    {
        perror("can't rmdir itself\n");
	closedir(dirp);
	return -1;
    }
    closedir(dirp);
    return 0;
}

int rm(const char* fname)
{
	char* fpath=(char*)malloc(sizeof(char)*(strlen(fname)+1));
	strcpy(fpath,fname);
	struct stat st;
	if(lstat(fpath,&st)==-1)
	{
		perror("no this file");
		return -1;
	}
	if(S_ISREG(st.st_mode))
	{
		if(unlink(fpath)==-1)
		{
			return 0;
		}
	}else if(S_ISDIR(st.st_mode))
	{
		if(strcmp(fpath,",")==0||strcmp(fpath,"..")==0)
		{
			return -1;
		}
		if(rm_dir(fpath)==-1)
		{
			return -1;
		}
	}
	free(fpath);
	return 0;
}
int rm2(int index,char cmd_argv[CMD_MAX][CMD_MAX])
{
	if(index==1)
	{
		perror("no name of file");
	}
	else if(index==2)
	{
		rm(cmd_argv[1]);
	}
	else if(index==3||strcmp(cmd_argv[1],"-r")==0)
	{
		rm(cmd_argv[2]);
	}
	return 0;
}
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
        case 7:rm2(index,cmd_argv);break;
        case 8:rename(index>1?cmd_argv[1]:NULL,index>2?cmd_argv[2]:NULL);break;
        case 9:break;
        case 10:break;
        case 11:break;
    }
    
    }
    printf("####### Quiting shelldemo #########\n");
    return 0;
}
