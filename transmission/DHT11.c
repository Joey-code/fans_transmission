#include <msp430g2553.h>
#include "DHT11.h"
unsigned char temph,templ,humdh,humdl,check,cal;
float templ_,humdl_,T,H,TL,HL;
 char count;
char receive(void)             //接受函数
{
  unsigned char num=0,tem,cnt;              //临时变量用于存储接受数据
  for(cnt=0;cnt<8;cnt++)
  {
    tem=0;
    count=2;
    while((!(P2IN&BIT5))&&count++);       //等待14us的低电平结束
    delay_us(30);  
    if((P2IN&BIT5))           /////////长于30us定义为1
    {
      tem=1; 
      count=2;
      while((P2IN&BIT5)&&count++);        //等待一位的采集结束
    }
    num<<=1;
    num|=tem;
  }
  return num;
}
void receive_init()
{
 
  P2DIR|=BIT5;  
  LOW;
  delay_ms(25);
  HIGH;
  delay_us(40);         //20--40
  P2DIR &=~BIT5; 
  if(!(P2IN&BIT5));             //从机发出响应信号
  {
     count=2;
    while((!(P2IN&BIT5))&&count++);//等待响应响应结束
     count=2;
    while((P2IN&BIT5)&&count++);//开始采集 
    humdh=receive();
    humdl=receive();
    temph=receive();
    templ=receive();
    check=receive();
  } 
}
void DHT11( void )
{
      char i;
    receive_init();
    cal=humdh+humdl+temph+templ;
    if(cal==check)
    {
        TL=0;
        HL=0;
      if(!(templ+humdl==0))
      {
        for(i=8;i>0;i--)
        {
          templ_=templ>>(i-1);
          TL+=1/(2^(i-9))*templ_;
            humdl_=humdl>>(i-1);
          HL+=1/(2^(i-9))*templ_;
          }
        }
      T=TL+temph;
      H=HL+humdh;
    }
  } 

