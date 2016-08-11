#include <msp430g2553.h>
#include "I2C.h"
#include"delay.h"
void I2C_Initial( void )
{
    P2DIR &=~(SCL+SDA);
    P2OUT &=~(SCL+SDA);
    //��SCL�ܽ�����Ϊ����ܽ�
    P2DIR |= SCL;		
    I2C_Set_sck_low();
    I2C_STOP();
    delay_nms(1);
    return;
}
void I2C_Set_sda_high( void )
{
    //��SDA����Ϊ���ģʽ
    P2DIR |= SDA;		
    //SDA�ܽ����Ϊ�ߵ�ƽ
    P2OUT |= SDA;		
    
    _NOP();
    _NOP();
    return;
}
void I2C_Set_sda_low ( void )
{
    //��SDA����Ϊ���ģʽ
    P2DIR |= SDA;		
    //SDA�ܽ����Ϊ�͵�ƽ
    P2OUT &= ~(SDA);		
    
    _NOP();
    _NOP();
    return;
}
void I2C_Set_sck_high( void )
{
    //��SCL����Ϊ���ģʽ
    P2DIR |= SCL;		
    //SCL�ܽ����Ϊ�ߵ�ƽ
    P2OUT |= SCL;		
    
    _NOP();
    _NOP();
    return;
}
void I2C_Set_sck_low ( void )
{
    //��SCL����Ϊ���ģʽ
    P2DIR |= SCL;	
    //SCL�ܽ����Ϊ�͵�ƽ
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
    //��SDA����Ϊ���뷽��
    P2DIR &= ~(SDA);		
    I2C_Set_sck_high();
    for(j = 30;j > 0;j--);
    //�������
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
    //�ӳ�һ��ʱ��
    delay_nms(1);		
    return;
}
void I2C_TxByte(int nValue)
{
    int i;
    int j;
    //�Ӹ��ֽڿ�ʼ����
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
    //��SDA�ܽ�����Ϊ���뷽��
    P2DIR &= ~(SDA);			
    _NOP();
    _NOP();
    _NOP();
    _NOP();
    //�Ӹ��ֽڿ�ʼ����
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
    //�����Ƿ�Ϊ��
    if(value < 0)
    {
        value = -value;
        *neg = 1;
    }
      
    temp = (value&0x011);
    //С������
    switch(temp)
    {
        case 0x00:
        {
            //С������Ϊ0
            pBuf[0] = 0;
            pBuf[1] = 0;
            break;
        }
        case 0x01:
        {
            //С������Ϊ0.25
            pBuf[0] = 1;
            pBuf[1] = 0;
            break;
        }
        case 0x10:
        {
            //С������Ϊ0.5
            pBuf[0] = 0;
            pBuf[1] = 1;
            break;
        }
        case 0x11:
        {
            //С������Ϊ0.75
            pBuf[0] = 1;
            pBuf[1] = 1;
            break;
        }
    }
      xiaoshu=pBuf[1]*0.5+pBuf[0]*0.25;
    //С���ĳ���Ϊ2λ   
    
    //�������ִ���
    Te=value >>= 2;
 
    zzz=Te+xiaoshu;
return zzz;
}

int  I2C_ReadTemp(void)
{
    int temp=0;
    char hi;
    char lo;

   
    I2C_START();  //��ʼ�ź�
    I2C_TxByte(0x90);  //����д����,����λ7416��1001������λ��A0-2,��һλ��R/Wλ
    temp=I2C_GetACK();//��ȡACK 
    I2C_TxByte(0x00);//д��ָ��Ĵ���ָ���¶ȼĴ���
     I2C_SetNAk();//��ȡACK
    I2C_STOP();


    I2C_START();
    I2C_TxByte(0x91);//���Ͷ�����
    temp = I2C_GetACK(); //��ȡACK
    hi = I2C_RxByte(); //�����ֽ�
    I2C_SetACK(); //����ACK
    lo = I2C_RxByte();  //�����ֽ�
    I2C_SetNAk(); //����NAK
    
    temp = hi * 256 + lo;//���ߵ��ֽ���ϳ�һ������
    temp >>= 6; //��16λ������Ч
    I2C_STOP();//ֹͣ�ź�
    
    return temp;
}

