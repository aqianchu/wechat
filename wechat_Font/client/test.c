#include <stdio.h>
#include <string.h>
/*
	���������ַ��������ó�Ĭ��default
*/
int main()
{
	Init_Lcd();
	Clean_Area(0,0,800,100); //Ĭ���ǰ�ɫ����
	Display_characterX(0,0,strlen("�����Ƕ"),"�����Ƕ",4); //ע�⺺�ֱ�������޸ĳ�GB2312
	UnInit_Lcd();
}