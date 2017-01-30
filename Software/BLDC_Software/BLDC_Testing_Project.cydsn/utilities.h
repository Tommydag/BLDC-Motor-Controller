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
/*For PWM*/
#define PWM_DISABLE ~MainPWM_CTRL_ENABLE

/***************************************
*    Function Prototypes
***************************************/
uint8_t hardware_init(void);

#endif
/* [] END OF FILE */
