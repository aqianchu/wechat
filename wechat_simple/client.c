#include "myhead.h"

int fd;
int flag=1;//��־�û��Ƿ�������������
int flag2=2;//��־ѡ����λ

 typedef struct node
{
	char ip_address[20];//�洢�ͻ���ip
	unsigned short someport;//��˿ں�
	int new_fd;//�׽����ļ�������
	int number;//�û�ͼ����
	
	struct node * next;
}node,*pnode;

pnode init_list()//��ʼ������
{
	pnode pHead=malloc(sizeof(node));//ͷ���
	if(pHead==NULL)
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(-1);
	}
	pHead->next=NULL;
	
	return pHead;
}


void add(pnode phead,char * ip,unsigned short someport,int fd,int number)//��ӽ��
{
	pnode p=phead;
	pnode newnode=malloc(sizeof(node));
	if(newnode==NULL)
	{
		printf("�ڴ����ʧ�ܣ�\n");
		exit(-1);
	}
	strcpy(newnode->ip_address,ip);
	newnode->someport=someport;
	newnode->new_fd=fd;
	newnode->number=number;
	newnode->next=NULL;
	while(p->next!=NULL)
	{
		p=p->next;
	}
	p->next=newnode;
}

void delete(pnode head,char * ip)//ɾ���ڵ�
{
	while(head->next!=NULL)
	{
		pnode q=head;
		head=head->next;
		if(strcmp(head->ip_address,ip)==0)
		{
			q->next=head->next;
			free(head);
			break;
		}
	}
}

//��ʾ���������û�ͼƬ
void show_user_picture(pnode head)
{
	int x=0,y=0;
	char buf[50];
	
	//����ʾ�����汳��ͼ
	show_fullbmp("/IOT/mypicture/back.bmp");
	
	pnode p=head->next;
	while(p!=NULL)
	{
		bzero(buf,sizeof(buf));
		sprintf(buf,"/IOT/mypicture/%d.bmp",p->number);
		show_shapebmp(x,y,100,100,buf);
		x+=100;
		if(x==800)
		{
			y+=100;
			x=0;
		}
		
		p=p->next;
	}
}

void * routine(void * arg)
{
	
	
	int ret,temp,i;
	int count=0;
	char buffer[100];
	char buf[100];
	char * str1,* str2,* str3,* str4;
	pnode head=(pnode)arg;
	
	
	//ѭ�����շ�������Ϣ
	while(1)
	{
		bzero(buffer,100);
		ret=recv(fd,buffer,100,0);
		if(ret==0)//����������Ͽ�����
		{
			exit(-1);
		}
		//�жϷ������Ƿ�Ϊ�������û���IP
		if(strncmp(buffer,"off line",8)==0)
		{
			strtok(buffer,":");
			str1=strtok(NULL,":");
			//ɾ�����û�����
			delete(head,str1);
			if(flag)
				//ˢ���û�ͼ��
				show_user_picture(head);
		}
		//�жϷ������Ƿ�Ϊ�������û�����Ϣ
		else if(strncmp(buffer,"go online",9)==0)
		{
			//����Ϣ���зָ�
			strtok(buffer,":");
			str1=strtok(NULL,":");
			str2=strtok(NULL,":");
			str3=strtok(NULL,":");
			str4=strtok(NULL,":");
			printf("�������û���ͼ����Ϊ:%d\n",atoi(str4));
			//����Ϣ�������
			add(head,str1,atoi(str2),atoi(str3),atoi(str4));
			//�����������û�ͼ��
			bzero(buf,sizeof(buf));
			sprintf(buf,"/IOT/mypicture/%d.bmp",atoi(str4));//IOT/mypicture/%d.bmp
			receive_file(buf,fd);
			if(flag)
				//ˢ���û�ͼ��
				show_user_picture(head);
		}
		//�ж��Ƿ����ļ�
		else if(strncmp(buffer,"@file",5)==0)
		{
			//�ָ�õ��ļ���
			strtok(buffer,":");
			str1=strtok(NULL,":");
			bzero(buf,sizeof(buf));
			sprintf(buf,"/IOT/%s",str1);
			//�����ļ�
			receive_file(buf,fd);
			printf("�����ļ��ɹ�!\n");
		}
		//��������
		else if(strncmp(buffer,"emjo",4)==0)
		{
			str1=strtok(buffer,":");
			str2=strtok(NULL,":");
			
			if(strcmp(str1,"emjo1")==0)//�������Ǳ���1
			{
				receive_file("/IOT/mypicture/emjo1.bmp",fd);
				//��ʾ����,ֻ�����������ʾ
				if(flag==0)
				{
					//ˢ����Ļ
					show_fullbmp("/IOT/mypicture/timg.bmp");
					show_shapebmp(350,100,100,100,"/IOT/mypicture/emjo1.bmp");
				}
			}
			if(strcmp(str1,"emjo2")==0)//�������Ǳ���2
			{
				receive_file("/IOT/mypicture/emjo2.bmp",fd);
				//��ʾ����
				if(flag==0)
				{
					//ˢ����Ļ
					show_fullbmp("/IOT/mypicture/timg.bmp");
					show_shapebmp(350,100,100,100,"/IOT/mypicture/emjo2.bmp");
				}
			}
			if(strcmp(str1,"emjo3")==0)//�������Ǳ���3
			{
				receive_file("/IOT/mypicture/gif1.bmp",fd);
				receive_file("/IOT/mypicture/gif2.bmp",fd);
				//��ʾgifͼ��
				if(flag==0)
				{
					//ˢ����Ļ
					show_fullbmp("/IOT/mypicture/timg.bmp");
					for(i=0;i<50;i++)
					{
						show_shapebmp(350,100,100,100,"/IOT/mypicture/gif1.bmp");
						usleep(20000);
						show_shapebmp(350,100,100,100,"/IOT/mypicture/gif2.bmp");
					}
				}
			}
			
		}
		//����������Ϣ
		else
		{
			printf("%s\n",buffer);
			
			str1=strtok(buffer,":");
			str2=strtok(NULL,":");
			
			//���������ҵ�����Ϣ�û�ͼ���λ��
			pnode p=head->next;
			count=0;
			while(p!=NULL)
			{
				count++;
				if(p->someport==atoi(str1))//�ĳ�ip
					break;
				p=p->next;
			}
			//���������ʱ
			if(flag==0)
			{
				bzero(buf,sizeof(buf));
				sprintf(buf,"��%d��ͼ���û�������������Ϣ:",count);
				//ˢ��һ����Ļ�������ʷ����
				show_fullbmp("/IOT/mypicture/timg.bmp");
				//��ʼ��lcd
				Init_Lcd();
				//��ʾ��ʾ��Ϣ
				Display_characterX(160,34,30,buf,2);
				//��ʾ������Ϣ
				Display_characterX(160,115,50,str2,2);
				UnInit_Lcd();
			}
			//��������ʱ
			else
			{
				bzero(buf,sizeof(buf));
				sprintf(buf,"��%d��ͼ���û�������������Ϣ:",count);
				//ˢ��һ����Ļ�������ʷ����
				show_user_picture(head);
				//��ʼ��lcd
				Init_Lcd();
				Display_characterX(160,323,30,buf,2);
				Display_characterX(160,370,50,str2,2);
				UnInit_Lcd();
			}
		}
	}
}

void function(pnode p)//ѡ���ܺ���
{
	int ret;
	char buf[100];
	char buf1[100];
	
	if(flag2==1)//����
	{
		printf("��������IP:%s\n",p->ip_address);
		printf("������������Ϣ:\n");
		bzero(buf,sizeof(buf));
		scanf("%s",buf);
		sprintf(buf1,"%s:%s",p->ip_address,buf);
		send(fd,buf1,sizeof(buf1),0);
	}
	if(flag2==2)//���ļ�
	{
		//ƴ�ӷ����ļ�ָ���Լ�Ŀ��ip
		bzero(buf,sizeof(buf));
		sprintf(buf,"@file:%s",p->ip_address);
		send(fd,buf,sizeof(buf),0);

		printf("��������Ҫ�����ļ��ľ���·����:\n");
		while(1)
		{
			bzero(buf1,sizeof(buf1));
			scanf("%s",buf1);
			//�ж��ļ��Ƿ����
			ret=open(buf1,O_RDONLY);
			if(ret==-1)
			{
				 perror("�ļ�·��������,���������룡\n");
			}
			else
				break;
		}
		//�����ļ�����·������������
		send(fd,buf1,sizeof(buf1),0);
		//�����ļ���������
		send_file(buf1,fd);//�ڿ������ϸĳ�ip
		printf("�����ļ��ɹ�!\n");
	}
	if(flag2==3)//���ͱ���1
	{
		bzero(buf,sizeof(buf));
		sprintf(buf,"emjo1:%s",p->ip_address);
		send(fd,buf,sizeof(buf),0);
	}
	if(flag2==4)//���ͱ���2
	{
		bzero(buf,sizeof(buf));
		sprintf(buf,"emjo2:%s",p->ip_address);
		send(fd,buf,sizeof(buf),0);
	}
	if(flag2==5)//���ͱ���3
	{
		bzero(buf,sizeof(buf));
		sprintf(buf,"emjo3:%s",p->ip_address);
		send(fd,buf,sizeof(buf),0);
	}
}

void *rout(void * arg)
{
	int x,y,a,b;
	int flag1=0;
	pnode p;
	
	while(1)
	{
		a=0;
		b=0;
		p=((pnode)arg)->next;
		
		get_xy(&x,&y);//��ȡ����
		flag=0;//���־λΪ0����ʱֹͣˢ���û�ͼ������Ļ����ʾ������Ϣ
		printf("x=%d,y=%d\n",x,y);
		//�ж��û����˭��ͷ��
		while(p!=NULL)
		{
			if((x>=a&&x<=a+100)&&(y>=b&&y<=b+100))
			{
				//��ʾ�������
				show_fullbmp("/IOT/mypicture/timg.bmp");
				while(1)
				{
					get_xy(&x,&y);//��ȡ����,�ж��û�ѡ�����ĸ�����
					if((x>=0&&x<=105)&&(y>=0&&y<=100))//����
					{
						//ˢ���û�ͼ��
						show_user_picture((pnode)arg);
						flag1=1;
						flag=1;
						break;
					}
					else if((x>=160&&x<=240)&&(y>=300&&y<=365))//����
					{
						//����ѡ���ܺ���
						flag2=1;
						function(p);
					}
					else if((x>=590&&x<=650)&&(y>=304&&y<=370))//���ļ�
					{
						flag2=2;
						function(p);
					}
					else if((x>=160&&x<=240)&&(y>=400&&y<=480))//������1
					{
						flag2=3;
						function(p);
					}
					else if((x>=358&&x<=440)&&(y>=404&&y<=480))//������2
					{
						flag2=4;
						function(p);
					}
					else if((x>=594&&x<=666)&&(y>=405&&y<=480))//������3
					{
						flag2=5;
						function(p);
					}
				}
				if(flag1)
				{
					flag1=0;
					break;
				}
			}
			p=p->next;
			
			
			a+=100;
			if(a==800)
			{
				a=0;
				b+=100;
			}
		}
	}
}

//�ӷ�������ȡ��Ϣ
void user_info(pnode head)
{
	int i;
	int num=1,count=0;
	int ret;
	char buf[100];
	char buf1[100];
	char * str1,* str2,* str3,* str4;
	
	bzero(buf,sizeof(buf));
	strcpy(buf,"user");
	send(fd,buf,strlen(buf),0);//������������"user"��Ϣ����������������û���Ϣ����
	//���������汳��ͼ
	ret=receive_file("/IOT/mypicture/back.bmp",fd);
	if(ret==-1)
	{
		exit(-1);
	}
	//�����������ͼƬ
	ret=receive_file("/IOT/mypicture/timg.bmp",fd);
	if(ret==-1)
	{
		exit(-1);
	}
	//���շ����������������û�����
	ret=recv(fd,&count,sizeof(count),0);
	if(ret==-1)
	{
		perror("���������û�����ʧ��!\n");
		exit(-1);
	}
	
	for(i=1;i<=count;i++)
	{
		//���������û���Ϣ
		bzero(buf,sizeof(buf));
		ret=recv(fd,buf,sizeof(buf),0);
		if(ret==-1)
		{
			perror("�����û���Ϣʧ��!\n");
			exit(-1);
		}
		//����Ϣ���зָ�
		str1=strtok(buf,":");
		str2=strtok(NULL,":");
		str3=strtok(NULL,":");
		str4=strtok(NULL,":");
		//�������
		add(head,str1,atoi(str2),atoi(str3),atoi(str4));
		//���������û�ͼ��
		bzero(buf1,sizeof(buf1));
		sprintf(buf1,"/IOT/mypicture/%d.bmp",atoi(str4));
		receive_file(buf1,fd);
	}
}

int main( int argc,char *argv[])
{
	int ret;
	int value=1;
	char buffer[100];
	char buf[100];
	pthread_t id;
	pthread_t id1;
	
	struct sockaddr_in ipv4_address;//����һ��ipv4�ṹ�����
	bzero(&ipv4_address,sizeof(ipv4_address));
	ipv4_address.sin_family=AF_INET;//��ŵ�ַЭ��
	ipv4_address.sin_port=htons(atoi(argv[1]));//�˿ں�
	ipv4_address.sin_addr.s_addr=inet_addr("INADDR_ANY");//IP��ַ��ע��Ҫת���ֽ���,��INADDR_ANY(��ʾʹ�ñ�������������һ��ip��ַ)
	
	struct sockaddr_in server_address;//����һ���������ṹ�����
	bzero(&server_address,sizeof(server_address));
	server_address.sin_family=AF_INET;//��ŵ�ַЭ��
	server_address.sin_port=htons(6666);//�������Ķ˿ں�
	//��������IP��ַ��ע��Ҫת���ֽ���
	server_address.sin_addr.s_addr=inet_addr("192.168.11.135");
	
	
	//�����׽���
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1)
	{
		perror("�����׽���ʧ�ܣ�\n");
		return -1;
	}
	setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));//�ظ����ö˿�
	
	//��IP�Ͷ˿ں�
	ret=bind(fd,(struct sockaddr *)&ipv4_address,sizeof(struct sockaddr_in));
	if(ret==-1)
	{
		perror("��ʧ�ܣ�\n");
		return -1;
	}
	printf("My port number is:%d\n",ntohs(ipv4_address.sin_port));
	//���ӷ�����
	ret=connect(fd,(struct sockaddr *)&server_address,sizeof(server_address));
	if(ret==-1)
	{
		perror("���ӷ�����ʧ��!\n");
		return -1;
	}
   	//�ڿ������ϴ���һ��Ŀ¼
	ret=mkdir("/IOT/mypicture",0777);
	if(ret==-1)
	{
		if(errno!=17)
		{
			perror("����Ŀ¼ʧ��!\n");
			return -1;
		}
	} 
	//�������������û�ͼ��
	bzero(buffer,sizeof(buffer));
	printf("���������ϴ���ͼ��ľ���·����,ͼ����Ϊ:100*100\n");
	while(1)
	{
		scanf("%s",buffer);
		ret=send_file(buffer,fd);//����ͼ��
		if(ret==0)
			break;
		printf("ͼƬ������,������������Ҫ�ϴ���ͼ��ľ���·������\n");
	}
	
	//��ȡ�û���Ϣ
	 pnode mylist=init_list();//��ʼ������
	 user_info(mylist);
	 
	//��ʾ���������û�ͼƬ
	show_user_picture(mylist);
	
	//�����߳�,����������Ϣ
	pthread_create(&id,NULL,routine,mylist);
	pthread_detach(id);//�����߳�Ϊ��������
	
 	//����һ���߳����������Ļ
	pthread_create(&id1,NULL,rout,mylist);
	pthread_detach(id1);//�����߳�Ϊ�������� 
	
	while(1);
	
	close(fd);
	
	return 0;
}