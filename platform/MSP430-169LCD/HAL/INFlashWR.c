/*
 * INFlashWR.c
 *
 *  Created on: 2016年8月29日
 *      Author: eric
 */
#include "INFlashWR.h"

/*
 * INFlashW
 * write data into flash-inside
 * Adds 参数地址，具体哪个Segment（InfoA-D）
 * offset  写入参数的偏移量
 * Len 写入参数的长度
 * *data 参数字段
 */
void INFlashW(unsigned int AddS,unsigned char offset,unsigned char Len,unsigned char *data)
{
	unsigned int i;
	unsigned char WBuff[128];
	char * Flash_ptr;                         // Initialize Flash pointer
	Flash_ptr = (char *) AddS;

	__disable_interrupt();                    // 5xx Workaround: Disable global
	                                            // interrupt while erasing. Re-Enable
	                                            // GIE if needed

	for (i = 0; i < 128; i++)
	{
		WBuff[i]= *Flash_ptr++;                 // Write value to flash
	}

	for (i = 0; i < Len; i++)                  //添加写入参数
	{
		WBuff[offset+i]= *data++;                 // Write value to flash
	}
	Flash_ptr = (char *) AddS;

	FCTL3 = FWKEY;                            // Clear Lock bit
	FCTL1 = FWKEY+ERASE;                      // Set Erase bit
	*Flash_ptr = 0;                           // Dummy write to erase Flash seg
	FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation


	for(i = 0; i < 128; i++)
	{
		*Flash_ptr++ = WBuff[i];
		while(FCTL3&BUSY);              // 等待写操作完成 // Write value to flash
	}
	//*Flash_ptr = 0xaa;
	while(FCTL3&BUSY);              // 等待写操作完成 // Write value to flash
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

	__enable_interrupt();
}

void halInternalFlashWrite(unsigned int AddS,unsigned char *data,unsigned int len)
{
        unsigned int i;
	char * Flash_ptr;                         // Initialize Flash pointer
	Flash_ptr = (char *) AddS;

	__disable_interrupt();                    // 5xx Workaround: Disable global
	                                            // interrupt while erasing. Re-Enable
	                                            // GIE if needed


	FCTL3 = FWKEY;                            // Clear Lock bit
	//FCTL1 = FWKEY+ERASE;                      // Set Erase bit
	//*Flash_ptr = 0;                           // Dummy write to erase Flash seg
	FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation


	for(i = 0; i < len; i++)
	{
		*Flash_ptr++ = data[i];
		while(FCTL3&BUSY);              // 等待写操作完成 // Write value to flash
	}
	//*Flash_ptr = 0xaa;
	while(FCTL3&BUSY);              // 等待写操作完成 // Write value to flash
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

	__enable_interrupt();
  
}

/*
 * INFlashERASE
 * ERASE data(Segment erase) into flash-inside
 * for example:Add=InfoC    0x1880--0x18ff  infoC will be erased
 */
void INFlashERASE(unsigned int Add)
{	
	char * Flash_ptr;                         // Initialize Flash pointer
	Flash_ptr = (char *) Add;

	__disable_interrupt();                    // 5xx Workaround: Disable global
	                                            // interrupt while erasing. Re-Enable
	                                            // GIE if needed
	FCTL3 = FWKEY;                            // Clear Lock bit
	FCTL1 = FWKEY+ERASE;                      // Set Erase bit
	*Flash_ptr = 0;                           // Dummy write to erase Flash seg
	FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

	__enable_interrupt();
}

/*
 * INFlashR
 * read data from flash-inside
 * Adds 参数地址
 * Len 参数长度
 * *data 参数字段输出
 */
void INFlashR(unsigned int AddS,unsigned char Len,unsigned char *data)
{
	unsigned int i;
	char * Flash_ptr;                         // Initialize Flash pointer
	Flash_ptr = (char *) AddS;//

	__disable_interrupt();

	for (i = 0; i < Len; i++)
	{
		*data++ = *Flash_ptr++;                 // Write value to flash
	}

	__enable_interrupt();
}



