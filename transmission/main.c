#include <msp430g2553.h>
#include "i2c.h"
#include "DHT11.h"
#include"flash.h"
#define F 6000
int	flag=0;
 char pBuf[10],modelflag,writepp,jisuanpp1,flag2;
int neg,value,Te,i,temp,modelcount;
float zzz,shidu,wenwen,vo,shudu2,lastpwm,pwm,data,data1,pp,mo,jisuanpp;
extern float T,H;
void Init_Sys(void);
void Init_IO(void);
void Init_TA(void);
void Init_data(void);
void deal(void);
int main( void )
{
    Init_Sys(); //系统初始化
    Init_TA();
    I2C_Initial();//I2C初始化 
     Init_IO();       //按键IO初始
      Init_data();    //数据初始化
      _EINT();
       while(1)
          {          
        deal();          
}     
}
void Init_Sys(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    if (CALBC1_16MHZ ==0xFF || CALDCO_16MHZ == 0xFF)
      {
      while(1);                 // If calibration constants erased
                              // do not load, trap CPU!!
      }
      BCSCTL1 = CALBC1_16MHZ;
      DCOCTL = CALDCO_16MHZ;  
}

void Init_TA(void)
{
 P1DIR |= 0x04;                            // P1.2 output
  P1SEL |= 0x04;                            // P1.2 TA1/2 options
  CCR0 =F;                             // PWM Period
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set
  CCR1 =0;                               // CCR1 PWM duty cycle
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, up mode
  TA1CCTL0 = CCIE;                      //TA1       // CCR0 interrupt enabled
  TA1CCR0 = 50000;
  TA1CTL = TASSEL_2 + MC_1;                  // SMCLK, 
}


void Init_data(void)
{
    Te = 0;
    value = 0;
    data=0;
    neg = 0;
    pp=0;
    for(i = 0;i < 10;i++)
    pBuf[i] = 0;
    modelcount=3000;
}
void Init_IO(void)
  {
  P1DIR&=~BIT0;
  P1REN|=BIT0;
  P1OUT|=BIT0;
  P2DIR&=~(BIT0+BIT1);
  P2REN|=(BIT0+BIT1);
  P2OUT|=(BIT0+BIT1);
  P2IES|=(BIT0+BIT1);
  P2IFG&=~(BIT0+BIT1);
  P2IE|=(BIT0+BIT1);
  P1IES|=BIT0;
  P1IFG&=~BIT0;
  P1IE|=BIT0;
   }


#pragma vector=PORT2_VECTOR
__interrupt void abjust(void) //按键调节脉宽
{
  if(flag2==1)  
            {
          pp=-pp;
            }
       if( P2IFG&BIT0)
	  {  
        P2IFG &= ~BIT0; 
     if((pwm+pp)>1)
       { 
          pwm=1;
          return;
        }
    else if((pwm+pp)<0.5)
            pp+=0.1;
      else 
            pp+=0.05;    
        	  } 
     
      if( P2IFG&BIT1)
      { 
        P2IFG &= ~BIT1;
   if((pwm+pp)<0)
        {
        pwm=0;
          return;
        }
   else if((pwm+pp)<0.5)
            pp-=0.1;
      else 
            pp-=0.05; 

      }
      if(pp<=0)  
            {
          flag2=1;
          pp=-pp;
            }
     else  flag2=0;
    writepp=(char)(pp*100);
     write(writepp);
}

#pragma vector=PORT1_VECTOR
__interrupt void model(void) //模式
{
   P1IFG &= ~BIT0; 
     modelflag=1;          
}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A (void)
{
          
          flag=flag+1;
		if(flag>31)
		{
                DHT11();
                shidu=H;
                shudu2=H*H;
                wenwen=T;
		flag=0;
              if(modelflag==1)
          {
              modelcount=modelcount-1;
          }

    }
       if(modelflag==1)                //开启模式后
         {
            if(modelcount>1)
                    {
                        if((pwm+pp)<0.5)
                          mo=0.8-((0.8-(pwm+jisuanpp))*(3000-modelcount)/3000);
                       else
                          mo=1-((1-(pwm+jisuanpp))*(3000-modelcount)/3000);
                    }                 //模式算法
            else
                    {
                    mo=0;
                    modelcount=3000;
                    modelflag=0;
                    }       //定时超过5min,恢复常态        
          }    

}

void deal(void)
{
   {
        for(i=0;i<8;i++)
              { 
               value=I2C_ReadTemp();//读出温度数据  
               zzz=procData(pBuf,value,&neg); //处理数据
               data=zzz+data;   
              }
       data=data/8; 
    }                    //ad7416测温

         data1=data; 
            
            {
       
          if(((data1-wenwen)>2)||(data1<5))
                        data1=wenwen;
              else if(((wenwen-data1)>2)||(wenwen<5))
                      data1=data1;
              else data1=(wenwen+data1)/2;  
            }   //温度处理
       
          
                {      
                  if(data1<25)  pwm=0;
                   else if(data1<28) 
                        pwm=(0.0000185*data1-0.000463)*shudu2;
                   else 
                           {                 
                               vo=(data1*(0.0000064*data1-0.000175))*shudu2-0.1;
                               pwm=vo*(vo*(0.0249*vo-0.2187)+0.7035);
                          }
                      data=0;
             }    //算法控制    
               {
                   if(pwm<0)  pwm=0;
                   else if(pwm>1) pwm=1;
                       
                           jisuanpp1=read();
                        //  jisuanpp1=10;
                            if(jisuanpp1==0xff)
                                 jisuanpp=0;
                            else 
                            jisuanpp=(float)(jisuanpp1)*0.01;
                            if(flag2==1)  
                            {
                              jisuanpp=-jisuanpp;
                            }
                          lastpwm=(pwm+jisuanpp+mo);
                   if(lastpwm<0)  lastpwm=0;
                   else if(lastpwm>1) lastpwm=1;
                        CCR1=(int)(lastpwm*F);
              }
}