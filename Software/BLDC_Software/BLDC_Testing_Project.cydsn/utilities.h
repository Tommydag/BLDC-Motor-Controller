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

/***************************************
* Constants Definitions
***************************************/
/*For Gate Driver*/
#define EN_GATE_DRIVER          0x01

/*For Direction Register*/
#define DIR_FORWARD             0x00
#define DIR_BACKWARD            0x01
    
/*For LEDs*/
#define LED_ENABLE              0x01
    
/*For Vin Fault*/
#define NO_FAULT_DETECTED       0x00
#define FAULT_DETECTED          0x01
   
/*For Throttle*/
#define THROTTLE_ERROR_LOW      0xAF
#define THROTTLE_ERROR_HIGH     0xBF
#define SAR_HIGH_POWER          0x00     
#define NUM_OF_SAMPLES          32
#define SHIFTS_FOR_AVG          5
#define ADC_RES_BITMASK         0xFFF
#define MIN_ADC_COUNTS          (uint32)382u
#define MAX_ADC_COUNTS          (uint32)3800u
#define MAX_PWM_COUNTS          (uint32)2399u
#define SLEW_RATE_DELTA_COUNT   1
#define SLEW_RATE_SLOW          0xFFFF
#define SLEW_RATE_MEDIUM        SLEW_RATE_SLOW>>1
#define SLEW_RATE_FAST          SLEW_RATE_SLOW>>2
    
#define MOTOR_DISABLED          1
#define MOTOR_ENABLED           0  
#define IS_DISABLED(x)          x&(0x02)   
#define IS_REVERSED(x)          x&(0x01) 

/***************************************
*    Function Prototypes
***************************************/
uint8_t hardware_init(void);
uint8_t update_throttle_val(uint8 disable);
uint32 slew_rate_limit(uint32 input_throttle);
uint8 check_vin_fault(void);
void updateIndicators(uint8 status);
CY_ISR_PROTO(Vin_Fault);


#endif
/* [] END OF FILE */
