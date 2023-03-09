/**************************************************************************
   FILE          :    dev_oper.c
 
   PURPOSE       :     
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
	CAUTION        : Dev_Oper_Func_Table[] is indexed based on dev_src_id_t enum data. Eg, COMM_DEV_ID_UART = 5, by enum const (dev_src_id_t)
                   Dev_Oper_Func_Table[COMM_DEV_ID_UART] must have datas related to COMM_DEV_ID_UART.
	
  NOTE           :   
  
  CHANGE LOGS    :
  
  FILE ID        : 08  
	   
 **************************************************************************/ 
 #include "main.h"

/* ------------------------------ macro defination ------------------------------ */

/* ----------------------------- global variable defination --------------------- */
uint32_t  alloc_dev_ids_ch_id_bit_field[NUM_DEVS];

/* ----------------------------- global variable declaration -------------------- */

/* ----------------------------- global function declaration -------------------- */
 static uint16_t Dev_Oper_Read_Status_Proc( const uint8_t read_dev_id, const uint8_t rcvd_read_status, unsigned char last_read_char);
 static uint16_t Cur_Data_ID_Dev_Src_Basic_Oper_Func(const uint8_t dev_src_basic_oper_func, const uint8_t dev_type, const dev_id_t *const dev_basic_oper_func_ptr);
 static uint16_t Data_ID_Read_Try_Complete_Proc(const uint8_t data_id);
 
/* ---------------------------- function pointer defination ---------------------- */

/* DONT_CHANGE: BEGIN - Dev_Oper_Func_Table's index data corresponds to dev_src_id_t enum corresponding dev id  
                       ie for Dev_Oper_Func_Table's index = 0, UART related data and in dev_src_id_t, COMM_DEV_ID_UART = 0 for UART  */

Dev_Oper_Func_t Dev_Oper_Func_Table[] = { 
                        #ifdef UART_MOD_ENABLE
                         {UART_DeInit, UART_Init, UART_No_Access, UART_Allow_Access, UART_Disable, UART_Enable, UART_Receive_Char, UART_Transmit, NULL_DATA_PTR, NULL_DATA_PTR},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif
						#ifdef I2C_MOD_ENABLE
                        { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif
                        #ifdef SPI_MOD_ENABLE
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif
              			#ifdef MISC_MOD_ENABLE
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif
                        #ifdef PWM_MOD_ENABLE
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif						
                        #ifdef DAC_MOD_ENABLE
                        { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif                        
						#ifdef SEG7_MOD_ENABLE
						{ Seg7_DeInit, Seg7_Init, Seg7_No_Access, Seg7_Allow_Access, Seg7_Disable, Seg7_Enable, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, Seg7_Write },
						#else
						 { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
					    #endif
						#ifdef LCD_MOD_ENABLE
						 { LCD_DeInit ,LCD_Init, LCD_No_Access, LCD_Allow_Access, LCD_Disable, LCD_Enable, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, LCD_Write},
						#else
						 { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
					    #endif
						#ifdef LED_MATRIX_MOD_ENABLE
                         { LED_Matrix_DeInit, LED_Matrix_Init, LED_Matrix_No_Access, LED_Matrix_Allow_Access, LED_Matrix_Disable, LED_Matrix_Enable, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, LED_Matrix_Write},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif                      	
                        #ifdef KEYBOARD_MOD_ENABLE
 						 { Keyboard_DeInit, Keyboard_Init, Keyboard_No_Access, Keyboard_Allow_Access, Keyboard_Disable, Keyboard_Enable, NULL_DATA_PTR, NULL_DATA_PTR, Keyboard_Read, NULL_DATA_PTR},
                        #else
                         { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif
						#ifdef ADC_MOD_ENABLE
						 { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #else
                        { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif
						#ifdef SW_MOD_ENABLE
						 { SW_DeInit, SW_Init, SW_No_Access, SW_Allow_Access, SW_Disable, SW_Enable, NULL_DATA_PTR, NULL_DATA_PTR, SW_Press_Proc, NULL_DATA_PTR},
                        #else
                        { NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR},
                        #endif
						{ NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR, NULL_DATA_PTR}
                      }; 

Comm_Dev_Spec_Oper_Enable_Ctrl_t Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[] =  {
	                                                                             #ifdef UART_MOD_ENABLE  
																				    {UART_Receive_Enable_Ctrl, UART_Transmit_Enable_Ctrl  },
                                                                                 #else
                                                                                  	{NULL_DATA_PTR, NULL_DATA_PTR },
                                                                                 #endif
																				 #ifdef I2C_MOD_ENABLE  
																				    {NULL_DATA_PTR,  NULL_DATA_PTR },
                                                                                 #else
                                                                                  	{NULL_DATA_PTR, NULL_DATA_PTR },
                                                                                 #endif
																				 #ifdef SPI_MOD_ENABLE  
																				    {NULL_DATA_PTR,  NULL_DATA_PTR },
                                                                                 #else
                                                                                  	{NULL_DATA_PTR, NULL_DATA_PTR },
                                                                                 #endif
																				    {NULL_DATA_PTR, NULL_DATA_PTR }
                                              };

/* DONT_CHANGE : END */	
										  
/*------------------------------------------------------------*
FUNCTION NAME  : Cur_Data_ID_Read_Oper_Func

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.01

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Cur_Data_ID_Read_Oper_Func(const uint8_t dev_type, unsigned char *const last_read_data_char_ptr)
{
 	data_id_status_para_t *cur_data_id_status_para_ptr;
	const dev_id_t *data_dev_read_oper_func;
    dev_read_ctrl_para_t dev_read_ctrl_para;
    uint16_t ret_status;
	uint8_t max_num_read_devs, min_read_dev_id, max_read_dev_id, read_data_stream_fmt, receive_data_service_type, src_interrupt_data_arr_search_index, search_interrupt_req_src_id; 
    volatile unsigned char rcvd_data_char;	
	static uint8_t i = 0;
  
	if(cur_data_id >= NUM_DATA_IDS)
	{
	   system_status_flag = ERR_DATA_ID_EXCEED;
	   Error_or_Warning_Proc("08.01.01", ERROR_OCCURED, system_status_flag);
	   return system_status_flag;
	}
	if(last_read_data_char_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
	    Error_or_Warning_Proc("08.01.02", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	if(dev_type != DEV_INPUT_TYPE && dev_type != DEV_RECEIVE_TYPE)
	{
		system_status_flag = ERR_READ_DEV_TYPE;
	    Error_or_Warning_Proc("08.01.03", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	*last_read_data_char_ptr = NULL_CHAR;
	cur_data_id_status_para_ptr = data_id_status_para + cur_data_id;
	if(cur_data_id_status_para_ptr->data_read_status.max_try_reached_flag == STATE_YES)
	{
		return CUR_DATA_ID_READ_MAX_TRIED;
	}
	if(cur_data_id_status_para_ptr->data_read_status.last_try_read_and_ready_to_process_flag == STATE_YES)
	{
		return CUR_DATA_ID_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS;
	}
	if(cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag == STATE_YES)
	{
		  return CUR_DATA_ID_UNTIL_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS;
	}
   switch(dev_type)
	{
		case DEV_INPUT_TYPE:
		    if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_INPUT_DATA_STREAM, &read_data_stream_fmt)) != SUCCESS)
			{
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
		          Error_or_Warning_Proc("08.01.04", ERROR_OCCURED, system_status_flag);
                  return system_status_flag; 
			}			
		    data_dev_read_oper_func =  data_ids_dev_srcs[cur_data_id].data_input_dev_ids;	        
			max_num_read_devs = MAX_DATA_INPUT_DEVS;
			min_read_dev_id = BEGIN_INPUT_DEV_ID;
			max_read_dev_id = END_INPUT_DEV_ID;			
			ret_status = CUR_DATA_ID_NO_INPUT_DEV;			
	    break;
		case DEV_RECEIVE_TYPE:
		    if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_RECEIVE_DATA_STREAM, &read_data_stream_fmt)) != SUCCESS)
			{
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
		          Error_or_Warning_Proc("08.01.05", ERROR_OCCURED, system_status_flag);
                  return system_status_flag; 
			}
		    data_dev_read_oper_func =  data_ids_dev_srcs[cur_data_id].data_receive_dev_ids;		    
		    max_num_read_devs = MAX_DATA_RECEIVE_DEVS;
			min_read_dev_id = BEGIN_COMM_DEV_ID;
			max_read_dev_id = END_COMM_DEV_ID;	
			if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_RECEIVE_SERVICE, &receive_data_service_type)) != SUCCESS)
	        {
	           system_status_flag = ERR_APPL_DATA_RETRIEVE;
	           Error_or_Warning_Proc("08.01.06", ERROR_OCCURED, system_status_flag);
               return system_status_flag; 
	        }
			ret_status = CUR_DATA_ID_NO_RECEIVER_DEV;
		break;
	}
  // To be called when data stream is supported
  /*  if(read_data_stream_fmt == DEVICE_DATA_NA)
	{
		system_status_flag = CUR_DATA_ID_NO_STREAM;
		Error_or_Warning_Proc("08.01.06", ERROR_OCCURED, system_status_flag);
        return system_status_flag;
	} */
	       
	
	    if(dev_type == DEV_INPUT_TYPE || receive_data_service_type == EVENT_POLLING) 
		{
			if(i >= max_num_read_devs || (data_dev_read_oper_func + i)->dev_id < min_read_dev_id || (data_dev_read_oper_func + i)->dev_id > max_read_dev_id)
			{
				//not a valid read device.
				i = 0;
				return ret_status;
			}
            else 
			{
               ++i;
			}			   
		}
		    if(cur_data_id_status_para_ptr->data_read_status.max_try_reached_flag == STATE_YES)
	        {
	    	    return CUR_DATA_ID_READ_MAX_TRIED;
	        }
		    if(cur_data_id_status_para_ptr->data_read_status.last_try_read_and_ready_to_process_flag == STATE_YES)
	        {
		       return CUR_DATA_ID_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS;
	        }
	        if(cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag == STATE_YES)
	        {
		        return CUR_DATA_ID_UNTIL_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS;
	        }
		
	        // num chars read with an appended NULL_CHAR
	        if(cur_data_id_status_para_ptr->data_value.data_str_len_or_pos + 1 >= cur_data_id_status_para_ptr->max_allocated_data_len)
	        {
		       cur_data_id_status_para_ptr->data_read_status.reach_max_alloc_read_chars_flag = STATE_YES;
		       switch( cur_data_id_status_para_ptr->data_read_status.data_read_mode) 
	           {
			        /* if data's max data allocated char < num entered chars, then new entered chars are taken, till terminator char is read.
			           if data's max data allocated char >= num entered chars, then newly read chars are discarded, till terminator char is read */
		            case DATA_ID_READ_MODE_TILL_TERMINATOR:			    					
		            break;
			        case DATA_ID_READ_MODE_TILL_ALLOC:
                       cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag = STATE_YES;
                       ret_status = Data_ID_Read_Try_Complete_Proc(cur_data_id);
                       return ret_status;				
                    //break;			
	           }
	        }
	        else
	        {
		        cur_data_id_status_para_ptr->data_read_status.reach_max_alloc_read_chars_flag = STATE_NO; 
	        }
	        dev_read_ctrl_para.cur_data_read_status = cur_data_id_status_para_ptr->data_read_status;			
            if(dev_type == DEV_INPUT_TYPE || receive_data_service_type == EVENT_POLLING)
			{
		       switch(dev_type)
		       {
		    	  case DEV_INPUT_TYPE:
			         if(Dev_Oper_Func_Table[(data_dev_read_oper_func + i)->dev_id].Dev_Input_Func_Ptr == NULL_DATA_PTR)
	                 {
		                system_status_flag = ERR_NULL_PTR;
		                Error_or_Warning_Proc("08.01.07", ERROR_OCCURED, system_status_flag);
                        return system_status_flag; 
	                 }
	                 ret_status = (*Dev_Oper_Func_Table[(data_dev_read_oper_func + i)->dev_id].Dev_Input_Func_Ptr)((data_dev_read_oper_func + i)->dev_ch_id, &dev_read_ctrl_para);
	              break;
			      case DEV_RECEIVE_TYPE:
			        if(Dev_Oper_Func_Table[(data_dev_read_oper_func + i)->dev_id].Dev_Receive_Func_Ptr == NULL_DATA_PTR)
	                {
		               system_status_flag = ERR_NULL_PTR;
		               Error_or_Warning_Proc("08.01.08", ERROR_OCCURED, system_status_flag);
                       return system_status_flag; 
	                }
			        ret_status = (*Dev_Oper_Func_Table[(data_dev_read_oper_func + i)->dev_id].Dev_Receive_Func_Ptr)((data_dev_read_oper_func + i)->dev_ch_id, &dev_read_ctrl_para);
			     break;
			   }          
			}
			else if(dev_type == DEV_RECEIVE_TYPE && receive_data_service_type == EVENT_INTERRUPT)
			{
				     /* data char is received by isr and dev_read_ctrl_para.read_data_char has just received char and dev_id is valid receiver comm to receive data.
					   if just received char, then ret_status = COMM_RCVD_CHAR */
					 /* TO_DO : from receive_type dev_ch to src_interrupt_dev */  
					 ret_status = Search_Src_Interrupt_Data_Arr_By_Src(search_interrupt_req_src_id, &src_interrupt_data_arr_search_index);
                     switch(ret_status)
					 {
                        case SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND:
                          ret_status = COMM_NO_RCV_CHAR;
                        break;
                        case SUCCESS:						
                         	switch(src_interrupt_dev_data[src_interrupt_data_arr_search_index].process_interrupt_status)
				         	{
                               case COMM_RCVD_CHAR:
					        	  ret_status = COMM_RCVD_CHAR;
					           break;
						       default:
						          ret_status = COMM_NO_RCV_CHAR;
						       break;
					        }
					 }
			}
	        switch(ret_status)
            {
                case DATA_CHAR_READ:
		        case COMM_RCVD_CHAR:
		          if(dev_type == DEV_INPUT_TYPE || receive_data_service_type == EVENT_POLLING)
				  {
					  *last_read_data_char_ptr = dev_read_ctrl_para.read_data_char;
                      ret_status = Dev_Oper_Read_Status_Proc((data_dev_read_oper_func + i)->dev_id, ret_status, dev_read_ctrl_para.read_data_char);
				  }
                  else if(dev_type == DEV_RECEIVE_TYPE && receive_data_service_type == EVENT_INTERRUPT)
				  {
					rcvd_data_char = *(volatile unsigned char *)src_interrupt_dev_data[src_interrupt_data_arr_search_index].src_interrupt_data_ptr;
					*last_read_data_char_ptr = rcvd_data_char;
					Reset_Src_Interrupt_Data_Arr(src_interrupt_data_arr_search_index);
					/* TO_DO : from src_interrupt_dev to receive dev_ch */  
                    ret_status = Dev_Oper_Read_Status_Proc(src_interrupt_dev_data[src_interrupt_data_arr_search_index].src_interrupt_dev, ret_status, *last_read_data_char_ptr);
				  }					  
                  switch(ret_status)
		          {
			           case ERR_DEV_OPER_READ_FUNC:
					      *last_read_data_char_ptr = NULL_CHAR;
			              system_status_flag = ret_status;
						  Error_or_Warning_Proc("08.01.09", ERROR_OCCURED, system_status_flag);
                          return system_status_flag;
                   //break;
                    case CUR_DATA_ID_COMPLETE_READ_AND_READY_PROCESS:
					    ret_status = Data_ID_Read_Try_Complete_Proc(cur_data_id);						
                        return ret_status;	
			           //break;	
					case CUR_DATA_ID_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS:					  
					break;
		          }
                break;              
                case SW_OR_KEY_NOT_PRESSED:
		        case COMM_NO_RCV_CHAR:
                case WARN_CUR_DATA_ID_DEV_DISABLED: 
                case WARN_CUR_DATA_ID_DEV_NO_ACCESS: 		    
                break;           
                default: 
                   system_status_flag = ERR_DEV_OPER_READ_FUNC;
			       Error_or_Warning_Proc("08.01.10", ERROR_OCCURED, system_status_flag);
			       return system_status_flag;
            }
	return ret_status;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Dev_Oper_Read_Status_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           :   

Func ID        : 08.02  

BUGS           :              
-*------------------------------------------------------------*/
static uint16_t Dev_Oper_Read_Status_Proc( const uint8_t read_dev_id, const uint8_t rcvd_read_status, unsigned char last_read_char)
{
    #ifdef KEYBOARD_MOD_ENABLE
	  file_t *stdin_ptr = &stdin_keyboard;	  	  
	#endif	
	data_id_status_para_t *cur_data_id_status_para_ptr = NULL_DATA_PTR;
	uint16_t ret_status = rcvd_read_status;
	uint8_t data_valid_read_char_flag = STATE_NO, input_data_type,
    	char_type_read_enable_flag, dev_type, receive_data_service_type;
	char data_read_terminator_char;
	
	if(cur_data_id >= NUM_DATA_IDS)
	{
	   system_status_flag = ERR_DATA_ID_EXCEED;
	   	Error_or_Warning_Proc("08.02.01", ERROR_OCCURED, system_status_flag);
	   return ERR_DEV_OPER_READ_FUNC;
	}
	cur_data_id_status_para_ptr = data_id_status_para + cur_data_id;
	if(read_dev_id >= BEGIN_INPUT_DEV_ID && read_dev_id <= END_INPUT_DEV_ID)
	{
		dev_type = DEV_INPUT_TYPE;
	}
	else if(read_dev_id >= BEGIN_COMM_DEV_ID && read_dev_id <= END_COMM_DEV_ID )
	{
		dev_type = DEV_RECEIVE_TYPE;		
	}
	else 
	{
		system_status_flag = ERR_READ_DEV_TYPE;
	    Error_or_Warning_Proc("08.02.02", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	if(cur_data_id_status_para_ptr->data_read_status.last_try_read_and_ready_to_process_flag == STATE_YES)
	{
	   	return CUR_DATA_ID_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS;
	}
	if(cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag == STATE_YES)
	{
	        return CUR_DATA_ID_COMPLETE_READ_AND_READY_PROCESS;
	}
	switch(rcvd_read_status)
	{ 
	    case  DATA_CHAR_READ:
		case  COMM_RCVD_CHAR:
		   if(last_read_char > ASCII_MAX_CHAR_CODE)
		   {
			   system_status_flag = ERR_DATA_OUT_OF_RANGE;
			   	Error_or_Warning_Proc("08.02.03", ERROR_OCCURED, system_status_flag);
			   return ERR_DEV_OPER_READ_FUNC;
		   }
		   switch( cur_data_id_status_para_ptr->data_read_status.data_read_mode) 
	       {
			 /* if data's max data allocated char < num entered chars, then new entered chars are taken, till terminator char is read.
			   if data's max data allocated char >= num entered chars, then newly read chars are discarded, till terminator char is read */
		      case DATA_ID_READ_MODE_TILL_TERMINATOR:
			     if(( Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_DATA_READ_TERMINATOR_CHAR, &data_read_terminator_char)) != SUCCESS)
				 {
					  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				      Error_or_Warning_Proc("08.02.04", ERROR_OCCURED, system_status_flag);
				      return system_status_flag; 
				 }
			    if(data_read_terminator_char == last_read_char )
		        {
					cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos]= NULL_CHAR;
			        cur_data_id_status_para_ptr->data_read_status.read_valid_terminator_char_flag = STATE_YES; 
			        cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag = STATE_YES;
					return CUR_DATA_ID_COMPLETE_READ_AND_READY_PROCESS;								       
		        }	
			    if(cur_data_id_status_para_ptr->data_read_status.reach_max_alloc_read_chars_flag == STATE_YES && last_read_char != BACKSPACE_CHAR && cur_data_id_status_para_ptr->data_read_status.read_valid_terminator_char_flag == STATE_NO )
				{					   
					return CUR_DATA_ID_REACH_MAX_CHARS_READ_WAIT_TERMINATOR_CHAR;
				}					
		      break;
			  case DATA_ID_READ_MODE_TILL_ALLOC:                 				
              break;			
	       }
		   		 
		  if(last_read_char >= NUM_0_CHAR && last_read_char <= NUM_9_CHAR)
		  {  
	         //numeric char read
	         if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_READ_NUM_CHAR_ENABLE_FLAG, &char_type_read_enable_flag)) != SUCCESS)
			 {				 
			      system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("08.02.05", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
			 }
		     if(char_type_read_enable_flag ==  STATE_NO)		
		     {
				 system_status_flag = WARN_CUR_DATA_ID_NUM_CHAR_READ_BUT_DISABLED;
				 //Error_or_Warning_Proc("08.02.06", WARNING_OCCURED, system_status_flag);
		         return system_status_flag;
		     }			 
		     data_valid_read_char_flag = STATE_YES;
			 break;
		  }
		  if(last_read_char >= ENGLISH_SMALL_ALPHA_a_CHAR && last_read_char <= ENGLISH_SMALL_ALPHA_z_CHAR)
		  {
			  if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_READ_SMALL_ENGLISH_ALPHA_CHAR_ENABLE_FLAG, &char_type_read_enable_flag)) != SUCCESS)
			  {
			      system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("08.02.07", ERROR_OCCURED, system_status_flag);
				  return system_status_flag; 
			  }
		      if(char_type_read_enable_flag  ==  STATE_NO)		
		      {
			   	 system_status_flag = WARN_CUR_DATA_ID_SMALL_ENGLISH_ALPHA_CHAR_READ_BUT_DISABLED; 	
				 //Error_or_Warning_Proc("08.02.08", WARNING_OCCURED, system_status_flag);
		         return system_status_flag;
		      }
		      data_valid_read_char_flag = STATE_YES;
			  break; 
		  }
		  if(last_read_char >= ENGLISH_BIG_ALPHA_A_CHAR && last_read_char <= ENGLISH_BIG_ALPHA_Z_CHAR)
		  {
			  if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_READ_BIG_ENGLISH_ALPHA_CHAR_ENABLE_FLAG, &char_type_read_enable_flag)) != SUCCESS)
			  {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("08.02.09", ERROR_OCCURED, system_status_flag);
				  return system_status_flag;  
			  }
			   if(char_type_read_enable_flag  ==  STATE_NO)		
		       {
			  	   system_status_flag = WARN_CUR_DATA_ID_BIG_ENGLISH_ALPHA_CHAR_READ_BUT_DISABLED;
				   // Error_or_Warning_Proc("08.02.10", WARNING_OCCURED, system_status_flag);
		           return system_status_flag;
		       }
		       data_valid_read_char_flag = STATE_YES;
			   break;
		  }
		  if(last_read_char >= BEGIN_CTRL_CHARS_ASCII_CODE && last_read_char <= END_CTRL_CHARS_ASCII_CODE)
		  {
			  switch(last_read_char)
			   {
			       case BACKSPACE_CHAR:
			      	  // read char is backspace
					  if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_INPUT_DATA_TYPE, &input_data_type)) != SUCCESS)
					  {
						    system_status_flag = ERR_APPL_DATA_RETRIEVE;
				            Error_or_Warning_Proc("08.02.11", ERROR_OCCURED, system_status_flag);
				            return system_status_flag;  
					  }
                      switch(input_data_type)
		              {
		                  case DATA_TYPE_IN_CHAR:
						     if(cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] != NULL_CHAR)
							 {
						         cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] = NULL_CHAR; 
							 }
			              break;
			              case DATA_TYPE_IN_STR:
			                if(cur_data_id_status_para_ptr->data_value.data_str_len_or_pos > 0)
				            {
						          cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] = NULL_CHAR;
						          --cur_data_id_status_para_ptr->data_value.data_str_len_or_pos;                                      									   
							}
                            else
				            {								
				                 cur_data_id_status_para_ptr->data_value.data_str[0] = NULL_CHAR;
								 if(dev_type == DEV_RECEIVE_TYPE)
								 {
								     return COMM_NO_RCV_CHAR;
								 }
								 return SW_OR_KEY_NOT_PRESSED;
			                }
                          break;
                          default:
         		              system_status_flag = ERR_CUR_DATA_ID_INVALID_DATA_TYPE;
			                  Error_or_Warning_Proc("08.02.12", ERROR_OCCURED, system_status_flag);
			                  return ERR_DEV_OPER_READ_FUNC;  
		               }					  						  
					   
					//break;
                   case ENTER_CHAR:				    
                   break;
                   default:	
                      system_status_flag = ERR_DATA_OUT_OF_RANGE;
			          Error_or_Warning_Proc("08.02.13", ERROR_OCCURED, system_status_flag);
			          	return ERR_DEV_OPER_READ_FUNC;  				   
			   } 
			  /* if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_READ_CTRL_CHAR_ENABLE_FLAG, &char_type_read_enable_flag)) != SUCCESS)
			     {
			         system_status_flag = ERR_APPL_DATA_RETRIEVE;
				     Error_or_Warning_Proc("08.02.14", ERROR_OCCURED, system_status_flag);
				     return system_status_flag;
				 }					 
			    if(char_type_read_enable_flag  ==  STATE_NO)		
		       {
			  	   system_status_flag = WARN_CUR_DATA_ID_CTRL_CHAR_READ_BUT_DISABLED;
				 //  Error_or_Warning_Proc("08.02.15", WARNING_OCCURED, system_status_flag);
		           return system_status_flag;
		       } */			   
		       return VALID_CTRL_CHAR_READ;
			   //break;
		  }
		  else
		  {
			  if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_READ_SPECIAL_CHAR_ENABLE_FLAG, &char_type_read_enable_flag)) != SUCCESS)
			  {
			      system_status_flag = ERR_APPL_DATA_RETRIEVE;
				  Error_or_Warning_Proc("08.02.16", ERROR_OCCURED, system_status_flag);
				  return system_status_flag;
			  }
		      if(char_type_read_enable_flag  ==  STATE_NO)		
		      {
				  system_status_flag = WARN_CUR_DATA_ID_SPECIAL_CHAR_READ_BUT_DISABLED; 	
				 // Error_or_Warning_Proc("08.02.17", WARNING_OCCURED, system_status_flag);
		          return system_status_flag;
		      }
		     data_valid_read_char_flag = STATE_YES;
		    // break;	  
		  }
		  break;
          case WARN_CUR_DATA_ID_DEV_DISABLED:
		  case WARN_CUR_DATA_ID_DEV_NO_ACCESS:
		  case SW_OR_KEY_NOT_PRESSED:	
	      case COMM_NO_RCV_CHAR:
          break;
		  default:
		    system_status_flag = ERR_DEV_OPER_READ_FUNC; 	
			Error_or_Warning_Proc("08.02.18", ERROR_OCCURED, system_status_flag);
            return system_status_flag; 
	} 
	if(data_valid_read_char_flag == STATE_YES)
	{
		  ret_status = CUR_DATA_ID_VALID_CHAR_READ;
		  switch(dev_type)
		  {		
              case DEV_INPUT_TYPE:	
                 if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_INPUT_DATA_TYPE, &input_data_type)) != SUCCESS)
				 {
                     system_status_flag = ERR_APPL_DATA_RETRIEVE;
				     Error_or_Warning_Proc("08.02.19", ERROR_OCCURED, system_status_flag);
				     return system_status_flag;		
				 }				  
		         switch(input_data_type)
		         {
		             case DATA_TYPE_IN_CHAR:
			            cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] = last_read_char; 
			         break;
			         case DATA_TYPE_IN_STR:
			            cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] = last_read_char;   
                        ++cur_data_id_status_para_ptr->data_value.data_str_len_or_pos;				 
                        cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] = NULL_CHAR;							
                     break;
                     default:
         		       system_status_flag = ERR_CUR_DATA_ID_INVALID_DATA_TYPE;
			           Error_or_Warning_Proc("08.02.20", ERROR_OCCURED, system_status_flag);
			           return ERR_DEV_OPER_READ_FUNC;	   
		         }
			  break;
              case DEV_RECEIVE_TYPE:
               	 cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] = last_read_char;   
                 ++cur_data_id_status_para_ptr->data_value.data_str_len_or_pos;				 
                 cur_data_id_status_para_ptr->data_value.data_str[cur_data_id_status_para_ptr->data_value.data_str_len_or_pos] = NULL_CHAR;	
              break;
		  }			  
	} 
	return ret_status;		
}

/*------------------------------------------------------------*
FUNCTION NAME  : Data_Dev_Src_Access_Oper_Func

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.03

Bugs           :   
-*------------------------------------------------------------*/
uint16_t Data_Dev_Src_Access_Oper_Func(const uint8_t prev_data_id)
{
    const dev_id_t *data_dev_access_oper; 
	dev_id_t cur_data_devs_src_allow[1] = {CUR_DATA_ID_ALL_SRC_OF_SPEC_OPER};
    uint8_t i = 0, data_dev_type, min_dev_id, max_dev_id, max_data_dev_type_srcs;
	
	 if(cur_data_id == prev_data_id)
	 {
		  return SUCCESS;
	 }
	 
	 if(prev_data_id < NUM_DATA_IDS)
	 {
		 for(data_dev_type = DEV_INPUT_TYPE; data_dev_type < NUM_DEV_TYPES; ++data_dev_type)
		 {
			 switch(data_dev_type)
			 {
			    case DEV_INPUT_TYPE:
     	           data_dev_access_oper = data_ids_dev_srcs[prev_data_id].data_input_dev_ids;
				   min_dev_id = BEGIN_INPUT_DEV_ID;
				   max_dev_id = END_INPUT_DEV_ID;
				   max_data_dev_type_srcs = MAX_DATA_INPUT_DEVS;
			    break;
			    case DEV_OUTPUT_TYPE:
			       data_dev_access_oper = data_ids_dev_srcs[prev_data_id].data_output_dev_ids;
				   min_dev_id = BEGIN_OUTPUT_DEV_ID;
				   max_dev_id = END_OUTPUT_DEV_ID;
				   max_data_dev_type_srcs = MAX_DATA_OUTPUT_DEVS;
			    break;
			    case DEV_RECEIVE_TYPE:
			       data_dev_access_oper = data_ids_dev_srcs[prev_data_id].data_receive_dev_ids;
				   min_dev_id = BEGIN_COMM_DEV_ID;
				   max_dev_id = END_COMM_DEV_ID;
				   max_data_dev_type_srcs = MAX_DATA_RECEIVE_DEVS;
			    break;
			    case DEV_TRANSMIT_TYPE:
			       data_dev_access_oper = data_ids_dev_srcs[prev_data_id].data_transmit_dev_ids;
				   min_dev_id = BEGIN_COMM_DEV_ID;
				   max_dev_id = END_COMM_DEV_ID;
				   max_data_dev_type_srcs = MAX_DATA_TRANSMIT_DEVS;
			    break;
          default:
			      system_status_flag = ERR_FORMAT_INVALID;
			      Error_or_Warning_Proc("08.02.11", ERROR_OCCURED, system_status_flag);
			      return ERR_DEV_OPER_READ_FUNC;
		     }
            i = 0;		 
		    while(i < max_data_dev_type_srcs && (data_dev_access_oper + i)->dev_id >= min_dev_id && (data_dev_access_oper + i)->dev_id <= max_dev_id )
		    {
				  if(Dev_Oper_Func_Table[(data_dev_access_oper + i)->dev_id].Dev_No_Access_Func_Ptr == NULL_DATA_PTR)
				   {
					   system_status_flag = ERR_NULL_PTR;
					   Error_or_Warning_Proc("08.03.04", ERROR_OCCURED, system_status_flag);
                       return system_status_flag;  
				   } 
				   if(((*Dev_Oper_Func_Table[(data_dev_access_oper + i)->dev_id].Dev_No_Access_Func_Ptr)((data_dev_access_oper + i)->dev_ch_id)) != SUCCESS)
				   {
					  system_status_flag = ERR_DEV_OPER_NO_ACCESS_FUNC;
					  Error_or_Warning_Proc("08.03.03", ERROR_OCCURED, system_status_flag);
					  return system_status_flag;
				   }
				   ++i;			   
			 }
	     }
	 }		 
	 if(cur_data_id < NUM_DATA_IDS)
	 {
	    if((Cur_Data_ID_Devs_Src_Basic_Oper_Func(INPUT_DEV_ALLOW_ACCESS_OPER_FUNC, DEV_INPUT_TYPE, cur_data_devs_src_allow )) != SUCCESS)
		{
			system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
			Error_or_Warning_Proc("08.03.09", ERROR_OCCURED, system_status_flag);
			return system_status_flag;
		}
        if((Cur_Data_ID_Devs_Src_Basic_Oper_Func(OUTPUT_DEV_ALLOW_ACCESS_OPER_FUNC, DEV_OUTPUT_TYPE, cur_data_devs_src_allow )) != SUCCESS)
		{
			system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
			Error_or_Warning_Proc("08.03.10", ERROR_OCCURED, system_status_flag);
			return system_status_flag;
		}
		if((Cur_Data_ID_Devs_Src_Basic_Oper_Func(COMM_DEV_ALLOW_ACCESS_OPER_FUNC, DEV_RECEIVE_TYPE, cur_data_devs_src_allow )) != SUCCESS)
		{
			system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
			Error_or_Warning_Proc("08.03.11", ERROR_OCCURED, system_status_flag);
			return system_status_flag;
		}
		if((Cur_Data_ID_Devs_Src_Basic_Oper_Func(COMM_DEV_ALLOW_ACCESS_OPER_FUNC, DEV_TRANSMIT_TYPE, cur_data_devs_src_allow )) != SUCCESS)
		{
			system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
			Error_or_Warning_Proc("08.03.12", ERROR_OCCURED, system_status_flag);
			return system_status_flag;
		}
	 }
      return SUCCESS;    
}

/*------------------------------------------------------------*
FUNCTION NAME  : Cur_Data_ID_Devs_Src_Basic_Oper_Func

DESCRIPTION    :
								
INPUT          :

OUTPUT         : 

NOTE           : 

Func ID        : 08.04 

Bugs           :   
-*------------------------------------------------------------*/
uint16_t Cur_Data_ID_Devs_Src_Basic_Oper_Func(const uint8_t dev_src_basic_oper_func, const uint8_t dev_type, const dev_id_t *const cur_data_devs_src_allow_ptr )
{
	  const dev_id_t *data_dev_basic_oper_func_ptr;
      uint8_t i = 0, j = 0, min_dev_id, max_dev_id, max_data_dev_type_srcs, dev_src_allow_match_flag = STATE_NO;
	  
	  if(cur_data_devs_src_allow_ptr == NULL_DATA_PTR ||
      !((dev_src_basic_oper_func >= START_INPUT_DEV_BASIC_OPER_FUNC && dev_src_basic_oper_func <= END_INPUT_DEV_BASIC_OPER_FUNC) || 
		 (dev_src_basic_oper_func >= START_OUTPUT_DEV_BASIC_OPER_FUNC && dev_src_basic_oper_func <= END_OUTPUT_DEV_BASIC_OPER_FUNC) ||
	     (dev_src_basic_oper_func >= START_COMM_DEV_BASIC_OPER_FUNC && dev_src_basic_oper_func <= END_COMM_DEV_BASIC_OPER_FUNC)))
	  {
		  system_status_flag = ERR_DEV_SRC_BASIC_OPER_FUNC_INVALID;
		  Error_or_Warning_Proc("08.04.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	  }
	  
	  if(dev_src_basic_oper_func >= START_INPUT_DEV_BASIC_OPER_FUNC && dev_src_basic_oper_func <= END_INPUT_DEV_BASIC_OPER_FUNC )
	  {
		  if(dev_type != DEV_INPUT_TYPE)
		  {
			    system_status_flag = ERR_INPUT_DEV_ID_OPER_FUNC;
		        Error_or_Warning_Proc("08.04.02", ERROR_OCCURED, system_status_flag);
                return system_status_flag; 
		  }
          data_dev_basic_oper_func_ptr =  data_ids_dev_srcs[cur_data_id].data_input_dev_ids;
		  if((data_dev_basic_oper_func_ptr->dev_id < BEGIN_INPUT_DEV_ID || data_dev_basic_oper_func_ptr->dev_id > END_INPUT_DEV_ID) )
	      {
			  return SUCCESS;
	      }
	      min_dev_id = BEGIN_INPUT_DEV_ID;
		  max_dev_id = END_INPUT_DEV_ID;
		  max_data_dev_type_srcs = MAX_DATA_INPUT_DEVS;		 
	  }
	  else
	  {
	      if(dev_src_basic_oper_func >= START_OUTPUT_DEV_BASIC_OPER_FUNC && dev_src_basic_oper_func <= END_OUTPUT_DEV_BASIC_OPER_FUNC )
	      {
			 if(dev_type != DEV_OUTPUT_TYPE)
		     {
			    system_status_flag = ERR_OUTPUT_DEV_ID_OPER_FUNC;
		        Error_or_Warning_Proc("08.04.03", ERROR_OCCURED, system_status_flag);
                return system_status_flag; 
		    } 
             data_dev_basic_oper_func_ptr =  data_ids_dev_srcs[cur_data_id].data_output_dev_ids;
	         if((data_dev_basic_oper_func_ptr->dev_id < BEGIN_OUTPUT_DEV_ID || data_dev_basic_oper_func_ptr->dev_id > END_OUTPUT_DEV_ID) )
	         {
		         return SUCCESS;
	         }
		     min_dev_id = BEGIN_OUTPUT_DEV_ID;
		     max_dev_id = END_OUTPUT_DEV_ID;
		     max_data_dev_type_srcs = MAX_DATA_OUTPUT_DEVS;		 
	      }
		  else
		  {
			  switch(dev_type)
			  {
				  case DEV_RECEIVE_TYPE:      			    
				    data_dev_basic_oper_func_ptr = data_ids_dev_srcs[cur_data_id].data_receive_dev_ids;
					max_data_dev_type_srcs = MAX_DATA_RECEIVE_DEVS;
			      break;
				  case DEV_TRANSMIT_TYPE:
				     data_dev_basic_oper_func_ptr = data_ids_dev_srcs[cur_data_id].data_transmit_dev_ids; 
				     max_data_dev_type_srcs = MAX_DATA_TRANSMIT_DEVS;
				  break;	 
				  default: 	     
			        system_status_flag = ERR_COMM_DEV_ID_OPER_FUNC;
		            Error_or_Warning_Proc("08.04.04", ERROR_OCCURED, system_status_flag);
                    return system_status_flag; 
		      }			 
	         if((data_dev_basic_oper_func_ptr->dev_id < BEGIN_COMM_DEV_ID || data_dev_basic_oper_func_ptr->dev_id > END_COMM_DEV_ID))
	         {
		         return SUCCESS;  
	         }
		     min_dev_id = BEGIN_COMM_DEV_ID;
		     max_dev_id = END_COMM_DEV_ID;		    	  
		  }
	  }
	  i = 0; 
      if((cur_data_devs_src_allow_ptr + 0)->dev_id != CUR_DATA_ID_ALL_SRC_OF_SPEC_OPER)
	  {		  
	      for(j = 0; j < max_data_dev_type_srcs; ++j)
		  {
			 dev_src_allow_match_flag = STATE_NO;
		     if((cur_data_devs_src_allow_ptr + j )->dev_id == DEV_ID_NA)
			 {				
				 return SUCCESS;
			 }		 
	         while(i < max_data_dev_type_srcs && (data_dev_basic_oper_func_ptr + i)->dev_id >= min_dev_id && (data_dev_basic_oper_func_ptr + i)->dev_id <= max_dev_id )
	         {
                 if((cur_data_devs_src_allow_ptr + j )->dev_id == (data_dev_basic_oper_func_ptr + i)->dev_id )
			     {
				      if((cur_data_devs_src_allow_ptr + j )->dev_ch_id == (data_dev_basic_oper_func_ptr + i)->dev_ch_id)
				      {
					      if((Cur_Data_ID_Dev_Src_Basic_Oper_Func(dev_src_basic_oper_func, dev_type, data_dev_basic_oper_func_ptr + i)) != SUCCESS)
						  {
							  system_status_flag = ERR_DEV_SRC_BASIC_OPER_FUNC_INVALID;
							  Error_or_Warning_Proc("08.04.05", ERROR_OCCURED, system_status_flag);
							  return system_status_flag;
						  }
						  dev_src_allow_match_flag = STATE_YES;
				      }
				 }
				 if(dev_src_allow_match_flag == STATE_YES)
				 {
				     break; 		              		 
		         }
                 ++i; 
             }
			 if(dev_src_allow_match_flag == STATE_NO && (data_dev_basic_oper_func_ptr + i)->dev_id != DEV_ID_NA)
			 {
				 system_status_flag = ERR_CUR_DATA_DEV_SRC_ALLOW_NOT_SRC_LIST;
				 Error_or_Warning_Proc("08.04.06", ERROR_OCCURED, system_status_flag);
				 return system_status_flag;
			 }			       
		  }  
	  }
      else
      {
          i = 0;
		  while(i < max_data_dev_type_srcs && (data_dev_basic_oper_func_ptr + i)->dev_id >= min_dev_id && (data_dev_basic_oper_func_ptr + i)->dev_id <= max_dev_id )
	      {
              if((Cur_Data_ID_Dev_Src_Basic_Oper_Func(dev_src_basic_oper_func, dev_type, data_dev_basic_oper_func_ptr + i)) != SUCCESS)
			  {
				  system_status_flag = ERR_DEV_SRC_BASIC_OPER_FUNC_INVALID;
				  Error_or_Warning_Proc("08.04.07", ERROR_OCCURED, system_status_flag);
			      return system_status_flag;
		      } 
			  ++i;		      
		  } 
	  }	
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Cur_Data_ID_Write_Oper_Func

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.05

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Cur_Data_ID_Write_Oper_Func(const uint8_t dev_type, const dev_write_ctrl_para_t *const dev_write_ctrl_para_ptr)
{
	data_write_ctrl_ptr_t data_write_ctrl;
    const dev_id_t *data_dev_write_oper;
	uint16_t ret_status;
	uint8_t i = 0, output_data_type, max_num_write_devs, min_write_dev_id, max_write_dev_id, write_data_stream_fmt;
	
    if(dev_write_ctrl_para_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("08.05.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}		
	if(cur_data_id >= NUM_DATA_IDS)
	{
	   system_status_flag = ERR_DATA_ID_EXCEED;
	   Error_or_Warning_Proc("08.05.02", ERROR_OCCURED, system_status_flag);
	   return system_status_flag;
	}
    if( dev_type != DEV_OUTPUT_TYPE && dev_type != DEV_TRANSMIT_TYPE)
	{
		system_status_flag = ERR_WRITE_DEV_TYPE;
	    Error_or_Warning_Proc("08.05.03", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}    
	switch(dev_type) 
	{	
	   case DEV_OUTPUT_TYPE:
          if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_OUTPUT_DATA_TYPE, &output_data_type)) != SUCCESS)
	      {
		    system_status_flag = ERR_APPL_DATA_RETRIEVE;
		    Error_or_Warning_Proc("08.05.04", ERROR_OCCURED, system_status_flag);
		    return system_status_flag; 
	      }
	      if(output_data_type == DATA_TYPE_NO_DATA || output_data_type == DATA_TYPE_IN_INVALID )
	      {
		     system_status_flag = ERR_DATA_TYPE_INVALID;
		     Error_or_Warning_Proc("08.05.05", ERROR_OCCURED, system_status_flag);
	         return system_status_flag;
	      }
		  if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_OUTPUT_DATA_STREAM, &write_data_stream_fmt)) != SUCCESS)
		  {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
		          Error_or_Warning_Proc("08.05.06", ERROR_OCCURED, system_status_flag);
                  return system_status_flag; 
		  }
		  data_write_ctrl.dev_output_ctrl_para_ptr = dev_write_ctrl_para_ptr;
          data_dev_write_oper =  data_ids_dev_srcs[cur_data_id].data_output_dev_ids;
		  min_write_dev_id = BEGIN_OUTPUT_DEV_ID;
		  max_write_dev_id = END_OUTPUT_DEV_ID;
		  max_num_write_devs = MAX_DATA_OUTPUT_DEVS;
		  ret_status = CUR_DATA_ID_NO_OUTPUT_DEV;
	   break;
       case DEV_TRANSMIT_TYPE:
	      if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_TRANSMIT_DATA_STREAM, &write_data_stream_fmt)) != SUCCESS)
		  {
				  system_status_flag = ERR_APPL_DATA_RETRIEVE;
		          Error_or_Warning_Proc("08.05.07", ERROR_OCCURED, system_status_flag);
                  return system_status_flag; 
		  }
	      data_write_ctrl.dev_transmit_ctrl_para_ptr = dev_write_ctrl_para_ptr;
          data_dev_write_oper =  data_ids_dev_srcs[cur_data_id].data_transmit_dev_ids;
		  min_write_dev_id = BEGIN_COMM_DEV_ID;
		  max_write_dev_id = END_COMM_DEV_ID;
		  max_num_write_devs = MAX_DATA_TRANSMIT_DEVS;
		  ret_status = CUR_DATA_ID_NO_TRANSMIT_DEV;
       break;   	   
	}	
 // To be called when data stream is supported
  /*  if(write_data_stream_fmt == DEVICE_DATA_NA)
	{
		system_status_flag = CUR_DATA_ID_NO_STREAM;
		Error_or_Warning_Proc("08.05.08", ERROR_OCCURED, system_status_flag);
        return system_status_flag;
	} */ 
	i = 0;	   
	while(i < max_num_write_devs && (data_dev_write_oper + i)->dev_id >= min_write_dev_id && (data_dev_write_oper + i)->dev_id <= max_write_dev_id )
	{
		switch(dev_type)
		{
			case DEV_OUTPUT_TYPE:
	          if(Dev_Oper_Func_Table[(data_dev_write_oper + i)->dev_id].Dev_Output_Func_Ptr == NULL_DATA_PTR)
	          {
		         system_status_flag = ERR_NULL_PTR;
		         Error_or_Warning_Proc("08.05.09", ERROR_OCCURED, system_status_flag);
                 return system_status_flag; 
	          }
	          ret_status = (*Dev_Oper_Func_Table[(data_dev_write_oper + i)->dev_id].Dev_Output_Func_Ptr)((data_dev_write_oper + i)->dev_ch_id, &data_write_ctrl);
		   break;
           case DEV_TRANSMIT_TYPE:
		      if(Dev_Oper_Func_Table[(data_dev_write_oper + i)->dev_id].Dev_Transmit_Func_Ptr == NULL_DATA_PTR)
	          {
		         system_status_flag = ERR_NULL_PTR;
		         Error_or_Warning_Proc("08.05.10", ERROR_OCCURED, system_status_flag);
                 return system_status_flag; 
	          }
	          ret_status = (*Dev_Oper_Func_Table[(data_dev_write_oper + i)->dev_id].Dev_Transmit_Func_Ptr)((data_dev_write_oper + i)->dev_ch_id, &data_write_ctrl);
           break;		   
		}
	   switch(ret_status)
       {
           case SUCCESS:
           case WARN_CUR_DATA_ID_DEV_DISABLED: 
           case WARN_CUR_DATA_ID_DEV_NO_ACCESS: 
           break;           
           default: 
             system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
             Error_or_Warning_Proc("08.05.11", ERROR_OCCURED, system_status_flag);
		     return system_status_flag;
       } 		
	   ++i;	  
	}
	return ret_status;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Set_Data_Devs_Src_Allow

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.06

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Set_Data_Devs_Src_Allow(const uint8_t dev_type, const uint8_t data_dev_allow_type, dev_id_t *const cur_data_devs_src_allow_ptr)
{
  dev_id_t data_dev;
	uint8_t i;
	
	if((Reset_Data_Devs_Src_Allow(cur_data_devs_src_allow_ptr)) != SUCCESS)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("08.06.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(data_dev_allow_type == CUR_DATA_ID_ALL_SRC_OF_SPEC_OPER)
	{
		(cur_data_devs_src_allow_ptr + 0)->dev_id = CUR_DATA_ID_ALL_SRC_OF_SPEC_OPER;
		return SUCCESS;
	}
	switch(dev_type)
	{
		case DEV_INPUT_TYPE:
		   for(i = 0; i < MAX_DATA_INPUT_DEVS; ++i)
		   {
			  data_dev = data_ids_dev_srcs[cur_data_id].data_input_dev_ids[i];
              if(data_dev.dev_id == DEV_ID_NA)
              {
                   break;
			  }
			  *(cur_data_devs_src_allow_ptr + i) = data_dev;			   
		   }
        break;
        case DEV_OUTPUT_TYPE:
           for(i = 0; i < MAX_DATA_OUTPUT_DEVS; ++i)
		   {
			  data_dev = data_ids_dev_srcs[cur_data_id].data_output_dev_ids[i];
              if(data_dev.dev_id == DEV_ID_NA)
              {
                   break;
			  }
			  *(cur_data_devs_src_allow_ptr + i) = data_dev;	
		   }
        break;	
        case DEV_RECEIVE_TYPE:
		   for(i = 0; i < MAX_DATA_RECEIVE_DEVS; ++i)
		   {
			  data_dev = data_ids_dev_srcs[cur_data_id].data_receive_dev_ids[i];
              if(data_dev.dev_id == DEV_ID_NA)
              {
                   break;
			  }
			  *(cur_data_devs_src_allow_ptr + i) = data_dev;
		   }
        break;
		case DEV_TRANSMIT_TYPE:
		   for(i = 0; i < MAX_DATA_TRANSMIT_DEVS; ++i)
		   {
			  data_dev = data_ids_dev_srcs[cur_data_id].data_transmit_dev_ids[i];
              if(data_dev.dev_id == DEV_ID_NA)
              {
                   break;
			  }
			  *(cur_data_devs_src_allow_ptr + i) = data_dev;
		   }
        break;
        default:
		  system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("08.06.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;		
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  :  Reset_Data_Devs_Src_Allow

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.07

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Reset_Data_Devs_Src_Allow(dev_id_t *const cur_data_devs_src_allow_ptr)
{
	uint8_t i, cur_data_devs_allow_size =  MAX_NUM(MAX_NUM(MAX_DATA_INPUT_DEVS, MAX_DATA_OUTPUT_DEVS), MAX_NUM(MAX_DATA_RECEIVE_DEVS, MAX_DATA_TRANSMIT_DEVS) );
	
	if(cur_data_devs_src_allow_ptr == NULL_DATA_PTR)
	{
		 system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("08.07.01", ERROR_OCCURED, system_status_flag);
         return system_status_flag;
	}
	for(i = 0; i < cur_data_devs_allow_size; ++i)
	{
		(cur_data_devs_src_allow_ptr + i)->dev_id = DEV_ID_NA;
	}
	return SUCCESS;
}	

/*------------------------------------------------------------*
FUNCTION NAME  :  Cur_Data_ID_Dev_Src_Basic_Oper_Func

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.08

BUGS           :    
-*------------------------------------------------------------*/
static uint16_t Cur_Data_ID_Dev_Src_Basic_Oper_Func(const uint8_t dev_src_basic_oper_func, const uint8_t dev_type, const dev_id_t *const dev_basic_oper_func_ptr)
{
	uint8_t comm_dev_func = 0, comm_dev_id_offset;
	
	if(dev_basic_oper_func_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("08.08.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
    switch(dev_src_basic_oper_func)
	{
         case INPUT_DEV_DEINIT_OPER_FUNC:
	     case OUTPUT_DEV_DEINIT_OPER_FUNC:
		 case COMM_DEV_DEINIT_OPER_FUNC:
             if(Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_DeInit_Func_Ptr == NULL_DATA_PTR)
		     {
				system_status_flag = ERR_NULL_PTR;
				Error_or_Warning_Proc("08.08.02", ERROR_OCCURED, system_status_flag);
                return system_status_flag; 		        	
	        }
		    if(((*Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_DeInit_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id, DEV_DEINIT_OPER)) != SUCCESS)
		    {
		    	system_status_flag = ERR_DEV_OPER_DEINIT_FUNC;
				Error_or_Warning_Proc("08.08.03", ERROR_OCCURED, system_status_flag);
				return system_status_flag;
		    } 	
    	 break;
	     case INPUT_DEV_INIT_OPER_FUNC:
		 case OUTPUT_DEV_INIT_OPER_FUNC:
		 case COMM_DEV_INIT_OPER_FUNC:
		     if(Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Init_Func_Ptr == NULL_DATA_PTR)
		     {
		        system_status_flag = ERR_NULL_PTR;
				Error_or_Warning_Proc("08.08.04", ERROR_OCCURED, system_status_flag);
                return system_status_flag;
					
	         }
		     if(((*Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Init_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id, DEV_INIT_OPER)) != SUCCESS)
			 {
			        system_status_flag = ERR_DEV_OPER_INIT_FUNC;
					Error_or_Warning_Proc("08.08.05", ERROR_OCCURED, system_status_flag);
				    return system_status_flag;
		     } 	
		 break;
		 case INPUT_DEV_NO_ACCESS_OPER_FUNC:
		 case OUTPUT_DEV_NO_ACCESS_OPER_FUNC:
		 case COMM_DEV_NO_ACCESS_OPER_FUNC:
		    if(Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_No_Access_Func_Ptr == NULL_DATA_PTR)
		    {
				system_status_flag = ERR_NULL_PTR;
				Error_or_Warning_Proc("08.08.06", ERROR_OCCURED, system_status_flag);
                return system_status_flag;
		        		
	        }
		    if(((*Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_No_Access_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id)) != SUCCESS)
		        {
		    	      system_status_flag = ERR_DEV_OPER_NO_ACCESS_FUNC;
					  Error_or_Warning_Proc("08.08.07", ERROR_OCCURED, system_status_flag);
			          return system_status_flag;
		        } 
        break;
        case INPUT_DEV_ALLOW_ACCESS_OPER_FUNC:
		case OUTPUT_DEV_ALLOW_ACCESS_OPER_FUNC:
		case COMM_DEV_ALLOW_ACCESS_OPER_FUNC:
		    if(Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Allow_Access_Func_Ptr == NULL_DATA_PTR)
		    {
				#ifdef TRACE_ERROR
				   Print("ERR: 08.08.08 - allow access dev id - %u\r", dev_basic_oper_func_ptr->dev_id);
				#endif
				system_status_flag = ERR_NULL_PTR;				
				Error_or_Warning_Proc("08.08.08", ERROR_OCCURED, system_status_flag);
                return system_status_flag;		        		
	        }
		    if(((*Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Allow_Access_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id)) != SUCCESS)
		        {
				      system_status_flag = ERR_DEV_OPER_ALLOW_ACCESS_FUNC;
					  Error_or_Warning_Proc("08.08.09", ERROR_OCCURED, system_status_flag);
				      return system_status_flag;
		        } 
        break;
        case COMM_DEV_DISABLE_OPER_FUNC:
           comm_dev_func = 0x01;
        case INPUT_DEV_DISABLE_OPER_FUNC:
		case OUTPUT_DEV_DISABLE_OPER_FUNC:		
		   if(Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Disable_Func_Ptr == NULL_DATA_PTR)
		   {
			    system_status_flag = ERR_NULL_PTR;
				Error_or_Warning_Proc("08.08.10", ERROR_OCCURED, system_status_flag);
                return system_status_flag;		       		
	       }
		   if(((*Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Disable_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id)) != SUCCESS)
		   {
			      system_status_flag = ERR_DEV_OPER_DISABLE_FUNC;
				    Error_or_Warning_Proc("08.08.11", ERROR_OCCURED, system_status_flag);
			      return system_status_flag;
		    }
            if(comm_dev_func == 0x01)
            {
              comm_dev_id_offset = dev_basic_oper_func_ptr->dev_id  - BEGIN_COMM_DEV_ID;
			  switch(dev_type)
			  {
			     case DEV_RECEIVE_TYPE:
			       if(Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Receive_Func_Ptr == NULL_DATA_PTR)
				   {
					   system_status_flag = ERR_NULL_PTR;
				       Error_or_Warning_Proc("08.08.12", ERROR_OCCURED, system_status_flag);
                       return system_status_flag;
				   }
				   if(((*Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Receive_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id, STATE_NO)) != SUCCESS)
		           {
			             system_status_flag = ERR_DEV_OPER_DISABLE_FUNC;
				         Error_or_Warning_Proc("08.08.13", ERROR_OCCURED, system_status_flag);
			             return system_status_flag;
		           }
				 break;
				 case DEV_TRANSMIT_TYPE:
			       if(Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Transmit_Func_Ptr == NULL_DATA_PTR)
				   {
					   system_status_flag = ERR_NULL_PTR;
				       Error_or_Warning_Proc("08.08.14", ERROR_OCCURED, system_status_flag);
                       return system_status_flag;
				   }
				   if(((*Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Transmit_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id, STATE_NO)) != SUCCESS)
		           {
			             system_status_flag = ERR_DEV_OPER_DISABLE_FUNC;
				         Error_or_Warning_Proc("08.08.15", ERROR_OCCURED, system_status_flag);
			             return system_status_flag;
		           }
				 break;
				 default:
				   system_status_flag = ERR_DEV_SRC_BASIC_OPER_FUNC_INVALID;
			       Error_or_Warning_Proc("08.08.14", ERROR_OCCURED, system_status_flag);
		           return system_status_flag; 
              } 
		   }			 
        break;
        case  COMM_DEV_ENABLE_OPER_FUNC:
           comm_dev_func = 0x03;
        case  INPUT_DEV_ENABLE_OPER_FUNC:
        case  OUTPUT_DEV_ENABLE_OPER_FUNC:         
		   if(Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Enable_Func_Ptr == NULL_DATA_PTR)
		   {
			   system_status_flag = ERR_NULL_PTR;
				Error_or_Warning_Proc("08.08.13", ERROR_OCCURED, system_status_flag);
                return system_status_flag;		      		
	       }
		   if(((*Dev_Oper_Func_Table[dev_basic_oper_func_ptr->dev_id].Dev_Enable_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id)) != SUCCESS)
		   {
			      system_status_flag = ERR_DEV_OPER_ENABLE_FUNC;
				  Error_or_Warning_Proc("08.08.12", ERROR_OCCURED, system_status_flag);
			      return system_status_flag;
		   }
           if(comm_dev_func == 0x03)
           {
              comm_dev_id_offset = dev_basic_oper_func_ptr->dev_id  - BEGIN_COMM_DEV_ID;
			  switch(dev_type)
			  {
			     case DEV_RECEIVE_TYPE:
			       if(Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Receive_Func_Ptr == NULL_DATA_PTR)
				   {
					   system_status_flag = ERR_NULL_PTR;
				       Error_or_Warning_Proc("08.08.12", ERROR_OCCURED, system_status_flag);
                       return system_status_flag;
				   }
				   if(((*Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Receive_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id, STATE_YES)) != SUCCESS)
		           {
			             system_status_flag = ERR_DEV_OPER_DISABLE_FUNC;
				         Error_or_Warning_Proc("08.08.13", ERROR_OCCURED, system_status_flag);
			             return system_status_flag;
		           }
				 break;
				 case DEV_TRANSMIT_TYPE:
			       if(Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Transmit_Func_Ptr == NULL_DATA_PTR)
				   {
					   system_status_flag = ERR_NULL_PTR;
				       Error_or_Warning_Proc("08.08.14", ERROR_OCCURED, system_status_flag);
                       return system_status_flag;
				   }
				   if(((*Comm_Dev_Spec_Oper_Enable_Ctrl_Func_Table[comm_dev_id_offset].Comm_Dev_Transmit_Func_Ptr)(dev_basic_oper_func_ptr->dev_ch_id, STATE_YES)) != SUCCESS)
		           {
			             system_status_flag = ERR_DEV_OPER_DISABLE_FUNC;
				         Error_or_Warning_Proc("08.08.15", ERROR_OCCURED, system_status_flag);
			             return system_status_flag;
		           }
				 break;
				 default:
				   system_status_flag = ERR_DEV_SRC_BASIC_OPER_FUNC_INVALID;
			       Error_or_Warning_Proc("08.08.14", ERROR_OCCURED, system_status_flag);
		           return system_status_flag; 
              } 
		   }
        break;
        default:
		    system_status_flag = ERR_DEV_SRC_BASIC_OPER_FUNC_INVALID;
			Error_or_Warning_Proc("08.08.14", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;
    }
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Data_ID_Src_Spec_Dev_Find_Dev_Ch_ID

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : Specified Device should be unique, if specified Device is present in the corresponding data_ids_dev_srcs[cur_data_id] list, 
                  then Specified Device's dev ch id is returned.
                 If specified Device is not unique, then from the index 0, if specified device is present in the corresponding data_ids_dev_srcs[cur_data_id] list, 
				 then first specified Device's dev ch id is returned.

Func ID        : 08.10

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Data_ID_Src_Spec_Dev_Find_Dev_Ch_ID(const uint8_t data_id, dev_id_t *const dev_data_ptr, const uint8_t dev_type)
{
	const dev_id_t *cur_search_dev_ptr;
	uint8_t i = 0, max_data_dev_type_srcs;
	
	if(data_id >= NUM_DATA_IDS)
	{
		system_status_flag = ERR_DATA_ID_EXCEED;
	    Error_or_Warning_Proc("08.10.01", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	if((Dev_ID_Validate(dev_data_ptr, dev_type)) != SUCCESS)
	{
		system_status_flag = ERR_DEV_ID_INVALID;
	    Error_or_Warning_Proc("08.10.02", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
    switch(dev_type)
	{
       case DEV_INPUT_TYPE:
           cur_search_dev_ptr =  data_ids_dev_srcs[data_id].data_input_dev_ids;		
	       max_data_dev_type_srcs = MAX_DATA_INPUT_DEVS;	
	   break;
       case DEV_OUTPUT_TYPE:
      	cur_search_dev_ptr =  data_ids_dev_srcs[data_id].data_output_dev_ids;
	  	max_data_dev_type_srcs = MAX_DATA_OUTPUT_DEVS;
	   break;
	   case DEV_RECEIVE_TYPE:
	     cur_search_dev_ptr =  data_ids_dev_srcs[data_id].data_receive_dev_ids;
		 max_data_dev_type_srcs = MAX_DATA_RECEIVE_DEVS;
	   break;
	   case DEV_TRANSMIT_TYPE:
	     cur_search_dev_ptr =  data_ids_dev_srcs[data_id].data_transmit_dev_ids;
		 max_data_dev_type_srcs = MAX_DATA_TRANSMIT_DEVS;
	   break;	 
	}
	i = 0;  
  while( i < max_data_dev_type_srcs && ((cur_search_dev_ptr + i)->dev_id != dev_data_ptr->dev_id) )
	{
		++i;
	}
  if(i >= max_data_dev_type_srcs)
	{
	  	system_status_flag = ERR_DEV_ID_NOT_MATCH;
 	   Error_or_Warning_Proc("08.10.03", ERROR_OCCURED, system_status_flag);
      return system_status_flag;
	}
    dev_data_ptr->dev_ch_id = (cur_search_dev_ptr + i)->dev_ch_id;
	return SUCCESS;  
}

/*------------------------------------------------------------*
FUNCTION NAME  : Dev_ID_Validate

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.11

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Dev_ID_Validate(dev_id_t *const dev_data_ptr, const uint8_t dev_type)
{
	if(dev_data_ptr == NULL_DATA_PTR)
	{
		 system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("08.11.01", ERROR_OCCURED, system_status_flag);
         return system_status_flag;
	}	
	
	 if(!((dev_data_ptr->dev_id >= BEGIN_COMM_DEV_ID && dev_data_ptr->dev_id <= END_COMM_DEV_ID) || 
	  (dev_data_ptr->dev_id >= BEGIN_OUTPUT_DEV_ID && dev_data_ptr->dev_id <= END_OUTPUT_DEV_ID) ||
      (dev_data_ptr->dev_id >= BEGIN_INPUT_DEV_ID && dev_data_ptr->dev_id <= END_INPUT_DEV_ID)))
	{
		 system_status_flag = ERR_DEV_ID_INVALID;
		 Error_or_Warning_Proc("08.11.02", ERROR_OCCURED, system_status_flag);
         return system_status_flag;
	}	
	switch(dev_type)
	{
		case DEV_INPUT_TYPE:
		   if(!(dev_data_ptr->dev_id >= BEGIN_INPUT_DEV_ID && dev_data_ptr->dev_id <= END_INPUT_DEV_ID))
		   {
			    system_status_flag = ERR_INPUT_DEV_ID_OPER_FUNC;
		        Error_or_Warning_Proc("08.11.03", ERROR_OCCURED, system_status_flag);
                return system_status_flag;
		   }
		break;
		case DEV_OUTPUT_TYPE:
		   if(!(dev_data_ptr->dev_id >= BEGIN_OUTPUT_DEV_ID && dev_data_ptr->dev_id <= END_OUTPUT_DEV_ID))
		   {
			    system_status_flag = ERR_OUTPUT_DEV_ID_OPER_FUNC;
		        Error_or_Warning_Proc("08.11.04", ERROR_OCCURED, system_status_flag);
                return system_status_flag;
		   }
		break;
		case DEV_TRANSMIT_TYPE:
		case DEV_RECEIVE_TYPE:
		  if(!(dev_data_ptr->dev_id >= BEGIN_COMM_DEV_ID && dev_data_ptr->dev_id <= END_COMM_DEV_ID))
		  {
			    system_status_flag = ERR_COMM_DEV_ID_OPER_FUNC;
		        Error_or_Warning_Proc("08.11.05", ERROR_OCCURED, system_status_flag);
                return system_status_flag; 
		  }
		break;
		default:
		  system_status_flag = ERR_FORMAT_INVALID;
		  Error_or_Warning_Proc("08.11.06", ERROR_OCCURED, system_status_flag);
         return system_status_flag;
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Data_ID_Read_Try_Complete_Proc

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 08.12

BUGS           :    
-*------------------------------------------------------------*/
static uint16_t Data_ID_Read_Try_Complete_Proc(const uint8_t data_id)
{
	data_id_status_para_t *cur_data_id_status_para_ptr = NULL_DATA_PTR;
	uint8_t max_num_data_read_try;	
	
	if(data_id >= NUM_DATA_IDS)
	{
	   system_status_flag = ERR_DATA_ID_EXCEED;
	   Error_or_Warning_Proc("08.12.01", ERROR_OCCURED, system_status_flag);
	   return system_status_flag;
	}
	cur_data_id_status_para_ptr = data_id_status_para + data_id;
		
	++cur_data_id_status_para_ptr->data_read_num_try;
	#ifdef SHOULD_REMOVE			 
		    disp_trace_num_fmt.sign_flag = STATE_NO;
		    disp_trace_num_fmt.least_digits_flag = STATE_YES;
		    disp_trace_num_fmt.left_format_flag = STATE_YES;
		    disp_trace_num_fmt.num_digits = 3;
			UART_Transmit_Str(TRACE_UART_CH_ID, "\r Retry : data_id - ");
			UART_Transmit_Num(TRACE_UART_CH_ID, DATA_TYPE_IN_DECIMAL, disp_trace_num_fmt, cur_data_id );
			UART_Transmit_Str(TRACE_UART_CH_ID, " , times - ");
			UART_Transmit_Num(TRACE_UART_CH_ID, DATA_TYPE_IN_DECIMAL, disp_trace_num_fmt, cur_data_id_status_para_ptr->data_read_num_try );
	#endif
	if((Appl_Data_Retrieve_Para(cur_data_id, DATA_RETRIEVE_CTRL_MAX_NUM_DATA_READ_TRY, &max_num_data_read_try)) != SUCCESS)
	{
		system_status_flag = ERR_APPL_DATA_RETRIEVE;
		Error_or_Warning_Proc("08.12.02", ERROR_OCCURED, system_status_flag);
        return system_status_flag;
	}
	if(cur_data_id_status_para_ptr->data_read_num_try < max_num_data_read_try)
	{
		 cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag = STATE_YES;
	     return CUR_DATA_ID_UNTIL_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS;
	}
	else if(cur_data_id_status_para_ptr->data_read_num_try == max_num_data_read_try)
	{
		 cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag = STATE_YES;
	   cur_data_id_status_para_ptr->data_read_status.last_try_read_and_ready_to_process_flag = STATE_YES;
	   return CUR_DATA_ID_LAST_TRY_COMPLETE_READ_AND_READY_PROCESS;
  }					 
	cur_data_id_status_para_ptr->data_read_status.max_try_reached_flag = STATE_YES;
	cur_data_id_status_para_ptr->data_read_status.read_and_ready_to_process_flag = STATE_NO;
	cur_data_id_status_para_ptr->data_read_status.last_try_read_and_ready_to_process_flag = STATE_NO;
	return CUR_DATA_ID_READ_MAX_TRIED;
}

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
