/**************************************************************************
   FILE          :    lcd.c
 
   PURPOSE       :   LCD library 
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  CAUTION        :
	
  NOTE           :   
  
  CHANGE LOGS    :
  
  FILE ID        : 09  
	   
 **************************************************************************/

#include "main.h"

#ifdef LCD_MOD_ENABLE

/* ------------------------------ macro defination ------------------------------ */

/* ----------------------------- global variable defination --------------------- */
lcd_status_t lcd_status[NUM_OUTPUT_DEV_ID_LCD_CHS];

/* ----------------------------- global variable declaration -------------------- */


/* ----------------------------- global function declaration -------------------- */
static uint16_t LCD_Check_Busy(const uint8_t lcd_ch_id);
static uint16_t LCD_Read_Command(const uint8_t lcd_ch_id, uint8_t *const lcd_read_command);
static uint16_t Error_or_Warning_LCD_Proc(const uint8_t lcd_ch_id, const char *const error_trace_str, const uint8_t warn_or_error_format);
static uint16_t LCD_Write_Data_to_IO_Ch(const uint8_t lcd_ch_id, const uint8_t lcd_data_or_cmd, const uint8_t lcd_data_start_pin);
static uint16_t LCD_Read_Data_from_IO_Ch(const uint8_t lcd_ch_id, const uint8_t lcd_data_start_pin);

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Init

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.01

BUGS           :    
-*------------------------------------------------------------*/
uint16_t LCD_Init(const uint8_t lcd_ch_id, const uint8_t init_mode)
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	io_config_t lcd_config;
	uint32_t lcd_func_command = 0x20;	
	uint8_t lcd_io_ch_ctrl_index, lcd_io_ch_ctrl_start_index;
	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.01", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;	
	if(cur_lcd_ctrl_ptr->max_config_lines_in_index > cur_lcd_ctrl_ptr->max_avail_lines_in_index || cur_lcd_ctrl_ptr->max_config_cols_in_index > cur_lcd_ctrl_ptr->max_avail_cols_in_index)
	{
		 system_status_flag = ERR_LCD_CONFIG_INVALID;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.02", ERROR_OCCURED);
		 return system_status_flag;
	}
	lcd_config.signal = PIN_SIG_DIGITAL;
	lcd_config.func = IO_FUNC_GPIO;
	lcd_config.dir = IO_DIR_OUTPUT;
	lcd_config.state = STATE_LOW;
	lcd_config.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	lcd_config.port_pin_len = 1;	
	for(lcd_io_ch_ctrl_index = 0; lcd_io_ch_ctrl_index < 3; ++lcd_io_ch_ctrl_index)
	{
       lcd_config.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index];	   
	   if((IO_Channels_Func_Set(&lcd_config)) != SUCCESS)
	   {
	    	system_status_flag = ERR_GPIO_FUNC_SET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.03", ERROR_OCCURED);
		    return system_status_flag;
	   }
	}
	if(cur_lcd_ctrl_ptr->interface == DATA_INTERFACE_4_BITS)
	{
		lcd_io_ch_ctrl_start_index = 3 + 4;
	}
	else 
	{
		lcd_io_ch_ctrl_start_index = 3; 
	}
	for(lcd_io_ch_ctrl_index = lcd_io_ch_ctrl_start_index; lcd_io_ch_ctrl_index < 11; ++lcd_io_ch_ctrl_index)
	{
	   lcd_config.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index];	   
	   if((IO_Channels_Func_Set(&lcd_config)) != SUCCESS)
	   {
	    	system_status_flag = ERR_GPIO_FUNC_SET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.04", ERROR_OCCURED);
		    return system_status_flag;
	   }
	}	
	/* wait for more than 15ms after LCD VSS rises to 4.5V, Busy Flag in LCD (BF) cannot be checked */
	Delay_MS(REQ_TIME_DELAY_LCD_VSS_4500mV_IN_MS);
	if((LCD_Write_Command_NO_BF(lcd_ch_id, 0x30)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.05", ERROR_OCCURED);
		return system_status_flag;
	}
	/* wait for more than 4.1 ms, Busy Flag in LCD (BF) cannot be checked */
	 Delay_MS(REQ_TIME_DELAY_LCD_1_FONT_IN_MS);
    if((LCD_Write_Command_NO_BF(lcd_ch_id, 0x30)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.06", ERROR_OCCURED);
		return system_status_flag;
	}
	/* wait for more than 100 us, Busy Flag in LCD (BF) cannot be checked */
	Delay_MS(REQ_TIME_DELAY_LCD_2_FONT_IN_MS);
	if((LCD_Write_Command_NO_BF(lcd_ch_id, 0x30)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.07", ERROR_OCCURED);
		return system_status_flag;
	}
	Delay_MS(REQ_TIME_DELAY_LCD_2_FONT_IN_MS);
	if(cur_lcd_ctrl_ptr->interface == DATA_INTERFACE_4_BITS)
	{		
	     if((LCD_Write_Command_NO_BF(lcd_ch_id, 0x20)) != SUCCESS)
		 {
			system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.08", ERROR_OCCURED);
		    return system_status_flag; 
		 }			 
	}	
    if((Write_Bit_in_Data(&lcd_func_command, 4, cur_lcd_ctrl_ptr->interface)) != SUCCESS)
	{
		system_status_flag = ERR_DATA_WRITE_BITS_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.09", ERROR_OCCURED);
		return system_status_flag; 
	}
	if(cur_lcd_ctrl_ptr->max_config_lines_in_index + 1 == LINE_NUM_01)
	{
	    if((Write_Bit_in_Data(&lcd_func_command, 3, MAX_DISP_NUM_LINES_1)) != SUCCESS)
		{
			system_status_flag = ERR_DATA_WRITE_BITS_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.10", ERROR_OCCURED);
		    return system_status_flag; 
		}
	}
	else
	{
		 if((Write_Bit_in_Data(&lcd_func_command, 3, MAX_DISP_NUM_LINES_ALL)) != SUCCESS)
		 {
			 system_status_flag = ERR_DATA_WRITE_BITS_PROC;
		     Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.11", ERROR_OCCURED);
		     return system_status_flag;
		 } 
	}
	if((Write_Bit_in_Data(&lcd_func_command, 2, cur_lcd_ctrl_ptr->font)) != SUCCESS)
	{
		system_status_flag = ERR_DATA_WRITE_BITS_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.12", ERROR_OCCURED);
		return system_status_flag;
	}
	if((LCD_Write_Command(lcd_ch_id, lcd_func_command)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.13", ERROR_OCCURED);
		return system_status_flag; 
	}		
    if((LCD_Write_Command(lcd_ch_id, 0x08)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.14", ERROR_OCCURED);
		return system_status_flag; 
	}
	if((LCD_Clear_Screen(lcd_ch_id)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.15", ERROR_OCCURED);
		return system_status_flag;
	}
	if((LCD_Write_Command(lcd_ch_id, 0x06)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.16", ERROR_OCCURED);
		return system_status_flag;
	}		
	if((LCD_Write_Command(lcd_ch_id, 0x0E)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.01.17", ERROR_OCCURED);
		return system_status_flag;
	}
	lcd_status[lcd_ch_id].lcd_cur_disp_loc = RESET_LCD_DISP_LOC;
	lcd_status[lcd_ch_id].lcd_cur_input_loc = RESET_LCD_INPUT_LOC;
	return SUCCESS;
} 
/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Clear_Screen

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.02

BUGS           :  
-*------------------------------------------------------------*/
uint16_t LCD_Clear_Screen(const uint8_t lcd_ch_id)
{
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(TRACE_LCD_CH_ID, "09.02.01", ERROR_OCCURED);
		 return system_status_flag;
	}	
	if((LCD_Write_Command(lcd_ch_id, 0x01)) != SUCCESS)
	{
		system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.02.02", ERROR_OCCURED);
		return system_status_flag;
	}
	//__delay_us(LCD_CLEAR_EXEC_IN_USEC);	
	Delay_MS(REQ_TIME_DELAY_LCD_CLEAR_SCR_IN_MS);	
	return SUCCESS;
}
 
/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Write_Command_NO_BF

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.03

BUGS           :   
-*------------------------------------------------------------*/
uint16_t LCD_Write_Command_NO_BF(const uint8_t lcd_ch_id, const uint8_t lcd_cmd)
{
   if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
   {
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(TRACE_LCD_CH_ID, "09.03.01", ERROR_OCCURED);
		 return system_status_flag;
   } 
   //LCD_RW_PIN = 0;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][1], STATE_LOW)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.03.02", ERROR_OCCURED);
	   return system_status_flag;
   }   
   //LCD_RS_PIN = 0;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][0], STATE_LOW)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.03.03", ERROR_OCCURED);
	   return system_status_flag;
   }    
   //__delay_us(LCD_ADDR_SETUP_IN_USEC);   
    Delay_MS(REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS);   
   //  LCD_EN_PIN = 1;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_HIGH)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.03.04", ERROR_OCCURED);
	   return system_status_flag;
   } 	
   // __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
    Delay_MS(REQ_TIME_DELAY_LCD_ENABLE_PULSE_IN_MS); 
	if((LCD_Write_Data_to_IO_Ch(lcd_ch_id, lcd_cmd >> 4, 4)) != SUCCESS)
	{
		  system_status_flag = ERR_LCD_WRITE_BY_IO_CH_PROC;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.03.05", ERROR_OCCURED);
	      return system_status_flag;
	}		 
    //__delay_us(LCD_DATA_SETUP_IN_USEC);
	Delay_MS(REQ_TIME_DELAY_LCD_DATA_SETUP_IN_MS);
	// LCD_EN_PIN = 0;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_LOW)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.03.06", ERROR_OCCURED);
	   return system_status_flag;
   }
    //__delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
    Delay_MS(REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS);	
    return SUCCESS;	     
}
/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Write_Command

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.04

BUGS           :   
-*------------------------------------------------------------*/
uint16_t LCD_Write_Command(const uint8_t lcd_ch_id, const uint8_t lcd_cmd)
{
    lcd_ctrl_t *cur_lcd_ctrl_ptr; 
		  
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.01", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	
	if(cur_lcd_ctrl_ptr->check_bf == CHECK_BUSY_FLAG)
	{
		LCD_Check_Busy(lcd_ch_id); 
	}
    else
	{
		Delay_MS(REQ_TIME_DELAY_LCD_WRITE_CMD_IN_MS);        		
	}		
   //LCD_RW_PIN = 0;
   if(( IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][1], STATE_LOW)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.02", ERROR_OCCURED);
	   return system_status_flag;
   }	   
   //LCD_RS_PIN = 0;
   if(( IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][0], STATE_LOW)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.03", ERROR_OCCURED);
	   return system_status_flag;
   }    
     // __delay_us(LCD_ADDR_SETUP_IN_USEC); 
    Delay_MS(REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS);	
    //LCD_EN_PIN = 1;
	if(( IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_HIGH)) != SUCCESS)
    {
	   system_status_flag = ERR_IO_CH_WRITE;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.04", ERROR_OCCURED); 
	   return system_status_flag;
    } 	
    //__delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	Delay_MS(REQ_TIME_DELAY_LCD_ENABLE_PULSE_IN_MS);	
	if(cur_lcd_ctrl_ptr->interface == DATA_INTERFACE_4_BITS)
	{
	    if((LCD_Write_Data_to_IO_Ch(lcd_ch_id, lcd_cmd >> 4, 4)) != SUCCESS)
	    {
		    system_status_flag = ERR_LCD_WRITE_BY_IO_CH_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.05", ERROR_OCCURED);
	        return system_status_flag;
	    }
        //__delay_us(LCD_DATA_SETUP_IN_USEC);
		Delay_MS(REQ_TIME_DELAY_LCD_DATA_SETUP_IN_MS);			 
       // LCD_EN_PIN = 0;
	    if(( IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_LOW)) != SUCCESS)
        {
	        system_status_flag = ERR_IO_CH_WRITE;
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.06", ERROR_OCCURED);
	        return system_status_flag;
        } 	
	    //__delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 	 
        Delay_MS(REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS);	
	    //__delay_us(LCD_ADDR_SETUP_IN_USEC);   
		Delay_MS(REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS);
		//LCD_EN_PIN = 1;
		if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_HIGH)) != SUCCESS)
        {
	        system_status_flag = ERR_IO_CH_WRITE;
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.07", ERROR_OCCURED);
	        return system_status_flag;
        } 
		//  __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
		Delay_MS(REQ_TIME_DELAY_LCD_ENABLE_PULSE_IN_MS);		
	    if((LCD_Write_Data_to_IO_Ch(lcd_ch_id, lcd_cmd & 0x0F, 4)) != SUCCESS)
	    {
		    system_status_flag = ERR_LCD_WRITE_BY_IO_CH_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.08", ERROR_OCCURED);
	        return system_status_flag;
	    }
	}
    else
	{	
        if((LCD_Write_Data_to_IO_Ch(lcd_ch_id, lcd_cmd, 0)) != SUCCESS)
	    {
		    system_status_flag = ERR_LCD_WRITE_BY_IO_CH_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.09", ERROR_OCCURED);
	        return system_status_flag;
	    }
	}
	 //__delay_us(LCD_DATA_SETUP_IN_USEC);
	 Delay_MS(REQ_TIME_DELAY_LCD_DATA_SETUP_IN_MS);     
    // LCD_EN_PIN = 0;
    if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_LOW)) != SUCCESS)
    {
	   system_status_flag = ERR_IO_CH_WRITE;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.04.10", ERROR_OCCURED);
	   return system_status_flag;
    }
    //__delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
    Delay_MS(REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS);	
    return SUCCESS;	      
}
/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disp_Char

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.05

BUGS           :  
-*------------------------------------------------------------*/
 uint16_t LCD_Disp_Char(const uint8_t lcd_ch_id, const char lcd_data)
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr;
	lcd_status_t *cur_lcd_status_ptr;
	lcd_point_t cur_lcd_loc_point_num;
	uint8_t cur_lcd_loc;
  
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.01", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;
	if(cur_lcd_status_ptr->data_till_end_point_flag == STATE_YES)
	{
    	if(cur_lcd_status_ptr->lcd_input_loc_flag == STATE_YES)
	    {
			cur_lcd_loc = cur_lcd_status_ptr->lcd_cur_input_loc;			
		}
		else 
		{
			cur_lcd_loc = cur_lcd_status_ptr->lcd_cur_disp_loc;
		}
		if((LCD_From_Loc_to_XY(lcd_ch_id, cur_lcd_loc, &cur_lcd_loc_point_num)) != SUCCESS)
		{
			system_status_flag = ERR_LCD_FROM_LOC_TO_XY_PROC;
	        Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.02", ERROR_OCCURED);
	        return system_status_flag;
		}			
		if(cur_lcd_loc_point_num.line_num_or_index == cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index )
		{
			if(cur_lcd_loc_point_num.col_num_or_index + 1 > cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index)
			{
			    system_status_flag = WARN_LCD_END_POINT_REACHED;
	            Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.03", WARNING_OCCURED);
	            return SUCCESS;	
			}
		}			
	}
	if(cur_lcd_ctrl_ptr->check_bf == CHECK_BUSY_FLAG)
	{
		LCD_Check_Busy(lcd_ch_id); 
	}
	else
	{
		Delay_MS(REQ_TIME_DELAY_LCD_WRITE_CMD_IN_MS);	
	}
	//LCD_RW_PIN = 0;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][1], STATE_LOW)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.04", ERROR_OCCURED); 
	   return system_status_flag;
   }	   
   //LCD_RS_PIN = 1;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][0], STATE_HIGH)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.05", ERROR_OCCURED);
	   return system_status_flag;
   } 	
	// __delay_us(LCD_ADDR_SETUP_IN_USEC); 
     Delay_MS(REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS);	
    //LCD_EN_PIN = 1;
	if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_HIGH)) != SUCCESS)
    {
	   system_status_flag = ERR_IO_CH_WRITE;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.06", ERROR_OCCURED);
	   return system_status_flag;
    } 	
    //__delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
	Delay_MS(REQ_TIME_DELAY_LCD_ENABLE_PULSE_IN_MS);	 
	 
	if(cur_lcd_ctrl_ptr->interface == DATA_INTERFACE_4_BITS)
	{
	    if((LCD_Write_Data_to_IO_Ch(lcd_ch_id, lcd_data >> 4, 4)) != SUCCESS)
	    {
		    system_status_flag = ERR_LCD_WRITE_BY_IO_CH_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.07", ERROR_OCCURED);
	        return system_status_flag;
	    }
        //__delay_us(LCD_DATA_SETUP_IN_USEC);
		Delay_MS(REQ_TIME_DELAY_LCD_DATA_SETUP_IN_MS);	
       // LCD_EN_PIN = 0;
	    if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_LOW)) != SUCCESS)
        {
	        system_status_flag = ERR_IO_CH_WRITE;
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.08", ERROR_OCCURED);
	        return system_status_flag;
        } 	
	    //__delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 	 
        Delay_MS(REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS);	
        //__delay_us(LCD_ADDR_SETUP_IN_USEC);   
		Delay_MS(REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS);	 
        //LCD_EN_PIN = 1;
		if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_HIGH)) != SUCCESS)
        {
	        system_status_flag = ERR_IO_CH_WRITE;
			Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.09", ERROR_OCCURED); 
	        return system_status_flag;
        } 
		//  __delay_us(LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC);
		Delay_MS(REQ_TIME_DELAY_LCD_ENABLE_PULSE_IN_MS);	
	    if((LCD_Write_Data_to_IO_Ch(lcd_ch_id, lcd_data & 0x0F, 4)) != SUCCESS)
	    {
		    system_status_flag = ERR_LCD_WRITE_BY_IO_CH_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.10", ERROR_OCCURED);
	        return system_status_flag;
	    }
	}
    else
	{	
        if((LCD_Write_Data_to_IO_Ch(lcd_ch_id, lcd_data, 0)) != SUCCESS)
	    {
		    system_status_flag = ERR_LCD_WRITE_BY_IO_CH_PROC;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.11", ERROR_OCCURED);
	        return system_status_flag;
	    }
	}
	 //__delay_us(LCD_DATA_SETUP_IN_USEC);
	Delay_MS(REQ_TIME_DELAY_LCD_DATA_SETUP_IN_MS);	
    // LCD_EN_PIN = 0;
    if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_LOW)) != SUCCESS)
    {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.05.12", ERROR_OCCURED); 
	   return system_status_flag;
    }
    //__delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
    Delay_MS(REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS);	
    return SUCCESS;	      
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Check_Busy

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.06

BUGS           :   
-*------------------------------------------------------------*/
static uint16_t LCD_Check_Busy(const uint8_t lcd_ch_id)
{
  lcd_ctrl_t *cur_lcd_ctrl_ptr;
  io_config_t lcd_config;
  uint8_t lcd_read_command, lcd_io_ch_ctrl_index, lcd_io_ch_ctrl_start_index;
	
    if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.01", ERROR_OCCURED);
		 return system_status_flag;
	}	
    //LCD_RW_PIN = 1;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][1], STATE_HIGH)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.02", ERROR_OCCURED); 
	   return system_status_flag;
   }	   
   //LCD_RS_PIN = 0;
   if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][0], STATE_LOW)) != SUCCESS)
   {
	   system_status_flag = ERR_IO_CH_WRITE;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.03", ERROR_OCCURED); 
	   return system_status_flag;
   } 		
    cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	lcd_config.port_pin_len = 1; 
	lcd_config.signal = PIN_SIG_DIGITAL;
	lcd_config.dir = IO_DIR_OUTPUT;
	lcd_config.func = IO_FUNC_GPIO;
	lcd_config.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	if(cur_lcd_ctrl_ptr->interface == DATA_INTERFACE_4_BITS)
	{
		lcd_io_ch_ctrl_start_index = 3 + 4;
	}
	else 
	{
		lcd_io_ch_ctrl_start_index = 3; 
	}	
	for(lcd_io_ch_ctrl_index = lcd_io_ch_ctrl_start_index; lcd_io_ch_ctrl_index < 11; ++lcd_io_ch_ctrl_index)
	{
	   lcd_config.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index];	   
	   if((IO_Ch_Func_Reset(&lcd_config)) != SUCCESS)
	   {
	    	system_status_flag = ERR_IO_CH_FUNC_RESET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.04", ERROR_OCCURED);
		    return system_status_flag;
	   }
	}	
	lcd_config.dir = IO_DIR_INPUT;
	for(lcd_io_ch_ctrl_index = lcd_io_ch_ctrl_start_index; lcd_io_ch_ctrl_index < 11; ++lcd_io_ch_ctrl_index)
	{
	   lcd_config.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index];	   
	   if((IO_Channels_Func_Set(&lcd_config)) != SUCCESS)
	   {
	    	system_status_flag = ERR_GPIO_FUNC_SET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.05", ERROR_OCCURED); 
	        return system_status_flag;
	    }
	}
	// send higher nibble first and higher data nibble of LCD PORT must be connected to LCD 	 
     /* busy flag = Bit 7 in LCD PORT, if busy flag == 1, wait till busy flag = 0, then any operation on LCD can be done */
    do
	{
		if((LCD_Read_Command(lcd_ch_id, &lcd_read_command)) != SUCCESS)
		{
			system_status_flag = ERR_LCD_READ_PROC;
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.06", ERROR_OCCURED); 
			return system_status_flag;
		}
    }
	while(Test_Bit_Is_Set_in_Data(&lcd_read_command, 7 ));	
	for(lcd_io_ch_ctrl_index = lcd_io_ch_ctrl_start_index; lcd_io_ch_ctrl_index < 11; ++lcd_io_ch_ctrl_index)
	{
	   lcd_config.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index];	   
	   if((IO_Ch_Func_Reset(&lcd_config)) != SUCCESS)
	   {
	    	system_status_flag = ERR_IO_CH_FUNC_RESET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.07", ERROR_OCCURED);
		    return system_status_flag;
	   }
	}	
	lcd_config.dir = IO_DIR_OUTPUT;
	lcd_config.state = STATE_LOW;
	for(lcd_io_ch_ctrl_index = lcd_io_ch_ctrl_start_index; lcd_io_ch_ctrl_index < 11; ++lcd_io_ch_ctrl_index)
	{
	   lcd_config.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index];	   
	   if((IO_Channels_Func_Set(&lcd_config)) != SUCCESS)
	   {
	    	system_status_flag = ERR_GPIO_FUNC_SET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.06.08", ERROR_OCCURED); 
	        return system_status_flag;
	    }
	}
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Read_Command

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.07

BUGS           : 
-*------------------------------------------------------------*/
static uint16_t LCD_Read_Command(const uint8_t lcd_ch_id, uint8_t *const lcd_read_command_ptr)
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr;
	lcd_status_t *cur_lcd_status_ptr;
	
	if(lcd_read_command_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("09.07.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.02", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;
	// __delay_us(LCD_ADDR_SETUP_IN_USEC);  
     Delay_MS(REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS);	
    //LCD_EN_PIN = 1;
	if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_HIGH)) != SUCCESS)
    {
	   system_status_flag = ERR_IO_CH_WRITE;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.03", ERROR_OCCURED);
	   return system_status_flag;
    } 	        
	if(cur_lcd_ctrl_ptr->interface == DATA_INTERFACE_4_BITS)
	{ 
        if((LCD_Read_Data_from_IO_Ch(lcd_ch_id, 4)) != SUCCESS)
		{
			system_status_flag = ERR_LCD_READ_BY_IO_CH_PROC;
			Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.04", ERROR_OCCURED);
			return system_status_flag;
		}
	    //LCD_EN_PIN = 0;		
	    if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_LOW)) != SUCCESS)
        {
	       system_status_flag = ERR_IO_CH_WRITE;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.05", ERROR_OCCURED);
	       return system_status_flag;
        } 	
	     *lcd_read_command_ptr = (uint8_t)cur_lcd_status_ptr->lcd_data_or_cmd << 4;	 
	     //__delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
		 Delay_MS(REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS); 
		 
	     //__delay_us(LCD_ADDR_SETUP_IN_USEC);
		 Delay_MS(REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS);
        //  LCD_EN_PIN = 1;
		 if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_HIGH)) != SUCCESS)
         {
	        system_status_flag = ERR_IO_CH_WRITE;
			Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.06", ERROR_OCCURED);
	        return system_status_flag;
         } 	
		 cur_lcd_status_ptr->lcd_data_or_cmd = 0;
	     // read lower nibble and higher port nibble of LCD PORT is connected to LCD(DB4 - DB7)	
		 if((LCD_Read_Data_from_IO_Ch(lcd_ch_id, 4)) != SUCCESS)
		 {
			system_status_flag = ERR_LCD_READ_BY_IO_CH_PROC;
			Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.07", ERROR_OCCURED);
			return system_status_flag;
		 }
          *lcd_read_command_ptr = *lcd_read_command_ptr | ( (uint8_t)cur_lcd_status_ptr->lcd_data_or_cmd & 0x0Fu);	   
	 }
	 else
	 {
		 if((LCD_Read_Data_from_IO_Ch(lcd_ch_id, 0)) != SUCCESS)
		 {
			system_status_flag = ERR_LCD_READ_BY_IO_CH_PROC;
			Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.08", ERROR_OCCURED);
			return system_status_flag;
		 }
		  *lcd_read_command_ptr  = (uint8_t)cur_lcd_status_ptr->lcd_data_or_cmd ;
	 }
	  //LCD_EN_PIN = 0;
	 if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][2], STATE_LOW)) != SUCCESS)
     {
	       system_status_flag = ERR_IO_CH_WRITE;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.07.09", ERROR_OCCURED);
	       return system_status_flag;
     } 		 
	 //__delay_us(LCD_OTHER_INST_EXEC_IN_USEC); 
	Delay_MS(REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS);  
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disp_Str

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.08

BUGS           :   
-*------------------------------------------------------------*/
uint16_t LCD_Disp_Str(const uint8_t lcd_ch_id, const char *const char_ptr)
{ 
      char cur_char;
	    uint8_t num_char = 0;
  
	   if(char_ptr == NULL_DATA_PTR)
	   {
		   system_status_flag = ERR_NULL_PTR;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.08.01", ERROR_OCCURED);
		   return system_status_flag;
	   }	   
	   if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	   {
		   system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.08.02", ERROR_OCCURED);
		   return system_status_flag;
	   }
       while(*(char_ptr + num_char))
       {
		    cur_char = *(char_ptr + num_char);
            if((LCD_Disp_Char(lcd_ch_id, cur_char)) != SUCCESS)
			{
				system_status_flag = ERR_LCD_WRITE_DATA_PROC;
		        Error_or_Warning_LCD_Proc(lcd_ch_id, "09.08.03", ERROR_OCCURED);
		        return system_status_flag;
			}
            ++num_char;        
       }	   
	   return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Config_Cur_Loc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : make sure that loc are within avail lcd loc limit

Func ID        : 09.10

BUGS           :  
-*------------------------------------------------------------*/
uint16_t LCD_Config_Cur_Loc(const uint8_t lcd_ch_id, const uint8_t set_loc_ctrl_flag, const uint8_t set_input_loc, const uint8_t set_disp_loc)
{
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
	   system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
	   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.10.01", ERROR_OCCURED);
	   return system_status_flag;
	}
    if((set_loc_ctrl_flag & (1 << 0))) 
	{		
       lcd_status[lcd_ch_id].lcd_cur_input_loc = set_input_loc;
	}
    if((set_loc_ctrl_flag & (1 << 1)))
    {
       lcd_status[lcd_ch_id].lcd_cur_disp_loc = set_disp_loc; 
    }
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disp_Goto_XY

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : max 4 lines and 20 columns

Func ID        : 09.11

BUGS           :    
-*------------------------------------------------------------*/
uint16_t LCD_Disp_Goto_XY(const uint8_t lcd_ch_id, const uint8_t start_line_num, const uint8_t start_col_num)
{
	/* for us, lcd line starts from 1, but for uC, line index starts from 0 */
	/* for us, lcd col starts from 1, but for uC, col index starts from 0 */
	lcd_ctrl_t *cur_lcd_ctrl_ptr; 
	lcd_status_t *cur_lcd_status_ptr;
	lcd_point_t start_lcd_disp_point_index;
	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.11.01", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;
	start_lcd_disp_point_index.line_num_or_index = start_line_num - 1;
	start_lcd_disp_point_index.col_num_or_index = start_col_num - 1; 	
   if(start_lcd_disp_point_index.line_num_or_index > cur_lcd_ctrl_ptr->max_config_lines_in_index || start_lcd_disp_point_index.col_num_or_index > cur_lcd_ctrl_ptr->max_config_cols_in_index )
   {
	    #ifdef TRACE_ERROR
		   uint32_temp_disp_data = (uint32_t)start_lcd_disp_point_index.line_num_or_index;
		   Print("ERR: line = %u, ", uint32_temp_disp_data);
		   uint32_temp_disp_data = (uint32_t)cur_lcd_ctrl_ptr->max_config_lines_in_index; 
		   Print("max_line = %u & ", uint32_temp_disp_data);
		   uint32_temp_disp_data = (uint32_t)start_lcd_disp_point_index.col_num_or_index; 
		   Print("col = %u, ", uint32_temp_disp_data); 
		   uint32_temp_disp_data = (uint32_t) cur_lcd_ctrl_ptr->max_config_cols_in_index;
		   Print("max_col = %u\r", uint32_temp_disp_data); 
		#endif
	   /* error due to invalid lcd DISP loc  */
	     system_status_flag = ERR_LCD_LOC_INVALID;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.11.02", ERROR_OCCURED);
		 return system_status_flag;
   }
     switch(start_line_num)
	 {
		 case LINE_NUM_01:
		   cur_lcd_status_ptr->lcd_cur_disp_loc = BEGIN_LOC_LINE_NUM_01;
		 break;
		 case LINE_NUM_02:
		   cur_lcd_status_ptr->lcd_cur_disp_loc = BEGIN_LOC_LINE_NUM_02;
		 break;
		 case LINE_NUM_03:
		   cur_lcd_status_ptr->lcd_cur_disp_loc = BEGIN_LOC_LINE_NUM_03;
		 break;
		 case LINE_NUM_04:
		   cur_lcd_status_ptr->lcd_cur_disp_loc = BEGIN_LOC_LINE_NUM_04;
		 break;         		 
	 }	
      cur_lcd_status_ptr->lcd_cur_disp_loc += start_lcd_disp_point_index.col_num_or_index;
      if((LCD_Write_Command(lcd_ch_id, cur_lcd_status_ptr->lcd_cur_disp_loc)) != SUCCESS)
	  {
		  system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.11.03", ERROR_OCCURED);
		  return system_status_flag;
	  }		  
    return SUCCESS;   
} 

 /*------------------------------------------------------------*
FUNCTION NAME  : LCD_Input_Goto_XY

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : max 4 lines and 20 columns

Func ID        : 09.12

BUGS           :    
-*------------------------------------------------------------*/ 
uint16_t LCD_Input_Goto_XY(const uint8_t lcd_ch_id, const uint8_t start_line_num, const uint8_t start_col_num)
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr; 
	lcd_status_t *cur_lcd_status_ptr;
	/* for us, lcd line starts from 1, but for uC, line index starts from 0 */
	/* for us, lcd col starts from 1, but for uC, col index starts from 0 */
	lcd_point_t start_lcd_input_point_index;    
	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.12.01", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;
  start_lcd_input_point_index.line_num_or_index = start_line_num - 1;
	start_lcd_input_point_index.col_num_or_index = start_col_num - 1;
	if(start_lcd_input_point_index.line_num_or_index > cur_lcd_ctrl_ptr->max_config_lines_in_index || start_lcd_input_point_index.col_num_or_index > cur_lcd_ctrl_ptr->max_config_cols_in_index )
   {
	   /* error due to invalid lcd DISP loc  */
	     system_status_flag = ERR_LCD_LOC_INVALID;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.12.02", ERROR_OCCURED);
		 return system_status_flag;
   }
	 switch(start_line_num)
	 {
		 case LINE_NUM_01:
		   cur_lcd_status_ptr->lcd_cur_input_loc = BEGIN_LOC_LINE_NUM_01;
		 break;
		 case LINE_NUM_02:
		   cur_lcd_status_ptr->lcd_cur_input_loc = BEGIN_LOC_LINE_NUM_02;
		 break;
		 case LINE_NUM_03:
		   cur_lcd_status_ptr->lcd_cur_input_loc = BEGIN_LOC_LINE_NUM_03;
		 break;
		 case LINE_NUM_04:
		    cur_lcd_status_ptr->lcd_cur_input_loc = BEGIN_LOC_LINE_NUM_04;
		 break; 		  
	 }
	 cur_lcd_status_ptr->lcd_cur_input_loc += start_lcd_input_point_index.col_num_or_index;
     if((LCD_Write_Command(lcd_ch_id, cur_lcd_status_ptr->lcd_cur_input_loc)) != SUCCESS)
	 {
		  system_status_flag = ERR_LCD_WRITE_CMD_PROC;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.12.03", ERROR_OCCURED);
		  return system_status_flag;
	 }	  
     return SUCCESS;
} 

 /*------------------------------------------------------------*
FUNCTION NAME  : LCD_From_XY_To_Loc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           :  max 4 lines and 20 columns

Func ID        : 09.13

BUGS           :  
-*------------------------------------------------------------*/
uint16_t LCD_From_XY_To_Loc(const uint8_t lcd_ch_id, const lcd_point_t *const lcd_point_num_ptr, uint8_t *const lcd_loc_ptr)
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr; 
	lcd_point_t lcd_point_index;
	
	if(lcd_loc_ptr == NULL_DATA_PTR || lcd_point_num_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.13.01", ERROR_OCCURED);
		return system_status_flag;
	}	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.13.02", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	lcd_point_index.line_num_or_index = lcd_point_num_ptr->line_num_or_index - 1;
	lcd_point_index.col_num_or_index = lcd_point_num_ptr->col_num_or_index - 1;
	if(lcd_point_index.line_num_or_index > cur_lcd_ctrl_ptr->max_config_lines_in_index || lcd_point_index.col_num_or_index > cur_lcd_ctrl_ptr->max_config_cols_in_index )
   {
	     system_status_flag = ERR_LCD_LOC_INVALID;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.13.03", ERROR_OCCURED); 
		 return system_status_flag;
   }
     switch(lcd_point_num_ptr->line_num_or_index)
	 {
		 case LINE_NUM_01:
		   *lcd_loc_ptr = BEGIN_LOC_LINE_NUM_01;
		   break;
		 case LINE_NUM_02:
		   *lcd_loc_ptr = BEGIN_LOC_LINE_NUM_02;
		   break;
		 case LINE_NUM_03:
		   *lcd_loc_ptr= BEGIN_LOC_LINE_NUM_03;
		   break;
		  case LINE_NUM_04:
		  *lcd_loc_ptr = BEGIN_LOC_LINE_NUM_04;
		   break;		 
	 }	
      *lcd_loc_ptr = *lcd_loc_ptr + lcd_point_index.col_num_or_index;           
    return SUCCESS;   
} 

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_From_Loc_to_XY

DESCRIPTION    : loc_lcd's corrosponding line num and col num 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.14

BUGS           :   
-*------------------------------------------------------------*/
uint16_t LCD_From_Loc_to_XY(const uint8_t lcd_ch_id, const uint8_t loc_lcd, lcd_point_t *const lcd_point_num_ptr)
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr; 
		
	if(lcd_point_num_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.14.01", ERROR_OCCURED);
		return system_status_flag;
	}
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.14.02", ERROR_OCCURED);
		 return system_status_flag;
	}
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;
	if(loc_lcd >= BEGIN_LOC_LINE_NUM_01 && loc_lcd <= END_LOC_LINE_NUM_01)
	{
	    lcd_point_num_ptr->line_num_or_index = LINE_NUM_01;
	    lcd_point_num_ptr->col_num_or_index = loc_lcd + 1 - BEGIN_LOC_LINE_NUM_01;	
        goto LOC_TO_XY_EXIT;	    
	}
    if(loc_lcd >= BEGIN_LOC_LINE_NUM_02 && loc_lcd <= END_LOC_LINE_NUM_02)
	{
	    lcd_point_num_ptr->line_num_or_index	= LINE_NUM_02;
		lcd_point_num_ptr->col_num_or_index =  loc_lcd + 1 - BEGIN_LOC_LINE_NUM_02; 
        goto LOC_TO_XY_EXIT;		
	}     
	if(loc_lcd >= BEGIN_LOC_LINE_NUM_03 && loc_lcd <= END_LOC_LINE_NUM_03)
	{
	    lcd_point_num_ptr->line_num_or_index = LINE_NUM_03;
	  	lcd_point_num_ptr->col_num_or_index = loc_lcd + 1 - BEGIN_LOC_LINE_NUM_03;
        goto LOC_TO_XY_EXIT; 		
	}
    if(loc_lcd >= BEGIN_LOC_LINE_NUM_04 && loc_lcd <= END_LOC_LINE_NUM_04)
	{
		lcd_point_num_ptr->line_num_or_index = LINE_NUM_04;
		lcd_point_num_ptr->col_num_or_index = loc_lcd + 1 - BEGIN_LOC_LINE_NUM_04;
		goto LOC_TO_XY_EXIT;
	}
    system_status_flag = ERR_LCD_LOC_INVALID; 
	 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.14.03", ERROR_OCCURED);
	return system_status_flag;
 	LOC_TO_XY_EXIT: 
	if((lcd_point_num_ptr->line_num_or_index - 1) > cur_lcd_ctrl_ptr->max_config_lines_in_index || (lcd_point_num_ptr->col_num_or_index - 1) > cur_lcd_ctrl_ptr->max_config_cols_in_index )
	{
			 system_status_flag = ERR_LCD_LOC_INVALID; 
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.14.04", ERROR_OCCURED);
	         return system_status_flag;
	}
	return SUCCESS;  
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Write

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.15

BUGS           :   
-*------------------------------------------------------------*/
uint16_t LCD_Write(const uint8_t lcd_ch_id, const void *const lcd_rcv_disp_num_ptr)
{
	data_write_ctrl_ptr_t *lcd_data_write_ptr;	
    lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	lcd_disp_ctrl_t *lcd_disp_ctrl_ptr; 
	lcd_status_t *cur_lcd_status_ptr = NULL_DATA_PTR;
	char retrieved_lcd_data_str[DATA_MAX_NUM_ALLOCATED_CHARS], *cur_lcd_disp_str;
	int32_t lcd_disp_num_int32;
	uint8_t output_data_type ;    
	
    if(lcd_rcv_disp_num_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.01", ERROR_OCCURED);
		return system_status_flag;
	}
    if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		   system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.02", ERROR_OCCURED);
		   return system_status_flag;
	}		
	cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id;   
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;
	if(  cur_lcd_ctrl_ptr->access_flag != STATE_YES) 
	{  
        system_status_flag = WARN_CUR_DATA_ID_DEV_NO_ACCESS;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.03", WARNING_OCCURED);
        return  system_status_flag;
	}		 
	   if( cur_lcd_ctrl_ptr->enable_flag != STATE_YES) 
	   {  
          system_status_flag = WARN_CUR_DATA_ID_DEV_DISABLED;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.04", WARNING_OCCURED);
          return  system_status_flag;
	   }     
     lcd_data_write_ptr  = (data_write_ctrl_ptr_t *)lcd_rcv_disp_num_ptr;
	 lcd_disp_ctrl_ptr = &(((dev_write_ctrl_para_t *)lcd_data_write_ptr->dev_output_ctrl_para_ptr)->lcd_disp_para);
     if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_OUTPUT_DATA_TYPE, &output_data_type)) != SUCCESS)
	 {
		  system_status_flag = ERR_APPL_DATA_RETRIEVE;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.05", ERROR_OCCURED); 
		  return system_status_flag; 
	 }
     if(lcd_disp_ctrl_ptr->use_start_point_flag == STATE_YES)
	 {		
         lcd_disp_ctrl_ptr->use_start_point_flag = STATE_NO;
	     if((LCD_Disp_Goto_XY(lcd_ch_id, lcd_disp_ctrl_ptr->start_point_num.line_num_or_index, lcd_disp_ctrl_ptr->start_point_num.col_num_or_index)) != SUCCESS)
		 {
			 system_status_flag = ERR_LCD_GOTO_XY_PROC;
              Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.06", ERROR_OCCURED);		 
		     return system_status_flag;
		 }
	 }
	 cur_lcd_status_ptr->data_till_end_point_flag = lcd_disp_ctrl_ptr->data_till_end_point_flag;
	 switch(output_data_type)
	 {
		case DATA_TYPE_IN_STR:
		   switch(lcd_disp_ctrl_ptr->use_data_status_flag)
		   {
		      case STATE_YES:
		         if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_STR, retrieved_lcd_data_str )) != SUCCESS)
		         {
			    	  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				      Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.07", ERROR_OCCURED);
				      return system_status_flag; 
		         }
				 cur_lcd_disp_str = retrieved_lcd_data_str;
              break;
              case STATE_NO:
                 cur_lcd_disp_str = lcd_disp_ctrl_ptr->lcd_disp_str;
              break;
		   }			  
		   if((LCD_Disp_Str(lcd_ch_id, cur_lcd_disp_str)) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_WRITE_DATA_PROC;
				 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.08", ERROR_OCCURED);
		        return system_status_flag;
		   }
		break;
		case DATA_TYPE_IN_CHAR:
		   switch(lcd_disp_ctrl_ptr->use_data_status_flag)
		   {
		      case STATE_YES:
		         if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_CHAR, retrieved_lcd_data_str )) != SUCCESS)
		         {
			    	  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				      Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.09", ERROR_OCCURED);
				      return system_status_flag; 
		         }
				 cur_lcd_disp_str = retrieved_lcd_data_str;
              break;
              case STATE_NO:
                 cur_lcd_disp_str = lcd_disp_ctrl_ptr->lcd_disp_str;
              break;
		   }	
		   if((LCD_Disp_Char(lcd_ch_id, *cur_lcd_disp_str)) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_WRITE_DATA_PROC;
				 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.10", ERROR_OCCURED);
		        return system_status_flag;
		   }			   
		break;
		case DATA_TYPE_IN_BINARY:
		case DATA_TYPE_IN_HEXA:
		case DATA_TYPE_IN_DECIMAL:
		   if(lcd_disp_ctrl_ptr->disp_num_fmt.num_digits <= 0 || lcd_disp_ctrl_ptr->disp_num_fmt.num_digits > MAX_LCD_DIGITS )
		   {
			  system_status_flag = ERR_LCD_DISP_FORMAT;
			   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.11", ERROR_OCCURED);
			  return system_status_flag;
		   }
		   switch(lcd_disp_ctrl_ptr->use_data_status_flag)
		   {
		      case STATE_YES:
		         if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_INT32, &lcd_disp_num_int32 )) != SUCCESS)
		         {
			    	  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				       Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.12", ERROR_OCCURED);
				      return system_status_flag; 
		         }				
              break;
              case STATE_NO:
                 lcd_disp_num_int32 = lcd_disp_ctrl_ptr->lcd_disp_num;
              break;
		   }	
		   if((LCD_Disp_Num(lcd_ch_id, output_data_type, lcd_disp_ctrl_ptr->disp_num_fmt, lcd_disp_num_int32)) != SUCCESS)
		   {
			  system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.13", ERROR_OCCURED);
			  return system_status_flag;
		   }        			
         break;
		 default:
		   system_status_flag = ERR_DATA_TYPE_INVALID;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.15.14", ERROR_OCCURED);
		   return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disp_Num

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : only display least numeric digits and from left to right display is implemented. 

Func ID        : 09.16

BUGS           :   
-*------------------------------------------------------------*/
uint16_t LCD_Disp_Num(const uint8_t lcd_ch_id, const uint8_t num_data_type, const disp_num_fmt_t disp_num_fmt, const int32_t lcd_num)
{
	 uint32_t base_power_val, lcd_disp_num;
	 int16_t place_val;
     uint8_t base, place_digit[MAX_LCD_DIGITS];     
	 char hex_data[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 
	 
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		   system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.01", ERROR_OCCURED);
		   return system_status_flag;
	}
	if(disp_num_fmt.num_digits <= 0 || disp_num_fmt.num_digits > MAX_LCD_DIGITS )
	{
		system_status_flag = ERR_LCD_DISP_FORMAT;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.02", ERROR_OCCURED);
		return system_status_flag;
	}
     switch(num_data_type)
	 {
		case DATA_TYPE_IN_BINARY:
           base = BASE_02;
	       if(lcd_num < 0)
		   {
				 if(disp_num_fmt.sign_flag == STATE_YES)
		         {
			    	 if((LCD_Disp_Char(lcd_ch_id, '-')) != SUCCESS)
					 {
						  system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			              Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.03", ERROR_OCCURED);
			              return system_status_flag;
					 }
				 }			    
			}
			else
			{
			   if(disp_num_fmt.sign_flag == STATE_YES)
		       {
			    	if((LCD_Disp_Char(lcd_ch_id, '+')) != SUCCESS)
					{
						 system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			             Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.04", ERROR_OCCURED);
			             return system_status_flag;
					}
			   }
		    }		
           lcd_disp_num = lcd_num;   		   
		   if((LCD_Disp_Str(lcd_ch_id, "0b")) != SUCCESS)
		   {
			   system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.05", ERROR_OCCURED);
			   return system_status_flag;
		   }
       break;			
	   case DATA_TYPE_IN_HEXA:
		   base = BASE_16;
	       if(lcd_num < 0)
			 {
				 if((LCD_Disp_Char(lcd_ch_id, '-')) != SUCCESS)
				 {
					 system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			         Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.06", ERROR_OCCURED);
			         return system_status_flag;
				 }
				 lcd_disp_num = -lcd_num; 
			 }
			 else
			 {
				 if(disp_num_fmt.sign_flag == STATE_YES)
		         {
			    	if((LCD_Disp_Char(lcd_ch_id, '+')) != SUCCESS)
					{
						 system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			              Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.07", ERROR_OCCURED);
			             return system_status_flag;
					}					
				 }
				 lcd_disp_num = lcd_num; 
			 }           			
			if((LCD_Disp_Str(lcd_ch_id, "0x")) != SUCCESS)
			{
			    system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.08", ERROR_OCCURED);
			    return system_status_flag;
			}
        break;			
		case DATA_TYPE_IN_DECIMAL:
		    base = BASE_10;
		    if(lcd_num < 0)
			 {
				 if(disp_num_fmt.sign_flag == STATE_YES)
		         {
			    	if((LCD_Disp_Char(lcd_ch_id, '-')) != SUCCESS)
				    {
					    system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			             Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.09", ERROR_OCCURED);
			            return system_status_flag;
					}
				 }
			     lcd_disp_num = -lcd_num; 
			 }
			 else
			 {
				 if(disp_num_fmt.sign_flag == STATE_YES)
		         {
			    	if((LCD_Disp_Char(lcd_ch_id, '+')) != SUCCESS)
					{
						 system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			              Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.10", ERROR_OCCURED);
			             return system_status_flag;
					}	
				 }
			  	 lcd_disp_num = lcd_num;  
			 }	
		break;
		default:
		  system_status_flag = ERR_LCD_DISP_FORMAT;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.11", ERROR_OCCURED);
		  return system_status_flag;
	}
	if(disp_num_fmt.least_digits_flag == STATE_YES && disp_num_fmt.left_format_flag == STATE_YES)
	{			
	       base_power_val =  Power_Of(base, disp_num_fmt.num_digits);
		   for(place_val = disp_num_fmt.num_digits - 1; place_val >= 0; --place_val)
		   {
		      lcd_disp_num %= base_power_val;
		      base_power_val /= base; 
		      place_digit[place_val] = lcd_disp_num /base_power_val;
		      if((LCD_Disp_Char(lcd_ch_id, hex_data[place_digit[place_val]])) != SUCCESS)
			  {
				 system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			      Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.12", ERROR_OCCURED);
			     return system_status_flag; 
			  }				  
		  } 
	}
    else
	{
		//only display least numeric digits and from left to right display is implemented
		system_status_flag = ERR_LCD_DISP_FORMAT;
		 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.16.13", ERROR_OCCURED);
		return system_status_flag;
	}		
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Disable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.17  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LCD_Disable(const uint8_t lcd_ch_id)
{
	 lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	
	 if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.17.01", ERROR_OCCURED);
		  return system_status_flag;
	 }
	 cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id ;  
     cur_lcd_ctrl_ptr->enable_flag = STATE_NO;	 
   return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Enable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.18  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LCD_Enable(const uint8_t lcd_ch_id)
{
      lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	
	 if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.18.01", ERROR_OCCURED);
		  return system_status_flag;
	 }
	 cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id ;  
     cur_lcd_ctrl_ptr->enable_flag = STATE_YES;
   return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Disable_All_LCDs

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.19  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Disable_All_LCDs(void)
{
	 uint8_t lcd_ch_id =0;
	
	 for(lcd_ch_id = 0; lcd_ch_id < NUM_OUTPUT_DEV_ID_LCD_CHS; ++lcd_ch_id)
	 {
		   LCD_Disable(lcd_ch_id);
	 }
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Allow_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.20

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LCD_Allow_Access(const uint8_t lcd_ch_id)
{
	 lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	
	 if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.20.01", ERROR_OCCURED);
		  return system_status_flag;
	 }
	 cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id ;   
     cur_lcd_ctrl_ptr->access_flag = STATE_YES;
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_No_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.21 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LCD_No_Access(const uint8_t lcd_ch_id)
{
	 lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	
	 if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.21.01", ERROR_OCCURED); 
		  return system_status_flag;
	 }
	 cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id ;   
     cur_lcd_ctrl_ptr->access_flag = STATE_NO;
     LCD_Disable(lcd_ch_id);
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_DeInit

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.22 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LCD_DeInit(const uint8_t lcd_ch_id, const uint8_t deinit_mode)
{
	 io_config_t lcd_unconfig;
	 lcd_ctrl_t *cur_lcd_ctrl_ptr;
	 uint8_t lcd_io_ch_ctrl_index, lcd_io_ch_ctrl_start_index;	
	 
	 if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.22.01", ERROR_OCCURED); 
		  return system_status_flag;
	 }
	 LCD_No_Access(lcd_ch_id);
	 cur_lcd_ctrl_ptr = lcd_ctrl + lcd_ch_id ; 
	 lcd_unconfig.port_pin_len = 1;
	 lcd_unconfig.func = IO_FUNC_GPIO;
	 lcd_unconfig.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	 for(lcd_io_ch_ctrl_index = 0; lcd_io_ch_ctrl_index < 3; ++lcd_io_ch_ctrl_index)
	 {
		lcd_unconfig.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index]; 
		if((IO_Ch_Func_Reset(&lcd_unconfig)) != SUCCESS)
	    {
	    	system_status_flag = ERR_IO_CH_FUNC_RESET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.22.02", ERROR_OCCURED); 
            return system_status_flag;		
	    }	  
	 }
	 if(cur_lcd_ctrl_ptr->interface == DATA_INTERFACE_4_BITS)
	 {
	     lcd_io_ch_ctrl_start_index = 3 + 4; 
	 }
	 else
	 {
		lcd_io_ch_ctrl_start_index = 3; 
	 }
	 for(lcd_io_ch_ctrl_index = lcd_io_ch_ctrl_start_index; lcd_io_ch_ctrl_index < 11; ++lcd_io_ch_ctrl_index)
	 {
		lcd_unconfig.io_ch = lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index]; 
		if((IO_Ch_Func_Reset(&lcd_unconfig)) != SUCCESS)
	    {
	    	system_status_flag = ERR_IO_CH_FUNC_RESET;
		    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.22.02", ERROR_OCCURED); 
            return system_status_flag;		
	    }	  
	 }
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_LCD_Cur_Data_Status_Para

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.23 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reset_LCD_Cur_Data_Status_Para(uint8_t lcd_ch_id)
{
   lcd_status_t *cur_lcd_status_ptr = NULL_DATA_PTR;	 
   
    if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.23.01", ERROR_OCCURED); 
		  return system_status_flag;
	 }
  cur_lcd_status_ptr = lcd_status +  lcd_ch_id;
  cur_lcd_status_ptr->cur_data_lcd_para.start_loc = LCD_LOC_INVALID;
  cur_lcd_status_ptr->cur_data_lcd_para.end_loc = LCD_LOC_INVALID;
  cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index = LCD_LINE_NUM_OR_INDEX_INVALID;
  cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index = LCD_COL_NUM_OR_INDEX_INVALID;
  cur_lcd_status_ptr->cur_data_lcd_para.max_num_chars_alloc = 0;
  cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.line_num_or_index = LCD_LINE_NUM_OR_INDEX_INVALID;
  cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.col_num_or_index = LCD_COL_NUM_OR_INDEX_INVALID;   
  return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Cur_Data_By_NumChars_Calc_Para

DESCRIPTION    : calc cur input data parameters by using given max num of chars from start cur data input line and start cur data input col.  
  If conf max num of chars for cur input's data > max avail num of chars, then allocate cur data input max num of chars = max avail num of chars,
  whereas max avail num of chars = num of chars from cur input's data start line and cur input's data start column num till end of configure lines and cols,
  else  then allocate num of chars for input data =  given conf max num of chars for cur input's data 
								
INPUT          : 

OUTPUT         : 

NOTE           : FIND MAX loc at which data can be entered from data_start_loc and max num of chars can be entered from data_start_loc,
	             max loc's line num and max loc's column num

Func ID        : 09.24 

Bugs           :  
-*------------------------------------------------------------*/	
uint16_t LCD_Cur_Data_By_NumChars_Calc_Para(const uint8_t lcd_ch_id, const lcd_point_config_t *const lcd_point_config_ptr) 
{
	 lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	 lcd_status_t *cur_lcd_status_ptr = NULL_DATA_PTR;
	 uint8_t max_config_num_chars_left_from_next_line,  max_avail_num_chars_start_line, max_avail_num_chars_from_next_line,
     	   max_avail_data_numchars ; 	
	
	if(lcd_point_config_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.24.01", ERROR_OCCURED); 
		return system_status_flag;
	}
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.24.02", ERROR_OCCURED); 
		  return system_status_flag;
	}
    Reset_LCD_Cur_Data_Status_Para(lcd_ch_id);
	cur_lcd_ctrl_ptr = lcd_ctrl +  lcd_ch_id;
	cur_lcd_status_ptr = lcd_status +  lcd_ch_id;	
   if(lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index > (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1) || lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index > (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1)) 
   { 	  
        /* error invalid lcd loc not within limit */ 
        system_status_flag = ERR_LCD_LOC_INVALID;		
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.24.03", ERROR_OCCURED); 
        return system_status_flag;		
   }		
        max_avail_num_chars_start_line = (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1) + 1 - lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index;		
		max_avail_num_chars_from_next_line = ((cur_lcd_ctrl_ptr->max_config_lines_in_index + 1) - lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index) * (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
		
		if(lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index == (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1))
		{
			max_avail_data_numchars = max_avail_num_chars_start_line;
		}	
		else
		{	
		    max_avail_data_numchars = max_avail_num_chars_start_line + max_avail_num_chars_from_next_line; 
		}	
		if(max_avail_data_numchars < lcd_point_config_ptr->max_num_chars_config )
		{
			cur_lcd_status_ptr->cur_data_lcd_para.max_num_chars_alloc = max_avail_data_numchars; 
			/*error: exceeds max_avail_data_numchars < lcd_point_config_ptr->max_num_chars_config */				
			 cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index = (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1);
			 cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index = (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
			 system_status_flag = WARN_LCD_ALLOC_LESS_THAN_CONFIG_CHARS;
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.24.04", WARNING_OCCURED); 			 
		}
        else
        {
			cur_lcd_status_ptr->cur_data_lcd_para.max_num_chars_alloc = lcd_point_config_ptr->max_num_chars_config;
			if(max_avail_num_chars_start_line >= lcd_point_config_ptr->max_num_chars_config)
			{
				 cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index = lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index;
				 cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index  = lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index + lcd_point_config_ptr->max_num_chars_config - 1;
            }
            else
			{				
		    	max_config_num_chars_left_from_next_line = lcd_point_config_ptr->max_num_chars_config - max_avail_num_chars_start_line;
			    cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index = (max_config_num_chars_left_from_next_line / (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1)) + 1 + lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index;
			    cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index = max_config_num_chars_left_from_next_line % ((cur_lcd_ctrl_ptr->max_config_cols_in_index + 1) + 1);
			}
        }
        if((LCD_From_XY_To_Loc(lcd_ch_id, &(cur_lcd_status_ptr->cur_data_lcd_para.end_point_num), &cur_lcd_status_ptr->cur_data_lcd_para.end_loc)) != SUCCESS)
		{
		     system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.24.05", ERROR_OCCURED); 
			 return system_status_flag;
		}
		cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.col_num_or_index = lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index;
		cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.line_num_or_index = lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index;
		if((LCD_From_XY_To_Loc(lcd_ch_id, &(cur_lcd_status_ptr->cur_data_lcd_para.start_point_num), &cur_lcd_status_ptr->cur_data_lcd_para.start_loc)) != SUCCESS)
		{
		     system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
			 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.24.06", ERROR_OCCURED); 
			 return system_status_flag;
		}	
        return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Cur_Data_By_XY_Calc_Para

DESCRIPTION    : calc cur input data parameters by using given cur data input start line, 
   cur data input start column,  next data start line, and next data start column. 
   XY(Cur input data's end line, Cur input data's end column) = previous pos of XY( next data start line, and next data start column).
   Max configured cur input's data max input chars =  num of chars from   XY(Cur input data's start line, Cur input data's start column) 
   to XY(Cur input data's end line, cur input data's end column).
   Max avail chars = num of chars from cur data input start line and cur data input start column, till end of configure max lines and configure max cols. 
   If max configured cur input's data max input chars > max avail chars, then   allocate max num of chars for cur input data = max avail chars,
   else, allocate max num of chars for input data = max configured cur input's data max input chars.
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.25 

Bugs           :  
-*------------------------------------------------------------*/		
uint16_t LCD_Cur_Data_By_XY_Calc_Para(const uint8_t lcd_ch_id, const lcd_point_config_t *const lcd_point_config_ptr) 
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR;
	lcd_status_t *cur_lcd_status_ptr = NULL_DATA_PTR;
	uint8_t  max_avail_num_chars_start_line,  num_chars_inbetween_start_end_lines, data_end_line_num,  data_end_col_num;	
	
	if(lcd_point_config_ptr == NULL_DATA_PTR )
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.01", ERROR_OCCURED); 
		return system_status_flag;
	}
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.02", ERROR_OCCURED); 
		  return system_status_flag;
	}
	 Reset_LCD_Cur_Data_Status_Para(lcd_ch_id);
	 cur_lcd_ctrl_ptr = lcd_ctrl +  lcd_ch_id;
	 cur_lcd_status_ptr = lcd_status +  lcd_ch_id;
   if(!( (((lcd_point_config_ptr->next_data_start_point_num.line_num_or_index <= (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1)) && (lcd_point_config_ptr->next_data_start_point_num.col_num_or_index <= (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1)) )  ||  
        ((lcd_point_config_ptr->next_data_start_point_num.line_num_or_index <=  (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1 + 1)) && lcd_point_config_ptr->next_data_start_point_num.col_num_or_index == COL_NUM_01 )) && 
       (lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index <= (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1)) && (lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index <= (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1) )))
   {
	   /* error invalid lcd loc not within limit */ 	 
        system_status_flag = ERR_LCD_LOC_INVALID;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.03", ERROR_OCCURED); 
        return system_status_flag;	
   }
      if(lcd_point_config_ptr->next_data_start_point_num.col_num_or_index == COL_NUM_01)
	  {
		  data_end_col_num = (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
		  data_end_line_num = lcd_point_config_ptr->next_data_start_point_num.line_num_or_index - 1;
	  }
      else
      {
		  data_end_col_num = lcd_point_config_ptr->next_data_start_point_num.col_num_or_index - 1;
		  data_end_line_num = lcd_point_config_ptr->next_data_start_point_num.line_num_or_index;
	  }	
	  
	  if(!( (lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index == data_end_line_num && lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index < data_end_col_num )|| lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index < data_end_line_num  ))
	  {
           /* error invalid start data input loc > end data input loc */ 
        	 system_status_flag = ERR_LCD_LOC_INVALID;
		     Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.04", ERROR_OCCURED); 
             return system_status_flag;	 
      }
         cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.col_num_or_index = lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index;
	     cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.line_num_or_index = lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index;
	     if((LCD_From_XY_To_Loc(lcd_ch_id, &(cur_lcd_status_ptr->cur_data_lcd_para.start_point_num), &cur_lcd_status_ptr->cur_data_lcd_para.start_loc)) != SUCCESS)
		 {
			  system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
			  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.05", ERROR_OCCURED); 
			  return system_status_flag;
		 }
         if(data_end_line_num > (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1))
         {
			cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index =  (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1);
			/* warning : end line data input exceed configured max line  */
			  system_status_flag  =  WARN_LCD_ALLOC_LESS_THAN_CONFIG_CHARS;	
			  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.06", WARNING_OCCURED); 			  
	     }
         else
		 {			 
			cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index =  data_end_line_num; 
		 }		
		 if(data_end_col_num > (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1))
         {
			cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index =  (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
			/* warning : end col data input exceed configured max col  */
		    system_status_flag =  WARN_LCD_ALLOC_LESS_THAN_CONFIG_CHARS;		
			Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.07", WARNING_OCCURED);
	     }
        else
		{			
			cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index =  data_end_col_num;
		}			
		if(lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index < data_end_line_num)
		{
			num_chars_inbetween_start_end_lines = (cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index - lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index - 1) * (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
			max_avail_num_chars_start_line =  (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1) + 1 - lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index;	
			cur_lcd_status_ptr->cur_data_lcd_para.max_num_chars_alloc = num_chars_inbetween_start_end_lines + max_avail_num_chars_start_line + cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index; 
		}
        else
        {	          
			cur_lcd_status_ptr->cur_data_lcd_para.max_num_chars_alloc =  data_end_col_num - lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index + 1;
        }			
		if((LCD_From_XY_To_Loc(lcd_ch_id, &(cur_lcd_status_ptr->cur_data_lcd_para.end_point_num), &cur_lcd_status_ptr->cur_data_lcd_para.end_loc)) != SUCCESS)
		{
			system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
			Error_or_Warning_LCD_Proc(lcd_ch_id, "09.25.08", ERROR_OCCURED);
			return system_status_flag;
		}
		return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Cur_Data_By_NumChars_and_XY_Calc_Para

DESCRIPTION    : calc cur input data parameters by using given cur data input start line, 
  cur data input start column, next data start line, next data start column, and conf num of chars for cur input data. 
  XY(Cur input data's end line, Cur input data's end column) = previous pos of XY( next data start line, and next data start column).
  Max avail num of chars = num of chars from cur data input start line and cur data input start column, till XY(Cur input data's end line, cur input data's end column).
  If conf max num of chars for cur input data > max avail num of chars, then allocate num of chars for cur input data = max avail num of chars, 
	else max allocated data = given configure num of chars for cur input data.
								
INPUT          : 

OUTPUT         : 

NOTE           : (NUM_CHARS_DATA = GIVEN_CHARS_MAX_XY)

Func ID        : 09.26 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LCD_Cur_Data_By_NumChars_and_XY_Calc_Para(const uint8_t lcd_ch_id, const lcd_point_config_t *const lcd_point_config_ptr)
{
	lcd_ctrl_t *cur_lcd_ctrl_ptr = NULL_DATA_PTR; 
	lcd_status_t *cur_lcd_status_ptr = NULL_DATA_PTR;
    uint8_t max_avail_num_chars_start_line, config_max_num_chars_from_next_line, 
    num_chars_inbetween_start_end_lines, data_end_line_num,  data_end_col_num, cur_data_max_avail_numchars ;
	
	if(lcd_point_config_ptr == NULL_DATA_PTR )
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.01", ERROR_OCCURED);
		return system_status_flag;
	}
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.02", ERROR_OCCURED);
		  return system_status_flag;
	}
	Reset_LCD_Cur_Data_Status_Para(lcd_ch_id);
	cur_lcd_ctrl_ptr = lcd_ctrl +  lcd_ch_id;
	cur_lcd_status_ptr = lcd_status +  lcd_ch_id;
    if(!(( ((lcd_point_config_ptr->next_data_start_point_num.line_num_or_index <= (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1)) && (lcd_point_config_ptr->next_data_start_point_num.col_num_or_index <= (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1)) )  ||  \
        ((lcd_point_config_ptr->next_data_start_point_num.line_num_or_index <= (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1 + 1) && lcd_point_config_ptr->next_data_start_point_num.col_num_or_index == COL_NUM_01 ))) && \
      ( lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index <= (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1) && lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index <= (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1))) )	 
    {
		/* error invalid lcd loc not within limit */ 
        system_status_flag = ERR_LCD_LOC_INVALID;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.03", ERROR_OCCURED);
        return system_status_flag;	
	}
		
		if(lcd_point_config_ptr->next_data_start_point_num.col_num_or_index == COL_NUM_01)
	     {
		    data_end_col_num = (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
		    data_end_line_num = lcd_point_config_ptr->next_data_start_point_num.line_num_or_index - 1;
	     }
         else
         {
		    data_end_col_num = lcd_point_config_ptr->next_data_start_point_num.col_num_or_index - 1;
		    data_end_line_num = lcd_point_config_ptr->next_data_start_point_num.line_num_or_index;
	     }
	     if(!( (lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index == data_end_line_num && 
		    lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index < data_end_col_num )|| 
			lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index < data_end_line_num  ))
		 {
			 /* error invalid start data input loc > end data input loc */
             system_status_flag = ERR_LCD_LOC_INVALID;
		     Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.04", ERROR_OCCURED);
             return system_status_flag;	
		 }
			if(data_end_line_num > (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1))
            {
		    	cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index =  (cur_lcd_ctrl_ptr->max_config_lines_in_index + 1);
		    	/* warning : end line data input exceed configured max line  */
				system_status_flag = WARN_LCD_ALLOC_LESS_THAN_CONFIG_CHARS;
			    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.05", WARNING_OCCURED);                 		 
	        }
            else
			{				
		    	cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index =  data_end_line_num; 
			}
		    if(data_end_col_num > (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1))
            {
		    	cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index =  (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
		    	/* warning : end col data input exceed configured max col  */		       
			     system_status_flag =  WARN_LCD_ALLOC_LESS_THAN_CONFIG_CHARS;
				 Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.06", WARNING_OCCURED);				  
	        }
            else
			{				
		    	cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index =  data_end_col_num; 
			}
		    cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.col_num_or_index = lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index;
		    cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.line_num_or_index = lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index;
		    if((LCD_From_XY_To_Loc(lcd_ch_id, &(cur_lcd_status_ptr->cur_data_lcd_para.start_point_num), &cur_lcd_status_ptr->cur_data_lcd_para.start_loc)) != SUCCESS)
			{
				system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
		    	Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.07", ERROR_OCCURED);
		    	return system_status_flag;
			}				
			
			if(lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index < data_end_line_num)
		    {
		    	num_chars_inbetween_start_end_lines = (cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index - lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index - 1) * (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1);
		    	max_avail_num_chars_start_line =  (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1) + 1 - lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index;	
		    	cur_data_max_avail_numchars = num_chars_inbetween_start_end_lines + max_avail_num_chars_start_line + cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index;                					
		    }
            else
            {	          
			    cur_data_max_avail_numchars =  data_end_col_num - lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index + 1;
            }
            if(lcd_point_config_ptr->max_num_chars_config > cur_data_max_avail_numchars)
			{
			   /* warning : end col data input exceed configured max col  */
		        cur_lcd_status_ptr->cur_data_lcd_para.max_num_chars_alloc = cur_data_max_avail_numchars;
				cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index =  data_end_line_num;
				cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index =  data_end_col_num;
				system_status_flag =  WARN_LCD_ALLOC_LESS_THAN_CONFIG_CHARS;
				Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.08", WARNING_OCCURED);
			}
            else
			{
				cur_lcd_status_ptr->cur_data_lcd_para.max_num_chars_alloc = lcd_point_config_ptr->max_num_chars_config;
			    if(lcd_point_config_ptr->max_num_chars_config < max_avail_num_chars_start_line)	
				{
					cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index =  lcd_point_config_ptr->cur_data_start_point_num.line_num_or_index;
				    cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index =  lcd_point_config_ptr->cur_data_start_point_num.col_num_or_index + lcd_point_config_ptr->max_num_chars_config - 1;					
				}
                else
				{
					config_max_num_chars_from_next_line = lcd_point_config_ptr->max_num_chars_config - max_avail_num_chars_start_line;
					cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.line_num_or_index = (config_max_num_chars_from_next_line / (cur_lcd_ctrl_ptr->max_config_cols_in_index + 1)) + 1 + cur_lcd_status_ptr->cur_data_lcd_para.start_point_num.line_num_or_index;
					cur_lcd_status_ptr->cur_data_lcd_para.end_point_num.col_num_or_index = (config_max_num_chars_from_next_line % ((cur_lcd_ctrl_ptr->max_config_cols_in_index + 1) + 1)) ;
				}						
			}
			if((LCD_From_XY_To_Loc(lcd_ch_id, &(cur_lcd_status_ptr->cur_data_lcd_para.end_point_num), &cur_lcd_status_ptr->cur_data_lcd_para.end_loc)) != SUCCESS)
			{
				system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
			    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.09", ERROR_OCCURED);
			    return system_status_flag;
			}
			return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Loc_Ctrl_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           :  keep track of lcd_cur_<input/disp>_loc as baskspace we need to -- it and also disp timeouts with.
 	             LCD automatically increments due to loc_command(0x06)in our case as after eg LCD_Write_Command(0x80)
	             ie set DDRAM  

Func ID        : 09.27 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LCD_Loc_Ctrl_Proc(const uint8_t lcd_ch_id, const uint8_t lcd_ctrl_format, const char lcd_rcvd_char )
{	
    lcd_status_t *cur_lcd_status_ptr;
	lcd_ctrl_t *cur_lcd_ctrl_ptr;
	lcd_point_t cur_loc_point_num, next_loc_point_num;	
    uint8_t *lcd_cur_loc_ptr;
	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.01", ERROR_OCCURED);
		  return system_status_flag;
	}
	cur_lcd_status_ptr = lcd_status +  lcd_ch_id;
	cur_lcd_ctrl_ptr = lcd_ctrl +  lcd_ch_id;
	switch(lcd_ctrl_format >> LCD_CTRL_LOC_TYPE_DATA_POS)
	{
		case LCD_OUTPUT_LOC:
		  lcd_cur_loc_ptr = &(cur_lcd_status_ptr->lcd_cur_disp_loc);		 
		break;
        case LCD_INPUT_LOC:
          lcd_cur_loc_ptr = &(cur_lcd_status_ptr->lcd_cur_input_loc);		  
	    break;		
	}
    switch(lcd_rcvd_char)
	{	
         case BACKSPACE_CHAR:
		    /* backspace at prev point */  
		    if((LCD_From_Loc_to_XY(lcd_ch_id, *lcd_cur_loc_ptr, &cur_loc_point_num)) != SUCCESS)
			{
				system_status_flag = ERR_LCD_FROM_LOC_TO_XY_PROC;
			    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.02", ERROR_OCCURED);
			    return system_status_flag;
			}
	        if((cur_loc_point_num.line_num_or_index == LINE_NUM_01) && (cur_loc_point_num.col_num_or_index == COL_NUM_01))
	        {	 
	    	   /* reached begin of line num 1 and valid backspace is pressed, retain same loc position */ 
	    	   system_status_flag =  WARN_LCD_TRY_BS_CHAR_ATBEGIN;			  
			   Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.03", WARNING_OCCURED);
	    	   return SUCCESS;
	       }
	        /* put cur input lcd loc to next location */
	       if(cur_loc_point_num.col_num_or_index == COL_NUM_01)
	       {	 
			   next_loc_point_num.line_num_or_index = cur_loc_point_num.line_num_or_index - 1;
			   next_loc_point_num.col_num_or_index = cur_lcd_ctrl_ptr->max_config_cols_in_index + 1;
	       }
           else
           {
	          next_loc_point_num.line_num_or_index = cur_loc_point_num.line_num_or_index;
	    	  next_loc_point_num.col_num_or_index = cur_loc_point_num.col_num_or_index - 1;
	       }	
	       if((LCD_From_XY_To_Loc(lcd_ch_id, &next_loc_point_num, lcd_cur_loc_ptr)) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
			     Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.04", ERROR_OCCURED);
			    return system_status_flag;
		   }
		   if((LCD_Write_Command(lcd_ch_id, *lcd_cur_loc_ptr)) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.05", ERROR_OCCURED);
			    return system_status_flag;
		   }
		   if((LCD_Disp_Char(lcd_ch_id, ' ')) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			     Error_or_Warning_LCD_Proc(lcd_ch_id, "09.26.01", ERROR_OCCURED);
			    return system_status_flag;
		   }	
		   //shift cursor to left
           if((LCD_Write_Command(lcd_ch_id, 0x10)) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.06", ERROR_OCCURED);
			    return system_status_flag;
		   }			   
		   if((LCD_Write_Command(lcd_ch_id, *lcd_cur_loc_ptr)) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			     Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.07", ERROR_OCCURED);
			    return system_status_flag;
		   }
		   switch(lcd_ctrl_format >> LCD_CTRL_LOC_TYPE_DATA_POS)
	       {
		       case LCD_OUTPUT_LOC:
		         if((LCD_Write_Command(lcd_ch_id, 0x0C)) != SUCCESS)
				 {
					 system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			         Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.08", ERROR_OCCURED);
			         return system_status_flag;
				 }
	           break;
               case LCD_INPUT_LOC:                
		         if((LCD_Write_Command(lcd_ch_id, 0x0E)) != SUCCESS)
				 {
					 system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			         Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.09", ERROR_OCCURED);
			         return system_status_flag;
				 }
	           break;		
           }           
		 break;		 
		 default:
		    /* disp char at next point */ 
		    if((LCD_Write_Command(lcd_ch_id, *lcd_cur_loc_ptr)) != SUCCESS)
			{
				system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			     Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.10", ERROR_OCCURED);
			    return system_status_flag;
			}
	        switch(lcd_ctrl_format & 0x03)
            {
               case RCVD_CHAR_PLAIN_DISP_FORMAT:
                  if((LCD_Disp_Char(lcd_ch_id, lcd_rcvd_char)) != SUCCESS)
				  {
					  system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			           Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.11", ERROR_OCCURED);
			          return system_status_flag;
				  }
               break;
               case RCVD_CHAR_HIDDEN_DISP_FORMAT:
                  if((LCD_Disp_Char(lcd_ch_id, lcd_rcvd_char)) != SUCCESS)
				  {
					  system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			           Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.12", ERROR_OCCURED);
			          return system_status_flag;
				  }
                  Delay_MS(REQ_TIME_DELAY_LCD_HIDDEN_DISP_IN_MS);
                  if((LCD_Write_Command(lcd_ch_id, *lcd_cur_loc_ptr)) != SUCCESS)
				  {
					  system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			           Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.13", ERROR_OCCURED);
			          return system_status_flag;
				  }					  
                  if((LCD_Disp_Char(lcd_ch_id, HIDDEN_KEY_DISP_CHAR)) != SUCCESS)
				  {
					  system_status_flag = ERR_LCD_WRITE_DATA_PROC;
			          Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.14", ERROR_OCCURED);
			          return system_status_flag;
				  }
               break;
	           case RCVD_CHAR_DONT_DISP_FORMAT:
	             return SUCCESS;
	           // break;
	           default:
                  /* error: invalid lcd enter char disp format */
                 system_status_flag = ERR_FORMAT_INVALID;
				  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.15", ERROR_OCCURED);
                return system_status_flag;		 
            }  
	        if((LCD_From_Loc_to_XY(lcd_ch_id, *lcd_cur_loc_ptr, &cur_loc_point_num)) != SUCCESS)
			{
				system_status_flag = ERR_LCD_FROM_LOC_TO_XY_PROC;
			    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.16", ERROR_OCCURED);
			    return system_status_flag;
			}
	        if((cur_loc_point_num.line_num_or_index == cur_lcd_ctrl_ptr->max_config_lines_in_index + 1) && (cur_loc_point_num.col_num_or_index  == cur_lcd_ctrl_ptr->max_config_cols_in_index + 1))
	        {	 
	    	   /* reached end of max configured line and data to disp, retain same loc position */ 
			    system_status_flag =  WARN_LCD_TRY_CHAR_ATEND_CONF_LINE;			  
			   	Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.17", WARNING_OCCURED);
	    	    return SUCCESS;	    	   
	       }
	        /* put cur input lcd loc to next location */
	       if(cur_loc_point_num.col_num_or_index == cur_lcd_ctrl_ptr->max_config_cols_in_index + 1)
	       {	 
			   next_loc_point_num.line_num_or_index = cur_loc_point_num.line_num_or_index  + 1;
			   next_loc_point_num.col_num_or_index = COL_NUM_01;
	       }
           else
           {
	          next_loc_point_num.line_num_or_index = cur_loc_point_num.line_num_or_index;
	    	  next_loc_point_num.col_num_or_index = cur_loc_point_num.col_num_or_index + 1;
	       }	
	       if((LCD_From_XY_To_Loc(lcd_ch_id, &next_loc_point_num, lcd_cur_loc_ptr)) != SUCCESS)
		   {
			    system_status_flag = ERR_LCD_FROM_XY_TO_LOC_PROC;
			   	Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.18", ERROR_OCCURED);
			    return system_status_flag;
		   }
		   if((LCD_Write_Command(lcd_ch_id, *lcd_cur_loc_ptr)) != SUCCESS)
		   {
			  system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.19", ERROR_OCCURED);
			  return system_status_flag; 
		   }
		   switch(lcd_ctrl_format >> LCD_CTRL_LOC_TYPE_DATA_POS)
	       {
		       case LCD_OUTPUT_LOC:
		         if((LCD_Write_Command(lcd_ch_id, 0x0C)) != SUCCESS)
				 {
					 system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			         Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.20",ERROR_OCCURED );
			         return system_status_flag; 
				 }
	           break;
               case LCD_INPUT_LOC:                
		         if((LCD_Write_Command(lcd_ch_id, 0x0E)) != SUCCESS)
				 {
					 system_status_flag = ERR_LCD_WRITE_CMD_PROC;
			         Error_or_Warning_LCD_Proc(lcd_ch_id, "09.27.21", ERROR_OCCURED);
			         return system_status_flag; 
				 }
	           break;		
          }    
	 }
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Error_or_Warning_LCD_Proc

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.28 

Bugs           :  
-*------------------------------------------------------------*/
static uint16_t Error_or_Warning_LCD_Proc(const uint8_t lcd_ch_id, const char *const error_trace_str, const uint8_t warn_or_error_format)
{
 	if(lcd_ch_id < NUM_OUTPUT_DEV_ID_LCD_CHS && lcd_ch_id != TRACE_LCD_CH_ID)
	{
	     Error_or_Warning_Proc(error_trace_str, warn_or_error_format, system_status_flag);
	}
	else
	{
		  Error_or_Warning_Occured_Proc(warn_or_error_format, system_status_flag);
		  if((Comm_Error_Proc(error_trace_str, warn_or_error_format, COMM_ERROR_BY_LED_AND_OR_UART, system_status_flag ))!= SUCCESS)
	    {
		     system_status_flag = ERR_PROC_ERROR;		
		     return system_status_flag;
	    }
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_All_LCD_Chs

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.29 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reset_All_LCD_Chs(const uint8_t lcd_reset_type)
{
	uint8_t lcd_ch_id;
	
	if(lcd_reset_type != RESET_WHOLE && lcd_reset_type != RESET_MOD_STATUS)
	{
		system_status_flag = ERR_FORMAT_INVALID;
	    Error_or_Warning_LCD_Proc(TRACE_LCD_CH_ID, "09.29.01", ERROR_OCCURED);
	    return system_status_flag;
	}
	for(lcd_ch_id = 0; lcd_ch_id < NUM_OUTPUT_DEV_ID_LCD_CHS; ++lcd_ch_id)
	{
		Reset_LCD_Ch(lcd_ch_id, lcd_reset_type);
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_LCD_Ch

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.30 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reset_LCD_Ch(const uint8_t lcd_ch_id, const uint8_t lcd_reset_type)
{
	lcd_status_t *cur_lcd_status_ptr;
    uint8_t lcd_line_num;
	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.30.01", ERROR_OCCURED);
		  return system_status_flag;
	}
	if(lcd_reset_type != RESET_WHOLE && lcd_reset_type != RESET_MOD_STATUS)
	{
		system_status_flag = ERR_FORMAT_INVALID;
	    Error_or_Warning_LCD_Proc(lcd_ch_id, "09.30.02", ERROR_OCCURED);
	    return system_status_flag;
	}
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;	
	Reset_LCD_Cur_Data_Status_Para(lcd_ch_id);	
	if(lcd_reset_type == RESET_WHOLE)
	{
	    LCD_Clear_Screen(lcd_ch_id);
	    cur_lcd_status_ptr->lcd_cur_disp_loc = RESET_LCD_DISP_LOC;
	    cur_lcd_status_ptr->lcd_cur_input_loc = RESET_LCD_INPUT_LOC;
	    for(lcd_line_num = 0; lcd_line_num <= MAX_AVAIL_NUM_LINES; ++lcd_line_num)
	    {
	        cur_lcd_status_ptr->cur_line_disp_data[lcd_line_num] = BLANK_LINE_DISP;
	    }
		cur_lcd_status_ptr->data_till_end_point_flag = STATE_NO;
        cur_lcd_status_ptr->lcd_input_loc_flag = STATE_NO;		
	}
	return SUCCESS;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Write_Data_to_IO_Ch 

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.30 

Bugs           :  
-*------------------------------------------------------------*/
static uint16_t LCD_Write_Data_to_IO_Ch(const uint8_t lcd_ch_id, const uint8_t lcd_data_or_cmd, const uint8_t lcd_data_start_pin)
{
	lcd_status_t *cur_lcd_status_ptr;
	uint8_t lcd_io_ch_ctrl_index, lcd_data_pin, lcd_io_ch_bit;  
	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.30.01", ERROR_OCCURED);
		  return system_status_flag;
	}
	if(lcd_data_start_pin > 7)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.30.02", ERROR_OCCURED);
		return system_status_flag;
	}
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;
	cur_lcd_status_ptr->lcd_data_or_cmd = lcd_data_or_cmd;
	#ifdef TRACE_DATA
	   Print("TRA: LCD - write data : 0x%X\r", cur_lcd_status_ptr->lcd_data_or_cmd);
	#endif
	for(lcd_data_pin = 0, lcd_io_ch_ctrl_index = lcd_data_start_pin + 3; lcd_io_ch_ctrl_index < 11; ++lcd_data_pin, ++lcd_io_ch_ctrl_index)
	{
		lcd_io_ch_bit = (cur_lcd_status_ptr->lcd_data_or_cmd  & (1 << lcd_data_pin )) >> lcd_data_pin;
		#ifdef TRACE_DATA
		   Print("TRA: LCD - IO_ch : %u, data_pos : %u, bit = %u\r", lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index], lcd_data_pin, lcd_io_ch_bit);
		#endif
		if((IO_Channel_Write(lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index], lcd_io_ch_bit)) != SUCCESS)
		{
			#ifdef TRACE_ERROR
			    Print("ERR: lcd_data_pin : %u\r", lcd_data_pin);
			#endif
			system_status_flag = ERR_IO_CH_WRITE;
	        Error_or_Warning_LCD_Proc(lcd_ch_id, "09.30.03", ERROR_OCCURED);
	        return system_status_flag;
		}
	}
	
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LCD_Read_Data_from_IO_Ch 

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 09.31 

Bugs           :  
-*------------------------------------------------------------*/
static uint16_t LCD_Read_Data_from_IO_Ch(const uint8_t lcd_ch_id, const uint8_t lcd_data_start_pin)
{
	lcd_status_t *cur_lcd_status_ptr;
	uint8_t lcd_io_ch_ctrl_index, lcd_data_pin, lcd_read_data = 0, io_ch_read_data;  
	
	if(lcd_ch_id >= NUM_OUTPUT_DEV_ID_LCD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_LCD_Proc(lcd_ch_id, "09.31.01", ERROR_OCCURED);
		  return system_status_flag;
	}
	if(lcd_data_start_pin > 7)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_LCD_Proc(lcd_ch_id, "09.31.02", ERROR_OCCURED);
		return system_status_flag;
	}
	lcd_read_data = 0;
	for(lcd_data_pin = 0, lcd_io_ch_ctrl_index = lcd_data_start_pin + 3; lcd_io_ch_ctrl_index < 11; ++lcd_data_pin, ++lcd_io_ch_ctrl_index)
	{
		if((IO_Channel_Read(lcd_io_ch_ctrl[lcd_ch_id][lcd_io_ch_ctrl_index], &io_ch_read_data)) != SUCCESS)
		{
			system_status_flag = ERR_IO_CH_READ;
	        Error_or_Warning_LCD_Proc(lcd_ch_id, "09.31.03", ERROR_OCCURED);
	        return system_status_flag;
		}
		lcd_read_data |= (io_ch_read_data << lcd_data_pin);
	}
	#ifdef TRACE_DATA
	   Print("TRA: LCD - read data : 0x%X\r", lcd_read_data);
	#endif
	cur_lcd_status_ptr = lcd_status + lcd_ch_id;
	cur_lcd_status_ptr->lcd_data_or_cmd = lcd_read_data;
	return SUCCESS;
}

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
