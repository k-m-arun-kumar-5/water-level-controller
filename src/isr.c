/* ********************************************************************
FILE                  : isr_vector_irq.c

PURPOSE               : interrupt service routine.  
	 
AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            : 

NOTE                  : 
						
CAUTION               :  
                                    
CHANGE LOGS           :  

FILE ID               : 13

*****************************************************************************/
#include "main.h"

/* ------------------------------ macro defination ------------------------------ */

/* ----------------------------- global variable defination --------------------- */
 uint32_t enable_interrupt_req_src_bitfield = 0;
 volatile src_interrupt_dev_data_t src_interrupt_dev_data[MAX_NUM_INTP_PROCESS_TO_EXEC];
 uint16_t isr_vector_irq_alloc = 0; 
 
/* ----------------------------- global variable declaration -------------------- */
#ifdef EXT_INTERRUPT_MOD_ENABLE 
   ext_interrupt_status_t  ext_interrupt_status[NUM_EXT_INTERRUPT_DEV_ID_CHS];
#endif
 
/* ----------------------------- global function declaration -------------------- */
#ifdef EXT_INTERRUPT_MOD_ENABLE  
   typedef uint16_t (*ext_intp_func_ptr_t)(void);
   ext_intp_func_ptr_t Ext_Intp_Event_Proc[NUM_EXT_INTERRUPT_DEV_ID_CHS] = {Ext_Interrupt_0_Proc, Ext_Interrupt_1_Proc, Ext_Interrupt_2_Proc, Ext_Interrupt_3_Proc};
   static uint16_t Conv_Ext_Interrupt_Ch_ID_to_IO_Ch(const uint8_t ext_interrupt_ch_id);
#endif

static uint16_t Reset_Src_Interrupt_Data_Arr_By_Specific_Index(const uint8_t src_interrupt_data_arr_index);
  
/*------------------------------------------------------------*
FUNCTION NAME  : Enable_Interrupt_Src

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.01

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Enable_Interrupt_Src(const uint8_t enable_interrupt_request_src)	  
{
	interrupt_request_ctrl_t *cur_intp_req_ctrl_ptr;
	
	if(enable_interrupt_request_src >= NUM_INTP_REQ_SRCS || enable_interrupt_request_src == INTP_REQ_RESERVED_SW_INTP)
	{
		 system_status_flag = ERR_INTERRUPT_SRCS_INVALID;
		 Error_or_Warning_Proc("13.01.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	if(enable_interrupt_req_src_bitfield & (1 << enable_interrupt_request_src ))
	{
		system_status_flag = ERR_ALREADY_ENABLED_INTERRUPT_SRC;
		Error_or_Warning_Proc("13.01.02", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
    cur_intp_req_ctrl_ptr = interrupt_request_ctrl + enable_interrupt_request_src;	
	switch(cur_intp_req_ctrl_ptr->interrupt_type)
	{
		case INTERRUPT_REQ_VECTOR_IRQ:
		   if(isr_vector_irq_alloc & (1 << cur_intp_req_ctrl_ptr->vector_irq_slot))
           {
               system_status_flag = ERR_IRQ_VECTOR_ALREADY_ALLOC;
		       Error_or_Warning_Proc("13.01.03", ERROR_OCCURED, system_status_flag);
		       return system_status_flag;
           }
		   //IRQ
		   VICIntSelect &= ~(1 << enable_interrupt_request_src);
		   switch(cur_intp_req_ctrl_ptr->vector_irq_slot)
		   {
		    	case VECTOR_SLOT_00:
			        VIC_VECTOR_ADDR(0) = cur_intp_req_ctrl_ptr->isr_vector_irq;
			        VIC_VECTOR_CTRL(0) = (1 << 5) | enable_interrupt_request_src;
			    break; 
			    case VECTOR_SLOT_01:
			       VIC_VECTOR_ADDR(1) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			       VIC_VECTOR_CTRL(1) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_02:
			      VIC_VECTOR_ADDR(2) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(2) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_03:
			       VIC_VECTOR_ADDR(3) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			       VIC_VECTOR_CTRL(3) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_04:
			      VIC_VECTOR_ADDR(4) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(4) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_05:
			       VIC_VECTOR_ADDR(5) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			       VIC_VECTOR_CTRL(5) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_06:
			       VIC_VECTOR_ADDR(6) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			       VIC_VECTOR_CTRL(6) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_07:
			      VIC_VECTOR_ADDR(7) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(7) = (1 << 5) | enable_interrupt_request_src ;
			    break;
			    case VECTOR_SLOT_08:
			      VIC_VECTOR_ADDR(8) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(8) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_09:
			      VIC_VECTOR_ADDR(9) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(9) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_10:
			      VIC_VECTOR_ADDR(10) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(10) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_11:
			       VIC_VECTOR_ADDR(11) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			       VIC_VECTOR_CTRL(11) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_12:
			       VIC_VECTOR_ADDR(12) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			       VIC_VECTOR_CTRL(12) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_13:
			       VIC_VECTOR_ADDR(13) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			       VIC_VECTOR_CTRL(13) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_14:
			      VIC_VECTOR_ADDR(14) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(14) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    case VECTOR_SLOT_15:
			      VIC_VECTOR_ADDR(15) =  cur_intp_req_ctrl_ptr->isr_vector_irq;
			      VIC_VECTOR_CTRL(15) = (1 << 5) | enable_interrupt_request_src;
			    break;
			    default:
			       system_status_flag = ERR_IRQ_VECTOR_SLOT;
		           Error_or_Warning_Proc("13.01.04", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		    }
			VICIntSelect &= ~(1 << enable_interrupt_request_src);
			isr_vector_irq_alloc |= (1 << cur_intp_req_ctrl_ptr->vector_irq_slot); 
		break;
        case INTERRUPT_REQ_FIQ:	
		    VICIntSelect |= (1 << enable_interrupt_request_src);	
        break;
        case INTERRUPT_REQ_NON_VECTOR_IRQ:
              VICIntSelect &= ~(1 << enable_interrupt_request_src);			     
        break;
        default:
              system_status_flag = ERR_FORMAT_INVALID;
		      Error_or_Warning_Proc("13.01.05", ERROR_OCCURED, system_status_flag);
		      return system_status_flag;		  
	}
	enable_interrupt_req_src_bitfield |= (1 << enable_interrupt_request_src );
    VICIntEnable |= (1 << enable_interrupt_request_src);    
    return SUCCESS;  
}

/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Non_Vector_IRQ

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.02

BUGS           :    
-*------------------------------------------------------------*/
void ISR_Non_Vector_IRQ(void) __irq
{
  	VICVectAddr = 0;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Disable_Interrupt_Src

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.03

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Disable_Interrupt_Src(const uint8_t disable_interrupt_request_src)
{
	interrupt_request_ctrl_t *cur_intp_req_ctrl_ptr;
	
	if(disable_interrupt_request_src >= NUM_INTP_REQ_SRCS || disable_interrupt_request_src == INTP_REQ_RESERVED_SW_INTP)
	{
		 system_status_flag = ERR_INTERRUPT_SRCS_INVALID;
		 Error_or_Warning_Proc("13.03.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
    cur_intp_req_ctrl_ptr = interrupt_request_ctrl + disable_interrupt_request_src;
	switch(cur_intp_req_ctrl_ptr->interrupt_type)
	{
		case INTERRUPT_REQ_VECTOR_IRQ:
		   //IRQ
           switch(cur_intp_req_ctrl_ptr->vector_irq_slot)
		   {
		    	case VECTOR_SLOT_00:
			        VIC_VECTOR_CTRL(0) = (0 << 5);
			    break; 
			    case VECTOR_SLOT_01:			       
			       VIC_VECTOR_CTRL(1) = (0 << 5);
			    break;
			    case VECTOR_SLOT_02:
			       VIC_VECTOR_CTRL(2) = (0 << 5);
			    break;
			    case VECTOR_SLOT_03:
			       VIC_VECTOR_CTRL(3) = (0 << 5) ;
			    break;
			    case VECTOR_SLOT_04:
			       VIC_VECTOR_CTRL(4) = (0 << 5);
			    break;
			    case VECTOR_SLOT_05:
			       VIC_VECTOR_CTRL(5) = (0 << 5);
			    break;
			    case VECTOR_SLOT_06:
			       VIC_VECTOR_CTRL(6) = (0 << 5);
			    break;
			    case VECTOR_SLOT_07:
			      VIC_VECTOR_CTRL(7) = (0 << 5);
			    break;
			    case VECTOR_SLOT_08:
			      VIC_VECTOR_CTRL(8) = (0 << 5);
			    break;
			    case VECTOR_SLOT_09:
			      VIC_VECTOR_CTRL(9) = (0 << 5);
			    break;
			    case VECTOR_SLOT_10:
			       VIC_VECTOR_CTRL(10) = (0 << 5);
			    break;
			    case VECTOR_SLOT_11:
			       VIC_VECTOR_CTRL(11) = (0 << 5);
			    break;
			    case VECTOR_SLOT_12:
			       VIC_VECTOR_CTRL(12) = (0 << 5);
			    break;
			    case VECTOR_SLOT_13:
			       VIC_VECTOR_CTRL(13) = (0 << 5);
			    break;
			    case VECTOR_SLOT_14:
			      VIC_VECTOR_CTRL(14) = (0 << 5);
			    break;
			    case VECTOR_SLOT_15:
			      VIC_VECTOR_CTRL(15) = (0 << 5);
			    break;
			    default:
			       system_status_flag = ERR_IRQ_VECTOR_SLOT;
		           Error_or_Warning_Proc("13.03.02", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		    }
		    isr_vector_irq_alloc &= ~(1 << cur_intp_req_ctrl_ptr->vector_irq_slot); 	
		break;
        case INTERRUPT_REQ_FIQ:	
        case INTERRUPT_REQ_NON_VECTOR_IRQ:     
        break;
        default:
            return SUCCESS;	  
	}
	VICIntEnClr |= (1 << disable_interrupt_request_src);
	enable_interrupt_req_src_bitfield &= ~(1 << disable_interrupt_request_src );
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_All_Interrupts

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.04

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Reset_All_Interrupts(const uint8_t interrupt_reset_type)
{
	uint8_t i = 0;
	
  if(interrupt_reset_type != RESET_WHOLE && interrupt_reset_type != RESET_MOD_STATUS && interrupt_reset_type != RESET_DATA_IDS_AND_APPL)
	{
		system_status_flag = ERR_FORMAT_INVALID;
		Error_or_Warning_Proc("13.04.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	Reset_Src_Interrupt_Data_Arr(MAX_NUM_INTP_PROCESS_TO_EXEC);
	if(interrupt_reset_type == RESET_WHOLE || interrupt_reset_type == RESET_MOD_STATUS)
	{
	    #ifdef EXT_INTERRUPT_MOD_ENABLE
	      for(i = 0; i < NUM_EXT_INTERRUPT_DEV_ID_CHS; ++i)
	      {
			  if(ext_interrupt_ctrl[i].ext_interrupt_func_mode <= IO_FUNC_MODE_03 )
			  {
				   Disable_Ext_Interrupt(i);	
    		  } 
	      }		 
	    #endif 
	    #ifdef TIMER_MOD_ENABLE
	       for(i = 0; i < NUM_TIMER_DEV_ID_CHS; ++i)
	       {
		       Disable_Timer_Interrupt(i);
	       }
	    #endif	
	    enable_interrupt_req_src_bitfield = 0;
	    isr_vector_irq_alloc = 0;
	}
	if(interrupt_reset_type == RESET_WHOLE)
	{
       VICDefVectAddr = (uint32_t)ISR_Non_Vector_IRQ; 
    }
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Src_Interrupt_Data_Arr_By_Specific_Index

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.05

BUGS           :    
-*------------------------------------------------------------*/
static uint16_t Reset_Src_Interrupt_Data_Arr_By_Specific_Index(const uint8_t src_interrupt_data_arr_index)
{
	if(src_interrupt_data_arr_index >= MAX_NUM_INTP_PROCESS_TO_EXEC)
	{
		system_status_flag = ERR_INTP_PROCESS_TO_EXEC_EXCEEDS;
	    Error_or_Warning_Proc("13.05.01", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	(src_interrupt_dev_data + src_interrupt_data_arr_index)->process_interrupt_status = NO_NEED_PROCESS_INTERRUPTED;
    (src_interrupt_dev_data + src_interrupt_data_arr_index)->src_interrupt_data_ptr = NULL_DATA_PTR;  
    (src_interrupt_dev_data + src_interrupt_data_arr_index)->src_interrupt_dev = INTP_REQ_SRC_INVALID;	
	
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Src_Interrupt_Data_Arr

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.06

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Reset_Src_Interrupt_Data_Arr(const uint8_t src_interrupt_data_arr_reset_index)
{
  uint8_t src_interrupt_data_arr_index; 
  
	if(src_interrupt_data_arr_reset_index > MAX_NUM_INTP_PROCESS_TO_EXEC)
	{
		  system_status_flag = ERR_INTP_PROCESS_TO_EXEC_EXCEEDS;
	    Error_or_Warning_Proc("13.06.01", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	if(src_interrupt_data_arr_reset_index == MAX_NUM_INTP_PROCESS_TO_EXEC)
	{
		for(src_interrupt_data_arr_index = 0; src_interrupt_data_arr_index < MAX_NUM_INTP_PROCESS_TO_EXEC; ++src_interrupt_data_arr_index)
		{
			Reset_Src_Interrupt_Data_Arr_By_Specific_Index(src_interrupt_data_arr_index);	
		}
	}
	else 
	{
		  Reset_Src_Interrupt_Data_Arr_By_Specific_Index(src_interrupt_data_arr_reset_index);	
	}
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Search_Src_Interrupt_Data_Arr_Free_Index

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.07

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Search_Src_Interrupt_Data_Arr_Free_Index(uint8_t *const src_interrupt_data_arr_free_index_ptr)
{
	uint8_t src_interrupt_data_arr_index;
	
	for(src_interrupt_data_arr_index = 0; src_interrupt_data_arr_index < MAX_NUM_INTP_PROCESS_TO_EXEC; ++src_interrupt_data_arr_index)
	{
			if((src_interrupt_dev_data + src_interrupt_data_arr_index)->process_interrupt_status == NO_NEED_PROCESS_INTERRUPTED)
			{
			   (src_interrupt_dev_data + src_interrupt_data_arr_index)->src_interrupt_data_ptr = NULL_DATA_PTR;  			
               (src_interrupt_dev_data + src_interrupt_data_arr_index)->src_interrupt_dev = INTP_REQ_SRC_INVALID;
			   *src_interrupt_data_arr_free_index_ptr = src_interrupt_data_arr_index;
			   return SUCCESS;
			}			   
	}
	//for src_interrupt_data_arr_index = MAX_NUM_INTP_PROCESS_TO_EXEC
	*src_interrupt_data_arr_free_index_ptr = src_interrupt_data_arr_index;
	system_status_flag = ERR_SRC_INTP_DATA_ARR_FULL;
	Error_or_Warning_Proc("13.07.01", ERROR_OCCURED, system_status_flag);
	return system_status_flag;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Search_Src_Interrupt_Data_Arr_By_Src

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 


Func ID        : 13.08

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Search_Src_Interrupt_Data_Arr_By_Src(const uint8_t search_interrupt_req_src_id, uint8_t *const src_interrupt_data_arr_search_index_ptr)
{
	uint8_t src_interrupt_data_arr_index;
	
  if(src_interrupt_data_arr_search_index_ptr == NULL_DATA_PTR)
  {
     system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("13.08.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
  }
	if(search_interrupt_req_src_id >= NUM_INTP_REQ_SRCS || search_interrupt_req_src_id == INTP_REQ_RESERVED_SW_INTP)
	{
		 system_status_flag = ERR_INTERRUPT_SRCS_INVALID;
		 Error_or_Warning_Proc("13.08.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	for(src_interrupt_data_arr_index = 0; src_interrupt_data_arr_index < MAX_NUM_INTP_PROCESS_TO_EXEC; ++src_interrupt_data_arr_index)
	{
			if((src_interrupt_dev_data + src_interrupt_data_arr_index)->src_interrupt_dev == search_interrupt_req_src_id)
			{
				*src_interrupt_data_arr_search_index_ptr = src_interrupt_data_arr_index;
				if((src_interrupt_dev_data + src_interrupt_data_arr_index)->process_interrupt_status == NO_NEED_PROCESS_INTERRUPTED)
				{
					return NO_NEED_PROCESS_INTERRUPTED;
				}
			    return SUCCESS;
			}			   
	}
	//for src_interrupt_data_arr_index = MAX_NUM_INTP_PROCESS_TO_EXEC	
	return SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Delete_Src_Interrupt_Data_Arr_By_Src

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 


Func ID        : 13.09

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Delete_Src_Interrupt_Data_Arr_By_Src(const uint8_t delete_interrupt_req_src_id)
{
	uint16_t ret_status = SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND;
	uint8_t src_interrupt_data_arr_index;
	
	if(delete_interrupt_req_src_id >= NUM_INTP_REQ_SRCS || delete_interrupt_req_src_id == INTP_REQ_RESERVED_SW_INTP)
	{
		 system_status_flag = ERR_INTERRUPT_SRCS_INVALID;
		 Error_or_Warning_Proc("13.09.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	for(src_interrupt_data_arr_index = 0; src_interrupt_data_arr_index < MAX_NUM_INTP_PROCESS_TO_EXEC; ++src_interrupt_data_arr_index)
	{
			if((src_interrupt_dev_data + src_interrupt_data_arr_index)->src_interrupt_dev == delete_interrupt_req_src_id)
			{
				if((src_interrupt_dev_data + src_interrupt_data_arr_index)->process_interrupt_status == NO_NEED_PROCESS_INTERRUPTED)
				{
					ret_status = NO_NEED_PROCESS_INTERRUPTED;
				}
				else 
				{
					if(ret_status == SEARCH_INTP_SRC_ONE_DATA_ARR)
					{
				    	ret_status = SEARCH_INTP_SRC_MORE_THAN_ONE_DATA_ARR;
					}
					else
					{
						 ret_status = SEARCH_INTP_SRC_ONE_DATA_ARR;
					}
				}
				Reset_Src_Interrupt_Data_Arr(src_interrupt_data_arr_index);
			}			   
	}
	return ret_status;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Dev_ID_To_Interrupt_Src_ID_Conv

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 


Func ID        : 13.10

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Dev_ID_To_Interrupt_Src_ID_Conv(uint8_t *const dev_in_interrupt_id_ptr, const dev_id_t *const io_func_dev_id_ptr)
{
	if(dev_in_interrupt_id_ptr == NULL_DATA_PTR || io_func_dev_id_ptr == NULL_DATA_PTR)
	{
		 system_status_flag = ERR_NULL_PTR;
		 Error_or_Warning_Proc("13.10.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	switch(io_func_dev_id_ptr->dev_id)
	{
		case COMM_DEV_ID_UART:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_UART_CH_0;
			   break;
               case CH_ID_01: 
                  *dev_in_interrupt_id_ptr = INTP_REQ_UART_CH_1;
			   break; 
               default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.02", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }				  
		break;
		case COMM_DEV_ID_I2C:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_I2C_CH_0;
			   break;
               case CH_ID_01: 
                  *dev_in_interrupt_id_ptr = INTP_REQ_I2C_CH_1;
			   break; 
               default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.03", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break; 
        case COMM_DEV_ID_SPI:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_SPI_CH_0;
			   break;
               case CH_ID_01: 
                  *dev_in_interrupt_id_ptr = INTP_REQ_SPI_CH_1;
			   break; 
               default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.04", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break; 
  		case IO_FUNC_SSP:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_SSP;
			   break;                
               default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.05", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break;  
		case OUTPUT_DEV_ID_PWM:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_PWM_CH_0;
			   break;
               default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.06", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break; 
        case IO_FUNC_TIMER:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_TMR_CH_0;
			   break;
			   case CH_ID_01:
			     *dev_in_interrupt_id_ptr = INTP_REQ_TMR_CH_1;
			   break;
               default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.07", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break;
        case IO_FUNC_EXTR_INTP:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_EXT_INTP_CH_0;
			   break;
			   case CH_ID_01:
			     *dev_in_interrupt_id_ptr = INTP_REQ_EXT_INTP_CH_1;
			   break;
			   case CH_ID_02:
			     *dev_in_interrupt_id_ptr = INTP_REQ_EXT_INTP_CH_2;
			   break;
			   case CH_ID_03:
			     *dev_in_interrupt_id_ptr = INTP_REQ_EXT_INTP_CH_3;
			   break;
               default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.08", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break; 
		case INPUT_DEV_ID_ADC:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_ADC_CH_0;
			   break;
			   case CH_ID_01:
			     *dev_in_interrupt_id_ptr = INTP_REQ_ADC_CH_1;
			   break;
			   default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.09", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break;
        case IO_FUNC_WATCH_DOG_TIMER:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_WDT;
			   break;			   
			   default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.10", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break; 
        case IO_FUNC_PLL:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_PLL;
			   break;			   
			   default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.11", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break; 
        case IO_FUNC_RTC:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_RTC;
			   break;			   
			   default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.12", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break;
        case IO_FUNC_BROWN_OUT_DETECT:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_BOD;
			   break;			   
			   default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.13", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break;
        case IO_FUNC_ARM_CORE:
		   switch(io_func_dev_id_ptr->dev_ch_id)
		   {
			   case CH_ID_00:
			     *dev_in_interrupt_id_ptr = INTP_REQ_ARM_CORE_CH_0;
			   break;
               case CH_ID_01:
			     *dev_in_interrupt_id_ptr = INTP_REQ_ARM_CORE_CH_1;
			   break;			   
			   default:
                  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		          Error_or_Warning_Proc("13.10.14", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;
		   }
		break;
        default:
           	system_status_flag = ERR_DEV_SRC_ID_INVALID;
		    Error_or_Warning_Proc("13.10.15", ERROR_OCCURED, system_status_flag);
		    return system_status_flag;	
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Interrupt_Src_ID_To_Dev_ID_Conv

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 


Func ID        : 13.11

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Interrupt_Src_ID_To_Dev_ID_Conv(dev_id_t *const dev_id_ptr, const uint8_t conv_interrupt_req_src_id)
{
	if(dev_id_ptr == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("13.11.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	switch(conv_interrupt_req_src_id)
	{
		case INTP_REQ_WDT:
		   dev_id_ptr->dev_id = IO_FUNC_WATCH_DOG_TIMER;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		case INTP_REQ_ARM_CORE_CH_0:
		   dev_id_ptr->dev_id = IO_FUNC_ARM_CORE;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		case INTP_REQ_ARM_CORE_CH_1:
		   dev_id_ptr->dev_id = IO_FUNC_ARM_CORE;
		   dev_id_ptr->dev_ch_id = CH_ID_01;
		break;
		case INTP_REQ_TMR_CH_0:
		   dev_id_ptr->dev_id = IO_FUNC_TIMER;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		case INTP_REQ_TMR_CH_1:
		   dev_id_ptr->dev_id = IO_FUNC_TIMER;
		   dev_id_ptr->dev_ch_id = CH_ID_01;
		break;
		case INTP_REQ_UART_CH_0:
		   dev_id_ptr->dev_id = COMM_DEV_ID_UART;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		case INTP_REQ_UART_CH_1:
		   dev_id_ptr->dev_id = COMM_DEV_ID_UART;
		   dev_id_ptr->dev_ch_id = CH_ID_01;
		break;
		case INTP_REQ_PWM_CH_0:
		   dev_id_ptr->dev_id = OUTPUT_DEV_ID_PWM;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break; 
		case INTP_REQ_I2C_CH_0:
		   dev_id_ptr->dev_id = COMM_DEV_ID_I2C;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break; 
		case INTP_REQ_I2C_CH_1:
		   dev_id_ptr->dev_id = COMM_DEV_ID_I2C;
		   dev_id_ptr->dev_ch_id = CH_ID_01;
		break; 
		case INTP_REQ_SPI_CH_0:
		   dev_id_ptr->dev_id = COMM_DEV_ID_SPI;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
    case INTP_REQ_SPI_CH_1:
		   dev_id_ptr->dev_id = COMM_DEV_ID_SPI;
		   dev_id_ptr->dev_ch_id = CH_ID_01;
		break; 
		case INTP_REQ_PLL:
		   dev_id_ptr->dev_id = IO_FUNC_PLL;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break; 
		case INTP_REQ_RTC:
		   dev_id_ptr->dev_id = IO_FUNC_RTC;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		case INTP_REQ_EXT_INTP_CH_0:
		   dev_id_ptr->dev_id = IO_FUNC_EXTR_INTP;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		case INTP_REQ_EXT_INTP_CH_1:
		   dev_id_ptr->dev_id = IO_FUNC_EXTR_INTP;
		   dev_id_ptr->dev_ch_id = CH_ID_01;
		break;
		case INTP_REQ_EXT_INTP_CH_2:
		   dev_id_ptr->dev_id = IO_FUNC_EXTR_INTP;
		   dev_id_ptr->dev_ch_id = CH_ID_02;
		break;
		case INTP_REQ_EXT_INTP_CH_3:
		   dev_id_ptr->dev_id = IO_FUNC_EXTR_INTP;
		   dev_id_ptr->dev_ch_id = CH_ID_03;
		break;		
		case INTP_REQ_ADC_CH_0:
		   dev_id_ptr->dev_id = INPUT_DEV_ID_ADC;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		case INTP_REQ_ADC_CH_1:
		   dev_id_ptr->dev_id = INPUT_DEV_ID_ADC;
		   dev_id_ptr->dev_ch_id = CH_ID_01;
		break;
		case INTP_REQ_BOD:
		   dev_id_ptr->dev_id = IO_FUNC_BROWN_OUT_DETECT;
		   dev_id_ptr->dev_ch_id = CH_ID_00;
		break;
		default:
		  system_status_flag = ERR_INTERRUPT_SRCS_INVALID;
		  Error_or_Warning_Proc("13.11.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;  
	}
	return SUCCESS;
}

#ifdef EXT_INTERRUPT_MOD_ENABLE
/*------------------------------------------------------------*
FUNCTION NAME  : Enable_Ext_Interrupt

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.12

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Enable_Ext_Interrupt(const uint8_t ext_interrupt_ch_id)  
{
   ext_interrupt_ctrl_t *cur_ext_intp_ctrl_ptr;
   ext_interrupt_status_t *cur_ext_intp_status_ptr;
   dev_id_t ext_interrupt_dev_id;
   uint8_t ext_interrupt_func_mode, ext_interrupt_req_src_id;
	
	if(ext_interrupt_ch_id >= NUM_EXT_INTERRUPT_DEV_ID_CHS)
	{
	       system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
	       Error_or_Warning_Proc("13.12.01", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
	}		   
	cur_ext_intp_ctrl_ptr = ext_interrupt_ctrl + ext_interrupt_ch_id;
	cur_ext_intp_status_ptr = ext_interrupt_status + ext_interrupt_ch_id;
	if(cur_ext_intp_ctrl_ptr->ext_interrupt_sense >= EXT_INTERRUPT_SENSE_NA)
	{
		 system_status_flag = ERR_FORMAT_INVALID;
		 Error_or_Warning_Proc("13.12.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag; 
	}
	ext_interrupt_dev_id.dev_id = IO_FUNC_EXTR_INTP;
	ext_interrupt_dev_id.dev_ch_id = ext_interrupt_ch_id; 
    if((Dev_ID_To_Interrupt_Src_ID_Conv(&ext_interrupt_req_src_id, &ext_interrupt_dev_id)) != SUCCESS)
	{
		system_status_flag = ERR_DEV_ID_TO_INTP_ID_CONV;
		Error_or_Warning_Proc("13.12.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag; 
	}		
    if((Conv_Ext_Interrupt_Ch_ID_to_IO_Ch(ext_interrupt_ch_id)) != SUCCESS)
    {
        system_status_flag = ERR_CONV_EXT_INTP_CH_ID_TO_IO_CH;
		Error_or_Warning_Proc("13.12.04", ERROR_OCCURED, system_status_flag);
		return system_status_flag;     		
    }
	if(cur_ext_intp_status_ptr->ext_interrupt_io_ch == IO_CH_20)
	{
		ext_interrupt_func_mode = IO_FUNC_MODE_03;
	}
	else
	{
		ext_interrupt_func_mode = cur_ext_intp_ctrl_ptr->ext_interrupt_func_mode;
	}
	if((Non_GPIO_Func_Set(cur_ext_intp_status_ptr->ext_interrupt_io_ch, ext_interrupt_func_mode )) != SUCCESS)
	{
	     system_status_flag = ERR_EXT_INTERRUPT_INIT;
	     Error_or_Warning_Proc("13.12.05", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	EXTINT = (1 << ext_interrupt_ch_id); 	
	switch(cur_ext_intp_ctrl_ptr->ext_interrupt_sense)
	{
		case EXT_INTERRUPT_LOW_LEVEL:
		   EXTMODE &= ~(1 << ext_interrupt_ch_id);
		   EXTPOLAR &= ~(1 << ext_interrupt_ch_id);
		break;
		case EXT_INTERRUPT_HIGH_LEVEL:
		    EXTMODE &= ~(1 << ext_interrupt_ch_id);
			EXTPOLAR |= (1 << ext_interrupt_ch_id);
		break;
		case EXT_INTERRUPT_FALLING_EDGE:
		    EXTMODE |= (1 << ext_interrupt_ch_id);
			EXTPOLAR &= ~(1 << ext_interrupt_ch_id);
		break;
		case EXT_INTERRUPT_RISING_EDGE:
		    EXTMODE |= (1 << ext_interrupt_ch_id);
			EXTPOLAR |= (1 << ext_interrupt_ch_id);
		break;
	}
	if((Enable_Interrupt_Src(ext_interrupt_req_src_id)) != SUCCESS)
	{
		 system_status_flag = ERR_ENABLE_INTERRUPT;
		 Error_or_Warning_Proc("13.12.06", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}  
    return SUCCESS;  
}

/*------------------------------------------------------------*
FUNCTION NAME  : Conv_Ext_Interrupt_Ch_ID_to_IO_Ch

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.19

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Conv_Ext_Interrupt_Ch_ID_to_IO_Ch(const uint8_t ext_interrupt_ch_id)  
{
	 if(ext_interrupt_ch_id >= NUM_EXT_INTERRUPT_DEV_ID_CHS)
	 {
	       system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
	       Error_or_Warning_Proc("13.19.01", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
	 }	
	 switch(ext_interrupt_ch_id)
     {
        case CH_ID_00:
	       switch(ext_interrupt_ctrl[ext_interrupt_ch_id].ext_interrupt_func_mode)
		   {
			   case IO_FUNC_MODE_01:
			       ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_16;
			   break;
			   case IO_FUNC_MODE_03:
                   ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_01; 
               break;
               default:
			      ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_INVALID; 
                  system_status_flag = ERR_NON_GPIO_FUNC_SET;
	              Error_or_Warning_Proc("13.19.02", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;    			   
		   }          
        break;
        case CH_ID_01:
		   switch(ext_interrupt_ctrl[ext_interrupt_ch_id].ext_interrupt_func_mode)
		   {
			   case IO_FUNC_MODE_02:
			      ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_14; 
			   break;
			   case IO_FUNC_MODE_03:
                   ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_03; 
               break;
               default:
			      ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_INVALID; 
                  system_status_flag = ERR_NON_GPIO_FUNC_SET;
	              Error_or_Warning_Proc("13.19.03", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;    			   
		   } 
        break;
        case CH_ID_02:
		   switch(ext_interrupt_ctrl[ext_interrupt_ch_id].ext_interrupt_func_mode)
		   {
			   case IO_FUNC_MODE_02:
			      ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_15; 
			   break;
			   case IO_FUNC_MODE_03:
                  ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_07; 
               break;
               default:
			      ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_INVALID; 
                  system_status_flag = ERR_NON_GPIO_FUNC_SET;
	              Error_or_Warning_Proc("13.19.04", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;    			   
		   }
        break;
        case CH_ID_03:
		   switch(ext_interrupt_ctrl[ext_interrupt_ch_id].ext_interrupt_func_mode)
		   {
			   case IO_FUNC_MODE_01:
			      ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_20; 
			   break;	  
			   case IO_FUNC_MODE_02:
			       ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_30; 
			   break;
			   case IO_FUNC_MODE_03:
                   ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_09; 
               break;
               default:
			      ext_interrupt_status[ext_interrupt_ch_id].ext_interrupt_io_ch = IO_CH_INVALID; 
                  system_status_flag = ERR_NON_GPIO_FUNC_SET;
	              Error_or_Warning_Proc("13.19.05", ERROR_OCCURED, system_status_flag);
		          return system_status_flag;    			   
		   }	      
        break;                    		
    }
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Disable_Ext_Interrupt

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.13

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Disable_Ext_Interrupt(const uint8_t ext_interrupt_ch_id)
{
 	ext_interrupt_status_t *cur_ext_intp_status_ptr;
	dev_id_t ext_interrupt_dev_id;
	io_config_t ext_interrupt_unconfig;	 
	uint8_t ext_interrupt_req_src_id;
	
	if(ext_interrupt_ch_id >= NUM_EXT_INTERRUPT_DEV_ID_CHS)
	{
	       system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
	       Error_or_Warning_Proc("13.13.01", ERROR_OCCURED, system_status_flag);
		   return system_status_flag;
	}
    ext_interrupt_dev_id.dev_id = IO_FUNC_EXTR_INTP;
 	ext_interrupt_dev_id.dev_ch_id = ext_interrupt_ch_id;
	if((Dev_ID_To_Interrupt_Src_ID_Conv(&ext_interrupt_req_src_id, &ext_interrupt_dev_id)) != SUCCESS)
	{
		 system_status_flag = ERR_DEV_ID_TO_INTP_ID_CONV;
	     Error_or_Warning_Proc("13.13.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	EXTINT = (1 << ext_interrupt_ch_id);    
	cur_ext_intp_status_ptr = ext_interrupt_status + ext_interrupt_ch_id;
	if((Conv_Ext_Interrupt_Ch_ID_to_IO_Ch(ext_interrupt_ch_id)) != SUCCESS)
    {
        system_status_flag = ERR_CONV_EXT_INTP_CH_ID_TO_IO_CH;
		Error_or_Warning_Proc("13.13.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;     		
    }
	ext_interrupt_unconfig.io_ch = cur_ext_intp_status_ptr->ext_interrupt_io_ch; 
	ext_interrupt_unconfig.port_pin_len = 1;
	ext_interrupt_unconfig.func = IO_FUNC_EXTR_INTP ;
	ext_interrupt_unconfig.func_type = IO_FUNC_TYPE_GPIO_NON_SW;
	if((IO_Ch_Func_Reset(&ext_interrupt_unconfig)) != SUCCESS)
	{
	     system_status_flag = ERR_IO_CH_FUNC_RESET;
	     Error_or_Warning_Proc("13.13.04", ERROR_OCCURED, system_status_flag);
         return system_status_flag;		
	}
	if((Disable_Interrupt_Src(ext_interrupt_req_src_id)) != SUCCESS)
	{
		system_status_flag = ERR_DISABLE_INTERRUPT;
	    Error_or_Warning_Proc("13.13.05", ERROR_OCCURED, system_status_flag);
        return system_status_flag;	
	}
	cur_ext_intp_status_ptr->ext_interrupt_io_ch = IO_CH_INVALID;
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Ext_Interrupt_Retrieve_Data_Arr

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.14

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Ext_Interrupt_Retrieve_Data_Arr(const uint8_t ext_intp_ch_id)
{
	  dev_id_t ext_interrupt_dev_id;
	  uint16_t ret_status;
	  uint8_t ext_intp_req_src_id, ext_interrupt_data_arr_index;
	  
      if(ext_intp_ch_id >= NUM_EXT_INTERRUPT_DEV_ID_CHS)
	  {
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_Proc("13.14.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	  }
	  ext_interrupt_dev_id.dev_id = IO_FUNC_EXTR_INTP;
	  ext_interrupt_dev_id.dev_ch_id = ext_intp_ch_id;
	  if((Dev_ID_To_Interrupt_Src_ID_Conv(&ext_intp_req_src_id, &ext_interrupt_dev_id)) != SUCCESS)
	  {
		  system_status_flag = ERR_DEV_ID_TO_INTP_ID_CONV;
		  Error_or_Warning_Proc("13.14.02", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	  }
	  ret_status = Search_Src_Interrupt_Data_Arr_By_Src(ext_intp_req_src_id, &ext_interrupt_data_arr_index);
      switch(ret_status)
	  {
		 case SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND:
		 break;
		 case SUCCESS:
		    switch((src_interrupt_dev_data + ext_interrupt_data_arr_index)->process_interrupt_status)
			{
			   case EXT_INTP_OCCURRED_YET_PROCESS:
			 	    if((Reset_Src_Interrupt_Data_Arr(ext_interrupt_data_arr_index)) != SUCCESS)
					{
						system_status_flag = ERR_RESET_SRC_INTP_DATA_ARR_PROC;
		                Error_or_Warning_Proc("13.14.03", ERROR_OCCURED, system_status_flag);
                        return system_status_flag;	
					}
					if(((*Ext_Intp_Event_Proc[ext_intp_ch_id])()) != SUCCESS)
					{
						system_status_flag = ERR_EXT_INTERRUPT_PROC;
		                Error_or_Warning_Proc("13.14.04", ERROR_OCCURED, system_status_flag);
		                return system_status_flag;
					}
			   break;
               default:
                    system_status_flag = ERR_PROCESS_INTP_STATUS_FLAG_INVALID;
		            Error_or_Warning_Proc("13.14.05", ERROR_OCCURED, system_status_flag);
                    return system_status_flag;						 
			}
		 break;
		 default:
		   system_status_flag = ERR_EXT_INTERRUPT_PROC;
		   Error_or_Warning_Proc("13.14.06", ERROR_OCCURED, system_status_flag);
           return system_status_flag;	
	 }
	 return ret_status;
}

/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Ext_Interrupt_0

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.15

BUGS           :    
-*------------------------------------------------------------*/
 void ISR_Ext_Interrupt_0(void) __irq
{
	 uint8_t src_interrupt_data_arr_free_index;
	
	if((ext_interrupt_ctrl + CH_ID_00)->same_triggered_irqs_while_proc_isr_flag == STATE_YES)
	{
		EXTINT =  (1 << 0); 	    
	}
	if((Search_Src_Interrupt_Data_Arr_Free_Index(&src_interrupt_data_arr_free_index)) != SUCCESS)
	{
		system_status_flag = ERR_SEARCH_INTP_SRC_DATA_PROC;
		Error_or_Warning_Proc("13.15.01", ERROR_OCCURED, system_status_flag);
		EXTINT =  (1 << 0); 
	}
	else
	{
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_dev = INTP_REQ_EXT_INTP_CH_0;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = EXT_INTP_OCCURRED_YET_PROCESS;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_data_ptr = NULL_DATA_PTR;
	   if((ext_interrupt_ctrl + CH_ID_00)->same_triggered_irqs_while_proc_isr_flag == STATE_NO)  
	   {
		  if((ext_interrupt_ctrl + CH_ID_00)->next_triggered_irqs_while_proc_isr_flag == STATE_YES)
		  {
	          EXTINT =  (1 << 0); 
		  }
		  else 
	      {
		      EXTINT |=  (1 << 0); 	
	      }
	   }
	}
	VICVectAddr = 0;
}

/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Ext_Interrupt_1

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.16

BUGS           :    
-*------------------------------------------------------------*/
 void ISR_Ext_Interrupt_1(void) __irq
{
	  uint8_t src_interrupt_data_arr_free_index;
	
	if((ext_interrupt_ctrl + CH_ID_01)->same_triggered_irqs_while_proc_isr_flag == STATE_YES)
	{
		EXTINT =  (1 << 1); 	    
	}
	if((Search_Src_Interrupt_Data_Arr_Free_Index(&src_interrupt_data_arr_free_index)) != SUCCESS)
	{
		system_status_flag = ERR_SEARCH_INTP_SRC_DATA_PROC;
		Error_or_Warning_Proc("13.16.01", ERROR_OCCURED, system_status_flag);
		EXTINT =  (1 << 1); 
	}
	else
	{
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_dev = INTP_REQ_EXT_INTP_CH_1;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = EXT_INTP_OCCURRED_YET_PROCESS;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_data_ptr = NULL_DATA_PTR;
	   if((ext_interrupt_ctrl + CH_ID_01)->same_triggered_irqs_while_proc_isr_flag == STATE_NO)  
	   {
		  if((ext_interrupt_ctrl + CH_ID_01)->next_triggered_irqs_while_proc_isr_flag == STATE_YES)
		  {
	          EXTINT =  (1 << 1); 
		  }
		  else 
	      {
		      EXTINT |=  (1 << 1); 	
	      }
	   }
	}
	VICVectAddr = 0;
}

/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Ext_Interrupt_2

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.17

BUGS           :    
-*------------------------------------------------------------*/
 void ISR_Ext_Interrupt_2(void) __irq
{
	 uint8_t src_interrupt_data_arr_free_index;
	
	if((ext_interrupt_ctrl + CH_ID_02)->same_triggered_irqs_while_proc_isr_flag == STATE_YES)
	{
		EXTINT = (1 << 2); 	    
	}
	if((Search_Src_Interrupt_Data_Arr_Free_Index(&src_interrupt_data_arr_free_index)) != SUCCESS)
	{
		system_status_flag = ERR_SEARCH_INTP_SRC_DATA_PROC;
		Error_or_Warning_Proc("13.17.01", ERROR_OCCURED, system_status_flag);
		EXTINT = (1 << 2); 
	}
	else
	{
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_dev = INTP_REQ_EXT_INTP_CH_2;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = EXT_INTP_OCCURRED_YET_PROCESS;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_data_ptr = NULL_DATA_PTR;
	   if((ext_interrupt_ctrl + CH_ID_02)->same_triggered_irqs_while_proc_isr_flag == STATE_NO)  
	   {
		  if((ext_interrupt_ctrl + CH_ID_02)->next_triggered_irqs_while_proc_isr_flag == STATE_YES)
		  {
	          EXTINT = (1 << 2); 
		  }
		  else 
	      {
		      EXTINT |=  (1 << 2);
	      }
	   }
	}
	VICVectAddr = 0; 
}

/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Ext_Interrupt_3

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 13.18

BUGS           :    
-*------------------------------------------------------------*/
 void ISR_Ext_Interrupt_3(void) __irq
{
	uint8_t src_interrupt_data_arr_free_index;
	
	if((ext_interrupt_ctrl + CH_ID_03)->same_triggered_irqs_while_proc_isr_flag == STATE_YES)
	{
		EXTINT =  (1 << 3); 	    
	}
	if((Search_Src_Interrupt_Data_Arr_Free_Index(&src_interrupt_data_arr_free_index)) != SUCCESS)
	{
		system_status_flag = ERR_SEARCH_INTP_SRC_DATA_PROC;
		Error_or_Warning_Proc("13.18.01", ERROR_OCCURED, system_status_flag);
		EXTINT =  (1 << 3); 
	}
	else
	{
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_dev = INTP_REQ_EXT_INTP_CH_3;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = EXT_INTP_OCCURRED_YET_PROCESS;
	   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_data_ptr = NULL_DATA_PTR;
	   if((ext_interrupt_ctrl + CH_ID_03)->same_triggered_irqs_while_proc_isr_flag == STATE_NO)  
	   {
		  if((ext_interrupt_ctrl + CH_ID_03)->next_triggered_irqs_while_proc_isr_flag == STATE_YES)
		  {
	          EXTINT =  (1 << 3); 
		  }
		  else
	      {
		      EXTINT |=  (1 << 3);
	      }
	   }	   
	}
	VICVectAddr = 0;
}

#endif




/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
