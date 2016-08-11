#ifndef DHT11_H_
#define DHT11_H_
typedef unsigned int uint;
typedef unsigned char uchar;
#define CPU           (16000000)
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))
#define HIGH P2OUT|= BIT5;
#define LOW P2OUT &=~BIT5;
extern float T,H;
extern char receive(void);
extern void receive_init();
extern void DHT11( void );
#endif