#include "myhead.h"

int num;//�û�ͼ����

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

pnode seek(pnode head,char * ip)//���ҽڵ㺯��
{
	pnode p=head->next;
	while(p!=NULL)
	{
		if(strcmp(p->ip_address,ip)==0)
		{
			return p;
		}
		p=p->next;
	}
	
	return NULL;
}

void * routine(void * arg)
{
	int ret;
	int count=0;//�����Լ����ڵ��û�����
	char buffer[100];
	char buf[100];
	char buf1[100];
	char buf2[100];
	char * str,* str1,* str2;
	pnode p=(pnode)arg;
	pnode q;
	
	//�����������β��,�ҵ���������ӿͻ������ݵĽڵ�
	while(p->next!=NULL)
	{
		p=p->next;
		count++;
	}
	count--;
	
	//�����û�ͼƬ
	bzero(buf,sizeof(buf));
	sprintf(buf,"/home/gec/picture/%d.bmp",num);
	receive_file(buf,p->new_fd);//�Ƚ����ļ���С��Ȼ�����ļ���Сһ��������MSG_WAITWALL
	
	while(1)
	{
		bzero(buffer,100);
		ret=recv(p->new_fd,buffer,100,0);
		printf("Client message:%s\n",buffer);
		if(ret==0)//����ͻ��˶Ͽ�����
		{
			//�����е������û����ͶϿ����ӵ��û���IP
			q=((pnode)arg)->next;
			while(q!=NULL)
			{
				if(strcmp(q->ip_address,p->ip_address)!=0)
				{
					bzero(buf,sizeof(buf));
					sprintf(buf,"off line:%s",p->ip_address);
					send(q->new_fd,buf,sizeof(buf),0);
				}
				q=q->next;
			}
			//ɾ���Ͽ����ӵĿͻ��˵�����
			delete((pnode)arg,p->ip_address);
			
			pthread_exit(NULL);
		}
		//�ж��Ƿ��ȡ�û���Ϣ(�������û���ȡ��Ϣ)
		if(strncmp(buffer,"user",4)==0)
		{
			//���������汳��ͼ
			ret=send_file("/mnt/hgfs/share/back.bmp",p->new_fd);
			if(ret==-1)
			{
				exit(-1);
			}
			//�����������ͼƬ
			ret=send_file("/mnt/hgfs/share/timg.bmp",p->new_fd);
			if(ret==-1)
			{
				exit(-1);
			}
			//���ͷ��˷��ͳ����Լ����ڵ������û�����
			send(p->new_fd,&count,sizeof(count),0);
			//��������������ӵĿͻ��˷��������û���Ϣ
			q=((pnode)arg)->next;
			while(q!=NULL)
			{
				if(strcmp(q->ip_address,p->ip_address)!=0)//��������Լ��Ŀͻ�������
				{
					//�ѽڵ��пͻ��˵�IP��ַ�Ͷ˿ں��Լ�ͼ����ƴ������
					bzero(buf,sizeof(buf));
					sprintf(buf,"%s:%hu:%d:%d",q->ip_address,q->someport,q->new_fd,q->number);
					//��ƴ����������Ϣ���͸�Ŀ��ͻ���
					send(p->new_fd,buf,sizeof(buf),0);
					//���������û�ͼ��
					bzero(buf,sizeof(buf));
					sprintf(buf,"/home/gec/picture/%d.bmp",q->number);
					send_file(buf,p->new_fd);
				}
				q=q->next;
			}
			//�ٸ������ߵ��û����������ߵ��û���Ϣ
			q=((pnode)arg)->next;
			while(q!=NULL)
			{
				if(strcmp(q->ip_address,p->ip_address)!=0)//��������Լ��Ŀͻ�������
				{
					bzero(buf,sizeof(buf));
					sprintf(buf,"go online:%s:%hu:%d:%d",p->ip_address,p->someport,p->new_fd,p->number);
					send(q->new_fd,buf,sizeof(buf),0);
					
					//�����������û�ͼ��
					bzero(buf,sizeof(buf));
					sprintf(buf,"/home/gec/picture/%d.bmp",p->number);
					send_file(buf,q->new_fd);
				}
				q=q->next;
			}
		}
		//�ж��Ƿ����ļ�
		else if(strncmp(buffer,"@file",5)==0)
		{
			strtok(buffer,":");
			str=strtok(NULL,":");//Ŀ���û�ip
			//�����ļ�·����
			bzero(buf,sizeof(buf));
			recv(p->new_fd,buf,100,0);
			//ѭ���ָ�
			str1=strtok(buf,"/");
			while((str2=strtok(NULL,"/"))!=NULL)
			{
				str1=str2;
			}
			bzero(buf1,sizeof(buf1));
			sprintf(buf1,"/home/gec/file/%s",str1);
			//�����ļ�
			receive_file(buf1,p->new_fd);
			//ת����Ŀ��ͻ���
			q=seek((pnode)arg,str);
			//ƴ�ӽ����ļ�ָ�����ļ���
			bzero(buf2,sizeof(buf2));
			sprintf(buf2,"@file:%s",str1);
			send(q->new_fd,buf2,sizeof(buf2),0);
			//�����ļ�
			send_file(buf1,q->new_fd);
			
		}
		//�ж��Ƿ�������
		else if(strncmp(buffer,"emjo",4)==0)
		{
			str=strtok(buffer,":");
			str1=strtok(NULL,":");
			
			if(strcmp(str,"emjo1")==0)//����1
			{
				q=seek((pnode)arg,str1);
				bzero(buf1,sizeof(buf1));
				sprintf(buf1,"emjo1:%s",str1);
				send(q->new_fd,buf1,sizeof(buf1),0);//���ͽ��ձ���ָ��
				send_file("/mnt/hgfs/share/emjo1.bmp",q->new_fd);
			}
			else if(strcmp(str,"emjo2")==0)//����2
			{
				q=seek((pnode)arg,str1);
				bzero(buf1,sizeof(buf1));
				sprintf(buf1,"emjo2:%s",str1);
				send(q->new_fd,buf1,sizeof(buf1),0);//���ͽ��ձ���ָ��
				send_file("/mnt/hgfs/share/emjo2.bmp",q->new_fd);
			}
			else//����3
			{
				q=seek((pnode)arg,str1);
				bzero(buf1,sizeof(buf1));
				sprintf(buf1,"emjo3:%s",str1);
				send(q->new_fd,buf1,sizeof(buf1),0);//���ͽ��ձ���ָ��
				send_file("/mnt/hgfs/share/gif1.bmp",q->new_fd);
				send_file("/mnt/hgfs/share/gif2.bmp",q->new_fd);
			}
		}
		//����������Ϣ
		else
		{
			str=strtok(buffer,":");
			q=seek((pnode)arg,str);
			str1=strtok(NULL,":");
			bzero(buf1,sizeof(buf1));
			sprintf(buf1,"%s:%s",p->ip_address,str1);
			send(q->new_fd,buf1,sizeof(buf1),0);
		}
	}
}

int main()
{
	int fd,ret;
	int newfd;//�����µ��׽����ļ�������
	pthread_t id;
	int value=1;
	
	struct sockaddr_in ipv4_address;//����һ��ipv4�ṹ�����
	bzero(&ipv4_address,sizeof(ipv4_address));
	ipv4_address.sin_family=AF_INET;//��ŵ�ַЭ��
	ipv4_address.sin_port=htons(6666);//�˿ں�
	ipv4_address.sin_addr.s_addr=inet_addr("192.168.11.135");//IP��ַ��ע��Ҫת���ֽ���
	
	struct sockaddr_in client_address;//����һ���ͻ��˽ṹ�����
	int address_size=sizeof(client_address);
	
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
	
	//����
	 ret=listen(fd,3);
	 if(ret==-1)
	 {
		perror("����ʧ�ܣ�\n");
		return -1;
	 }
	 
	 pnode mylist=init_list();//��ʼ������
	 
	 //�����������󲢴���ͻ�������
	 while(1)
	 {
		 newfd=accept(fd,(struct sockaddr *)&client_address,&address_size);
		 if(newfd==-1)
		 {
			perror("������������ʧ�ܣ�\n");
			return -1;
		 }
		 printf("IP:%s port:%d Connection success!\n",inet_ntoa(client_address.sin_addr),\
		 ntohs(client_address.sin_port),newfd);
		 //�û�ͼ���ż�1
		 num++;
		 //������ͻ��˵���Ϣ
		 add(mylist,inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port),newfd,num);
		 //�����߳�
		 pthread_create(&id,NULL,routine,mylist);
		 //�����߳�Ϊ��������
		 pthread_detach(id);
	 }
	 
	 
	close(fd);
	
	return 0;
}