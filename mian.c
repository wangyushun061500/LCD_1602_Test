#include "config.h"


//函数提前声明
void init (void);
void Delay100us();
u8 LCD_Data_Read (void);
void LCD_Data_Write (u8 Data);
void Write_Cmd(u8 cmd);
void Write_Dat(u8 dat);

void main (void)
{
    init();
    while (1)
    {
        
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
    D0 = (bit)(Data & 0x01);
    D1 = (bit)(Data & 0x02);
    D2 = (bit)(Data & 0x04);
    D3 = (bit)(Data & 0x08);
    D4 = (bit)(Data & 0x10);
    D5 = (bit)(Data & 0x12);
    D6 = (bit)(Data & 0x14);
    D7 = (bit)(Data & 0x18);
}

//写命令操作
void Write_Cmd(u8 cmd)
{
	RS = 0;		                	//写入命令
	RW = 0;
	LCD_Data_Write(cmd);			
    Delay100us();
	CE = 1;		                	//使能端高电平
	Delay100us();
	CE = 0;
}
 
//写数据操作
void Write_Dat(u8 dat)
{
	RS = 1;
	RW = 0;
	LCD_Data_Write(dat);
	Delay100us();
	CE = 1;			            //使能端高电平
	Delay100us();
	CE = 0;
}
