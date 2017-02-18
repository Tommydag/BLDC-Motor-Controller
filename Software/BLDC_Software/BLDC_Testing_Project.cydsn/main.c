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
//#include "drv8301_utilities.h"

int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable;
    
    //Here we go baby! WDT deadman switch enabled!
    CyWdtStart(CYWDT_1024_TICKS,CYWDT_LPMODE_DISABLED);
    
    //Get hardware setup
    hardware_init();
    
    uint8_t cstate = 0;
    
    RedLedBrightness_WritePulse0(0x010F);
    BlueLedBrightness_WritePulse0(0x010F);
    GreenLedBrightness_WritePulse0(0x010F);
    DebugLED_Write(1u);
    for(;;)
    {
        uint8 status = update_throttle_val(0);
        updateIndicators(status);
        
        check_vin_fault();
        CyDelayUs(1);
        CyWdtClear();
    }
}

/* [] END OF FILE */
