/**
 * @file   testebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/ebbchar.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main()
{
	int testdev;
    int i ;
    char buf[50]= "pear to dev!";
    printf("program test is running!\n");
    testdev = open("/dev/mydev",O_RDWR);
    if(testdev==-1)
    {
       printf("can't open file \n");
       exit(0);
    }
    //向设备写入"pear to dev!"
    write(testdev,buf,50);
    printf("write \"%s\"\n",buf,50);
    //更改buf内容为"apple to dev!"
    strcpy(buf,"apple to dev!");
    printf("buffer is changed to \"%s\"\n",buf,50);
   //由设备读出内容, 比较与buf不同
    read(testdev,buf,50);
    printf("read from dev is \"%s\"\n",buf);
    //释放设备
    close(testdev);
}
