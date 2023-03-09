/**************************************************************************
   FILE          :    keyboard.c
 
   PURPOSE       :     
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
	CAUTION        :
	
  NOTE           :   
  
  CHANGE LOGS    :
  
  FILE ID        : 04  
	   
 **************************************************************************/
  
 #include "main.h" 
 
 #ifdef KEYBOARD_MOD_ENABLE
 
 #include <stdarg.h> 
 #include <stdlib.h>
 
/* ------------------------------ macro defination ------------------------------ */

/* ----------------------------- global variable defination --------------------- */
keyboard_status_t keyboard_status[NUM_INPUT_DEV_ID_KEYBOARD_CHS];
static uint32_t cur_pressed_key_or_sw;	
file_t stdin_keyboard;

/* ----------------------------- global variable declaration -------------------- */
extern uint8_t last_sw_ch_id;

/* ----------------------------- global function declaration -------------------- */
static uint16_t Keyboard_Col_Scan(const uint8_t keyboard_ch_id, const uint8_t offset_row_select );
static int16_t Stream_Stdin_Oper(const uint8_t keyboard_ch_id);
int16_t VFile_Scan(file_t *const fp, const char *const fmt, va_list *const scan_arg_list_ptr);

/* ----------------------------- function pointer defination -------------------- */

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Read

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : long key press and no key press timeout is not implemented.                 

Func ID        : 04.01  

BUGS           :     
-*------------------------------------------------------------*/
uint16_t Keyboard_Read(const uint8_t keyboard_ch_id, void *const dev_input_ctrl_para_ptr)
{
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr = NULL_DATA_PTR; 
	 dev_read_ctrl_para_t *rcvd_dev_input_ctrl_para_ptr = (dev_read_ctrl_para_t *)dev_input_ctrl_para_ptr;
	 uint16_t ret_status = SUCCESS;
	 uint8_t error_status_flag = NO_ERROR, retrieve_data_str_len, row_select_state, offset_row_select;
	 static uint8_t prev_str_len = 0;  //SHOULD_REMOVE
	 
	   if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS || rcvd_dev_input_ctrl_para_ptr == NULL_DATA_PTR)
		 {
			   system_status_flag = ERR_DEV_CH_ID_OR_NULL_PTR;
			    Error_or_Warning_Proc("04.01.01", ERROR_OCCURED, system_status_flag);
			   return system_status_flag;
		 }		   
       cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	   if(  cur_keyboard_ctrl_ptr->access_flag != STATE_YES) 
	   {  
          system_status_flag = WARN_CUR_DATA_ID_DEV_NO_ACCESS;			 
          return  system_status_flag;
	   }
	   if( cur_keyboard_ctrl_ptr->enable_flag != STATE_YES) 
	   {  
          system_status_flag = WARN_CUR_DATA_ID_DEV_DISABLED;		 
          return system_status_flag;
	   }
	   if(rcvd_dev_input_ctrl_para_ptr->cur_data_read_status.max_try_reached_flag == STATE_YES || rcvd_dev_input_ctrl_para_ptr->cur_data_read_status.read_and_ready_to_process_flag == STATE_YES)
	   {
		   Keyboard_Disable(keyboard_ch_id);
		   system_status_flag = WARN_CUR_DATA_ID_DEV_DISABLED;
		   return system_status_flag;
	   }	    
	   ret_status = Keyboard_BS_SW_Read_Proc(keyboard_ch_id);
	   switch(ret_status)
	   {
		   case SUCCESS:
		      rcvd_dev_input_ctrl_para_ptr->read_data_char = BACKSPACE_CHAR;
			  if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_NUM_CHARS_READ, &retrieve_data_str_len)) != SUCCESS)
			  {
					error_status_flag = ERR_APPL_DATA_RETRIEVE;
					 goto KEYPAD_READ_EXIT;
			   }			  
			   if(retrieve_data_str_len == 1)
			   {
				   ret_status = Keyboard_Enter_SW_Oper(keyboard_ch_id, DEV_DISABLE_OPER);
				   switch(ret_status)
				   {
					   case SUCCESS:
					   case ERR_KEYBOARD_ENTER_SW_OPER_BUT_NOT_CONFIG:		               
		               break;
		               default:
		                error_status_flag = ERR_DEV_OPER_DISABLE_FUNC;		     			 
                        goto KEYPAD_READ_EXIT;
	               }
				   ret_status = Keyboard_BS_SW_Oper(keyboard_ch_id, DEV_DISABLE_OPER);
			       switch(ret_status)
			       {
				      case SUCCESS:
				      case ERR_KEYBOARD_BS_SW_OPER_BUT_NOT_CONFIG:		               
		              break;
		              default:
		                error_status_flag = ERR_DEV_OPER_DISABLE_FUNC;		     			 
                        goto KEYPAD_READ_EXIT;
			       }
			   }			  
			   return DATA_CHAR_READ;
		  // break;
		   case SW_OR_KEY_NOT_PRESSED:
		   case ERR_KEYBOARD_BS_SW_OPER_BUT_NOT_CONFIG:
		   case WARN_CUR_DATA_ID_DEV_DISABLED:
		   case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		   break;
		   default:
		     error_status_flag = ERR_KEYBOARD_BS_SW_PROC;		     			 
             goto KEYPAD_READ_EXIT;
	   }
	   ret_status = Keyboard_Enter_SW_Read_Proc(keyboard_ch_id);
	   switch(ret_status)
	   {
		   case SUCCESS:
		       rcvd_dev_input_ctrl_para_ptr->read_data_char = ENTER_CHAR;
			   ret_status = Keyboard_Enter_SW_Oper(keyboard_ch_id, DEV_DISABLE_OPER);
			   switch(ret_status)
			   {
				   case SUCCESS:
				   case ERR_KEYBOARD_ENTER_SW_OPER_BUT_NOT_CONFIG:		               
		           break;
		           default:
		              error_status_flag = ERR_DEV_OPER_DISABLE_FUNC;		     			 
                      goto KEYPAD_READ_EXIT;
	          }			
			  ret_status = Keyboard_BS_SW_Oper(keyboard_ch_id, DEV_DISABLE_OPER);
			  switch(ret_status)
			  {
				  case SUCCESS:
				  case ERR_KEYBOARD_BS_SW_OPER_BUT_NOT_CONFIG:		               
		          break;
		          default:
		             error_status_flag = ERR_DEV_OPER_DISABLE_FUNC;		     			 
                     goto KEYPAD_READ_EXIT;
			  }
			  return DATA_CHAR_READ;
		  // break;
		   case SW_OR_KEY_NOT_PRESSED:
		   case ERR_KEYBOARD_ENTER_SW_OPER_BUT_NOT_CONFIG:
		   case WARN_CUR_DATA_ID_DEV_DISABLED:
		   case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		   break;
		   default:
		     error_status_flag = ERR_KEYBOARD_BS_SW_PROC;		    		 
             goto KEYPAD_READ_EXIT;
	   }
	   
       row_select_state = cur_keyboard_ctrl_ptr->keypad_pressed_state;	
	   //KEYPAD_ROWA_SELECT for offset_row_select = 0, and so on.
       for(offset_row_select = 0; offset_row_select < KEYPAD_NUM_ROWS; ++offset_row_select )
	   {
		   if((IO_Channel_Write(cur_keyboard_ctrl_ptr->io_ch_rowa + offset_row_select,  !row_select_state)) != SUCCESS)
		   {	
	          error_status_flag = ERR_KEYPAD_ROW_WRITE;	         
              goto KEYPAD_READ_EXIT;
		  }		
	  }
	  //KEYPAD_ROWA_SELECT for offset_row_select = 0, and so on.
	  for(offset_row_select = 0; offset_row_select < KEYPAD_NUM_ROWS ; ++offset_row_select )
	  {
		   if((IO_Channel_Write(cur_keyboard_ctrl_ptr->io_ch_rowa + offset_row_select,  row_select_state)) != SUCCESS)
		   {	
	          error_status_flag = ERR_KEYPAD_ROW_WRITE;	         
              goto KEYPAD_READ_EXIT;
		   }
		   ret_status = Keyboard_Col_Scan(keyboard_ch_id, offset_row_select);
		   if((IO_Channel_Write(cur_keyboard_ctrl_ptr->io_ch_rowa + offset_row_select,  !row_select_state)) != SUCCESS)
		   {						 
			  error_status_flag = ERR_KEYPAD_ROW_WRITE;	
			  goto KEYPAD_READ_EXIT;
		   }
         switch(ret_status)
		 {
		    case DATA_CHAR_READ:
			     rcvd_dev_input_ctrl_para_ptr->read_data_char = cur_pressed_key_or_sw;
				if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_NUM_CHARS_READ, &retrieve_data_str_len)) != SUCCESS)
			   {
					error_status_flag = ERR_APPL_DATA_RETRIEVE;
					goto KEYPAD_READ_EXIT;
			   }
			   if(retrieve_data_str_len == 0)
			   {
				   ret_status = Keyboard_Enter_SW_Oper(keyboard_ch_id, DEV_ENABLE_OPER);
				   switch(ret_status)
				   {
					   case SUCCESS:
					   case ERR_KEYBOARD_ENTER_SW_OPER_BUT_NOT_CONFIG:		               
		               break;
		               default:
		                error_status_flag = ERR_DEV_OPER_ENABLE_FUNC;		     			 
                        goto KEYPAD_READ_EXIT;
	               }					
				   ret_status = Keyboard_BS_SW_Oper(keyboard_ch_id, DEV_ENABLE_OPER); 
				   switch(ret_status)
				   {
					   case SUCCESS:
					   case ERR_KEYBOARD_BS_SW_OPER_BUT_NOT_CONFIG:		               
		               break;
		               default:
		                error_status_flag = ERR_DEV_OPER_ENABLE_FUNC;		     			 
                        goto KEYPAD_READ_EXIT;
	               }	
			   }
			  return DATA_CHAR_READ;
			//break;	
			case SW_OR_KEY_NOT_PRESSED:
            case WARN_CUR_DATA_ID_DEV_DISABLED:
			case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
			break;	
			default:
			  error_status_flag = ERR_KEYBOARD_COL_SCAN_PROC;		    		 
             goto KEYPAD_READ_EXIT;			  
		 } 		   
	 }	 
KEYPAD_READ_EXIT: 
   if(error_status_flag != NO_ERROR)
   {
	     Error_or_Warning_Proc("04.01.04", ERROR_OCCURED, error_status_flag);
		 system_status_flag = ERR_KEYBOARD_READ_OPER;
		 Keyboard_Disable(keyboard_ch_id);
         return system_status_flag;	  	 
   }
   return ret_status;	 
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Col_Scan

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : long key press and no key press timeout is not implemented.
                 halt CPU process by using while(KEY == KEY_PRESSED); ie till key is released is not used
                 
Func ID        : 04.02  

BUGS           :     
-*------------------------------------------------------------*/
static uint16_t Keyboard_Col_Scan(const uint8_t keyboard_ch_id, const uint8_t offset_row_select )
{
	keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	keyboard_status_t *cur_keyboard_status_ptr;
	char keypad_char[]   = {'1','2', '3', '4', '5', '6','7','8','9','*', '0', '#'};
	uint8_t pressed_status = 0, offset_col_select;
	
	if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("04.02.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}		   
    cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id;
	if(cur_keyboard_ctrl_ptr->keypad_keys_enable_flag == STATE_NO)
	{
		  system_status_flag = WARN_CUR_DATA_ID_DEV_DISABLED;
		  Error_or_Warning_Proc("04.02.02", WARNING_OCCURED, system_status_flag);
          return  system_status_flag;
	}
	//KEYPAD_COL1_SELECT for offset_col_select = 0, and so on.
	for(offset_col_select = 0; offset_col_select < KEYPAD_NUM_COLS; ++offset_col_select)
	{
		if((SW_Press_Proc(cur_keyboard_status_ptr->base_sw_ch_id + offset_col_select, NULL_DATA_PTR)) == SUCCESS)
	    {
	    	pressed_status = 0x01;
			cur_pressed_key_or_sw = keypad_char[(offset_row_select * KEYPAD_NUM_COLS) + offset_col_select]; //latest pressed key/switch			 
	    	break;		 
	   }
	}
	if(pressed_status == 0x01)
    {
		if((Entered_Key_No_Long_Press_Proc(cur_keyboard_ctrl_ptr)) != SUCCESS)
	    {
	        return ERR_KEYBOARD_NO_LONG_KEY_PRESS_PROC;
	    }
        return DATA_CHAR_READ;
   }
	return SW_OR_KEY_NOT_PRESSED;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Entered_Key_No_Long_Press_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : make sure that entered key is within max available lcd loc and line, currently cur_data_conf_parameter.cur_data_allocated_max_num_chars_to_rcv 
	 sures that entered key is within max available lcd loc and within max line 

Func ID        : 04.03

BUGS           :   
-*------------------------------------------------------------*/
uint16_t Entered_Key_No_Long_Press_Proc(keyboard_ctrl_t *const cur_keyboard_ctrl_ptr)
{ 
    return SUCCESS; 
} 

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Disable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.04  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Disable(const uint8_t keyboard_ch_id)
{
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	 keyboard_status_t *cur_keyboard_status_ptr;
	 uint8_t offset_sw_ch_id;	
	 
	 if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_Proc("04.04.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	 cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id;
   cur_keyboard_ctrl_ptr->enable_flag = STATE_NO;
	 cur_keyboard_ctrl_ptr->keypad_keys_enable_flag = STATE_NO;	
	 for(offset_sw_ch_id = 0; offset_sw_ch_id < cur_keyboard_status_ptr->keyboard_num_sw_chs; ++offset_sw_ch_id)
	 {
		if((SW_Disable(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id)) != SUCCESS)
		{
			system_status_flag = ERR_DEV_OPER_DISABLE_FUNC;
		    Error_or_Warning_Proc("04.04.02", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;
		}
	 }
     return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Enable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.05  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Enable(const uint8_t keyboard_ch_id)
{
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr;	 
	 	  
	if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.05.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	}
	cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
    cur_keyboard_ctrl_ptr->enable_flag = STATE_YES;
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Disable_All_Keyboards

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.06  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Disable_All_Keyboards(void)
{
	 uint8_t keyboard_ch_id =0;
	
	 for(keyboard_ch_id = 0; keyboard_ch_id < NUM_INPUT_DEV_ID_KEYBOARD_CHS; ++keyboard_ch_id)
	 {
		   Keyboard_Disable(keyboard_ch_id);
	 }
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Allow_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.07  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Allow_Access(const uint8_t keyboard_ch_id)
{
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr ;
	 keyboard_status_t *cur_keyboard_status_ptr;
     uint8_t offset_sw_ch_id;
	 
     if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_Proc("04.07.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id;
    cur_keyboard_ctrl_ptr->access_flag = STATE_YES;
	for(offset_sw_ch_id = 0; offset_sw_ch_id < cur_keyboard_status_ptr->keyboard_num_sw_chs; ++offset_sw_ch_id)
	{
		if((SW_Allow_Access(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id)) != SUCCESS)
		{
			system_status_flag = ERR_DEV_ALLOW_ACCESS;
		    Error_or_Warning_Proc("04.07.02", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;
		}			
	}
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_No_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.08  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_No_Access(const uint8_t keyboard_ch_id)
{
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	 keyboard_status_t *cur_keyboard_status_ptr;
     uint8_t offset_sw_ch_id;
	 
   if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.08.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id;
    cur_keyboard_ctrl_ptr->access_flag = STATE_NO;
	Keyboard_Disable(keyboard_ch_id);
	for(offset_sw_ch_id = 0; offset_sw_ch_id < cur_keyboard_status_ptr->keyboard_num_sw_chs ; ++offset_sw_ch_id)
	{
		if((SW_No_Access(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id)) != SUCCESS)
		{
			system_status_flag = ERR_DEV_OPER_NO_ACCESS_FUNC;
		    Error_or_Warning_Proc("04.08.02", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;
		}			
	}	
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Init

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.09  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Init(const uint8_t keyboard_ch_id, const uint8_t init_mode)
{
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	 keyboard_status_t *cur_keyboard_status_ptr;
	 io_config_t keyboard_config;
	 uint8_t alloc_base_sw_ch_id, offset_sw_ch_id;
	 
	 if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_Proc("04.09.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	 cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id;
	 keyboard_config.io_ch = cur_keyboard_ctrl_ptr->io_ch_rowa;
	 keyboard_config.signal = PIN_SIG_DIGITAL;
	 keyboard_config.func = IO_FUNC_GPIO;
	 keyboard_config.dir = IO_DIR_OUTPUT;
	 keyboard_config.state = STATE_LOW;
	 keyboard_config.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	 keyboard_config.port_pin_len = KEYPAD_NUM_ROWS;		 
	 if((IO_Channels_Func_Set(&keyboard_config)) != SUCCESS)
	 {
		system_status_flag = ERR_GPIO_FUNC_SET;
		 Error_or_Warning_Proc("04.09.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 }
	 offset_sw_ch_id = KEYPAD_NUM_COLS;
	  keyboard_config.io_ch = cur_keyboard_ctrl_ptr->io_ch_col1;
	  keyboard_config.port_pin_len = offset_sw_ch_id;
	  if(cur_keyboard_ctrl_ptr->io_ch_enter_sw != IO_CH_INVALID)
	  {
		  ++offset_sw_ch_id;
	  }
	  if(cur_keyboard_ctrl_ptr->io_ch_bs_sw != IO_CH_INVALID)
	  {
		  ++offset_sw_ch_id;
	  }
	  cur_keyboard_status_ptr->keyboard_num_sw_chs = offset_sw_ch_id;
	  keyboard_config.state = cur_keyboard_ctrl_ptr->keypad_pressed_state;
	  keyboard_config.func_type = IO_FUNC_TYPE_GPIO_SW;
	  keyboard_config.ch_id_alloc_type = CH_ID_ALLOC_DYNAMIC;
	  keyboard_config.dir = IO_DIR_INPUT;
	  if((SW_Ch_ID_Check_And_Alloc( keyboard_config.ch_id_alloc_type, &alloc_base_sw_ch_id, cur_keyboard_status_ptr->keyboard_num_sw_chs)) != SUCCESS)
      {
		 system_status_flag = ERR_SW_CONFIG;
		 Error_or_Warning_Proc("04.09.03", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	  } 
	  if((SW_Para_Init(alloc_base_sw_ch_id, cur_keyboard_ctrl_ptr->io_ch_col1, keyboard_config.port_pin_len, cur_keyboard_ctrl_ptr->keypad_pressed_state )) != SUCCESS)
	  {
		 system_status_flag = ERR_SW_CONFIG;
		 Error_or_Warning_Proc("04.09.04", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	  }
	  cur_keyboard_status_ptr->base_sw_ch_id = alloc_base_sw_ch_id;
	 if((IO_Channels_Func_Set(&keyboard_config)) != SUCCESS)
	 {
		system_status_flag = ERR_GPIO_FUNC_SET;
		Error_or_Warning_Proc("04.09.05", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 }
	  offset_sw_ch_id = KEYPAD_NUM_COLS;
	  if(cur_keyboard_ctrl_ptr->io_ch_enter_sw != IO_CH_INVALID)
	  {
		 keyboard_config.io_ch = cur_keyboard_ctrl_ptr->io_ch_enter_sw;
	     keyboard_config.port_pin_len = 1;
	     keyboard_config.state = cur_keyboard_ctrl_ptr->enter_sw_pressed_state;
	     keyboard_config.func_type = IO_FUNC_TYPE_GPIO_SW;
	     keyboard_config.ch_id_alloc_type = CH_ID_ALLOC_FIXED;		 
	     keyboard_config.dir = IO_DIR_INPUT;		 
		  if((SW_Para_Init(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id, cur_keyboard_ctrl_ptr->io_ch_enter_sw, keyboard_config.port_pin_len, cur_keyboard_ctrl_ptr->enter_sw_pressed_state)) != SUCCESS)
		 {
			  system_status_flag = ERR_SW_CONFIG;
		      Error_or_Warning_Proc("04.09.06", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;
		 }		 
	     if((IO_Channels_Func_Set(&keyboard_config)) != SUCCESS)
	     {
		    system_status_flag = ERR_GPIO_FUNC_SET;
		    Error_or_Warning_Proc("04.09.07", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;
	     }
		 ++offset_sw_ch_id;
	  }
	  if(cur_keyboard_ctrl_ptr->io_ch_bs_sw != IO_CH_INVALID)
	  {
	     keyboard_config.io_ch = cur_keyboard_ctrl_ptr->io_ch_bs_sw;
	     keyboard_config.port_pin_len = 1;
	     keyboard_config.state = cur_keyboard_ctrl_ptr->bs_sw_pressed_state;
	     keyboard_config.func_type = IO_FUNC_TYPE_GPIO_SW;
	     keyboard_config.ch_id_alloc_type = CH_ID_ALLOC_FIXED;
	     keyboard_config.dir = IO_DIR_INPUT;                 	
		 if((SW_Para_Init(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id, cur_keyboard_ctrl_ptr->io_ch_bs_sw, keyboard_config.port_pin_len, cur_keyboard_ctrl_ptr->bs_sw_pressed_state)) != SUCCESS)
		 {
			  system_status_flag = ERR_SW_CONFIG;
		      Error_or_Warning_Proc("04.09.08", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;
		 }
	     if((IO_Channels_Func_Set(&keyboard_config)) != SUCCESS)
	     {
		    system_status_flag = ERR_GPIO_FUNC_SET;
		    Error_or_Warning_Proc("04.09.09", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;
	     }
	  }
	  if((Init_File(&stdin_keyboard, STDIN_FILE_DESP, CHAR_DEVICE, STREAM_TYPE_IO, STREAM_IO_INPUT )) != SUCCESS)
	  {
		  system_status_flag = ERR_INIT_IO_FILE_PROC;
		  Error_or_Warning_Proc("04.09.10", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	  }		  
	  return SUCCESS; 	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_DeInit

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.10 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_DeInit(const uint8_t keyboard_ch_id, const uint8_t deinit_mode)
{
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	 keyboard_status_t *cur_keyboard_status_ptr;
	 io_config_t keyboard_unconfig;	 
	 int16_t offset_keyboard_sw_ch_id;
	 
	 if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.10.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 Keyboard_No_Access(keyboard_ch_id);
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	 cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id;
	 keyboard_unconfig.io_ch = cur_keyboard_ctrl_ptr->io_ch_rowa;
	 keyboard_unconfig.func = IO_FUNC_GPIO;
	 keyboard_unconfig.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	 keyboard_unconfig.port_pin_len = KEYPAD_NUM_ROWS;	 
	 if((IO_Ch_Func_Reset(&keyboard_unconfig)) != SUCCESS)
	 {
		system_status_flag = ERR_IO_CH_FUNC_RESET;
		 Error_or_Warning_Proc("04.10.02", ERROR_OCCURED, system_status_flag);
        return system_status_flag;		
	 }	 
	 keyboard_unconfig.io_ch = cur_keyboard_ctrl_ptr->io_ch_col1;
	 keyboard_unconfig.port_pin_len = KEYPAD_NUM_COLS;	
	 keyboard_unconfig.func_type = IO_FUNC_TYPE_GPIO_SW;
	 keyboard_unconfig.ch_id_alloc_type = CH_ID_ALLOC_DYNAMIC;	 
	 if((IO_Ch_Func_Reset(&keyboard_unconfig)) != SUCCESS)
	 {
		system_status_flag = ERR_IO_CH_FUNC_RESET;
		Error_or_Warning_Proc("04.10.03", ERROR_OCCURED, system_status_flag);
        return system_status_flag;		
	 }
	 if(cur_keyboard_ctrl_ptr->io_ch_enter_sw != IO_CH_INVALID)
	 {
		 keyboard_unconfig.io_ch = cur_keyboard_ctrl_ptr->io_ch_enter_sw;
	     keyboard_unconfig.port_pin_len = 1;	
	     keyboard_unconfig.func_type = IO_FUNC_TYPE_GPIO_SW;
	     keyboard_unconfig.ch_id_alloc_type = CH_ID_ALLOC_FIXED;
		 if((IO_Ch_Func_Reset(&keyboard_unconfig)) != SUCCESS)
	     {
		     system_status_flag = ERR_IO_CH_FUNC_RESET;
		     Error_or_Warning_Proc("04.10.04", ERROR_OCCURED, system_status_flag);
            return system_status_flag;		
	     }
	 }
	 if(cur_keyboard_ctrl_ptr->io_ch_bs_sw != IO_CH_INVALID)
	 {
		 keyboard_unconfig.io_ch = cur_keyboard_ctrl_ptr->io_ch_bs_sw;
	     keyboard_unconfig.port_pin_len = 1;	
	     keyboard_unconfig.func_type = IO_FUNC_TYPE_GPIO_SW;
	     keyboard_unconfig.ch_id_alloc_type = CH_ID_ALLOC_FIXED;
		 if((IO_Ch_Func_Reset(&keyboard_unconfig)) != SUCCESS)
	     {
		     system_status_flag = ERR_IO_CH_FUNC_RESET;
		     Error_or_Warning_Proc("04.10.05", ERROR_OCCURED, system_status_flag);
            return system_status_flag;		
	     }
	 }
	 for(offset_keyboard_sw_ch_id = cur_keyboard_status_ptr->keyboard_num_sw_chs - 1; offset_keyboard_sw_ch_id >= 0 ; --offset_keyboard_sw_ch_id)
	 {
		 Reset_SW_Ch(cur_keyboard_status_ptr->base_sw_ch_id + offset_keyboard_sw_ch_id, RESET_WHOLE);
	 }
	 cur_keyboard_status_ptr->base_sw_ch_id = CH_ID_INVALID;
	 cur_keyboard_status_ptr->keyboard_num_sw_chs = 0;
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Part_Oper_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.11 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Part_Oper_Proc(const uint8_t keyboard_ch_id, const uint8_t keyboard_part_and_oper_data)
{
	typedef uint16_t (*keyboard_part_oper_t)(const uint8_t keyboard_ch_id, const uint8_t dev_oper);
	keyboard_part_oper_t Keyboard_Part_Oper_Func_Ptr;
		
	if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.11.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	}
	switch(keyboard_part_and_oper_data >> KEYBOARD_PART_DATA_POS )
	{
		case PART_KEYPAD:
		  Keyboard_Part_Oper_Func_Ptr = Keyboard_Keypad_Oper;
		break;  
        case PART_ENTER_SW:
		  Keyboard_Part_Oper_Func_Ptr = Keyboard_Enter_SW_Oper;
		break;
        case PART_BS_SW:
		   Keyboard_Part_Oper_Func_Ptr = Keyboard_BS_SW_Oper;
		break;		
		default:
		  system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("04.11.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;		  
	}
	if(((*Keyboard_Part_Oper_Func_Ptr)(keyboard_ch_id, (keyboard_part_and_oper_data & 0x0F)))!= SUCCESS)
	{
		system_status_flag = ERR_KEYBOARD_PART_OPER;
		Error_or_Warning_Proc("04.11.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Keypad_Oper

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.12  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Keypad_Oper(const uint8_t keyboard_ch_id, const uint8_t dev_oper)
{
	 typedef uint16_t (*keypad_oper_t)(const uint8_t sw_ch_id);
	 keypad_oper_t Keypad_Oper_Func_Ptr;
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	 keyboard_status_t *cur_keyboard_status_ptr;
	 uint16_t error_status;
	 uint8_t offset_sw_ch_id;	
	 
	 if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.12.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id; 
	 cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id; 
     switch(dev_oper)
	 {
        case DEV_DISABLE_OPER: 		 
           cur_keyboard_ctrl_ptr->keypad_keys_enable_flag = STATE_NO;
		   Keypad_Oper_Func_Ptr = SW_Disable;
		   error_status = ERR_DEV_OPER_DISABLE_FUNC;
		break;   
        case DEV_ENABLE_OPER:
          cur_keyboard_ctrl_ptr->keypad_keys_enable_flag = STATE_YES;
		  Keypad_Oper_Func_Ptr = SW_Enable;
		  error_status = ERR_DEV_OPER_ENABLE_FUNC;
        break;  
        case DEV_NO_ACCESS_OPER:
           	Keypad_Oper_Func_Ptr = SW_No_Access;
			error_status = ERR_DEV_OPER_NO_ACCESS_FUNC;
        break;
		case DEV_ALLOW_ACCESS_OPER:
     		Keypad_Oper_Func_Ptr = SW_Allow_Access;
			error_status = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
		break;	
		default:
          system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("04.12.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;	
	 }	 
	 for(offset_sw_ch_id = 0; offset_sw_ch_id < KEYPAD_NUM_COLS; ++offset_sw_ch_id)
	 {
		if(((*Keypad_Oper_Func_Ptr)(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id)) != SUCCESS)
		{
			 system_status_flag = error_status;
		     Error_or_Warning_Proc("04.12.03", ERROR_OCCURED, system_status_flag);
		     return system_status_flag;
		}
	 }	
     return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Enter_SW_Oper

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.13  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Enter_SW_Oper(const uint8_t keyboard_ch_id, const uint8_t dev_oper)
{
	 typedef uint16_t (*enter_sw_oper_t)(const uint8_t sw_ch_id);
	 enter_sw_oper_t Enter_SW_Oper_Func_Ptr;
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr ;
	 keyboard_status_t *cur_keyboard_status_ptr;
	 uint16_t error_status;
	 
	 if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.13.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id; 
	 cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id; 
     switch(dev_oper)
	 {
        case DEV_DISABLE_OPER: 	           
		    Enter_SW_Oper_Func_Ptr = SW_Disable;
			error_status = ERR_DEV_OPER_DISABLE_FUNC;
		break;   
        case DEV_ENABLE_OPER:         
		   Enter_SW_Oper_Func_Ptr = SW_Enable;
		   error_status = ERR_DEV_OPER_ENABLE_FUNC;
        break;  
        case DEV_NO_ACCESS_OPER:
           Enter_SW_Oper_Func_Ptr = SW_No_Access;
		   error_status = ERR_DEV_OPER_NO_ACCESS_FUNC;
        break;
		case DEV_ALLOW_ACCESS_OPER:
     		Enter_SW_Oper_Func_Ptr = SW_Allow_Access;
			error_status = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
		break;
        default:
          system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("04.13.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;	
	 }	
	if(cur_keyboard_ctrl_ptr->io_ch_enter_sw == IO_CH_INVALID)
	{
		 system_status_flag = ERR_KEYBOARD_ENTER_SW_OPER_BUT_NOT_CONFIG;
		 Error_or_Warning_Proc("04.13.03", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;		
	} 		
    if(((*Enter_SW_Oper_Func_Ptr)(cur_keyboard_status_ptr->base_sw_ch_id + KEYPAD_NUM_COLS)) != SUCCESS)
	{
		system_status_flag = error_status;
		Error_or_Warning_Proc("04.13.04", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_BS_SW_Oper

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.14  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_BS_SW_Oper(const uint8_t keyboard_ch_id, const uint8_t dev_oper)
{
	 typedef uint16_t (*bs_sw_oper_t)(const uint8_t sw_ch_id);
	 bs_sw_oper_t BS_SW_Oper_Func_Ptr;
	 keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	 keyboard_status_t *cur_keyboard_status_ptr;
	 uint16_t error_status;
	 uint8_t offset_sw_ch_id;
	 
	 if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.14.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id; 
	  cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id; 
     switch(dev_oper)
	 {
        case DEV_DISABLE_OPER: 	           
		    BS_SW_Oper_Func_Ptr = SW_Disable;
			error_status = ERR_DEV_OPER_DISABLE_FUNC;
		break;   
        case DEV_ENABLE_OPER:         
		   BS_SW_Oper_Func_Ptr = SW_Enable;
		   error_status = ERR_DEV_OPER_ENABLE_FUNC;
        break;  
        case DEV_NO_ACCESS_OPER:
           BS_SW_Oper_Func_Ptr = SW_No_Access;
		    error_status = ERR_DEV_OPER_NO_ACCESS_FUNC;
        break;
		case DEV_ALLOW_ACCESS_OPER:
     	   BS_SW_Oper_Func_Ptr = SW_Allow_Access;
		   error_status = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
		break;
        default:
          system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("04.14.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;	
	 }	
	if(cur_keyboard_ctrl_ptr->io_ch_bs_sw == IO_CH_INVALID)
	{
		system_status_flag = ERR_KEYBOARD_BS_SW_OPER_BUT_NOT_CONFIG;
		Error_or_Warning_Proc("04.14.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	offset_sw_ch_id = KEYPAD_NUM_COLS;
    if(cur_keyboard_ctrl_ptr->io_ch_enter_sw != IO_CH_INVALID)
	{
		++offset_sw_ch_id; 
	}
    if(((*BS_SW_Oper_Func_Ptr)(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id))!= SUCCESS)
	{
		system_status_flag = error_status;
		Error_or_Warning_Proc("04.14.04", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	return SUCCESS; 
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Enter_SW_Read_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.15  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Enter_SW_Read_Proc(const uint8_t keyboard_ch_id)
{
	keyboard_ctrl_t *cur_keyboard_ctrl_ptr;
	keyboard_status_t *cur_keyboard_status_ptr;
	uint16_t ret_status;
  
	if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.15.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	 cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id; 
     if(cur_keyboard_ctrl_ptr->io_ch_enter_sw == IO_CH_INVALID)
	 {
		 system_status_flag = ERR_KEYBOARD_ENTER_SW_OPER_BUT_NOT_CONFIG;
		 Error_or_Warning_Proc("04.15.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;		
	} 		
    ret_status = SW_Press_Proc(cur_keyboard_status_ptr->base_sw_ch_id + KEYPAD_NUM_COLS, NULL_DATA_PTR);
	switch(ret_status)
	{
		case SUCCESS:		
		case SW_OR_KEY_NOT_PRESSED:
		case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		case WARN_CUR_DATA_ID_DEV_DISABLED:
		   return ret_status;
        //break;        
	}	
    system_status_flag = ERR_KEYBOARD_ENTER_SW_PROC;
	Error_or_Warning_Proc("04.15.03", ERROR_OCCURED, system_status_flag);
	return system_status_flag;		
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_BS_SW_Read_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.16  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_BS_SW_Read_Proc(const uint8_t keyboard_ch_id)
{
	keyboard_ctrl_t *cur_keyboard_ctrl_ptr ;
	keyboard_status_t *cur_keyboard_status_ptr;
	uint16_t ret_status;
	uint8_t offset_sw_ch_id;
  
	if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.16.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_keyboard_ctrl_ptr = keyboard_ctrl + keyboard_ch_id;
	 cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id; 
     if(cur_keyboard_ctrl_ptr->io_ch_bs_sw == IO_CH_INVALID)
	 {
		 system_status_flag = ERR_KEYBOARD_BS_SW_OPER_BUT_NOT_CONFIG;
		 Error_or_Warning_Proc("04.16.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;		
	 }
	offset_sw_ch_id = KEYPAD_NUM_COLS;
    if(cur_keyboard_ctrl_ptr->io_ch_enter_sw != IO_CH_INVALID)
	{
		++offset_sw_ch_id;
	}
    ret_status = SW_Press_Proc(cur_keyboard_status_ptr->base_sw_ch_id + offset_sw_ch_id, NULL_DATA_PTR);
	switch(ret_status)
	{
		case SUCCESS:
		case SW_OR_KEY_NOT_PRESSED:
		case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		case WARN_CUR_DATA_ID_DEV_DISABLED:
		   return ret_status;
        //break;        
	}	
    system_status_flag = ERR_KEYBOARD_BS_SW_PROC;
	Error_or_Warning_Proc("04.16.03", ERROR_OCCURED, system_status_flag);
	return system_status_flag;		
}

/*------------------------------------------------------------*
FUNCTION NAME  : Keyboard_Ready_Read

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.17 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Keyboard_Ready_Read(const uint8_t keyboard_ch_id)
{
  uint16_t ret_status;
  
	 if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.18.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }	 
	if((Keyboard_Keypad_Oper(keyboard_ch_id, DEV_ALLOW_ACCESS_OPER)) != SUCCESS)
	{
		  system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
		  Error_or_Warning_Proc("04.18.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	}
	if((Keyboard_Keypad_Oper(keyboard_ch_id, DEV_ENABLE_OPER)) != SUCCESS)
	{
		system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
		Error_or_Warning_Proc("04.18.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	ret_status = Keyboard_Enter_SW_Oper(keyboard_ch_id, DEV_ALLOW_ACCESS_OPER);
	switch(ret_status)
	{
		case ERR_KEYBOARD_ENTER_SW_OPER_BUT_NOT_CONFIG:
		break;
		case SUCCESS:
		    if((Keyboard_Enter_SW_Oper(keyboard_ch_id, DEV_ENABLE_OPER)) != SUCCESS)
			{
				system_status_flag = ERR_DEV_OPER_ENABLE_FUNC;
		        Error_or_Warning_Proc("04.18.04", ERROR_OCCURED, system_status_flag);
		        return system_status_flag;
			}
		break; 
	}
    ret_status = Keyboard_BS_SW_Oper(keyboard_ch_id, DEV_ALLOW_ACCESS_OPER);
	switch(ret_status)
	{
		case ERR_KEYBOARD_BS_SW_OPER_BUT_NOT_CONFIG:
		break;
		case SUCCESS:
		    if((Keyboard_BS_SW_Oper(keyboard_ch_id, DEV_ENABLE_OPER)) != SUCCESS)
			{
				system_status_flag = ERR_DEV_OPER_ENABLE_FUNC;
		        Error_or_Warning_Proc("04.18.05", ERROR_OCCURED, system_status_flag);
		        return system_status_flag;
			}
		break; 
	}
	if((Keyboard_Allow_Access(keyboard_ch_id)) != SUCCESS)
	{
		 system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
		 Error_or_Warning_Proc("04.18.06", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}		
	if((Keyboard_Enable(keyboard_ch_id) ) != SUCCESS)
	{
		system_status_flag = ERR_DEV_OPER_ENABLE_FUNC;
		 Error_or_Warning_Proc("04.18.07", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Scan

DESCRIPTION    : Scan() operation is similar to scanf()
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.18 

Bugs           :  
-*------------------------------------------------------------*/
int16_t Scan(const char *const format_str,...) 
{ 
    va_list ap;
	int16_t count;
	
	if(format_str == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("04.18.01", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
	if(*format_str == NULL_CHAR)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("04.18.02", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
	va_start(ap, format_str);
    count = VFile_Scan(&stdin_keyboard, format_str, &ap);
	va_end(ap);
	if(count == EOF)
	{
		system_status_flag = ERR_EOF;
		Error_or_Warning_Proc("04.18.03", ERROR_OCCURED, system_status_flag);
	}
	return(count);
} 

/*------------------------------------------------------------*
FUNCTION NAME  : Get_Char

DESCRIPTION    : Get_Char() operation is similar to getchar()
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.19 

Bugs           :  
-*------------------------------------------------------------*/
char Get_Char(void)
{
	dev_id_t keyboard_or_uart_dev_id;
	int16_t last_read_char = NULL_CHAR;
		
	keyboard_or_uart_dev_id.dev_id = INPUT_DEV_ID_KEYBOARD;
	if((Data_ID_Src_Spec_Dev_Find_Dev_Ch_ID(cur_data_id, &keyboard_or_uart_dev_id, DEV_INPUT_TYPE))!= SUCCESS)
	{
	    system_status_flag = ERR_CUR_DATA_DEV_NOT_SRC;
	    Error_or_Warning_Proc("04.19.01", ERROR_OCCURED, system_status_flag);
	    return NULL_CHAR;
	}	
	if((Keyboard_Ready_Read(keyboard_or_uart_dev_id.dev_ch_id)) != SUCCESS)
	{
		system_status_flag = ERR_KEYBOARD_READY_READ;
	    Error_or_Warning_Proc("04.19.02", ERROR_OCCURED, system_status_flag);
	    return NULL_CHAR;
	}
  do
	{
        last_read_char = Stream_Stdin_Oper(keyboard_or_uart_dev_id.dev_ch_id);
	}
    while(last_read_char == NULL_CHAR && last_read_char != EOF);	
	#ifdef UART_MOD_ENABLE	    
       keyboard_or_uart_dev_id.dev_id = COMM_DEV_ID_UART;
	   if((Data_ID_Src_Spec_Dev_Find_Dev_Ch_ID(cur_data_id, &keyboard_or_uart_dev_id, DEV_TRANSMIT_TYPE))!= SUCCESS)
	   {
	        keyboard_or_uart_dev_id.dev_ch_id = TRACE_UART_CH_ID;	    
	   }	
       if((UART_Transmit_Char(keyboard_or_uart_dev_id.dev_ch_id, last_read_char)) != SUCCESS)
	   {
		  system_status_flag = ERR_UART_TX_PROC;
		  Error_or_Warning_Proc("04.19.04", ERROR_OCCURED, system_status_flag);
		  return NULL_CHAR;
	   }
    #endif	
	return last_read_char;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Get_Str

DESCRIPTION    : Get_Str() operation is similar to gets()
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.20 

Bugs           :  
-*------------------------------------------------------------*/
char *Get_Str(char *const get_str)
{
	char *read_ptr;
    dev_id_t keyboard_dev_id, uart_dev_id;
	int16_t last_read_char = NULL_CHAR;
	uint8_t num_chars_read_str = 0;	
	
	if(get_str == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
	    Error_or_Warning_Proc("04.20.01", ERROR_OCCURED, system_status_flag);
	    return NULL_DATA_PTR;
	}
	read_ptr = get_str;
	keyboard_dev_id.dev_id = INPUT_DEV_ID_KEYBOARD;
	if((Data_ID_Src_Spec_Dev_Find_Dev_Ch_ID(cur_data_id, &keyboard_dev_id, DEV_INPUT_TYPE))!= SUCCESS)
	{
	    system_status_flag = ERR_CUR_DATA_DEV_NOT_SRC;
	    Error_or_Warning_Proc("04.20.02", ERROR_OCCURED, system_status_flag);
	    return NULL_DATA_PTR;
	}	
	if((Keyboard_Ready_Read(keyboard_dev_id.dev_ch_id)) != SUCCESS)
	{
		system_status_flag = ERR_KEYBOARD_READY_READ;
	    Error_or_Warning_Proc("04.20.03", ERROR_OCCURED, system_status_flag);
	    return NULL_DATA_PTR;
	}
	uart_dev_id.dev_id = COMM_DEV_ID_UART;
	if((Data_ID_Src_Spec_Dev_Find_Dev_Ch_ID(cur_data_id, &uart_dev_id, DEV_TRANSMIT_TYPE))!= SUCCESS)
	{
	     uart_dev_id.dev_ch_id = TRACE_UART_CH_ID;	    
	}
	do
	{
		if(stdin_keyboard.num_chars_stream_buf + stdin_keyboard.stream_buf_cur_pos + num_chars_read_str + 1 > MAX_STREAM_BUFFER_SIZE )
		{
			system_status_flag = ERR_DATA_OUT_OF_RANGE;
	        Error_or_Warning_Proc("04.20.02", ERROR_OCCURED, system_status_flag);            
			*read_ptr = NULL_CHAR;
	        return NULL_DATA_PTR;
		}
        last_read_char = Stream_Stdin_Oper(keyboard_dev_id.dev_ch_id);
		if(last_read_char != NULL_CHAR && last_read_char != EOF )
		{
			if(last_read_char != BACKSPACE_CHAR && last_read_char != ENTER_CHAR)
			{
				#ifdef UART_MOD_ENABLE               
                   if((UART_Transmit_Char(uart_dev_id.dev_ch_id, last_read_char)) != SUCCESS)
	               {
		                 system_status_flag = ERR_UART_TX_PROC;
		                 Error_or_Warning_Proc("04.20.05", ERROR_OCCURED, system_status_flag);
		                 return read_ptr;
	               }
                 #endif
				  *read_ptr = last_read_char;
			      ++num_chars_read_str;
			      ++read_ptr;
                  *read_ptr = NULL_CHAR;	
			}
		    else
		    {
			   if(last_read_char == BACKSPACE_CHAR && num_chars_read_str > 0 )
			   {
				   #ifdef UART_MOD_ENABLE               
                      if((UART_Transmit_Char(uart_dev_id.dev_ch_id, last_read_char)) != SUCCESS)
	                  {
		                 system_status_flag = ERR_UART_TX_PROC;
		                 Error_or_Warning_Proc("04.20.06", ERROR_OCCURED, system_status_flag);
		                 return read_ptr;
	                  }
                   #endif
				    --num_chars_read_str;
				   --read_ptr;
				   *read_ptr = NULL_CHAR;				   
			   }
		   }
		}			
	}
    while(last_read_char != ENTER_CHAR && last_read_char != EOF);
	*read_ptr = NULL_CHAR;
	File_Flush(&stdin_keyboard);	
	return read_ptr;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Stream_Stdin_Oper

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.21 

Bugs           :  
-*------------------------------------------------------------*/
static int16_t Stream_Stdin_Oper(const uint8_t keyboard_ch_id)
{
	file_t *stdin_ptr = &stdin_keyboard;
	dev_read_ctrl_para_t dev_read_ctrl_para;
	uint16_t ret_status;
	char last_read_char = NULL_CHAR;
	
    ret_status =  Keyboard_Read(keyboard_ch_id, &dev_read_ctrl_para);
	switch(ret_status)
    {
        case DATA_CHAR_READ:
	      last_read_char = dev_read_ctrl_para.read_data_char;
		  if(last_read_char != BACKSPACE_CHAR)
		  {
			 if(stdin_ptr->stream_buf_cur_pos + stdin_ptr->num_chars_stream_buf + 1 >= MAX_STREAM_BUFFER_SIZE)
			 {
			 	 system_status_flag = ERR_DATA_OUT_OF_RANGE;
	             Error_or_Warning_Proc("04.21.01", ERROR_OCCURED, system_status_flag);
	     		 return EOF;                     
			 }
			  stdin_ptr->stream_buf[stdin_ptr->stream_buf_cur_pos + stdin_ptr->num_chars_stream_buf] = last_read_char;
			  ++stdin_ptr->num_chars_stream_buf; 
			  stdin_ptr->stream_buf[stdin_ptr->stream_buf_cur_pos + stdin_ptr->num_chars_stream_buf] = NULL_CHAR;
	      }				
          else
		  {
		     //read char is BACKSPACE_CHAR
			 if(stdin_ptr->num_chars_stream_buf > 0)
			 {
				 --stdin_ptr->num_chars_stream_buf; 
			     stdin_ptr->stream_buf[stdin_ptr->stream_buf_cur_pos + stdin_ptr->num_chars_stream_buf] = NULL_CHAR;			     							                     
			 }
	      } 	
	    break;              
        case SW_OR_KEY_NOT_PRESSED:
        break;		   
        default: 
            system_status_flag = ERR_DEV_OPER_READ_FUNC;
		    Error_or_Warning_Proc("04.21.02", ERROR_OCCURED, system_status_flag);
		    return EOF;
    }
	return last_read_char;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_All_Keyboard_Chs

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.22 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reset_All_Keyboard_Chs(const uint8_t keyboard_reset_type)
{
	int16_t keyboard_ch_id;
	
	if(keyboard_reset_type != RESET_WHOLE && keyboard_reset_type != RESET_MOD_STATUS)
	{
		system_status_flag = ERR_FORMAT_INVALID;
	    Error_or_Warning_Proc("04.22.01", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	File_Flush(&stdin_keyboard);
	for(keyboard_ch_id = NUM_INPUT_DEV_ID_KEYBOARD_CHS - 1; keyboard_ch_id >= 0 ; --keyboard_ch_id)
	{
		Reset_Keyboard_Ch(keyboard_ch_id, keyboard_reset_type);
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Keyboard_Ch

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 04.23 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reset_Keyboard_Ch(const uint8_t keyboard_ch_id, const uint8_t keyboard_reset_type)
{
	keyboard_status_t *cur_keyboard_status_ptr;
	int16_t offset_keyboard_sw_ch_id;
	
	if(keyboard_ch_id >= NUM_INPUT_DEV_ID_KEYBOARD_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("04.23.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	}
	if(keyboard_reset_type != RESET_WHOLE && keyboard_reset_type != RESET_MOD_STATUS)
	{
		system_status_flag = ERR_FORMAT_INVALID;
		Error_or_Warning_Proc("04.23.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	cur_keyboard_status_ptr = keyboard_status + keyboard_ch_id;
	Keyboard_No_Access(keyboard_ch_id);
	for(offset_keyboard_sw_ch_id = cur_keyboard_status_ptr->keyboard_num_sw_chs - 1; offset_keyboard_sw_ch_id >= 0; --offset_keyboard_sw_ch_id)
	{
		 Reset_SW_Ch(cur_keyboard_status_ptr->base_sw_ch_id + offset_keyboard_sw_ch_id, keyboard_reset_type);
	}
    if(keyboard_reset_type == RESET_WHOLE)
	{
		cur_keyboard_status_ptr->base_sw_ch_id = CH_ID_INVALID;
		cur_keyboard_status_ptr->keyboard_num_sw_chs = 0;
	}		
	return SUCCESS;
}
		
#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
