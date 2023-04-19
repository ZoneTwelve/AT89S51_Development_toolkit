/* ch05-4-3.c - 4�ӤC�`��ܾ��]���O����,P1�����˫H�� P2���C�`��ܾ� */
//==�ŧi��==================================
#include  <reg51.h>     // �w�q8051�Ȧs�������Y��,P2-17~19 
#define	SCANP	P1		// �w�q���˽X��Port 1��X 
#define	SEG7P	P2		// �w�q�C�`��ܽX��Port 2��X 
char code TAB[11]={	0xc0, 0xf9, 0xa4, 0xb0, 0x99,			// �Ʀr0-4
				  	0x92, 0x83, 0xf8, 0x80, 0x98, 0xff };	// �Ʀr5-9,�ť�
#define  counts  8	// �ŧi�r�ռƶq	
#define  digits  8	// �ŧi�C�`��ܾ����	
char disp[2*digits+counts-1]={	10,10,10,10,10,10,10,10,
								2,9,9,9,2,9,9,3,
								10,10,10,10,10,10,10};//--------29992993-------
void delay1ms(int);	// �ŧi������ 
void scanner(char);	// ���˨��
//==�D�{��===================================
main()				// �D�{���}�l 
{	char i;			// �ŧi�ܼ�i
	while(1)		// �L�a�j��,�{���@���] 
		for(i=0;i<digits+counts;i++)	// ���count�C�r��,for�j��(�r��i)�}�l
			scanner(i);	// ���˲�i�C�r��
}					// �D�{������
//==�Ƶ{��====================================
/* ������,�����x*1ms */
void delay1ms(int x)	// �����ƶ}�l 
{	int i,j;			// �ŧi����ܼ�i,j 
	for (i=0;i<x;i++)	// �p��x��,����x*1ms 
		for (j=0;j<120;j++);	// �p��120���A����1ms 
}					// �����Ƶ��� 
/* ���˦r�ը��,��ܲ�x�ռƦr */
void scanner(char x)	// ���˦r�ը�ƶ}�l
{	char i,j,BCD,scan;	// �ŧi�ܼ�
	for (i=0;i<30;i++)	// ����30��i�j��
	{	scan=1;		// ���˫H�����0000 0001
		for (j=0;j<digits;j++)	// ����4�ӼƦrj�j��
		{	SEG7P=0xff;	// �����C�q��ܾ�(����{��)
			SCANP=~scan;	// ��X���˫H��(�C4�줸)
			BCD=disp[x+digits-1-j];	// Ū����x�ղ�j�ӼƦr��BCD�X
			SEG7P=TAB[BCD];	// ��X�ܤC�`��ܾ�
			delay1ms(16/digits);	// ����
			scan<<=1;	// ���ͤU�ӱ��˫H��
		}			// ��������4�ӼƦrj�j��
	}				// ��������30��i�j��
}					//	scanner��Ƶ���