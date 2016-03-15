/*
*********************************************************************************************************
*                                           GRP550/700 CODE
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
*                                               GRP550/700 CODE
*
*                                             CYPRESS PSoC5LP
*                                                with the
*                                            CY8C5969AXI-LP035
*
* Filename      : main.c
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
#include "Protocolo.h"
#include "VariablesG.h"
#include "Print.h"
#include "Sonido.h"
#include "LCD.h"
#include "ibutton.h"
#include "I2C.h"

/*
*********************************************************************************************************
*                                             DECLARACION DE FUNCIONES
*********************************************************************************************************
*/
uint8 letras[26]={0x25,0x42,0x31,0x27,0x1D,0x28,0x29,0x2A,0x22,0x2B,0x2C,0x2D,0x35,0x34,0x23,0x24,0x1B,0x1E,0x26,0x1F,0x21,0x32,0x1C,0x30,0x20,0x2F};
uint8 test[18]="Test de Impresora";
uint8 puk[8]="18928005";
uint8 serial[17]="0FFFFF8FEEBEB2DC0";
uint8 pasword_corte[5]={4,'1','2','3','4'};
CY_ISR(animacion2);
CY_ISR(animacion);
/*
*********************************************************************************************************
*                                         init( void )
*
* Description : Verifica el serial, inicia los perosfericos, la version y los datos de la estación.
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : main()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void init(void){
	/****Inicio de perifericos****/
    CyGlobalIntEnable;
    Surtidor_EnableRxInt();
    PC_EnableRxInt();
    I2C_1_Start();
    Surtidor_Start();
    Impresora_Start();
    LCD_1_Start();
    LCD_2_Start();
    Impresora_EnableRxInt();
    LCD_1_EnableRxInt();
    LCD_2_EnableRxInt();
    PC_Start();
    VDAC8_3_Start();
    CyDelay(5);	

	/****Lectura de variables en memoria eeprom****/
	/*serial[0]=16;
	y=0;
	leer_eeprom(615,17);
	if(buffer_i2c[0]!=16){
		serial[0]=16;
		write_eeprom(615,serial);
	}	
	else{
		while(y!=16){
			for(x=1;x<=16;x++){
				if(buffer_i2c[x]==serial[x]){
					y++;
				}
			}
		}
	}*/
	leer_eeprom(0,32);
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.nombre[x]=buffer_i2c[x];
	}
	leer_eeprom(32,15);
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.nit[x]=buffer_i2c[x];
	}	
	leer_eeprom(64,32);
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.direccion[x]=buffer_i2c[x];
	}
	leer_eeprom(128,32);
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.lema1[x]=buffer_i2c[x];
	}	
	leer_eeprom(160,32);
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.lema2[x]=buffer_i2c[x];
	}	
	leer_eeprom(47,12);
	for(x=0;x<=buffer_i2c[0];x++){
		rventa.telefono[x]=buffer_i2c[x];
	}	
	leer_eeprom(96,9);
	if((buffer_i2c[0]<=0x09)&&(buffer_i2c[0]>=0x01)){
		for(x=0;x<=7;x++){
			rventa.password[x+1]=buffer_i2c[x+1];
		}
		rventa.password[0]=buffer_i2c[0];
	}
	else{
		for(x=1;x<=8;x++){
			rventa.password[x]=x;
		}	
	}
    leer_eeprom(1130,10);
    for(x=0;x<=buffer_i2c[0];x++){
		producto1n[x]=buffer_i2c[x];
	}
	leer_eeprom(671,5);
	if(buffer_i2c[0]==4){
		for(x=1;x<=4;x++){
			pasword_corte[x]=buffer_i2c[x];
		}
		pasword_corte[0]=buffer_i2c[0];
	}
	leer_eeprom(449,2);
	if(buffer_i2c[0]==1){
		extra=buffer_i2c[1];
	}
	leer_eeprom(451,2);
	if(buffer_i2c[0]==1){
		corriente=buffer_i2c[1];
	}
	leer_eeprom(453,2);
	if(buffer_i2c[0]==1){
		diesel=buffer_i2c[1];
	}
    leer_eeprom(1006,2);
	if(buffer_i2c[0]==1){
		kero=buffer_i2c[1];
	}
	leer_eeprom(442,2);
	if(buffer_i2c[0]==5){
		 versurt=(buffer_i2c[1]&0x0f);
	}
	leer_eeprom(634,2);
	if(buffer_i2c[0]==1){
		extra2=buffer_i2c[1];
	}
	leer_eeprom(636,2);
	if(buffer_i2c[0]==1){
		corriente2=buffer_i2c[1];
	}
	leer_eeprom(638,2);
	if(buffer_i2c[0]==1){
		diesel2=buffer_i2c[1];
	}
    leer_eeprom(1008,2);
	if(buffer_i2c[0]==1){
		kero2=buffer_i2c[1];
	}
	leer_eeprom(602,6);
	if(buffer_i2c[0]==5){
		 decimalD=(buffer_i2c[3]&0x0f);
		 decimalV=(buffer_i2c[5]&0x0f);
	}	
	leer_eeprom(455,2);
    if(buffer_i2c[0]==1){
        bandera[0]=buffer_i2c[0];    /// carga bandera seleccionada
	    bandera[1]=(buffer_i2c[1]&0x0f);
    }  
	leer_eeprom(444,2);
	if(buffer_i2c[0]==1){
		 ppux10=(buffer_i2c[1]&0x0f);
	}
	leer_eeprom(611,2);
	if(buffer_i2c[0]==1){
		 print1[1]=(buffer_i2c[1]&0x0f);
	}
	leer_eeprom(613,2);
	if(buffer_i2c[0]==1){
		 print2[1]=(buffer_i2c[1]&0x0f);
	}
	leer_eeprom(668,3);
	if(buffer_i2c[0]==2){
		 id_corte=((buffer_i2c[2]<<8)|buffer_i2c[2]);
	}
	else{
		 id_corte=1;
	}	
    leer_eeprom(984,2);										//Copia de Recibo
	if(buffer_i2c[0]==1){
		 copia_recibo[1]=buffer_i2c[1];
	}
	else{
		 copia_recibo[0]=1;
		 copia_recibo[1]=1;
		 write_eeprom(984,copia_recibo);
	}	
    leer_eeprom(978,6);										//Id venta
	if(buffer_i2c[0]==5){
		for(x=0;x<=buffer_i2c[0];x++){
			id_venta[x]=buffer_i2c[x]; 
		}
	}
	else{
		id_venta[0]=5;	
		id_venta[1]='0';
		id_venta[2]='0';
		id_venta[3]='0';
		id_venta[4]='0';
		id_venta[5]='0';
		write_eeprom(978,id_venta);
	}
	no_venta=((id_venta[5]&0x0F)*10000)+((id_venta[4]&0x0F)*1000)+((id_venta[3]&0x0F)*100)+((id_venta[2]&0x0F)*10)+((id_venta[1]&0x0F));
	
    /*********Activa GRP700 o GRP500******************/
	version=2; 	
}

/*
*********************************************************************************************************
*                                         init_surt( void )
*
* Description : Busca las posiciones del surtidor y las graba en lado.a.dir y lado.b.dir
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : main()
*
* Note(s)     : Falta generar codigo para los casos 1 y 2
*********************************************************************************************************
*/
void init_surt(void){
	uint8 seguir=0;
	while(seguir==0){
		switch(ver_pos()){
			case 0:
														//mostrar error de comunicacion	
			break;
			
			case 1:
														//Una de las pos no responde
			break;
			
			case 2:
			CyDelay(100);
			if(get_estado(lado.a.dir)==6){	
				if(get_estado(lado.b.dir)==6){
					seguir=2;							//Las dos pos contestaron
				}
			}	
			break;
		}
	}
		
}

/*
************************************************************************************************************
*                                         void error_op()
*
* Description : Muestra en la pantalla el mensaje de operación incorrecta y regresa al inicio del Flujo LCD
*               
*
* Argument(s) : uint8 lcd, para elegir cual pantalla entra en esta función
*
* Return(s)   : none
*
* Caller(s)   : Desde cualquier momento de la operacion donde ocurra un error por parte del usuario
*
* Note(s)     : none.
************************************************************************************************************
*/
void error_op(uint8 lcd){
	if(lcd==1){
	    set_imagen(1,85);
	    flujo_LCD=100;
		count_protector=1;
	    isr_3_StartEx(animacion);  
	    Timer_Animacion_Start();
	}
	else{
	    set_imagen(2,85);
	    flujo_LCD2=100;
		count_protector2=1;
	    isr_4_StartEx(animacion2);  
	    Timer_Animacion2_Start();	
	}
}


/*
***********************************************************************************************************
*                                         void polling_LCD1(void)
*
* Description : Ejecuta las diferentes funciones que se pueden realizar desde la pantalla 1
*               
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : Se ejecuta dentro del ciclo infinito del main()
*
* Note(s)     : none.
***********************************************************************************************************
*/

void polling_LCD1(void){  
    switch(flujo_LCD){
        case 100:  
         if(count_protector>=2){					//Espera a que pasen 1.6 seg aprox e inicia la pantalla
            flujo_LCD=0;							 
            isr_3_Stop(); 
            Timer_Animacion_Stop(); 
            count_protector=0;
         }            
        break;
        
        case 0:			
         isr_3_StartEx(animacion); 
         Timer_Animacion_Start();
         count_protector=0;  
         flujo_LCD=1;		 
        break;
        
        case 1:
         if(LCD_1_GetRxBufferSize()==8){ 
             LCD_1_ClearRxBuffer(); 
             isr_3_Stop(); 
             Timer_Animacion_Stop();
			 if(version==2){			
            	set_imagen(1,99); //GRP700
			 }
			 else{
            	set_imagen(1,69);// version 550			 	
			 }
            
             flujo_LCD=3;
             CyDelay(100);   //Cambio para case 3
             LCD_1_ClearRxBuffer();
         }
        break;
//        
//        case 2: 
//         flujo_LCD=3;
//
//        
//        break;
//        
        case 3:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x5C:
					  Buffer_LCD1.preset=0;	
					  if(version==1){	      				//Venta 550
	                      set_imagen(1,5);
	                      flujo_LCD=4;      
					  }
					  if(version==2){	
                          set_imagen(1,73);
	                      flujo_LCD=25;            				//Venta 700	                      
					  }					
                    break;                
                    
                    case 0x45:		  
					  set_imagen(1,37);	
                      teclas1=0; 						    //Otras opciones
                      flujo_LCD=15;   
                    break; 

                    case 0x59:		 
                      set_imagen(1,37);
					  teclas1=0;								//Corte
                      flujo_LCD=39;   
                    break;					
					
                    case 0x7E:									//ir a menu
					  set_imagen(1,0);	
                      flujo_LCD=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
        
        case 4:   
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				Buffer_LCD1.preset&=0xFC;
                switch(LCD_1_rxBuffer[3]){
                    case 0x0F:		   					 	 //Preset por dinero	                               
                      set_imagen(1,6);                   
                      teclas1=0;                            	 //Inicia el contador de teclas                                         
                      write_LCD(1,'$',0);
					  Buffer_LCD1.preset|=2;
                      flujo_LCD=5;   
                    break;
                    
                    case 0x10:  	                           //Inicia el contador de teclas
                      set_imagen(1,13);              
                      teclas1=0;       
                      comas1=0;
                      write_LCD(1,'G',0);
					  Buffer_LCD1.preset|=1;							// Preset por volumen
                      flujo_LCD=5;   
                    break;
                    
                    case 0x43:  	                                     
                      set_imagen(1,7);							//Full
                      flujo_LCD=6;    
                    break;
                    
                    case 0x3B:                        			//Cancel                     
					  if(version==2){			
		            	 set_imagen(1,99);
					  }
					  else{
		            	 set_imagen(1,69);			 	
					  } 
					  flujo_LCD=3;  
                    
                    break; 
					
                    case 0x7E:									//ir a menu
					  set_imagen(1,0);
                      flujo_LCD=0;     
                    break;					
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;
        
        case 5:   
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=(versurt-1)){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }  
                    if(LCD_1_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas1>=1 && comas1==0){
                            teclas1++;
                            Buffer_LCD1.valor[teclas1]=0x2C;
                            write_LCD(1,0x2C,teclas1);
                            comas1=1;
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(1,5);
                        flujo_LCD=4;
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1>=1){
                        set_imagen(1,7);
						Buffer_LCD1.valor[0]=teclas1;
                        flujo_LCD=6;                                                 				
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }          
        break;
        
        case 6:
		 CyDelay(100);
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3]==0x7E){							//ir a menu
					set_imagen(1,0);					//Cancel
					flujo_LCD=0;
                }												
			}
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
			break;
		 }		
         if(get_estado(lado.a.dir)==7){													//Espera a que este en listo el equipo	
			CyDelay(50);
			producto1=estado_ex(lado.a.dir);											//Obtiene el grado de la manguera
			CyDelay(50);		
			if((Buffer_LCD1.preset&0x02)==0x02||(Buffer_LCD1.preset&0x01)==0x01){		//Dependiendo del preset hace la programación
				if(programar(lado.a.dir,producto1,Buffer_LCD1.valor,(Buffer_LCD1.preset&0x03))==0){
					set_imagen(1,0);
					flujo_LCD=0;
					break;
				}					
			}
			SDown_Write(1);
			producto1=estado_ex(lado.a.dir);
			if(producto1!=0){															//Si el grado es correcto reproduce audio
				if(producto1==(extra&0x0f)||producto1==(corriente&0x0f)||producto1==(diesel&0x0f)||producto1==(kero&0x0f)){
					sonido(1);																		
				}
				Surtidor_PutChar(0x10|lado.a.dir);									//Autoriza el surtidor
			    set_imagen(1,8);
                flujo_LCD=7;				
			}
         }  	
        break;
        
        case 7:
		 CyDelay(50);
		 switch(get_estado(lado.a.dir)){
	        case 0x0B:                     //Termino venta            
				CyDelay(100);
				if(venta(lado.a.dir)==1){	
		            flujo_LCD=8;
				}
			 break;	
				
	        case 0x0A:                         
				CyDelay(100);                  //Termino venta
				if(venta(lado.a.dir)==1){	
		            flujo_LCD=8;
				}
			break;

	        case 0x06: 
                set_imagen(1,0); //No hizo venta
				flujo_LCD=0;
			break;		
             			 	
         }		
        break;
        
        case 8:
         switch(version){                     		  
             case 1:										//Si es 550 pasa a imprimir
             set_imagen(1,11);
	         isr_3_StartEx(animacion); 
	         Timer_Animacion_Start();               
	         count_protector=0;	
             flujo_LCD=10; 		
            break;
            
            case 2:                                	 	
			 if((Buffer_LCD1.preset&0x04)!=0x04){				
	            set_imagen(1,11);
		        isr_3_StartEx(animacion); 
		        Timer_Animacion_Start();
		        count_protector=0; 		        
		        flujo_LCD=10;                         	
			 }
			 else{
              set_imagen(1,11);
			  Buffer_LCD1.posventa=0;	
		      isr_3_StartEx(animacion); 
		      Timer_Animacion_Start();
		      count_protector=0;
              flujo_LCD=10;				
			 }
            break;            
         } 
        break;
        
        case 9:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				count_protector=0;
                if(teclas1<=6){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.placa[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);                        
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas1++;                        
                        Buffer_LCD1.placa[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);                        
                    }  
                    if(LCD_1_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas1++;                        
                        Buffer_LCD1.placa[teclas1]='-';
                        write_LCD(1,'-',teclas1);                        
                    } 					
                    if(LCD_1_rxBuffer[3]>=0x1B && LCD_1_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_1_rxBuffer[3]==letras[x]){
                                teclas1++;                            
                                Buffer_LCD1.placa[teclas1]=x+0x41;
                                write_LCD(1,(x+0x41),teclas1);                            
                            }
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){                                        //Borrar - Cancelar
                    if(teclas1==0){                                                 //Si no tiene nada pasa a pedir impresion
                        set_imagen(1,11);                        
                        Buffer_LCD1.placa[0]=0;
						Buffer_LCD1.posventa=0;
                        flujo_LCD=10;
                    }
                    else{
                        write_LCD(1,0x20,teclas1);                        
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){                                        //Enter pasa a imprimir
                    if(teclas1>=1){                        
                        Buffer_LCD1.placa[0]=teclas1;                        
                        Buffer_LCD1.posventa=1;
                        flujo_LCD=13;
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }
		 if(count_protector>=30){
            set_imagen(1,11);
            Buffer_LCD1.placa[0]=0;
			Buffer_LCD1.posventa=0;	
			count_protector=0;
            flujo_LCD=10;
		 }
        break;

        case 10:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x39:                          //Si Imprime pasar a placa                      
                      teclas1 = 0;
                      if((Buffer_LCD1.preset&0x04)!=0x04){
                        set_imagen(1,10);
                        flujo_LCD=9; 
                      }else{
                        flujo_LCD=13;
                      }
                    break; 
                    
                    case 0x38:                          //No Imprime 
                      set_imagen(1,12);
				      count_protector=1;
				      isr_3_StartEx(animacion);  
				      Timer_Animacion_Start();
                      flujo_LCD=100; 
                   
                    break;                     
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
		 if(count_protector>=30){
            set_imagen(1,12);
		    count_protector=1;
            flujo_LCD=100; 
		 }		
        break;  
        
        case 11:		
		if(touch_read_memoria(1,1)>=6){
			CyDelay(10);
			if(touch_read_memoria(1,0x21)>=6){
		        if(touch_present(1)==1){
		            LCD_1_PutChar(0x33);
		            if(touch_write(1,0x33)){
		                for(z=0;z<=7;z++){
		                    Buffer_LCD1.id[z]=touch_read_byte(1);
		                }				
						set_imagen(1,19);  //Id correctamente reconocida
						Buffer_LCD1.preset|=0x04;
						CyDelay(500);
						set_imagen(1,14); 
						teclas1=0;
						Buffer_LCD1.km[0]=0; //Pedir Kilometraje
						flujo_LCD=12;
					}
				}	
			}
		}
	    if(LCD_1_GetRxBufferSize()==8){
	        if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
	            switch(LCD_1_rxBuffer[3]){
	                case 0x3B:
	                 set_imagen(1,99); //Kilometraje
	                 flujo_LCD=3;	                 
	                break; 
	            }
	        }
	        CyDelay(100);            
	        LCD_1_ClearRxBuffer();
	    }
        break;
        
        case 12:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=5){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;                    
                        Buffer_LCD1.km[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            //Comando de 0
                        teclas1++;                    
                        Buffer_LCD1.km[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){				//Cancelar
                    if(teclas1==0){
                        set_imagen(1,5);
						flujo_LCD=4;
                    }
                    else{
                        write_LCD(1,0x20,teclas1);
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){				//Enter
                    if(teclas1>=1 && Buffer_LCD1.km[1]!=0x30){                       
                        set_imagen(1,5);
                        Buffer_LCD1.km[0]=teclas1;   
                        flujo_LCD=4;                                           
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;
        
        case 13:
			imprimir(print1[1], producto1,0,lado.a.dir);  //Imprime recibo sin copia
			if(version==1||(copia_recibo[1]==0)){
				set_imagen(1,12);
				count_protector=1;
			    isr_3_StartEx(animacion);  
			    Timer_Animacion_Start();
				flujo_LCD=100;
			}
			else{
				set_imagen(1,49);
				teclas1=0;
				count_protector=1;
			    isr_3_StartEx(animacion);  
			    Timer_Animacion_Start();   //Va a copia de recibo
				flujo_LCD=33;					
			}
        break;  
        
		case 14:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x58:								 	 //Configurar Fecha y Hora	                                                                            
                      teclas1=0;                            	 //Inicia el contador de teclas  
                      set_imagen(1,62); 
					  if(leer_hora()==1){
						Hora_LCD(1);
					  } 
					  if(leer_fecha()==1){
						Fecha_LCD(1);						
					  }		
                    flujo_LCD=16;  
                    break;
                    
                    case 0x5D:  								//Cambiar Precio
                                        
                      set_imagen(1,92);
                      Precio_LCD(1,0x00,0xD6,429); // primer producto  
					  Precio_LCD(1,0x00,0x89,435); // segundo producto 			  
                      Precio_LCD(1,0x01,0x24,423); // tercer producto 
                      Precio_LCD(1,0x01,0x78,1000); //cuarto producto 
                      flujo_LCD=19; 
                      
                    break;
                    
                   case 0x5A:  					
                      set_imagen(1,96);
					  Nombre_LCD(1);
					  direccion_LCD(1);
					  telefono_LCD(1);
					  nit_LCD(1);
					  lema1_LCD(1);
					  lema2_LCD(1);							//Datos de Recibo
                      flujo_LCD=18;
                    
                    break;
                    
                    case 0x5B:         
                      set_imagen(1,47); 
					  teclas1=0;               			//Cambiar pasword                     
                      flujo_LCD=17;
                    
                    break;   					
					
                    case 0x96:     
                      set_imagen(1,95); 
					  teclas1=0;                   			//Configurar Surtidor                   
                      flujo_LCD=27;
                    
                    break; 
					
                    case 0x65:        
                      set_imagen(1,77);                 			//Test                     
                      flujo_LCD=20;
                    break;
					
                    case 0x93:     
                      set_imagen(1,110); 
					  if(touch_read_memoria(1,1)>=6){
						CyDelay(10);
						Placa_LCD(1);
						if(touch_read_memoria(1,0x21)>=6){						
							Cuenta_LCD(1);
						}
					  }			                  
                      flujo_LCD=35;                              //Grabar Ibutton  
                    
                    break;
                    
                    case 0x70:         
                        set_imagen(1,137);                			//Tipo de impresora 
                        flujo_LCD = 41;
                         				
                    break;
					
                    case 0x7E:									//ir a menu
					  set_imagen(1,0);
                      flujo_LCD=0;     
                    break;				
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		  
		break;
        
        case 15: 
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=7){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD1.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,'*',teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD1.password[teclas1]=0x30;
                        write_LCD(1,'*',teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						error_op(1);						
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas1>=1){
						y=0;
						for(x=0;x<teclas1;x++){						
							if(rventa.password[x+1]==(Buffer_LCD1.password[x]-0x30)){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==rventa.password[0]){								
	                        set_imagen(1,38);
							CyDelay(500);	                        
							if(version==1){
		                    	set_imagen(1,93);                     
							}
							else if(version==2){
								set_imagen(1,112);
							}
                            flujo_LCD=14;
						}
						else{
							y=0;
							for(x=0;x<teclas1;x++){						
								if(puk[x]==Buffer_LCD1.password[x]){
									y++;
								}
							}
							if(y==8){								
								set_imagen(1,38);
								CyDelay(500);								
								if(version==1){
									set_imagen(1,93);                     
								}
								else if(version==2){
									set_imagen(1,112);
								}		
                                flujo_LCD=14;
							}	
							else{		
								set_imagen(1,39);								
								count_protector=1;
								isr_3_StartEx(animacion);  
								Timer_Animacion_Start();
                                flujo_LCD=100;
							}		
						}
                    }
					else{
                        set_imagen(1,39);                        
					 	count_protector=1;
			         	isr_3_StartEx(animacion);  
			         	Timer_Animacion_Start();					
                        flujo_LCD=100;
					}					
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		  
        break; 
        
        case 16:   
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
					case 0x3F:								//Hora
						set_imagen(1,64);
                        teclas1=0;
						comas1=0;												
						write_LCD(1,'a',6);						
						write_LCD(1,'m',7);
                        flujo_LCD=21;
					break;
					
					case 0x3E:								//Fecha
						set_imagen(1,63);					
                        teclas1=0;
						comas1=0;
						flujo_LCD=22;
						
					break;
						
                    case 0x7E:								//ir a menu
					  set_imagen(1,112);	//Original en 0
                      flujo_LCD=14;     //Otras Opciones Devolver original en 0
                    break;						
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         } 
        break;
        
        case 17:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=7){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD1.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD1.password[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						error_op(1);						
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			   
					if(teclas1>=6){
						set_imagen(1,57);		                    						
						rventa.password[0]=teclas1;
						for(x=0;x<teclas1;x++){
							rventa.password[x+1]=(Buffer_LCD1.password[x]-0x30);
						}
						if(write_eeprom(96,rventa.password)==0){							//Guarda el nombre en la eeprom
							set_imagen(1,85);
							flujo_LCD=100;
						}
                        flujo_LCD=0;
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }   
        break;
        
        case 18:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x76:								 	 //Cambiar Nombre	             
                      set_imagen(1,48);
					  teclado=0;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 0;
                      flujo_LCD=24;
                    break;
					
                    case 0x82:								 	 //Cambiar Lema1	
                      set_imagen(1,121);                       
					  teclado=2;
                      teclas1=0;                            	 //Inicia el contador de teclas  
                      nombreproducto = 0;
                      flujo_LCD=24;  					
                    break;
					
                    case 0x83:								 	 //Cambiar Lema2	             
                      set_imagen(1,122);
					  teclado=3;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 0;
                      flujo_LCD=24;
                    break;					
                    
                    case 0x77:  								//Cambiar Direccion                      
					  set_imagen(1,120);
                      teclado=1;                      
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 0;
                      flujo_LCD=24;
                    break;
                    
                    case 0x78:  								//Cambiar telefono
                      set_imagen(1,101);
					  teclado=1;                      
                      teclas1=0;                            	 //Inicia el contador de teclas
                      flujo_LCD=23;	
                    break;
                    
                    case 0x7A:                         			//Cambiar NIT                     
                      set_imagen(1,50);
					  teclado=0;					 
                      teclas1=0;                            	 //Inicia el contador de teclas
                      flujo_LCD=23;
                    break;   

                    case 0x95:                         			//Cambiar Bandera 
						if(version==2){
						  set_imagen(1,102);
	                      flujo_LCD=32;
						}
                    break; 
					
                    case 0x7E:									//ir a menu
					  set_imagen(1,112);            //Volver a otras opciones Maicol
                      flujo_LCD=14;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }	
        break;
        
        case 19:
        if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:		
                      set_imagen(1,6);   
                      teclas1=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=corriente;	
					  write_LCD(1,'$',teclas1);							 	 //Producto 1	                               
                      flujo_LCD=26; 
                    break;
                    
                    case 0x80:  	
                      set_imagen(1,6);		
                      teclas1=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=diesel;
					  write_LCD(1,'$',teclas1);						 //Producto 2
                      flujo_LCD=26;              
                    break;
                    
                    case 0x81:  
                      set_imagen(1,6);			
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=extra;
					  write_LCD(1,'$',teclas1);						//Preducto 3
                      flujo_LCD=26;	
                    break;
                    case 0x82:  	
                      set_imagen(1,6); 			
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=kero;
					  write_LCD(1,'$',teclas1);					//Otro producto
                      flujo_LCD=26;		
                    break;
                   										
                    case 0x7E:									//ir a menu
					  set_imagen(1,112);            //Otras Opciones
                      flujo_LCD=14;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;
       
        case 20:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				switch(LCD_1_rxBuffer[3]){
	                case 0x62:								 	 //Comunicacion Surtidores
						set_imagen(1,57);
						CyDelay(100);
						if(get_estado(lado.a.dir)==6){	
							if(get_estado(lado.b.dir)==6){
								set_imagen(1,74);
								CyDelay(700);				  
								flujo_LCD=0; 
							}
							else{
								error_op(1);
							}
						}	
						else{
							error_op(1);
						}
	                break;
	                
	                case 0x63:  								 //Comunicacion Impresora
						set_imagen(1,55);
                        if(tipo_imp[1] == 1){    //Impresora Kiosko
						    for(x=0;x<=16;x++){
			        		    write_psoc1(1,test[x]);			
						    }
					        write_psoc1(1,10);
						    write_psoc1(1,10);
						    write_psoc1(1,10);
						    write_psoc1(1,10);
						    for(x=0;x<=16;x++){
			        		    write_psoc1(2,test[x]);			
						    }
					        write_psoc1(2,10);
						    write_psoc1(2,10);
						    write_psoc1(2,10);
						    write_psoc1(2,10);						
                        }
                        if (tipo_imp[1] == 2){             //Impresora panel
                            for(x=0;x<=16;x++){
			        		    write_psoc1(3,test[x]);			
						    }
					        write_psoc1(3,10);
						    write_psoc1(3,10);
						    write_psoc1(3,10);
						    write_psoc1(3,10);
						    for(x=0;x<=16;x++){
			        		    write_psoc1(4,test[x]);			
						    }
					        write_psoc1(4,10);
						    write_psoc1(4,10);
						    write_psoc1(4,10);
					    	write_psoc1(4,10);
                        }    
						set_imagen(1,107);
						flujo_LCD=37;
	                break;
	                
	                case 0x64:  								 //Comunicacion Ibutton
					if(touch_present(1)==1){
						LCD_1_PutChar(0x33);
						if(touch_write(1,0x33)){
							for(z=0;z<=7;z++){
							    Buffer_LCD1.id[z]=touch_read_byte(1);
							}
							crc_total=0;
							for(z=0;z<7;z++){
							    crc_total=crc_check(crc_total,Buffer_LCD1.id[z]);
							}	
							if(crc_total==Buffer_LCD1.id[7]){
								set_imagen(1,75);
								CyDelay(1000);				  
							   	flujo_LCD=0;
							}
							else{
								error_op(1);
							}
						}
					}
					else{
						error_op(1);
					}
	                break;
	               										
	                case 0x7E:									//ir a menu
					  set_imagen(1,0);
	                  flujo_LCD=0;     
	                break;	
				}
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }                    
        break;
        
        case 21:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				switch(teclas1){
	                case 0:										//Decenas de la Hora
	                    if(LCD_1_rxBuffer[3]<=1)					
						{
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[1]=LCD_1_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas1++;
	                        rventa.hora[1]=0;
	                        write_LCD(1,0x30,teclas1);
	                    }                     
	                break;
						
	                case 1:											//Unidades de la Hora
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[1]|=LCD_1_rxBuffer[3];						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas1++;
	                        write_LCD(1,0x30,teclas1);
	                    }                     
	                break;
						
	                case 2:											//Decenas de la Minutos					
	                    if(LCD_1_rxBuffer[3]<=5)					
						{
							teclas1++;					
							write_LCD(1,':',teclas1);							
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[0]=LCD_1_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
							teclas1++;					
							write_LCD(1,':',teclas1);							
	                        teclas1++;
	                        write_LCD(1,0x30,teclas1);
							rventa.hora[0]=0;
	                    } 
	                break;
						
	                case 4:											//Unidades de la Minutos					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    teclas1++;
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.hora[0]|=LCD_1_rxBuffer[3];						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas1++;
	                        write_LCD(1,0x30,teclas1);
	                    } 
	                break;
				}
                if(LCD_1_rxBuffer[3]==0x40){            		//Comando de Am/Pm
                    if(comas1==0){
                        comas1=1;
                        write_LCD(1,'p',6);
                    }
					else{
                        comas1=0;
                        write_LCD(1,'a',6);						
					}
					write_LCD(1,'m',7);
                } 				
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior						
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==5){
						rventa.hora[1]|=((comas1|2)<<5);
						if(write_hora()==1){
	                        flujo_LCD=14;                         
							if(version==1){
		                    	set_imagen(1,93);                     
							}
							else if(version==2){
								set_imagen(1,112);
							}
						}
                    }
                }

            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }        
        break;
		
		case 22:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
				switch(teclas1){
	                case 0:										//Decenas de dia
	                    if(LCD_1_rxBuffer[3]<=3)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    teclas1++;							
		                    rventa.fecha[0]=LCD_1_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        rventa.fecha[0]=0;
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    }                     
	                break;
						
	                case 1:											//Unidades de dia
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[0]|=LCD_1_rxBuffer[3];	
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    }                     
	                break;
						
	                case 2:											//Decenas de mes					
	                    if(LCD_1_rxBuffer[3]<=1)					
						{					
							write_LCD(1,'/',teclas1);
							teclas1++;							
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[1]=LCD_1_rxBuffer[3]<<4;
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0;					
							write_LCD(1,'/',teclas1);
							teclas1++;							
	                        write_LCD(1,0x30,teclas1);
							rventa.fecha[1]=0;
	                        teclas1++;							
	                    } 
	                break;
						
	                case 4:											//Unidades de mes					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[1]|=LCD_1_rxBuffer[3];	
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    } 
	                break;
						
	                case 5:											//Decenas de año					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{					
							write_LCD(1,'/',teclas1);
							teclas1++;							
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[2]=LCD_1_rxBuffer[3]<<4;
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0;					
							write_LCD(1,'/',teclas1);
							teclas1++;							
	                        write_LCD(1,0x30,teclas1);
							rventa.fecha[2]=0;
	                        teclas1++;							
	                    } 
	                break;
						
	                case 7:											//Unidades de año					
	                    if(LCD_1_rxBuffer[3]<=9)					
						{
		                    write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
		                    rventa.fecha[2]|=LCD_1_rxBuffer[3];	
		                    teclas1++;							
	                    }
	                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(1,0x30,teclas1);
	                        teclas1++;							
	                    } 
	                break;						
				}				
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior						
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==8){
						if(write_fecha()==1){
	                        flujo_LCD=14;                         
							if(version==1){
		                    	set_imagen(1,93);                     
							}
							else if(version==2){
								set_imagen(1,112);
							}
						}
                    }
                }

            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			
		break;
		
		case 23:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=12){
                    if(LCD_1_rxBuffer[3]<=9){ 
                        teclas1++;						
						switch(teclado){
							case 0:
								rventa.nit[teclas1]=LCD_1_rxBuffer[3]+0x30;
							break;
							
							case 1:
								rventa.telefono[teclas1]=LCD_1_rxBuffer[3]+0x30;
							break;
						}
                        writemini_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1); 						
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            //Comando de 0    
                        teclas1++;						
						switch(teclado){
							case 0:
								rventa.nit[teclas1]=0x30;
							break;
							
							case 1:
								rventa.telefono[teclas1]=0x30;
							break;
						}						
                        writemini_LCD(1,0x30,teclas1); 						
                    }  
                    if(LCD_1_rxBuffer[3]==0x51){            //Comando de Coma
                        if(teclas1>=1){ 
                        	teclas1++;							
							switch(teclado){
								case 0:
									rventa.nit[teclas1]='.';
								break;
								
								case 1:
									rventa.telefono[teclas1]='.';
								break;
							}							
                            writemini_LCD(1,'.',teclas1);
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){				//Cancel
                    if(teclas1==0){
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
						leer_eeprom(47,12);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.telefono[x]=buffer_i2c[x];
						}
						leer_eeprom(32,15);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.nit[x]=buffer_i2c[x];
						}						
                    }
                    else{					
                        writemini_LCD(1,0x20,teclas1);
                        teclas1--;							
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){				//Enter
                    if(teclas1>=0){
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						
						switch(teclado){
							case 0:
		                        rventa.nit[0]=teclas1;  
								if(write_eeprom(32,rventa.nit)==0){
										set_imagen(1,85);
										flujo_LCD=100;
								}
							break;	
							case 1:
		                        rventa.telefono[0]=teclas1;  							
								if(write_eeprom(47,rventa.telefono)==0){
										set_imagen(1,85);
										flujo_LCD=100;
								}
							break;								
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
		
		case 24:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=23){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
						switch(teclado){
							case 0:                        		
                                if(nombreproducto == 1){
                                    producto1n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.nombre[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;
							
							case 1:                       			
                                if(nombreproducto == 1){
                                    producto2n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.direccion[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;

							case 2:                       			
                                if(nombreproducto == 1){
                                    producto3n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema1[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;								

							case 3:                       			
                                if(nombreproducto == 1){
                                    producto4n[teclas1]=LCD_1_rxBuffer[3]+0x30;                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema2[teclas1]=LCD_1_rxBuffer[3]+0x30;
                                }
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;									
							break;								
									
						}
                        writemini_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);                        
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]=0x30;
							break;
							
							case 1:
                       			rventa.direccion[teclas1]=0x30;									
							break;
								
							case 2:
                       			rventa.lema1[teclas1]=0x30;									
							break;								

							case 3:
                       			rventa.lema2[teclas1]=0x30;									
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]=0x30;									
							break;								
						}						
                        writemini_LCD(1,0x30,teclas1);                        
                    } 
                    if(LCD_1_rxBuffer[3]==0x67){                                        //Comando de #
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='#';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='#';									
							break;
								
							case 2:
                       			rventa.lema1[teclas1]='#';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='#';									
							break;	
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='#';									
							break;								
						}						
                        writemini_LCD(1,'#',teclas1);                        
                    } 
                    if(LCD_1_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='-';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='-';									
							break;

							case 2:
                       			rventa.lema1[teclas1]='-';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='-';									
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='-';									
							break;								
						}						
                        writemini_LCD(1,'-',teclas1);                        
                    } 
                    if(LCD_1_rxBuffer[3]==0x66){                                        //Comando de @
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='@';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='@';									
							break;

							case 2:
                       			rventa.lema1[teclas1]='@';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='@';									
							break;	
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='@';									
							break;								
						}						
                        writemini_LCD(1,'@',teclas1);                        
                    } 					
                    if(LCD_1_rxBuffer[3]==0x69){                                        //Comando de .
                        teclas1++;                        
						switch(teclado){
							case 0:
                        		rventa.nombre[teclas1]='.';
							break;
							
							case 1:
                       			rventa.direccion[teclas1]='.';									
							break;
							
							case 2:
                       			rventa.lema1[teclas1]='.';									
							break;								

							case 3:
                       			rventa.lema2[teclas1]='.';									
							break;	
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]='.';									
							break;								
						}						
                        writemini_LCD(1,'.',teclas1);                        
                    }	
                    if(LCD_1_rxBuffer[3]==0x68){                                        //Comando de ESPACIO
                        teclas1++;                        
						switch(teclado){
							case 0:
		                        if(nombreproducto == 1){
                                    producto1n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.nombre[teclas1]=' ';
                                }
                            break;
							
							case 1:
                                if(nombreproducto == 1){
                                    producto2n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.direccion[teclas1]=' ';
                                }
                       												
							break;

							case 2:
                                if(nombreproducto == 1){
                                    producto3n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema1[teclas1]=' ';
                                }
                       												
							break;								

							case 3:
                                if(nombreproducto == 1){
                                    producto4n[teclas1]=' ';                                    
                                }
                                if(nombreproducto == 0){
                                    rventa.lema2[teclas1]=' ';
                                }
                       												
							break;
								
							case 4:
                       			Buffer_LCD1.valor[teclas1]=' ';									
							break;								
						}						
                        writemini_LCD(1,' ',teclas1);                        
                    }					
                    if(LCD_1_rxBuffer[3]>=0x1B && LCD_1_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_1_rxBuffer[3]==letras[x]){
                                teclas1++;                            
								switch(teclado){
									case 0:
		                        		if(nombreproducto == 1){
                                            producto1n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.nombre[teclas1]=x+0x41;
                                        }
                                    break;
									
									case 1:                       			
                                        if(nombreproducto == 1){
                                         producto2n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.direccion[teclas1]=x+0x41;
                                        }
							        break;

									case 2:                       			
                                        if(nombreproducto == 1){
                                            producto3n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.lema1[teclas1]=x+0x41;
                                        }
							        break;								

									case 3:                       			
                                        if(nombreproducto == 1){
                                            producto4n[teclas1]=x+0x41;                                    
                                        }
                                        if(nombreproducto == 0){
                                            rventa.lema2[teclas1]=x+0x41;
                                        }
							        break;		
										
										
									case 4:
		                       			Buffer_LCD1.valor[teclas1]=x+0x41;									
									break;										
								}                                                                                     		                                						
                                writemini_LCD(1,(x+0x41),teclas1);                            
                            }
                        }
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){                                        			//Borrar - Cancelar
                    if(teclas1==0){                                                 			//Si no tiene nada pasa a pedir impresion
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
						leer_eeprom(64,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.direccion[x]=buffer_i2c[x];
						}
						leer_eeprom(128,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.lema1[x]=buffer_i2c[x];
						}	
						leer_eeprom(160,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.lema2[x]=buffer_i2c[x];
						}
						leer_eeprom(0,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.nombre[x]=buffer_i2c[x];
						}						
                    }
                    else{
                        writemini_LCD(1,0x20,teclas1);                        
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){                                        			//Enter para guardar
                    if(teclas1>=0){
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
						switch(teclado){
							case 0:
                                if(nombreproducto == 1){
                                    producto1n[0]=teclas1;
                                    write_eeprom(1130,producto1n);	//Guarda el nombre en la eeprom
									    
								    }
                                
                                if(nombreproducto == 0){
                                    rventa.nombre[0]=teclas1;
                                    if(write_eeprom(0,rventa.nombre)==0){							//Guarda el nombre en la eeprom
									set_imagen(1,85);
									flujo_LCD=100;
								}
                                }
								
							break;
								
							case 1:
                                if(nombreproducto == 1){
                                    producto2n[0]=teclas1;
                                    write_eeprom(1141,producto2n);	//Guarda el nombre en la eeprom
                                }
								if(nombreproducto == 0){    
								rventa.direccion[0]=teclas1;
								if(write_eeprom(64,rventa.direccion)==0){						//Guarda la direccion en la eeprom
									set_imagen(1,85);
									flujo_LCD=100;
								}
                                }
							break;	
								
							case 2:
                                if(nombreproducto == 1){
                                    producto3n[0]=teclas1;
                                    write_eeprom(1152,producto3n);	//Guarda el nombre en la eeprom
                                }
                                if(nombreproducto == 0){ 
								rventa.lema1[0]=teclas1;
								if(write_eeprom(128,rventa.lema1)==0){							//Guarda el lema 1 en la eeprom
									set_imagen(1,85);
									flujo_LCD=100;
								}
                                }
							break;	
								
							case 3:
                                if(nombreproducto == 1){
                                    producto4n[0]=teclas1;
                                    write_eeprom(1163,producto4n);	//Guarda el nombre en la eeprom
                                }
                                if(nombreproducto == 0){
								rventa.lema2[0]=teclas1;
								if(write_eeprom(160,rventa.lema2)==0){							//Guarda el lema 2 en la eeprom
									set_imagen(1,85);
									flujo_LCD=100;
								}
                                }
							break;	
								
							case 4:
								Buffer_LCD1.valor[0]=teclas1;
								set_imagen(1,57);
								flujo_LCD=36;
							break;									
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;

		case 25:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x5F:								 	 //Sin ID	                               
                      flujo_LCD=4;                          
                      set_imagen(1,5); 				
                    break;
                    
                    case 0x5E:  								//Con ID                                         
                      set_imagen(1,29);
                      flujo_LCD=11;
                    break;	

                    case 0x7E:									//ir a menu
					  set_imagen(1,0);	
                      flujo_LCD=0;     
                    break;					
					
                }					
            }
            CyDelay(70);            
            LCD_1_ClearRxBuffer();
         }		  
		break;
		
		case 26:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=6){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }  
                 /*   if(LCD_1_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas1>=1 && comas1==0){
                            teclas1++;
                            //Buffer_LCD1.valor[teclas1]=0x2C;
                            write_LCD(1,0x2C,teclas1);
                            comas1=1;
                        }
                    }  */                  
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1>=4){
                        flujo_LCD=6;                         
                        set_imagen(1,57);	
						Buffer_LCD1.valor[0]=teclas1;					
						if(cambiar_precio(lado.a.dir)!=0){ 	
							if(rventa.producto==corriente){
								write_eeprom(435,Buffer_LCD1.valor); //423							
							}
							if(rventa.producto==extra){
								write_eeprom(423,Buffer_LCD1.valor);	//429						
							}
							if(rventa.producto==diesel){
								write_eeprom(429,Buffer_LCD1.valor);	//435						
							}
                            if(rventa.producto==kero){
								write_eeprom(1000,Buffer_LCD1.valor);							
							}
	                        set_imagen(1,60);	
							CyDelay(500);
							flujo_LCD=14;
							if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}							
						}
						else{
							error_op(1);
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
		
        case 27:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:								 	 //Configurar Productos	                                                      
                      set_imagen(1,88); 
					  Grado_LCD(1,0x00,0x73,451); // primer producto 449
					  Grado_LCD(1,0x00,0xCF,453); // segundo producto			  
                      Grado_LCD(1,0x01,0x29,449); // tercer producto
                      Grado_LCD(1,0x01,0x7B,1006); //cuarto producto
                      flujo_LCD=28;   
                    break;
                    
                    case 0x80:  								 //Version de Digitos                      
                      set_imagen(1,6); 	
					  teclas1=0;
                      flujo_LCD=29;
                    break;
                    
                    case 0x81:  								 //PPU                      
                      set_imagen(1,109); 	
                      flujo_LCD=30;	
                    break;
                    
                    case 0x82:  								 //Nombre de productos                      
                      set_imagen(1,138); 	
                      flujo_LCD=42;	
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(1,0);
                      flujo_LCD=0;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;	
		
        case 28:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x7F:								 	 //Extra	                                                         
                      set_imagen(1,6); 
					  teclas1=0; 
					  rventa.producto=1;
                      flujo_LCD=31;
                    break;
                    
                    case 0x80:  								 //Corriente                      
                      set_imagen(1,6); 	 
					  teclas1=0;
					  rventa.producto=2;
                      flujo_LCD=31;
                    break;
                    
                    case 0x81:  								 //Diesel                      	
                      set_imagen(1,6); 	
					  teclas1=0;
					  rventa.producto=3;
                      flujo_LCD=31;
                    break;
                    
                    case 0x82:  								 //Otro producto                      	
                      set_imagen(1,6); 	
					  teclas1=0;
					  rventa.producto=4;
                      flujo_LCD=31;
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(1,0);
                      flujo_LCD=0;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			                     
        break;	
		
		case 29:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=4){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }  
					
                    if(LCD_1_rxBuffer[3]==0x51){            	//Comando de coma
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=',';
                        write_LCD(1,',',teclas1);
                    }					
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==5 && (Buffer_LCD1.valor[1]==0x35||Buffer_LCD1.valor[1]==0x36||Buffer_LCD1.valor[1]==0x37)){                        
                        set_imagen(1,57);	
						Buffer_LCD1.valor[0]=teclas1;
						if(write_eeprom(442,Buffer_LCD1.valor)==1){	
							versurt=(Buffer_LCD1.valor[1]&0x0F);
						}
						if(write_eeprom(602,Buffer_LCD1.valor)==1){	
							decimalD=(Buffer_LCD1.valor[3]&0x0F);
							decimalV=(Buffer_LCD1.valor[5]&0x0F);
							set_imagen(1,60);
							CyDelay(500);
							flujo_LCD=14;
							if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}							
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
		
		case 30:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x8F:								 	 //PPUx1                              
						rventa.configppu[0]=0;
						rventa.configppu[1]=0;
						if(write_eeprom(444,rventa.configppu)==1){
							ppux10=0;
	                        set_imagen(1,60);	
							CyDelay(500);
							if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}	
							flujo_LCD=14;						
						}
						else{
							error_op(1);
						}
                    break;
                    
                    case 0x90:  								 //PPUx10
						rventa.configppu[0]=1;
						rventa.configppu[1]=1;
						if(write_eeprom(444,rventa.configppu)==1){
							ppux10=1;
	                        set_imagen(1,60);	
							CyDelay(500);
							if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}	
							flujo_LCD=14;						
						}
						else{
							error_op(2);
						}
                    break;               
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(1,112);
                      flujo_LCD=14;     //Volver a otras opciones
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }			
		break;
		
		case 31:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=0){
                    if(LCD_1_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                    }                    
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(1,0);
                        flujo_LCD=0;
                    }
                    else{
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas1=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1==1){                        
                        set_imagen(1,57);	
						Buffer_LCD1.valor[0]=teclas1;
						if(rventa.producto==1){
							extra=Buffer_LCD1.valor[1];
							write_eeprom(449,Buffer_LCD1.valor);							
						}
						if(rventa.producto==2){
							corriente=Buffer_LCD1.valor[1];
							write_eeprom(451,Buffer_LCD1.valor);							
						}
						if(rventa.producto==3){
							diesel=Buffer_LCD1.valor[1];
							write_eeprom(453,Buffer_LCD1.valor);							
						}
                        if(rventa.producto==4){
							kero=Buffer_LCD1.valor[1];
							write_eeprom(1006,Buffer_LCD1.valor);							
						}
						CyDelay(500);
						flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		
		break;
		
		case 32:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x84:								 	//Biomax	
					  bandera[0]=1;
					  bandera[1]=0;                        
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;
                    
                    case 0x86:  								//Terpel
					  bandera[0]=1;
					  bandera[1]=11;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  };
                    break;
                    
                    case 0x88:  								//ESSO
					  bandera[0]=1;
					  bandera[1]=4;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;
                    
                    case 0x89:                         			//Texaco                     
					  bandera[0]=1;
					  bandera[1]=12;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;   

                    case 0x8B:                         			//Petrobras                    
					  bandera[0]=1;
					  bandera[1]=9;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break; 
					
                    case 0x8A:                         			//Mobil                    
					  bandera[0]=1;
					  bandera[1]=8;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break; 	
					
                    case 0x85:                         			//Brio                    
					  bandera[0]=1;
					  bandera[1]=1;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break; 
					
                    case 0x87:                         			//Gulf                   
					  bandera[0]=1;
					  bandera[1]=6;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break; 	
					
                    case 0x98:                         			//Petromil                 
					  bandera[0]=1;
					  bandera[1]=14;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break; 
					
                    case 0x99:                         			//Zeus                
					  bandera[0]=1;
					  bandera[1]=13;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break; 					
					
                    case 0xA0:                         			//Ecospetrol                
					  bandera[0]=1;
					  bandera[1]=3;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;
					
                    case 0xA1:                         			//Exito               
					  bandera[0]=1;
					  bandera[1]=5;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;
					
                    case 0xA2:                         			//Mineroil               
					  bandera[0]=1;
					  bandera[1]=7;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;
					
                    case 0xA3:                         			//Plus               
					  bandera[0]=1;
					  bandera[1]=10;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;	
					
                    case 0x97:                         			//Cencosur               
					  bandera[0]=1;
					  bandera[1]=2;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;
					
                    case 0x94:                         			//Sin Bandera               
					  bandera[0]=0;
					  bandera[1]=0;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(1,60); 
					  	CyDelay(500);
					  	flujo_LCD=14;
					  	if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }
                    break;					
					
                    case 0x7E:									//ir a menu
					  bandera[0]=0;
					  bandera[1]=0;
					  if(write_eeprom(455,bandera)==1){                               	 
                        flujo_LCD=14;
						if(version==1){
                        	set_imagen(1,93);
						}
						else{
							set_imagen(1,112);
						}						  
					  }	
					  else{
						set_imagen(1,0);
	                	flujo_LCD=0;
					  }
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }					
		break;	

		case 33:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(LCD_1_rxBuffer[3] == 0x0A)
                {
                    imprimir(print1[1],producto1,1,lado.a.dir);
                    CyDelay(500);
                    set_imagen(1,12);
					flujo_LCD=100;
				    isr_3_StartEx(animacion);   //Si confirman copia imprime
				    Timer_Animacion_Start();    
                    
                }
                else{
                    set_imagen(1,12);
					flujo_LCD=100;
				    isr_3_StartEx(animacion);  // Si presionan NO o cualquier otro touch
				    Timer_Animacion_Start();
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }	
		 if(count_protector>=20){
            set_imagen(1,12);
            flujo_LCD=100; 
		    count_protector=0;
		 }		 
		break;
		
        case 34:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x38:								//No Corte
                      flujo_LCD=0;     
					  set_imagen(1,0);	
                    break; 

                    case 0x39:								//Si Corte
                      flujo_LCD=0;                   
                      set_imagen(1,59);
					  imprimir_corte(print1[1]);	
					  write_psoc1(print1[1],10);
					  write_psoc1(print1[1],10);
					  write_psoc1(print1[1],10);
					  write_psoc1(print1[1],0x1D);
					  write_psoc1(print1[1],0x56);
					  write_psoc1(print1[1],0x31);					
					  flujo_LCD=100;
					  count_protector=1;
				      isr_3_StartEx(animacion);  
				      Timer_Animacion_Start(); 						
                    break;
					
                    case 0x94:								//cambiar clave corte
					  set_imagen(1,47);
					  teclas1=0;
                      flujo_LCD=40;     
                    break;				
					
                    case 0x7E:								//ir a menu
					  set_imagen(1,0);	
                      flujo_LCD=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
		
        case 35:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x91:								//Placa                          
					  set_imagen(1,10);	
					  pos_ibutton=1;
					  teclado=4;
					  teclas1=0;	
                      flujo_LCD=24; 
                    break; 

                    case 0x92:								//Cuenta                         
					  set_imagen(1,111);
					  pos_ibutton=0x21;
					  teclado=4;
					  teclas1=0;					
                      flujo_LCD=24;  
                    break;					
					
                    case 0x7E:								//ir a menu
					  set_imagen(1,0);	
                      flujo_LCD=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
		
        case 36:  
			if(touch_write_memoria(1,Buffer_LCD1.valor[0],pos_ibutton,Buffer_LCD1.valor)==1){
				set_imagen(1,110);
				flujo_LCD=35;
				if(touch_read_memoria(1,1)>=6){
					CyDelay(10);
					Placa_LCD(1);
					if(touch_read_memoria(1,0x21)>=6){						
						Cuenta_LCD(1);
					}
				}	
				else{
					error_op(1);
				}	
			} 
			else{
				error_op(1);
			}
        break;
			
        case 37:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x39:								//SI
					  if(tipo_imp[1] == 1){
					        print1[0]=1;
					        print1[1]=1;
					        print2[0]=1;
					        print2[1]=2;
                        }
                        if(tipo_imp[1] == 2){
					        print1[0]=1;
					        print1[1]=3;
					        print2[0]=1;
					        print2[1]=4;
                        }
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,104); 
						  	CyDelay(500);
						  	flujo_LCD=14;
						  	if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}
						}	
					  }
                    break; 

                    case 0x38:								//NO
                      flujo_LCD=38;     
					  set_imagen(1,103);					
                    break;					
					
                    case 0x7E:								//ir a menu
                      if(copia_recibo[1]==1){
						copia_recibo[1]=0;
					  }	
					  else{
						copia_recibo[1]=1;
					  }
					  write_eeprom(986,copia_recibo);
					  set_imagen(1,60);
					  CyDelay(500);
					  set_imagen(1,0);	
                      flujo_LCD=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
		
        case 38:  
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){                               
                    case 0x8C:								//La impresora 1
                        if(tipo_imp[1] == 1){
					        print1[0]=1;
					        print1[1]=2;
					        print2[0]=1;
					        print2[1]=2;
                        }
                        if(tipo_imp[1] == 2){
					        print1[0]=1;
					        print1[1]=4;
					        print2[0]=1;
					        print2[1]=4;
                        }
                        
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,106); 
						  	CyDelay(500);
						  	flujo_LCD=14;
						  	if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}
						}	
					  }
                    break; 

                    case 0x8D:								//La impresora 2
                    
                        if(tipo_imp[1] ==1){   //Kiosko
					        print1[0]=1;
					        print1[1]=1;
					        print2[0]=1;
					        print2[1]=1;					
                        }
                        if (tipo_imp[1] ==2){ //Panel
                            print1[0]=1;
					        print1[1]=3;
					        print2[0]=1;
					        print2[1]=3;
                        }
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,105); 
						  	CyDelay(500);
						    if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}
					     flujo_LCD=14;
						}	
					  }					
                    break;
					
                    case 0x8E:								//Ninguna
					  print1[0]=1;
					  print1[1]=0;
					  print2[0]=1;
					  print2[1]=0;					
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(1,108); 
						  	CyDelay(500);
						  	flujo_LCD=14;
						  	if(version==1){
	                        	set_imagen(1,93);
							}
							else{
								set_imagen(1,112);
							}
						}	
					  }				
                    break;					
					
                    case 0x7E:								//ir a menu
					  set_imagen(1,0);	
                      flujo_LCD=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_1_ClearRxBuffer();
         }       
        break;
		
        case 39: 
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=3){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD1.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,'*',teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD1.password[teclas1]=0x30;
                        write_LCD(1,'*',teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						error_op(1);						
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas1>=1){
						y=0;
						for(x=0;x<teclas1;x++){						
							if(pasword_corte[x+1]==Buffer_LCD1.password[x]){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==pasword_corte[0]){								
	                        set_imagen(1,38);
							CyDelay(500);
	                        flujo_LCD=34;
							set_imagen(1,113);							
						}
						else{
							y=0;
							for(x=0;x<teclas1;x++){						
								if(puk[x]==Buffer_LCD1.password[x]){
									y++;
								}
							}
							if(y==4){								
								set_imagen(1,38);
								CyDelay(500);
								flujo_LCD=34;
								set_imagen(1,113);							
							}	
							else{		
								set_imagen(1,39);
								flujo_LCD=100;
								count_protector=1;
								isr_3_StartEx(animacion);  
								Timer_Animacion_Start();
							}		
						}
                    }
					else{
                        set_imagen(1,39);
                        flujo_LCD=100;
					 	count_protector=1;
			         	isr_3_StartEx(animacion);  
			         	Timer_Animacion_Start();					
					}					
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }		  
        break;
		
        case 40:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                if(teclas1<=3){
                    if(LCD_1_rxBuffer[3]<=9){
                        Buffer_LCD1.password[teclas1]=LCD_1_rxBuffer[3]+0x30;
                        write_LCD(1,(LCD_1_rxBuffer[3]+0x30),teclas1);
						teclas1++;
                    }
                    if(LCD_1_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD1.password[teclas1]=0x30;
                        write_LCD(1,0x30,teclas1);
                        teclas1++;						
                    }                     
                }
                if(LCD_1_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado regresa al inicio
						error_op(1);						
                    }
                    else{
                        teclas1--;						
                        write_LCD(1,0x20,(teclas1));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_1_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			   
					if(teclas1==4){
						set_imagen(1,57);	
	                    flujo_LCD=0;						
						pasword_corte[0]=teclas1;
						for(x=0;x<teclas1;x++){
							pasword_corte[x+1]=(Buffer_LCD1.password[x]);
						}
						if(write_eeprom(671,pasword_corte)==0){							//Guarda password en la eeprom
							set_imagen(1,85);
							flujo_LCD=100;
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }    
        break;
        
        case 41:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){ 
                    
                    case 0xBC:								//Impresora Kiosko
					    tipo_imp[0] = 1;
                        tipo_imp[1] = 1;

                        write_eeprom(1012,tipo_imp);
                        set_imagen(1,112);	
                        flujo_LCD=14;
                    break; 

                    case 0xBD:								//Impresora panel
                        tipo_imp[0] = 1;
                        tipo_imp[1] = 2; 
                         write_eeprom(1012,tipo_imp);
                        set_imagen(1,112);	
                        flujo_LCD=14;
                    break;
                     
                    }
                }
            
            CyDelay(100);            
            LCD_1_ClearRxBuffer();
         }    
        break;
        
        case 42:
         if(LCD_1_GetRxBufferSize()==8){
            if((LCD_1_rxBuffer[0]==0xAA) && (LCD_1_rxBuffer[6]==0xC3) && (LCD_1_rxBuffer[7]==0x3C)){
                switch(LCD_1_rxBuffer[3]){
                    case 0x76:								 	 //Cambiar Nombre producto 1	             
                      set_imagen(1,48);
					  teclado=0;
                      teclas1=0;                            	 //Inicia el contador de teclas                         
                      nombreproducto = 1;
                      flujo_LCD=24;
                    break;
					
                    case 0x82:								 	 //Cambiar Nombre producto 2	
                      set_imagen(1,48);                       
					  teclado=1;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 1;
                      flujo_LCD=24;  					
                    break;
					
                    case 0x83:								 	 //Cambiar Nombre producto 3	             
                      set_imagen(1,48);
					  teclado=2;
                      teclas1=0;                            	 //Inicia el contador de teclas 
                      nombreproducto = 1;
                      flujo_LCD=24;
                    break;					
                    
                    case 0x77:  								//Cambiar Nombre producto 4                     
					  set_imagen(1,48);
					  teclado=3;
                      teclas1=0;                            	 //Inicia el contador de teclas
                      nombreproducto = 1;
                      flujo_LCD=24;
                    break;
                }
            }
                
    }
        break;
    }
}
 
        

/*
*********************************************************************************************************
*                                         void polling_LCD2(void)
*
* Description : Ejecuta las diferentes funciones que se pueden realizar desde la pantalla 2
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

void polling_LCD2(void){  
    switch(flujo_LCD2){
         case 100:  
         if(count_protector2>=2){
            flujo_LCD2=0;
            isr_4_Stop(); 
            Timer_Animacion2_Stop(); 
            count_protector2=0;
         }            
        break;
        
        case 0:			
         isr_4_StartEx(animacion2); 
         Timer_Animacion2_Start();
         count_protector2=0;  
         flujo_LCD2=1;		 
        break;
        
        case 1:
         if(LCD_2_GetRxBufferSize()==8){             
             LCD_2_ClearRxBuffer(); 
             isr_4_Stop(); 
             Timer_Animacion2_Stop();
			 if(version==2){			
            	set_imagen(2,99);//GRP700
			 }
			 else{
            	set_imagen(2,69);			 	
			 }             
		     CyDelay(100);
             LCD_2_ClearRxBuffer();
			 flujo_LCD2=3;
         }
        break;
        
//        case 2: 
//        	
//        break;
        
        case 3:  
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x5C:
					  Buffer_LCD2.preset=0;	
					  if(version==1){	
                          set_imagen(2,5);
	                      flujo_LCD2=4;            				//Venta 550	                      
					  }
					  if(version==2){	
                          set_imagen(2,73);
	                      flujo_LCD2=25;            				//Venta 700	                      
					  }					
                    break;                
                    
                    case 0x45:								    //Otras opciones
                      set_imagen(2,37);
                      teclas2=0;  
                      flujo_LCD2=15;                           				  	
                    break; 

                    case 0x59:								//Corte
                      set_imagen(2,37);	
                      teclas2=0;  
                      flujo_LCD2=39; 					                        						
                    break;					
					
                    case 0x7E:								//ir a menu
					  set_imagen(2,0);	
                      flujo_LCD2=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
         }       
        break;
        
        case 4:   
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
				Buffer_LCD2.preset&=0xFC;
                switch(LCD_2_rxBuffer[3]){
                    case 0x0F:								 	 //Preset por dinero	                               
                      set_imagen(2,6);
                      teclas2=0;        //Inicia el contador de teclas  
                      write_LCD(2,'$',0);
                      Buffer_LCD2.preset|=2;
                      flujo_LCD2=5;                                                                           	                                               					  
                    break;
                    
                    case 0x10:  								//Preset por volumen
                      set_imagen(2,13);                                        
                      teclas2=0;                                //Inicia el contador de teclas                      
                      comas2=0;
                      write_LCD(2,'G',0);
					  Buffer_LCD2.preset|=1;
                      flujo_LCD2=5; 
                    break;
                    
                    case 0x43:  								//Full
                      set_imagen(2,7);  
                      flujo_LCD2=6;                                                               
                    break;
                    
                    case 0x3B:                         			//Cancel                     
					  if(version==2){			
		            	 set_imagen(2,99);
					  }
					  else{
		            	 set_imagen(2,69);			 	
					  }
					  flujo_LCD2=3;
                    break; 
					
                    case 0x7E:									//ir a menu
					  set_imagen(2,0);
                      flujo_LCD2=0;     
                    break;					
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
        break;
        
        case 5:   
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=(versurt-1)){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD2.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD2.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }  
                    if(LCD_2_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas2>=1 && comas2==0){
                            teclas2++;
                            Buffer_LCD2.valor[teclas2]=0x2C;
                            write_LCD(2,0x2C,teclas2);
                            comas2=1;
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(2,5);
                        flujo_LCD2=4;
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD2.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2>=1){
						set_imagen(2,7);
                        Buffer_LCD2.valor[0]=teclas2;
                        flujo_LCD2=6;                                                 					
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }          
        break;
        
        case 6:
		 CyDelay(100);
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(LCD_2_rxBuffer[3]==0x7E){					//Cancel
                    set_imagen(2,0);					//Cancel
					flujo_LCD2=0;
                }												
			}
            CyDelay(100);            
            LCD_2_ClearRxBuffer();	
			break;
		 }		
         if(get_estado(lado.b.dir)==7){
			CyDelay(50);
			producto2=estado_ex(lado.b.dir);
			CyDelay(50);		
			if((Buffer_LCD2.preset&0x02)==0x02||(Buffer_LCD2.preset&0x01)==0x01){
				if(programar(lado.b.dir,producto2,Buffer_LCD2.valor,(Buffer_LCD2.preset&0x03))==0){
					set_imagen(2,0);
					flujo_LCD2=0;
					break;
				}					
			}
			SDown_Write(1);
			producto2=estado_ex(lado.b.dir);
			if(producto2!=0){
				if(producto2==(extra&0x0f)||producto2==(corriente&0x0f)||producto2==(diesel&0x0f)||producto2==(kero&0x0f)){
					sonido(2);
				}
				Surtidor_PutChar(0x10|lado.b.dir);
			    set_imagen(2,8);
                flujo_LCD2=7;
																
			}
         }  	
        break;
        
        case 7:
		 CyDelay(50);
		 switch(get_estado(lado.b.dir)){
	         case 0x0B:                     //Termino venta
				CyDelay(100);
				if(venta(lado.b.dir)==1){	
		            flujo_LCD2=8;
				}
			 break;	
				
	         case 0x0A:                     //Termino venta
				if(venta(lado.b.dir)==1){	
		            flujo_LCD2=8;
				}
			 break;
                
	         case 0x06:                     
                set_imagen(2,0); //No hizo venta
				flujo_LCD2=0;
			 break;				
			 	
         }		
        break;
        
        case 8:
         switch(version){                     		  
            case 1:										//Si es 550 pasa a imprimir
             set_imagen(2,11);             
	         isr_4_StartEx(animacion2); 
	         Timer_Animacion2_Start();
	         count_protector2=0;	
             flujo_LCD2=10;
            break;
            
            case 2:                                	 	//pasa a pedir placa
			 if((Buffer_LCD2.preset&0x04)!=0x04){				
	            set_imagen(2,11);	
		        isr_4_StartEx(animacion2); 
		        Timer_Animacion2_Start();
		        count_protector2=0; 	          		        
                flujo_LCD2=10;
			 }
//			 else{
//                set_imagen(2,11);
//                flujo_LCD2=10;
//				Buffer_LCD2.posventa=0;	
//		        isr_4_StartEx(animacion2); 
//		        Timer_Animacion2_Start();
//		        count_protector2=0;				
//			 }
            break;            
         } 
        break;
        
        case 9:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
				count_protector2=0;
                if(teclas2<=6){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD2.placa[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);                        
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas2++;                        
                        Buffer_LCD2.placa[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);                        
                    }  
                    if(LCD_2_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas2++;                        
                        Buffer_LCD2.placa[teclas2]='-';
                        write_LCD(2,'-',teclas2);                        
                    } 					
                    if(LCD_2_rxBuffer[3]>=0x1B && LCD_2_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_2_rxBuffer[3]==letras[x]){
                                teclas2++;                            
                                Buffer_LCD2.placa[teclas2]=x+0x41;
                                write_LCD(2,(x+0x41),teclas2);                            
                            }
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){                                        //Borrar - Cancelar
                    if(teclas2==0){                                                 //Si no tiene nada pasa a pedir impresion
                        set_imagen(2,11);
                        Buffer_LCD2.placa[0]=0;
						Buffer_LCD2.posventa=0;
                        flujo_LCD2=10;
                    }
                    else{
                        write_LCD(2,0x20,teclas2);                        
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){                                        //Enter pasa a imprimir
                    if(teclas2>=1){                        
                        Buffer_LCD2.placa[0]=teclas2;                                                
                        Buffer_LCD2.posventa=1;
                        flujo_LCD2=13;
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }
		 if(count_protector2>=30){
            set_imagen(2,11);            
            Buffer_LCD2.placa[0]=0;
			Buffer_LCD2.posventa=0;	
			count_protector2=0;
            flujo_LCD2=10;
		 }
        break;

        case 10:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x39:                          //Si Imprime pasar a placa
                      set_imagen(2,10);
                      teclas2 = 0;
                      flujo_LCD2=9; 
                    break; 
                    
                    case 0x38:                          //No Imprime 
                      set_imagen(2,12);                      
				      count_protector2=1;
				      isr_4_StartEx(animacion2);  
				      Timer_Animacion2_Start();		
                      flujo_LCD2=100; 
                    break;                     
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
		 if(count_protector2>=30){
            set_imagen(2,12);             
		    count_protector2=1;
            flujo_LCD2=100;
		 }		
        break;  
        
        case 11:
		if(touch_read_memoria(2,1)>=6){
			CyDelay(10);
			if(touch_read_memoria(2,0x21)>=6){
		        if(touch_present(2)==1){
		            LCD_2_PutChar(0x33);
		            if(touch_write(2,0x33)){
		                for(z=0;z<=7;z++){
		                    Buffer_LCD2.id[z]=touch_read_byte(2);
		                }				
                        set_imagen(2,19);
						Buffer_LCD2.preset|=0x04;						 
						CyDelay(500);
                        set_imagen(2,14);
						teclas2=0;
						Buffer_LCD2.km[0]=0;						
						flujo_LCD2=12;
					}
				}	
			}
		}
	    if(LCD_2_GetRxBufferSize()==8){
	        if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
	            switch(LCD_2_rxBuffer[3]){
	                case 0x3B:
                     set_imagen(2,99); //Kilometraje
	                 flujo_LCD2=3;	                   
	                break; 
	            }
	        }
	        CyDelay(100);            
	        LCD_2_ClearRxBuffer();
	    }
        break;
        
        case 12:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=5){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;                    
                        Buffer_LCD2.km[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            //Comando de 0
                        teclas2++;                    
                        Buffer_LCD2.km[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){				//Cancelar
                    if(teclas2==0){
                        set_imagen(2,5);
						flujo_LCD2=4;
                    }
                    else{
                        write_LCD(2,0x20,teclas2);
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){				//Enter
                    if(teclas2>=1 && Buffer_LCD2.km[1]!=0x30){
                        set_imagen(2,5);                        
                        Buffer_LCD2.km[0]=teclas2;                         
                        flujo_LCD2=4;
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
        break;
        
        case 13:
			imprimir(print2[1], producto2,0,lado.b.dir);
			if(version==1||(copia_recibo[1]==0)){
				set_imagen(2,12);				
				count_protector2=1;
			    isr_4_StartEx(animacion2);  
			    Timer_Animacion2_Start();
                flujo_LCD2=100;
			}
			else{
				set_imagen(2,49);					
				teclas2=0;
				count_protector2=1;
			    isr_4_StartEx(animacion2);  
			    Timer_Animacion2_Start();	
                flujo_LCD2=33;
			}
        break;  
        
		case 14:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x58:								 	 //Configurar Fecha y Hora	                                                                           
                      teclas2=0;                            	 //Inicia el contador de teclas  
                      set_imagen(2,62); 
					  if(leer_hora()==1){
						Hora_LCD(2);
					  } 
					  if(leer_fecha()==1){
						Fecha_LCD(2);						
					  }	
                    flujo_LCD2=16;   
                    break;
                    
                    case 0x5D:  								//Cambiar Precio                                         
                      set_imagen(2,92);
					  Precio_LCD(2,0x00,0x89,435);	
					  Precio_LCD(2,0x00,0xD6,429);	
					  Precio_LCD(2,0x01,0x24,423);	
                      Precio_LCD(2,0x01,0x78,1000); //cuarto producto 78
                      flujo_LCD2=19;
                      
                    break;
                    
                    case 0x5A:  								//Datos de Recibo                      			
                      set_imagen(2,96);
					  Nombre_LCD(2);
					  direccion_LCD(2);
					  telefono_LCD(2);
					  nit_LCD(2);
					  lema1_LCD(2);
					  lema2_LCD(2);
                      flujo_LCD2=18;	
                    break;
                    
                    case 0x5B:                         			//Cambiar pasword                                           
                      set_imagen(2,47);
					  teclas2=0;
                      flujo_LCD2=17;
                    break;   					
					
                    case 0x96:                         			//Configurar Surtidor                                         
                      set_imagen(2,95);
					  teclas2=0;
                      flujo_LCD2=27;
                    break; 
					
                    case 0x65:                         			//Test                                           
                      set_imagen(2,77);
                      flujo_LCD2=20;
                    break;
					
                    case 0x93:                         			//Grabar Ibutton                                          
                      set_imagen(2,110);
					  if(touch_read_memoria(2,1)>=6){
						CyDelay(10);
						Placa_LCD(2);
						if(touch_read_memoria(2,0x21)>=6){						
							Cuenta_LCD(2);
						}
					  }
                     flujo_LCD2=35;
                    break;	
                    
                    case 0x70:                         			//Tipo de impresora                         
                        set_imagen(2,137);
                        flujo_LCD2 = 41;
					break;
                        
                        
                    case 0x7E:									//ir a menu
					  set_imagen(2,0);
                      flujo_LCD2=0;     
                    break;							
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		  
		break;
        
        case 15: 
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=7){
                    if(LCD_2_rxBuffer[3]<=9){
                        Buffer_LCD2.password[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,'*',teclas2);
						teclas2++;
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD2.password[teclas2]=0x30;
                        write_LCD(2,'*',teclas2);
                        teclas2++;						
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado regresa al inicio
						error_op(2);						
                    }
                    else{
                        teclas2--;						
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas2>=1){
						y=0;
						for(x=0;x<teclas2;x++){						
							if(rventa.password[x+1]==(Buffer_LCD2.password[x]-0x30)){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==rventa.password[0]){								
	                        set_imagen(2,38);
							CyDelay(500);
	                        flujo_LCD2=14;
							if(version==1){
		                    	set_imagen(2,93);                     
							}
							else if(version==2){
								set_imagen(2,112);
							}								
						}
						else{
							y=0;
							for(x=0;x<teclas2;x++){						
								if(puk[x]==Buffer_LCD2.password[x]){
									y++;
								}
							}
							if(y==8){								
								set_imagen(2,38);
								CyDelay(500);
								flujo_LCD2=14;
								if(version==1){
									set_imagen(2,93);                     
								}
								else if(version==2){
									set_imagen(2,112);
								}								
							}	
							else{		
								set_imagen(2,39);
								flujo_LCD2=100;
								count_protector2=1;
								isr_4_StartEx(animacion2);  
								Timer_Animacion2_Start();
							}		
						}
                    }
					else{
                        set_imagen(2,39);
                        flujo_LCD2=100;
					 	count_protector2=1;
			         	isr_4_StartEx(animacion2);  
			         	Timer_Animacion2_Start();					
					}					
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		  
        break; 
        
        case 16:   
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
					case 0x3F:								//Hora
                        set_imagen(2,64);
						teclas2=0;
						comas2=0;												
						write_LCD(2,'a',6);						
						write_LCD(2,'m',7);
                        flujo_LCD2=21;
					break;
					
					case 0x3E:								//Fecha
						set_imagen(2,63);
                        teclas2=0;
						comas2=0;
						flujo_LCD2=22;
											
					break;
						
                    case 0x7E:								//ir a menu
					  set_imagen(2,112);	
                      flujo_LCD2=14;     
                    break;						
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         } 
        break;
        
        case 17:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=7){
                    if(LCD_2_rxBuffer[3]<=9){
                        Buffer_LCD2.password[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
						teclas2++;
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD2.password[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                        teclas2++;						
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado regresa al inicio
						error_op(2);						
                    }
                    else{
                        teclas2--;						
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			   
					if(teclas2>=6){
						set_imagen(2,57);		                    
						rventa.password[0]=teclas2;
						for(x=0;x<teclas2;x++){
							rventa.password[x+1]=(Buffer_LCD2.password[x]-0x30);
						}
						if(write_eeprom(96,rventa.password)==0){							//Guarda el nombre en la eeprom
							set_imagen(2,85);
							flujo_LCD2=100;
						}
                        flujo_LCD2=0;
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }      
        break;
        
        case 18:
          if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x76:								 	 //Cambiar Nombre	                               
                      set_imagen(2,48); 	
					  teclado2=0;
                      teclas2=0;                            	 //Inicia el contador de teclas                       
                      flujo_LCD2=24;    
                    break;
					
                    case 0x82:								 	 //Cambiar Lema1	                               
                      set_imagen(2,121); 					    
					  teclado2=2;
                      teclas2=0;                            	 //Inicia el contador de teclas                       
                      flujo_LCD2=24;
                    break;
					
                    case 0x83:								 	 //Cambiar Lema2	             
                      set_imagen(2,122);
					  teclado2=3;
                      teclas2=0;                            	 //Inicia el contador de teclas                         					
                      flujo_LCD2=24;  
                    break;					
                    
                    case 0x77:  								//Cambiar Direccion                      
                      set_imagen(2,120);
					  teclado2=1;                      
                      teclas2=0;                            	 //Inicia el contador de teclas 
                      flujo_LCD2=24;
                    break;
                    
                    case 0x78:  
                      set_imagen(2,101);								//Cambiar telefono
                      teclado2=1;
                      teclas2=0;                            	 //Inicia el contador de teclas
                      flujo_LCD2=23;
                    break;
                    
                    case 0x7A:                         			//Cambiar NIT  
					  set_imagen(2,50);  
					  teclado2=0;
                      teclas2=0;                            	 //Inicia el contador de teclas                 
                      flujo_LCD2=23;
                    break;   

                    case 0x95:                         			//Cambiar Bandera      
						if(version==2){
						  set_imagen(2,102);
	                      flujo_LCD2=32;
						}
                    break; 
					
                    case 0x7E:									//ir a menu
					  set_imagen(2,112);
                      flujo_LCD2=14;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }	      
        break;
        
        case 19:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:								 	 //Extra	                                                        
                      teclas1=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=corriente2;
                      set_imagen(2,6); 	
					  write_LCD(2,'$',teclas1);	
                      flujo_LCD2=26; 
                    break;
                    
                    case 0x80:  								 //Corriente                      
                      teclas1=0;                            	 //Inicia el contador de teclas 
					  rventa.producto=diesel2;
                      set_imagen(2,6); 	
					  write_LCD(2,'$',teclas1);	      
                      flujo_LCD2=26; 
                    break;
                    
                    case 0x81:  								//Diesel                      
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=extra2;
                      set_imagen(2,6); 	
					  write_LCD(2,'$',teclas1);
                      flujo_LCD2=26; 
                    break;
                    
                    case 0x82:  								//Otro producto                      
                      teclas1=0;                            	//Inicia el contador de teclas 
					  rventa.producto=kero2;
                      set_imagen(2,6); 	
					  write_LCD(2,'$',teclas1);	
                      flujo_LCD2=26;	
                    break;
                   										
                    case 0x7E:									//ir a menu
					  set_imagen(2,0);
                      flujo_LCD2=0;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;
       
        case 20:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
				switch(LCD_2_rxBuffer[3]){
	                case 0x62:								 	 //Comunicacion Surtidores
						set_imagen(2,57);
						CyDelay(100);
						if(get_estado(lado.b.dir)==6){	
							if(get_estado(lado.b.dir)==6){
								set_imagen(2,74);
								CyDelay(700);				  
								flujo_LCD2=0; 
							}
							else{
								error_op(2);
							}
						}	
						else{
							error_op(2);
						}
	                break;
	                
	                case 0x63:  								 //Comunicacion Impresora
						set_imagen(2,55);
						if(tipo_imp[1] == 1){    //Impresora Kiosko
						    for(x=0;x<=16;x++){
			        		    write_psoc1(1,test[x]);			
						    }
					        write_psoc1(1,10);
						    write_psoc1(1,10);
						    write_psoc1(1,10);
						    write_psoc1(1,10);
						    for(x=0;x<=16;x++){
			        		    write_psoc1(2,test[x]);			
						    }
					        write_psoc1(2,10);
						    write_psoc1(2,10);
						    write_psoc1(2,10);
						    write_psoc1(2,10);						
                        }
                        if (tipo_imp[1] == 2){             //Impresora panel
                            for(x=0;x<=16;x++){
			        		    write_psoc1(3,test[x]);			
						    }
					        write_psoc1(3,10);
						    write_psoc1(3,10);
						    write_psoc1(3,10);
						    write_psoc1(3,10);
						    for(x=0;x<=16;x++){
			        		    write_psoc1(4,test[x]);			
						    }
					        write_psoc1(4,10);
						    write_psoc1(4,10);
						    write_psoc1(4,10);
					    	write_psoc1(4,10);
                        }						
						set_imagen(2,107);
						flujo_LCD2=37;
	                break;
	                
	                case 0x64:  								 //Comunicacion Ibutton
					if(touch_present(2)==1){
						LCD_2_PutChar(0x33);
						if(touch_write(2,0x33)){
							for(z=0;z<=7;z++){
							    Buffer_LCD2.id[z]=touch_read_byte(2);
							}
							crc_total=0;
							for(z=0;z<7;z++){
							    crc_total=crc_check(crc_total,Buffer_LCD2.id[z]);
							}	
							if(crc_total==Buffer_LCD2.id[7]){
								set_imagen(2,75);
								CyDelay(1000);				  
							   	flujo_LCD2=0;
							}
							else{
								error_op(2);
							} 						
						}
					}
					else{
						error_op(2);
					}
	                break;
	               										
	                case 0x7E:									//ir a menu
					  set_imagen(2,0);
	                  flujo_LCD2=0;     
	                break;	
				}
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }                    
        break;
        
        case 21:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
				switch(teclas2){
	                case 0:										//Decenas de la Hora
	                    if(LCD_2_rxBuffer[3]<=1)					
						{
		                    teclas2++;
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.hora[1]=LCD_2_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas2++;
	                        rventa.hora[1]=0;
	                        write_LCD(2,0x30,teclas2);
	                    }                     
	                break;
						
	                case 1:											//Unidades de la Hora
	                    if(LCD_2_rxBuffer[3]<=9)					
						{
		                    teclas2++;
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.hora[1]|=LCD_2_rxBuffer[3];						
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas2++;
	                        write_LCD(2,0x30,teclas2);
	                    }                     
	                break;
						
	                case 2:											//Decenas de la Minutos					
	                    if(LCD_2_rxBuffer[3]<=5)					
						{
							teclas2++;					
							write_LCD(2,':',teclas2);							
		                    teclas2++;
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.hora[0]=LCD_2_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
							teclas2++;					
							write_LCD(2,':',teclas2);							
	                        teclas2++;
	                        write_LCD(2,0x30,teclas2);
							rventa.hora[0]=0;
	                    } 
	                break;
						
	                case 4:											//Unidades de la Minutos					
	                    if(LCD_2_rxBuffer[3]<=9)					
						{
		                    teclas2++;
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.hora[0]|=LCD_2_rxBuffer[3];						
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
	                        teclas2++;
	                        write_LCD(2,0x30,teclas2);
	                    } 
	                break;
				}
                if(LCD_2_rxBuffer[3]==0x40){            		//Comando de Am/Pm
                    if(comas2==0){
                        comas2=1;
                        write_LCD(2,'p',6);
                    }
					else{
                        comas2=0;
                        write_LCD(2,'a',6);						
					}
					write_LCD(2,'m',7);
                } 				
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior						
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2==5){
						rventa.hora[1]|=((comas2|2)<<5);
						if(write_hora()==1){
	                        flujo_LCD2=14;                         
							if(version==1){
		                    	set_imagen(2,93);                     
							}
							else if(version==2){
								set_imagen(2,112);
							}
						}
                    }
                }

            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }        
        break;
		
		case 22:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
				switch(teclas2){
	                case 0:										//Decenas de dia
	                    if(LCD_2_rxBuffer[3]<=3)					
						{
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    teclas2++;							
		                    rventa.fecha[0]=LCD_2_rxBuffer[3]<<4;						
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
	                        rventa.fecha[0]=0;
	                        write_LCD(2,0x30,teclas2);
	                        teclas2++;							
	                    }                     
	                break;
						
	                case 1:											//Unidades de dia
	                    if(LCD_2_rxBuffer[3]<=9)					
						{
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.fecha[0]|=LCD_2_rxBuffer[3];	
		                    teclas2++;							
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(2,0x30,teclas2);
	                        teclas2++;							
	                    }                     
	                break;
						
	                case 2:											//Decenas de mes					
	                    if(LCD_2_rxBuffer[3]<=1)					
						{					
							write_LCD(2,'/',teclas2);
							teclas2++;							
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.fecha[1]=LCD_2_rxBuffer[3]<<4;
		                    teclas2++;							
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0;					
							write_LCD(2,'/',teclas2);
							teclas2++;							
	                        write_LCD(2,0x30,teclas2);
							rventa.fecha[1]=0;
	                        teclas2++;							
	                    } 
	                break;
						
	                case 4:											//Unidades de mes					
	                    if(LCD_2_rxBuffer[3]<=9)					
						{
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.fecha[1]|=LCD_2_rxBuffer[3];	
		                    teclas2++;							
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(2,0x30,teclas2);
	                        teclas2++;							
	                    } 
	                break;
	                case 5:											//Decenas de año					
	                    if(LCD_2_rxBuffer[3]<=9)					
						{					
							write_LCD(2,'/',teclas2);
							teclas2++;							
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.fecha[2]=LCD_2_rxBuffer[3]<<4;
		                    teclas2++;							
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0;					
							write_LCD(2,'/',teclas2);
							teclas2++;							
	                        write_LCD(2,0x30,teclas2);
							rventa.fecha[2]=0;
	                        teclas2++;							
	                    } 
	                break;
						
	                case 7:											//Unidades de año					
	                    if(LCD_2_rxBuffer[3]<=9)					
						{
		                    write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
		                    rventa.fecha[2]|=LCD_2_rxBuffer[3];	
		                    teclas2++;							
	                    }
	                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
	                        write_LCD(2,0x30,teclas2);
	                        teclas2++;							
	                    } 
	                break;						
				}				
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior						
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
                    }
                    else{
                        teclas2--;						
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2==8){
						if(write_fecha()==1){
	                        flujo_LCD2=14;                         
							if(version==1){
		                    	set_imagen(2,93);                     
							}
							else if(version==2){
								set_imagen(2,112);
							}
						}
                    }
                }

            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			
		break;
		
		case 23:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=12){
                    if(LCD_2_rxBuffer[3]<=9){ 
                        teclas2++;						
						switch(teclado2){
							case 0:
								rventa.nit[teclas2]=LCD_2_rxBuffer[3]+0x30;
							break;
							
							case 1:
								rventa.telefono[teclas2]=LCD_2_rxBuffer[3]+0x30;
							break;
						}
                        writemini_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2); 						
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            //Comando de 0    
                        teclas2++;						
						switch(teclado2){
							case 0:
								rventa.nit[teclas2]=0x30;
							break;
							
							case 1:
								rventa.telefono[teclas2]=0x30;
							break;
						}						
                        writemini_LCD(2,0x30,teclas2); 						
                    }  
                    if(LCD_2_rxBuffer[3]==0x51){            //Comando de Coma
                        if(teclas2>=1){ 
                        	teclas2++;							
							switch(teclado2){
								case 0:
									rventa.nit[teclas2]='.';
								break;
								
								case 1:
									rventa.telefono[teclas2]='.';
								break;
							}							
                            writemini_LCD(2,'.',teclas2);
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){				//Cancel
                    if(teclas2==0){
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
						leer_eeprom(47,12);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.telefono[x]=buffer_i2c[x];
						}
						leer_eeprom(32,15);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.nit[x]=buffer_i2c[x];
						}						
                    }
                    else{					
                        writemini_LCD(2,0x20,teclas2);
                        teclas2--;							
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){				//Enter
                    if(teclas2>=0){
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						
						switch(teclado2){
							case 0:
		                        rventa.nit[0]=teclas2;  
								if(write_eeprom(32,rventa.nit)==0){
										set_imagen(2,85);
										flujo_LCD2=100;
								}
							break;	
							case 1:
		                        rventa.telefono[0]=teclas2;  							
								if(write_eeprom(47,rventa.telefono)==0){
										set_imagen(2,85);
										flujo_LCD2=100;
								}
							break;								
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
		
		case 24:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=23){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
						switch(teclado2){
							case 0:
                        		rventa.nombre[teclas2]=LCD_2_rxBuffer[3]+0x30;
							break;
							
							case 1:
                       			rventa.direccion[teclas2]=LCD_2_rxBuffer[3]+0x30;									
							break;

							case 2:
                       			rventa.lema1[teclas2]=LCD_2_rxBuffer[3]+0x30;									
							break;								

							case 3:
                       			rventa.lema2[teclas2]=LCD_2_rxBuffer[3]+0x30;									
							break;
								
							case 4:
                       			Buffer_LCD2.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;									
							break;								
									
						}
                        writemini_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);                        
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){                                        //Comando de 0
                        teclas2++;                        
						switch(teclado2){
							case 0:
                        		rventa.nombre[teclas2]=0x30;
							break;
							
							case 1:
                       			rventa.direccion[teclas2]=0x30;									
							break;
								
							case 2:
                       			rventa.lema1[teclas2]=0x30;									
							break;								

							case 3:
                       			rventa.lema2[teclas2]=0x30;									
							break;
								
							case 4:
                       			Buffer_LCD2.valor[teclas2]=0x30;									
							break;								
						}						
                        writemini_LCD(2,0x30,teclas2);                        
                    } 
                    if(LCD_2_rxBuffer[3]==0x67){                                        //Comando de #
                        teclas2++;                        
						switch(teclado2){
							case 0:
                        		rventa.nombre[teclas2]='#';
							break;
							
							case 1:
                       			rventa.direccion[teclas2]='#';									
							break;
								
							case 2:
                       			rventa.lema1[teclas2]='#';									
							break;								

							case 3:
                       			rventa.lema2[teclas2]='#';									
							break;	
								
							case 4:
                       			Buffer_LCD2.valor[teclas2]='#';									
							break;								
						}						
                        writemini_LCD(2,'#',teclas2);                        
                    } 
                    if(LCD_2_rxBuffer[3]==0x6A){                                        //Comando de -
                        teclas2++;                        
						switch(teclado2){
							case 0:
                        		rventa.nombre[teclas2]='-';
							break;
							
							case 1:
                       			rventa.direccion[teclas2]='-';									
							break;

							case 2:
                       			rventa.lema1[teclas2]='-';									
							break;								

							case 3:
                       			rventa.lema2[teclas2]='-';									
							break;
								
							case 4:
                       			Buffer_LCD2.valor[teclas2]='-';									
							break;								
						}						
                        writemini_LCD(2,'-',teclas2);                        
                    } 
                    if(LCD_2_rxBuffer[3]==0x66){                                        //Comando de @
                        teclas2++;                        
						switch(teclado2){
							case 0:
                        		rventa.nombre[teclas2]='@';
							break;
							
							case 1:
                       			rventa.direccion[teclas2]='@';									
							break;

							case 2:
                       			rventa.lema1[teclas2]='@';									
							break;								

							case 3:
                       			rventa.lema2[teclas2]='@';									
							break;	
								
							case 4:
                       			Buffer_LCD2.valor[teclas2]='@';									
							break;								
						}						
                        writemini_LCD(2,'@',teclas2);                        
                    } 					
                    if(LCD_2_rxBuffer[3]==0x69){                                        //Comando de .
                        teclas2++;                        
						switch(teclado2){
							case 0:
                        		rventa.nombre[teclas2]='.';
							break;
							
							case 1:
                       			rventa.direccion[teclas2]='.';									
							break;
							
							case 2:
                       			rventa.lema1[teclas2]='.';									
							break;								

							case 3:
                       			rventa.lema2[teclas2]='.';									
							break;	
								
							case 4:
                       			Buffer_LCD2.valor[teclas2]='.';									
							break;								
						}						
                        writemini_LCD(2,'.',teclas2);                        
                    }	
                    if(LCD_2_rxBuffer[3]==0x68){                                        //Comando de ESPACIO
                        teclas2++;                        
						switch(teclado2){
							case 0:
                        		rventa.nombre[teclas2]=' ';
							break;
							
							case 1:
                       			rventa.direccion[teclas2]=' ';									
							break;

							case 2:
                       			rventa.lema1[teclas2]=' ';									
							break;								

							case 3:
                       			rventa.lema2[teclas2]=' ';									
							break;
								
							case 4:
                       			Buffer_LCD2.valor[teclas2]=' ';									
							break;								
						}						
                        writemini_LCD(2,' ',teclas2);                        
                    }					
                    if(LCD_2_rxBuffer[3]>=0x1B && LCD_2_rxBuffer[3]<=0x42){            //Comando de Letra
                        for(x=0;x<=25;x++){                                            //Compara el dato que llego con un vector que tiene todas las letras     
                            if(LCD_2_rxBuffer[3]==letras[x]){
                                teclas2++;                            
								switch(teclado2){
									case 0:
		                        		rventa.nombre[teclas2]=x+0x41;
									break;
									
									case 1:
		                       			rventa.direccion[teclas2]=x+0x41;									
									break;

									case 2:
		                       			rventa.lema1[teclas2]=x+0x41;									
									break;								

									case 3:
		                       			rventa.lema2[teclas2]=x+0x41;									
									break;		
										
										
									case 4:
		                       			Buffer_LCD2.valor[teclas2]=x+0x41;									
									break;										
								}								
                                writemini_LCD(2,(x+0x41),teclas2);                            
                            }
                        }
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){                                        			//Borrar - Cancelar
                    if(teclas2==0){                                                 			//Si no tiene nada pasa a pedir impresion
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
						leer_eeprom(64,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.direccion[x]=buffer_i2c[x];
						}
						leer_eeprom(128,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.lema1[x]=buffer_i2c[x];
						}	
						leer_eeprom(160,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.lema2[x]=buffer_i2c[x];
						}
						leer_eeprom(0,32);
						for(x=0;x<=buffer_i2c[0];x++){
							rventa.nombre[x]=buffer_i2c[x];
						}						
                    }
                    else{
                        writemini_LCD(2,0x20,teclas2);                        
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){                                        			//Enter para guardar
                    if(teclas2>=0){
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
						switch(teclado2){
							case 0:
								rventa.nombre[0]=teclas2;
								if(write_eeprom(0,rventa.nombre)==0){							//Guarda el nombre en la eeprom
									set_imagen(2,85);
									flujo_LCD2=100;
								}
							break;
								
							case 1:
								rventa.direccion[0]=teclas2;
								if(write_eeprom(64,rventa.direccion)==0){						//Guarda la direccion en la eeprom
									set_imagen(2,85);
									flujo_LCD2=100;
								}
							break;	
								
							case 2:
								rventa.lema1[0]=teclas2;
								if(write_eeprom(128,rventa.lema1)==0){							//Guarda el lema 1 en la eeprom
									set_imagen(2,85);
									flujo_LCD2=100;
								}
							break;	
								
							case 3:
								rventa.lema2[0]=teclas2;
								if(write_eeprom(160,rventa.lema2)==0){							//Guarda el lema 2 en la eeprom
									set_imagen(2,85);
									flujo_LCD2=100;
								}
							break;	
								
							case 4:
								Buffer_LCD2.valor[0]=teclas2;
								set_imagen(2,57);
								flujo_LCD2=36;
							break;									
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;

		case 25:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x5F:								 	 //Sin ID	                               
                      flujo_LCD2=4;                          
                      set_imagen(2,5); 				
                    break;
                    
                    case 0x5E:  								//Con ID
                      flujo_LCD2=11;                   
                      set_imagen(2,29);
                    break;	

                    case 0x7E:									//ir a menu
					  set_imagen(2,0);	
                      flujo_LCD2=0;     
                    break;					
					
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		  
		break;
		
		case 26:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas1<=6){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas1);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas1++;
                        Buffer_LCD1.valor[teclas1]=0x30;
                        write_LCD(2,0x30,teclas1);
                    }  
                    /*if(LCD_2_rxBuffer[3]==0x51){            	//Comando de Coma
                        if(teclas1>=1 && comas2==0){
                            teclas1++;
                            Buffer_LCD1.valor[teclas1]=0x2C;
                            write_LCD(2,0x2C,teclas1);
                            comas2=1;
                        }
                    } */                   
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas1==0){								//Si no ha presionado nada regresa al menu anterior
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
                    }
                    else{
                        write_LCD(2,0x20,(teclas1));			//Si ya presiono borra el dato	
                        if(Buffer_LCD1.valor[teclas1]==0x2C){
                            comas2=0;
                        }
                        teclas1--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas1>=4){
                        flujo_LCD2=6;                         
                        set_imagen(2,57);	
						Buffer_LCD1.valor[0]=teclas1;					
						if(cambiar_precio(lado.b.dir)!=0){ 	
							if(rventa.producto==corriente2){
								write_eeprom(435,Buffer_LCD1.valor);//423							
							}
							if(rventa.producto==extra2){
								write_eeprom(423,Buffer_LCD1.valor); //429							
							}
							if(rventa.producto==diesel2){
								write_eeprom(429,Buffer_LCD1.valor); //435							
							}
                            if(rventa.producto==kero2){
								write_eeprom(1000,Buffer_LCD1.valor);							
							}
	                        set_imagen(2,60);	
							CyDelay(500);
							flujo_LCD2=14;
							if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}							
						}
						else{
							error_op(2);
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
		
        case 27:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:								 	 //Configurar Productos	                                                         
                      set_imagen(2,88); 
                      Grado_LCD(2,0x00,0x73,636);//636 primer producto
                      Grado_LCD(2,0x00,0xCF,638);//638 segundo producto
					  Grado_LCD(2,0x01,0x29,634);//634 tercer producto					  					  
                      Grado_LCD(2,0x01,0x7B,1008);//cuarto producto
                      flujo_LCD2=28;
                    break;
                    
                    case 0x80:  								 //Version de Digitos                      
                      set_imagen(2,6); 	
					  teclas2=0;
                      flujo_LCD2=29;
                    break;
                    
                    case 0x81:  								 //PPU                      	
                      set_imagen(2,109); 	
                      flujo_LCD2=30;
                    break;
                    
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(2,0);
                      flujo_LCD2=0;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;	
		
        case 28:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x7F:								 	 //Extra	                                                         
                      set_imagen(2,6); 
					  teclas2=0; 
					  rventa.producto=1;
                      flujo_LCD2=31;
                    break;
                    
                    case 0x80:  								 //Corriente                      
                      set_imagen(2,6); 	 
					  teclas2=0;
					  rventa.producto=2;
                      flujo_LCD2=31;
                    break;
                    
                    case 0x81:  								 //Diesel                      	
                      set_imagen(2,6); 	
					  teclas2=0;
					  rventa.producto=3;
                      flujo_LCD2=31;
                    break;
                    case 0x82:  								 //Otro producto                      	
                      set_imagen(2,6); 	
					  teclas2=0;
					  rventa.producto=4;
                      flujo_LCD2=31;
                    break;
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(2,0);
                      flujo_LCD2=0;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }			                     
        break;	
		
		case 29:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=4){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD2.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD2.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }  
					
                    if(LCD_2_rxBuffer[3]==0x51){            	//Comando de coma
                        teclas2++;
                        Buffer_LCD2.valor[teclas2]=',';
                        write_LCD(2,',',teclas2);
                    }					
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD2.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2==5 && (Buffer_LCD2.valor[1]==0x35||Buffer_LCD2.valor[1]==0x36||Buffer_LCD2.valor[1]==0x37)){                        
                        set_imagen(2,57);	
						Buffer_LCD2.valor[0]=teclas2;
						if(write_eeprom(442,Buffer_LCD2.valor)==1){	
							versurt=(Buffer_LCD2.valor[1]&0x0F);
						}
						if(write_eeprom(602,Buffer_LCD2.valor)==1){	
							decimalD=(Buffer_LCD2.valor[3]&0x0F);
							decimalV=(Buffer_LCD2.valor[5]&0x0F);
							set_imagen(2,60);
							CyDelay(500);
							flujo_LCD2=14;
							if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}							
						}						
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
		
		case 30:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x8F:								 	 //PPUx1                              
						rventa.configppu[0]=0;
						rventa.configppu[1]=0;
						if(write_eeprom(444,rventa.configppu)==1){
							ppux10=0;
	                        set_imagen(2,60);	
							CyDelay(500);
							flujo_LCD2=14;
							if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}							
						}
						else{
							error_op(2);
						}
                    break;
                    
                    case 0x90:  								 //PPUx10
						rventa.configppu[0]=1;
						rventa.configppu[1]=1;
						if(write_eeprom(444,rventa.configppu)==1){
							ppux10=1;
	                        set_imagen(2,60);	
							CyDelay(500);
							flujo_LCD2=14;
							if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}							
						}
						else{
							error_op(2);
						}
                    break;               
                   										
                    case 0x7E:									 //ir a menu
					  set_imagen(2,0);
                      flujo_LCD2=0;     
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
		
		case 31:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=0){
                    if(LCD_2_rxBuffer[3]<=9){
                        teclas2++;
                        Buffer_LCD2.valor[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        teclas2++;
                        Buffer_LCD2.valor[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                    }                    
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado nada regresa al menu anterior
                        set_imagen(2,0);
                        flujo_LCD2=0;
                    }
                    else{
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                        if(Buffer_LCD2.valor[teclas2]==0x2C){
                            comas2=0;
                        }
                        teclas2--;
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Enter
                    if(teclas2==1){                        
                        set_imagen(2,57);	
						Buffer_LCD2.valor[0]=teclas2;
						if(rventa.producto==1){
							extra2=Buffer_LCD2.valor[1];
							write_eeprom(634,Buffer_LCD2.valor);							
						}
						if(rventa.producto==2){
							corriente2=Buffer_LCD2.valor[1];
							write_eeprom(636,Buffer_LCD2.valor);							
						}
						if(rventa.producto==3){
							diesel2=Buffer_LCD2.valor[1];
							write_eeprom(638,Buffer_LCD2.valor);							
						}
                        if(rventa.producto==4){
							kero2=Buffer_LCD2.valor[1];
							write_eeprom(1008,Buffer_LCD2.valor);							
						}
						CyDelay(500);
						flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		
		break;
		
		case 32:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){
                    case 0x84:								 	//Biomax	
					  bandera[0]=1;
					  bandera[1]=0;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;
                    
                    case 0x86:  								//Terpel
					  bandera[0]=1;
					  bandera[1]=11;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  };
                    break;
                    
                    case 0x88:  								//ESSO
					  bandera[0]=1;
					  bandera[1]=4;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;
                    
                    case 0x89:                         			//Texaco                     
					  bandera[0]=1;
					  bandera[1]=12;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;   

                    case 0x8B:                         			//Petrobras                    
					  bandera[0]=1;
					  bandera[1]=9;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break; 
					
                    case 0x8A:                         			//Mobil                    
					  bandera[0]=1;
					  bandera[1]=8;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break; 	
					
                    case 0x85:                         			//Brio                    
					  bandera[0]=1;
					  bandera[1]=1;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break; 
					
                    case 0x87:                         			//Gulf                   
					  bandera[0]=1;
					  bandera[1]=6;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break; 	
					
                    case 0x98:                         			//Petromil                  
					  bandera[0]=1;
					  bandera[1]=14;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break; 
					
                    case 0x99:                         			//Zeus                
					  bandera[0]=1;
					  bandera[1]=13;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break; 					
					
                    case 0xA0:                         			//Ecospetrol                
					  bandera[0]=1;
					  bandera[1]=3;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;
					
                    case 0xA1:                         			//Exito               
					  bandera[0]=1;
					  bandera[1]=5;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;
					
                    case 0xA2:                         			//Mineroil               
					  bandera[0]=1;
					  bandera[1]=7;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;
					
                    case 0xA3:                         			//Plus               
					  bandera[0]=1;
					  bandera[1]=10;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;	
					
                    case 0x97:                         			//Cencosur               
					  bandera[0]=1;
					  bandera[1]=2;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;
					
                    case 0x94:                         			//Sin Bandera               
					  bandera[0]=0;
					  bandera[1]=0;
					  if(write_eeprom(455,bandera)==1){                               	 
                    	set_imagen(2,60); 
					  	CyDelay(500);
					  	flujo_LCD2=14;
					  	if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }
                    break;					
					
                    case 0x7E:									//ir a menu
					  bandera[0]=0;
					  bandera[1]=0;
					  if(write_eeprom(455,bandera)==1){                               	 
                        flujo_LCD2=14;
						if(version==1){
                        	set_imagen(2,93);
						}
						else{
							set_imagen(2,112);
						}						  
					  }	
					  else{
						set_imagen(2,0);
	                	flujo_LCD2=0;
					  }
                    break;					
                }					
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }					
		break;	

		case 33:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(LCD_2_rxBuffer[3] == 0x0A)
                {
                    imprimir(print2[1],producto2,1,lado.b.dir);
                    CyDelay(500);
                    set_imagen(2,12);
					flujo_LCD2=100;
					count_protector2=1;
				    isr_4_StartEx(animacion2);       // Confirma e imprime copia de recibo
				    Timer_Animacion2_Start();
                    
                }
                else{
					set_imagen(2,12);
					flujo_LCD2=100;
					count_protector2=1;             // Si presionan NO o cualquier otro touch
				    isr_4_StartEx(animacion2);  
				    Timer_Animacion2_Start();
                }                             
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }	
		 if(count_protector2>=20){
            set_imagen(2,12);
            flujo_LCD2=100; 
		    count_protector2=0;
		 }		 
		break;
		
        case 34:  
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){                               
                    case 0x38:								//No Corte
                      flujo_LCD2=0;     
					  set_imagen(2,0);	
                    break; 

                    case 0x39:								//Si Corte
                      flujo_LCD2=0;                   
                      set_imagen(2,59);
					  imprimir_corte(print2[1]);	
					  write_psoc1(print2[1],10);
					  write_psoc1(print2[1],10);
					  write_psoc1(print2[1],10);
					  write_psoc1(print2[1],0x1D);
					  write_psoc1(print2[1],0x56);
					  write_psoc1(print2[1],0x31);					
					  flujo_LCD2=100;
					  count_protector2=1;
				      isr_4_StartEx(animacion2);  
				      Timer_Animacion2_Start(); 						
                    break;					
					
                    case 0x94:								//Cambiar pasword
					  set_imagen(2,47);
					  teclas2=0;
                      flujo_LCD2=40;     
                    break;
					
                    case 0x7E:								//ir a menu
					  set_imagen(2,0);	
                      flujo_LCD2=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
         }       
        break;
		
        case 35:  
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){                               
                    case 0x91:								//Placa
                      flujo_LCD2=24;     
					  set_imagen(2,10);	
					  pos_ibutton=1;
					  teclado2=4;
					  teclas2=0;					
                    break; 

                    case 0x92:								//Cuenta
                      flujo_LCD2=24;     
					  set_imagen(2,111);
					  pos_ibutton=0x21;
					  teclado2=4;
					  teclas2=0;					
                    break;					
					
                    case 0x7E:								//ir a menu
					  set_imagen(2,0);	
                      flujo_LCD2=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
         }       
        break;
		
        case 36:  
			if(touch_write_memoria(2,Buffer_LCD2.valor[0],pos_ibutton,Buffer_LCD2.valor)==1){
				set_imagen(2,110);
				flujo_LCD2=35;
				if(touch_read_memoria(2,1)>=6){
					CyDelay(10);
					Placa_LCD(2);
					if(touch_read_memoria(2,0x21)>=6){						
						Cuenta_LCD(2);
					}
				}	
				else{
					error_op(2);
				}
			} 
			else{
				error_op(2);
			}
        break;
			
        case 37:  
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){                               
                    case 0x39:								//SI
					 if(tipo_imp[1] == 1){
					        print1[0]=1;
					        print1[1]=1;
					        print2[0]=1;
					        print2[1]=2;
                        }
                        if(tipo_imp[1] == 2){
					        print1[0]=1;
					        print1[1]=3;
					        print2[0]=1;
					        print2[1]=4;
                        }					
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(2,104); 
						  	CyDelay(500);
						  	flujo_LCD2=14;
						  	if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}
						}	
					  }
                    break; 

                    case 0x38:								//NO
                      flujo_LCD2=38;     
					  set_imagen(2,103);					
                    break;					
					
                    case 0x7E:								//ir a menu
					  if(copia_recibo[1]==1){
						copia_recibo[1]=0;
					  }	
					  else{
						copia_recibo[1]=1;
					  }
					  write_eeprom(986,copia_recibo);
					  set_imagen(2,60);
					  CyDelay(500);
					  set_imagen(2,0);
                      flujo_LCD2=0;    
                    break;						
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
         }       
        break;
		
        case 38:  
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){                               
                    case 0x8C:								//La impresora 1
					 if(tipo_imp[1] == 1){
					        print1[0]=1;
					        print1[1]=2;
					        print2[0]=1;
					        print2[1]=2;
                        }
                        if(tipo_imp[1] == 2){
					        print1[0]=1;
					        print1[1]=4;
					        print2[0]=1;
					        print2[1]=4;
                        }				
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(2,106); 
						  	CyDelay(500);
						  	flujo_LCD2=14;
						  	if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}
						}	
					  }
                    break; 

                    case 0x8D:								//La impresora 2
					   if(tipo_imp[1] ==1){   //Kiosko
					        print1[0]=1;
					        print1[1]=1;
					        print2[0]=1;
					        print2[1]=1;					
                        }
                        if (tipo_imp[1] ==2){ //Panel
                            print1[0]=1;
					        print1[1]=3;
					        print2[0]=1;
					        print2[1]=3;
                        }					
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(2,105); 
						  	CyDelay(500);
						  	flujo_LCD2=14;
						  	if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}
						}	
					  }					
                    break;
					
                    case 0x8E:								//Ninguna
					  print1[0]=1;
					  print1[1]=0;
					  print2[0]=1;
					  print2[1]=0;					
					  if(write_eeprom(611,print1)==1){ 
						if(write_eeprom(613,print2)==1){
	                    	set_imagen(2,108); 
						  	CyDelay(500);
						  	flujo_LCD2=14;
						  	if(version==1){
	                        	set_imagen(2,93);
							}
							else{
								set_imagen(2,112);
							}
						}	
					  }				
                    break;					
					
                    case 0x7E:								//ir a menu
					  set_imagen(2,0);	
                      flujo_LCD2=0;     
                    break;					
                }
            }
            CyDelay(100);
            LCD_2_ClearRxBuffer();
         }       
        break;
		
        case 39: 
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=3){
                    if(LCD_2_rxBuffer[3]<=9){
                        Buffer_LCD2.password[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,'*',teclas2);
						teclas2++;
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD2.password[teclas2]=0x30;
                        write_LCD(2,'*',teclas2);
                        teclas2++;						
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado regresa al inicio
						error_op(2);						
                    }
                    else{
                        teclas2--;						
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			
                    if(teclas2>=1){
						y=0;
						for(x=0;x<teclas2;x++){						
							if(pasword_corte[x+1]==Buffer_LCD2.password[x]){
								y++;
							}
							else{
								y=0;
							}
						}
						if(y==pasword_corte[0]){								
	                        set_imagen(2,38);
							CyDelay(500);
	                        flujo_LCD2=34;
							set_imagen(2,113);							
						}
						else{
							y=0;
							for(x=0;x<teclas2;x++){						
								if(puk[x]==Buffer_LCD2.password[x]){
									y++;
								}
							}
							if(y==4){								
								set_imagen(2,38);
								CyDelay(500);
								flujo_LCD2=34;
								set_imagen(2,113);							
							}	
							else{		
								set_imagen(2,39);
								flujo_LCD2=100;
								count_protector2=1;
								isr_4_StartEx(animacion2);  
								Timer_Animacion2_Start();
							}		
						}
                    }
					else{
                        set_imagen(2,39);
                        flujo_LCD2=100;
					 	count_protector2=1;
			         	isr_4_StartEx(animacion2);  
			         	Timer_Animacion2_Start();					
					}					
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }		  
        break;
		
        case 40:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                if(teclas2<=3){
                    if(LCD_2_rxBuffer[3]<=9){
                        Buffer_LCD2.password[teclas2]=LCD_2_rxBuffer[3]+0x30;
                        write_LCD(2,(LCD_2_rxBuffer[3]+0x30),teclas2);
						teclas2++;
                    }
                    if(LCD_2_rxBuffer[3]==0x0A){            	//Comando de 0
                        Buffer_LCD2.password[teclas2]=0x30;
                        write_LCD(2,0x30,teclas2);
                        teclas2++;						
                    }                     
                }
                if(LCD_2_rxBuffer[3]==0x0B){					//Cancel
                    if(teclas2==0){								//Si no ha presionado regresa al inicio
						error_op(2);						
                    }
                    else{
                        teclas2--;						
                        write_LCD(2,0x20,(teclas2));			//Si ya presiono borra el dato	
                    }
                }
                if(LCD_2_rxBuffer[3]==0x0C){					//Si presiona enter revisa que el password coinsida			   
					if(teclas2==4){
						set_imagen(2,57);	
	                    flujo_LCD2=0;
						pasword_corte[0]=teclas2;
						for(x=0;x<teclas2;x++){
							pasword_corte[x+1]=Buffer_LCD2.password[x];
						}
						if(write_eeprom(671,pasword_corte)==0){							//Guarda el nombre en la eeprom
							set_imagen(2,85);
							flujo_LCD2=100;
						}
                    }
                }
            }
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }    
        break;
        
        case 41:
         if(LCD_2_GetRxBufferSize()==8){
            if((LCD_2_rxBuffer[0]==0xAA) && (LCD_2_rxBuffer[6]==0xC3) && (LCD_2_rxBuffer[7]==0x3C)){
                switch(LCD_2_rxBuffer[3]){ 
                    
                    case 0xBC:								//Impresora Kiosko
					    tipo_imp[0] = 1;
                        tipo_imp[1] = 1;
                        set_imagen(2,112);	
                        flujo_LCD2 = 14;
                    break; 

                    case 0xBD:								//Impresora panel
                        tipo_imp[0] = 1;
                        tipo_imp[1] = 2;
                        set_imagen(2,112);	
                        flujo_LCD2 = 14;
                    break;
                     
                    }
                }
            
            CyDelay(100);            
            LCD_2_ClearRxBuffer();
         }    
        break;
    }
}


/*
*********************************************************************************************************
*                                         CY_ISR(animacion)
*
* Description : Interrupcion que temporiza las imagenes informativas que aparecen en la pantalla 1
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
CY_ISR(animacion){
    Timer_Animacion_ReadStatusRegister();
    if(flujo_LCD==1){										//Muestra el protector de la pantalla
        if(count_protector<=6){
            count_protector++;
            set_imagen(1,(iprotector5+count_protector));  
        }
        else{
           count_protector=0; 
           set_imagen(1,(iprotector5+count_protector));  
        }
    }
    else{													//Incrementa el contador 
        count_protector++;
    }
}

/*
*********************************************************************************************************
*                                         CY_ISR(animacion2)
*
* Description : Interrupcion que temporiza las imagenes informativas que aparecen en la pantalla 2
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
CY_ISR(animacion2){
    Timer_Animacion2_ReadStatusRegister();
    if(flujo_LCD2==1){
        if(count_protector2<=6){
            count_protector2++;
            set_imagen(2,(iprotector5+count_protector2));  
        }
        else{
           count_protector2=0; 
           set_imagen(2,(iprotector5+count_protector2));  
        }
    }
    else{
        count_protector2++;
    }
}

/*
*********************************************************************************************************
*                                         main( void )
*
* Description : Ejecuta las funciones de inicio y verifica el estado de las pantallas
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
int main()
{	
    init();
	init_surt();
    for(;;)
    {
    	polling_LCD1();
		polling_LCD2();
    }	
}

/* [] END OF FILE */
