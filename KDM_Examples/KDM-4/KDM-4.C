/* ch10-3-1.c - 1���X�ʹ��� */
//�Q��delay5mDELAY�Ƶ{��(5ms times), �����X�ʫH��
//��P1��X	�t�׬� 1/(5ms timers) �B/��
#include  <reg51.h> 		// �]�treg51.h�� 
#define  OUTPUT 	P1	// �w�q��X��P1
unsigned int times=50;		// �ŧi����ɶ��ܼ�( 5ms) 
unsigned char excite;		// �ŧi�E���ܼ� 
void step_rst(void);		// �ŧi�w���� 
void delay5ms(int);		// �ŧi������ 
//=====�D�{��======================================
main()				// �D�{���}�l 
{	OUTPUT=0;	// ������X 
	step_rst();	// �w�� 
	while (1)	//	while�j�� 
		step_rst();			// �B��
}					// �����D�{�� 
//======�w����========================================
void step_rst(void)		// �w���ƶ}�l
{	char i;				// �ŧi�ܼ�
	excite=1;				// �E�Ϫ��
	for(i=0;i<4;i++)			// ��X�|�ӫH��
	{	OUTPUT=excite;		// ��X�E�ϫH��
		delay5ms(times);		// ����0.25��
		excite<<=1;			// �����A�U�@�ӿE��
	}				// ���� 
}
//=======������=====================================
void delay5ms(int x)		// �����ƶ}�l
{ int i,j;	 			// �ŧi�ܼ� 
	for(i=0;i<x;i++)			// i�j�� 
		for(j=0;j<600;j++); }	// j�j��