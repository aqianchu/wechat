#include <stdio.h>
#include <string.h>
/*
	���������ַ��������ó�Ĭ��default
*/
int main()
{
	//��ʼ��lcd
	Init_Lcd();
	//���������ñ�����ɫ
	//int  Clean_Area(int X,int Y,int width,int height)
	Clean_Area(0,0,800,100); //Ĭ���ǰ�ɫ����
	//��ʾ�ַ���
	//int  Display_characterX(unsigned int x,unsigned int y,unsigned int len,unsigned char *string,int size)
	Display_characterX(0,0,strlen("�����Ƕ"),"�����Ƕ",4); //ע�⺺�ֱ�������޸ĳ�GB2312
	UnInit_Lcd();
}