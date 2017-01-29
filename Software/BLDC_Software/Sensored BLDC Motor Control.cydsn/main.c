/*******************************************************************************
*
* Filename:             main.c
* Owner :               Rod Wang (rowa@cypress.com)
*
* Version:              V1.0 
* Description:          BLDC Motor Control. 
*                       Support 3 phase BLDC motor control with Hall sensor Input. 
*                       Speed is adjustable.
* -------------------------------------------------------------------------------
* ChangeList: 
*   V1.0                Initial version
* -------------------------------------------------------------------------------
* Known issues:         
*   V1.0                N/A
* -------------------------------------------------------------------------------
* Hardare Dependency:   
*   1. CY8CKIT-042(MCU board) + CY8CKIT-037(Driver Board)  
* -------------------------------------------------------------------------------
* Related documents:
*   N/A
* -------------------------------------------------------------------------------
* Code Tested with:     PSoC Creator  3.0 SP2 (3.0.0.3140)  
*                       ARM GCC 4.7.3;
*
********************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is 
* protected by and subject to worldwide patent protection (United States and 
* foreign), United States copyright laws and international treaty provisions. 
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable 
* license to copy, use, modify, create derivative works of, and compile the 
* Cypress Source Code and derivative works for the sole purpose of creating 
* custom software in support of licensee product to be used only in conjunction 
* with a Cypress integrated circuit as specified in the applicable agreement. 
* Any reproduction, modification, translation, compilation, or representation of 
* this software except as specified above is prohibited without the express 
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. Use may be 
* limited by and subject to the applicable Cypress software license agreement. 
*******************************************************************************/
#include <project.h>
#include "motor.h"
#include "speed.h"
#include "userinterface.h"
#include "getvalue.h"


void main()
{  
    uint8 HallReader =0;    
	uint16 pwmCntLocal = 0;
    
    /* initialize parameters in UI FW */
    Init_UI_FW();    
    /* disable global interrupt */ 
	CyGlobalIntDisable;	
    /* initialize system hardware components */	
    Init_HW();   	
    /*Enable Global interrupt*/
	CyGlobalIntEnable;    
    /* initialize UI hardware components */ 
	Init_UI_HW();
    
    /* stop motor as default */
	UI_Cmd.run =  FALSE;     
    /* Set Motor Direction */
    /* CLOCK_WISE = 0x01, and COUNTER_CLOCK_WISE = 0x00 */  
    CtrlReg_Dir_Write(UI_Data.Dir);
    
    for(;;)
    {
        /* Scan sensors and handle event */
	    ButtonProcess();
		
        /* Measure bus voltage */
        VoltageCheck();	
		
        /*Stop motor by disable PWM output */
		if(UI_Cmd.run == FALSE)
        {
			/* Turning off LED when motor stopping */
			STATUS_LED_Write(1);			
        	CtrlReg_PWMOut_Write(0x00);  
			PWM_Drive_WriteCompare(PWM_Drive_ReadPeriod()>>16);
            
        	piOut = INIT_PIOUT;
        	HallReader = TRUE;
        } 
		
		/* Error Protection: Disable PWM and shining LED at 1Hz */
		if(errorState != 0)
		{
			CtrlReg_PWMOut_Write(0x00);
            
			STATUS_LED_Write(~(STATUS_LED_Read()));
			CyDelay(500);
			STATUS_LED_Write(~(STATUS_LED_Read()));
			CyDelay(500);
		}		
        
		/* Motor Running */
        if(UI_Cmd.run == TRUE && errorState == 0)
        {
			/* Lighting LED when motor running */
			STATUS_LED_Write(0);
			
            /* only send out UART data when motor is running */
		    BCPPoll();
            
            CtrlReg_PWMOut_Write(0x03);  
            
            pwmCntLocal = pwmCnt;
            
            /* Update every 12.5mS*/
           	if((pwmCntLocal & 0xff) == 0xff) 
    	    {
				pwmCntLocal++;
                
                /* Calculate motor speed reference, unit is RPM*/
				UI_Data.speedRpmRef = ReadRpmRef();   
								
                speedRef = (60*FREQ_CAPTURE_CLK)/(uint32)(MOTOR_POLE_PAIRS*UI_Data.speedRpmRef);
    			
                if(UI_Cmd.run == TRUE)
                {
                /*Speed close loop control */            
                	SpeedPID();  
                }	 
    			
                /* Hall Error Detection*/
                HallReader  = Hall_Error_Read();
                if(HallReader == TRUE)
                {               
    				errorState = hallError;
                	UpdateStatusError();
                }			
    		}
            /* Calculate the real time motor speed every 2000 PWM period, unit: RPM*/
            if(pwmCntLocal >= 2000) 
            {
                uint32 tmp1 = 0;
                uint32 tmp2 = 0;
				pwmCntLocal = 0;
                tmp1 = (60*FREQ_CAPTURE_CLK);
                tmp2 = (MOTOR_POLE_PAIRS*speedCur);            
                UI_Data.speedRpm = (tmp1/tmp2);      
            }
            
            pwmCnt = pwmCntLocal;
        }	
    }
}

/* [] END OF FILE */
