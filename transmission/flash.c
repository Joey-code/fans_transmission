#include"flash.h"
void write(char in)
{
	FCTL2 = FWKEY + FSSEL0 + FN1;
    char *Flash_ptr;
	unsigned int i;

	Flash_ptr = (char *) 0x1040;
	FCTL1 = FWKEY + ERASE;
	FCTL3 = FWKEY;
	*Flash_ptr = 0;

	FCTL1 = FWKEY + WRT;
	for (i=0; i<64; i++)
	{
	   *Flash_ptr++ = in;
	}


	FCTL1 = FWKEY;
	FCTL3 = FWKEY + LOCK;
}
 char read(void)
{
	FCTL2 = FWKEY + FSSEL0 + FN1;
	 char *Flash_ptrC;
	 char o;

	unsigned int i;

	Flash_ptrC = ( char *) 0x1040;
	FCTL1 = FWKEY + ERASE;
	FCTL3 = FWKEY;
	FCTL1 = FWKEY + WRT;

	for (i=0; i<64; i++)
	{
	  o = *Flash_ptrC++;
	}


	FCTL1 = FWKEY;
	FCTL3 = FWKEY + LOCK;
	return o;
}