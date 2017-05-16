/*
 * INFlashWR.h
 *
 *  Created on: 2016年8月29日
 *      Author: eric
 */

#ifndef _INFLASHWR_H_
#define _INFLASHWR_H_

#include "cc430x513x.h"
#define  INFlashInfoD   0x01800//-0x0187f//
#define  INFlashInfoC   0x01880//-0x018ff//amsRN
#define  INFlashInfoB   0x01900//-0x0197f//DeviceID
#define  INFlashInfoA   0x01980//-0x019ff//不可写?




void INFlashW(unsigned int AddS,unsigned char offset,unsigned char Len,unsigned char *data);
void INFlashR(unsigned int AddS,unsigned char Len,unsigned char *data);

void INFlashERASE(unsigned int Add);

#endif /* SRC_INFLASHWR_INFLASHWR_H_ */
