/* ********************************************************************
FILE                   : seg7_lib.c

PURPOSE                : 
                     									 	 
AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            : 

NOTE                  : 
                       
CHANGE LOGS           : 

FILE ID               : 06 

*****************************************************************************/

#include "main.h"

#ifdef SEG7_MOD_ENABLE

/* ------------------------------ macro defination ------------------------------ */

/* ----------------------------- global variable defination --------------------- */

/* ----------------------------- global variable declaration -------------------- */

/* ----------------------------- global function declaration -------------------- */

/* ----------------------------- const function pointer defination -------------- */


/*------------------------------------------------------------*-
FUNCTION NAME  : Seg7_Write

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.01 

BUGS           :
-*------------------------------------------------------------*/
uint16_t Seg7_Write(const uint8_t seg7_ch_id, const void *const seg7_rcv_disp_num_ptr)
{
    seg7_ctrl_t *cur_seg7_ctrl_ptr = NULL_DATA_PTR;
	data_write_ctrl_ptr_t *retrieve_data_write_ctrl_ptr;
	char retrieved_seg7_data_str[DATA_MAX_NUM_ALLOCATED_CHARS] ;
	const char *retrieved_seg7_data_ptr;
    int32_t seg7_num, cur_seg7_num;	
	uint32_t base_power_val;		
	consucc_bit_t consucc_bit_data;
    int16_t place_val, repeat, str_len;
	uint8_t cc_digit[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x3E ,0x5E, 0x40, 0x79 }, base;
    uint8_t place_digit[MAX_SEG7_DIGITS], i, output_data_type, mask_data ;
	
    if(seg7_rcv_disp_num_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("06.01.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
    if(seg7_ch_id >= NUM_OUTPUT_DEV_ID_SEG7_CHS)
	{
		   system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_Proc("06.01.02", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
	}	
	cur_seg7_ctrl_ptr = seg7_ctrl + seg7_ch_id;     
	if(  cur_seg7_ctrl_ptr->access_flag != STATE_YES) 
	   {  
          system_status_flag = WARN_CUR_DATA_ID_DEV_NO_ACCESS;
		  Error_or_Warning_Proc("06.01.03", ERROR_OCCURED, system_status_flag);
          return  system_status_flag;
	   }
		 
	   if(cur_seg7_ctrl_ptr->enable_flag != STATE_YES) 
	   {  
          system_status_flag = WARN_CUR_DATA_ID_DEV_DISABLED;
		  Error_or_Warning_Proc("06.01.04", ERROR_OCCURED, system_status_flag);
          return  system_status_flag;
	   }    
    
	if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_OUTPUT_DATA_TYPE, &output_data_type)) != SUCCESS)
	{
		system_status_flag = ERR_APPL_DATA_RETRIEVE;
		Error_or_Warning_Proc("06.01.05", ERROR_OCCURED, system_status_flag);
		return system_status_flag; 
	}	 
	 switch(output_data_type)
	 {
		case DATA_TYPE_IN_STR:
		   if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_STR, retrieved_seg7_data_str )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("06.01.06", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   }
		   if(retrieved_seg7_data_str[0] == NULL_CHAR)
		   {
			   seg7_num = 0;
		   }
		   else
		   {
		        if((Str_to_Num_Conv(&seg7_num, retrieved_seg7_data_str)) != SUCCESS)
		        {
		     	  system_status_flag = ERR_DATA_CONV_PROC;
			      Error_or_Warning_Proc("06.01.07", ERROR_OCCURED, system_status_flag);
                  return system_status_flag;
		        }
		   }
		  base = 10;
		break;
		case DATA_TYPE_IN_CHAR:
		   if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_CHAR, retrieved_seg7_data_str )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("06.01.08", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   }	
		  seg7_num = retrieved_seg7_data_str[0] - NUM_0_CHAR;
		  if(seg7_num < 0 || seg7_num > 9 || cur_seg7_ctrl_ptr->num_digits != 1)
		  {
			  system_status_flag = ERR_DATA_OUT_OF_RANGE;
			  Error_or_Warning_Proc("06.01.09", ERROR_OCCURED, system_status_flag);
              return system_status_flag; 
		  }
		   base = BASE_10;		   
		break;
		case DATA_TYPE_IN_BINARY:
		     if(cur_seg7_ctrl_ptr->num_digits <= 0 || cur_seg7_ctrl_ptr->num_digits > MAX_SEG7_DIGITS)
		     {
		         system_status_flag = ERR_DATA_OUT_OF_RANGE;
				 Error_or_Warning_Proc("06.01.10", ERROR_OCCURED, system_status_flag);
                 return system_status_flag;
		     }	 
      		base = BASE_02;
			if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_INT32, &seg7_num )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("06.01.11", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   } 
			 
        break;			
		case DATA_TYPE_IN_HEXA:
		    if(cur_seg7_ctrl_ptr->num_digits <= 0 || cur_seg7_ctrl_ptr->num_digits > MAX_SEG7_DIGITS)
		     {
		         system_status_flag = ERR_DATA_OUT_OF_RANGE;
				 Error_or_Warning_Proc("06.01.12", ERROR_OCCURED, system_status_flag);
                 return system_status_flag;
		     }
		    base = BASE_16;
			if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_INT32, &seg7_num )) != SUCCESS)
		    {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("06.01.13", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		    } 	 			
	    break;		
		case DATA_TYPE_IN_DECIMAL:
		    if(cur_seg7_ctrl_ptr->num_digits <= 0 || cur_seg7_ctrl_ptr->num_digits > MAX_SEG7_DIGITS)
		    {
		         system_status_flag = ERR_DATA_OUT_OF_RANGE;
				 Error_or_Warning_Proc("06.01.14", ERROR_OCCURED, system_status_flag);
                 return system_status_flag;
		    }  		  
		   if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_INT32, &seg7_num )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("06.01.15", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   }
           base = BASE_10;
		break;
        case  DATA_TYPE_IN_APPL_DEFINED:
		   retrieve_data_write_ctrl_ptr = (data_write_ctrl_ptr_t *) seg7_rcv_disp_num_ptr;
		   retrieved_seg7_data_ptr =  (const char *)((dev_write_ctrl_para_t *)retrieve_data_write_ctrl_ptr->dev_output_ctrl_para_ptr)->seg7_appl_defined_str;
		   str_len = Str_Len(retrieved_seg7_data_ptr);
		   if(str_len < 0 || str_len > cur_seg7_ctrl_ptr->num_digits)
		   {
			     system_status_flag = ERR_DATA_OUT_OF_RANGE;
				 Error_or_Warning_Proc("06.01.17", ERROR_OCCURED, system_status_flag);
                 return system_status_flag;
		   }
		   
        break;  		
		default:
		   system_status_flag = ERR_DATA_TYPE_INVALID;
		   Error_or_Warning_Proc("06.01.18", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
	}
	 
	for(i = 0; i < cur_seg7_ctrl_ptr->num_digits; ++i)
	{
		 if((IO_Channel_Write(cur_seg7_ctrl_ptr->io_ch_unit_digit + i, !(cur_seg7_ctrl_ptr->digit_place_active_state) )) != SUCCESS)
		 {						 
		      system_status_flag = ERR_SEG7_NO_WRITE;
			  Error_or_Warning_Proc("06.01.19", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;  
		 } 
    }
  consucc_bit_data.start_bit_pos = cur_seg7_ctrl_ptr->io_ch_a_led % NUM_PINS_PER_PORT;
  if(cur_seg7_ctrl_ptr->dot_disp_flag == STATE_YES )
  {
    consucc_bit_data.bits_len = 8; 
    mask_data = 0xFF;
  }
  else
  {
    consucc_bit_data.bits_len = 7; 
     mask_data = 0x7F;
  } 
 switch(output_data_type)
{
	case DATA_TYPE_IN_STR:
	case DATA_TYPE_IN_CHAR:
	case DATA_TYPE_IN_BINARY:
	case DATA_TYPE_IN_HEXA:
	case DATA_TYPE_IN_DECIMAL:	
       for(repeat = 0; repeat < REQ_CNT_SEG7_REPEAT_NUM; ++repeat)
       {
		   cur_seg7_num = seg7_num;
           base_power_val =  Power_Of(base, cur_seg7_ctrl_ptr->num_digits);
		   for(place_val = cur_seg7_ctrl_ptr->num_digits - 1; place_val >= 0; --place_val)
		  {
		     cur_seg7_num %= base_power_val;
		     base_power_val /= base; 
		     place_digit[place_val] =  cur_seg7_num /base_power_val;
			 if(cur_seg7_ctrl_ptr->seg7_type == SEG7_COMMON_CATHODE)
             {
	            consucc_bit_data.write_or_config_consucc_val = 0;
             }
             else
             {
 	                consucc_bit_data.write_or_config_consucc_val = mask_data;	            
             }
			 if((Port_Write(cur_seg7_ctrl_ptr->io_ch_a_led, &consucc_bit_data)) != SUCCESS)
		     {
			    system_status_flag = ERR_SEG7_NO_WRITE;
				Error_or_Warning_Proc("06.01.25", ERROR_OCCURED, system_status_flag);
			    return system_status_flag;
		     }			
			 
		  	 if(cur_seg7_ctrl_ptr->seg7_type == SEG7_COMMON_CATHODE)    
             {       
		        consucc_bit_data.write_or_config_consucc_val = cc_digit[place_digit[place_val]] & mask_data ;
             }
             else
             {				 
				 consucc_bit_data.write_or_config_consucc_val = ~cc_digit[place_digit[place_val]] & mask_data ; 
			 }
			 if((Port_Write(cur_seg7_ctrl_ptr->io_ch_a_led, &consucc_bit_data)) != SUCCESS)
		     {
			    system_status_flag = ERR_SEG7_NO_WRITE;
				Error_or_Warning_Proc("06.01.22", ERROR_OCCURED, system_status_flag);
			    return system_status_flag;
		     }
 
		     if((IO_Channel_Write(cur_seg7_ctrl_ptr->io_ch_unit_digit + place_val, cur_seg7_ctrl_ptr->digit_place_active_state )) != SUCCESS)
		     {						 
			      system_status_flag = ERR_SEG7_NO_WRITE;
				    Error_or_Warning_Proc("06.01.21", ERROR_OCCURED, system_status_flag);
			      return system_status_flag;  
		     }
             Delay_MS(REQ_TIME_DELAY_SEG7_DIGITS_IN_MS);
		    if((IO_Channel_Write(cur_seg7_ctrl_ptr->io_ch_unit_digit + place_val, !(cur_seg7_ctrl_ptr->digit_place_active_state) )) != SUCCESS)
		    {						 
			   system_status_flag = ERR_SEG7_NO_WRITE;
			   Error_or_Warning_Proc("06.01.23", ERROR_OCCURED, system_status_flag);
			   return system_status_flag;  
		    }
		 } 		
	  }
	  break;
	  case DATA_TYPE_IN_APPL_DEFINED: 
	  break;
   }	
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Seg7_Disable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.02  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Seg7_Disable(const uint8_t seg7_ch_id)
{
	 seg7_ctrl_t *cur_seg7_ctrl_ptr = NULL_DATA_PTR;
	
	 if(seg7_ch_id >= NUM_OUTPUT_DEV_ID_SEG7_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("06.02.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_seg7_ctrl_ptr = seg7_ctrl + seg7_ch_id ;  
     cur_seg7_ctrl_ptr->enable_flag = STATE_NO;
	 
   return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Seg7_Enable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.03  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Seg7_Enable(const uint8_t seg7_ch_id)
{
	 seg7_ctrl_t *cur_seg7_ctrl_ptr = NULL_DATA_PTR;
	
	 if(seg7_ch_id >= NUM_OUTPUT_DEV_ID_SEG7_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("06.03.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_seg7_ctrl_ptr = seg7_ctrl + seg7_ch_id ;  
   cur_seg7_ctrl_ptr->enable_flag = STATE_YES;
   return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_All_Seg7_Chs

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.04  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reset_All_Seg7_Chs(const uint8_t seg7_reset_type)
{
	 uint8_t seg7_ch_id;
	
	 if(seg7_reset_type != RESET_WHOLE && seg7_reset_type != RESET_MOD_STATUS)
	 {
		system_status_flag = ERR_FORMAT_INVALID;
		Error_or_Warning_Proc("06.04.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 } 
	 for(seg7_ch_id = 0; seg7_ch_id < NUM_OUTPUT_DEV_ID_SEG7_CHS; ++seg7_ch_id)
	 {
		  Seg7_No_Access(seg7_ch_id);
	 }
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Seg7_Allow_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.05 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Seg7_Allow_Access(const uint8_t seg7_ch_id)
{
	 seg7_ctrl_t *cur_seg7_ctrl_ptr = NULL_DATA_PTR;
	
	 if(seg7_ch_id >= NUM_OUTPUT_DEV_ID_SEG7_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("06.05.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_seg7_ctrl_ptr = seg7_ctrl + seg7_ch_id ;  
     cur_seg7_ctrl_ptr->access_flag = STATE_YES;
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Seg7_No_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.06 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Seg7_No_Access(const uint8_t seg7_ch_id)
{
	 seg7_ctrl_t *cur_seg7_ctrl_ptr = NULL_DATA_PTR;
	
	 if(seg7_ch_id >= NUM_OUTPUT_DEV_ID_SEG7_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("06.06.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_seg7_ctrl_ptr = seg7_ctrl + seg7_ch_id ;  
     cur_seg7_ctrl_ptr->access_flag = STATE_NO;
     Seg7_Disable(seg7_ch_id);
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Seg7_Init

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.07 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Seg7_Init(const uint8_t seg7_ch_id, const uint8_t init_mode)
{
	 seg7_ctrl_t *cur_seg7_ctrl_ptr = NULL_DATA_PTR;
	 io_config_t seg7_config;
	 	 
	 if(seg7_ch_id >= NUM_OUTPUT_DEV_ID_SEG7_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("06.07.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_seg7_ctrl_ptr = seg7_ctrl + seg7_ch_id ;
	 
	 seg7_config.io_ch = cur_seg7_ctrl_ptr->io_ch_a_led;
	 seg7_config.signal = PIN_SIG_DIGITAL;
	 seg7_config.func = IO_FUNC_GPIO;
	 seg7_config.dir = IO_DIR_OUTPUT;
	 seg7_config.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	 if(cur_seg7_ctrl_ptr->seg7_type == SEG7_COMMON_CATHODE)
	 {
		 seg7_config.state = STATE_LOW;		 
	 }
	 else
	 {
		  seg7_config.state = STATE_HIGH;		 
	 }	 
	 if(cur_seg7_ctrl_ptr->dot_disp_flag == STATE_YES)
	 {
	   seg7_config.port_pin_len = 8;
	 }
	 else
	 {
		 seg7_config.port_pin_len = 7;
	 }
		 
	 if((IO_Channels_Func_Set(&seg7_config)) != SUCCESS)
	 {
		system_status_flag = ERR_GPIO_FUNC_SET;
		Error_or_Warning_Proc("06.07.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 }
	 seg7_config.io_ch = cur_seg7_ctrl_ptr->io_ch_unit_digit;
	 seg7_config.port_pin_len = cur_seg7_ctrl_ptr->num_digits;
	 
	 seg7_config.state = !(cur_seg7_ctrl_ptr->digit_place_active_state);
	 if((IO_Channels_Func_Set(&seg7_config)) != SUCCESS)
	 {
		system_status_flag = ERR_GPIO_FUNC_SET;
		Error_or_Warning_Proc("06.07.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 }
	 return SUCCESS; 	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Seg7_DeInit

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 06.08 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Seg7_DeInit(const uint8_t seg7_ch_id, const uint8_t deinit_mode)
{
	 io_config_t seg7_unconfig;
	 seg7_ctrl_t *cur_seg7_ctrl_ptr = NULL_DATA_PTR;
	 	 
	 if(seg7_ch_id >= NUM_OUTPUT_DEV_ID_SEG7_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("06.08.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 Seg7_No_Access(seg7_ch_id);
	 cur_seg7_ctrl_ptr = seg7_ctrl + seg7_ch_id ; 
	 seg7_unconfig.io_ch  = cur_seg7_ctrl_ptr->io_ch_a_led;	
	 if(cur_seg7_ctrl_ptr->dot_disp_flag == STATE_YES)
	 {
	      seg7_unconfig.port_pin_len = 8;
	 }
	 else
	 {
		 seg7_unconfig.port_pin_len = 7;
	 }
	 seg7_unconfig.func = IO_FUNC_GPIO;
	 seg7_unconfig.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	 if((IO_Ch_Func_Reset(&seg7_unconfig)) != SUCCESS)
	 {
		system_status_flag = ERR_IO_CH_FUNC_RESET;
		 Error_or_Warning_Proc("06.08.02", ERROR_OCCURED, system_status_flag);
        return system_status_flag;		
	 }	 
	 seg7_unconfig.io_ch  = cur_seg7_ctrl_ptr->io_ch_unit_digit;
	 seg7_unconfig.port_pin_len = cur_seg7_ctrl_ptr->num_digits;
	 if((IO_Ch_Func_Reset(&seg7_unconfig)) != SUCCESS)
	 {
		system_status_flag = ERR_IO_CH_FUNC_RESET;
		 Error_or_Warning_Proc("06.08.03", ERROR_OCCURED, system_status_flag);
        return system_status_flag;		
	 }
	 return SUCCESS;
}

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
