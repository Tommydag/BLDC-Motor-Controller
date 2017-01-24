/* ========================================
 *
 * Engineer: Tom D'Agostino
 * Project: STA EVT BLDC Motor Controller
 * Date: 7 Jan 2017
 * Description: Header file for utilities.c
 * Language: C
 * Target Hardware: PSOC5LP
 * Version: 0_1
 *
 * ========================================
*/

#ifndef UTILITIES_H
#define UTILITIES_H

/***************************************
* Header Files
***************************************/
#include "project.h"

/***************************************
* Data Struct Definitions
***************************************/
typedef struct DRV8301_STATUS_STRUCT
{
    uint8_t COM_ERROR,
            FAULT,
            GVDD_UV,
            PVDD_UV,
            OTSD,
            OTW,
            FETHA_OC,   
            FETLA_OC,
            FETHB_OC,
            FETLB_OC,
            FETHC_OC,
            FETLC_OC; 
}DRV8301_STATUS;

/***************************************
* Constants Definitions
***************************************/
/*For PWM*/
#define PWM_DISABLE ~MainPWM_CTRL_ENABLE

/*For SPI*/
#define SPI_TIMEOUT_VALUE               1000000

/*For DRV8301*/
#define DRV8301_STATUS_REG_1            0x0
#define DRV8301_STATUS_REG_2            0x1
#define DRV8301_CONTROL_REG_1           0x2
#define DRV8301_CONTROL_REG_2           0x3
#define DRV8301_WRITE                   0<<15
#define DRV8301_READ                    1<<15
#define DRV8301_NO_COM_ERROR            0
#define DRV8301_COM_ERROR_TXBUF_NEMPTY  1

/*For DRV8301 read register 1 bit positions*/
#define DRV8301_R1_FAULT                10
#define DRV8301_R1_GVDD_UV              9
#define DRV8301_R1_PVDD_UV              8
#define DRV8301_R1_OTSD                 7
#define DRV8301_R1_OTW                  6
#define DRV8301_R1_FETHA_OC             5
#define DRV8301_R1_FETLA_OC             4
#define DRV8301_R1_FETHB_OC             3
#define DRV8301_R1_FETLB_OC             2
#define DRV8301_R1_FETHC_OC             1
#define DRV8301_R1_FETLC_OC             0

/*For DRV8301 read register 2 bit positions*/
#define DRV8301_R2_GVDD_OV              7
#define DRV8301_R2_DEV_ID3              3
#define DRV8301_R2_DEV_ID2              2
#define DRV8301_R2_DEV_ID1              1
#define DRV8301_R2_DEV_ID0              0

#define DRV8301_READ_FAULT(x)       

/*For DRV8301 read register 2*/

/*For DRV8301 control register 1*/
#define DRV8301_GATEDRV_SHIFT           0x00
#define DRV8301_GATEDRV_0_25A           0x02<<DRV8301_GATEDRV_SHIFT
#define DRV8301_GATEDRV_0_7A            0x01<<DRV8301_GATEDRV_SHIFT
#define DRV8301_GATEDRV_1_7A            0x00<<DRV8301_GATEDRV_SHIFT

#define DRV8301_GATE_RST_SHIFT          0x2
#define DRV8301_GATE_RST_NORMAL         0<<DRV8301_GATE_RST_SHIFT 
#define DRV8301_GATE_RST_LATCHED        1<<DRV8301_GATE_RST_SHIFT 

#define DRV8301_PWM_SHIFT               0x3
#define DRV8301_PWM_6                   0<<DRV8301_PWM_SHIFT 
#define DRV8301_PWM_3                   1<<DRV8301_PWM_SHIFT 

#define DRV8301_OCP_SHIFT               0x4
#define DRV8301_OCP_LIMIT               0<<DRV8301_OCP_SHIFT 
#define DRV8301_OCP_OC_LATCH_SD         1<<DRV8301_OCP_SHIFT 
#define DRV8301_OCP_REPORT_ONLY         2<<DRV8301_OCP_SHIFT
#define DRV8301_OCP_DISABLE             3<<DRV8301_OCP_SHIFT

#define DRV8301_OC_ADJ_SHIFT            0x6

/*For DRV8301 Control register 2*/

/***************************************
*    Function Prototypes
***************************************/
uint8_t hardware_init(void);
DRV8301_STATUS drv8301_read_status(void);
uint8_t drv8301_register_parse(uint16_t reg_copy, uint8_t reg_bit);

#endif
/* [] END OF FILE */
