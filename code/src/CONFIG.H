
#ifndef __CONFIG_H 
#define __CONFIG_H

//This segment should not be modified
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

/********************************/
/*      ARM���������           */
/********************************/
//��һ������Ķ�
#include    "LPC213x.h"
#include  <stdio.h>

/********************************/
/*     Ӧ�ó�������             */
/********************************/
//���¸�����Ҫ�Ķ�

/********************************/
/*     �����ӵ�����             */
/********************************/
/* ϵͳ����, Fosc��Fcclk��Fcco��Fpclk���붨��*/
#define Fosc            12000000
						    //����Ƶ��,Ӧ����ʵ��һ��
#define Fcclk           (Fosc * 5)                  
						    //ϵͳƵ�ʣ�����ΪFosc��������(1~32)����<=60MHZ
#define Fcco            (Fcclk * 4)                
						    //CCOƵ�ʣ�����ΪFcclk��2��4��8��16������ΧΪ156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             
						    //VPBʱ��Ƶ�ʣ�ֻ��Ϊ(Fcclk / 4)��1��2��4��

#include    "target.h"              //��һ�䲻��ɾ��

#endif