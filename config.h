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

#endif