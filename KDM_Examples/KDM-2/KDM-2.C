/* ch05-4-3.c - 4個七節顯示器跑馬燈實驗,P1為掃瞄信號 P2接七節顯示器 */
//==宣告區==================================
#include  <reg51.h>     // 定義8051暫存器之標頭檔,P2-17~19 
#define	SCANP	P1		// 定義掃瞄碼由Port 1輸出 
#define	SEG7P	P2		// 定義七節顯示碼由Port 2輸出 
char code TAB[11]={	0xc0, 0xf9, 0xa4, 0xb0, 0x99,			// 數字0-4
				  	0x92, 0x83, 0xf8, 0x80, 0x98, 0xff };	// 數字5-9,空白
#define  counts  8	// 宣告字組數量	
#define  digits  8	// 宣告七節顯示器位數	
char disp[2*digits+counts-1]={	10,10,10,10,10,10,10,10,
								2,9,9,9,2,9,9,3,
								10,10,10,10,10,10,10};//--------29992993-------
void delay1ms(int);	// 宣告延遲函數 
void scanner(char);	// 掃瞄函數
//==主程式===================================
main()				// 主程式開始 
{	char i;			// 宣告變數i
	while(1)		// 無窮迴圈,程式一直跑 
		for(i=0;i<digits+counts;i++)	// 顯示count列字組,for迴圈(字組i)開始
			scanner(i);	// 掃瞄第i列字組
}					// 主程式結束
//==副程式====================================
/* 延遲函數,延遲約x*1ms */
void delay1ms(int x)	// 延遲函數開始 
{	int i,j;			// 宣告整數變數i,j 
	for (i=0;i<x;i++)	// 計數x次,延遲x*1ms 
		for (j=0;j<120;j++);	// 計數120次，延遲1ms 
}					// 延遲函數結束 
/* 掃瞄字組函數,顯示第x組數字 */
void scanner(char x)	// 掃瞄字組函數開始
{	char i,j,BCD,scan;	// 宣告變數
	for (i=0;i<30;i++)	// 掃瞄30次i迴圈
	{	scan=1;		// 掃瞄信號初值0000 0001
		for (j=0;j<digits;j++)	// 掃瞄4個數字j迴圈
		{	SEG7P=0xff;	// 關閉七段顯示器(防止閃動)
			SCANP=~scan;	// 輸出掃瞄信號(低4位元)
			BCD=disp[x+digits-1-j];	// 讀取第x組第j個數字之BCD碼
			SEG7P=TAB[BCD];	// 輸出至七節顯示器
			delay1ms(16/digits);	// 延遲
			scan<<=1;	// 產生下個掃瞄信號
		}			// 結束掃瞄4個數字j迴圈
	}				// 結束掃瞄30次i迴圈
}					//	scanner函數結束
