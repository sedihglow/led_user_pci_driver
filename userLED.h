/*******************************************************************************
 * filename: userLED.h
 *
 *
 * Written by: James Ross
 ******************************************************************************/

#ifndef _USERLED_H
#define _USERLED_H

#include "utility_sys.h"

/* device identity */
#define PCI_DEV_ID    0x1501     /* 82567V-3 gbe */
#define PCI_VEND_ID   0x8086     /* intel */

/* register offsets*/
#define LED_CTRL_OFFSET  0xE00  /* LEDCTL */
#define GPR_STAT_OFFSET  0x4074
#define REC_CTRL_OFFSET  0x100  /* RCTL */

/* RCTL enable bit */
#define REC_EN 0x2

/* led ctrl mode values */
#define LED_MODE_ON      0xE   /* always asserted */
#define LED_MODE_OFF     0xF   /* always de-asserted */

/* led ctrl led numbers */
#define RIGHT_GRN        0x1   /* right green is LED0 */
#define LEFT_GRN         0x2   /* left green is LED1 */
#define LEFT_AMBER       0x4   /* amber led is LED2 */

/* led mode locations 
 * example: regVal |= (LED_MODE_ON << LEFT_AMBER_SHIFT) */
#define RIGHT_GRN_SHIFT  0  /* LED0 */
#define LEFT_GRN_SHIFT   8  /* LED1 */
#define LEFT_AMBER_SHIFT 16 /* LED2 */

/* memory definitions */
#define MM_BAR0  0 /* memory map IO */

                        /* macros */
#define HWREG32(addr) (*((volatile uint32_t*)(addr)))
#define READ_LED_REG(bar) (HWREG32((bar)+LED_CTRL_OFFSET))
#define WRITE_LED_REG(bar, writeVal) (HWREG32((bar)+LED_CTRL_OFFSET)=(writeVal))
#define CLEAR_LED_REG(bar) (WRITE_LED_REG(bar, 0x0))
#define GOOD_PACK_CNT(bar) (HWREG32((bar)+GPR_STAT_OFFSET))
#define EN_RCTL(bar) (HWREG32((bar)+REC_CTRL_OFFSET) |= REC_EN)

typedef struct user_dev_data{
    size_t mapLen;
    pciaddr_t pciBar;
    void *mappedAddr;
}devData_s;

devData_s* init_dev_map(int fd);
void disable_all_leds(devData_s *devData);
void set_led(devData_s *devData, int ledNum, bool setTo);
void led_blink(devData_s *devData, int ledNum, int blinkRate);
#endif
/******************** EOF *****************/
