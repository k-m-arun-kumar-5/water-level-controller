/* ********************************************************************
FILE                   : appl.c

PURPOSE                :  Application 
	 
AUTHOR                : K.M.Arun Kumar
	 
KNOWN BUGS            : 

NOTE                  : 
						
CAUTION               :  
                                    
CHANGE LOGS           :  

FILE ID               : 11

*****************************************************************************/
#ifndef _MAIN_H
    #include "main.h"
#endif

#ifndef _APPL_H
    #include "appl.h"
#endif


/* ------------------------------ macro defination ------------------------------ */

/* ----------------------------- global variable defination --------------------- */ 
water_lvl_data_t water_lvl_data;
uint32_t appl_status_flag = NO_ERROR; 
uint32_t io_chs_map_data = 0; 
uint8_t ctrl_prev_fsm_state = FSM_STATE_INIT, proc_data = 0;
			
/* ----------------------------- global variable declaration -------------------- */


/* ----------------------------- global function declaration -------------------- */
static uint16_t Set_Data_Lock_Proc(const uint8_t data_type_set_lock);


/*------------------------------------------------------------*
FUNCTION NAME  : Appl_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.02

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Appl_Proc(void)
{	
      	
	switch(cur_data_id) 
	{	
	    case DATA_ID_WATER_LEVEL_CTRL_NORMAL: 
         	if((Ctrl_Water_Lvl_FSM_Proc(READ_ALL_SENSORS_SWS)) != SUCCESS)
            {
				appl_status_flag = ERR_WATER_LVL_PROC;
		        return appl_status_flag;
			}				
		break;
        default:
		   appl_status_flag = ERR_CUR_DATA_ID_INVALID;
		   Error_or_Warning_Proc("11.02.02", ERROR_OCCURED, appl_status_flag);
		   return appl_status_flag;
	 } 
     return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Appl_Init

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.03  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Appl_Init(const void *const data_ptr)
{	
  uint8_t cur_sw_ch_id;
		
	if((Next_Data_Conf_Parameter(DATA_ID_WATER_LEVEL_CTRL_NORMAL)) != SUCCESS)
	{
		 appl_status_flag = ERR_NEXT_DATA_CONF;
		 Error_or_Warning_Proc("11.03.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	return SUCCESS; 
}

/*------------------------------------------------------------*
FUNCTION NAME  : Appl_HW_Init

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : First initialise independent SW in order from SW_CH_ID from 0 if any, then initialize Keyboard, if any.

Func ID        : 11.04  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Appl_HW_Init(void)
{
  uint8_t cur_sw_ch_id;
		
	if((UART_Init(TRACE_UART_CH_ID, COMM_TRANSMIT_FUNC)) != SUCCESS)
	{
		return FAILURE;
	}
	if((UART_Init(TRACE_UART_CH_ID, COMM_RECEIVE_FUNC)) != SUCCESS)
	{
		return FAILURE;
	}
	if((UART_Init(TRACE_UART_CH_ID, DEV_INIT_OPER)) != SUCCESS)
	{
		return FAILURE;
	}
	if((Output_Dev_Init(INTERNAL_ERROR_LED_IO_CH, 1)) != SUCCESS)
	{
		return FAILURE;
	}
	if((Output_Dev_Init(WATER_PUMP_CTRL_IO_CH, 1)) != SUCCESS)
	{
		return FAILURE;
	}
	if((LCD_Init(CH_ID_00, DATA_NA)) != SUCCESS)
	{
		return FAILURE;
	}
	for(cur_sw_ch_id = 0; cur_sw_ch_id < NUM_INPUT_DEV_ID_SW_CHS; ++cur_sw_ch_id )
	{
		if((SW_Init(cur_sw_ch_id, DATA_NA)) != SUCCESS)
		{
			   return FAILURE;
		}
	}
	#ifdef TRACE_FLOW
	   Print("TRA: HW Init over\r");
	#endif
	return SUCCESS; 
}

/*------------------------------------------------------------*
FUNCTION NAME  : Error_or_Warning_Occured_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.04

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Error_or_Warning_Occured_Proc(const uint8_t warn_or_error_format, const uint32_t warning_or_error_code)
{
	switch(warn_or_error_format)
	{
		case WARNING_OCCURED:			
		  if((error_or_warning_proc_flag & (1 << 0)) == 0)
		  {
			  /* warning occured, process to do  */ 
			  error_or_warning_proc_flag |= (1 << 0);
		  }		 
		break;
		case ERROR_OCCURED:
		   if((error_or_warning_proc_flag & (1 << 1)) == 0)
		   {
		       /* BEGIN: error occured, process to do  */
			   	  		  
               Ctrl_Water_Lvl_Error_Proc(WATER_LVL_INTERNAL_ERR_PROC);   
			   
			   /* END: error occured, process to do  */
			   
			   error_or_warning_proc_flag |= (1 << 1);
			   error_or_warning_proc_flag |= (1 << 0); 
		   }		   
		break;
		case FATAL_OCCURED:
		  if((error_or_warning_proc_flag & (1 << 2)) == 0)
		  {
		      /* fatal error occured, process to do */
			  error_or_warning_proc_flag |= (1 << 2);
			  error_or_warning_proc_flag |= (1 << 1);
			  error_or_warning_proc_flag |= (1 << 0); 
		  }		  
        break;
		default:
		   error_or_warning_proc_flag |= (1 << 3);
           system_status_flag = ERR_FORMAT_INVALID;		
		   return system_status_flag;
	}	
	return SUCCESS;
}


/*------------------------------------------------------------*
FUNCTION NAME  : Appl_Reset

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.01

BUGS           :    
-*------------------------------------------------------------*/
 uint16_t Appl_Reset(const uint8_t reset_type)
{

	switch(reset_type)
	{
		case RESET_APPL:	
           appl_status_flag = NO_ERROR;	
		   Appl_Reset_Proc(RESET_APPL);
		break;
		default:
		   appl_status_flag = ERR_FORMAT_INVALID;
		   Error_or_Warning_Proc("11.01.04", ERROR_OCCURED, appl_status_flag);
		   return appl_status_flag;
	}
	return SUCCESS;
} 

/*------------------------------------------------------------*
FUNCTION NAME  : Appl_Reset_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.11

BUGS           :    
-*------------------------------------------------------------*/ 
uint16_t Appl_Reset_Proc(const uint8_t reset_data)
{
		
	Reset_Water_Lvl_Datas(RESET_WHOLE_DATAS);    
	return SUCCESS;
}	

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Water_Lvl_Datas

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.13

BUGS           :    
-*------------------------------------------------------------*/ 
uint16_t Reset_Water_Lvl_Datas(const uint8_t reset_data)
{
	uint8_t cur_sw_ch_id;
	
	switch(reset_data)
	{
		case RESET_WHOLE_DATAS: 
           if((LCD_Enable(CH_ID_00)) != SUCCESS)
	       {
	        	appl_status_flag = ERR_DEV_ENABLE_PROC;
		        Error_or_Warning_Proc("11.13.01", ERROR_OCCURED, appl_status_flag);
		        return appl_status_flag;
	       }
	       if((UART_Transmit_Enable_Ctrl(TRACE_UART_CH_ID, STATE_YES)) != SUCCESS)
	       {
	        	appl_status_flag = ERR_DEV_ENABLE_PROC;
		        Error_or_Warning_Proc("11.03.02", ERROR_OCCURED, appl_status_flag);
		        return appl_status_flag;
	       }
	       if((UART_Receive_Enable_Ctrl(TRACE_UART_CH_ID, STATE_YES)) != SUCCESS)
	       {
	        	appl_status_flag = ERR_DEV_ENABLE_PROC;
	            Error_or_Warning_Proc("11.13.03", ERROR_OCCURED, appl_status_flag);
		        return appl_status_flag;
	       }
	       if((UART_Enable(TRACE_UART_CH_ID)) != SUCCESS)
	       {
		      appl_status_flag = ERR_DEV_ENABLE_PROC;
		      Error_or_Warning_Proc("11.13.04", ERROR_OCCURED, appl_status_flag);
		      return appl_status_flag;
	       }	
	       for(cur_sw_ch_id = 0; cur_sw_ch_id < NUM_INPUT_DEV_ID_SW_CHS; ++cur_sw_ch_id )
		   {
	    	   if((SW_Allow_Access(cur_sw_ch_id) ) != SUCCESS)
		       {
			       appl_status_flag = ERR_DEV_ALLOW_ACCESS_PROC;
		           Error_or_Warning_Proc("11.13.05", ERROR_OCCURED, appl_status_flag );
		           return appl_status_flag;  
		       }
		       if((SW_Enable(cur_sw_ch_id)) != SUCCESS)
		       {
			      appl_status_flag = ERR_DEV_ALLOW_ACCESS_PROC;
		          Error_or_Warning_Proc("11.13.06", ERROR_OCCURED, appl_status_flag);
		          return appl_status_flag;  
		       }
		   }			
		   if((LCD_Clear_Screen(CH_ID_00)) != SUCCESS)
	       {
		       appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	           Error_or_Warning_Proc("11.13.07", ERROR_OCCURED, appl_status_flag);
	           return appl_status_flag;
	       }
	       if((LCD_Data_Disp(LCD_DISP_CONSTANT_DATA)) != SUCCESS)
	       {
	        	appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.13.08", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	       }
	       if((Dev_IO_Chs_Map_Data_Validate(sws_io_ch_map_datas, NUM_SWS_IO_CH_MAP, DEV_INPUT_TYPE )) != SUCCESS)
	       {
	        	appl_status_flag = ERR_IO_DEV_IO_CHS_MAP_PROC;
	            Error_or_Warning_Proc("11.13.09", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	       }
		   if((Enable_Ext_Interrupt(RESET_SW_EXT_INTP_CH_ID) ) != SUCCESS)
		   {
			    appl_status_flag = ERR_ENABLE_INTERRUPT;
	            Error_or_Warning_Proc("11.13.04", ERROR_OCCURED, appl_status_flag);			
				return appl_status_flag;
		   }
        case RESET_BY_RESET_SW:	
            Timer_Stop(CH_ID_01);
			Set_Data_Lock_Proc(ALL_SET_DATA_LOCK);
            ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;			
	        water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INIT;
	       #ifdef TRACE_FLOW
	          Print("TRA: Reset Water Level Datas \r");
	       #endif
		break;
	}	
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Data_Disp

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.14

BUGS           :    
-*------------------------------------------------------------*/ 
uint16_t LCD_Data_Disp(const uint8_t lcd_disp_data)
{
	disp_num_fmt_t lcd_disp_ctrl;
	uint8_t lcd_col_num_index;
	
	switch(lcd_disp_data)
	{
		 case LCD_DISP_PUMP_ON:
		   	if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, LCD_DISP_PUMP_STATE_COL_NUM)) != SUCCESS)
	        {
	           	 appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	             Error_or_Warning_Proc("11.14.01", ERROR_OCCURED, appl_status_flag);
	             return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, " ON ")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.02", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }	
		 break;
	     case LCD_DISP_PUMP_OFF:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, LCD_DISP_PUMP_STATE_COL_NUM)) != SUCCESS)
	        {
	           	 appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	             Error_or_Warning_Proc("11.14.03", ERROR_OCCURED, appl_status_flag);
	             return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "OFF ")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.04", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }    
		 break;
		 case LCD_DISP_MANUAL_MODE:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, LCD_DISP_MODE_COL_NUM)) != SUCCESS)
	        {
	           	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	            Error_or_Warning_Proc("11.14.05", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "MANUAL")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.06", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }		
	     break;
         case LCD_DISP_AUTO_MODE:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, LCD_DISP_MODE_COL_NUM)) != SUCCESS)
	        {
	           	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	            Error_or_Warning_Proc("11.14.07", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "AUTO  ")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.08", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }		  
		 break;
		 case LCD_DISP_ERROR:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, LCD_DISP_MODE_COL_NUM)) != SUCCESS)
	        {
	           	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	            Error_or_Warning_Proc("11.14.09", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "ERROR ")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.10", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }		
		 break;
		 case LCD_DISP_SENSORS_ABNORMAL:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, LCD_DISP_MODE_COL_NUM)) != SUCCESS)
	        {
	           	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	            Error_or_Warning_Proc("11.14.11", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "SENSOR")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.12", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }		
		 break;
		 case LCD_DISP_DRY_RUN:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, LCD_DISP_MODE_COL_NUM)) != SUCCESS)
	        {
	           	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	            Error_or_Warning_Proc("11.14.13", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "DRYRUN")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.14", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }		
		 break;
		 case LCD_DISP_PRESET_WATER_LVL:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_03, LCD_DISP_PRESET_WATER_LVL_COL_NUM)) != SUCCESS)
	        {
	              	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	       	        Error_or_Warning_Proc("11.14.15", ERROR_OCCURED, appl_status_flag);
	                return appl_status_flag;
            }		
            lcd_disp_ctrl.sign_flag = STATE_NO;
	        lcd_disp_ctrl.least_digits_flag = STATE_YES;
	        lcd_disp_ctrl.left_format_flag = STATE_YES;
	        lcd_disp_ctrl.num_digits = LCD_WATER_LVL_DATA_NUM_DIGITS;
	        if((LCD_Disp_Num(CH_ID_00, DATA_TYPE_IN_DECIMAL, lcd_disp_ctrl, water_lvl_data.preset_water_lvl)) != SUCCESS)
	        {
	        	appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.16", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        } 								
		 break;
		 case LCD_DISP_CUR_WATER_LVL:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_01, LCD_DISP_CUR_WATER_LVL_COL_NUM )) != SUCCESS)
	        {
	              	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	       	        Error_or_Warning_Proc("11.14.17", ERROR_OCCURED, appl_status_flag);
	                return appl_status_flag;
            }		
            lcd_disp_ctrl.sign_flag = STATE_NO;
	        lcd_disp_ctrl.least_digits_flag = STATE_YES;
	        lcd_disp_ctrl.left_format_flag = STATE_YES;
	        lcd_disp_ctrl.num_digits = LCD_WATER_LVL_DATA_NUM_DIGITS;
	        if((LCD_Disp_Num(CH_ID_00, DATA_TYPE_IN_DECIMAL, lcd_disp_ctrl, water_lvl_data.cur_water_lvl)) != SUCCESS)
	        {
	        	appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.18", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        } 								
		 break;
		 case LCD_DISP_CONSTANT_DATA:
		    if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_01, COL_NUM_01)) != SUCCESS)
	        {
	           	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
		        Error_or_Warning_Proc("11.14.19", ERROR_OCCURED, appl_status_flag);
		        return appl_status_flag;
            }
		    if((LCD_Disp_Str(CH_ID_00, "Water Level: ")) != SUCCESS)
	        {
		        appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.20", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }					 
	        if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_01, LCD_DISP_CUR_WATER_LVL_PERC_COL_NUM)) != SUCCESS)
	        {
		         appl_status_flag = ERR_LCD_DISP_GOTO_XY;
		         Error_or_Warning_Proc("11.14.21", ERROR_OCCURED, appl_status_flag);
		         return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "%")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.22", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }
	        if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_03, COL_NUM_01)) != SUCCESS)
	        {
	           	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	    	    Error_or_Warning_Proc("11.14.23", ERROR_OCCURED, appl_status_flag);
	    	    return appl_status_flag;
            }
		    if((LCD_Disp_Str(CH_ID_00, "Pre Water Lvl: ")) != SUCCESS)
	        {
		         appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	             Error_or_Warning_Proc("11.14.24", ERROR_OCCURED, appl_status_flag);
	             return appl_status_flag;
			}	    	
	        if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_03, LCD_DISP_PRESET_WATER_LVL_PERC_COL_NUM)) != SUCCESS)
	        {
	         	appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	         	Error_or_Warning_Proc("11.14.25", ERROR_OCCURED, appl_status_flag);
	         	return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "%")) != SUCCESS)
	        {
	             appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	             Error_or_Warning_Proc("11.14.26", ERROR_OCCURED, appl_status_flag);
	             return appl_status_flag;
	        }
            if((LCD_Disp_Goto_XY(CH_ID_00, LINE_NUM_02, COL_NUM_01)) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	            Error_or_Warning_Proc("11.14.27", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
            }
	        if((LCD_Disp_Str(CH_ID_00, "PUMP : ")) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.14.28", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }	
		 break;
		 case LCD_DISP_ERR_LINE_BLANK:
		    if((LCD_Disp_Goto_XY(CH_ID_00, ERROR_LINE_NUM, COL_NUM_01)) != SUCCESS)
	        {
	            appl_status_flag = ERR_LCD_DISP_GOTO_XY;
	            Error_or_Warning_Proc("11.14.29", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
            }
		    for(lcd_col_num_index = 0; lcd_col_num_index <= lcd_ctrl[CH_ID_00].max_config_cols_in_index; ++lcd_col_num_index)
			{
			    if((LCD_Disp_Char(CH_ID_00, ' ')) != SUCCESS)
				{
					appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	                Error_or_Warning_Proc("11.14.30", ERROR_OCCURED, appl_status_flag);
	                return appl_status_flag;
				}
			}
		 break;		
		 default:
		    appl_status_flag = ERR_FORMAT_INVALID;
	        Error_or_Warning_Proc("11.14.31", ERROR_OCCURED, appl_status_flag);
	        return appl_status_flag; 
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Ctrl_Water_Lvl_FSM_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.15

BUGS           :    
-*------------------------------------------------------------*/ 
uint16_t Ctrl_Water_Lvl_FSM_Proc(const uint8_t read_sw_or_sensor_type)
{
	consucc_bit_t read_state;
	timer_or_counter_data_t timer_timeout_status; 
	uint16_t ret_status = SUCCESS, timer_ret_status;
	uint8_t cur_sw_ch_id, temp_data  = NUM_SWS_IO_CH_MAP; 
	static uint8_t temp_cur_water_lvl = WATER_LVL_INVALID, prev_water_lvl_sensor_map = 0;
	
	ret_status = Ext_Interrupt_Retrieve_Data_Arr(RESET_SW_EXT_INTP_CH_ID);
	switch(ret_status)
	{
		case SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND:
		        timer_ret_status = Timer_Interrupt_Retrieve_Data_Arr(CH_ID_01, &timer_timeout_status);
                switch (timer_ret_status)
			    {
			         case SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND:
				     break;
				     case TMR_NO_MAX_NUM_TIMEOUT_PROC:
				     case TMR_BEFORE_LAST_TIMEOUT_PROC:
				     case TMR_AT_LAST_TIMEOUT_PROC:
				     break; 
			         case TMR_MAX_NUM_TIMEOUT_PROC:	
					    switch(timer_timeout_status.timer_or_counter_run_id)
						{
							case TIMER_ID_DRY_RUN:  
				                water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_TIMEOUT_DRY_RUN;
				                #ifdef TRACE_FLOW
				                    Print("FLOW: -> TIMEOUT_DRY_RUN \r");
				                #endif
							break;
							case TIMER_ID_PRESET_WATER_LVL_LOCK:
							    Set_Data_Lock_Proc(WATER_LVL_PRESET_SET_LOCK);								
							break;
						}							
				     break;
				     default: 
                         water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
				         appl_status_flag = ERR_TIMEOUT_PROC;
	                     Error_or_Warning_Proc("11.15.01", ERROR_OCCURED, appl_status_flag);
                         return appl_status_flag;	            
			    }

		break;
		case SUCCESS:
		   #ifdef TRACE_INFO
	           Print("INFO: Reset SW triggered ON \r");
	       #endif
		   ret_status = Delete_Src_Interrupt_Data_Arr_By_Src(INTP_REQ_TMR_CH_1);
	       switch(ret_status)
	       {
		     	case SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND:
		        break;
		        case SEARCH_INTP_SRC_MORE_THAN_ONE_DATA_ARR:
		        case SEARCH_INTP_SRC_ONE_DATA_ARR:
		        break;
		        default:
		           water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
				   appl_status_flag = ERR_DELETE_INTP_SRC_DATA_PROC;
	               Error_or_Warning_Proc("11.15.02", ERROR_OCCURED, appl_status_flag);
		           return appl_status_flag;
	        }
	        water_lvl_data.func_state = TRIGGERED_RESET_SW;
	        Reset_Water_Lvl_Datas(RESET_BY_RESET_SW);	
		break;
		default: 
		   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		   appl_status_flag = ERR_EXT_INTP_PROC_FOR_RESET;
	       Error_or_Warning_Proc("11.15.03", ERROR_OCCURED, appl_status_flag);
		   return appl_status_flag;
	}
	ret_status = SUCCESS;
	switch(water_lvl_data.ctrl_cur_fsm_state)
	{
		case FSM_STATE_IDLE:
		break; 
		case FSM_STATE_INIT:		    
		    water_lvl_data.preset_water_lvl = DEFAULT_PRESET_WATER_LVL;	
	        water_lvl_data.cur_water_lvl = 0; 
		    water_lvl_data.cur_oper_mode = OPER_MODE_INVALID;
		    water_lvl_data.changed_oper_mode_flag = STATE_YES;
	        water_lvl_data.first_time_proc_flag = STATE_YES; 
			appl_status_flag = NO_ERROR;	
		    system_status_flag = NO_ERROR;	
		    error_or_warning_proc_flag = 0; 
		    io_chs_map_data = 0; 
			proc_data = 0;
		    temp_cur_water_lvl = WATER_LVL_INVALID;
		    prev_water_lvl_sensor_map = 0;			
			if((LCD_Data_Disp(LCD_DISP_PRESET_WATER_LVL)) != SUCCESS)
	        {
				 water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		         appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	             Error_or_Warning_Proc("11.15.04", ERROR_OCCURED, appl_status_flag);
		         return appl_status_flag;
	        }
		    if((LCD_Data_Disp(LCD_DISP_ERR_LINE_BLANK)) != SUCCESS)
	        {
				 water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		         appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	             Error_or_Warning_Proc("11.15.05", ERROR_OCCURED, appl_status_flag);
		         return appl_status_flag;
	        }
			if((IO_Channel_Write(INTERNAL_ERROR_LED_IO_CH, STATE_LOW)) != SUCCESS)
	        {
				water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
	        	appl_status_flag = ERR_IO_CH_WRITE;
	            Error_or_Warning_Proc("11.15.06", ERROR_OCCURED, appl_status_flag);
	            return appl_status_flag;
	        }
			if((SW_Oper(FUNC_SW_IO_CH, DEV_ENABLE_OPER)) != SUCCESS)
		    {
				water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			    appl_status_flag = ERR_DEV_OPER_ENABLE_FUNC;
		        Error_or_Warning_Proc("11.15.07", ERROR_OCCURED, appl_status_flag);
		        return appl_status_flag;  
		    }
		    if((SW_Oper(MODE_SW_IO_CH, DEV_ENABLE_OPER)) != SUCCESS)
		    {
				water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			    appl_status_flag = ERR_DEV_OPER_ENABLE_FUNC;
		        Error_or_Warning_Proc("11.15.08", ERROR_OCCURED, appl_status_flag);
		        return appl_status_flag;  
		    }
			ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			if(water_lvl_data.water_pump_state != WATER_PUMP_STATE_OFF)
			{
				water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_PUMP_TRIG_OFF;				
			}
            else
			{				
			    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_VALIDATE_PUMP_STATE;
				#ifdef TRACE_FLOW
			       Print("FLOW: INIT -> VALIDATE_PUMP_STATE \r"); 
			    #endif
			}		    
		break;
		case FSM_STATE_VALIDATE_PUMP_STATE:
		   read_state.read_or_test_consucc_val = 0;
	       read_state.start_bit_pos =  WATER_PUMP_CTRL_IO_CH % NUM_PINS_PER_PORT;
	       read_state.bits_len = 1;
	       if((ret_status = Port_Read(WATER_PUMP_CTRL_IO_CH, &read_state)) != SUCCESS)
	       {
			   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		       appl_status_flag = ERR_READ_PORT_VAL;					
	           Error_or_Warning_Proc("11.15.09", ERROR_OCCURED, appl_status_flag);
	           return appl_status_flag;
	       }
	       ret_status = Test_Consucc_Bits(FLAG_CONSUCC_BITS_1, &read_state);
		   switch(ret_status)
		   {
			    case TEST_OK_1_CONSUCC_BITS:
			       if((Test_Bit_Is_Clear_in_Data(&proc_data, 0)))
			       {
					   if(water_lvl_data.water_pump_state != WATER_PUMP_STATE_ON)
					   {
				         // WATER PUMP is not Incorrectly configured; 
			    		   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
				       	   appl_status_flag = ERR_WATER_PUMP_INCORRECT_STATE;							   
	                       Error_or_Warning_Proc("11.15.10", ERROR_OCCURED, appl_status_flag);
	                       return appl_status_flag;
					   }
					   else
					   {
						    if((LCD_Data_Disp(LCD_DISP_PUMP_ON)) != SUCCESS)
				             {
								 water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR; 
				                 appl_status_flag = ERR_LCD_WRITE_DATA_PROC;									
	                             Error_or_Warning_Proc("11.15.11", ERROR_OCCURED, appl_status_flag);
		                         return appl_status_flag;
					         }
					         Set_Bit_in_Data(&proc_data, 0); 
					         Clear_Bit_in_Data(&proc_data, 1); 
					         #ifdef TRACE_INFO
	                              Print("INFO: Water Pump - ON \r");
	                         #endif
					   }
				   }				   
			    break; 
                case TEST_FAIL_1_CONSUCC_BITS:
			         if((Test_Bit_Is_Clear_in_Data(&proc_data, 1)))
				     {
						 if(water_lvl_data.water_pump_state != WATER_PUMP_STATE_OFF)
						 {
				              // WATER PUMP is not Incorrectly configured; 
						      water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
				              appl_status_flag = ERR_WATER_PUMP_INCORRECT_STATE;
	                          Error_or_Warning_Proc("11.15.12", ERROR_OCCURED, appl_status_flag);
	                          return appl_status_flag;
						 }
						 else
						 {
							 if((LCD_Data_Disp(LCD_DISP_PUMP_OFF)) != SUCCESS)
					         {
					 			   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
					               appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	                               Error_or_Warning_Proc("11.15.13", ERROR_OCCURED, appl_status_flag);
		                           return appl_status_flag;
					         }
					         Set_Bit_in_Data(&proc_data, 1);
					         Clear_Bit_in_Data(&proc_data, 0); 
					         #ifdef TRACE_INFO
	                              Print("INFO: Water Pump - OFF \r");
	                         #endif
						 }	 
				     }				     
			     break; 
			     default:
				     water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			         appl_status_flag = ERR_WATER_PUMP_INCORRECT_STATE;
	                 Error_or_Warning_Proc("11.15.14", ERROR_OCCURED, appl_status_flag);
	                 return appl_status_flag;
		      
	        }
			ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_READ_MODE_SW;
			#ifdef TRACE_FLOW
			    if(temp_cur_water_lvl != water_lvl_data.cur_water_lvl || water_lvl_data.first_time_proc_flag == STATE_YES)
				{
			        Print("FLOW: VALIDATE_PUMP_STATE -> READ_MODE_SW \r"); 
				}					
			#endif
		break; 
		case FSM_STATE_READ_MODE_SW:
	 	    ret_status = SW_Oper(MODE_SW_IO_CH, DEV_READ_OPER);
	        switch(ret_status)
	        {
		        case SUCCESS:
		           switch(water_lvl_data.cur_oper_mode)
		           {
			  	       case AUTOMATIC_MODE:				    
			   	       break;
				       case OPER_MODE_INVALID:
			           case MANUAL_MODE:				    				    
                          if((LCD_Data_Disp(LCD_DISP_AUTO_MODE)) != SUCCESS)
			     	      {
							   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
						       appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	                           Error_or_Warning_Proc("11.15.15", ERROR_OCCURED, appl_status_flag);
		                       return appl_status_flag;
					      }
					      water_lvl_data.cur_oper_mode = AUTOMATIC_MODE;
					      water_lvl_data.changed_oper_mode_flag = STATE_YES;
				          #ifdef TRACE_INFO
	                             Print("INFO: Operator Mode: AUTOMATIC \r");
	                      #endif 
		              break;				 
			        }		       
		        break;
		        case SW_OR_KEY_NOT_PRESSED:
		            switch(water_lvl_data.cur_oper_mode)
		            {
			       	    case MANUAL_MODE:
			       	    break;
			       	    case AUTOMATIC_MODE:
				        case OPER_MODE_INVALID:
					       if((LCD_Data_Disp(LCD_DISP_MANUAL_MODE)) != SUCCESS)
					       {
							    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
						        appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	                            Error_or_Warning_Proc("11.15.16", ERROR_OCCURED, appl_status_flag);
		                        return appl_status_flag;;
					       }
						   water_lvl_data.cur_oper_mode = MANUAL_MODE;
						   water_lvl_data.changed_oper_mode_flag = STATE_YES;
                           #ifdef TRACE_INFO
	                            Print("INFO: Operator Mode: MANUAL \r");
	                       #endif		
				           if(water_lvl_data.water_pump_state == WATER_PUMP_STATE_ON)
						   {
							    water_lvl_data.func_state = WATER_PUMP_AUTO_TRIG_OFF;
							    #ifdef TRACE_INFO
	                                 Print("INFO: PUMP turned OFF Auto, MANUAL MODE \r");
	                            #endif
								ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
								water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_PUMP_TRIG_OFF;
								#ifdef TRACE_FLOW
			                        Print("FLOW: READ_MODE_SW -> PUMP_TRIG_OFF \r"); 
			                    #endif
								return SUCCESS; 
					        }                            				
		                break;
			        } 
		        break;
		        case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		        case WARN_CUR_DATA_ID_DEV_DISABLED:
				   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
				   appl_status_flag = ERR_DEV_DISABLE_PROC;
	               Error_or_Warning_Proc("11.15.17", ERROR_OCCURED, appl_status_flag);
		           return appl_status_flag;				    
		        break;
                default:
				    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
                  	appl_status_flag = ERR_SW_READ_PROC;
	                Error_or_Warning_Proc("11.15.18", ERROR_OCCURED, appl_status_flag);
	                return appl_status_flag;
	       }
		   ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
	       water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_READ_WATER_LVL_SENSORS;
		   #ifdef TRACE_FLOW
		      if(temp_cur_water_lvl != water_lvl_data.cur_water_lvl || water_lvl_data.first_time_proc_flag == STATE_YES)
	          {
				  Print("FLOW: READ_MODE_SW -> READ_WATER_LVL_SENSORS \r");
			  }
		   #endif 
		break; 
		case FSM_STATE_READ_WATER_LVL_SENSORS:
		   for( cur_sw_ch_id = 0; cur_sw_ch_id < NUM_SWS_IO_CH_MAP; ++cur_sw_ch_id)
		   {
			   temp_data  = STATE_NO;
			   if((SW_Present_State_By_IO_Ch(sws_io_ch_map_datas[cur_sw_ch_id], &temp_data)) != SUCCESS)
			   {
				    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			      	appl_status_flag = ERR_SW_READ_PROC;
	                Error_or_Warning_Proc("11.15.19", ERROR_OCCURED, appl_status_flag);
	                return appl_status_flag;
			   }
			   if(temp_data == STATE_YES)
			   {
			   	    Set_Bit_in_Data(&io_chs_map_data, cur_sw_ch_id);			
			   }
               else
			   {
				    Clear_Bit_in_Data(&io_chs_map_data, cur_sw_ch_id);			    				
		       }
		   }			   
		   temp_data  = NUM_SWS_IO_CH_MAP ;
		   if((Test_Bit_Is_Clear_in_Data(&proc_data, 5)))
		   {			
	          	for(cur_sw_ch_id = 0; cur_sw_ch_id < (NUM_SWS_IO_CH_MAP ); ++cur_sw_ch_id )
		        {
		          	if(Test_Bit_Is_Set_in_Data(&io_chs_map_data, cur_sw_ch_id ))
		           	{
			           	if(temp_data == NUM_SWS_IO_CH_MAP )
			   	        {
			               continue; 
			            }
			            else if(temp_data < NUM_SWS_IO_CH_MAP)
			            {
			    	        #ifdef TRACE_ERROR
			                    Print("ERR: Water Lvl Sensor(s) HIGH abnormal, Sensor Water Lvl: %u \r", (cur_sw_ch_id * WATER_LVL_RESOLUTION) );
			                #endif				
			  	            Set_Bit_in_Data(&proc_data, 5);  	
			            }
			        }
			        else
			        {
						if(temp_data == NUM_SWS_IO_CH_MAP)
			   	        {							
							temp_data = cur_sw_ch_id;				            
			            }
						if(temp_data <  NUM_SWS_IO_CH_MAP && (cur_sw_ch_id < (NUM_SWS_IO_CH_MAP - 1)) && (Test_Bit_Is_Set_in_Data(&io_chs_map_data, (cur_sw_ch_id + 1)))) 
			            {
				            #ifdef TRACE_ERROR
			                    Print("ERR: Water Lvl Sensor(s) LOW abnormal, Sensor Water Lvl: %u \r", (cur_sw_ch_id * WATER_LVL_RESOLUTION) );
			                #endif				
			   	            Set_Bit_in_Data(&proc_data, 5);  					
			            }	
			        }
		        }
		    } 			
			if(temp_data == 0)
		    {
		       	// Water level is at bottom most sensor (in our case water level == 0%) 
			    water_lvl_data.cur_water_lvl = (temp_data) * WATER_LVL_RESOLUTION; 			
		    }
		    else if(temp_data > 0 && temp_data <  NUM_SWS_IO_CH_MAP)
		    {
		         // Water level is between bottom most sensor and top most sensor (in our case water level water level > 0 % && < 100%) 
		         water_lvl_data.cur_water_lvl = (temp_data - 1 ) * WATER_LVL_RESOLUTION;                      	
		    }
		    else if(temp_data  == NUM_SWS_IO_CH_MAP)
		    {
			    // Water level is at top most sensor (in our case water level water level == 100%) 
			     water_lvl_data.cur_water_lvl = (NUM_SWS_IO_CH_MAP - 1 ) * WATER_LVL_RESOLUTION;
			}
			if((Test_Bit_Is_Set_in_Data(&proc_data, 5)))
		    {
				water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_WATER_LVL_SENSORS_ABNORMAL;
				#ifdef TRACE_FLOW
			        Print("FLOW: READ_WATER_LVL_SENSORS -> WATER_LVL_SENSORS_ABNORMAL \r"); 
			    #endif
				return SUCCESS;
			}
			//  WATER LEVEL SENSORS WORKING CORRECTLY 
			if(temp_data  == NUM_SWS_IO_CH_MAP)
			{
			     if(prev_water_lvl_sensor_map != io_chs_map_data || (water_lvl_data.first_time_proc_flag == STATE_YES))
		         {
		             switch(water_lvl_data.cur_oper_mode)
		             {
			             case AUTOMATIC_MODE:	
						   	water_lvl_data.func_state = WATER_PUMP_AUTO_TRIG_OFF;										
				            #ifdef TRACE_INFO
			                    Print("INFO: Water Overflow : %u - Pump: OFF, Auto \r", water_lvl_data.cur_water_lvl);						   
			                #endif	
							ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
							water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_PUMP_TRIG_OFF;  
							#ifdef TRACE_FLOW
			                    Print("FLOW: READ_WATER_LVL_SENSORS -> PUMP_TRIG_OFF \r"); 
			                #endif
							return SUCCESS;
                         break;	
                         case MANUAL_MODE:	
				            if(water_lvl_data.water_pump_state == WATER_PUMP_STATE_ON)
					        {
				                #ifdef TRACE_ERROR
			                        Print("WARN: Water Overflow : %u - Pump has been ON, Manual \r", water_lvl_data.cur_water_lvl);						   
			                    #endif	
					        }
					        else
					        {
					        	#ifdef TRACE_INFO
			                        Print("INFO: Water Overflow : %u - Pump has been OFF, Manual \r", water_lvl_data.cur_water_lvl);						   
			                    #endif	
					        }
                         break;
			         }                  	
			      }                  
		     }
		     if((temp_data == 0 || ( water_lvl_data.cur_water_lvl >= MIN_WATER_LVL_TRIG_TURN_ON_PUMP && water_lvl_data.cur_water_lvl < MAX_WATER_LVL_TRIG_TURN_ON_PUMP) || 
			     water_lvl_data.cur_water_lvl <= water_lvl_data.preset_water_lvl)  && water_lvl_data.water_pump_state == WATER_PUMP_STATE_OFF)
		     {			
		       	if((prev_water_lvl_sensor_map != io_chs_map_data) || water_lvl_data.changed_oper_mode_flag == STATE_YES || water_lvl_data.first_time_proc_flag == STATE_YES)
		    	{
			       	switch(water_lvl_data.cur_oper_mode)
		            {
				        case AUTOMATIC_MODE:
						    ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
				            water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_PUMP_TRIG_ON;							
				            #ifdef TRACE_INFO
				                 Print("INFO: Water Lvl :%u (LOW), Preset :%u - Pump: ON, Auto \r", water_lvl_data.cur_water_lvl, water_lvl_data.preset_water_lvl);						   
				            #endif 
                            water_lvl_data.func_state = WATER_PUMP_AUTO_TRIG_ON;	
				    		#ifdef TRACE_FLOW
			                     Print("FLOW: READ_WATER_LVL_SENSORS -> PUMP_TRIG_ON \r"); 
			                #endif 
							return SUCCESS; 
                        break;
                        case MANUAL_MODE:
					        #ifdef TRACE_ERROR
				                Print("WARN: Water Lvl : %u (LOW) - Pump: OFF, Manual \r", water_lvl_data.cur_water_lvl);						   
				            #endif						
                        break;	 				
			        }
				}			
			 }
			 ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			 water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_READ_FUNC_SW; 	
             #ifdef TRACE_FLOW
			     if(temp_cur_water_lvl != water_lvl_data.cur_water_lvl || water_lvl_data.first_time_proc_flag == STATE_YES)
	             {
			          Print("FLOW: READ_WATER_LVL_SENSORS -> READ_FUNC_SW \r"); 
			     }
			 #endif 	
		break;
		case FSM_STATE_READ_FUNC_SW:
		   switch(water_lvl_data.cur_oper_mode)
	       {
		        case MANUAL_MODE:
			       ret_status = SW_Oper(FUNC_SW_IO_CH, DEV_READ_OPER);
		           switch(ret_status)
	               { 
			           case SUCCESS:
			           	   if(water_lvl_data.water_pump_state == WATER_PUMP_STATE_OFF)
				           {
							   if((water_lvl_data.cur_water_lvl >= MIN_WATER_LVL_TRIG_TURN_ON_PUMP && water_lvl_data.cur_water_lvl < MAX_WATER_LVL_TRIG_TURN_ON_PUMP) || 
							       water_lvl_data.cur_water_lvl <= water_lvl_data.preset_water_lvl)
						       {									       
				                   water_lvl_data.func_state = WATER_PUMP_MANUAL_TRIG_ON;
						           #ifdef TRACE_INFO
	                                    Print("INFO: PUMP turned ON, MANUAL MODE \r");
	                               #endif	
								   ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
								   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_PUMP_TRIG_ON;
								   #ifdef TRACE_FLOW
			                           Print("FLOW: READ_FUNC_SW -> PUMP_TRIG_ON \r"); 
			                       #endif
                                   return SUCCESS;						  
							   }
						   }							   
				           else 
						   { 	   
				                if(water_lvl_data.water_pump_state == WATER_PUMP_STATE_ON)
					            {
					               	 water_lvl_data.func_state = WATER_PUMP_MANUAL_TRIG_OFF;
							         #ifdef TRACE_INFO
	                                     Print("INFO: PUMP turned OFF, MANUAL MODE \r");
	                                 #endif
									 ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
						    		 water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_PUMP_TRIG_OFF;
                                     #ifdef TRACE_FLOW
			                             Print("FLOW: READ_FUNC_SW -> PUMP_TRIG_OFF \r"); 
			                         #endif 
                                      return SUCCESS;										 
							    }
					       }				         					  
			           break;
			           case SW_OR_KEY_NOT_PRESSED:
					      if(water_lvl_data.cur_water_lvl >= MAX_WATER_LVL_TRIG_TURN_OFF_PUMP)
						  {
						  	  if(water_lvl_data.water_pump_state == WATER_PUMP_STATE_ON)
					          {
								  water_lvl_data.func_state = WATER_PUMP_AUTO_TRIG_OFF;
								  #ifdef TRACE_INFO
	                                  Print("INFO: PUMP turned OFF Auto, MANUAL MODE \r");
	                              #endif
								  ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
						    	  water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_PUMP_TRIG_OFF;	
                                  #ifdef TRACE_FLOW
			                          Print("FLOW: READ_FUNC_SW -> PUMP_TRIG_OFF \r"); 
			                      #endif 
                                  return SUCCESS;									   
							  }
						  }
			           break;
			           case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		               case WARN_CUR_DATA_ID_DEV_DISABLED:
						     water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		                     appl_status_flag = ERR_DEV_DISABLE_PROC;
	                         Error_or_Warning_Proc("11.15.20", ERROR_OCCURED, appl_status_flag);
		                     return appl_status_flag;	
						   break;
                           default:
						      water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
                	          appl_status_flag = ERR_SW_READ_PROC;
	                          Error_or_Warning_Proc("11.15.21", ERROR_OCCURED, appl_status_flag);
	                          return appl_status_flag;
	                }
		        break;
		        case AUTOMATIC_MODE:
		            ret_status = SW_Oper(FUNC_SW_IO_CH, DEV_READ_OPER);
			        switch(ret_status)
			        {
			       	    case SUCCESS:	
                            ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
                            water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_SET_PRESET_WATER_LVL;
						    #ifdef TRACE_FLOW
						        Print("FLOW: READ_FUNC_SW -> SET_PRESET_WATER_LVL  \r");
						    #endif
							return SUCCESS;
                        break;	
				        case SW_OR_KEY_NOT_PRESSED:
			            break;
			            case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		                case WARN_CUR_DATA_ID_DEV_DISABLED:
						     water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		                     appl_status_flag = ERR_DEV_DISABLE_PROC;
	                         Error_or_Warning_Proc("11.15.24", ERROR_OCCURED, appl_status_flag);
		                     return appl_status_flag;	
					    break;
                        default:
					       water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
                	       appl_status_flag = ERR_SW_READ_PROC;
	                       Error_or_Warning_Proc("11.15.25", ERROR_OCCURED, appl_status_flag);
	                       return appl_status_flag;
	                }
			    break;
                default:
				    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
                    appl_status_flag = ERR_FORMAT_INVALID;
	                Error_or_Warning_Proc("11.15.26", ERROR_OCCURED, appl_status_flag);
	               return appl_status_flag; 			 
		      }
			  ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
	    	  water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_DISP_CUR_WATER_LVL;
		      #ifdef TRACE_FLOW
		         if(temp_cur_water_lvl != water_lvl_data.cur_water_lvl || water_lvl_data.first_time_proc_flag == STATE_YES)
	             {
			         Print("FLOW: READ_FUNC_SW -> DISP_CUR_WATER_LVL \r"); 
			     }
		      #endif 
		break;
		case FSM_STATE_SET_PRESET_WATER_LVL:
		     Timer_Stop(CH_ID_01);						
			 if((Timer_Run(CH_ID_01, TIMER_ID_PRESET_WATER_LVL_LOCK)) != SUCCESS)
			 {
				  water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			      appl_status_flag = ERR_TIMER_RUN_PROC;
	              Error_or_Warning_Proc("11.15.31", ERROR_OCCURED, appl_status_flag);
		          return appl_status_flag;
			 }
			 if(Test_Bit_Is_Clear_in_Data(&proc_data, 2))
			 {
				  Set_Bit_in_Data(&proc_data, 2);
			 }
			 switch(water_lvl_data.water_pump_state) 
			 {
			      case WATER_PUMP_STATE_ON:
			          //while water pump is on, not allowed to set preset water level val
			          #ifdef TRACE_ERROR
	                      Print("WARN: Try to Set Preset Water Level, While PUMP ON, AUTO \r");
	                  #endif  
                  break;
                  case WATER_PUMP_STATE_OFF:
				      // set preset water level val					 
				      if( water_lvl_data.preset_water_lvl < MIN_PRESET_WATER_LVL || water_lvl_data.preset_water_lvl >= MAX_PRESET_WATER_LVL)
					  { 
  					       water_lvl_data.preset_water_lvl = MIN_PRESET_WATER_LVL;
					  }
					  else
				      {
					      water_lvl_data.preset_water_lvl += PRESET_WATER_LVL_INCREMENT_VAL;
					  }
                      if((LCD_Data_Disp(LCD_DISP_PRESET_WATER_LVL)) != SUCCESS)
					  {
						  water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
					   	  appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	                      Error_or_Warning_Proc("11.15.22", ERROR_OCCURED, appl_status_flag);
		                  return appl_status_flag;
					  }
					  #ifdef TRACE_INFO
	                      Print("DATA: Preset Water Lvl: %u \r",  water_lvl_data.preset_water_lvl);
	                  #endif
                  break;
                  default:
					  water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
                      appl_status_flag = ERR_FORMAT_INVALID;
	                  Error_or_Warning_Proc("11.15.23", ERROR_OCCURED, appl_status_flag);
	                  return appl_status_flag; 							 
	         }
			 ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
             water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_DISP_CUR_WATER_LVL;
		break;
		case FSM_STATE_PUMP_TRIG_OFF:
		    Timer_Stop(CH_ID_01);
            Set_Data_Lock_Proc(ALL_SET_DATA_LOCK);   
		    if((IO_Channel_Write(WATER_PUMP_CTRL_IO_CH, STATE_LOW)) != SUCCESS)
	        {
				  water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
	              appl_status_flag = ERR_IO_CH_WRITE;
	              Error_or_Warning_Proc("11.15.27", ERROR_OCCURED, appl_status_flag);
	              return appl_status_flag;
	        }
	        water_lvl_data.water_pump_state = WATER_PUMP_STATE_OFF;
			if((LCD_Data_Disp(LCD_DISP_PUMP_OFF)) != SUCCESS)
			{
			    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			    appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            Error_or_Warning_Proc("11.15.28", ERROR_OCCURED, appl_status_flag);
		        return appl_status_flag;;
			}
            if(ctrl_prev_fsm_state != FSM_STATE_INIT)
			{				
			    ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_DISP_CUR_WATER_LVL;
                #ifdef TRACE_FLOW
			       Print("FLOW: PUMP_TRIG_OFF -> DISP_CUR_WATER_LVL \r"); 
                #endif 	
			}
            else
			{
				ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_VALIDATE_PUMP_STATE;
				#ifdef TRACE_FLOW
			       Print("FLOW: PUMP_TRIG_OFF -> VALIDATE_PUMP_STATE \r"); 
			    #endif
			}				
		break;
		case FSM_STATE_PUMP_TRIG_ON:
		     if((IO_Channel_Write(WATER_PUMP_CTRL_IO_CH, STATE_HIGH)) != SUCCESS)
	         {
				   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
	               appl_status_flag = ERR_IO_CH_WRITE;
	               Error_or_Warning_Proc("11.15.29", ERROR_OCCURED, appl_status_flag);
	               return appl_status_flag;
	         }
			 if((LCD_Data_Disp(LCD_DISP_PUMP_ON)) != SUCCESS)
			 {
				   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			       appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	               Error_or_Warning_Proc("11.15.30", ERROR_OCCURED, appl_status_flag);
		           return appl_status_flag;
			 }
			 Set_Data_Lock_Proc(ALL_SET_DATA_LOCK);
			 if((Timer_Run(CH_ID_01, TIMER_ID_DRY_RUN)) != SUCCESS)
			 {
				   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			       appl_status_flag = ERR_TIMER_RUN_PROC;
	               Error_or_Warning_Proc("11.15.31", ERROR_OCCURED, appl_status_flag);
		           return appl_status_flag;
			 } 
			 water_lvl_data.water_pump_state = WATER_PUMP_STATE_ON;
			 ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			 water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_DISP_CUR_WATER_LVL;	
			 #ifdef TRACE_FLOW
			     Print("FLOW: PUMP_TRIG_ON -> DISP_CUR_WATER_LVL \r"); 
		     #endif 
	    break;
		case FSM_STATE_DISP_CUR_WATER_LVL:
		    if(prev_water_lvl_sensor_map != io_chs_map_data || water_lvl_data.first_time_proc_flag == STATE_YES)	          
            {
	           prev_water_lvl_sensor_map = io_chs_map_data;
	           #ifdef TRACE_INFO
		           Print("DATA: Water Level Sensor Map: 0X%x \r", io_chs_map_data);
		       #endif   
	        }	
	        if(temp_cur_water_lvl != water_lvl_data.cur_water_lvl || water_lvl_data.first_time_proc_flag == STATE_YES)
	        {
			   if((LCD_Data_Disp(LCD_DISP_CUR_WATER_LVL)) != SUCCESS)
	           {
				    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
	                appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	                Error_or_Warning_Proc("11.15.32", ERROR_OCCURED, appl_status_flag);
	                return appl_status_flag;
	           }
			   #ifdef TRACE_INFO
		          Print("DATA: Current Water Lvl : %u \r", water_lvl_data.cur_water_lvl);
		       #endif
			   if(water_lvl_data.cur_water_lvl == (temp_cur_water_lvl + WATER_LVL_RESOLUTION))
			   {
				   if(water_lvl_data.cur_water_lvl < MAX_WATER_LVL_SENSOR)
				   {
    				   if(water_lvl_data.water_pump_state == WATER_PUMP_STATE_ON)
					   {
					       Timer_Stop(CH_ID_01);
                           if((Timer_Run(CH_ID_01, TIMER_ID_DRY_RUN)) != SUCCESS)
			               {
				                water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
			                    appl_status_flag = ERR_TIMER_RUN_PROC;
	                            Error_or_Warning_Proc("11.15.31", ERROR_OCCURED, appl_status_flag);
		                        return appl_status_flag;
						   }
			           } 
				   }
			   }
			   temp_cur_water_lvl = water_lvl_data.cur_water_lvl; 	           			   
	        }
		    if((Test_Bit_Is_Set_in_Data(&proc_data, 5)))
		    {
				ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			    water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_IDLE;
				#ifdef TRACE_FLOW
			          Print("FLOW: DISP_CUR_WATER_LVL -> IDLE \r"); 
		        #endif
		    }
            else
            {	
		       ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
		       water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_VALIDATE_PUMP_STATE;
			   #ifdef TRACE_FLOW
			       if(temp_cur_water_lvl != water_lvl_data.cur_water_lvl || water_lvl_data.first_time_proc_flag == STATE_YES)
	               {
			            Print("FLOW: DISP_CUR_WATER_LVL -> VALIDATE_PUMP_STATE \r"); 
				   }
		       #endif
		    } 
		    if(water_lvl_data.changed_oper_mode_flag == STATE_YES)
	        {
				Set_Data_Lock_Proc(ALL_SET_DATA_LOCK);
	        	water_lvl_data.changed_oper_mode_flag = STATE_NO;
	        }
		    if(water_lvl_data.first_time_proc_flag == STATE_YES)
	        {
	           water_lvl_data.first_time_proc_flag = STATE_NO;
	        }
		break;
		case FSM_STATE_TIMEOUT_DRY_RUN:
		    if((Ctrl_Water_Lvl_Error_Proc(WATER_LVL_TIMEOUT_DRY_RUN_PROC)) != SUCCESS)
			{
				water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		        appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            return appl_status_flag;
			}
			#ifdef TRACE_ERROR
			    Print("ERR: Timeout due to next sensor not reached \r");
			#endif
			water_lvl_data.func_state = TIMEOUT_DRY_RUN; 
			ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_IDLE;
			#ifdef TRACE_FLOW
			    Print("FLOW: TIMEOUT_DRY_RUN -> IDLE \r"); 
		    #endif
		break;
		case FSM_STATE_WATER_LVL_SENSORS_ABNORMAL:
		    if((Ctrl_Water_Lvl_Error_Proc(WATER_LVL_SENSORS_ABNORMAL_PROC)) != SUCCESS)
			{
			   	water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		        appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	            return appl_status_flag;
			}
			#ifdef TRACE_ERROR
			    Print("ERR: Water Lvl Sensor abnormal, Current Water Lvl : %u \r", water_lvl_data.cur_water_lvl);
			#endif
			ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
			water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_DISP_CUR_WATER_LVL;
			#ifdef TRACE_FLOW
			    Print("FLOW: WATER_LVL_SENSORS_ABNORMAL -> DISP_CUR_WATER_LVL \r"); 
		    #endif
		break;
		case FSM_STATE_INTERNAL_ERROR:
		   ctrl_prev_fsm_state = water_lvl_data.ctrl_cur_fsm_state;
		   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_IDLE;
		   #ifdef TRACE_FLOW
			   Print("FLOW: INTERNAL_ERROR -> IDLE \r"); 
		   #endif
		break;
		default:
		   water_lvl_data.ctrl_cur_fsm_state = FSM_STATE_INTERNAL_ERROR;
		   appl_status_flag = ERR_FORMAT_INVALID;
	       Error_or_Warning_Proc("11.15.33", ERROR_OCCURED, appl_status_flag);
	       return appl_status_flag; 
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Ctrl_Water_Lvl_Error_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.17  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Ctrl_Water_Lvl_Error_Proc(const uint8_t error_proc)
{
	Timer_Stop(CH_ID_01);	
	if((IO_Channel_Write(INTERNAL_ERROR_LED_IO_CH, STATE_HIGH)) != SUCCESS)
	{
	    appl_status_flag = ERR_IO_CH_WRITE;
	    return appl_status_flag;
	}
	if((IO_Channel_Write(WATER_PUMP_CTRL_IO_CH, STATE_LOW)) != SUCCESS)
	{
	    appl_status_flag = ERR_IO_CH_WRITE;
	    return appl_status_flag;
	}
	if((SW_Oper(MODE_SW_IO_CH, DEV_DISABLE_OPER)) != SUCCESS)
	{
		appl_status_flag = ERR_DEV_OPER_DISABLE_FUNC;		  
		return appl_status_flag;
	}			
	if((SW_Oper(FUNC_SW_IO_CH, DEV_DISABLE_OPER)) != SUCCESS)
	{
		appl_status_flag = ERR_DEV_OPER_DISABLE_FUNC;		
		return appl_status_flag;
	}		
	water_lvl_data.water_pump_state = WATER_PUMP_STATE_OFF;
	water_lvl_data.func_state = INTERNAL_ERR;
	if((LCD_Data_Disp(LCD_DISP_PUMP_OFF)) != SUCCESS)
	{
	    appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	    return appl_status_flag;
	}
	switch(error_proc)
	{
		case WATER_LVL_INTERNAL_ERR_PROC:
		  if((LCD_Data_Disp(LCD_DISP_ERROR)) != SUCCESS)
	      {
	         appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	         return appl_status_flag;
	      }
		break;
        case WATER_LVL_SENSORS_ABNORMAL_PROC:
		  if((LCD_Data_Disp(LCD_DISP_SENSORS_ABNORMAL)) != SUCCESS)
	      {
	         appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	         return appl_status_flag;
	      }
        break;
        case WATER_LVL_TIMEOUT_DRY_RUN_PROC:
		  if((LCD_Data_Disp(LCD_DISP_DRY_RUN)) != SUCCESS)
	      {
	         appl_status_flag = ERR_LCD_WRITE_DATA_PROC;
	         return appl_status_flag;
	      }
        break;		
	}
	return SUCCESS;
}	
	
/*------------------------------------------------------------*
FUNCTION NAME  : Set_Data_Lock_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.18  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Set_Data_Lock_Proc(const uint8_t data_type_set_lock)	
{
	if(data_type_set_lock == ALL_SET_DATA_LOCK || data_type_set_lock == WATER_LVL_PRESET_SET_LOCK)
	{
	    if(Test_Bit_Is_Set_in_Data(&proc_data, 2))
	    {
            #ifdef TRACE_INFO
		    	Print("INFO: Preset Water Lvl change LOCKED \r");
		    #endif
		    Clear_Bit_in_Data(&proc_data, 2);
		    Timer_Stop(CH_ID_01); 
	        //TO_DO: save Preset water level in EEPROM	
		  }			
	}
	return SUCCESS;
}



#ifdef TIMER_MOD_ENABLE 
/*------------------------------------------------------------*
FUNCTION NAME  : Timer_0_Timeout_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.05  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Timer_0_Timeout_Proc(const uint16_t timeout_type, volatile const timer_or_counter_data_t *const timer_0_or_counter_data_ptr)
{
	uint16_t ret_status;
	
	if(timer_0_or_counter_data_ptr == NULL_DATA_PTR)
	{
		appl_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("11.05.01", ERROR_OCCURED, appl_status_flag);
		return appl_status_flag;
	}
	if(timer_0_or_counter_data_ptr->timer_or_counter_run_id != TMR_OR_COUNTER_SYSTEM_TICK_STATE)
	{
		appl_status_flag = ERR_TIMER_ID_EXCEEDS;
		Error_or_Warning_Proc("11.05.02", ERROR_OCCURED, appl_status_flag);
		return appl_status_flag;
	}
    /* --------------------------------------- User Code: Begin ------------------------------------------ */ 
	
	/* --------------------------------------- User Code: End   ------------------------------------------ */ 
	switch(timeout_type)
	{
		 case TMR_NO_MAX_NUM_TIMEOUT_YET_PROCESS:
		    ret_status = TMR_NO_MAX_NUM_TIMEOUT_PROC;
	     break;		
		 case TMR_AT_LAST_TIMEOUT_YET_PROCESS:
		   ret_status = TMR_AT_LAST_TIMEOUT_PROC;
		 break;  
		 case TMR_BEFORE_LAST_TIMEOUT_YET_PROCESS:
		   ret_status = TMR_BEFORE_LAST_TIMEOUT_PROC;
		 break;
         case TMR_MAX_TIMEOUT_YET_PROCESS:
		    ret_status = TMR_MAX_NUM_TIMEOUT_PROC; 
         break;		
         default:
             appl_status_flag = ERR_TMR_TIMEOUT_TYPE_INVALID;
		     Error_or_Warning_Proc("11.05.03", ERROR_OCCURED, appl_status_flag);
		     return appl_status_flag;
	}
    return ret_status;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Timer_1_Timeout_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.06  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Timer_1_Timeout_Proc(const uint16_t timeout_type, volatile const timer_or_counter_data_t *const timer_1_or_counter_data_ptr)
{
	 uint16_t ret_status;
    
    if(timer_1_or_counter_data_ptr == NULL_DATA_PTR)
	{
		appl_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("11.06.01", ERROR_OCCURED, appl_status_flag);
		return appl_status_flag;
	}
	if(timer_1_or_counter_data_ptr->timer_or_counter_run_id >= NUM_TIMER_AND_COUNTER_IDS)
	{
		appl_status_flag = ERR_TIMER_ID_EXCEEDS;
		Error_or_Warning_Proc("11.06.02", ERROR_OCCURED, appl_status_flag);
		return appl_status_flag;
	}
	/* --------------------------------------- User Code: Begin ------------------------------------------ */ 
	
		
	/* --------------------------------------- User Code: End   ------------------------------------------ */ 
	
	 switch(timeout_type)
	 {
		 case TMR_NO_MAX_NUM_TIMEOUT_YET_PROCESS:
		    ret_status = TMR_NO_MAX_NUM_TIMEOUT_PROC;
	     break;		
		 case TMR_AT_LAST_TIMEOUT_YET_PROCESS:
		   ret_status = TMR_AT_LAST_TIMEOUT_PROC;
		 break;  
		 case TMR_BEFORE_LAST_TIMEOUT_YET_PROCESS:
		   ret_status = TMR_BEFORE_LAST_TIMEOUT_PROC;
		 break;
         case TMR_MAX_TIMEOUT_YET_PROCESS:
		    ret_status = TMR_MAX_NUM_TIMEOUT_PROC; 
         break;		 
         default:
		     appl_status_flag = ERR_TMR_TIMEOUT_TYPE_INVALID;
		     Error_or_Warning_Proc("11.06.05", ERROR_OCCURED, appl_status_flag);
		     return appl_status_flag;
	 }
    return ret_status;	
}

#endif

#ifdef EXT_INTERRUPT_MOD_ENABLE
/*------------------------------------------------------------*
FUNCTION NAME  : Ext_Interrupt_0_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.07  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Ext_Interrupt_0_Proc(void)
{
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Ext_Interrupt_1_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.08  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Ext_Interrupt_1_Proc(void)
{
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Ext_Interrupt_2_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.09  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Ext_Interrupt_2_Proc(void)
{
	uint16_t ret_status;   
	
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Ext_Interrupt_3_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 11.10  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Ext_Interrupt_3_Proc(void)
{
	uint16_t ret_status;
	
	return SUCCESS;
}

#endif

		    
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
