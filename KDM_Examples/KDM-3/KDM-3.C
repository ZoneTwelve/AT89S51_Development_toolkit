/* ch05-4-4.c - 4x4鍵盤與4個7節顯示器實驗, P1.4~7為共用掃描信號 */
/* P1.0~3為鍵盤輸入值,P2為7節顯示器直接輸出 */
//==宣告區================================================
#include	<reg51.h>	// 定義8051暫存器之標頭檔
#define	KEYP	 P1		// 掃瞄輸出埠(高位元)及鍵盤輸入埠(低位元)
#define	SEG7P	 P2		// 7節顯示器(g~a)輸出埤
unsigned char code TAB[17]=			// 共陽7節顯示器(g~a)編碼
{ 	0xc0, 0xf9, 0xa4, 0xb0, 0x99,	// 數字0-4
 	0x92, 0x82, 0xf8, 0x80, 0x98, 	// 數字5-9
  	0xa0, 0x83, 0xa7, 0xa1, 0x84, 	// 字母a-e(10-14)
  	0x8e, 0xbf};  					// 字母F(15),負號(-)
unsigned char disp[4]={ 0xbf, 0xbf, 0xbf, 0xbf }; // 顯示陣列初值為負號(-)
unsigned char scan[4]={ 0xef, 0xdf ,0xbf ,0x7f }; // 7顯示器及鍵盤之掃瞄碼
void  delay1ms(int);				// 宣告延遲函數
void  scanner(void);				// 宣告掃瞄函數
//==主程式================================================
main()								// 主程式開始 
{	while(1)						// 無窮迴圈,程式一直跑 
	scanner();						// 掃瞄鍵盤及顯示7段顯示器
}									// 主程式結束 
// === 延遲函數,延遲約x*1ms ================================
void delay1ms(int x)				// 防彈跳函數開始 
{	int i,j;						// 宣告整數變數i 
	for(i=0;i<x;i++)				// 計數x次,延遲約*1ms		
		for(j=0;j<120;j++);			// 計數120次,延遲約1ms 
}									// 防彈跳函數結束 
// ======= 掃瞄4*4鍵盤及4個7節顯示器函數 ================
void scanner(void)					// 掃瞄函數開始 
{	unsigned char col,row,dig;  	// 宣告變數(col:行,row:列,dig:顥示位)
	unsigned char rowkey,kcode;		// 宣告變數(rowkey:列鍵值,kcode:按鍵碼)
	for(col=0;col<4;col++)    		// for迴圈,掃瞄第col行
	{	KEYP  = scan[col];			// 高4位輸出掃瞄信號,低4位元輸入列值
		SEG7P = disp[col];			// 輸出第col行數字
		rowkey= ~KEYP & 0x0f;
		// 讀入KEYP低4位，反相再清除高4位求出列鍵值	
		if(rowkey != 0)				// 若有按鍵
		{	if(rowkey == 0x01)     	row=0;	// 若第0列被按下
			else if(rowkey == 0x02)	row=1;	// 若第1列被按下
			else if(rowkey == 0x04)	row=2;	// 若第2列被按下
			else if(rowkey == 0x08)	row=3;	// 若第3列被按下
			kcode = 4 * col + row;			// 算出按鍵之號碼
			for(dig = 0; dig < 3 ; dig++)		// 顯示陣列之左3字
				disp[dig]=disp[dig+1];		// 將右側編碼左移1位
			disp[3]=TAB[kcode];				// 鍵值編碼後，寫入最右側
			while(rowkey != 0)				// 當按鈕未放開	
				rowkey=~KEYP & 0x0f;		// 再讀入列鍵值
		}						//	if敘述(有按鍵時)結束	
		delay1ms(4);			//	延遲4ms
	}							//	for迴圈結束(掃瞄col行)
}								//	掃瞄函數scanner()結束	
