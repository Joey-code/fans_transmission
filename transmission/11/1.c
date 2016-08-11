#include<msp430g2553.h>
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR = 0x01;                             // P1.0 output, else input
  P1OUT = 0x10;                             // P1.4 set, else reset
  P1REN |= 0x10;                            // P1.4 pullup

  while (1)                                 // Test P1.4
  {
    if (0x10 & P1IN) P1OUT |= 0x01;         // if P1.4 set, set P1.0
    else P1OUT &= ~0x01;                    // else reset
  }
}


