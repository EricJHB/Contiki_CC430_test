/*
 * Copyright (c) 2010, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)$Id: uart0x.c,v 1.1 2010/08/24 16:23:20 joxe Exp $
 */

/*
 * Machine dependent MSP430X UART0 code.
 */

//#include <stdlib.h>

#include "contiki.h"
#ifdef __IAR_SYSTEMS_ICC__
#include "cc430x513x.h"
#else
#include <io.h>
#include <signal.h>
#endif

#include "sys/energest.h"
#include "dev/uart0.h"
#include "dev/watchdog.h"
#include "lib/ringbuf.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static int (*uart0_input_handler)(unsigned char c);

static volatile uint8_t transmitting;

#ifdef UART0_CONF_TX_WITH_INTERRUPT
#define TX_WITH_INTERRUPT UART0_CONF_TX_WITH_INTERRUPT
#else /* UART0_CONF_TX_WITH_INTERRUPT */
#define TX_WITH_INTERRUPT 1
#endif /* UART0_CONF_TX_WITH_INTERRUPT */


#if TX_WITH_INTERRUPT
#define TXBUFSIZE 64

static struct ringbuf txbuf;
static uint8_t txbuf_data[TXBUFSIZE];
#endif /* TX_WITH_INTERRUPT */

/*---------------------------------------------------------------------------*/
uint8_t
uart0_active(void)
{
  return (UCA0STAT & UCBUSY) | transmitting;
}
/*---------------------------------------------------------------------------*/
void
uart0_set_input(int (*input)(unsigned char c))
{
  uart0_input_handler = input;
}
/*---------------------------------------------------------------------------*/
void
uart0_writeb(unsigned char c)
{
  watchdog_periodic();
#if TX_WITH_INTERRUPT
  /* Put the outgoing byte on the transmission buffer. If the buffer
     is full, we just keep on trying to put the byte into the buffer
     until it is possible to put it there. */
  while(ringbuf_put(&txbuf, c) == 0);

  /* If there is no transmission going, we need to start it by putting
     the first byte into the UART. */
  if(transmitting == 0) {
    transmitting = 1;
    UCA0TXBUF = ringbuf_get(&txbuf);
  }

#else /* TX_WITH_INTERRUPT */

  /* Loop until the transmission buffer is available. */
  /*Enric while((IFG2 & UCA0TXIFG) == 0); */
  while((UCA0STAT & UCBUSY));

  /* Transmit the data. */
  UCA0TXBUF = c;
#endif /* TX_WITH_INTERRUPT */
}
/*---------------------------------------------------------------------------*/
#if ! WITH_UIP /* If WITH_UIP is defined, putchar() is defined by the SLIP driver */

#endif /* ! WITH_UIP */
/*---------------------------------------------------------------------------*/
/**
 * Initalize the RS232 port.
 *
 */
void
uart0_init(unsigned long ubr)
{

  /* RS232 */
  UCA0CTL1 |= UCSWRST;            /* Hold peripheral in reset state */
  UCA0CTL1 |= UCSSEL_2;           /* CLK = SMCLK */

  UCA0BR0 = ubr;                 /* 8MHz/115200 = 69 = 0x45 */
  UCA0BR1 = 0x00;
  UCA0MCTL = UCBRS_3;             /* Modulation UCBRSx = 3 */

#if defined (__CC430F5137__)//ERIC 2017/04/26
  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
  P1MAP5 = PM_UCA0RXD;                     // Map UCA0SIMO output to P1.5  RX
  P1MAP6 = PM_UCA0TXD;                     // Map UCA0SOMI output to P1.6  TX
  PMAPPWD = 0;                              // Lock port mapping registers

  P1DIR |= BIT6;                            // Set P1.6 as TX output
  P1SEL |= BIT5 + BIT6;                     // Select P1.5 & P1.6 to UART function
#else
  P3DIR &= ~0x20;                 /* P3.5 = USCI_A0 RXD as input */
  P3DIR |= 0x10;                  /* P3.4 = USCI_A0 TXD as output */
  P3SEL |= 0x30;                  /* P3.4,5 = USCI_A0 TXD/RXD */
  /*UCA0CTL1 &= ~UCSWRST;*/       /* Initialize USCI state machine */
#endif

  transmitting = 0;

  /* XXX Clear pending interrupts before enable */
  IFG2 &= ~UCA0RXIFG;
  IFG2 &= ~UCA0TXIFG;
  UCA0CTL1 &= ~UCSWRST;                   /* Initialize USCI state machine **before** enabling interrupts */
  IE2 |= UCA0RXIE;                        /* Enable UCA0 RX interrupt */
  /* Enable USCI_A0 TX interrupts (if TX_WITH_INTERRUPT enabled) */
#if TX_WITH_INTERRUPT
  ringbuf_init(&txbuf, txbuf_data, sizeof(txbuf_data));
  IE2 |= UCA0TXIE;                        /* Enable UCA0 TX interrupt */
#endif /* TX_WITH_INTERRUPT */
}
/*---------------------------------------------------------------------------*/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)//eric 2016/10/17 Æ¥ÅäCC430¼Ä´æÆ÷
#if defined (__CC430F5137__)
#pragma vector=USCI_A0_VECTOR
#else
#pragma vector=USCIAB0RX_VECTOR
#endif
__interrupt void
#else
interrupt(USCIAB0RX_VECTOR)
#endif
uart0_RX_TX_interrupt(void)
{
  uint8_t c;

  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  leds_toggle(LEDS_RED);
  /*RX_ISR*/
	if (UCA0STAT & UCRXERR) {
		c = UCA0RXBUF; /* Clear error flags by forcing a dummy read. */
	} else {
		c = UCA0RXBUF;
		if (uart0_input_handler != NULL) {
			if (uart0_input_handler(c)) {
				LPM4_EXIT;
			}
		}
	}

#if TX_WITH_INTERRUPT
  /*TX_ISR*/
	if ((IFG2 & UCA0TXIFG)) {
		if (ringbuf_elements(&txbuf) == 0) {
			transmitting = 0;
		} else {
			UCA0TXBUF = ringbuf_get(&txbuf);
		}
		/* In a stand-alone app won't work without this. Is the UG misleading? */
		IFG2 &= ~UCA0TXIFG;
	}
#endif /* TX_WITH_INTERRUPT */

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}
/*---------------------------------------------------------------------------*/


/**
 * \brief  Use the printf, include "stdio.h" also need use the MicroLIB
 * \param
 * ÖØ¶¨Ïò printf
 *
 */
#if 1
int
fputc(int ch, register FILE* f)
{
	uart0_writeb((uint8_t)ch);
    return ch;
}
#endif
/****************************************************************************/
/*  Function name: USART_Send_Str                                       */
/****************************************************************************/
void USART_Send_Str(char *str)
{
	unsigned char i;
	for(i=0;i<strlen(str);i++)
	{
		uart0_writeb((unsigned char)(*(str+i)));
	}
}


/************************************************
*usart DIY_printf
*eric 2016/7/20
************************************************/
void D_printf(char * Data, ...)
{
#ifdef Debug
	char buff[50];
	//unsigned int  buff_s = 0;

	va_list ap;
        va_start(ap, Data);
        vsprintf(buff,Data,ap);

	USART_Send_Str(buff);
	va_end(ap);
#else
	return ;
#endif
}


