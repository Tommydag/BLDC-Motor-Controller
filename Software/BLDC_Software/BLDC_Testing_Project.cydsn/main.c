/* ========================================
 *
 * Engineer: Tom D'Agostino
 * Project: STA EVT BLDC Motor Controller
 * Date: 6 Jan 2017
 * Description: Main function for microcontroller
 * Language: C
 * Target Hardware: PSOC5LP
 * Version: 0_1
 *
 * ========================================
*/

#include "project.h"
#include "utilities.h"
#include "drv8301_utilities.h"

int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable;
    
    //Here we go baby! WDT deadman switch enabled!
    //CyWdtStart(CYWDT_1024_TICKS,CYWDT_LPMODE_DISABLED);
    
    //Get hardware setup
    hardware_init();
    
    uint8_t cstate = 0;
    int16 die_temp_degC = 0; 
    
    DebugLED_Write(1u); 
    uint32 tt = 0;
    CyDelay(1);
    //drv8301_read_status();
    //drv8301_write_status();
    for(;;)
    {
        
            uint8 status = update_throttle_val(UserIOStatus_Read());
            status |= check_vin_fault();
            updateIndicators(status);
            //CyWdtClear();
            CyDelayUs(10);
        
            //drv8301_read_status();
    }
}

/* [] END OF FILE */
