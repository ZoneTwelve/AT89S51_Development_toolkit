/* ch05-4-4.c - 4x4��L�P4��7�`��ܾ�����, P1.4~7���@�α��y�H�� */
/* P1.0~3����L��J��,P2��7�`��ܾ�������X */
//==�ŧi��================================================
#include	<reg51.h>	// �w�q8051�Ȧs�������Y��
#define	KEYP	 P1		// ���˿�X��(���줸)����L��J��(�C�줸)
#define	SEG7P	 P2		// 7�`��ܾ�(g~a)��X��
unsigned char code TAB[17]=			// �@��7�`��ܾ�(g~a)�s�X
{ 	0xc0, 0xf9, 0xa4, 0xb0, 0x99,	// �Ʀr0-4
 	0x92, 0x82, 0xf8, 0x80, 0x98, 	// �Ʀr5-9
  	0xa0, 0x83, 0xa7, 0xa1, 0x84, 	// �r��a-e(10-14)
  	0x8e, 0xbf};  					// �r��F(15),�t��(-)
unsigned char disp[4]={ 0xbf, 0xbf, 0xbf, 0xbf }; // ��ܰ}�C��Ȭ��t��(-)
unsigned char scan[4]={ 0xef, 0xdf ,0xbf ,0x7f }; // 7��ܾ�����L�����˽X
void  delay1ms(int);				// �ŧi������
void  scanner(void);				// �ŧi���˨��
//==�D�{��================================================
main()								// �D�{���}�l 
{	while(1)						// �L�a�j��,�{���@���] 
	scanner();						// ������L�����7�q��ܾ�
}									// �D�{������ 
// === ������,�����x*1ms ================================
void delay1ms(int x)				// ���u����ƶ}�l 
{	int i,j;						// �ŧi����ܼ�i 
	for(i=0;i<x;i++)				// �p��x��,�����*1ms		
		for(j=0;j<120;j++);			// �p��120��,�����1ms 
}									// ���u����Ƶ��� 
// ======= ����4*4��L��4��7�`��ܾ���� ================
void scanner(void)					// ���˨�ƶ}�l 
{	unsigned char col,row,dig;  	// �ŧi�ܼ�(col:��,row:�C,dig:�V�ܦ�)
	unsigned char rowkey,kcode;		// �ŧi�ܼ�(rowkey:�C���,kcode:����X)
	for(col=0;col<4;col++)    		// for�j��,���˲�col��
	{	KEYP  = scan[col];			// ��4���X���˫H��,�C4�줸��J�C��
		SEG7P = disp[col];			// ��X��col��Ʀr
		rowkey= ~KEYP & 0x0f;
		// Ū�JKEYP�C4��A�ϬۦA�M����4��D�X�C���	
		if(rowkey != 0)				// �Y������
		{	if(rowkey == 0x01)     	row=0;	// �Y��0�C�Q���U
			else if(rowkey == 0x02)	row=1;	// �Y��1�C�Q���U
			else if(rowkey == 0x04)	row=2;	// �Y��2�C�Q���U
			else if(rowkey == 0x08)	row=3;	// �Y��3�C�Q���U
			kcode = 4 * col + row;			// ��X���䤧���X
			for(dig = 0; dig < 3 ; dig++)		// ��ܰ}�C����3�r
				disp[dig]=disp[dig+1];		// �N�k���s�X����1��
			disp[3]=TAB[kcode];				// ��Ƚs�X��A�g�J�̥k��
			while(rowkey != 0)				// �����s����}	
				rowkey=~KEYP & 0x0f;		// �AŪ�J�C���
		}						//	if�ԭz(�������)����	
		delay1ms(4);			//	����4ms
	}							//	for�j�鵲��(����col��)
}								//	���˨��scanner()����	