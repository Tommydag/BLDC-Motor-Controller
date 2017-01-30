/* ========================================
 *
 * Engineer: Tom D'Agostino
 * Project: STA EVT BLDC Motor Controller
 * Date: 7 Jan 2017
 * Description: Utility functions
 * Language: C
 * Target Hardware: PSOC5LP
 * Version: 0_1
 *
 * ========================================
*/

/***************************************
* Header Files
***************************************/
#include "utilities.h"
#include "drv8301_utilities.h"

/***************************************
* Forward Declarations
***************************************/

/*******************************************************************************
* Function Name: hardware_init
****************************************************************************//**
*
*******************************************************************************/
uint8_t hardware_init(void) {
    uint8_t status = CYRET_SUCCESS;
    
    //Make sure we have the motor off before we start
    CReg_Dir_Write(0);
    CReg_PWMOut_Write(0);
    
    //Now lets enable the different systems
    
    
    SPI_Start();
    
    drv8301_read_status();
    
    MotorPWM_Start();
   
    
    
    
    
    //Enable Gate Out
    CReg_PWMOut_Write(0xFF);
    
    return status;
}


/* [] END OF FILE */
