#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define RECORDLEN 32   //一条记录整个的大小，包括key和内容
#define COLLISIONFACTOR 0.5  //Hash函数冲突率，已经定义过，要改的话在这改

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "HashFile.h"
#include "jtRecord.h"
#define KEYOFFSET 0   //key在struct里的offset,单位字节
#define KEYLEN sizeof(int)
#define FILENAME "jing.hash"

void showHashFile();

int main(int argc, char *argv[])
{
  int total_record_number=6;
  printf("输入hash总空间长度\n");
  scanf("%d",&total_record_number);


  int fd=hashfile_creat(FILENAME,O_RDWR|O_CREAT,RECORDLEN,total_record_number);
  if(fd!=-1)
    printf("已经成功创建文件\n");
  else
    return 0;

  char cmd[10];
  while(scanf("%s",cmd))
  {
    fd=hashfile_open(FILENAME,O_RDWR,0);
    struct jtRecord record;

    switch (cmd[0])
    {
      case 'a':
        printf("添加记录\n");
        printf("输入key和字符串内容\n");
        scanf("%d%s",&record.key,record.other);
        hashfile_saverec(fd,KEYOFFSET,KEYLEN,&record);
        hashfile_close(fd);
        break;

      case 's':
        if(cmd[1]=='e')
        {
          printf("\n查找记录...\n");
          printf("输入字符串内容\n");
          scanf("%s",record.other);
          int offset=hashfile_findrec(fd,KEYOFFSET,KEYLEN,&record);
          printf("偏移是 %d\n",offset);
          hashfile_close(fd);
          struct jtRecord jt;
          struct CFTag tag;
          fd=open(FILENAME,O_RDWR);
          lseek(fd,offset,SEEK_SET);
          read(fd,&tag,sizeof(struct CFTag));
          printf("标签是 <%d,%d>\t",tag.collision,tag.free);
          read(fd,&jt,sizeof(struct jtRecord));
          printf("记录是 {%d,%s}\n",jt.key,jt.other);
          close(fd);

        }
        else if(cmd[1]=='h')
        {
          showHashFile();
          hashfile_close(fd);
        }
        break;

      case 'd':
        printf("\n删除记录...\n");
        printf("输入字符串内容\n");
        scanf("%s",record.other);
        hashfile_delrec(fd,KEYOFFSET,KEYLEN,&record);
        hashfile_close(fd);
        break;

      case 'r':
        scanf("%s",record.other);
        char buf[RECORDLEN];
        memcpy(buf,&record,KEYLEN);
        hashfile_read(fd,KEYOFFSET,KEYLEN,buf);
        printf("\n读出的记录是{%d,%s}\n",((struct jtRecord*)buf)->key,((struct jtRecord*)buf)->other);
        hashfile_close(fd);
        break;
    }
  }
}

void showHashFile()
{
    int fd;
    printf("\n");
    fd=open(FILENAME,O_RDWR);
    lseek(fd,sizeof(struct HashFileHeader),SEEK_SET);
    struct jtRecord jt;
    struct CFTag tag;
    while(1)
    {
        if (read(fd,&tag,sizeof(struct CFTag))<=0)
            break;
        if(read(fd,&jt,sizeof(struct jtRecord))<=0)
            break;
        if (tag.free) {
            printf("标签是<%d,%d>\t",tag.collision,tag.free);
            printf("记录是{%d,%s}\n",jt.key,jt.other);
        }
    }
    close(fd);
}
