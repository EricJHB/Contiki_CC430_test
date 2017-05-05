/**
 * @Header   :  The cc430 shell example
 * @Author   :  ERIC
 * @Email    :  
 * @Date     :  2017/05/05
 * @Update   :
 * 
 * @Notice   :    the end of the command must be 0x0A
 */
#define WITH_CONTIKI_SHELL

#ifdef WITH_CONTIKI_SHELL

#include "contiki.h"
#include "shell.h"
#include "serial-shell.h"

PROCESS(cc430_shell_process, "cc430 Contiki Shell");

extern void shell_blink_init(void);

PROCESS_THREAD(cc430_shell_process, ev, data)
{
    PROCESS_BEGIN();

    serial_shell_init();//≥ı ºªØshell
	/* add the shell blink command */
    shell_blink_init();
    PROCESS_END();
}


#endif /* end of WITH_CONTIKI_SHELL */
