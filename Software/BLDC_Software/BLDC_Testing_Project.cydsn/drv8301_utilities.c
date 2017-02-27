/* ========================================
 *
 * Engineer: Tom D'Agostino
 * Project: STA EVT BLDC Motor Controller
 * Date: 29 Jan 2017
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
#include "drv8301_utilities.h"

/***************************************
* Forward Declarations
***************************************/
DRV8301_STATUS  drv8301_status;
DRV8301_CONTROL drv8301_control;

/*******************************************************************************
* Function Name: drv8301_read_status
****************************************************************************//**
*
*******************************************************************************/
DRV8301_STATUS drv8301_read_status(void){
    DRV8301_STATUS dstat;
    dstat.COM_ERROR = DRV8301_NO_COM_ERROR;
    uint32_t timeout_counter = 0;
    
    //Check to see if the SPI tx buffer has data that was not transmitted
    if( !(SPI_ReadTxStatus()&SPI_STS_SPI_IDLE) ){
        dstat.COM_ERROR = DRV8301_COM_ERROR_TXBUF_NEMPTY;
    }
    else{
        //Form packet that corrisponds to a data request from status reg 1 and 2
        uint16_t packets_to_tx[2] = {   DRV8301_READ|DRV8301_CONTROL_REG_1|0x0000,
                                        DRV8301_READ|DRV8301_CONTROL_REG_1|0x0000};
        
        //Send the packet, not a blocking call if data is under 4 packets
        CyDelayUs(300);
        SPI_WriteTxData(packets_to_tx[0]);
        uint16 stuff[10];
        uint8 add = 0;
        uint8 address = 0;
        CyDelayUs(300);
        SPI_WriteTxData(packets_to_tx[1]);
//        while( !(address>1)){
//            if(timeout_counter>=SPI_TIMEOUT_VALUE){
//                dstat.COM_ERROR = DRV8301_COM_ERROR_TIMEOUT;
//                break;
//            }
//            else{
//                uint8 tx_status = SPI_ReadTxStatus();
//                if((SPI_STS_SPI_IDLE&tx_status)&&(SPI_STS_TX_FIFO_EMPTY&tx_status)){
//                    for(timeout_counter = 0;timeout_counter<255;timeout_counter++){
//                        if(SPI_ReadRxStatus()&SPI_STS_RX_FIFO_NOT_EMPTY){
//                            stuff[address] = SPI_ReadRxData();
//                            break;
//                        }    
//                    }
//                    CyDelayUs(300);
//                    SPI_WriteTxData(packets_to_tx[address]);
//                    address++;
//                    timeout_counter = 0;
//                }
//                else{
//                    timeout_counter++;
//                }
//            }
//        }
        
        /*If we did not hit any errors, continue with execution*/
        uint16_t drv8301_stat_0 ;
        uint16_t drv8301_stat_1 ;
        uint16_t drv8301_ctrl_1 ;
        uint16_t drv8301_ctrl_2;
        uint16_t drv8301_ctrl_3;
        
        if(dstat.COM_ERROR == DRV8301_NO_COM_ERROR){
            /*Check to make sure we recieved data back on spi bus*/
            if(SPI_GetRxBufferSize()>0){
                drv8301_stat_0 = SPI_ReadRxData();
                drv8301_stat_1 = SPI_ReadRxData();
                drv8301_ctrl_1 = SPI_ReadRxData();
                drv8301_ctrl_2 = SPI_ReadRxData();
                drv8301_ctrl_3 = SPI_ReadRxData();
                uint16_t fun = 0;
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
* Function Name: drv8301_write_control
****************************************************************************//**
*
*******************************************************************************/
uint8 drv8301_write_status(void){
    uint8 status = CYRET_SUCCESS;
    uint32_t timeout_counter = 0;
    DRV8301_CONTROL drv8301_ctrl;
    drv8301_ctrl.GATE_CURRENT= 0u;
    drv8301_ctrl.GATE_RESET = 0;
    drv8301_ctrl.PWM_MODE = 0;
    drv8301_ctrl.OCP_MODE = 0x02;
    drv8301_ctrl.OC_ADJ_SET = 0x1F;
    drv8301_ctrl.OCTW_MODE = 0;
    drv8301_ctrl.GAIN = 0x01;
    drv8301_ctrl.DC_CAL_CH1= 0x01;
    drv8301_ctrl.DC_CAL_CH2= 0x01;
    drv8301_ctrl.OC_TOFF = 0;
    
    if( !(SPI_ReadTxStatus()&SPI_STS_SPI_IDLE) ){
       status = DRV8301_COM_ERROR_TXBUF_NEMPTY;
    }
    else{
        //Form packet that corrisponds to a data request from status reg 1 and 2
        uint16_t packets_to_tx[3];
        packets_to_tx[0] = (DRV8301_WRITE|
                            DRV8301_CONTROL_REG_1| 
                            drv8301_ctrl.GATE_CURRENT<<0|
                            drv8301_ctrl.GATE_RESET<<2|
                            drv8301_ctrl.PWM_MODE<<3|
                            drv8301_ctrl.OCP_MODE<<4|
                            drv8301_ctrl.OC_ADJ_SET<<6);
        packets_to_tx[1] = (DRV8301_WRITE|
                            DRV8301_CONTROL_REG_2| 
                            drv8301_ctrl.OCTW_MODE<<0|
                            drv8301_ctrl.GAIN<<2|
                            drv8301_ctrl.DC_CAL_CH1<<4|
                            drv8301_ctrl.DC_CAL_CH2<<5|
                            drv8301_ctrl.OC_TOFF<<6);
        packets_to_tx[2] = DRV8301_READ|DRV8301_STATUS_REG_1|0x0000;
        
        //Send the packet, not a blocking call if data is under 4 packets
        SPI_PutArray(packets_to_tx,3);
        uint16 stuff[5];
        uint8 add = 0;
        while( !(SPI_ReadTxStatus()&SPI_STS_SPI_DONE) ){
            if(timeout_counter==SPI_TIMEOUT_VALUE){
                status = DRV8301_COM_ERROR_TIMEOUT;
                break;
            }
            else{
                if(SPI_ReadRxStatus()&SPI_STS_RX_FIFO_NOT_EMPTY){
                    stuff[add] = SPI_ReadRxData();
                    add++;
                    timeout_counter = 0;
                }
                timeout_counter++;
            }
        }
        
        /*If we did not hit any errors, continue with execution*/
        uint16_t drv8301_stat_0 ;
        uint16_t drv8301_stat_1 ;
        uint16_t drv8301_ctrl_1 ;
        
        if(status== DRV8301_NO_COM_ERROR){
            /*Check to make sure we recieved data back on spi bus*/
            if(SPI_GetRxBufferSize()>0){
                drv8301_stat_0 = SPI_ReadRxData();
                drv8301_stat_1 = SPI_ReadRxData();
                drv8301_ctrl_1 = SPI_ReadRxData();
                uint16_t fun = 0;
            }
            
        }
    }
    
    
    
}

/*******************************************************************************
* Function Name: drv8301_register_parse
****************************************************************************//**
*
*******************************************************************************/
uint8_t drv8301_register_parse(uint16_t reg_copy, uint8_t reg_bit){
    return (uint8_t) (0x0001&(reg_copy>>reg_bit));
}


/* [] END OF FILE */
