#include <msp430g2553.h>
#include "I2C.h"
#include"delay.h"
void I2C_Initial( void )
{
    P2DIR &=~(SCL+SDA);
    P2OUT &=~(SCL+SDA);
    //将SCL管脚设置为输出管脚
    P2DIR |= SCL;		
    I2C_Set_sck_low();
    I2C_STOP();
    delay_nms(1);
    return;
}
void I2C_Set_sda_high( void )
{
    //将SDA设置为输出模式
    P2DIR |= SDA;		
    //SDA管脚输出为高电平
    P2OUT |= SDA;		
    
    _NOP();
    _NOP();
    return;
}
void I2C_Set_sda_low ( void )
{
    //将SDA设置为输出模式
    P2DIR |= SDA;		
    //SDA管脚输出为低电平
    P2OUT &= ~(SDA);		
    
    _NOP();
    _NOP();
    return;
}
void I2C_Set_sck_high( void )
{
    //将SCL设置为输出模式
    P2DIR |= SCL;		
    //SCL管脚输出为高电平
    P2OUT |= SCL;		
    
    _NOP();
    _NOP();
    return;
}
void I2C_Set_sck_low ( void )
{
    //将SCL设置为输出模式
    P2DIR |= SCL;	
    //SCL管脚输出为低电平
    P2OUT &= ~(SCL);		
    
    _NOP();
    _NOP();
    return;
}
int  I2C_GetACK(void)
{     
    unsigned char errtime=255;
    int nTemp = 0;
    int j;
    _NOP();
    _NOP();
   I2C_Set_sda_high();
    for(j = 30;j > 0;j--);
    I2C_Set_sck_low();
    for(j = 30;j > 0;j--);
    //将SDA设置为输入方向
    P2DIR &= ~(SDA);		
    I2C_Set_sck_high();
    for(j = 30;j > 0;j--);
    //获得数据
    nTemp = (int)(P2IN & SDA);	
    while(nTemp)
    { 
          errtime--;
          if(!errtime)
          {   
              I2C_STOP();
          }
        
     }
    I2C_Set_sck_low();
     for(j = 30;j > 0;j--);
    return (nTemp & SDA);
}
void I2C_SetACK(void)
{   
    I2C_Set_sck_low();
    I2C_Set_sda_low();
    I2C_Set_sck_high();
    I2C_Set_sck_low();
    return;
}
void I2C_SetNAk(void)
{
    I2C_Set_sck_low();
    I2C_Set_sda_high();
    I2C_Set_sck_high();
    I2C_Set_sck_low();
    return;
}
void I2C_START(void)
{
    int i;
    
    I2C_Set_sda_high();
    for(i = 5;i > 0;i--);
    I2C_Set_sck_high();
    for(i = 5;i > 0;i--);
    I2C_Set_sda_low();
    for(i = 5;i > 0;i--);
    I2C_Set_sck_low();
    return;
}
void I2C_STOP(void)
{
    int i;
    
    I2C_Set_sda_low();
    for(i = 5;i > 0;i--);
    I2C_Set_sck_low();
    for(i = 5;i > 0;i--);
    I2C_Set_sck_high();
    for(i = 5;i > 0;i--);
    I2C_Set_sda_high();
    for(i = 5;i > 0;i--);
    I2C_Set_sck_low();
    //延迟一点时间
    delay_nms(1);		
    return;
}
void I2C_TxByte(int nValue)
{
    int i;
    int j;
    //从高字节开始发送
    for(i = 0;i < 8;i++)
    {
         I2C_Set_sck_low();
         for(j = 30;j > 0;j--);
    	if(nValue & 0x80)
    	    I2C_Set_sda_high();
    	else
    	    I2C_Set_sda_low();
         nValue <<= 1;
    	for(j = 30;j > 0;j--);
    	I2C_Set_sck_high();
    	for(j = 30;j > 0;j--);
    	
    }
    I2C_Set_sck_low();
    for(j = 30;j > 0;j--);
    return;
}

int  I2C_RxByte(void)
{
    int nTemp = 0;
    int i;
    int j;
    
    I2C_Set_sda_high();
    //将SDA管脚设置为输入方向
    P2DIR &= ~(SDA);			
    _NOP();
    _NOP();
    _NOP();
    _NOP();
    //从高字节开始接收
    for(i = 0;i < 8;i++)
    {
    	I2C_Set_sck_low();
        for(j = 30;j > 0;j--);
        I2C_Set_sck_high();
    	for(j = 30;j > 0;j--);
    	if(P2IN & SDA)
    	{
    	    nTemp |= (0x01 << (7 - i));
    	}

    }     	
   I2C_Set_sck_low(); 
  for(j = 30;j > 0;j--);
    return nTemp;
}


float procData(char *pBuf,int value,int *neg)
{
  int Te = 0;
    int temp,i;
    float  xiaoshu=0;
    float zzz;
    //数据是否为负
    if(value < 0)
    {
        value = -value;
        *neg = 1;
    }
      
    temp = (value&0x011);
    //小数处理
    switch(temp)
    {
        case 0x00:
        {
            //小数部分为0
            pBuf[0] = 0;
            pBuf[1] = 0;
            break;
        }
        case 0x01:
        {
            //小数部分为0.25
            pBuf[0] = 1;
            pBuf[1] = 0;
            break;
        }
        case 0x10:
        {
            //小数部分为0.5
            pBuf[0] = 0;
            pBuf[1] = 1;
            break;
        }
        case 0x11:
        {
            //小数部分为0.75
            pBuf[0] = 1;
            pBuf[1] = 1;
            break;
        }
    }
      xiaoshu=pBuf[1]*0.5+pBuf[0]*0.25;
    //小数的长度为2位   
    
    //整数部分处理
    Te=value >>= 2;
 
    zzz=Te+xiaoshu;
return zzz;
}

int  I2C_ReadTemp(void)
{
    int temp=0;
    char hi;
    char lo;

   
    I2C_START();  //起始信号
    I2C_TxByte(0x90);  //发送写命令,高四位7416是1001，低三位是A0-2,第一位是R/W位
    temp=I2C_GetACK();//读取ACK 
    I2C_TxByte(0x00);//写入指针寄存器指向温度寄存器
     I2C_SetNAk();//读取ACK
    I2C_STOP();


    I2C_START();
    I2C_TxByte(0x91);//发送读命令
    temp = I2C_GetACK(); //读取ACK
    hi = I2C_RxByte(); //读高字节
    I2C_SetACK(); //发送ACK
    lo = I2C_RxByte();  //读低字节
    I2C_SetNAk(); //发送NAK
    
    temp = hi * 256 + lo;//将高低字节组合成一个整数
    temp >>= 6; //低16位数据无效
    I2C_STOP();//停止信号
    
    return temp;
}

