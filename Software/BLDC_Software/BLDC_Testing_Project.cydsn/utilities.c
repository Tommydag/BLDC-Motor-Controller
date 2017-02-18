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
uint32 throttle_values[NUM_OF_SAMPLES] = {0};
uint8 throttle_array_index = 0;
volatile uint8 Vin_Fault_Change_Flag = 0;

/*******************************************************************************
* Function Name: hardware_init
****************************************************************************//**
*
*******************************************************************************/
uint8_t hardware_init(void) {
    uint8_t status = CYRET_SUCCESS;
    
    //Make sure we have the motor off before we start
    CReg_Dir_Write(DIR_FORWARD);
    CReg_DriveMotor_Write(~EN_GATE_DRIVER);
    
    SlewRateTimer_Start();
    SlewRateTimer_WritePeriod(SLEW_RATE_MEDIUM);
    
    //Now lets enable the different systems
    ThrottleBuffer_Start();
    ThrottleSAR_Start();
    ThrottleSAR_SetPower(SAR_HIGH_POWER);
    ThrottleSAR_IRQ_Disable();
    ThrottleSAR_StartConvert();
    
    SPI_Start();
    
    //Vin_Fault_ISR_StartEx(Vin_Fault);
    
    RedLedBrightness_Start();
    GreenLedBrightness_Start();
    BlueLedBrightness_Start();
    CReg_LED_Write(LED_ENABLE);
    
    drv8301_read_status();
    
    MotorPWM_Start();
    
    //Enable Gate Out
    CReg_DriveMotor_Write(EN_GATE_DRIVER);
    
    return status;
}

/*******************************************************************************
* Function Name: update_throttle_val
********************************************************************************
*
*******************************************************************************/
uint8_t update_throttle_val(uint8 disable) {
    uint8_t status = CYRET_SUCCESS;
    
    //Check if the motor is supposed to be disabled
    if(~disable){
        //Collect sample and index samples array
        if(throttle_array_index<NUM_OF_SAMPLES){
            throttle_values[throttle_array_index] = CY_GET_REG16(ThrottleSAR_SAR_WRK0_PTR);
            throttle_array_index++;
        }
        else{
            throttle_values[0] = CY_GET_REG16(ThrottleSAR_SAR_WRK0_PTR);
            throttle_array_index = 1;
        }
        
        //Only update the throttle every 4th collected sample
        if( (throttle_array_index%4)==0){
            
            //Compute the sum of of the last NUM_OF_SAMPLES
            uint32 running_sum4_throttle = 0u;
            uint8 sum_index = 0u;
            for(;sum_index<NUM_OF_SAMPLES;sum_index++){
                running_sum4_throttle += throttle_values[sum_index];
            }
            
            //Lets not compute the average until all other calculations are complete
            uint32 average_throttle = (running_sum4_throttle);
            
            //Check throttle values for errors
            if(average_throttle<(MIN_ADC_COUNTS<<SHIFTS_FOR_AVG)){
                average_throttle = 0;
                status = THROTTLE_ERROR_LOW;
            }
            else if(average_throttle>(MAX_ADC_COUNTS<<SHIFTS_FOR_AVG)){
                average_throttle = 0;
                status = THROTTLE_ERROR_HIGH;
            }
            else{
                average_throttle -= (MIN_ADC_COUNTS<<SHIFTS_FOR_AVG);   
                average_throttle *= (MAX_PWM_COUNTS);
                average_throttle /= MAX_ADC_COUNTS;
                average_throttle >>= SHIFTS_FOR_AVG;
            }
            
            //Limit the slew rate
            average_throttle = slew_rate_limit(average_throttle);
            
            //Set motor throttle
            if(average_throttle<=MAX_PWM_COUNTS){
                MotorPWM_WriteCompare((uint16)average_throttle); 
            }
            else{
                status = THROTTLE_ERROR_HIGH;
            }
        }
    }
    //We were told to disable the motor, so let's do this gracefully
    else{
        //Limit the slew rate
        uint32 disabled_throttle = slew_rate_limit(0);
        
        //Set motor throttle
        if(disabled_throttle<=MAX_PWM_COUNTS){
            MotorPWM_WriteCompare((uint16)disabled_throttle); 
        }
        else{
            status = THROTTLE_ERROR_HIGH;
        }
    }
    
    return status;
}
/*******************************************************************************
* Function Name: vin_fault_pwr_save
********************************************************************************
*
*******************************************************************************/
uint32 slew_rate_limit(uint32 input_throttle){
    
    uint32 current_throttle_val = (uint32)MotorPWM_ReadCompare();
    uint32 slew_rate_limited_throttle = current_throttle_val;
    if(SlewRateTimer_ReadStatusRegister()&SlewRateTimer_STATUS_TC){
            SlewRateTimer_WriteControlRegister(SlewRateTimer_CTRL_ENABLE);
            if(input_throttle>=current_throttle_val){
            if((input_throttle-current_throttle_val)>SLEW_RATE_DELTA_COUNT){
                slew_rate_limited_throttle = current_throttle_val+SLEW_RATE_DELTA_COUNT;
            }
            else{
                slew_rate_limited_throttle = input_throttle;
            }
        }
        else{
            if((current_throttle_val-input_throttle)>SLEW_RATE_DELTA_COUNT){
                slew_rate_limited_throttle = current_throttle_val-SLEW_RATE_DELTA_COUNT;
            }
            else{
                slew_rate_limited_throttle = input_throttle;
            }
        }   
    }
    return slew_rate_limited_throttle;
    
}
/*******************************************************************************
* Function Name: vin_fault_pwr_save
********************************************************************************
*
*******************************************************************************/
uint8 check_vin_fault(void){
    
    
    if(Vin_Fault_Change_Flag){
        uint8 status;
        //This case means that a fault has just begun,
        //  so disable motor and save power.
        if(CStat_Vin_Fault_Read()){
            //CReg_DriveMotor_Write(~EN_GATE_DRIVER);
            CReg_LED_Write(~LED_ENABLE);
            
            RedLedBrightness_Sleep();
            GreenLedBrightness_Sleep();
            BlueLedBrightness_Sleep();
            //MotorPWM_Sleep();
            return FAULT_DETECTED;
        }
        //This case means that the fault has cleared,
        //  resume operation
        else{
            RedLedBrightness_Wakeup();
            GreenLedBrightness_Wakeup();
            BlueLedBrightness_Wakeup();
            //MotorPWM_Wakeup();
            
            //CReg_DriveMotor_Write(EN_GATE_DRIVER);
            CReg_LED_Write(LED_ENABLE);
            
            return NO_FAULT_DETECTED;     
        }     
    }else{
        return NO_FAULT_DETECTED;   
    }
    
}

/*******************************************************************************
* ISR Name: vin_fault
********************************************************************************
*
*******************************************************************************/
CY_ISR(Vin_Fault){
    Vin_Fault_Change_Flag = 1; 
}

/*******************************************************************************
* Function Name: updateIndicators
********************************************************************************
*
*******************************************************************************/
void updateIndicators(uint8 status){
    if(status==CYRET_SUCCESS){
        GreenLedBrightness_WritePulse0(0x00FF);
        BlueLedBrightness_WritePulse0(0x0000);
        RedLedBrightness_WritePulse0(0x0000);
    }
    else if(status==THROTTLE_ERROR_HIGH){
        BlueLedBrightness_WritePulse0(0x00FF);
        GreenLedBrightness_WritePulse0(0x0000);
        RedLedBrightness_WritePulse0(0x0000);
    }
    else{
        RedLedBrightness_WritePulse0(0x00FF);
        GreenLedBrightness_WritePulse0(0x0000);
        BlueLedBrightness_WritePulse0(0x0000);
    }
}
/* [] END OF FILE */
