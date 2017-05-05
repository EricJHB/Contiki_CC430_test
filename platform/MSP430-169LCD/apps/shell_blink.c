/*
 * @Header   :  shell blink
 * @Author   :  ERIC
 * @Email    :  
 * @Date     :  2017/02/04
 * @Update   :
 *
 */

#include "contiki.h"
#include "shell.h"

#include "stdio.h"
#include "string.h"
#include "leds.h"


PROCESS(shell_blink_process, "shell blink");
SHELL_COMMAND(blink_command, "blink",
              "blink on/off : led on/off", &shell_blink_process);
							
static void
led_init()
{
}

/**
 * \brief
 * \param
 *
 */
static void
led_on()
{
  leds_on(LEDS_GREEN);
}

static void
led_off()
{  
  leds_off(LEDS_GREEN);
}


PROCESS_THREAD(shell_blink_process, ev, data)
{
    PROCESS_BEGIN();
    if(data != NULL)
    {
        if(!strcmp(data, "on"))
        {
            led_on();
            printf("led on!\r\n");
        }
        else if(!strcmp(data, "off"))
        {
            led_off();
            printf("led off!\r\n");
        }
        else
        {
            printf("Invalid param!\r\n");
        }
    }

    PROCESS_END();
}

void
shell_blink_init(void)
{
    shell_register_command(&blink_command);
}

