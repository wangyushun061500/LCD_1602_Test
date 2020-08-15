#include "config.h"

//函数提前声明
void init (void);

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