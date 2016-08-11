#ifndef I2C_H_
#define I2C_H_
#define SCL BIT4
#define SDA BIT3
//¶¨Òåº¯Êý
void I2C_Initial(void);
void I2C_Set_sda_high(void);
void I2C_Set_sda_low (void);
void I2C_Set_sck_high(void);
void I2C_Set_sck_low (void);
int  I2C_GetACK(void);
void I2C_SetACK(void);
void I2C_SetNAk(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_TxByte(int);
int  I2C_RxByte(void);
int  I2C_ReadTemp(void);
float procData(char *pBuf,int value,int *neg);
#endif
