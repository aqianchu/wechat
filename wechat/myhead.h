//把系统中常用的头文件全部都包含，然后丢到系统头文件的环境变量中/usr/include，那么以后就可以将myhead.h当成系统提供的一个头文件
#ifndef _MYHEAD_H
#define _MYHEAD_H
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/input.h>//跟输入子系统模型有关的头文件
#include <sys/mman.h>
#include <sys/wait.h>//跟wait有关
#include <signal.h>//跟信号有关的头文件
#include <sys/shm.h>//跟共享内存有关的头文件
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/select.h>
#include <poll.h>
#include <termios.h>

#include <sys/select.h>
#include <stropts.h>

#include "jpeglib.h"
#include "jconfig.h"
#include "jerror.h"
#include <linux/videodev2.h>//跟摄像头有关的

#define MAXSIZE 6666
#define SOUNDSIZE 50

struct usrbuf
{
	void *addr;
	int somelength;
};

//显示800*480jpeg图片函数
void show_jpeg(char *argv);
//显示任意大小jpeg图片函数
int show_anyjpg(int x,int y,char *jpgpath);
int camera();


#endif
