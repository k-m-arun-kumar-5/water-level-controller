/**************************************************************************
   FILE          :    conf_misc_ids.h
 
   PURPOSE       :   Data IDs, Timer IDs and general interrupt request sources configuration header
 
   AUTHOR        :    K.M. Arun Kumar
 
  KNOWN BUGS     :
	
  NOTE           :   If data terminator char = NULL_CHAR, then newly read chars are taken till max nums of chars is reached.
                 :   If data terminator char != NULL_CHAR, then if data's max data allocated char < num entered chars, then new entered chars are taken, till terminator char is read.
			            else, then newly read chars are discarded, till terminator char is read. 
  
  CHANGE LOGS    : 
	   
 **************************************************************************/
 
 #ifndef _CONF_MISC_IDS_H
 #define _CONF_MISC_IDS_H (5)

  #include "constants.h"
  
 //                                            DATA_ID,      NUM_CHAR, SMALL_ALPHA, BIG_ALPHA, SPECIAL_CHAR, CTRL_CHAR, O/P_STREAM,    I/P_STREAM,   OUTPUT_DISP_FORMAT,                    
 const data_id_ctrl_para_t data_id_ctrl_para[]  = {  
	                                         { DATA_ID_WATER_LEVEL_CTRL_NORMAL,  STATE_YES,  STATE_YES, STATE_YES, STATE_NO,  STATE_NO, DEVICE_DATA_NA, DEVICE_DATA_NA, RCVD_CHAR_PLAIN_DISP_FORMAT,  
											 
//											 O/P_DATA_TYPE,     I/P_DATA_TYPE,        TX_STREAM,         RCV_STREAM,       TX_DATA_EVENT,   RCV_DATA_EVENT,  MAX_READ_TRY,     TERMINATOR,      REQ_MAX_CHARS
											  DATA_TYPE_IN_STR, DATA_TYPE_IN_STR,  DEVICE_DATA_NA,   DEVICE_DATA_NA,       EVENT_POLLING ,   EVENT_POLLING,     DATA_NA,       DATA_NA,        DATA_NA }
                                                  };

//                                                 DATA_ID,               INPUT_DEV,                            OUTPUT_DEV,     	                            RECEIVE_DEV,      TRANSMIT_DEV        									   
 const data_id_dev_src_t  data_ids_dev_srcs[] = {	                                             
                                                  { DATA_ID_WATER_LEVEL_CTRL_NORMAL, {DEV_ID_NA}, {OUTPUT_DEV_ID_LCD, CH_ID_00, DEV_ID_NA},  {DEV_ID_NA}, {DEV_ID_NA}}												
		                                       };	
											   
/* DONT_CHANGE: BEGIN - INTP_REQ SRC's position in interrupt_request_ctrl must not change as it corresponds to LPC2138 peripherial interrupt request bit pos */											   
//                               VECTOR_IRQ_ISR,            INTERRUPT_REQUEST_SRC,          INTERRUPT_TYPE,         VECTOR_SLOT                                    	
interrupt_request_ctrl_t  interrupt_request_ctrl[NUM_INTP_REQ_SRCS] = 
                              {
								  #ifdef WATCHDOG_TIMER_MOD_ENABLE
								                   {(uint32_t )ISR_Watch_Dog_Timer,                                INTP_REQ_WDT,            INTERRUPT_REQ_NA,              VECTOR_SLOT_NA    },
								  #else			
                                     {(uint32_t)0,                                INTP_REQ_WDT,            INTERRUPT_REQ_NA,              VECTOR_SLOT_NA    },
                                  #endif								 
																	 {(uint32_t )0,                           INTP_REQ_RESERVED_SW_INTP,    INTERRUPT_REQ_NA,              VECTOR_SLOT_NA },  
																	 {(uint32_t )0,                                INTP_REQ_ARM_CORE_CH_0,      INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
																	 {(uint32_t )0,                                INTP_REQ_ARM_CORE_CH_1,      INTERRUPT_REQ_NA,              VECTOR_SLOT_NA },
      														#ifdef TIMER_MOD_ENABLE			 
																	 {(uint32_t )ISR_Timer_0,                      INTP_REQ_TMR_CH_0,           INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
                                                                     {(uint32_t )ISR_Timer_1,                      INTP_REQ_TMR_CH_1,           INTERRUPT_REQ_VECTOR_IRQ,       VECTOR_SLOT_06},
															#else	
																{(uint32_t )0,                      INTP_REQ_TMR_CH_0,           INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
                                                                     {(uint32_t )0,                      INTP_REQ_TMR_CH_1,           INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
															#endif	
														    #ifdef UART_MOD_ENABLE
																	 {(uint32_t )ISR_UART_0,                                INTP_REQ_UART_CH_0,          INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },  
																	 {(uint32_t )ISR_UART_1,                                INTP_REQ_UART_CH_1,          INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
                                                            #else 	
                                                                {(uint32_t )0,                                INTP_REQ_UART_CH_0,          INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },  
																	 {(uint32_t )0,                                INTP_REQ_UART_CH_1,          INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
														    #endif	
                                                            #ifdef PWM_MOD_ENABLE															
																	 {(uint32_t )ISR_PWM_0,                                INTP_REQ_PWM_CH_0,           INTERRUPT_REQ_NA,              VECTOR_SLOT_NA}, 
															#else		
                                                               	{0,                                INTP_REQ_PWM_CH_0,           INTERRUPT_REQ_NA,              VECTOR_SLOT_NA}, 
															#endif
															#ifdef I2C_MOD_ENABLE
																	 {(uint32_t )ISR_I2C_0,                                INTP_REQ_I2C_CH_0,           INTERRUPT_REQ_NA,               VECTOR_SLOT_NA}, 
															#else	
                                                               	 {(uint32_t)0,                                INTP_REQ_I2C_CH_0,           INTERRUPT_REQ_NA,               VECTOR_SLOT_NA},
															#endif 
															#ifdef SPI_MOD_ENABLE
																	 {(uint32_t )ISR_SPI_0,                                INTP_REQ_SPI_CH_0,          INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
																	 {(uint32_t )ISR_SPI_1,                                INTP_REQ_SPI_CH_1,    INTERRUPT_REQ_NA,               VECTOR_SLOT_NA}, 
															#else	
																 {(uint32_t )0,                                INTP_REQ_SPI_CH_0 ,          INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
																	 {(uint32_t )0,                                INTP_REQ_SPI_CH_1,    INTERRUPT_REQ_NA,               VECTOR_SLOT_NA}, 
															#endif
                                                           		 {(uint32_t )ISR_PLL,                                INTP_REQ_PLL,            INTERRUPT_REQ_NA,               VECTOR_SLOT_NA }, 
															
															#ifdef RTC_MOD_ENABLE
																	 {(uint32_t )ISR_RTC,                                INTP_REQ_RTC,            INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
															#else	
                                                               	 {(uint32_t)0,                                INTP_REQ_RTC,            INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
															#endif
                                                            #ifdef EXT_INTERRUPT_MOD_ENABLE															
																	 {(uint32_t )ISR_Ext_Interrupt_0,    INTP_REQ_EXT_INTP_CH_0,          INTERRUPT_REQ_NA,              VECTOR_SLOT_NA }, 
																	 {(uint32_t )ISR_Ext_Interrupt_1,    INTP_REQ_EXT_INTP_CH_1,          INTERRUPT_REQ_NA,              VECTOR_SLOT_NA }, 
																	 {(uint32_t )ISR_Ext_Interrupt_2,    INTP_REQ_EXT_INTP_CH_2,          INTERRUPT_REQ_NA,              VECTOR_SLOT_NA },  
																	 {(uint32_t )ISR_Ext_Interrupt_3,    INTP_REQ_EXT_INTP_CH_3,          INTERRUPT_REQ_VECTOR_IRQ,      VECTOR_SLOT_05 }, 
															#else
																{(uint32_t )0,    INTP_REQ_EXT_INTP_CH_0,          INTERRUPT_REQ_NA,              VECTOR_SLOT_NA }, 
																{(uint32_t )0,    INTP_REQ_EXT_INTP_CH_1,          INTERRUPT_REQ_NA,              VECTOR_SLOT_NA }, 
																{(uint32_t )0,    INTP_REQ_EXT_INTP_CH_2,          INTERRUPT_REQ_NA,              VECTOR_SLOT_NA },  
																{(uint32_t )0,    INTP_REQ_EXT_INTP_CH_3,          INTERRUPT_REQ_NA,               VECTOR_SLOT_NA }, 
															#endif	
                                                            #ifdef ADC_MOD_ENABLE															
																	 {(uint32_t )ISR_ADC_0,                                INTP_REQ_ADC_CH_0,            INTERRUPT_REQ_NA,               VECTOR_SLOT_NA }, 
                                                            #else
                                                                      {(uint32_t )0,                               INTP_REQ_ADC_CH_0,            INTERRUPT_REQ_NA,               VECTOR_SLOT_NA },
                                                            #endif
                                                            #ifdef I2C_MOD_ENABLE															
																	 {(uint32_t )ISR_I2C_1,                                INTP_REQ_I2C_CH_1,           INTERRUPT_REQ_NA,    VECTOR_SLOT_NA }, 
															#else		 
															         {(uint32_t )0,                                INTP_REQ_I2C_CH_1,           INTERRUPT_REQ_NA,    VECTOR_SLOT_NA }, 	
                                                            #endif 														 
																	 {(uint32_t )ISR_Brown_Out_Detect,                                INTP_REQ_BOD,            INTERRUPT_REQ_NA,             VECTOR_SLOT_NA },
															#ifdef ADC_MOD_ENABLE		 
																	 {(uint32_t )ISR_ADC_1,                                INTP_REQ_ADC_CH_1,            INTERRUPT_REQ_NA,             VECTOR_SLOT_NA }
                                                            #else
                                                            	{(uint32_t )0,                                INTP_REQ_ADC_CH_1,            INTERRUPT_REQ_NA,             VECTOR_SLOT_NA }
                                                            #endif															
							  }; 

/* DONT_CHANGE : END */	
						  
#ifdef TIMER_MOD_ENABLE

timer_or_counter_id_ctrl_t timer_or_counter_id_ctrl[] =                                         
                             {
//                               TIMER_ID,        TIMER_OR_COUNTER_MODE,  MATCH_OR_CAPTURE_ID, MATCH_INTERRUPT_OR_CAPTURE_RISING,   CAPTURE_FALLING,   								 
                              { TIMER_ID_DRY_RUN,    TIMER_MODE,              CH_ID_01,               STATE_YES,                         STATE_NO,
							  
//                             CAPTURE_INTERRUPT,         STATE,               MAX_NUM_TIMEOUTS,      TIMEOUT_IN_MS_OR_COUNT,                    
                                STATE_NO,        TMR_OR_COUNTER_IDLE_STATE,     1,                       TIMEOUT_NEXT_SENSOR_WATER_LVL_IN_MS                },
                              
//                                TIMER_ID,            TIMER_OR_COUNTER_MODE,  MATCH_OR_CAPTURE_ID, MATCH_INTERRUPT_OR_CAPTURE_RISING,             CAPTURE_FALLING,   								 
                              { TIMER_ID_PRESET_WATER_LVL_LOCK,   TIMER_MODE,               CH_ID_01,               STATE_YES,                                  STATE_NO,   
							  
//                              CAPTURE_INTERRUPT,        STATE,               MAX_NUM_TIMEOUTS,   TIMEOUT_IN_MS_OR_COUNT,                       
                                  STATE_NO,        TMR_OR_COUNTER_IDLE_STATE,      1,              TIMEOUT_PRESET_WATER_LVL_LOCK_IN_MS}								
							 };
 
#endif		

#endif
																						 
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
																						 
