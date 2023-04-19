#include	<reg51.h>	
#define	SEGP	P0		// SEG
#define	scanP	P1		// SEG
#define KEYP	P2		// KeyPad
#define Motor 	P3
#define digits  8		// SEG ��� 
#define counts  1000 	// scan_SEG = counts us, scan_KEY= 3*counts us
#define speed   250 	// stepper moter = speed ms/step
#define excite1	1		// �E�ϼҦ� 
#define excite2	8		// �E�ϼҦ� 
#define MAX_steps	1000// �̦h�B�� 
sbit 	buzzer=P3^7;
unsigned char	T0H,T0L,T1H,T1L;
char	scan_S=1,KEYs=0,i=0;
bit		stop_F=0;
char code TAB[17]=		// �@��7�`��ܾ�(g~a)�s�X 
{ 	0xc0, 0xf9, 0xa4, 0xb0, 0x99,
 	0x92, 0x82, 0xf8, 0x80, 0x98,
  	0xa0, 0x83, 0xa7, 0xa1, 0x84,
  	0x8e, 0xff};  				
char disp[digits]; 		// SEG buffer
char scan[4]={ 0xef, 0xdf ,0xbf ,0x7f }; 
void	beep(char);
void	delay500us(int);
void	delay1ms(int);				
void	forward(void);
void	reverse(void);
unsigned int  steps(void);// ����B�� 
//==�D�{��================================================
main()								// �D�{���}�l 
{	char i;
	EA=ET0=ET1=1;
	PT0=1;
	TMOD=0x11;
	// Timer 0 �C 1ms ���ˤC�`��ܾ��@��   
	TH0=T0H=(65536-counts)/256;
	TL0=T0L=(65536-counts)%256;
	TR0=1;
	// Timer 0 �C 20ms ����KeyPad�@��   
	TH1=T1H=(65536-20*counts)/256;
	TL1=T1L=(65536-20*counts)%256;
	TR1=1;
	// ��ܽw�İ϶�J�ť� 
	for(i=0;i<digits;i++)  disp[i]=16;
	disp[0]=0;
	beep(2);
	while(1)
	{	if (KEYs==1)   		// ���� 
		{	beep(2);
			stop_F=0;
			KEYs=0;
			forward();
		}
		else if (KEYs==2)	// ���� 
		{	beep(3);
			stop_F=0;
			KEYs=0;
			reverse();
		}
	}
}								
// === ������,�����x*1ms ================================
void delay1ms(int x)			
{	int i,j;					
	for(i=0;i<x;i++)			
		for(j=0;j<120;j++);		
}								
// ======= ����4*4��L��4��7�`��ܾ���� ================
void scan_KEY(void)	interrupt 3		// Timer 1
{	unsigned char col,row,dig;  	// �ŧi�ܼ�(col:��,row:�C,dig:�V�ܦ�)
	unsigned char rowkey,kcode;		// �ŧi�ܼ�(rowkey:�C���,kcode:����X)
	TH1=T1H;TL1=T1L;
	for(col=0;col<4;col++)    		// for�j��,���˲�col�� 
	{	KEYP  = scan[col];			// ��4���X���˫H��,�C4�줸��J�C�� 
		rowkey= ~KEYP & 0x0f;
		// Ū�JKEYP�C4��A�ϬۦA�M����4��D�X�C���	
		if(rowkey != 0)				// �Y������ 
		{	if(rowkey == 0x01)     	row=0;	// �Y��0�C�Q���U 
			else if(rowkey == 0x02)	row=1;	// �Y��1�C�Q���U 
			else if(rowkey == 0x04)	row=2;	// �Y��2�C�Q���U 
			else if(rowkey == 0x08)	row=3;	// �Y��3�C�Q���U 
			kcode = 4 * col + row;			// ��X���䤧���X 
			if (kcode==12) 					//clear ==> Key C
				for(dig = 0; dig < digits; dig++) disp[dig]=16;
			else if (kcode==13) 			// delete one bit ==> Key D
			{	for(dig = 0;dig < digits-2; dig++)
					disp[dig]=disp[dig+1];		
				disp[digits-1]=16;		
			}
			else if (kcode==14) KEYs=1;		// forward	==> Key E
			else if (kcode==15)	KEYs=2;		// reverse	==> Key F
			else if (kcode==10)	stop_F=1;	// Nop		==> Key A
			else if (kcode==11)	stop_F=1;	// Nop		==> Key B
			else
			{	for(dig = digits-1; dig > 0; dig--)	
					disp[dig]=disp[dig-1];		
				disp[0]=kcode;				
			}
			for(dig = digits-2; dig > 0; dig--)
			{	if ((disp[dig]==0)&disp[dig+1]==16) disp[dig]=16;
			}
			while(rowkey != 0)			
				rowkey=~KEYP & 0x0f;	
		}				
	}					
}						
//=================================
void  scan_SEG(void)  interrupt 1  //Timer 0
{	TH0=T0H;TL0=T0L;
	scanP=0xff;
	SEGP=TAB[disp[i]];
	scanP=~scan_S;
	scan_S<<=1;
	if (++i==digits) 
	{	scan_S=1; i=0; }
}
//========================================
void  forward(void)
{	unsigned int x,i;
	unsigned char signal=excite1; // �E�ϼҦ�  
	x=steps();
	if (x>MAX_steps) x=MAX_steps;
	for(i=0;i<x;i++)
	{	Motor=signal;
		signal=signal<<1;
		if (signal==0x10) signal=1;
		if (disp[0]>0) disp[0]--;
		else { 	disp[0]=9; 
				if (disp[1]>0) 
				{	disp[1]--;
					if ((disp[1]==0)&disp[2]==16) disp[1]=16;
				}
				else {	disp[1]=9;
						if (disp[2]>0) 
						{	disp[2]--;
							if ((disp[2]==0)&disp[3]==16) disp[2]=16;
						}
						else {	disp[2]=9;
								if (disp[3]>0) 
								{	disp[3]--;
									if ((disp[3]==0)&disp[4]==16) disp[3]=16;
								}
								else {	disp[3]=9;
										if (disp[4]>0) disp[4]--;
										if ((disp[4]==0)&disp[5]==16) disp[4]=16;
									 }
							 }
					  }
			  }
		delay1ms(speed);
		if	(stop_F) break;
	}
	beep(3);
}
//======================
void  reverse(void)
{	unsigned int x,i;
	unsigned char signal=excite2; // �E�ϼҦ�  
	x=steps();
	if (x>MAX_steps) x=MAX_steps;
	for(i=0;i<x;i++)
	{	Motor=signal;
		signal=signal>>1;
		if (signal==0) signal=8;
		if (disp[0]>0) disp[0]--;
		else { 	disp[0]=9; 
				if (disp[1]>0) 
				{	disp[1]--;
					if ((disp[1]==0)&disp[2]==16) disp[1]=16;
				}
				else {	disp[1]=9;
						if (disp[2]>0) 
						{	disp[2]--;
							if ((disp[2]==0)&disp[3]==16) disp[2]=16;
						}
						else {	disp[2]=9;
								if (disp[3]>0) 
								{	disp[3]--;
									if ((disp[3]==0)&disp[4]==16) disp[3]=16;
								}
								else {	disp[3]=9;
										if (disp[4]>0) disp[4]--;
										if ((disp[4]==0)&disp[5]==16) disp[4]=16;
									 }
							 }
					  }
			  }
		delay1ms(speed);
		if	(stop_F) break;
	}
	beep(3);
}
//======================
unsigned int  steps(void)
{	char  i;
	char  TMP[5];
	unsigned int x=0;
	beep(1);
	for(i=0;i<5;i++)
	{	if (disp[i]==16)  TMP[i]=0; 
		else TMP[i]=disp[i];
	}
	x=TMP[0]+10*TMP[1]+100*TMP[2]+1000*TMP[3]+10000*TMP[4];
	return x;
}
//======================
void	beep(char x)
{	char i,j;					
	for(i=0;i<x;i++)			
	{	for(j=0;j<100;j++)
		{	buzzer=1;delay500us(1);		
			buzzer=0;delay500us(1);
		}
		delay500us(120);		
	}								
}
//======================
void	delay500us(int x)
{	int i,j;					
	for(i=0;i<x;i++)			
		for(j=0;j<60;j++);		
}								