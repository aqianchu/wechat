#include "myhead.h"

int send_file(char * path,int newfd)//���ļ�
{
	int bmpfd,ret;
	//���ļ�
	 bmpfd=open(path,O_RDWR);
	 if(bmpfd==-1)
	 {
		 perror("File opening failed!\n");
		 return -1;
	 }
	 
	 int size=lseek(bmpfd,0,SEEK_END);//���ļ��Ĵ�С
	 printf(" file size:%d\n",size);
	 //�����ļ�ʵ�ʴ�С��������
	 char buf[size];
	 lseek(bmpfd,0,SEEK_SET);//���ļ�ָ���Ƶ��ļ���ͷ
	 //���ļ���С���͸��ͻ���
	 ret=send(newfd,&size,sizeof(int),0);
	 bzero(buf,sizeof(buf));
	 read(bmpfd,buf,size);
	 //���ļ����͸��ͻ���
	 ret=send(newfd,buf,size,0);
	 printf("Size of the sending fil:%d\n",ret);
	
	return 0;
}

int receive_file(char * bmppath,int fd)//�����ļ�
{
	int size,ret,bmpfd,ret1;
	//�����ļ���С��Ϣ
	ret=recv(fd,&size,sizeof(int),0);
	if(ret==-1)
	{
		perror("Failed to receive file siz!\n");
		return -1;
	}
	//�����ļ�ʵ�ʴ�С��������
	 char buf[size];
	//�����ļ�
	bzero(buf,sizeof(buf));
	ret=recv(fd,buf,size,MSG_WAITALL);
	if(ret==-1)
	{
		perror("Failed to receive file!\n");
		return -1;
	}
	printf("Receive File size:%d\n",ret);
	//����һ���ļ�
	bmpfd=open(bmppath,O_CREAT|O_RDWR|O_TRUNC,0777);
	if(bmpfd==-1)
	{
		perror("File creation failed!\n");
		return -1;
	}
	ret1=write(bmpfd,buf,ret);
	if(ret1==-1)
	{
		perror("Write file failed!\n");
	}
	
	close(bmpfd);
	
	return 0;
}