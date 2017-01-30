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
    
    //Her we go baby! WDT deadman switch enabled!
    //CyWdtStart(CYWDT_1024_TICKS,CYWDT_LPMODE_DISABLED);
    
    //Get hardware setup
    hardware_init();
    
     
    
    for(;;)
    {
        DRV8301_STATUS dstat;
        uint32_t timeout_counter = 0;
        DebugLED_Write(1u);
        CyDelay(500);
        CyWdtClear();
        DebugLED_Write(0u);
        CyDelay(500);
        CyWdtClear();
        
    }
}

/* [] END OF FILE */
