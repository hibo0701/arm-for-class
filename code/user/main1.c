//----------------------------------------------------------------------------
#include "config.h"
//----------------------------------------------------------------------------
//�궨��
#define uchar   unsigned char
#define uint   unsigned int
#define uint8   unsigned char
#define uint32   unsigned int

//�Ӻ�������
void Delayms(unsigned int n)    //��Լ1������ʱ
{
	unsigned int i,j;
	for(i=0;i<n;i++)
	   for(j=1;j<2500;j++); //5000
}

//�Ӻ�������
void Delayus(unsigned int n)    //��Լ1΢����ʱ
{
	unsigned int  i,j;
	for(i=0;i<8;i++)
	   for(j=1;j<n;j++);
}

//----------------------------------------------------------------------------
//                        ����Ϊ�û��Լ��Ĺ��ܳ���   
//----------------------------------------------------------------------------

#define	 LED	(1<<25)		// �͵�ƽ����
  #define	 LEDR	(1<<26)	

//====================================================
//�������ƣ�main()
//�������ܣ�����˸ ʵ��
//====================================================
int main(void)
{	
	PINSEL1	= 0x00000000;	//���ùܽ�����GPIO
	IO0DIR	= IO0DIR|LED;	//����LED���ƿ����	
	IO0DIR	= IO0DIR|LEDR;
	while (1)
	{
		IO0SET |= LED;  //P0.25 �øߵ�ƽ������
		Delayms(500);
		IO0CLR |= LED;  //P0.25 �õ͵�ƽ������
		Delayms(500);
		
		IO0CLR |= LEDR;  //P0.25 �õ͵�ƽ������
		Delayms(500);		
	}   
}

