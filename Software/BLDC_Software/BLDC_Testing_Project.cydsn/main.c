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

int main(void)
{
    
    hardware_init();
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    for(;;)
    {
        DebugLED_Write(1u);
        CyDelay(500);
        DebugLED_Write(0u);
        CyDelay(500);
    }
}

/* [] END OF FILE */
