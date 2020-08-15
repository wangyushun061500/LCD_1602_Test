#ifndef __CONFIG_H_
#define __CONFG_H_

#include "15w.h"
#include "INTRINS.H"

/***数据类型定义（u表示数据，数字表示数据的长度）***/
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

/*引脚定义**/
#define VSS P17
#define GND P16 
#define VO P15
#define RS P14
#define RW P13
#define CE P12
#define D0 P11
#define D1 P10
#define D2 P07
#define D3 P06
#define D4 P05
#define D5 P04
#define D6 P03
#define D7 P02
#define LEDA P01
#define LEDK P00

// 声明串口传输设置

#define FOSC 24000000L		//系统频率
#define BAUD 115200 		//波特率
#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验
#define PARITYBIT NONE_PARITY   //定义校验位
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

#endif