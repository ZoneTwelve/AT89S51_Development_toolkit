/* ch10-3-1.c - 1相驅動實驗 */
//利用delay5mDELAY副程式(5ms times), 產生驅動信號
//由P1輸出	速度為 1/(5ms timers) 步/秒
#include  <reg51.h> 		// 包含reg51.h檔 
#define  OUTPUT 	P1	// 定義輸出埠為P1
unsigned int times=50;		// 宣告延遲時間變數( 5ms) 
unsigned char excite;		// 宣告激磁變數 
void step_rst(void);		// 宣告定位函數 
void delay5ms(int);		// 宣告延遲函數 
//=====主程式======================================
main()				// 主程式開始 
{	OUTPUT=0;	// 關閉輸出 
	step_rst();	// 定位 
	while (1)	//	while迴圈 
		step_rst();			// 運轉
}					// 結束主程式 
//======定位函數========================================
void step_rst(void)		// 定位函數開始
{	char i;				// 宣告變數
	excite=1;				// 激磁初值
	for(i=0;i<4;i++)			// 輸出四個信號
	{	OUTPUT=excite;		// 輸出激磁信號
		delay5ms(times);		// 延遲0.25秒
		excite<<=1;			// 左移，下一個激磁
	}				// 結束 
}
//=======延遲函數=====================================
void delay5ms(int x)		// 延遲函數開始
{ int i,j;	 			// 宣告變數 
	for(i=0;i<x;i++)			// i迴圈 
		for(j=0;j<600;j++); }	// j迴圈
