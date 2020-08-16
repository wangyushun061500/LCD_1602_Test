#include "config.h"


//������ǰ����
void init (void);
u8 LCD_Data_Read (void);
void Delay1us(u16 time);
void Delay1ms(u16 time);
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

//��������
bit busy,init_Done;


void main (void)
{
    init();
    uart_init();
	T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;                               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                                           //ʹ�ܴ���1�ж�
    EA = 1;
    init_Done = 0;
    Delay1ms(500);
	SendString("STC15F2K60S2\r\nUART Done!\r\n");
    LCD1602_Init();
    Delay1ms(500);
    init_Done = 1;
    SendString("LCDReSet Done!\r\n");
    while (1)
    {
        // LCD1602_SetRAM(0,0);
        // LCD1602_Data(0x41);
        // SendString("A-->Yes!\r\n");
        // Delay1ms(500);
        LCD1602_Show(0,0,"WangYushun");
        Delay1ms(1000);
        SendString("���һ������д��\r\n");
        Delay1ms(10);
        SendString("һ����ʱ\r\n");
    }
}

//��ʼ������
void init (void)
{
    P0M0 = 0x03;
    P0M1 = 0x00;
    P1M0 = 0xe0;
    P1M1 = 0x00;
    VCC = 1;
    GND = 0;
    LEDA = 1;
    LEDK = 0;
    VO = 0;
}

//1us��ʱ����
void Delay1us(u16 time)		//@24.000MHz
{
	unsigned char i;
    for (time; time > 0; time--)
    {
        _nop_();
        _nop_();
        i = 3;
        while (--i);
    }
}


//1ms��ʱ����
void Delay1ms(u16 time)		//@24.000MHz
{
	unsigned char i, j;
    for (time; time > 0; time--)
    {
        i = 24;
        j = 85;
        do
        {
            while (--j);
        } while (--i);
    }
}

//������
u8 LCD_Data_Read (void)
{
     u8 Data;
     Data = (P0 >> 2) | (P1 << 6);
     Data = (Data << 4) | (Data >> 4);
     Data = ((Data >> 2) & 0x33) | ((Data << 2) & 0xcc);
     Data = ((Data >> 1) & 0x55) | ((Data << 1) & 0xaa);
     return Data;
}

//д����
void LCD_Data_Write (u8 Data)
{
    D0 = (bit)(Data & 0x01);
    D1 = (bit)(Data & 0x02);
    D2 = (bit)(Data & 0x04);
    D3 = (bit)(Data & 0x08);
    D4 = (bit)(Data & 0x10);
    D5 = (bit)(Data & 0x20);
    D6 = (bit)(Data & 0x40);
    D7 = (bit)(Data & 0x80);
}

//�ȴ�����
void LCD1602_wait()
{
    u8 sta;
    // LCD_Data_Write(0xff);
    RS = 0;
    RW = 1;
    CE = 1;
    Delay1ms(10);
    do{
        sta = LCD_Data_Read();    //��ȡ״̬��
        Delay1ms(1);
    }while(sta & 0x80);      //bit7����1��ʾҺ����æ���ظ����ֱ����Ϊ0Ϊֹ
}


//д�������
void LCD1602_cmd(u8 cmd)
{
    if (init_Done == 1)
    {
        LCD1602_wait();
    }
	RS = 0;		                
	RW = 0;
	LCD_Data_Write(cmd);
    Delay1ms(10);	
	CE = 1;		              
	CE = 0;
}
 
//д���ݲ���
void LCD1602_Data(u8 dat)
{
    if (init_Done == 1)
    {
        LCD1602_wait();
    }
	RS = 1;
	RW = 0;
	LCD_Data_Write(dat);
    Delay1ms(10);
	CE = 1;			          
	CE = 0;
}

//LCD1602��ʼ��
void LCD1602_Init()
{
	LCD1602_cmd(0x01);
    Delay1ms(10);
    LCD1602_cmd(0x03);
    Delay1ms(10);
    LCD1602_cmd(0x06);
    Delay1us(100);
    LCD1602_cmd(0x0e);
    Delay1us(100);
    LCD1602_cmd(0x14);
    Delay1us(100);
    LCD1602_cmd(0x38);
    Delay1us(100);
}

//������ʾRAM��ʼ��ַ��x��y��Ӧ��Ļ���ַ����꣩
void LCD1602_SetRAM(u8 x,u8 y)
{
    u8 addr;
    if(y == 0)
        addr = 0x80 + x;
    else
        addr = 0xc0 + x;
    LCD1602_cmd(addr);
}

//��ʾ�ַ���
void LCD1602_Show(u8 x,u8 y,u8 *str)
{
    LCD1602_SetRAM(x,y);
    while(*str != '\0')
    {
        LCD1602_Data(*str++);
    }
}

/*uart��ʼ������*/
void uart_init ()
{
	ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
	#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8λ�ɱ䲨����
	#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9λ�ɱ䲨����,У��λ��ʼΪ1
	#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0
	#endif
}

/*�������ݷ���*/
void SendData(u8 dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
		#if (PARITYBIT == ODD_PARITY)
			TB8 = 0;                //����У��λΪ0
		#elif (PARITYBIT == EVEN_PARITY)
			TB8 = 1;                //����У��λΪ1
		#endif
    }
    else
    {
		#if (PARITYBIT == ODD_PARITY)
    	    TB8 = 1;                //����У��λΪ1
		#elif (PARITYBIT == EVEN_PARITY)
    	    TB8 = 0;                //����У��λΪ0
		#endif
    }
    busy = 1;
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
}

/*�����ַ���*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}


/*uart�жϷ������*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //���RIλ
        // P0 = SBUF;              //P0��ʾ��������
        P54 = RB8;              //P2.2��ʾУ��λ
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy = 0;               //��æ��־
    }
}
