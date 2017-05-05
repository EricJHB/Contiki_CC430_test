/* -*- C -*- */
/* @(#)$Id: contiki-conf.h,v 1.91 2011/01/09 21:04:14 adamdunkels Exp $ */

#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

//added by jelline
#ifndef BV
#define BV(x) (1<<(x))
#endif

#define AUTOSTART_ENABLE 1  //added by jelline

#ifdef PLATFORM_CONF_H
#include PLATFORM_CONF_H
#else
#include "platform-conf.h"
#endif /* PLATFORM_CONF_H */


#define CONTIKI_TARGET_WISMOTE 1 //eric  2017/05/03

#if defined (__CC430F5137__) //eric 2016/10/17 Æ¥ÅäCC430¼Ä´æÆ÷
#define IE1       UCA0IE
#define IE2       UCA0IE
#define UCA0RXIE  UCRXIE
#define UCA0TXIE  UCTXIE
#define IFG2      UCA0IFG
#define UCA0RXIFG UCRXIFG
#define UCA0TXIFG UCTXIFG
#define TACCR0    TA0CCR0
#define TACCR1 	  TA0CCR1
#define TACCTL0	  TA0CCTL0
#define TACCTL1   TA0CCTL1
#define TACTL     TA0CTL
#define TAIV      TA0IV
#define TAR       TA0R
#define U0BR0     UCA0BR0
#define U0BR1     UCA0BR1
#define U0CTL     UCA0CTL
#define CCTL2     TA0CCTL2
#define SREF_1    ADC12SREF_1
/* ADC12MCTLx Control Bits */
#define INCH0             ADC12INCH0            /* ADC12 Input Channel Select Bit 0 */
#define INCH1             ADC12INCH1           /* ADC12 Input Channel Select Bit 1 */
#define INCH2             ADC12INCH2           /* ADC12 Input Channel Select Bit 2 */
#define INCH3             ADC12INCH3          /* ADC12 Input Channel Select Bit 3 */
#define SREF0             ADC12SREF0           /* ADC12 Select Reference Bit 0 */
#define SREF1             ADC12SREF1            /* ADC12 Select Reference Bit 1 */
#define SREF2             ADC12SREF2           /* ADC12 Select Reference Bit 2 */
#define EOS               ADC12EOS              /* ADC12 End of Sequence */

#define INCH_0           ADC12INCH_0           /* ADC12 Input Channel 0 */
#define INCH_1           ADC12INCH_1           /* ADC12 Input Channel 1 */
#define INCH_2           ADC12INCH_2           /* ADC12 Input Channel 2 */
#define INCH_3           ADC12INCH_3            /* ADC12 Input Channel 3 */
#define INCH_4           ADC12INCH_4            /* ADC12 Input Channel 4 */
#define INCH_5           ADC12INCH_5            /* ADC12 Input Channel 5 */
#define INCH_6           ADC12INCH_6            /* ADC12 Input Channel 6 */
#define INCH_7           ADC12INCH_7            /* ADC12 Input Channel 7 */
#define INCH_8           ADC12INCH_8            /* ADC12 Input Channel 8 */
#define INCH_9           ADC12INCH_9            /* ADC12 Input Channel 9 */
#define INCH_10          ADC12INCH_10            /* ADC12 Input Channel 10 */
#define INCH_11          ADC12INCH_11           /* ADC12 Input Channel 11 */
#define INCH_12          ADC12INCH_12           /* ADC12 Input Channel 12 */
#define INCH_13          ADC12INCH_13           /* ADC12 Input Channel 13 */
#define INCH_14          ADC12INCH_14           /* ADC12 Input Channel 14 */
#define INCH_15          ADC12INCH_15           /* ADC12 Input Channel 15 */

#define SREF_0          ADC12SREF_0           /* ADC12 Select Reference 0 */
#define SREF_1          ADC12SREF_1           /* ADC12 Select Reference 1 */
#define SREF_2          ADC12SREF_2           /* ADC12 Select Reference 2 */
#define SREF_3          ADC12SREF_3           /* ADC12 Select Reference 3 */
#define SREF_4          ADC12SREF_4           /* ADC12 Select Reference 4 */
#define SREF_5          ADC12SREF_5           /* ADC12 Select Reference 5 */
#define SREF_6          ADC12SREF_6           /* ADC12 Select Reference 6 */
#define SREF_7          ADC12SREF_7           /* ADC12 Select Reference 7 */
#endif

#ifndef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     csma_driver
#endif /* NETSTACK_CONF_MAC */

#ifndef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     contikimac_driver
#endif /* NETSTACK_CONF_RDC */

#ifndef NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 8
#endif /* NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE */

#ifndef NETSTACK_CONF_RADIO
#define NETSTACK_CONF_RADIO   cc2420_driver
#endif /* NETSTACK_CONF_RADIO */

#ifndef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154
#endif /* NETSTACK_CONF_FRAMER */

#ifndef CC2420_CONF_AUTOACK
#define CC2420_CONF_AUTOACK              1
#endif /* CC2420_CONF_AUTOACK */


#if WITH_UIP6
/* Network setup for IPv6 */
#define NETSTACK_CONF_NETWORK sicslowpan_driver

/* Specify a minimum packet size for 6lowpan compression to be
   enabled. This is needed for ContikiMAC, which needs packets to be
   larger than a specified size, if no ContikiMAC header should be
   used. */
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD 63
#define CONTIKIMAC_CONF_WITH_CONTIKIMAC_HEADER 0

#define CXMAC_CONF_ANNOUNCEMENTS         0
#define XMAC_CONF_ANNOUNCEMENTS          0

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM                8
#endif

#else /* WITH_UIP6 */

/* Network setup for non-IPv6 (rime). */

#define NETSTACK_CONF_NETWORK rime_driver

#define COLLECT_CONF_ANNOUNCEMENTS       1
#define CXMAC_CONF_ANNOUNCEMENTS         0
#define XMAC_CONF_ANNOUNCEMENTS          0
#define CONTIKIMAC_CONF_ANNOUNCEMENTS    0

#define CONTIKIMAC_CONF_COMPOWER         1
#define XMAC_CONF_COMPOWER               1
#define CXMAC_CONF_COMPOWER              1

#ifndef COLLECT_NEIGHBOR_CONF_MAX_COLLECT_NEIGHBORS
#define COLLECT_NEIGHBOR_CONF_MAX_COLLECT_NEIGHBORS     32
#endif /* COLLECT_NEIGHBOR_CONF_MAX_COLLECT_NEIGHBORS */

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM                16
#endif /* QUEUEBUF_CONF_NUM */

#ifndef TIMESYNCH_CONF_ENABLED
#define TIMESYNCH_CONF_ENABLED           0
#endif /* TIMESYNCH_CONF_ENABLED */

#if TIMESYNCH_CONF_ENABLED
/* CC2420 SDF timestamps must be on if timesynch is enabled. */
#undef CC2420_CONF_SFD_TIMESTAMPS
#define CC2420_CONF_SFD_TIMESTAMPS       1
#endif /* TIMESYNCH_CONF_ENABLED */

#endif /* WITH_UIP6 */

#define PACKETBUF_CONF_ATTRS_INLINE 1

#ifndef RF_CHANNEL
#define RF_CHANNEL              26
#endif /* RF_CHANNEL */

#define CONTIKIMAC_CONF_BROADCAST_RATE_LIMIT 0

#define IEEE802154_CONF_PANID       0xABCD

#define SHELL_VARS_CONF_RAM_BEGIN 0x1100
#define SHELL_VARS_CONF_RAM_END 0x2000

#define PROFILE_CONF_ON 0
#ifndef ENERGEST_CONF_ON
#define ENERGEST_CONF_ON 1
#endif /* ENERGEST_CONF_ON */

#define ELFLOADER_CONF_TEXT_IN_ROM 0
#ifndef ELFLOADER_CONF_DATAMEMORY_SIZE
#define ELFLOADER_CONF_DATAMEMORY_SIZE 0x400
#endif /* ELFLOADER_CONF_DATAMEMORY_SIZE */
#ifndef ELFLOADER_CONF_TEXTMEMORY_SIZE
#define ELFLOADER_CONF_TEXTMEMORY_SIZE 0x800
#endif /* ELFLOADER_CONF_TEXTMEMORY_SIZE */


#define AODV_COMPLIANCE
#define AODV_NUM_RT_ENTRIES 32

#define WITH_ASCII 1

#define PROCESS_CONF_NUMEVENTS 8
#define PROCESS_CONF_STATS 1
/*#define PROCESS_CONF_FASTPOLL    4*/

#ifdef WITH_UIP6

#define RIMEADDR_CONF_SIZE              8

#define UIP_CONF_LL_802154              1
#define UIP_CONF_LLH_LEN                0

#define UIP_CONF_ROUTER                 1
#ifndef UIP_CONF_IPV6_RPL
#define UIP_CONF_IPV6_RPL               1
#endif /* UIP_CONF_IPV6_RPL */

/* configure number of neighbors and routes */
#ifndef UIP_CONF_DS6_NBR_NBU
#define UIP_CONF_DS6_NBR_NBU     30
#endif /* UIP_CONF_DS6_NBR_NBU */
#ifndef UIP_CONF_DS6_ROUTE_NBU
#define UIP_CONF_DS6_ROUTE_NBU   30
#endif /* UIP_CONF_DS6_ROUTE_NBU */

#define UIP_CONF_ND6_SEND_RA		0
#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER      10000

#define UIP_CONF_IPV6                   1
#ifndef UIP_CONF_IPV6_QUEUE_PKT
#define UIP_CONF_IPV6_QUEUE_PKT         0
#endif /* UIP_CONF_IPV6_QUEUE_PKT */
#define UIP_CONF_IPV6_CHECKS            1
#define UIP_CONF_IPV6_REASSEMBLY        0
#define UIP_CONF_NETIF_MAX_ADDRESSES    3
#define UIP_CONF_ND6_MAX_PREFIXES       3
#define UIP_CONF_ND6_MAX_NEIGHBORS      4
#define UIP_CONF_ND6_MAX_DEFROUTERS     2
#define UIP_CONF_IP_FORWARD             0
#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE		240
#endif

#define SICSLOWPAN_CONF_COMPRESSION_IPV6        0
#define SICSLOWPAN_CONF_COMPRESSION_HC1         1
#define SICSLOWPAN_CONF_COMPRESSION_HC01        2
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_COMPRESSION_HC06
#ifndef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG                    1
#define SICSLOWPAN_CONF_MAXAGE                  8
#endif /* SICSLOWPAN_CONF_FRAG */
#define SICSLOWPAN_CONF_CONVENTIONAL_MAC	1
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS       2
#ifndef SICSLOWPAN_CONF_MAX_MAC_TRANSMISSIONS
#define SICSLOWPAN_CONF_MAX_MAC_TRANSMISSIONS   5
#endif /* SICSLOWPAN_CONF_MAX_MAC_TRANSMISSIONS */
#else /* WITH_UIP6 */
#define UIP_CONF_IP_FORWARD      1
#define UIP_CONF_BUFFER_SIZE     108
#endif /* WITH_UIP6 */

#define UIP_CONF_ICMP_DEST_UNREACH 1

#define UIP_CONF_DHCP_LIGHT
#define UIP_CONF_LLH_LEN         0
#ifndef  UIP_CONF_RECEIVE_WINDOW
#define UIP_CONF_RECEIVE_WINDOW  48
#endif
#ifndef  UIP_CONF_TCP_MSS
#define UIP_CONF_TCP_MSS         48
#endif
#define UIP_CONF_MAX_CONNECTIONS 4
#define UIP_CONF_MAX_LISTENPORTS 8
#define UIP_CONF_UDP_CONNS       12
#define UIP_CONF_FWCACHE_SIZE    30
#define UIP_CONF_BROADCAST       1
#define UIP_ARCH_IPCHKSUM        1
#define UIP_CONF_UDP             1
#define UIP_CONF_UDP_CHECKSUMS   1
#define UIP_CONF_PINGADDRCONF    0
#define UIP_CONF_LOGGING         0

#define UIP_CONF_TCP_SPLIT       0



/* include the project config */
/* PROJECT_CONF_H might be defined in the project Makefile */
#ifdef PROJECT_CONF_H
#include PROJECT_CONF_H
#endif /* PROJECT_CONF_H */


#endif /* CONTIKI_CONF_H */
