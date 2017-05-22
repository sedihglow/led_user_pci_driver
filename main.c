/*******************************************************************************
 * filename: main.c
 *
 *
 * Written by: James Ross
 ******************************************************************************/

#include "userLED.h"

#define IO_MEM_DRIVER "/dev/mem"
#define NUM_ROT_BLINK 5 /* number of times to blink led's in rotation */

int main(void)
{
    devData_s *devData;
    uint32_t initConfig;
    int fd;
    int i;

    
    fd = open(IO_MEM_DRIVER, O_RDWR | O_SYNC);
    if(fd == FAILURE)
        errExit("main: IO_MEM_DRIVER failed to open.");

    devData = init_dev_map(fd);

    /* get init config */
    initConfig = READ_LED_REG(devData->mappedAddr);
    printf("Initial configuration of led ctrl reg: 0x%x\n", initConfig);

    /* clear the led register */
    CLEAR_LED_REG(devData->mappedAddr);

    /* turn all LED's off */
    disable_all_leds(devData);

    /* print good packets recieved */
    printf("Good Packets Recieved: %u\n", GOOD_PACK_CNT(devData->mappedAddr));
    
    /**************************************************************************
     * TODO: Reading the Good Packet Count register after this point will
     *       always result in a read of 0, any point before this it will give
     *       a value. Why? What changes after blinking the LED
     **************************************************************************/

    /* turn both green LED's on for 2 seconds */
    led_blink(devData, RIGHT_GRN|LEFT_GRN, 2);
    sleep(2); /* stay de-asserted for 2 more seconds */


    /* turn each LED on for 1 second, x times */
    for(i = 0; i < NUM_ROT_BLINK; ++i){
        led_blink(devData, LEFT_AMBER, 1);
        led_blink(devData, RIGHT_GRN, 1);
        led_blink(devData, LEFT_GRN, 1);
    }

    /* set init config */
    WRITE_LED_REG(devData->mappedAddr, initConfig);

    if(munmap(devData->mappedAddr, devData->mapLen) == FAILURE)
        errExit("main: failed to unmap");
    free(devData);
    close(fd);
    exit(EXIT_SUCCESS);
}/* end main */
/******************** EOF *****************/
