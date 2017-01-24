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


/***************************************
* Forward Declarations
***************************************/
DRV8301_STATUS drv8301_status;

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
    
    //SPI_WriteTxData(
    
    
    MainPWM_Start();
   
    
    
    
    
    //Enable Gate Out
    CReg_PWMOut_Write(0xFF);
    
    return status;
}

/*******************************************************************************
* Function Name: drv8301_read_status
****************************************************************************//**
*
*******************************************************************************/
DRV8301_STATUS drv8301_read_status(void){
    DRV8301_STATUS dstat;
    uint32_t timeout_counter = 0;
    
    //Check to see if the SPI tx buffer has data that was not transmitted
    if(SPI_ReadTxStatus()&SPI_STS_TX_FIFO_NOT_FULL){
        dstat.COM_ERROR = DRV8301_COM_ERROR_TXBUF_NEMPTY;
    }
    else{
        //Form packet that corrisponds to a data request from status reg 1
        uint16_t packets_to_tx[3] = {   DRV8301_READ|DRV8301_STATUS_REG_1|0x0000,
                                        DRV8301_READ|DRV8301_STATUS_REG_2|0x0000,
                                        DRV8301_READ|DRV8301_STATUS_REG_1|0x0000    };
        //Send the packet, blocking call
        SPI_PutArray(packets_to_tx,3);
        
        
        while( !(SPI_ReadTxStatus()&SPI_STS_SPI_DONE) ){
            if(timeout_counter==SPI_TIMEOUT_VALUE){
                timeout_counter++;
            }
            else{
               // dstat.COM_ERROR = bad;
                break;
            }
        }
        
    }
    
    
    
    
    
    
    dstat.COM_ERROR = 0;
    dstat.FAULT=0;
    dstat.GVDD_UV=0;
    dstat.PVDD_UV=0;
    dstat.OTSD=0;
    dstat.OTW=0;
    dstat.FETHA_OC=0;  
    dstat.FETLA_OC=0;
    dstat.FETHB_OC=0;
    dstat.FETLB_OC=0;
    dstat.FETHC_OC=0;
    dstat.FETLC_OC=0;
    
    return dstat;
}

/*******************************************************************************
* Function Name: drv8301_register_parse
****************************************************************************//**
*
*******************************************************************************/
uint8_t drv8301_register_parse(uint16_t reg_copy, uint8_t reg_bit){
    return (uint8_t)0x0001&(reg_copy>>reg_bit);
}


/* [] END OF FILE */
