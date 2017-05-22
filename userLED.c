/*******************************************************************************
 * filename: userLED.c
 *
 *
 * Written by: James Ross
 ******************************************************************************/

#include "userLED.h"

#define CLEAR_MASK 0xF

/* returns mapLen, length used in mmap */
devData_s *init_dev_map(int fd)
{
    struct pci_access *pacc;
    struct pci_dev *dev;
    devData_s *devData;

    devData = malloc(sizeof(devData));
    if(devData == NULL)
        errExit("init_dev_map: devData malloc failure.");

    pacc = pci_alloc(); /* alloc pci_access */
    pci_init(pacc);      /* default init */
    pci_scan_bus(pacc); /* populate device list */

    /* traverse device list until device is found or end of devices */
    for(dev = pacc->devices; dev != NULL; dev = dev->next){
        pci_fill_info(dev, PCI_FILL_IDENT|PCI_FILL_BASES|PCI_FILL_CLASS);

        if(dev->vendor_id == PCI_VEND_ID && dev->device_id == PCI_DEV_ID)
            break;
    }

    if(dev != NULL){
        devData->pciBar = dev->base_addr[MM_BAR0];
        devData->mapLen = dev->size[MM_BAR0];
        pci_cleanup(pacc); /* deallocate pci_access */
    }
    else 
        errExit("init_dev_map: vendor id: %x, device id: %x, not found");

    devData->mappedAddr = mmap(NULL, devData->mapLen, PROT_READ|PROT_WRITE, 
                               MAP_SHARED, fd, devData->pciBar);
    if(devData->mappedAddr == MAP_FAILED)
        errExit("init_dev_map: mmap failure, MAP_FAILED.");

    return devData;
}/* end init_dev_map */

void set_led(devData_s *devData, int ledNums, bool setTo)
{
    uint32_t toWrite;
    uint32_t setMode = (setTo == HIGH ? LED_MODE_ON : LED_MODE_OFF);

    if(devData == NULL)
    {
        noerr_msg("set_led: NULL devData, exiting function");
        return;
    }

    toWrite = READ_LED_REG(devData->mappedAddr);

    if(ledNums & RIGHT_GRN){
        toWrite &= ~(CLEAR_MASK << RIGHT_GRN_SHIFT); /* clear mode bits */
        toWrite |= setMode << RIGHT_GRN_SHIFT;
    }
    if(ledNums & LEFT_GRN){
        toWrite &= ~(CLEAR_MASK << LEFT_GRN_SHIFT); /* clear mode bits */
        toWrite |= setMode << LEFT_GRN_SHIFT;

    }
    if(ledNums & LEFT_AMBER){
        toWrite &= ~(CLEAR_MASK << LEFT_AMBER_SHIFT); /* clear mode bits */
        toWrite |= setMode << LEFT_AMBER_SHIFT;
    }

    WRITE_LED_REG(devData->mappedAddr, toWrite);
}/* end set_led */

void disable_all_leds(devData_s *devData)
{
    uint32_t toWrite = 0;

    if(devData == NULL)
    {
        noerr_msg("disable_all_leds: NULL devData, exiting function");
        return;
    }
   
    /* set all led mode bits to 0 */
    toWrite &= ~(CLEAR_MASK << RIGHT_GRN_SHIFT) |
               ~(CLEAR_MASK << LEFT_GRN_SHIFT)  |
               ~(CLEAR_MASK << LEFT_AMBER_SHIFT);
    
    /* set all led mode bits to off */
    toWrite |= (LED_MODE_OFF << RIGHT_GRN_SHIFT) |
               (LED_MODE_OFF << LEFT_GRN_SHIFT)  |
               (LED_MODE_OFF << LEFT_AMBER_SHIFT);
    WRITE_LED_REG(devData->mappedAddr, toWrite);
}/* end disable_all_leds */

void led_blink(devData_s *devData, int ledNum, int blinkRate)
{
    if(devData == NULL)
    {
        noerr_msg("disable_all_leds: NULL devData, exiting function");
        return;
    } 
    
    set_led(devData, ledNum, HIGH);
    sleep(blinkRate);
    set_led(devData, ledNum, LOW);
}/* end led_blink */
/******************** EOF *****************/
