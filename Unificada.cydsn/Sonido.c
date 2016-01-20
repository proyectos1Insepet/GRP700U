/*
*********************************************************************************************************
*                                           GRP500 CODE
*
*                             (c) Copyright 2013; Sistemas Insepet LTDA
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                               GRP500 CODE
*
*                                             CYPRESS PSoC5LP
*                                                with the
*                                            CY8C5969AXI-LP035
*
* Filename      : Sonido.c
* Version       : V1.00
* Programmer(s) : 
                  
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <device.h>
#include "VariablesG.h"
#include "Corriente.h"
#include "Extra.h"
#include "Diesel.h"

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/


unsigned int i1;
unsigned int i2;
char NextSample_1;
char NextSample_2;





/*
*********************************************************************************************************
*                                         CY_ISR(Interrupcion_1)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/

CY_ISR(Interrupcion_1){

    Timer_1_ReadStatusRegister();
    VDAC8_1_SetValue(NextSample_1);
    if(producto1==(extra&0x0f)){
        if(i1 <= ExtraNumSamples){
        	NextSample_1=(Extra[i1]);
        	i1++;  
        }
        else{
            Timer_1_Stop();
            VDAC8_1_Stop();
            //SDown_Write(0);
        }
    }
    if(producto1==(corriente&0x0f)){
        if(i1 <= CorrienteNumSamples){
        	NextSample_1=(Corriente[i1]);
        	i1++;  
        }
        else{
            Timer_1_Stop();
            VDAC8_1_Stop();;
            //SDown_Write(0);
        }
    }
    if(producto1==(diesel&0x0f)){
        if(i1 <= DieselNumSamples){
        	NextSample_1=(Diesel[i1]);
        	i1++;  
        }
        else{
            Timer_1_Stop();
            VDAC8_1_Stop();;
           // SDown_Write(0);
        }
    }
}

/*
*********************************************************************************************************
*                                        CY_ISR(Interrupcion_2)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/

CY_ISR(Interrupcion_2){

    Timer_2_ReadStatusRegister();
    VDAC8_2_SetValue(NextSample_2);
    SDown_Write(1);
    if(producto2==(extra2&0x0f)){
        if(i2 <= ExtraNumSamples){
        	NextSample_2=(Extra[i2]);
        	i2++;  
        }
        else{
            Timer_2_Stop();
            VDAC8_2_Stop();
            //SDown_Write(0);
        }
    }
    if(producto2==(corriente2&0x0f)){
        if(i2 <= CorrienteNumSamples){
        	NextSample_2=(Corriente[i2]);
        	i2++;  
        }
        else{
            Timer_2_Stop();
            VDAC8_2_Stop();
            //SDown_Write(0);
        }
    }
    if(producto2==(diesel2&0x0f)){
        if(i2 <= DieselNumSamples){
        	NextSample_2=(Diesel[i2]);
        	i2++;  
        }
        else{
            Timer_2_Stop();
            VDAC8_2_Stop();
            //SDown_Write(0);
        }
    }
    
}

/*
*********************************************************************************************************
*                                         sonido(uint8 audio)
*
* Description : 
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/

void sonido(uint8 ld)
{
    SDown_Write(1);
    CyDelay(250);
    if(ld==1){
        isr_1_StartEx(Interrupcion_1);
        Timer_1_Start();
        VDAC8_1_Start();
        i1=0;
        NextSample_1=0;
    }
    else{
        isr_2_StartEx(Interrupcion_2);
        Timer_2_Start();
        VDAC8_2_Start();
        i2=0;
        NextSample_2=0;        
    }
    
}