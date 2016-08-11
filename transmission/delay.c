#include <msp430g2553.h>
#define uint unsigned int
#define uchar unsigned char
void delay_1us(void) 
{
         unsigned char i;
        for (i = 0; i < 16; i++)
        asm("nop");
}
//N usÑÓÊ±º¯Êý
void delay_nus(unsigned int n) 
{
        unsigned int j;
        for (j = 0; j < n; j++)
                delay_1us();
}
 void delay_1ms()
{
        uint j; 
        uchar i;
        for (i = 0; i < 16; i++)
        for(j=0;j<1140;j++);
}
 
void delay_nms(uint n)
{
    uint x = 0;
    for(x = n;x > 0;x--)
        delay_1ms();
}