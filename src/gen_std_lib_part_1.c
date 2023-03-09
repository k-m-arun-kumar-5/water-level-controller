/* ********************************************************************
FILE                   : gen_std_lib_part_1.c

PURPOSE                : 
                     									 	 
AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            : 

NOTE                  : 
                       
CHANGE LOGS           : 

FILE ID               : 02 

*****************************************************************************/

#include "main.h"

/* ------------------------------ macro defination ------------------------------ */
/* endianness testing */
const uint32_t endian_test_data = 0x04030201;

/* ----------------------------- global variable defination --------------------- */
uint8_t internal_error_state = NO_ERROR_OCCURED;

/* ----------------------------- global variable declaration -------------------- */


/* ----------------------------- global function declaration -------------------- */


/*------------------------------------------------------------*
FUNCTION NAME  : SW_Time_Delay

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.01  

BUGS           :              
-*------------------------------------------------------------*/
void SW_Time_Delay(const uint32_t max_i_count, const uint32_t max_j_count)
{
	 uint32_t i, j;
  
	 for(i = 0; i < max_i_count; ++i)
	 { 
         for(j = 0;j < max_j_count; ++j);
	 } 
}

/*------------------------------------------------------------*-
FUNCTION NAME  : Config_Consucc_Bits

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.02 

BUGS           :
-*------------------------------------------------------------*/
uint16_t Config_Consucc_Bits( const uint8_t flag_consucc_bit, void *const data_ptr)
{
	uint32_t from_bit0_consucc_bits = 0;	
	consucc_bit_t *consucc_bit_ptr = (consucc_bit_t *)data_ptr;
	uint8_t i, end_bit_pos;     
	
	end_bit_pos = consucc_bit_ptr->start_bit_pos + consucc_bit_ptr->bits_len - 1;
	if(consucc_bit_ptr == NULL_DATA_PTR || end_bit_pos >= INT_BIT_SIZE)
	{
		system_status_flag = ERR_CONSUCC_PARA;
		 Error_or_Warning_Proc("02.02.01", ERROR_OCCURED, system_status_flag);
		return  system_status_flag;
	}
	for(i = 0; i < consucc_bit_ptr->bits_len; ++i)
	{
	  	from_bit0_consucc_bits |= 1 << i;			
	}
	switch(flag_consucc_bit)
	{
         case FLAG_CONSUCC_BITS_1:
		    consucc_bit_ptr->write_or_config_consucc_val |=  (from_bit0_consucc_bits << consucc_bit_ptr->start_bit_pos);
	     break;		
	     case FLAG_CONSUCC_BITS_0:
		    consucc_bit_ptr->write_or_config_consucc_val &= ~(from_bit0_consucc_bits << consucc_bit_ptr->start_bit_pos);
	     break;
		 case FLAG_CONSUCC_BITS_TOGGLE:
		    consucc_bit_ptr->write_or_config_consucc_val ^= (from_bit0_consucc_bits << consucc_bit_ptr->start_bit_pos);
		 break;
		 default:
		    system_status_flag = ERR_CONSUCC_PARA;
			 Error_or_Warning_Proc("02.02.02", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*-
FUNCTION NAME  : Test_Consucc_Bits

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.03 

BUGS           :
-*------------------------------------------------------------*/
uint16_t Test_Consucc_Bits( const uint8_t flag_consucc_bit, const void *const data_ptr)
{
	uint32_t from_bit0_consucc_bits = 0, mask_configured_bits ;
	consucc_bit_t *consucc_bit_ptr = (consucc_bit_t *)data_ptr;
	uint8_t i, end_bit_pos;	
	
	end_bit_pos = consucc_bit_ptr->start_bit_pos + consucc_bit_ptr->bits_len - 1;
	if(consucc_bit_ptr == NULL_DATA_PTR || end_bit_pos >= INT_BIT_SIZE)
	{
		system_status_flag = ERR_CONSUCC_PARA;
		 Error_or_Warning_Proc("02.03.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	for(i = 0; i < consucc_bit_ptr->bits_len; ++i)
	{
	  	from_bit0_consucc_bits |= 1 << i;			
	}
	switch(flag_consucc_bit)
	{
         case FLAG_CONSUCC_BITS_1:
					 mask_configured_bits = from_bit0_consucc_bits << consucc_bit_ptr->start_bit_pos;
		       if( mask_configured_bits == (consucc_bit_ptr->read_or_test_consucc_val & mask_configured_bits))
			   {
					    return TEST_OK_1_CONSUCC_BITS;
			   }
			   system_status_flag = ERR_TEST_FAIL_1_CONSUCC_BITS;
			   return TEST_FAIL_1_CONSUCC_BITS;				  
	    // break;		
	     case FLAG_CONSUCC_BITS_0:
				  mask_configured_bits = ~(from_bit0_consucc_bits << consucc_bit_ptr->start_bit_pos);
			    if(mask_configured_bits == (consucc_bit_ptr->read_or_test_consucc_val | mask_configured_bits))
					{
					    return TEST_OK_0_CONSUCC_BITS;
				  }
				  system_status_flag = ERR_TEST_FAIL_0_CONSUCC_BITS;
					return TEST_FAIL_0_CONSUCC_BITS;				 		     
	    // break;
		 case FLAG_CONSUCC_BITS_VAL:
		     mask_configured_bits = from_bit0_consucc_bits << consucc_bit_ptr->start_bit_pos;
			 if((consucc_bit_ptr->read_or_test_consucc_val & mask_configured_bits) == ((consucc_bit_ptr->write_or_config_consucc_val << consucc_bit_ptr->start_bit_pos) & mask_configured_bits))
			 {
			     return TEST_OK_VAL_CONSUCC_BITS;
			 }
			 system_status_flag = ERR_TEST_FAIL_VAL_CONSUCC_BITS;
			 return TEST_FAIL_VAL_CONSUCC_BITS;			
		 //break;
			 default:
			   system_status_flag = ERR_CONSUCC_PARA;
			   Error_or_Warning_Proc("02.03.02", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;
	}
	return SUCCESS;
}

#ifdef STR_TO_NUM_FROM_RIGHT_TO_LEFT
/*------------------------------------------------------------*
FUNCTION NAME  : Str_to_Num_Conv

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : digits are extracted from right to left format from least digit in num_in_str
                 and no sign symbol is present at num_in_str[0] 

Func ID        : 02.04  

BUGS           :  
-*------------------------------------------------------------*/
uint16_t Str_to_Num_Conv(int32_t *const num_conv_from_str, const char *const num_in_str  )
{	
	 int32_t num = 0;
	 uint32_t place = 1;
	 int8_t cur_unit;
	 uint8_t num_chars = 0, cur_digit= 0, base = 10, pos = 0 ;
	
	 if(num_conv_from_str == NULL_DATA_PTR || num_in_str == NULL_DATA_PTR )
	 {
		 system_status_flag = ERR_STR_TO_NUM_PARA;
		 Error_or_Warning_Proc("02.04.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	 }
	 num_chars = Str_Len(num_in_str);	
     place = 1;
     pos = num_chars - 1;
     cur_unit = num_in_str[pos] - NUM_0_CHAR;
	 if(cur_unit < 0 ||  cur_unit > 9 )
	 {
		 *num_conv_from_str = 0;
         system_status_flag = ERR_STR_TO_NUM_PARA;	
		  Error_or_Warning_Proc("02.04.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	 }	
     num = place * cur_unit;
     for(cur_digit = 1; cur_digit < num_chars; ++cur_digit)
     {
         place =  place * base;
         pos = num_chars - 1 - cur_digit;
         cur_unit = num_in_str[pos] - NUM_0_CHAR;
	     if(cur_unit < 0 ||  cur_unit > 9 )
	     {
			 *num_conv_from_str = 0;
			 system_status_flag = ERR_STR_TO_NUM_PARA;
			 Error_or_Warning_Proc("02.04.03", ERROR_OCCURED, system_status_flag);
			 return system_status_flag;
		 }			 
         num += (cur_unit * place);     
     }
	 *num_conv_from_str = num; 	 
     return SUCCESS;
}

#else
/*------------------------------------------------------------*
FUNCTION NAME  : Str_to_Num_Conv

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : digits are extracted from left to right format from digit in num_in_str

Func ID        : 02.04  

BUGS           :  
-*------------------------------------------------------------*/
uint16_t Str_to_Num_Conv(int32_t *const num_conv_from_str, const char *const num_in_str  )
{	
	 int32_t num = 0;
	 uint32_t place;
	 int16_t cur_unit;
	 uint8_t num_chars = 0, base = BASE_10, pos = 0, start_num_pos = 0 ;
	
	 if(num_conv_from_str == NULL_DATA_PTR || num_in_str == NULL_DATA_PTR )
	 {
		 system_status_flag = ERR_NULL_PTR;
		   Error_or_Warning_Proc("02.04.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	 }
	 if(num_in_str[0] == '+' || num_in_str[0] == '-')
	 {
		 start_num_pos = 1;
	 }
	 else if(num_in_str[0] >= '0' && num_in_str[0] <= '9')
	 {
		  start_num_pos = 0;
	 }
	 else
	 {
		 *num_conv_from_str = 0;
         system_status_flag = ERR_DATA_OUT_OF_RANGE;
		   Error_or_Warning_Proc("02.04.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag; 
	 }		 
	 num_chars = Str_Len(num_in_str + start_num_pos);
	 if(num_chars == 0)
	 {
		 *num_conv_from_str = 0;
		 system_status_flag = ERR_DATA_OUT_OF_RANGE;
		 Error_or_Warning_Proc("02.04.03", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	 }
	 pos = start_num_pos; 	     
     for( place = Power_Of(base, num_chars - 1); place >= 1; place /= base, ++pos )
     {
     	 cur_unit = num_in_str[pos] - NUM_0_CHAR;
    	 if(cur_unit < 0 ||  cur_unit > 9 )
    	 {
	    	 *num_conv_from_str = 0;
             system_status_flag = ERR_DATA_OUT_OF_RANGE;	
			   Error_or_Warning_Proc("02.04.04", ERROR_OCCURED, system_status_flag);
		     return system_status_flag;
	     }		 
         num += (cur_unit * place);		      
     }
	 if(num_in_str[0] == '-')
	 {
		 *num_conv_from_str = -num;  
	 }
	 else
	 {
	     *num_conv_from_str = num; 
	 }
     return SUCCESS;
}
#endif

/*------------------------------------------------------------*
FUNCTION NAME  : Str_Len

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.05  

BUGS           :  
-*------------------------------------------------------------*/
uint8_t Str_Len(const char *const str)
{
    uint8_t num_chars = 0;
	
	  if(str == NULL_DATA_PTR)
	  {
		  system_status_flag = ERR_STR_PTR_NULL;	
		  Error_or_Warning_Proc("02.05.01", ERROR_OCCURED, system_status_flag);
		  return 0;
	  }
    while(*(str + num_chars))
	{
		++num_chars;
	}
    return num_chars;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Data_ID_Status

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.06

BUGS           :
-*------------------------------------------------------------*/
uint16_t Reset_Data_ID_Status(const uint8_t data_id, const uint8_t set_cur_data_status_reset_type)
{
	data_id_status_para_t *data_id_status_para_ptr = NULL_DATA_PTR;
		
	if(data_id >= NUM_DATA_IDS)
	{
		 system_status_flag = ERR_DATA_ID_EXCEED;
		   Error_or_Warning_Proc("02.06.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	data_id_status_para_ptr = data_id_status_para + data_id;
	switch(set_cur_data_status_reset_type)
	{
		case DATA_ID_RESET_INIT_STATUS:
        #ifdef DMA_DATA_ID_STR
	   if(data_id_status_para_ptr->data_value.data_str != NULL_DATA_PTR)
	    {
	    	free(data_id_status_para_ptr->data_value.data_str);
		    data_id_status_para_ptr->data_value.data_str = NULL_DATA_PTR;
	    } 
		#endif
		data_id_status_para_ptr->max_allocated_data_len = 0;
		case DATA_ID_RESET_WHOLE_STATUS:		 
	    data_id_status_para_ptr->data_read_num_try = 0;
		data_id_status_para_ptr->data_read_status.max_try_reached_flag = STATE_NO;
		data_id_status_para_ptr->data_read_status.last_try_read_and_ready_to_process_flag = STATE_NO;
		case DATA_ID_RESET_RETRY_STATUS:
		 data_id_status_para_ptr->data_value.data_str_len_or_pos = 0;        
	    data_id_status_para_ptr->data_value.data_num.val_uint_64  = DATA_NUM_INVALID;
	#ifdef DMA_DATA_ID_STR
    if(data_id_status_para_ptr->data_value.data_str != NULL_DATA_PTR)    		
	  {
         if(set_cur_data_status_reset_type != DATA_ID_RESET_INIT_STATUS)
         {
            memset(data_id_status_para_ptr->data_value.data_str, NULL_CHAR, data_id_status_para_ptr->max_allocated_data_len * sizeof(char) );
         }         
	  } 
  #else
    memset(data_id_status_para_ptr->data_value.data_str, NULL_CHAR, DATA_MAX_NUM_ALLOCATED_CHARS );
  #endif
	  case DATA_ID_RESET_APPEND_INPUT_STATUS_FLAG:
	    data_id_status_para_ptr->data_read_status.read_valid_terminator_char_flag = STATE_NO;
	    data_id_status_para_ptr->data_read_status.reach_max_alloc_read_chars_flag = STATE_NO; 
		data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag = STATE_NO; 		 
     break;
	 default:
    	system_status_flag = ERR_FORMAT_INVALID;
		 Error_or_Warning_Proc("02.06.02", ERROR_OCCURED, system_status_flag);
      return system_status_flag;	 
	}
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Data_IDs_Status

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.07

BUGS           :
-*------------------------------------------------------------*/
void Reset_Data_IDs_Status(void)
{
	 Set_Status_Reset_Data_IDs(DATA_ID_RESET_INIT_STATUS);
	 cur_data_id = DATA_ID_INVALID;	
	 error_or_warning_proc_flag = 0;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Data_IDs_Set_Para

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : 

Func ID        : 02.08 

Bugs           :   
-*------------------------------------------------------------*/
uint16_t Data_IDs_Set_Para(void)
{
	uint8_t data_id = 0, max_req_data_len;
	
	for(data_id = 0; data_id < NUM_DATA_IDS; ++data_id)
	{
	  	 if((Appl_Data_Retrieve_Para(data_id, DATA_RETRIEVE_CTRL_MAX_REQ_DATA_CHARS, &max_req_data_len)) != SUCCESS)
		 {
		     system_status_flag = ERR_APPL_DATA_RETRIEVE;
		     Error_or_Warning_Proc("02.08.01", ERROR_OCCURED, system_status_flag);
             return system_status_flag; 
		 }
		 if((Data_ID_Set_Para(data_id,  max_req_data_len)) != SUCCESS)
		 {
			 system_status_flag = ERR_DATA_ID_CONF;
			  Error_or_Warning_Proc("02.08.02", ERROR_OCCURED, system_status_flag);
			 break;
		 }
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Data_ID_Set_Para

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : 

Func ID        : 02.09 

Bugs           :   
-*------------------------------------------------------------*/
uint16_t Data_ID_Set_Para(const uint8_t data_id, const uint8_t max_alloc_data_len)
{
	data_id_status_para_t *data_id_status_para_ptr = NULL_DATA_PTR;
	char data_read_terminator_char;
	
	if(data_id >= NUM_DATA_IDS)
	{
		 system_status_flag = ERR_DATA_ID_EXCEED;
		  Error_or_Warning_Proc("02.09.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	data_id_status_para_ptr = data_id_status_para + data_id;
	// max_alloc_data_len  = req max allocated data len with appended a NULL_CHAR
	if(max_alloc_data_len > DATA_MAX_NUM_ALLOCATED_CHARS)
	{
				  data_id_status_para_ptr->max_allocated_data_len = 0;
				  system_status_flag = ERR_EXCEEDS_DATA_NUM_CHARS;
				   Error_or_Warning_Proc("02.09.02", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;
	}
			data_id_status_para_ptr->data_id = data_id;	        
		    data_id_status_para_ptr->max_allocated_data_len = max_alloc_data_len;
		if((Appl_Data_Retrieve_Para(data_id, DATA_RETRIEVE_CTRL_DATA_READ_TERMINATOR_CHAR, &data_read_terminator_char)) != SUCCESS)
		{
		   system_status_flag = ERR_APPL_DATA_RETRIEVE;
		   Error_or_Warning_Proc("02.09.03", ERROR_OCCURED, system_status_flag);
           return system_status_flag; 
		}
		if(data_read_terminator_char == NULL_CHAR)
	    {
		      data_id_status_para_ptr->data_read_status.data_read_mode = DATA_ID_READ_MODE_TILL_ALLOC;				 
	    }
	    else
    	{
		     data_id_status_para_ptr->data_read_status.data_read_mode = DATA_ID_READ_MODE_TILL_TERMINATOR;
	    }
		
		#ifdef DMA_DATA_ID_STR
		if(data_id_status_para_ptr->data_value.data_str != NULL_DATA_PTR)
	    {
           system_status_flag = ERR_ALREADY_MEM_ALLOC;
		    Error_or_Warning_Proc("02.09.04", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
	    }
        data_id_status_para_ptr->data_value.data_str = (char *) malloc(data_id_status_para_ptr->max_allocated_data_len * sizeof(char));	
        #endif	
		Reset_Data_ID_Status(data_id, DATA_ID_RESET_WHOLE_STATUS);      
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Power_Of

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : 

Func ID        : 02.10 

Bugs           :   
-*------------------------------------------------------------*/
uint32_t Power_Of(const uint8_t base, const uint8_t power )
{
    uint32_t power_val = 1;
    uint8_t i = 0;
  
    if(power == 0)
    {
       return power_val;
    }
    for(i = 1; i <= power; ++i)
    {
      power_val *= base;
    }
    return power_val;
}

#ifdef DELAY_BY_CLOCK
/*------------------------------------------------------------*
FUNCTION NAME  : Delay_MS

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : each for loop iteration takes 4 instruction cycles  

Func ID        : 02.11 

Bugs           : NOT_WORKING  
-*------------------------------------------------------------*/
void Delay_MS(const uint32_t max_elapsed_time_in_ms )
{
   uint32_t max_elapsed_inst_cycle = max_elapsed_time_in_ms * _XTAL_FREQ * (1000 / 4), elapsed_inst_cycle;
	
	for(elapsed_inst_cycle = 0; elapsed_inst_cycle < max_elapsed_inst_cycle; ++elapsed_inst_cycle)
	{
		   __asm("NOP");
	}
}

#else
/*------------------------------------------------------------*
FUNCTION NAME  : Delay_MS

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : for Cclk = 60MHz 

Func ID        : 02.11 

Bugs           : 
-*------------------------------------------------------------*/
void Delay_MS(const uint32_t max_elapsed_time_in_ms )
{
	uint16_t x, elasped_time_in_ms;
	
	for(elasped_time_in_ms = 0; elasped_time_in_ms < max_elapsed_time_in_ms; ++elasped_time_in_ms)
	{
        for(x = 0; x < 6000; x++);    /* loop to generate 1 millisecond delay with Cclk = 60MHz */
	}
}	
#endif

#ifdef DELAY_BY_CLOCK
/*------------------------------------------------------------*
FUNCTION NAME  : Delay_US

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           :  each for loop iteration takes 4 instruction cycles

Func ID        : 02.12 

Bugs           : NOT_WORKING  
-*------------------------------------------------------------*/
void Delay_US(const uint32_t max_elapsed_time_in_us )
{
   uint32_t max_elapsed_inst_cycle = max_elapsed_time_in_us * (_XTAL_FREQ / 4) , elapsed_inst_cycle;
	
	for(elapsed_inst_cycle = 0; elapsed_inst_cycle < max_elapsed_inst_cycle; ++elapsed_inst_cycle)
	{
		   __asm("NOP");
	}
}

#endif 

/*------------------------------------------------------------*-
FUNCTION NAME  : Write_Bit_in_Data

DESCRIPTION     : write bit valve to data's bit, without affecting other bits of data .
                . 0 is the least bit pos and 7 is the most bit pos 

INPUT          : 

OUTPUT         : none

NOTE           : 

Func ID        : 02.13 

Bugs           :   
-*------------------------------------------------------------*/
uint16_t Write_Bit_in_Data( uint32_t *const data_ptr, const uint8_t bit_pos, const uint8_t set_bit_val )
{
     if(data_ptr == NULL_DATA_PTR)
	 {
		 system_status_flag = ERR_NULL_PTR;
		  Error_or_Warning_Proc("02.13.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	 }
	 switch(set_bit_val)
     {
		 case STATE_LOW:
		    Clear_Bit_in_Data(data_ptr, bit_pos );
         break;
         case STATE_HIGH:		 
            Set_Bit_in_Data(data_ptr, bit_pos);
		 break;
         default:
           system_status_flag = ERR_FORMAT_INVALID;
		    Error_or_Warning_Proc("02.13.02", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
     }
      return SUCCESS; 
}	

/*------------------------------------------------------------*
FUNCTION NAME  : Error_or_Warning_Proc

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : process to do when a error or warning has occured is not implemented.

Func ID        : 02.14  

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Error_or_Warning_Proc(const char *const error_trace_str, const uint8_t warn_or_error_format, const uint32_t warning_or_error_code)
{
	internal_error_state = warn_or_error_format;
	switch(warn_or_error_format)
	{
		case WARNING_OCCURED: 
		case ERROR_OCCURED:	
		case FATAL_OCCURED:
		    Error_or_Warning_Occured_Proc(warn_or_error_format, warning_or_error_code);		   
		break;
        default:
           system_status_flag = ERR_FORMAT_INVALID;		
		   return system_status_flag;	    
	}
	if((Comm_Error_Proc(error_trace_str, warn_or_error_format, COMM_ERROR_BY_LED_UART_AND_OR_LCD, warning_or_error_code ))!= SUCCESS)
	{
		system_status_flag = ERR_PROC_ERROR;		
		return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Comm_Error_Proc

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           :

Func ID        : 02.15 

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Comm_Error_Proc(const char *const error_trace_str, const uint8_t warn_or_error_format, const uint8_t error_comm_by_dev, const uint32_t warning_or_error_code)
{
  #ifdef LCD_MOD_ENABLE 
    lcd_status_t *cur_lcd_status_ptr;
	uint8_t error_lcd_ch_id = TRACE_LCD_CH_ID, start_col_num;  
  #endif
	const char warning_msg_disp[] = "WRN: ", fatal_msg_disp[] = "FTL: ", error_msg_disp[] = "ERR: "; 
	disp_num_fmt_t disp_error_code;
	uint8_t error_uart_ch_id = TRACE_UART_CH_ID, error_comm_dev_flag = 0;
	
	if(internal_error_led_io_ch < IO_CH_INVALID)
	{
	   if((IO_Channel_Write(internal_error_led_io_ch, STATE_HIGH))!= SUCCESS)
	   {
		   system_status_flag = ERR_IO_CH_WRITE;		 
		   return system_status_flag;
	   }
	}
	switch(error_comm_by_dev)
	{
		case COMM_ERROR_BY_LED:
		  return SUCCESS;
		//break;		
		case COMM_ERROR_BY_LED_AND_OR_LCD:
		  #if !defined (LCD_MOD_ENABLE)
		     return SUCCESS;
		  #endif
		  error_comm_dev_flag |= (1 << 0); 
		break;
		case COMM_ERROR_BY_LED_AND_OR_UART:
		   #if !defined (UART_MOD_ENABLE)
              return SUCCESS;
		   #endif
		   error_comm_dev_flag |= (1 << 1);
		break;
		case COMM_ERROR_BY_LED_UART_AND_OR_LCD:
		  #if defined (LCD_MOD_ENABLE)
		     error_comm_dev_flag |= (1 << 0); 
		  #endif
		  #if defined (UART_MOD_ENABLE)
             error_comm_dev_flag |= (1 << 1);	
          #endif
          if(error_comm_dev_flag == 0)
		  {
			  return SUCCESS;
		  }			  
		break;
		default:
		  system_status_flag = ERR_FORMAT_INVALID;		 
		  return system_status_flag;
	}
	#ifdef LCD_MOD_ENABLE
	   cur_lcd_status_ptr = lcd_status + error_lcd_ch_id;
	#endif
	#ifdef UART_MOD_ENABLE
	   if((error_comm_dev_flag & (1 << 1)))
	   {
	      if((UART_Transmit_Char(error_uart_ch_id, ENTER_CHAR)) != SUCCESS)
	      {
		     system_status_flag = ERR_UART_TX_PROC;		 
		     return system_status_flag;
	      }
	   }
	#endif  
	switch(warn_or_error_format)
	{
		case WARNING_OCCURED:	
          #ifdef TRACE_ERROR  
	         #ifdef LCD_MOD_ENABLE
			 	 if((error_comm_dev_flag & (1 << 0)))
	             {
		            if((LCD_Disp_Goto_XY(error_lcd_ch_id, ERROR_LINE_NUM, COL_NUM_01)) != SUCCESS)
				    {
					   system_status_flag = ERR_LCD_GOTO_XY_PROC;		 
		               return system_status_flag;
				    }					
	                if((LCD_Disp_Str(error_lcd_ch_id, warning_msg_disp)) != SUCCESS)
				    {
					   system_status_flag = ERR_LCD_WRITE_DATA_PROC;		 
		               return system_status_flag;
				    }
			        cur_lcd_status_ptr->cur_line_disp_data[ERROR_LINE_NUM] = WARNING_DISP;
				 }
            #endif			
			#ifdef UART_MOD_ENABLE
			    if((error_comm_dev_flag & (1 << 1)))
	            { 
			        if((UART_Transmit_Str(error_uart_ch_id, warning_msg_disp)) != SUCCESS)
				    {
				    	system_status_flag = ERR_UART_TX_PROC;		 
		                return system_status_flag;
				    }
				}					
			#endif
		#endif		 
        break;
		case ERROR_OCCURED:
		   #ifdef TRACE_ERROR  
	          #ifdef LCD_MOD_ENABLE
			  if((error_comm_dev_flag & (1 << 0)))
			  {
		          if((LCD_Disp_Goto_XY(error_lcd_ch_id, ERROR_LINE_NUM, COL_NUM_01)) != SUCCESS)
			      {
				     system_status_flag = ERR_LCD_GOTO_XY_PROC;		 
		             return system_status_flag;
			      }				   
	              if((LCD_Disp_Str(error_lcd_ch_id, error_msg_disp)) != SUCCESS)
			      {
				     system_status_flag = ERR_LCD_WRITE_DATA_PROC;		 
		             return system_status_flag;
			      }
			      cur_lcd_status_ptr->cur_line_disp_data[ERROR_LINE_NUM] = ERROR_DISP;
			  }
			#endif          
            #ifdef UART_MOD_ENABLE
			  if((error_comm_dev_flag & (1 << 1)))
	          { 
                  if((UART_Transmit_Str(error_uart_ch_id, error_msg_disp)) != SUCCESS)
			      {
				     system_status_flag = ERR_UART_TX_PROC;		 
		             return system_status_flag; 
			      }
			  }			   
            #endif			   
		  #endif		 
		break;
        case FATAL_OCCURED:	            
		   #ifdef TRACE_ERROR 
	        #ifdef LCD_MOD_ENABLE
			   if((error_comm_dev_flag & (1 << 0)))
			   {
		          if((LCD_Disp_Goto_XY(error_lcd_ch_id, ERROR_LINE_NUM, COL_NUM_01)) != SUCCESS)
			      {
				     system_status_flag = ERR_LCD_GOTO_XY_PROC;		 
		             return system_status_flag;
			      }				   
	              if((LCD_Disp_Str(error_lcd_ch_id, fatal_msg_disp)) != SUCCESS)
			      {
				     system_status_flag = ERR_LCD_WRITE_DATA_PROC;		 
		             return system_status_flag;
			      }
			      cur_lcd_status_ptr->cur_line_disp_data[ERROR_LINE_NUM] = FATAL_DISP;
			   }
			#endif			
			#ifdef UART_MOD_ENABLE
			   if((error_comm_dev_flag & (1 << 1)))
	           {
                  if((UART_Transmit_Str(error_uart_ch_id, fatal_msg_disp)) != SUCCESS)
			      {
				     system_status_flag = ERR_UART_TX_PROC;		 
		             return system_status_flag; 
			      }
			   }				 
            #endif
		 #endif	  
		break;
        default:
		/* warning invalid error or warning format*/	
          system_status_flag = ERR_FORMAT_INVALID;		  
		  return system_status_flag;         		    
	}
	 #ifdef TRACE_ERROR
	 	 disp_error_code.sign_flag = STATE_NO;
		 disp_error_code.least_digits_flag = STATE_YES;
		 disp_error_code.left_format_flag = STATE_YES;
		 disp_error_code.num_digits = 4;
		 #ifdef LCD_MOD_ENABLE
            if((error_comm_dev_flag & (1 << 0)))
	        {		 
		        start_col_num = Str_Len(error_msg_disp) + 1;
		 	    if((LCD_Disp_Goto_XY(error_lcd_ch_id, ERROR_LINE_NUM, start_col_num )) != SUCCESS)  
                {
                   system_status_flag = ERR_LCD_GOTO_XY_PROC;		 
		           return system_status_flag;
                } 
		        if((LCD_Disp_Str(error_lcd_ch_id, error_trace_str)) != SUCCESS)
			    {
			    	system_status_flag = ERR_LCD_WRITE_DATA_PROC;		 
		            return system_status_flag;
			    }
			    start_col_num += Str_Len(error_trace_str);
		        if((LCD_Disp_Goto_XY(error_lcd_ch_id, ERROR_LINE_NUM, start_col_num)) != SUCCESS)
                {
                  system_status_flag = ERR_LCD_GOTO_XY_PROC;		 
		          return system_status_flag;
                }
		        if((LCD_Disp_Char(error_lcd_ch_id, ' ')) != SUCCESS)
			    {
				   system_status_flag = ERR_LCD_WRITE_DATA_PROC;		 
		           return system_status_flag;
			    }
			    start_col_num += 1;
		        if((LCD_Disp_Goto_XY(error_lcd_ch_id, ERROR_LINE_NUM, start_col_num)) != SUCCESS)
			    {
			    	system_status_flag = ERR_LCD_GOTO_XY_PROC;		 
		            return system_status_flag;
			    }				
		        if((LCD_Disp_Num(error_lcd_ch_id, DATA_TYPE_IN_DECIMAL, disp_error_code, warning_or_error_code)) != SUCCESS)
			    {
			    	system_status_flag = ERR_LCD_WRITE_DATA_PROC;		 
		            return system_status_flag;
			    }
			}
          #endif		
		  #ifdef UART_MOD_ENABLE
		   if((error_comm_dev_flag & (1 << 1)))
	       {
		       if((UART_Transmit_Str(error_uart_ch_id, error_trace_str)) != SUCCESS)
			   {
				  system_status_flag = ERR_UART_TX_PROC;		 
		          return system_status_flag;
			   }				
		       if((UART_Transmit_Char(error_uart_ch_id, ' ')) != SUCCESS)
			   {
			    	system_status_flag = ERR_UART_TX_PROC;		 
		            return system_status_flag;
			   }
		       if((UART_Transmit_Num(error_uart_ch_id, DATA_TYPE_IN_DECIMAL, disp_error_code, warning_or_error_code)) != SUCCESS)
			   {
				  system_status_flag = ERR_UART_TX_PROC;		 
		          return system_status_flag;
			   }
			   if((UART_Transmit_Char(error_uart_ch_id, ENTER_CHAR)) != SUCCESS)
			   {
			    	system_status_flag = ERR_UART_TX_PROC;		 
		           return system_status_flag; 
			   }
		   }			   
		 #endif  		 	
             		
	#endif
	return SUCCESS;
}
	
/*------------------------------------------------------------*
FUNCTION NAME  : Appl_Data_Retrieve_Para

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           :

Func ID        : 02.16 

BUGS           :              
-*------------------------------------------------------------*/
uint16_t Appl_Data_Retrieve_Para(const uint8_t data_id, const uint8_t retrieve_data_form, void *const retrive_data_ptr)
{
	const data_id_ctrl_para_t *data_id_ctrl_para_ptr;
	const data_id_status_para_t *data_id_status_para_ptr;
	int32_t *retrieve_data_int32_ptr;
	uint8_t *retrive_data_uint8_ptr;
	char *retrive_data_char_ptr;
	
	if(retrive_data_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.16.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(data_id >= NUM_DATA_IDS)
	{
		system_status_flag = ERR_DATA_ID_EXCEED;
		Error_or_Warning_Proc("02.16.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	data_id_ctrl_para_ptr = data_id_ctrl_para + data_id;
	data_id_status_para_ptr = data_id_status_para + data_id;
	switch(retrieve_data_form)
	{
		case DATA_RETRIEVE_CTRL_MAX_REQ_DATA_CHARS:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->max_req_data_len;
		break;
        case DATA_RETRIEVE_CTRL_MAX_NUM_DATA_READ_TRY:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->max_num_data_read_try;
        break;
		case DATA_RETRIEVE_CTRL_DATA_DISP_FORMAT:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_output_disp_format;
		break;
		case DATA_RETRIEVE_CTRL_OUTPUT_DATA_TYPE:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->output_data_type;
		break;
		case DATA_RETRIEVE_CTRL_INPUT_DATA_TYPE:
		    retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->input_data_type;
		break;
		case DATA_RETRIEVE_CTRL_INPUT_DATA_STREAM:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_input_stream_format;
		break;
		case DATA_RETRIEVE_CTRL_OUTPUT_DATA_STREAM:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_output_stream_format;
		break;
        case DATA_RETRIEVE_CTRL_TRANSMIT_DATA_STREAM:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_tx_stream_format;
		break;	
		case DATA_RETRIEVE_CTRL_RECEIVE_DATA_STREAM:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_rcv_stream_format;
		break;
		case DATA_RETRIEVE_CTRL_READ_NUM_CHAR_ENABLE_FLAG:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_read_num_char_enable_flag; 
		break;
		case DATA_RETRIEVE_CTRL_READ_SMALL_ENGLISH_ALPHA_CHAR_ENABLE_FLAG:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_read_small_english_alpha_char_enable_flag; 
		break;
		case DATA_RETRIEVE_CTRL_READ_BIG_ENGLISH_ALPHA_CHAR_ENABLE_FLAG:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_read_big_english_alpha_char_enable_flag; 
		break;
		case DATA_RETRIEVE_CTRL_READ_SPECIAL_CHAR_ENABLE_FLAG:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_read_special_char_enable_flag; 
		break;
		case DATA_RETRIEVE_CTRL_READ_CTRL_CHAR_ENABLE_FLAG:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_read_ctrl_char_enable_flag; 
		break;
		case DATA_RETRIEVE_CTRL_DATA_READ_TERMINATOR_CHAR:
		    retrive_data_char_ptr = (char *) retrive_data_ptr;
		   *retrive_data_char_ptr = data_id_ctrl_para_ptr->data_read_terminator_char; 
		break;
		case DATA_RETRIEVE_CTRL_TRANSMIT_SERVICE:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_tx_event; 
		break;
		case DATA_RETRIEVE_CTRL_RECEIVE_SERVICE:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_ctrl_para_ptr->data_rcv_event; 
		break;
		case DATA_RETRIEVE_STATUS_NUM_CHARS_READ:
		   retrive_data_uint8_ptr = (uint8_t *) retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_status_para_ptr->data_value.data_str_len_or_pos;		   
		break;
		case DATA_RETRIEVE_STATUS_DATA_STR:
		  retrive_data_char_ptr = (char *)retrive_data_ptr;
		  memcpy(retrive_data_char_ptr, data_id_status_para_ptr->data_value.data_str, data_id_status_para_ptr->data_value.data_str_len_or_pos); 
		  retrive_data_char_ptr[data_id_status_para_ptr->data_value.data_str_len_or_pos] = NULL_CHAR;
		break;
		case DATA_RETRIEVE_STATUS_DATA_CHAR:
		  retrive_data_char_ptr = (char *)retrive_data_ptr;
		  *retrive_data_char_ptr = data_id_status_para_ptr->data_value.data_str[data_id_status_para_ptr->data_value.data_str_len_or_pos];
		break;
		case DATA_RETRIEVE_STATUS_DATA_INT32:
		  retrieve_data_int32_ptr = (int32_t *)retrive_data_ptr;
		   *retrieve_data_int32_ptr = data_id_status_para_ptr->data_value.data_num.val_int_32[0];
		break;
		case DATA_RETRIEVE_STATUS_DATA_UINT8:
		   retrive_data_uint8_ptr = (uint8_t *)retrive_data_ptr;
		   *retrive_data_uint8_ptr = data_id_status_para_ptr->data_value.data_num.val_uint_8[0];
		break;
        default:
           system_status_flag = ERR_FORMAT_INVALID;
		   Error_or_Warning_Proc("02.16.03", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;	
	}
	return SUCCESS;
}	

/*------------------------------------------------------------*
FUNCTION NAME  : Next_Data_Conf_Parameter

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.17

Bugs           :   
-*------------------------------------------------------------*/
uint16_t Next_Data_Conf_Parameter(const uint8_t set_cur_data_id)
{
  static uint8_t prev_data_id = DATA_ID_INVALID;	
  
  if(!(set_cur_data_id == DATA_ID_INVALID || set_cur_data_id <= NUM_DATA_IDS))
  {
      system_status_flag = ERR_CUR_DATA_ID_INVALID;
	  Error_or_Warning_Proc("02.17.01", ERROR_OCCURED, system_status_flag);
	  return system_status_flag;
  }  
   if(cur_data_id == set_cur_data_id) 
   {
	   return SUCCESS;
   }	   
	 prev_data_id = cur_data_id;			  
	 cur_data_id = set_cur_data_id;          
	 if((Data_Dev_Src_Access_Oper_Func(prev_data_id)) != SUCCESS)
	 {
		 system_status_flag = ERR_NEXT_DATA_CONF;
		 Error_or_Warning_Proc("02.17.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	 }		 
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Set_Status_Reset_Data_IDs

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.18

BUGS           :
-*------------------------------------------------------------*/
uint16_t Set_Status_Reset_Data_IDs(const uint8_t set_data_status_reset_type)
{
	 uint8_t data_id;
	
	 for(data_id = 0; data_id < NUM_DATA_IDS; ++data_id)
	 {
		  if((Reset_Data_ID_Status(data_id, set_data_status_reset_type)) != SUCCESS)
		  {
			  system_status_flag = ERR_RESET_DATA_ID_STATUS;
		      Error_or_Warning_Proc("02.18.01", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;
		  }			  
	 }	 
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Num_To_Str_Conv_Till_Conf_Size

DESCRIPTION    : Convert integer number into octal, hex, etc.
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.19 

Bugs           :  
-*------------------------------------------------------------*/
char *Num_To_Str_Conv_Till_Conf_Size(const int32_t num_to_convert, const uint8_t base, const uint8_t alpha_flag, const uint8_t conv_till_conf_size ) 
{ 
    char start_alpha_char; 
	static char buffer[NUM_CONVERT_BUFFER_LEN];
	char *num_in_str_ptr;  
	int32_t cur_num_to_convert = num_to_convert;
	int32_t num_to_convert_num_digits; 
	uint8_t char_index, conv_cur_size = 0, conv_till_size;	
	
	if(base > BASE_16)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
	    Error_or_Warning_Proc("02.19.01", ERROR_OCCURED, system_status_flag);
		return NULL_DATA_PTR;
	}
	if(base <= BASE_10)
	{
		if(alpha_flag != NUM_CONV_ALPHA_NA)
		{
			system_status_flag = ERR_FORMAT_INVALID;
	        Error_or_Warning_Proc("02.19.02", ERROR_OCCURED, system_status_flag);
			return NULL_DATA_PTR;
		}
	}
	else
	{
	    switch(alpha_flag)
	    {
		   case NUM_CONV_BIG_ALPHA:
		     start_alpha_char = 'A';
		   break;  
		   case NUM_CONV_SMALL_ALPHA:
		     start_alpha_char = 'a';
		   break;
		   default:
		      system_status_flag = ERR_FORMAT_INVALID;
	          Error_or_Warning_Proc("02.19.03", ERROR_OCCURED, system_status_flag);
		      return NULL_DATA_PTR;
		}
	}
	memset(buffer, NULL_CHAR, NUM_CONVERT_BUFFER_LEN);
    num_in_str_ptr = &buffer[NUM_CONVERT_BUFFER_LEN - 1];
	*num_in_str_ptr = NULL_CHAR;	
	if(conv_till_conf_size == 0)
	{
		if(cur_num_to_convert < 0)
		{
			cur_num_to_convert = -cur_num_to_convert;
		}
	   	do 
    	{ 
	        char_index = cur_num_to_convert % base;
	    	if(char_index >= 0 && char_index <= 9)
		    {
		    	*--num_in_str_ptr = NUM_0_CHAR + char_index; 
	    	}
	    	else
	    	{			
		        *--num_in_str_ptr = start_alpha_char + (char_index - 10); 
		    }
	    	cur_num_to_convert /= base; 
	     }	 
    	while(cur_num_to_convert != 0); 
    	if(num_to_convert  < 0)
    	{
    		*--num_in_str_ptr = '-';
		}
	}
	else
	{
	 	if(conv_till_conf_size >= NUM_CONVERT_BUFFER_LEN)
	    {
			system_status_flag = ERR_DATA_OUT_OF_RANGE;
	        Error_or_Warning_Proc("02.19.04", ERROR_OCCURED, system_status_flag);
	    	return NULL_DATA_PTR;
	    }
		if(cur_num_to_convert < 0)
		{
			cur_num_to_convert = -cur_num_to_convert;
			//adjust 
			conv_cur_size = 1;
		}
	    if((Get_Num_Digits_Based_Data(cur_num_to_convert, &num_to_convert_num_digits, base, GET_NUM_DIGITS )) != SUCCESS)
	    {
			system_status_flag = ERR_GET_NUM_DIGITS_DATA_PROC;
	        Error_or_Warning_Proc("02.19.05", ERROR_OCCURED, system_status_flag);
	     	return NULL_DATA_PTR;
		}
		conv_till_size = (num_to_convert_num_digits > conv_till_conf_size - conv_cur_size) ? num_to_convert_num_digits: (conv_till_conf_size - conv_cur_size) ;		
		for(conv_cur_size = 0; conv_cur_size < conv_till_size; ++conv_cur_size) 
		{
		      char_index = cur_num_to_convert % base;
	          if(char_index >= 0 && char_index <= 9)
		      {
		      	   *--num_in_str_ptr = NUM_0_CHAR + char_index; 
	          }
	    	  else
	          {			
		           *--num_in_str_ptr = start_alpha_char + (char_index - 10); 
		      }
	    	  cur_num_to_convert /= base; 
	    }
		if(num_to_convert  < 0)
    	{
    		*--num_in_str_ptr = '-';
		}	 
    	   
	}
	return(num_in_str_ptr); 
}

/*------------------------------------------------------------*
FUNCTION NAME  : Init_File

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.20 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Init_File(file_t *const file_ptr, const uint8_t file_desp, const uint8_t device_type, const uint8_t stream_type, const uint8_t stream_io_type)
{
	if(file_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.20.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag; 
	}
	if(device_type >= DEVICE_DATA_NA)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.20.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;		
	}
	if(stream_type >= STREAM_TYPE_NA)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.20.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag; 
	}
	if(stream_io_type >= STREAM_IO_NA)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.20.04", ERROR_OCCURED, system_status_flag);
		return system_status_flag; 
	}
	if((File_Flush(file_ptr)) != SUCCESS)
	{
		system_status_flag = ERR_FILE_FLUSH_PROC;
		Error_or_Warning_Proc("02.20.05", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	file_ptr->device_type = device_type;
	file_ptr->stream_type = stream_type;
	file_ptr->stream_io_type = stream_io_type;
	file_ptr->file_desp = file_desp;
	return SUCCESS;		
}

/*------------------------------------------------------------*
FUNCTION NAME  : File_Flush

DESCRIPTION    : File_Flush() operation is similar to fflush()
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.21 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t File_Flush(file_t *const file_ptr)
{
	if(file_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.21.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	memset(file_ptr->stream_buf, NULL_CHAR, MAX_STREAM_BUFFER_SIZE + 1);
	file_ptr->num_chars_stream_buf = 0;
	file_ptr->stream_buf_cur_pos = 0;
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : strtoq

DESCRIPTION    : converts the string pointed to by nptr to a 64-bit, long-long integer representation. 
        This function recognizes (in order) an optional string of spaces, an optional sign, an optional base indicator (0 for octal, X or x for hexadecimal),
        and a string of digits. The first unrecognized character ends the string. A pointer to this unrecognized character is stored in the object addressed by endptr, 
       if endptr is not NULL.

       If base is non-zero, its value determines the set of recognized digits and overrides the optional base indicator character. 
        If base is zero, nptr is assumed to be base 10, unless an optional base indicator character is given
								
INPUT          : nptr - Points to a character string for strtoq() to convert.
                 endptr - Is a result parameter that, if not NULL, returns a string beginning with the first character that strtoq() does not attempt to convert. 
                 base  - Is the base of the string, a value between 0 and 36.
				 
OUTPUT         : returns the converted value, if there is any. If no conversion was performed, strtoq() returns a zero. 
                 If the converted value overflows, strtoq() returns QUAD_MAX or QUAD_MIN (according to the sign of the value) 

NOTE           : 

Func ID        : 02.22 

Bugs           :  
-*------------------------------------------------------------*/
quad_t strtoq(const char *const nptr, char **endptr, uint8_t base)
{
	const char *s;
	u_quad_t acc;
	unsigned char c;
	u_quad_t qbase, cutoff;
	int neg, any, cutlim;

	if(nptr == NULL_DATA_PTR) 
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.22.01", ERROR_OCCURED, system_status_flag);
		return 0;
	}
	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	s = nptr;
	do 
	{
		c = *s++;
	} while (isspace(c));
	if (c == '-')
	{
		neg = 1;
		c = *s++;
	} 
	else
	{
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == BASE_16) &&
	    c == '0' && (*s == 'x' || *s == 'X'))
	{
		c = s[1];
		s += 2;
		base = BASE_16;
	}
	if (base == 0)
		base = c == '0' ? BASE_08 : BASE_10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for quads is
	 * [-9223372036854775808..9223372036854775807] and the input base
	 * is 10, cutoff will be set to 922337203685477580 and cutlim to
	 * either 7 (neg==0) or 8 (neg==1), meaning that if we have
	 * accumulated a value > 922337203685477580, or equal but the
	 * next digit is > 7 (or 8), the number is too big, and we will
	 * return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	qbase = (unsigned)base;
	cutoff = neg ? (u_quad_t)-(QUAD_MIN + QUAD_MAX) + QUAD_MAX : QUAD_MAX;
	cutlim = cutoff % qbase;
	cutoff /= qbase;
	for (acc = 0, any = 0;; c = *s++)
	{
		if ((isascii(c)) != STATE_YES)
			break;
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
		{
			any = -1;
		}
		else
		{
			any = 1;
			acc *= qbase;
			acc += c;
		}
	}
	if (any < 0)
	{
		acc = neg ? QUAD_MIN : QUAD_MAX;
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
	    Error_or_Warning_Proc("02.22.02", WARNING_OCCURED, system_status_flag);
	} 
	else if (neg)
		acc = -acc;
	if (endptr != 0)
		*((const char **)endptr) = any ? s - 1 : nptr;
	return (acc);
}


 /*------------------------------------------------------------*
FUNCTION NAME  : strtoul

DESCRIPTION    :  converts the string pointed to by nptr to a 64-bit, unsigned-long integer representation. 
        This function recognizes (in order) an optional string of spaces, an optional sign, an optional base indicator (0 for octal, X or x for hexadecimal),
        and a string of digits. The first unrecognized character ends the string. A pointer to this unrecognized character is stored in the object addressed by endptr, 
       if endptr is not NULL.

       If base is non-zero, its value determines the set of recognized digits and overrides the optional base indicator character. 
        If base is zero, nptr is assumed to be base 10, unless an optional base indicator character is given
								
INPUT          : nptr - points to a sequence of characters that can be interpreted as a numeric value of type unsigned long int.
                 endptr - Is a result parameter that, if not NULL, returns a string beginning with the first character that strtoul() does not attempt to convert. 
                 base  - Is the base of the string, a value between 0 and 36.
				 
OUTPUT         : returns the converted value, if there is any. If no conversion was performed, strtoul() returns a zero. 
                 If the converted value overflows, strtoul() returns ULONG_MAX. 

NOTE           : 

Func ID        : 02.23 

Bugs           :  
-*------------------------------------------------------------*/
unsigned long strtoul(const char *const nptr, char **endptr, uint8_t base)
{
	const char *s;
	unsigned long acc, cutoff;
	int c;
	int neg, any, cutlim;
	
	if(nptr == NULL_DATA_PTR) 
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.23.01", ERROR_OCCURED, system_status_flag);
		return 0;
	}
	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	s = nptr;
	do {
		c = (unsigned char) *s++;
	} while (isspace(c));
	if (c == '-')
	{
		neg = 1;
		c = *s++;
	}
	else
	{
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == BASE_16) &&
	    c == '0' && (*s == 'x' || *s == 'X'))
	{
		c = s[1];
		s += 2;
		base = BASE_16;
	}
	if (base == 0)
		base = c == '0' ? BASE_08: BASE_10;
	cutoff = ULONG_MAX / (unsigned long)base;
	cutlim = ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = (unsigned char) *s++)
	{
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0)
			continue;
		if (acc > cutoff || (acc == cutoff && c > cutlim))
		{
			any = -1;
			acc = ULONG_MAX;
			system_status_flag = ERR_DATA_OUT_OF_RANGE;
	    	Error_or_Warning_Proc("02.23.02", WARNING_OCCURED, system_status_flag);
		} 
		else
		{
			any = 1;
			acc *= (unsigned long)base;
			acc += c;
		}
	}
	if (neg && any > 0)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

/*------------------------------------------------------------*
FUNCTION NAME  : Num_To_Str_Conv_No_Conf_Size

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : Num_To_Str_Conv_No_Conf_Size() function is simliar to itoa()

Func ID        : 02.24

Bugs           :   
-*------------------------------------------------------------*/
char* Num_To_Str_Conv_No_Conf_Size(const int32_t num_to_str, char *const num_in_str, const uint8_t base) 
{ 
    int32_t num = num_to_str; 
    uint8_t is_num_negative_flag = STATE_NO, num_in_str_len = 0; 
    int8_t reverse_start_index = 0, reverse_end_index;
	char temp;
	
    if(num_in_str == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.24.01", ERROR_OCCURED, system_status_flag);
		return NULL_DATA_PTR;
	}
    if(base > BASE_16)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.24.02", ERROR_OCCURED, system_status_flag);
		return NULL_DATA_PTR;
	}
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        num_in_str[num_in_str_len++] = '0'; 
        num_in_str[num_in_str_len] = NULL_CHAR; 
        return num_in_str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with base 10. 
    if (num < 0)  
    { 
        if( base != BASE_10)
        {
			system_status_flag = ERR_FORMAT_INVALID;
		    Error_or_Warning_Proc("02.24.03", ERROR_OCCURED, system_status_flag);
        	return NULL_DATA_PTR;	
        }
        is_num_negative_flag = STATE_YES; 
        num = -num; 
    } 
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        num_in_str[num_in_str_len++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0'; 
        num = num/base; 
    }  
    // If number is negative, append '-' 
    if (is_num_negative_flag == STATE_YES) 
	{
        num_in_str[num_in_str_len++] = '-'; 
	}  
    num_in_str[num_in_str_len] = NULL_CHAR; // Append string terminator   
    // Reverse the string 
	reverse_start_index = 0; 
    reverse_end_index = num_in_str_len - 1;
    	
    while (reverse_start_index < reverse_end_index) 
    { 
        //swap 
        temp = *(num_in_str + reverse_start_index );
		*(num_in_str + reverse_start_index ) = *(num_in_str + reverse_end_index );
		*(num_in_str + reverse_end_index ) = temp;
        ++reverse_start_index;
        --reverse_end_index; 
    } 	
    return num_in_str; 
} 

/*------------------------------------------------------------*
FUNCTION NAME  : Reverse_Str_Use_Diff_Str

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : reversed_str must be memory allocated with size >= str_len_to_reverse

Func ID        : 02.25 

Bugs           :   
-*------------------------------------------------------------*/
uint16_t Reverse_Str_Use_Diff_Str(const char *const str_to_reverse_ptr, char *const reversed_str_ptr, const uint8_t conf_str_to_reverse_start_pos, const uint8_t conf_str_to_reverse_len)
{
	uint8_t str_to_reverse_start_index, str_to_reverse_end_index, str_len_to_reverse, str_len_for_reverse;
	int8_t cur_len = 0; 
	
	if(str_to_reverse_ptr == NULL_DATA_PTR || reversed_str_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.25.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if((str_len_for_reverse = Str_Len(str_to_reverse_ptr)) == 0)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.25.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;	    
	}
	if(conf_str_to_reverse_len != 0)
	{
	   if(conf_str_to_reverse_len > str_len_for_reverse)
       {
		   system_status_flag = ERR_DATA_OUT_OF_RANGE;
		   Error_or_Warning_Proc("02.25.03", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;	
       }	    
	   str_len_to_reverse = conf_str_to_reverse_len;
	}
	else
	{
    	str_len_to_reverse = str_len_for_reverse - conf_str_to_reverse_start_pos;
	}
    str_to_reverse_start_index = conf_str_to_reverse_start_pos;
	str_to_reverse_end_index = str_to_reverse_start_index + str_len_to_reverse - 1;	 	  
	if(str_to_reverse_start_index >= str_len_for_reverse || str_to_reverse_end_index >= str_len_for_reverse)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.25.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}	
	memset(reversed_str_ptr, NULL_CHAR, str_len_to_reverse);	
	for(cur_len = 0 ; cur_len < str_len_to_reverse; ++cur_len)
	{
    	reversed_str_ptr[cur_len] = str_to_reverse_ptr[str_to_reverse_end_index - cur_len];
	}
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Get_Num_Digits_Based_Data

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.26  

BUGS           :  
-*------------------------------------------------------------*/
uint16_t Get_Num_Digits_Based_Data(const uint32_t num, int32_t *const num_digits_data_ptr, const uint8_t base, const uint8_t get_num_digits_type)
{
	uint32_t power_base_cur_digit;
    int32_t max_num_digits, cur_num_digits; 
	
	*num_digits_data_ptr = 0;
	if(num_digits_data_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.26.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;  
	}
	if(base > BASE_16)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.26.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;  
	}
	switch(get_num_digits_type)
	{
		case GET_NUM_DIGITS:
		case GET_POWER_OF_CUR_NUM_DIGITS:
		case GET_POWER_OF_NEXT_NUM_DIGITS:
		break;
		default:
		  system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("02.26.03", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;  
	}
	max_num_digits = sizeof(uint32_t) * 8;
	switch(base)
	{
		case BASE_08:
		   max_num_digits = ((sizeof(uint32_t) * 8) / 3) + 1;
		break;
        case BASE_16:
            max_num_digits = ((sizeof(uint32_t) * 8) / 4);
		break;
        case BASE_10: 
           max_num_digits = (sizeof(uint32_t) * 3);
        break; 
	}
	for(cur_num_digits = max_num_digits; cur_num_digits >= 0; --cur_num_digits )
	{
		power_base_cur_digit = Power_Of(base, (uint8_t)cur_num_digits);
		if((num / power_base_cur_digit) != 0)
		{
			switch(get_num_digits_type)
			{
				case GET_NUM_DIGITS:
				  *num_digits_data_ptr = cur_num_digits + 1; 
				break;
                case GET_POWER_OF_CUR_NUM_DIGITS:
				  *num_digits_data_ptr = power_base_cur_digit;
                break;	
                case GET_POWER_OF_NEXT_NUM_DIGITS:
				    *num_digits_data_ptr = power_base_cur_digit * base;
                break;                               				
			}
			return SUCCESS;
		}
	}
    system_status_flag = ERR_DATA_OUT_OF_RANGE;
	Error_or_Warning_Proc("02.26.04", ERROR_OCCURED, system_status_flag);
	return system_status_flag;  
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reverse_Str_Use_Same_Str

DESCRIPTION    : reverses a string 'str' of length 'len'
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.27 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reverse_Str_Use_Same_Str(char *const str, const uint8_t len) 
{ 
    uint8_t i, j;
    char temp;
 	
	if(str == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.27.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;  
	}
	i = Str_Len(str);
	if(i == 0)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.27.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag; 
	}
	if(len > i)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.27.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag; 
	}
	i = 0;
	j = len - 1;
    while (i < j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        ++i; 
		--j; 
    } 
	return SUCCESS;
} 	

/*------------------------------------------------------------*
FUNCTION NAME  : isascii

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.28 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t isascii(const unsigned char test_char)
{
	if(test_char > ASCII_MAX_CHAR_CODE)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("02.28.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(test_char >= NUM_0_CHAR && test_char <= NUM_9_CHAR)
	{
		return STATE_YES;
	}
	if(test_char >= ENGLISH_SMALL_ALPHA_a_CHAR && test_char <= ENGLISH_SMALL_ALPHA_z_CHAR)
	{
		return STATE_YES;
	}
	if(test_char >= ENGLISH_BIG_ALPHA_A_CHAR && test_char <= ENGLISH_BIG_ALPHA_Z_CHAR)
	{
		return STATE_YES;
	}
	if(test_char >= BEGIN_CTRL_CHARS_ASCII_CODE && test_char <= END_CTRL_CHARS_ASCII_CODE)
	{
		if(isspace(test_char))
		{
		   return STATE_YES;
		}
		else
		{
			return STATE_NO;
		}
	}
	return STATE_YES;
}
	
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
