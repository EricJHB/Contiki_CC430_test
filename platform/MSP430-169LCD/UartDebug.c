/*
 * User_debug.c
 *
 *  Created on: Mar 18, 2017
 *      Author: a0222632
 */

#ifndef USER_DEBUG_C_
#define USER_DEBUG_C_

#include "msp430.h"
#include <stdio.h>
//#include "User_debug.h"

//#define UART_PRINTF
#ifdef UART_PRINTF
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);
#endif

#ifdef UART_PRINTF
int fputc(int _c, register FILE *_fp)
{
  while(!(UCA0IFG&UCTXIFG));
  UCA0TXBUF = (unsigned char) _c;

  return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++)
  {
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF = (unsigned char) _ptr[i];
  }

  return len;
}
#endif


#endif /* USER_DEBUG_C_ */
