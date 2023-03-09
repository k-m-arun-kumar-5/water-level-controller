/**************************************************************************
   FILE          :    led_matrix.c
 
   PURPOSE       :     
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
	CAUTION        :
	
  NOTE           :   
  
  CHANGE LOGS    :
  
  FILE ID        : 05  
	   
 **************************************************************************/
 
 #include "main.h"
 
 #ifdef LED_MATRIX_MOD_ENABLE
 
/* ------------------------------ macro defination ------------------------------ */
    

/* ----------------------------- global variable defination --------------------- */
 // for 5 * 7 led DOT matrix, seq write_type = SEQ_WRITE_COL, type common row cathode define row values for each of the five columns corresponding to Alphabets A through Z and some symbols
      const uint8_t led_dot_disp_map[MAX_LED_MATRIX_NUM_CHARS * NUM_DISP_MAP_EACH_CHAR]=
	                              { 
									    0x3E, 0x41, 0x41, 0x41, 0x3E, //0
										0x00, 0x42, 0x7F, 0x40, 0x00,
										0x42, 0x61, 0x51, 0x49, 0x46,
										0x41, 0x41, 0x49, 0x49, 0x7F,
                                        0x0F, 0x08, 0x08, 0x08, 0x7F,
                                        0x4F, 0x49, 0x49, 0x49, 0x79,
                                        0x7F, 0x49, 0x49, 0x49, 0x79,
                                        0x01, 0x01, 0x01, 0x01, 0x7F,
                                        0x7F, 0x49, 0x49, 0x49, 0x7F,
                                        0x4F, 0x49, 0x49, 0x49, 0x7F, // 9 										
									    0x7e, 0x09, 0x09, 0x09, 0x7E, // A
									    0x7f, 0x49, 0x49, 0x49, 0x36, // B
									    0x3e, 0x41, 0x41, 0x41, 0x22,
									    0x7f, 0x41, 0x41, 0x22, 0x1c,
									    0x7f, 0x49, 0x49, 0x49, 0x63,
									    0x7f, 0x09, 0x09, 0x09, 0x01,
									    0x3e, 0x41, 0x41, 0x49, 0x7a,
									    0x7f, 0x08, 0x08, 0x08, 0x7f,
									    0x00, 0x41, 0x7f, 0x41, 0x00,  // I
									    0x20, 0x40, 0x41, 0x3f, 0x01,
									    0x7f, 0x08, 0x14, 0x22, 0x41,
									    0x7f, 0x40, 0x40, 0x40, 0x60,
									    0x7f, 0x02, 0x04, 0x02, 0x7f,
									    0x7f, 0x04, 0x08, 0x10, 0x7f,
									    0x7F, 0x41, 0x41, 0x41, 0x7F,
									    0x7f, 0x09, 0x09, 0x09, 0x06,
									    0x3e, 0x41, 0x51, 0x21, 0x5e,
									    0x7f, 0x09, 0x19, 0x29, 0x46,
									    0x46, 0x49, 0x49, 0x49, 0x31,  // S
									    0x01, 0x01, 0x7f, 0x01, 0x01,
									    0x3f, 0x40, 0x40, 0x40, 0x3f,
									    0x1f, 0x20, 0x40, 0x20, 0x1f,
									    0x3f, 0x40, 0x30, 0x40, 0x3f,
									    0x63, 0x14, 0x08, 0x14, 0x63,
									    0x07, 0x08, 0x70, 0x08, 0x07,
									    0x61, 0x51, 0x49, 0x45, 0x43, // Z
									    0x24, 0x2A, 0x7f, 0x2A, 0x12, // $
									    0x08, 0x14, 0x22, 0x41, 0x00, // <
									    0x41, 0x22, 0x14, 0x08, 0x00, // >
									    0x14, 0x14, 0x14, 0x14, 0x14, // =
									    0x36, 0x49, 0x55, 0x22, 0x50, // &
									    0x44, 0x3c, 0x04, 0x7c, 0x44 // PI
                               };
static consucc_bit_t port_write;   	  
static int32_t cur_disp_map_index = (0 * NUM_DISP_MAP_EACH_CHAR);
static uint8_t max_num_seq_write, io_ch_seq_write_base, io_ch_port_write, mask_data = 0;

/* ----------------------------- global variable declaration -------------------- */


/* ----------------------------- global function declaration -------------------- */
static uint16_t LED_Matrix_Disp_Char_To_Char_Code(const char disp_char, uint8_t *const led_matrix_char_code_ptr);
static uint16_t LED_Matrix_Disp_Refresh(const uint8_t led_matrix_ch_id);

/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_Write

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           :  Does not support data type in str.
 
                  Disp map ie led_dot_disp_map[] is coded for only seq write type = SEQ_WRITE_COL. 
                   				  

Func ID        : 05.01  

BUGS           :     
-*------------------------------------------------------------*/
uint16_t LED_Matrix_Write(const uint8_t led_matrix_ch_id, const void *const led_matrix_char_ptr)
{
	 data_write_ctrl_ptr_t *led_matrix_data_write_ptr;
	 led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr;
	 dev_write_ctrl_para_t *led_matrix_ctrl_ptr;
	 char retrieve_led_matrix_char;    
	 consucc_bit_t port_write_scan;     
     uint8_t base, port_pin, led_matrix_char_code, repeat, output_data_type ;
	   
	if(led_matrix_char_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("05.01.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}		 
    if(led_matrix_ch_id >= NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_Proc("05.01.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	}	 
	cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id ;
	if( cur_led_matrix_ctrl_ptr->access_flag != STATE_YES) 
	{  
          system_status_flag = WARN_CUR_DATA_ID_DEV_NO_ACCESS;
		   Error_or_Warning_Proc("05.01.03", ERROR_OCCURED, system_status_flag);
          return  system_status_flag;
	}		 
	if( cur_led_matrix_ctrl_ptr->enable_flag != STATE_YES) 
	{  
         system_status_flag = WARN_CUR_DATA_ID_DEV_DISABLED;	
		  Error_or_Warning_Proc("05.01.04", ERROR_OCCURED, system_status_flag);
         return  system_status_flag;
	}	  
	led_matrix_data_write_ptr  = (data_write_ctrl_ptr_t *)led_matrix_char_ptr;	
	if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_OUTPUT_DATA_TYPE, &output_data_type)) != SUCCESS)
	{
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("05.01.05", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
	}	
	switch(output_data_type)
	{
		case DATA_TYPE_IN_CHAR:
		   if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_CHAR, &retrieve_led_matrix_char )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("05.01.06", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   }	
		  if((LED_Matrix_Disp_Char_To_Char_Code(retrieve_led_matrix_char, &led_matrix_char_code))!= SUCCESS)
      { 
          system_status_flag = ERR_LED_MATRIX_DATA_TYPE_INVALID;
		   Error_or_Warning_Proc("05.01.07", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;
      }        
		break;
		case DATA_TYPE_IN_BINARY:
		   base = BASE_02;
		  if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_UINT8, &led_matrix_char_code )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("05.01.08", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   } 
           if(led_matrix_char_code > base)
		   {
				system_status_flag = ERR_LED_MATRIX_DATA_TYPE_INVALID;
				 Error_or_Warning_Proc("05.01.09", ERROR_OCCURED, system_status_flag);
		        return system_status_flag; 
		   }	   
		break; 
		case DATA_TYPE_IN_DECIMAL:
		   base = BASE_10;
		   if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_UINT8, &led_matrix_char_code )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("05.01.10", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   } 
           if(led_matrix_char_code > base)
		   {
				system_status_flag = ERR_LED_MATRIX_DATA_TYPE_INVALID;
				 Error_or_Warning_Proc("05.01.11", ERROR_OCCURED, system_status_flag);
		        return system_status_flag; 
		   }	   
		break; 
        case DATA_TYPE_IN_HEXA:
		   base = BASE_16;
		   if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_STATUS_DATA_UINT8, &led_matrix_char_code )) != SUCCESS)
		   {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("05.01.12", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
		   } 
           if(led_matrix_char_code > base)
		   {
				system_status_flag = ERR_LED_MATRIX_DATA_TYPE_INVALID;
				 Error_or_Warning_Proc("05.01.13", ERROR_OCCURED, system_status_flag);
		        return system_status_flag; 
		   }	   
		break; 		
		default:
		   system_status_flag = ERR_LED_MATRIX_DATA_TYPE_INVALID;
		    Error_or_Warning_Proc("05.01.14", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
	}
	
	      if(cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_COL)
	      {
		     io_ch_port_write = cur_led_matrix_ctrl_ptr->io_ch_rowa;		  
		     io_ch_seq_write_base = cur_led_matrix_ctrl_ptr->io_ch_col1;		
	         if(cur_led_matrix_ctrl_ptr->font_size == FONT_SIZE_5_7)
		     {
	            port_write.bits_len = 7;
		        max_num_seq_write = 5;			   
		    }
            else
		    {
	           port_write.bits_len = 8;
		       max_num_seq_write = 8;
		    }
	     }
	     else
	     {
	    	io_ch_port_write = cur_led_matrix_ctrl_ptr->io_ch_col1;
	    	io_ch_seq_write_base = cur_led_matrix_ctrl_ptr->io_ch_rowa;		
	        if(cur_led_matrix_ctrl_ptr->font_size == FONT_SIZE_5_7)
		    {
	           port_write.bits_len = 5;
		       max_num_seq_write = 7;
		    }
            else
		    {
	           port_write.bits_len = 8;
		       max_num_seq_write = 8;
		    }	
	    }
	    port_write_scan.start_bit_pos = io_ch_seq_write_base % NUM_PINS_PER_PORT;
	    port_write_scan.bits_len = max_num_seq_write;
        if((cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_COL && cur_led_matrix_ctrl_ptr->row_type == ROW_CATHODE_TYPE) ||
          (cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_ROW && cur_led_matrix_ctrl_ptr->row_type == ROW_ANODE_TYPE))	
	    {
	        mask_data = 0;
	        for(repeat = 0; repeat < port_write.bits_len; ++repeat)
	        {
	         	mask_data |= 1 << repeat;
	        }
	    }
	    port_write_scan.write_or_config_consucc_val = 0;
	    if((cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_COL && cur_led_matrix_ctrl_ptr->row_type == ROW_ANODE_TYPE) || 
          (cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_ROW && cur_led_matrix_ctrl_ptr->row_type == ROW_CATHODE_TYPE))
	    {
	        for(repeat = 0; repeat < max_num_seq_write; ++repeat)
	        {
	           port_write_scan.write_or_config_consucc_val |=  1 << repeat;  
	        }
	    }
	    if((Port_Write(io_ch_seq_write_base, &port_write_scan))!= SUCCESS)
	    {
		    system_status_flag = ERR_LED_MATRIX_WRITE;
			 Error_or_Warning_Proc("05.01.15", ERROR_OCCURED, system_status_flag);
	  	    return system_status_flag;
	    }
        port_pin = io_ch_port_write % NUM_PINS_PER_PORT;
	    port_write.start_bit_pos = port_pin;
	    led_matrix_ctrl_ptr = (dev_write_ctrl_para_t *)led_matrix_data_write_ptr->dev_output_ctrl_para_ptr;
	    /* led_matrix_ctrl_ptr->led_matrix_scroll_fmt has led matrix scroll format,
    		which is not required, in our case. We just required to display any char in any order without scroll format */
		cur_disp_map_index = led_matrix_char_code * (max_num_seq_write);		 
	    if((LED_Matrix_Disp_Refresh(led_matrix_ch_id)) != SUCCESS)
		{
		     system_status_flag = ERR_LED_MATRIX_WRITE;
			  Error_or_Warning_Proc("05.01.16", ERROR_OCCURED, system_status_flag);
		     return system_status_flag;
		}			   
		      
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_Disp_Refresh

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           :               

Func ID        : 05.02  

BUGS           :     
-*------------------------------------------------------------*/
static uint16_t LED_Matrix_Disp_Refresh(const uint8_t led_matrix_ch_id)
{   
    led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr;	
	uint32_t repeat;
	uint8_t select_state ; 
	int8_t cur_num_seq_write;
	
    cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id ;
    for(repeat = 0; repeat < REG_CNT_LED_MATRIX_CHAR; ++repeat)
	{
		 for(cur_num_seq_write = 0; cur_num_seq_write < max_num_seq_write ; ++cur_num_seq_write)
		 {
           if((cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_COL && cur_led_matrix_ctrl_ptr->row_type == ROW_ANODE_TYPE) ||
            (cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_ROW && cur_led_matrix_ctrl_ptr->row_type == ROW_CATHODE_TYPE))		 
           {
              port_write.write_or_config_consucc_val = 0; 	 
           }          
		   else
           {
		      port_write.write_or_config_consucc_val = mask_data;              
           }
		   if(( Port_Write(io_ch_port_write, &port_write)) != SUCCESS)
		   {
			    system_status_flag = ERR_LED_MATRIX_WRITE;
				 Error_or_Warning_Proc("05.02.01", ERROR_OCCURED, system_status_flag);
			    return system_status_flag;
		   }
		   
		   if((cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_COL && cur_led_matrix_ctrl_ptr->row_type == ROW_CATHODE_TYPE) || 
             (cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_ROW && cur_led_matrix_ctrl_ptr->row_type == ROW_ANODE_TYPE))
		   {
			    select_state = STATE_HIGH;
		   }
		   else
		   {
			    select_state = STATE_LOW;
		   }		   
	       if((IO_Channel_Write(io_ch_seq_write_base + cur_num_seq_write, select_state )) != SUCCESS)
		   {
			   system_status_flag = ERR_LED_MATRIX_WRITE;
			    Error_or_Warning_Proc("05.02.02", ERROR_OCCURED, system_status_flag);
			   return system_status_flag;
		   }
       
		 if((cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_COL && cur_led_matrix_ctrl_ptr->row_type == ROW_ANODE_TYPE) ||
            (cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_ROW && cur_led_matrix_ctrl_ptr->row_type == ROW_CATHODE_TYPE))		 
        {
            port_write.write_or_config_consucc_val = led_dot_disp_map[cur_disp_map_index + cur_num_seq_write]; 	 
        }          
		else
        {
		      port_write.write_or_config_consucc_val = (~led_dot_disp_map[cur_disp_map_index + cur_num_seq_write]) & mask_data;           
        }
		   if((Port_Write(io_ch_port_write, &port_write)) != SUCCESS)
		   {
			    system_status_flag = ERR_LED_MATRIX_WRITE;
				 Error_or_Warning_Proc("05.02.03", ERROR_OCCURED, system_status_flag);
			    return system_status_flag;
		   }		   
		   Delay_MS(REQ_TIME_DELAY_LED_MATRIX_SCAN_IN_MS);
		   if((cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_COL && cur_led_matrix_ctrl_ptr->row_type == ROW_CATHODE_TYPE) || 
              (cur_led_matrix_ctrl_ptr->seq_write_type == SEQ_WRITE_ROW && cur_led_matrix_ctrl_ptr->row_type == ROW_ANODE_TYPE))
		   {
			   select_state = STATE_LOW;
		   }
		   else
		   {
			   select_state = STATE_HIGH;
		   }
		   if((IO_Channel_Write(io_ch_seq_write_base + cur_num_seq_write, select_state )) != SUCCESS)
		   {
		      system_status_flag = ERR_LED_MATRIX_WRITE;
			   Error_or_Warning_Proc("05.02.04", ERROR_OCCURED, system_status_flag);
			  return system_status_flag;
		   }
		}
    }
	return SUCCESS;	
 }
   	
/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_Disp_Char_To_Char_Code

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.03 

Bugs           :  
-*------------------------------------------------------------*/
static uint16_t LED_Matrix_Disp_Char_To_Char_Code(const char disp_char, uint8_t *const led_matrix_char_code_ptr)
{
	uint8_t led_matrix_base_char_code ;
	
	if(led_matrix_char_code_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("05.03.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(disp_char >= '0' && disp_char <= '9')
	{
		// '0' char code = 0
	    led_matrix_base_char_code = 0;
		*led_matrix_char_code_ptr = led_matrix_base_char_code + (disp_char - '0');
	}
	else if (disp_char >= 'A' && disp_char <= 'Z')
	{
		// 'A' char code = 10
	    led_matrix_base_char_code = 10;
		*led_matrix_char_code_ptr = led_matrix_base_char_code + (disp_char - 'A');
	} 		
	else
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		 Error_or_Warning_Proc("05.03.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_Disable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.04  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LED_Matrix_Disable(const uint8_t led_matrix_ch_id)
{
	 led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr = NULL_DATA_PTR;
	
	 if(led_matrix_ch_id >= NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("05.04.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id ;  
     cur_led_matrix_ctrl_ptr->enable_flag = STATE_NO;
	 
   return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_Enable

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.05  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LED_Matrix_Enable(const uint8_t led_matrix_ch_id)
{
	 led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr = NULL_DATA_PTR;
	
	 if(led_matrix_ch_id >= NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("05.05.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id ;  
     cur_led_matrix_ctrl_ptr->enable_flag = STATE_YES;
   return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_All_LED_Matrix_Chs

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.06  

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Reset_All_LED_Matrix_Chs(const uint8_t led_matrix_reset_type)
{
	 uint8_t led_matrix_ch_id;
	
	 if(led_matrix_reset_type != RESET_WHOLE && led_matrix_reset_type != RESET_MOD_STATUS)
	 {
		system_status_flag = ERR_FORMAT_INVALID;
		Error_or_Warning_Proc("05.06.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 } 
	 for(led_matrix_ch_id = 0; led_matrix_ch_id < NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS; ++led_matrix_ch_id)
	 {
		 LED_Matrix_No_Access(led_matrix_ch_id);
	 }
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_Allow_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.07 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LED_Matrix_Allow_Access(const uint8_t led_matrix_ch_id)
{
	led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr = NULL_DATA_PTR;
	
	 if(led_matrix_ch_id >= NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("05.07.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id ;  
     cur_led_matrix_ctrl_ptr->access_flag = STATE_YES;
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_No_Access

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.08 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LED_Matrix_No_Access(const uint8_t led_matrix_ch_id)
{
	 led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr = NULL_DATA_PTR;
	
	 if(led_matrix_ch_id >= NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("05.08.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id ;  
     cur_led_matrix_ctrl_ptr->access_flag = STATE_NO;
     LED_Matrix_Disable(led_matrix_ch_id);
	 return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_Init

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.09 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LED_Matrix_Init(const uint8_t led_matrix_ch_id, const uint8_t init_mode)
{
	 led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr = NULL_DATA_PTR;
	 io_config_t led_matrix_config;	 
		 
	 if(led_matrix_ch_id >= NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_Proc("05.09.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id ;
	 
	 led_matrix_config.io_ch = cur_led_matrix_ctrl_ptr->io_ch_rowa;
	 led_matrix_config.signal = PIN_SIG_DIGITAL;
	 led_matrix_config.dir = IO_DIR_OUTPUT;
	 led_matrix_config.func = IO_FUNC_GPIO;
	 led_matrix_config.state = STATE_LOW;
	 led_matrix_config.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	 if(cur_led_matrix_ctrl_ptr->font_size == FONT_SIZE_5_7)
	    led_matrix_config.port_pin_len = 7;
	 else
		led_matrix_config.port_pin_len = 8;
		 
	 if((IO_Channels_Func_Set(&led_matrix_config)) != SUCCESS)
	 {
		system_status_flag = ERR_GPIO_FUNC_SET;
		 Error_or_Warning_Proc("05.09.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 }
	 led_matrix_config.io_ch = cur_led_matrix_ctrl_ptr->io_ch_col1;
	 if(cur_led_matrix_ctrl_ptr->font_size == FONT_SIZE_5_7)
	 {
	    led_matrix_config.port_pin_len = 5;
	 }
	 else
	 {
		led_matrix_config.port_pin_len = 8;
	 }
	 led_matrix_config.state = STATE_LOW;
	 if((IO_Channels_Func_Set(&led_matrix_config)) != SUCCESS)
	 {
		system_status_flag = ERR_GPIO_FUNC_SET;
		 Error_or_Warning_Proc("05.09.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	 }
	 return SUCCESS; 	
}

/*------------------------------------------------------------*
FUNCTION NAME  : LED_Matrix_DeInit

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 05.10

Bugs           :  
-*------------------------------------------------------------*/
uint16_t LED_Matrix_DeInit(const uint8_t led_matrix_ch_id, const uint8_t deinit_mode )
{
	 io_config_t led_matrix_unconfig;
	 led_matrix_ctrl_t *cur_led_matrix_ctrl_ptr = NULL_DATA_PTR;
		 
	 if(led_matrix_ch_id >= NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS)
	 {
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		   Error_or_Warning_Proc("05.10.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	 }
	 cur_led_matrix_ctrl_ptr = led_matrix_ctrl + led_matrix_ch_id;
	 
	 LED_Matrix_No_Access(led_matrix_ch_id);
	 led_matrix_unconfig.io_ch  = cur_led_matrix_ctrl_ptr->io_ch_rowa;
	 if(cur_led_matrix_ctrl_ptr->font_size ==FONT_SIZE_5_7)
	    led_matrix_unconfig.port_pin_len = 7;
	 else
		led_matrix_unconfig.port_pin_len = 8; 
	 led_matrix_unconfig.func = IO_FUNC_GPIO;
	 led_matrix_unconfig.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	 if((IO_Ch_Func_Reset(&led_matrix_unconfig)) != SUCCESS)
	 {
		system_status_flag = ERR_IO_CH_FUNC_RESET;
		 Error_or_Warning_Proc("05.10.02", ERROR_OCCURED, system_status_flag);
        return system_status_flag;		
	 }	 
	 led_matrix_unconfig.io_ch  = cur_led_matrix_ctrl_ptr->io_ch_col1;
	 if(cur_led_matrix_ctrl_ptr->font_size == FONT_SIZE_5_7)
	 {
	    led_matrix_unconfig.port_pin_len = 5;
	 }
	 else
	 {
		led_matrix_unconfig.port_pin_len = 8;  
	 }
     if((IO_Ch_Func_Reset(&led_matrix_unconfig)) != SUCCESS)
	 {
		system_status_flag = ERR_IO_CH_FUNC_RESET;
		 Error_or_Warning_Proc("05.10.03", ERROR_OCCURED, system_status_flag);
        return system_status_flag;		
	 }
	 return SUCCESS;
}

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
