#include "sys/types.h"
#include "sys/file.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/wait.h"

char r_buf[4];
char w_buf[4];
int pipe_fd[2];   //读写管道，0读1写
pid_t pid1,pid2,pid3,pid4;
int producer(int id);
int consumer(int id);

int main()
{
	if(pipe(pipe_fd)<0)
	{
			printf("pipe create error \n");   //创建管道
			exit(-1);
	}
	else
	{
		printf("pipe is created successfully! \n");   //创建进程
		if((pid1=fork())==0)
			producer(1);
		if((pid2=fork())==0)
			producer(2);
		if((pid4=fork())==0)
			consumer(2);

	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);


	int i,pid,status;
	for(i=0;i<4;i++)
	{
		pid=wait(&status);
	}
	exit(0);
}

int producer(int id)
{
	printf("producer %d is running! \n",id);
	close(pipe_fd[0]);
	int i=0;
	for(i=1;i<10;i++)
	{
		sleep(3);
		if(id==1)
			strcpy(w_buf,"aaa\0");
		else
			strcpy(w_buf,"bbb\0");
		if(write(pipe_fd[1],w_buf,4)!=-1)
			printf("after write %d\n",id);
	}
	close(pipe_fd[1]);
	printf("producer %d is over! \n",id);
	exit(id);
}

int consumer(int id)
{
  close(pipe_fd[1]);   //关闭写开始读
  printf("consumer %d is running! \n",id);
  if(id==1)
    strcpy(w_buf,"ccc\0");
  else
    strcpy(w_buf,"ddd\0");
  while(1)
  {
    sleep(4);
    strcpy(r_buf,"eee\0");
    if(read(pipe_fd[0],r_buf,4)==-1)
      break;
    printf("consummer %d get %s,while the w_buf is %s\n",id,r_buf,w_buf);
  }
  close(pipe_fd[0]);
  printf("consumer %d is over! \n",id);
  exit(id);
}
