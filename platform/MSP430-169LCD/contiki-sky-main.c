/*
 * Contiki-2.5 test
 * for CC430F5137
 * ERIC
 * date: 2017/04/25
 */

#include <stdio.h>
//#include <string.h>

#include <signal.h>

#include "contiki.h"
//fputc  putchar stdio autoinit_wdt
//#include "dev/ds2411.h"
#include "dev/leds.h"
#include "dev/serial-line.h"
//#include "dev/slip.h"
#include "dev/uart0.h"
#include "dev/watchdog.h"
//#include "dev/xmem.h"
#include "lib/random.h"
//#include "net/netstack.h"

#if WITH_UIP6
#include "net/uip-ds6.h"
#endif /* WITH_UIP6 */

//#include "net/rime.h"

#include "node-id.h"
#include "cfs-coffee-arch.h"
//#include "cfs/cfs-coffee.h"
#include "sys/autostart.h"
//#include "sys/profile.h"

#if DCOSYNCH_CONF_ENABLED
static struct timer mgt_timer;
#endif
extern int msp430_dco_required;

#ifndef WITH_UIP
#define WITH_UIP 0
#endif

#if WITH_UIP
#include "net/uip.h"
#include "net/uip-fw.h"
#include "net/uip-fw-drv.h"
#include "net/uip-over-mesh.h"
static struct uip_fw_netif slipif =
  {UIP_FW_NETIF(192,168,1,2, 255,255,255,255, slip_send)};
static struct uip_fw_netif meshif =
  {UIP_FW_NETIF(172,16,0,0, 255,255,0,0, uip_over_mesh_send)};

#endif /* WITH_UIP */

#define UIP_OVER_MESH_CHANNEL 8
#if WITH_UIP
static uint8_t is_gateway;
#endif /* WITH_UIP */

#ifdef EXPERIMENT_SETUP
#include "experiment-setup.h"
#endif

void init_platform(void);
/*----------------------------PROCESS_NAME-----------------------------------*/
PROCESS_NAME(blink_process);
PROCESS_NAME(fader_process);
PROCESS_NAME(burn_process);
PROCESS_NAME(cc430_shell_process);
/*---------------------------------------------------------------------------*/
#if 0
int
force_float_inclusion()
{
  extern int __fixsfsi;
  extern int __floatsisf;
  extern int __mulsf3;
  extern int __subsf3;

  return __fixsfsi + __floatsisf + __mulsf3 + __subsf3;
}
#endif
/*---------------------------------------------------------------------------*/
void uip_log(char *msg) { puts(msg); }
/*---------------------------------------------------------------------------*/
#ifndef RF_CHANNEL
#define RF_CHANNEL              26
#endif
/*---------------------------------------------------------------------------*/
#if 1
void
force_inclusion(int d1, int d2)
{
  snprintf(NULL, 0, "%d", d1 % d2);
}
#endif
/*---------------------------------------------------------------------------
static void
set_rime_addr(void)
{
  rimeaddr_t addr;
  int i;

  memset(&addr, 0, sizeof(rimeaddr_t));
#if UIP_CONF_IPV6
  memcpy(addr.u8, ds2411_id, sizeof(addr.u8));
#else
  if(node_id == 0) {
    for(i = 0; i < sizeof(rimeaddr_t); ++i) {
      addr.u8[i] = ds2411_id[7 - i];
    }
  } else {
    addr.u8[0] = node_id & 0xff;
    addr.u8[1] = node_id >> 8;
  }
#endif
  rimeaddr_set_node_addr(&addr);
  printf("Rime started with address ");
  for(i = 0; i < sizeof(addr.u8) - 1; i++) {
    printf("%d.", addr.u8[i]);
  }
  printf("%d\n", addr.u8[i]);
}
*/

static void
print_processes(struct process * const processes[])
{
  /*  const struct process * const * p = processes;*/
  printf("Starting");
  while(*processes != NULL) {
    printf(" '%s'", (*processes)->name);
    processes++;
  }
  putchar('\n');
}

AUTOSTART_PROCESSES(&cc430_shell_process);
int
main(int argc, char **argv)
{
  /*
   * Initalize hardware.
   */
  msp430_cpu_init();
  clock_init();
  leds_init();
  leds_on(LEDS_RED);


  /*usart 初始化*/
  uart0_init(BAUD2UBR(115200)); /* Must come before first printf */


  //leds_on(LEDS_GREEN);
  //ds2411_init();

  /* XXX hack: Fix it so that the 802.15.4 MAC address is compatible
     with an Ethernet MAC address - byte 0 (byte 2 in the DS ID)
     cannot be odd. */
 // ds2411_id[2] &= 0xfe;

  //leds_on(LEDS_BLUE);
  //xmem_init();

  leds_off(LEDS_RED);
  rtimer_init();
  /*
   * Hardware initialization done!
   */
  leds_off(LEDS_BLUE);
  /*
   * Initialize Contiki and our processes.
   */
  process_init();
  process_start(&etimer_process, NULL);

  ctimer_init();

  /*serial_line初始化*/
  uart0_set_input(serial_line_input_byte);//
  serial_line_init();
  
  //init_platform();
 // process_start(&blink_process, NULL);
  /*等待初始化完成*/
  clock_delay(20000);
  printf(CONTIKI_VERSION_STRING " started. ");

#if !WITH_UIP && !WITH_UIP6
  //uart1_set_input(serial_line_input_byte);
  //serial_line_init();
#endif

  leds_off(LEDS_GREEN);

  energest_init();
  ENERGEST_ON(ENERGEST_TYPE_CPU);

  watchdog_start();

  print_processes(autostart_processes);
  autostart_start(autostart_processes);

  
  /*  watchdog_stop();*/
  while(1) {
    int r;

    do {
      /* Reset watchdog. */
      watchdog_periodic();
      r = process_run();
    } while(r > 0);

    /*
     * Idle processing.
     */
    int s = splhigh();		/* Disable interrupts. */
    /* uart1_active is for avoiding LPM3 when still sending or receiving */
    if(process_nevents() != 0 /*|| uart1_active()*/) {
      splx(s);			/* Re-enable interrupts. */
    } else {
      static unsigned long irq_energest = 0;

      
      /* Re-enable interrupts and go to sleep atomically. */
      ENERGEST_OFF(ENERGEST_TYPE_CPU);
      ENERGEST_ON(ENERGEST_TYPE_LPM);
      /* We only want to measure the processing done in IRQs when we
	 are asleep, so we discard the processing time done when we
	 were awake. */
      energest_type_set(ENERGEST_TYPE_IRQ, irq_energest);
      watchdog_stop();
      /* check if the DCO needs to be on - if so - only LPM 1 */
      if (msp430_dco_required) {
	_BIS_SR(GIE | CPUOFF); /* LPM1 sleep for DMA to work!. */
      } else {
	_BIS_SR(GIE | SCG0 | SCG1 | CPUOFF); /* LPM3 sleep. This
						statement will block
						until the CPU is
						woken up by an
						interrupt that sets
						the wake up flag. */
      }
      /* We get the current processing time for interrupts that was
	 done during the LPM and store it for next time around.  */
      dint();
      irq_energest = energest_type_time(ENERGEST_TYPE_IRQ);
      eint();
      watchdog_start();
      ENERGEST_OFF(ENERGEST_TYPE_LPM);
      ENERGEST_ON(ENERGEST_TYPE_CPU);
    }
  }

  //return 0;

}
/*---------------------------------------------------------------------------*/
#if LOG_CONF_ENABLED
void
log_message(char *m1, char *m2)
{
  printf("%s%s\n", m1, m2);
}
#endif /* LOG_CONF_ENABLED */
