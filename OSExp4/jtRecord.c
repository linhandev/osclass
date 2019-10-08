#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "HashFile.h"
#include "jtRecord.h"
#define KEYOFFSET 0
#define KEYLEN sizeof(int)
#define FILENAME "jing.hash"

void showHashFile();

int main(int argc, char *argv[]) {
    struct jtRecord rec[6]=
    {{1,"jing"},{2,"wang"},{3,"li"},{4,"zhang"},{5,"qing"},{6,"yuan"}};
    int j=0;
    for(j=0; j<6; j++) {
        printf("<%d,%d>\t",rec[j].key,hash(KEYOFFSET,KEYLEN,&rec[j],6));
    }

    int fd=hashfile_creat(FILENAME,O_RDWR|O_CREAT,RECORDLEN,6);

    int i=0;
    printf("\n打开并保存记录...\n");
    fd=hashfile_open(FILENAME,O_RDWR,0);
    for(i=0; i<6; i++) {
        hashfile_saverec(fd,KEYOFFSET,KEYLEN,&rec[i]);
    }
    hashfile_close(fd);
    showHashFile();

    //Demo find Rec
    printf("\n查找记录...\n");
    fd=hashfile_open(FILENAME,O_RDWR,0);
    int offset=hashfile_findrec(fd,KEYOFFSET,KEYLEN,&rec[4]);
    printf("\n偏移是 %d\n",offset);
    hashfile_close(fd);
    struct jtRecord jt;
    struct CFTag tag;
    fd=open(FILENAME,O_RDWR);
    lseek(fd,offset,SEEK_SET);
    read(fd,&tag,sizeof(struct CFTag));
    printf("标签是 <%d,%d>\t",tag.collision,tag.free);
    read(fd,&jt,sizeof(struct jtRecord));
    printf("记录是 {%d,%s}\n",jt.key,jt.other);

    //Demo Delete Rec
    printf("\n删除记录...\n");
    fd=hashfile_open(FILENAME,O_RDWR,0);
    hashfile_delrec(fd,KEYOFFSET,KEYLEN,&rec[2]);
    hashfile_close(fd);
    showHashFile();

    //Demo Read
    fd=hashfile_open(FILENAME,O_RDWR,0);
    char buf[32];
    memcpy(buf,&rec[1],KEYLEN);
    hashfile_read(fd,KEYOFFSET,KEYLEN,buf);
    printf("\n读出的记录是{%d,%s}\n",((struct jtRecord*)buf)->key,((struct jtRecord*)buf)->other);
    hashfile_close(fd);

    //Demo Write
    printf("\n写入记录...\n");
    fd=hashfile_open(FILENAME,O_RDWR,0);
    hashfile_write(fd,KEYOFFSET,KEYLEN,&rec[3]);
    hashfile_close(fd);
    showHashFile();
    return 0;
}

void showHashFile() {
    int fd;
    printf("\n");
    fd=open(FILENAME,O_RDWR);
    lseek(fd,sizeof(struct HashFileHeader),SEEK_SET);
    struct jtRecord jt;
    struct CFTag tag;
    while(1) {
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
