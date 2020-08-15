#include "config.h"


//函数提前声明
void init (void);
void Delay100us();
u8 LCD_Data_Read (void);
void LCD_Data_Write (u8 Data);
void LCD1602_wait();
void LCD1602_cmd(u8 cmd);
void LCD1602_Data(u8 dat);
void LCD1602_Init();
void LCD1602_SetRAM(u8 x,u8 y);
void LCD1602_Show(u8 x,u8 y,u8 *str);
void uart_init();
void SendData(u8 dat);
void SendString(char *s);
void Delay1000ms();

//变量声明
bit busy;


void main (void)
{
    init();
    uart_init();
	T2L = (65536 - (FOSC/4/BAUD));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
    ES = 1;                     //使能串口1中断
    EA = 1;
	Delay1000ms();
	SendString("STC15F2K60S2\r\nUart ON !\r\n");
    LCD1602_Init();
    Delay1000ms();
    LCD1602_cmd(0x80);
    LCD1602_Data('A');
    while (1)
    {
        LCD1602_cmd(0x18);
        // LCD1602_Show(2,0,"Xiaomo_HAA");
        // LCD1602_Show(0,11,"HAppy!");
        Delay1000ms();
    }
    
}

//初始化函数
void init (void)
{
    P0M0 = 0x03;
    P0M1 = 0x00;
    P1M0 = 0xe0;
    P1M1 = 0x00;
    VSS = 1;
    GND = 0;
    LEDA = 1;
    LEDK = 0;
}

//延时函数100us
void Delay100us()		//@24.000MHz
{
	unsigned char i, j;

	i = 3;
	j = 82;
	do
	{
		while (--j);
	} while (--i);
}

//读数据
u8 LCD_Data_Read (void)
{
     u8 Data;
     Data = (P0 >> 2) | (P1 << 6);
     Data = (Data << 4) | (Data >> 4);
     Data = ((Data >> 2) & 0x33) | ((Data << 2) & 0xcc);
     Data = ((Data >> 1) & 0x55) | ((Data << 1) & 0xaa);
     return Data;
}

//写数据
void LCD_Data_Write (u8 Data)
{
    D0 = (Data & 0x01);
    D1 = (bit)(Data & 0x02);
    D2 = (bit)(Data & 0x04);
    D3 = (bit)(Data & 0x08);
    D4 = (bit)(Data & 0x10);
    D5 = (bit)(Data & 0x20);
    D6 = (bit)(Data & 0x40);
    D7 = (bit)(Data & 0x80);
}

//等待操作
void LCD1602_wait()
{
    u8 sta;
    LCD_Data_Write(0xff);
    RS = 0;
    RW = 1;
 
    do{
        CE = 1;
        sta = LCD_Data_Read();    //读取状态字
        CE = 0;      
    }while(sta & 0x80);      //bit7等于1表示液晶正忙，重复检测直到其为0为止
}


//写命令操作
void LCD1602_cmd(u8 cmd)
{
    LCD1602_wait();
	RS = 0;		                
	RW = 0;
	LCD_Data_Write(cmd);			
	CE = 1;		              
	CE = 0;
}
 
//写数据操作
void LCD1602_Data(u8 dat)
{
    LCD1602_wait();
	RS = 1;
	RW = 0;
	LCD_Data_Write(dat);
	CE = 1;			          
	CE = 0;
}

//LCD1602初始化
void LCD1602_Init()
{
	LCD1602_cmd(0x38);		//设置显示模式
	LCD1602_cmd(0x0c);		//开显示不显示光标，光标不闪烁
	LCD1602_cmd(0x06);		//写一个指针+1
	LCD1602_cmd(0x01);		//清屏
}

//设置显示RAM起始地址（x，y对应屏幕上字符坐标）
void LCD1602_SetRAM(u8 x,u8 y)
{
    u8 addr;
    if(y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    LCD1602_cmd(addr | 0x80);
}

//显示字符串
void LCD1602_Show(u8 x,u8 y,u8 *str)
{
    LCD1602_SetRAM(x,y);
    while(*str != '\0')
    {
        LCD1602_Data(*str++);
    }
}

/*uart初始化函数*/
void uart_init ()
{
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
	#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8位可变波特率
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9位可变波特率,校验位初始为1
	#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9位可变波特率,校验位初始为0
	#endif
}

/*串口数据发送*/
void SendData(u8 dat)
{
    while (busy);               //等待前面的数据发送完成
    ACC = dat;                  //获取校验位P (PSW.0)
    if (P)                      //根据P来设置校验位
    {
		#if (PARITYBIT == ODD_PARITY)
			TB8 = 0;                //设置校验位为0
		#elif (PARITYBIT == EVEN_PARITY)
			TB8 = 1;                //设置校验位为1
		#endif
    }
    else
    {
		#if (PARITYBIT == ODD_PARITY)
    	    TB8 = 1;                //设置校验位为1
		#elif (PARITYBIT == EVEN_PARITY)
    	    TB8 = 0;                //设置校验位为0
		#endif
    }
    busy = 1;
    SBUF = ACC;                 //写数据到UART数据寄存器
}

/*发送字符串*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

//1000ms延时函数
void Delay1000ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 92;
	j = 50;
	k = 238;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


/*uart中断服务程序*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
        // P0 = SBUF;              //P0显示串口数据
        P54 = RB8;              //P2.2显示校验位
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}
