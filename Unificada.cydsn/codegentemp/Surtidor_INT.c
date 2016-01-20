/*******************************************************************************
* File Name: Surtidor_INT.c
* Version 2.30
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Surtidor.h"
#include "CyLib.h"


/***************************************
* Custom Declratations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if( (Surtidor_RX_ENABLED || Surtidor_HD_ENABLED) && \
     (Surtidor_RXBUFFERSIZE > Surtidor_FIFO_LENGTH))


    /*******************************************************************************
    * Function Name: Surtidor_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Surtidor_rxBuffer - RAM buffer pointer for save received data.
    *  Surtidor_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  Surtidor_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  Surtidor_rxBufferOverflow - software overflow flag. Set to one
    *     when Surtidor_rxBufferWrite index overtakes
    *     Surtidor_rxBufferRead index.
    *  Surtidor_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when Surtidor_rxBufferWrite is equal to
    *    Surtidor_rxBufferRead
    *  Surtidor_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  Surtidor_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(Surtidor_RXISR)
    {
        uint8 readData;
        uint8 increment_pointer = 0u;
        #if(CY_PSOC3)
            uint8 int_en;
        #endif /* CY_PSOC3 */

        /* User code required at start of ISR */
        /* `#START Surtidor_RXISR_START` */

        /* `#END` */

        #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
            int_en = EA;
            CyGlobalIntEnable;
        #endif /* CY_PSOC3 */

        readData = Surtidor_RXSTATUS_REG;

        if((readData & (Surtidor_RX_STS_BREAK | Surtidor_RX_STS_PAR_ERROR |
                        Surtidor_RX_STS_STOP_ERROR | Surtidor_RX_STS_OVERRUN)) != 0u)
        {
            /* ERROR handling. */
            /* `#START Surtidor_RXISR_ERROR` */

            /* `#END` */
        }

        while((readData & Surtidor_RX_STS_FIFO_NOTEMPTY) != 0u)
        {

            #if (Surtidor_RXHW_ADDRESS_ENABLED)
                if(Surtidor_rxAddressMode == (uint8)Surtidor__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readData & Surtidor_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readData & Surtidor_RX_STS_ADDR_MATCH) != 0u)
                        {
                            Surtidor_rxAddressDetected = 1u;
                        }
                        else
                        {
                            Surtidor_rxAddressDetected = 0u;
                        }
                    }

                    readData = Surtidor_RXDATA_REG;
                    if(Surtidor_rxAddressDetected != 0u)
                    {   /* store only addressed data */
                        Surtidor_rxBuffer[Surtidor_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* without software addressing */
                {
                    Surtidor_rxBuffer[Surtidor_rxBufferWrite] = Surtidor_RXDATA_REG;
                    increment_pointer = 1u;
                }
            #else  /* without addressing */
                Surtidor_rxBuffer[Surtidor_rxBufferWrite] = Surtidor_RXDATA_REG;
                increment_pointer = 1u;
            #endif /* End SW_DETECT_TO_BUFFER */

            /* do not increment buffer pointer when skip not adderessed data */
            if( increment_pointer != 0u )
            {
                if(Surtidor_rxBufferLoopDetect != 0u)
                {   /* Set Software Buffer status Overflow */
                    Surtidor_rxBufferOverflow = 1u;
                }
                /* Set next pointer. */
                Surtidor_rxBufferWrite++;

                /* Check pointer for a loop condition */
                if(Surtidor_rxBufferWrite >= Surtidor_RXBUFFERSIZE)
                {
                    Surtidor_rxBufferWrite = 0u;
                }
                /* Detect pre-overload condition and set flag */
                if(Surtidor_rxBufferWrite == Surtidor_rxBufferRead)
                {
                    Surtidor_rxBufferLoopDetect = 1u;
                    /* When Hardware Flow Control selected */
                    #if(Surtidor_FLOW_CONTROL != 0u)
                    /* Disable RX interrupt mask, it will be enabled when user read data from the buffer using APIs */
                        Surtidor_RXSTATUS_MASK_REG  &= (uint8)~Surtidor_RX_STS_FIFO_NOTEMPTY;
                        CyIntClearPending(Surtidor_RX_VECT_NUM);
                        break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                    #endif /* End Surtidor_FLOW_CONTROL != 0 */
                }
            }

            /* Check again if there is data. */
            readData = Surtidor_RXSTATUS_REG;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START Surtidor_RXISR_END` */

        /* `#END` */

        #if(CY_PSOC3)
            EA = int_en;
        #endif /* CY_PSOC3 */

    }

#endif /* End Surtidor_RX_ENABLED && (Surtidor_RXBUFFERSIZE > Surtidor_FIFO_LENGTH) */


#if(Surtidor_TX_ENABLED && (Surtidor_TXBUFFERSIZE > Surtidor_FIFO_LENGTH))


    /*******************************************************************************
    * Function Name: Surtidor_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  Surtidor_txBuffer - RAM buffer pointer for transmit data from.
    *  Surtidor_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmited byte.
    *  Surtidor_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(Surtidor_TXISR)
    {

        #if(CY_PSOC3)
            uint8 int_en;
        #endif /* CY_PSOC3 */

        /* User code required at start of ISR */
        /* `#START Surtidor_TXISR_START` */

        /* `#END` */

        #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
            int_en = EA;
            CyGlobalIntEnable;
        #endif /* CY_PSOC3 */

        while((Surtidor_txBufferRead != Surtidor_txBufferWrite) &&
             ((Surtidor_TXSTATUS_REG & Surtidor_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer. */
            if(Surtidor_txBufferRead >= Surtidor_TXBUFFERSIZE)
            {
                Surtidor_txBufferRead = 0u;
            }

            Surtidor_TXDATA_REG = Surtidor_txBuffer[Surtidor_txBufferRead];

            /* Set next pointer. */
            Surtidor_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START Surtidor_TXISR_END` */

        /* `#END` */

        #if(CY_PSOC3)
            EA = int_en;
        #endif /* CY_PSOC3 */

    }

#endif /* End Surtidor_TX_ENABLED && (Surtidor_TXBUFFERSIZE > Surtidor_FIFO_LENGTH) */


/* [] END OF FILE */
