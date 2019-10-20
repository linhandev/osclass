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
    testdev = open("/dev/ebbchar",O_RDWR);
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
// int main()
// {
//    int ret, fd;
//    char stringToSend[BUFFER_LENGTH];
//    printf("Starting device test code example...\n");
//    fd = open("/dev/ebbchar", O_RDWR);             // Open the device with read/write access
//    if (fd < 0)
//    {
//       perror("Failed to open the device...");
//       return errno;
//    }
//
//    printf("Type in a short string to send to the kernel module:\n");
//    scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)
//    printf("Writing message to the device [%s].\n", stringToSend);
//    ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
//
//    if (ret < 0)
//    {
//       perror("Failed to write the message to the device.");
//       return errno;
//    }
//
//    printf("Press ENTER to read back from the device...\n");
//    getchar();
//
//    printf("Reading from the device...\n");
//    ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
//    if (ret < 0)
//    {
//       perror("Failed to read the message from the device.");
//       return errno;
//    }
//    printf("The received message is: [%s]\n", receive);
//    printf("End of the program\n");
//    return 0;
// }
