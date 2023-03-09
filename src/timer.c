/* ********************************************************************
FILE                   : timer.c

PURPOSE                :  Timer library 
	 
AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            : 

NOTE                  : 
						
CAUTION               :  
                                    
CHANGE LOGS           :  

FILE ID               : 12

*****************************************************************************/
#include "main.h"

#ifdef TIMER_MOD_ENABLE

/* ------------------------------ macro defination ------------------------------ */
        

/* ----------------------------- global variable defination --------------------- */
static uint8_t volatile *const tmr_ctcr_ptr[NUM_TIMER_DEV_ID_CHS] = {(uint8_t *) &T0CTCR, (uint8_t *) &T1CTCR };
static uint32_t volatile *const tmr_pr_ptr[NUM_TIMER_DEV_ID_CHS] = {(uint32_t *) &T0PR, (uint32_t *) &T1PR };
static uint8_t volatile *const tmr_tcr_ptr[NUM_TIMER_DEV_ID_CHS] = {(uint8_t *) &T0TCR, (uint8_t *) &T1TCR };
static uint32_t volatile *const tmr_tc_ptr[NUM_TIMER_DEV_ID_CHS] =   {(uint32_t *) &T0TC, (uint32_t *) &T1TC};
/*static uint32_t volatile *const tmr_mr_ptr[NUM_TIMER_DEV_ID_CHS][NUM_TIMER_MATCH_REGS] = {
	                                                                     {(uint32_t *) &T0MR0, (uint32_t *) &T0MR1, (uint32_t *) &T0MR2,(uint32_t *) &T0MR3},
                                                                         {(uint32_t *) &T1MR0, (uint32_t *) &T1MR1, (uint32_t *) &T1MR2,(uint32_t *) &T1MR3}
                                                                      }; */
static uint32_t volatile *const tmr0_mr_ptr[NUM_TIMER_MATCH_REGS] = {(uint32_t *) &T0MR0, (uint32_t *) &T0MR1, (uint32_t *) &T0MR2,(uint32_t *) &T0MR3};																	  
static uint32_t volatile *const tmr1_mr_ptr[NUM_TIMER_MATCH_REGS] = {(uint32_t *) &T1MR0, (uint32_t *) &T1MR1, (uint32_t *) &T1MR2,(uint32_t *) &T1MR3};
uint16_t volatile *const tmr_mcr_ptr[NUM_TIMER_DEV_ID_CHS] = {(uint16_t *) &T0MCR, (uint16_t *) &T1MCR};
static uint32_t volatile *const *timer_mr_ptr[NUM_TIMER_DEV_ID_CHS] = {tmr0_mr_ptr, tmr1_mr_ptr};
static uint8_t volatile *const tmr_ir_ptr[NUM_TIMER_DEV_ID_CHS] = {(uint8_t *) &T0IR, (uint8_t *) &T1IR};
volatile timer_or_counter_dev_status_t timer_or_counter_dev_status[NUM_TIMER_DEV_ID_CHS];
volatile timer_or_counter_id_status_t timer_or_counter_id_status[NUM_TIMER_AND_COUNTER_IDS];
volatile timer_or_counter_data_t timer_or_counter_interrupt_data[NUM_TIMER_DEV_ID_CHS];
timer_0_ctrl_t timer_0_ctrl;

/* ----------------------------- global variable declaration -------------------- */


/* ----------------------------- global function declaration -------------------- */
uint32_t volatile *const *Tmr_Match_Ptr(const uint8_t timer_id);
static uint16_t Reset_Timer_Data_Specific_ID(const uint8_t timer_data_id);

/* ----------------------------- function pointer defination -------------------- */
typedef uint16_t (*timer_timeout_func_ptr_t)(const uint16_t timeout_type, volatile const timer_or_counter_data_t *const timer_or_counter_data_ptr);
timer_timeout_func_ptr_t Timer_Timeout_Proc_Func_Ptr[NUM_TIMER_DEV_ID_CHS] = {Timer_0_Timeout_Proc, Timer_1_Timeout_Proc}; 

/*------------------------------------------------------------*
FUNCTION NAME  : Timer_Run

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz.
                 Used only by timer_ch_id > CH_ID_00 && timer_ch_id < NUM_TIMER_DEV_ID_CHS

Func ID        : 12.01

BUGS           :    
-*------------------------------------------------------------*/
 uint16_t Timer_Run(const uint8_t timer_ch_id, const uint8_t run_timer_run_id)
{
	volatile timer_or_counter_dev_status_t *cur_timer_or_counter_dev_status_ptr;
	volatile timer_or_counter_id_status_t *cur_timer_or_counter_id_status_ptr;
	timer_or_counter_id_ctrl_t  *cur_timer_or_counter_id_ctrl_ptr;
	uint16_t timeout_type, actual_timeout_in_ms_or_max_count, timer_min_timeout_in_ms, ret_status;
	uint8_t timer_tick, timer_remainder, max_count_per_timeout;
	
	if(run_timer_run_id >= NUM_TIMER_AND_COUNTER_IDS || timer_ch_id == CH_ID_00)
	{
		 system_status_flag = ERR_TIMER_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.01.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	if((Timer_Stop(timer_ch_id)) != SUCCESS)
    {
         system_status_flag = ERR_TIMER_STOP_PROC;
		 Error_or_Warning_Proc("12.01.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag; 
    }	  
	cur_timer_or_counter_id_ctrl_ptr = timer_or_counter_id_ctrl + run_timer_run_id;
	cur_timer_or_counter_id_status_ptr = timer_or_counter_id_status + run_timer_run_id;	
	cur_timer_or_counter_dev_status_ptr = timer_or_counter_dev_status + timer_ch_id;
	cur_timer_or_counter_id_status_ptr->timer_or_counter_id = run_timer_run_id;
	if(cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id >= NUM_TIMER_MATCH_REGS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.01.03", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	cur_timer_or_counter_dev_status_ptr->timer_or_counter_last_run_id = cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id =  run_timer_run_id;
 	if(cur_timer_or_counter_id_ctrl_ptr->mode_type == TIMER_MODE)
	{
		    switch(timer_ch_id)
		    {
		    	case CH_ID_01:		
			        timer_tick = TIMER_1_TICK_FOR_1_MILLI_SEC;
				      timer_min_timeout_in_ms =  TIMER_1_MIN_TIMEOUT_IN_MS;
			    break;
		     }
			 timer_remainder = cur_timer_or_counter_id_ctrl_ptr->conf_timeout_in_ms_or_max_count % timer_min_timeout_in_ms;			 
		     if(timer_remainder == 0)
		     {
		      	actual_timeout_in_ms_or_max_count = cur_timer_or_counter_id_ctrl_ptr->conf_timeout_in_ms_or_max_count;
		     } 
             else
		     {			 
		          // set_timer_req_time_delay_in_ms is not in multiples of timer_min_timeout_in_ms
		          #if TIMER_SET_TIME_DELAY_IN_MULTIPLE == TIMER_PRESET_TIME_DELAY_IN_MULTIPLE
		             //actual_timeout_in_ms_or_max_count is previous valid req_time_delay_in_milli_sec, which is multiple of timer_min_timeout_in_ms
		            actual_timeout_in_ms_or_max_count = cur_timer_or_counter_id_ctrl_ptr->conf_timeout_in_ms_or_max_count - timer_remainder;
		          #else //TIMER_SET_TIME_DELAY_IN_MULTIPLE == TIMER_POSTSET_TIME_DELAY_IN_MULTIPLE
		             //actual_timeout_in_ms_or_max_count is next valid req_time_delay_in_milli_sec, which is multiple of timer_min_timeout_in_ms
                     actual_timeout_in_ms_or_max_count =  cur_timer_or_counter_id_ctrl_ptr->conf_timeout_in_ms_or_max_count + timer_min_timeout_in_ms - timer_remainder ;	
                  #endif	
		     }
			 max_count_per_timeout = actual_timeout_in_ms_or_max_count / timer_min_timeout_in_ms;
			 #ifdef TRACE_DATA
			   uint32_temp_disp_data = actual_timeout_in_ms_or_max_count;
			   Print("TRA: actual_timeout_in_ms : %u \r",uint32_temp_disp_data );
			 #endif
			 if(max_count_per_timeout == 0)
			 {
				   system_status_flag = ERR_TIMER_TIMEOUT_LESS_THAN_MIN;
		           Error_or_Warning_Proc("12.01.04", ERROR_OCCURED, system_status_flag);
		           return system_status_flag;
			 }
			 
			 cur_timer_or_counter_id_status_ptr->max_count_per_timeout = max_count_per_timeout;
			 cur_timer_or_counter_id_status_ptr->actual_timeout_in_ms_or_max_count = actual_timeout_in_ms_or_max_count;
			 cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts = 0;
			 cur_timer_or_counter_id_status_ptr->elapsed_count_per_timeout = 0;
			 cur_timer_or_counter_id_status_ptr->timer_max_num_timeout_occured_flag = STATE_NO;
			 *tmr_ctcr_ptr[timer_ch_id] = 0x00; // Timer mode, increment on every rising edge 
	         *tmr_pr_ptr[timer_ch_id] = (timer_tick * 1000 - 1) ; // Load Pre-Scalar counter with timer_tick (0 to 14 = 15), so that timer counts every 1msec 
			 *tmr_mcr_ptr[timer_ch_id] = 0;
			 //TC and PC will reset, if MR matches the TC.
			 *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
			 *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));			
		     //*timer_mr_ptr[timer_ch_id][cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id] = cur_timer_or_counter_id_status_ptr->actual_timeout_in_ms_or_max_count;
			 *timer_mr_ptr[timer_ch_id][cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id] = timer_min_timeout_in_ms; 			 
             if(cur_timer_or_counter_id_ctrl_ptr->match_interrupt_or_capture_rising_flag == STATE_YES)
	         {
				  //EVENT_INTERRUPT
              if( cur_timer_or_counter_id_status_ptr->max_count_per_timeout == 1)
              {     
                	 //	commented due to timer running infinitely bug, without interrupting on TC match on Match_Register_id and process ISR_Timer_1()	    
				     /* 
					  *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
				        	//TC and PC will stop and TCR[0]= 0 if MR matches the TC.					
				       *tmr_mcr_ptr[timer_ch_id] |= (1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id))); 
					*/
                      *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                      // TC and PC will reset and TCR[0]= 0 if MR matches the TC.					
                      *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));						
              }
              else
              {                 
                  *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                // TC and PC will reset and TCR[0]= 0 if MR matches the TC.					
                  *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
              }
			  *tmr_mcr_ptr[timer_ch_id] |= (1 << (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id));	
				
				 if(cur_timer_or_counter_dev_status_ptr->exec_timer_interrupt_enable_proc == STATE_NO)
				 {
				    if((Enable_Interrupt_Src(INTP_REQ_TMR_CH_0 + timer_ch_id)) != SUCCESS)
	                {
	            	    system_status_flag = ERR_ENABLE_INTERRUPT;
	            	    Error_or_Warning_Proc("12.01.05", ERROR_OCCURED, system_status_flag);
		                return system_status_flag;
	                }
				    cur_timer_or_counter_dev_status_ptr->exec_timer_interrupt_enable_proc = STATE_YES;
				 }
				 cur_timer_or_counter_dev_status_ptr->timer_or_counter_state = TMR_OR_COUNTER_RUNNING_STATE;
				*tmr_tcr_ptr[timer_ch_id] = (1 << 0);  		  // timer run
	         }
             else
			 {
				// EVENT_POLLING 
          if(cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts != NO_MAX_NUM_TIMEOUTS_OR_COUNT)  
          {            
              if(cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts == 1)
              {                
				      	 *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
				        	//TC and PC will stop and TCR[0]= 0 if MR matches the TC.					
				        	*tmr_mcr_ptr[timer_ch_id] |= (1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
                       timeout_type = TMR_AT_LAST_TIMEOUT_YET_PROCESS;						
              }
              else
              {                 
                  *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                // TC and PC will reset and TCR[0]= 0 if MR matches the TC.					
                  *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
                  timeout_type = TMR_BEFORE_LAST_TIMEOUT_YET_PROCESS;				  
              }
         }
		 else
		 {
			 timeout_type = TMR_NO_MAX_NUM_TIMEOUT_YET_PROCESS;
		 }
        *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id));	  
		cur_timer_or_counter_dev_status_ptr->timer_or_counter_state = TMR_OR_COUNTER_RUNNING_STATE;
		 *tmr_tcr_ptr[timer_ch_id] = (1 << 0);  		  // Enable timer 
				do 
				{
         
             while(*tmr_tc_ptr[timer_ch_id] != *timer_mr_ptr[timer_ch_id][cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id]);			       
            if(cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts != NO_MAX_NUM_TIMEOUTS_OR_COUNT)
            {
             	++cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts;
            }
            (timer_or_counter_interrupt_data + timer_ch_id)->timer_or_counter_run_id = cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;          
            (timer_or_counter_interrupt_data + timer_ch_id)->elapsed_num_timeouts_or_count = cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts;
		    ret_status = (*Timer_Timeout_Proc_Func_Ptr[timer_ch_id])(timeout_type, (timer_or_counter_interrupt_data + timer_ch_id));
			switch(ret_status)
			{
				case TMR_NO_MAX_NUM_TIMEOUT_PROC:
				case TMR_BEFORE_LAST_TIMEOUT_PROC:
			    case TMR_AT_LAST_TIMEOUT_PROC:
			    case TMR_MAX_NUM_TIMEOUT_PROC:				
				break;
			    default:

				    Timer_Stop(timer_ch_id);
				   	system_status_flag = ERR_TIMEOUT_PROC;
		            Error_or_Warning_Proc("12.01.06", ERROR_OCCURED, system_status_flag);
		           return system_status_flag;
			}
					if(cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts != NO_MAX_NUM_TIMEOUTS_OR_COUNT)
			        {
					   	if(cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts == cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts - 1)
				    	{
					         *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                            // TC and PC will reset and TCR[0]= 0 if MR matches the TC.					
                             *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                             timeout_type = TMR_AT_LAST_TIMEOUT_YET_PROCESS;	    					    
					    }						

			        }
			    } while(cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts == NO_MAX_NUM_TIMEOUTS_OR_COUNT || cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts < cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts);
          if(cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts != NO_MAX_NUM_TIMEOUTS_OR_COUNT) 
          {
               Timer_Stop(timer_ch_id); 
			        cur_timer_or_counter_id_status_ptr->timer_max_num_timeout_occured_flag = STATE_YES;
          }
		}
	 }
	 return SUCCESS;	
}
 
/*------------------------------------------------------------*
FUNCTION NAME  : Timer_Stop

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz 

Func ID        : 12.02

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Timer_Stop(const uint8_t timer_ch_id )
{	
  volatile timer_or_counter_dev_status_t *cur_timer_or_counter_dev_status_ptr;
  volatile timer_or_counter_id_ctrl_t  *cur_timer_or_counter_id_ctrl_ptr;
  
	if(timer_ch_id >= NUM_TIMER_DEV_ID_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.02.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	/* When one, the Timer Counter and the Prescale Counter are synchronously reset on the next positive edge of
               PCLK. The counters remain reset until TCR[1] is returned to zero. */  
	*tmr_tcr_ptr[timer_ch_id] = (1 << 0) | (1 << 1);  
    *tmr_tcr_ptr[timer_ch_id] = (1 << 1); //reset
    *tmr_tcr_ptr[timer_ch_id] = 0; // Stop timer 
	switch(timer_ch_id)
	{
		 case CH_ID_00:
		   *tmr_ir_ptr[timer_ch_id] = (1 << timer_0_ctrl.match_ch_id); 
		   *tmr_mcr_ptr[timer_ch_id] &= ~(7 << (3 * timer_0_ctrl.match_ch_id));			
         break;	
		case CH_ID_01:
            cur_timer_or_counter_dev_status_ptr = timer_or_counter_dev_status + timer_ch_id;
            if(cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id == TMR_OR_COUNTER_STOP_ID)
            {
			    return SUCCESS;
            }
			if(cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id == TMR_OR_COUNTER_PAUSE_ID)
			{
				cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id = TMR_OR_COUNTER_STOP_ID;
		        cur_timer_or_counter_id_ctrl_ptr = timer_or_counter_id_ctrl + cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
                cur_timer_or_counter_dev_status_ptr->timer_or_counter_last_run_id = cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
                return SUCCESS;  				
			}
            if( cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id >= NUM_TIMER_AND_COUNTER_IDS )
        	{
	         	 system_status_flag = ERR_TIMER_ID_EXCEEDS;
	         	 Error_or_Warning_Proc("12.02.02", ERROR_OCCURED, system_status_flag);
	        	 return system_status_flag;
	        }
	        cur_timer_or_counter_id_ctrl_ptr = timer_or_counter_id_ctrl + cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
            cur_timer_or_counter_dev_status_ptr->timer_or_counter_last_run_id = cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
			*tmr_ir_ptr[timer_ch_id] = (1 << cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id);
            *tmr_mcr_ptr[timer_ch_id] &= ~(7 << (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id));	           
           cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id = TMR_OR_COUNTER_STOP_ID;		  
		break;       
	}
	 cur_timer_or_counter_dev_status_ptr->timer_or_counter_state = TMR_OR_COUNTER_STOPPED_STATE;
    return SUCCESS; 
}


/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Timer_0

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz 

Func ID        : 12.03

BUGS           :    
-*------------------------------------------------------------*/
void ISR_Timer_0(void) __irq
{
	uint8_t src_interrupt_data_arr_free_index;
	
	if((Search_Src_Interrupt_Data_Arr_Free_Index(&src_interrupt_data_arr_free_index)) != SUCCESS)
	{
		 Timer_Stop(CH_ID_00);
         system_status_flag = ERR_SEARCH_INTP_SRC_DATA_PROC;
		 Error_or_Warning_Proc("12.03.01", ERROR_OCCURED, system_status_flag);		 
	}
    else
	{		
        (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_dev = INTP_REQ_TMR_CH_0;
		(src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = TMR_NO_MAX_NUM_TIMEOUT_YET_PROCESS;
		(timer_or_counter_interrupt_data + CH_ID_00)->elapsed_num_timeouts_or_count = NO_MAX_NUM_TIMEOUTS_OR_COUNT;
		(src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_data_ptr = (volatile void *) (timer_or_counter_interrupt_data + CH_ID_00);
	}
	*tmr_ir_ptr[CH_ID_00] = (1 << timer_0_ctrl.match_ch_id); //Write back to IR to clear Interrupt Flag	    
  	VICVectAddr = 0x0; //This is to signal end of interrupt execution
}

/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Timer_1

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz 

Func ID        : 12.04

BUGS           :    
-*------------------------------------------------------------*/
void ISR_Timer_1(void) __irq
{
	timer_or_counter_id_ctrl_t  *cur_timer_or_counter_id_ctrl_ptr;	
	volatile timer_or_counter_dev_status_t *cur_timer_or_counter_dev_status_ptr;
	volatile timer_or_counter_id_status_t *cur_timer_or_counter_id_status_ptr;
	uint8_t src_interrupt_data_arr_free_index;
	
	cur_timer_or_counter_dev_status_ptr = timer_or_counter_dev_status + CH_ID_01;	
	if(cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id == TMR_OR_COUNTER_STOP_ID || cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id == TMR_OR_COUNTER_PAUSE_ID || cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id >= NUM_TIMER_AND_COUNTER_IDS)
	{
		Timer_Stop(CH_ID_01);		 
	}
	else
	{
		cur_timer_or_counter_id_ctrl_ptr = timer_or_counter_id_ctrl + cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
		cur_timer_or_counter_id_status_ptr = timer_or_counter_id_status + cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
        if((Search_Src_Interrupt_Data_Arr_Free_Index(&src_interrupt_data_arr_free_index)) != SUCCESS)
		{
			 Timer_Stop(CH_ID_01);
			 system_status_flag = ERR_SEARCH_INTP_SRC_DATA_PROC;
		     Error_or_Warning_Proc("12.04.01", ERROR_OCCURED, system_status_flag);
		}
        else
		{	
            ++cur_timer_or_counter_id_status_ptr->elapsed_count_per_timeout;
            if(cur_timer_or_counter_id_status_ptr->elapsed_count_per_timeout >= cur_timer_or_counter_id_status_ptr->max_count_per_timeout)
			{
				 cur_timer_or_counter_id_status_ptr->elapsed_count_per_timeout = 0;							
                (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_dev = INTP_REQ_TMR_CH_1;
			    (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = TMR_NO_MAX_NUM_TIMEOUT_YET_PROCESS;
			    (timer_or_counter_interrupt_data + CH_ID_01)->timer_or_counter_run_id = cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
			    (timer_or_counter_interrupt_data + CH_ID_01)->elapsed_num_timeouts_or_count = NO_MAX_NUM_TIMEOUTS_OR_COUNT;
			    if(cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts != NO_MAX_NUM_TIMEOUTS_OR_COUNT)
		        {
			    	++cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts;
				    (timer_or_counter_interrupt_data + CH_ID_01)->elapsed_num_timeouts_or_count = cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts;
				    if(cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts < cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts - 1)
				    {
                        (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = TMR_BEFORE_LAST_TIMEOUT_YET_PROCESS;				   
                    }
				    else if(cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts == cur_timer_or_counter_id_ctrl_ptr->max_num_timeouts - 1)
				    {
				  	   //commented due to timer running infinitely bug, without interrupting on TC match on Match_Register_id and process ISR_Timer_1()	  
				        /* 
						 *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
				         //TC and PC will stop and TCR[0]= 0 if MR matches the TC.					
				         *tmr_mcr_ptr[timer_ch_id] |= (1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id))); 
					   */	 
					   (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = TMR_AT_LAST_TIMEOUT_YET_PROCESS; 
				    }              
                    else
                    {
                       Timer_Stop(CH_ID_01);						
				       cur_timer_or_counter_id_status_ptr->timer_max_num_timeout_occured_flag = STATE_YES; 
				      (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->process_interrupt_status = TMR_MAX_TIMEOUT_YET_PROCESS;
		            }					    
		        }
		        (src_interrupt_dev_data + src_interrupt_data_arr_free_index)->src_interrupt_data_ptr = (volatile void *)(timer_or_counter_interrupt_data + CH_ID_01);
		     }
		     *tmr_ir_ptr[CH_ID_01] = (1 << cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id); //Write back to IR to clear Interrupt Flag
	    }
	}		
	VICVectAddr = 0x0; //This is to signal end of interrupt execution
}

/*------------------------------------------------------------*
FUNCTION NAME  : Disable_Timer_Interrupt

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 12.05

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Disable_Timer_Interrupt(const uint8_t timer_ch_id)
{
	volatile timer_or_counter_dev_status_t *cur_timer_or_counter_dev_status_ptr;
   
	if(timer_ch_id >= NUM_TIMER_DEV_ID_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.05.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
    if((Timer_Stop(timer_ch_id)) != SUCCESS)
    {
         system_status_flag = ERR_TIMER_STOP_PROC;
		 Error_or_Warning_Proc("12.05.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag; 
    }
    switch(timer_ch_id)
    {
	   case CH_ID_01:
	   	 cur_timer_or_counter_dev_status_ptr = timer_or_counter_dev_status + timer_ch_id;			
         cur_timer_or_counter_dev_status_ptr->exec_timer_interrupt_enable_proc = STATE_NO;
	   break;	
    }
    if((Disable_Interrupt_Src(INTP_REQ_TMR_CH_0 + timer_ch_id)) != SUCCESS)
    {
       system_status_flag = ERR_DISABLE_INTERRUPT;
	   Error_or_Warning_Proc("12.05.03", ERROR_OCCURED, system_status_flag);
	   return system_status_flag; 
    }
    return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Timer_0_Run

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz 

Func ID        : 12.06

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Timer_0_Run(const uint8_t match_ch_id, const uint16_t appl_proc_exec_conf_time_interval_in_ms)
{
	uint8_t timer_remainder;
	
	if(match_ch_id >= NUM_TIMER_MATCH_REGS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.06.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	if((Timer_Stop(CH_ID_00)) != SUCCESS)
    {
         system_status_flag = ERR_TIMER_STOP_PROC;
		 Error_or_Warning_Proc("12.06.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag; 
    }
	timer_0_ctrl.match_ch_id = match_ch_id;
	 *tmr_ir_ptr[CH_ID_00] = (1 << timer_0_ctrl.match_ch_id); //Write back to IR to clear Interrupt Flag
    timer_remainder = appl_proc_exec_conf_time_interval_in_ms % TIMER_0_MIN_TIMEOUT_IN_MS;
    if(timer_remainder == 0)
	{
	   	timer_0_ctrl.appl_proc_exec_time_interval_in_ms = appl_proc_exec_conf_time_interval_in_ms;
	} 
    else
	{			 
	     // set_timer_req_time_delay_in_ms is not in multiples of TIMER_0_MIN_TIMEOUT_IN_MS
	     #if TIMER_SET_TIME_DELAY_IN_MULTIPLE == TIMER_PRESET_TIME_DELAY_IN_MULTIPLE
	       //appl_proc_exec_conf_time_interval_in_ms is previous valid req_time_delay_in_milli_sec, which is multiple of TIMER_0_MIN_TIMEOUT_IN_MS
	        timer_0_ctrl.appl_proc_exec_time_interval_in_ms = appl_proc_exec_conf_time_interval_in_ms - timer_remainder;
		 #else //TIMER_SET_TIME_DELAY_IN_MULTIPLE == TIMER_POSTSET_TIME_DELAY_IN_MULTIPLE
		    //appl_proc_exec_time_interval_in_ms is next valid req_time_delay_in_milli_sec, which is multiple of TIMER_0_MIN_TIMEOUT_IN_MS
             timer_0_ctrl.appl_proc_exec_time_interval_in_ms =  appl_proc_exec_conf_time_interval_in_ms + TIMER_0_MIN_TIMEOUT_IN_MS - timer_remainder ;	
         #endif	
	}
	if((timer_0_ctrl.appl_proc_exec_time_interval_in_ms / TIMER_0_MIN_TIMEOUT_IN_MS) == 0)
	{
		system_status_flag = ERR_TIMER_TIMEOUT_LESS_THAN_MIN;
		Error_or_Warning_Proc("12.06.03", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	*tmr_ctcr_ptr[CH_ID_00] = 0x00; // Timer mode, increment on every rising edge 
	*tmr_pr_ptr[CH_ID_00] = (TIMER_0_TICK_FOR_1_MILLI_SEC * 1000 - 1) ; // Load Pre-Scalar counter with TIMER_0_TICK_FOR_1_MILLI_SEC (0 to 14 = 15), so that timer counts every 1msec 
	*tmr_mcr_ptr[CH_ID_00] = 0;
	//TC and PC will reset, if MR matches the TC.
	 *tmr_mcr_ptr[CH_ID_00] |= (1 << (1 + (3 * timer_0_ctrl.match_ch_id)));
	 *timer_mr_ptr[CH_ID_00][match_ch_id] = timer_0_ctrl.appl_proc_exec_time_interval_in_ms;
     *tmr_mcr_ptr[CH_ID_00] &= ~(1 << (2 + (3 * timer_0_ctrl.match_ch_id)));	
     // TC and PC will reset and TCR[0]= 0 if MR matches the TC.					
     *tmr_mcr_ptr[CH_ID_00] |= (1 << (1 + (3 * timer_0_ctrl.match_ch_id)));	
     *tmr_mcr_ptr[CH_ID_00] |= (1 << (3 * timer_0_ctrl.match_ch_id));	
	 if((Enable_Interrupt_Src(INTP_REQ_TMR_CH_0 + CH_ID_00)) != SUCCESS)
	 {
	   	    system_status_flag = ERR_ENABLE_INTERRUPT;
	   	    Error_or_Warning_Proc("12.06.04", ERROR_OCCURED, system_status_flag);
	        return system_status_flag;

	 }
	 *tmr_tcr_ptr[CH_ID_00] = (1 << 0);  		  // timer run
	 return SUCCESS;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Timer_Interrupt_Retrieve_Data_Arr

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz 

Func ID        : 12.07

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Timer_Interrupt_Retrieve_Data_Arr(const uint8_t timer_ch_id, timer_or_counter_data_t *const retrieved_timer_or_counter_interrupt_data_ptr)
{
	  dev_id_t timer_dev_id;
	 uint16_t ret_status, timeout_type;
	 uint8_t timer_intp_req_src_id, timer_interrupt_data_arr_index;
	  
    if(retrieved_timer_or_counter_interrupt_data_ptr == NULL_DATA_PTR)
    {
        system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("12.07.01", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
    }
	  if(timer_ch_id >= NUM_TIMER_DEV_ID_CHS)
	  {
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.07.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	  }
	  timer_dev_id.dev_id = IO_FUNC_TIMER;
	  timer_dev_id.dev_ch_id = timer_ch_id;
	  if((Dev_ID_To_Interrupt_Src_ID_Conv(&timer_intp_req_src_id, &timer_dev_id)) != SUCCESS)
	  {
		  system_status_flag = ERR_DEV_ID_TO_INTP_ID_CONV;
		  Error_or_Warning_Proc("12.07.03", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	  }
      ret_status = Search_Src_Interrupt_Data_Arr_By_Src(timer_intp_req_src_id, &timer_interrupt_data_arr_index);
      switch(ret_status)
	  {
		 case SEARCH_INTP_SRC_DATA_ARR_NOT_FOUND:
		 break;
		 case SUCCESS:
		    timeout_type = (src_interrupt_dev_data + timer_interrupt_data_arr_index)->process_interrupt_status;
		    switch(timeout_type)
			{
			   case TMR_NO_MAX_NUM_TIMEOUT_YET_PROCESS:
			   case TMR_BEFORE_LAST_TIMEOUT_YET_PROCESS:
			   case TMR_AT_LAST_TIMEOUT_YET_PROCESS:
			   case TMR_MAX_TIMEOUT_YET_PROCESS:
			        memcpy(retrieved_timer_or_counter_interrupt_data_ptr, (timer_or_counter_data_t *)((src_interrupt_dev_data + timer_interrupt_data_arr_index)->src_interrupt_data_ptr), sizeof(timer_or_counter_data_t));
					if((ret_status = Reset_Src_Interrupt_Data_Arr(timer_interrupt_data_arr_index)) != SUCCESS)
					{
						system_status_flag = ERR_RESET_SRC_INTP_DATA_ARR_PROC;
		                Error_or_Warning_Proc("12.07.04", ERROR_OCCURED, system_status_flag);
                        return system_status_flag;	
					}
					ret_status = (*Timer_Timeout_Proc_Func_Ptr[timer_ch_id])(timeout_type, retrieved_timer_or_counter_interrupt_data_ptr);
					switch(ret_status)
					{
						case TMR_NO_MAX_NUM_TIMEOUT_PROC:
				        case TMR_BEFORE_LAST_TIMEOUT_PROC:
			            case TMR_AT_LAST_TIMEOUT_PROC:
			            case TMR_MAX_NUM_TIMEOUT_PROC:	
						break;
						default:
						    Timer_Stop(timer_ch_id);
					      	system_status_flag = ERR_TIMEOUT_PROC;
		                    Error_or_Warning_Proc("12.07.05", ERROR_OCCURED, system_status_flag);
		                    return system_status_flag;
					}
			   break;
               default:
                    system_status_flag = ERR_PROCESS_INTP_STATUS_FLAG_INVALID;
		            Error_or_Warning_Proc("12.07.06", ERROR_OCCURED, system_status_flag);
                    return system_status_flag;						 
			}
		 break;
		 default:
		   system_status_flag = ERR_TIMEOUT_PROC;
		   Error_or_Warning_Proc("12.07.07", ERROR_OCCURED, system_status_flag);
           return system_status_flag;	
	 }
	 return ret_status;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Timer_Pause

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz 

Func ID        : 12.08

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Timer_Pause(const uint8_t timer_ch_id)
{	
  volatile timer_or_counter_dev_status_t *cur_timer_or_counter_dev_status_ptr;
  timer_or_counter_id_ctrl_t  *cur_timer_or_counter_id_ctrl_ptr;
  
	if(timer_ch_id >= NUM_TIMER_DEV_ID_CHS)
	{
		 system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.08.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	/* When one, the Timer Counter and the Prescale Counter are synchronously reset on the next positive edge of
               PCLK. The counters remain reset until TCR[1] is returned to zero. */  
	*tmr_tcr_ptr[timer_ch_id] = (1 << 0) | (1 << 1);  
    *tmr_tcr_ptr[timer_ch_id] = (1 << 1); //reset
    *tmr_tcr_ptr[timer_ch_id] = 0; // Stop timer 
	switch(timer_ch_id)
	{
		 case CH_ID_00:
		   *tmr_ir_ptr[timer_ch_id] = (1 << timer_0_ctrl.match_ch_id); 
		   *tmr_mcr_ptr[timer_ch_id] &= ~(7 << (3 * timer_0_ctrl.match_ch_id));			
         break;	
		case CH_ID_01:
            cur_timer_or_counter_dev_status_ptr = timer_or_counter_dev_status + timer_ch_id;
            if(cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id == TMR_OR_COUNTER_STOP_ID)
            {
			     system_status_flag = ERR_TIMER_ALREADY_STOPPED_IN_PAUSE_PROC;
	         	 Error_or_Warning_Proc("12.08.02", ERROR_OCCURED, system_status_flag);
	        	 return system_status_flag;
            }
			if(cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id == TMR_OR_COUNTER_PAUSE_ID)
            {
				return SUCCESS;
            }
            if(cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id >= NUM_TIMER_AND_COUNTER_IDS )
        	{
	         	 system_status_flag = ERR_TIMER_ID_EXCEEDS;
	         	 Error_or_Warning_Proc("12.08.03", ERROR_OCCURED, system_status_flag);
	        	 return system_status_flag;

	        }
	        cur_timer_or_counter_id_ctrl_ptr = timer_or_counter_id_ctrl + cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
            cur_timer_or_counter_dev_status_ptr->timer_or_counter_last_run_id = cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id;
			*tmr_ir_ptr[timer_ch_id] = (1 << cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id);
            *tmr_mcr_ptr[timer_ch_id] &= ~(7 << (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id));	           
            cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id = TMR_OR_COUNTER_PAUSE_ID;
		break;       
	}
	cur_timer_or_counter_dev_status_ptr->timer_or_counter_state = TMR_OR_COUNTER_PAUSED_STATE;
    return SUCCESS; 
}

/*------------------------------------------------------------*
FUNCTION NAME  : Timer_Resume

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz.
                 Used only by timer_ch_id > CH_ID_00 && timer_ch_id < NUM_TIMER_DEV_ID_CHS

Func ID        : 12.09

BUGS           :    
-*------------------------------------------------------------*/
 uint16_t Timer_Resume(const uint8_t timer_ch_id, const uint8_t resume_timer_run_id)
{
	volatile timer_or_counter_dev_status_t *cur_timer_or_counter_dev_status_ptr;
	volatile timer_or_counter_id_status_t *cur_timer_or_counter_id_status_ptr;
	timer_or_counter_id_ctrl_t  *cur_timer_or_counter_id_ctrl_ptr;
	uint16_t timer_min_timeout_in_ms;
	uint8_t timer_tick;
	
	if(resume_timer_run_id >= NUM_TIMER_AND_COUNTER_IDS || timer_ch_id == CH_ID_00)
	{
		 system_status_flag = ERR_TIMER_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.09.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	cur_timer_or_counter_id_ctrl_ptr = timer_or_counter_id_ctrl + resume_timer_run_id;
	if(cur_timer_or_counter_dev_status_ptr->timer_or_counter_state != TMR_OR_COUNTER_PAUSED_STATE)
	{
		 system_status_flag = ERR_TIMER_NOT_IN_PAUSE_STATE_IN_RESUME;
		 Error_or_Warning_Proc("12.09.02", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	cur_timer_or_counter_dev_status_ptr = timer_or_counter_dev_status + timer_ch_id;	
	cur_timer_or_counter_dev_status_ptr->timer_or_counter_last_run_id = cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id =  resume_timer_run_id;
	cur_timer_or_counter_id_status_ptr = timer_or_counter_id_status + resume_timer_run_id;
 	if(cur_timer_or_counter_id_ctrl_ptr->mode_type == TIMER_MODE)
	{
		    switch(timer_ch_id)
		    {
		    	case CH_ID_01:		
			       timer_tick = TIMER_1_TICK_FOR_1_MILLI_SEC;
             timer_min_timeout_in_ms = TIMER_1_MIN_TIMEOUT_IN_MS;
			    break;
		     }
			 *tmr_ctcr_ptr[timer_ch_id] = 0x00; // Timer mode, increment on every rising edge 
	         *tmr_pr_ptr[timer_ch_id] = (timer_tick * 1000 - 1) ; // Load Pre-Scalar counter with timer_tick (0 to 14 = 15), so that timer counts every 1msec 
			 *tmr_mcr_ptr[timer_ch_id] = 0;
			 //TC and PC will reset, if MR matches the TC.
			 *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
			 *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));			
		     //*timer_mr_ptr[timer_ch_id][cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id] = cur_timer_or_counter_id_status_ptr->actual_timeout_in_ms_or_max_count;
			 *timer_mr_ptr[timer_ch_id][cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id] = timer_min_timeout_in_ms; 			 
             if(cur_timer_or_counter_id_ctrl_ptr->match_interrupt_or_capture_rising_flag == STATE_YES)
	         {
				  //EVENT_INTERRUPT
                 if( cur_timer_or_counter_id_status_ptr->elapsed_count_per_timeout + 1 == cur_timer_or_counter_id_status_ptr->max_count_per_timeout)
                 {     
                	 //	commented due to timer running infinitely bug, without interrupting on TC match on Match_Register_id and process ISR_Timer_1()	    
				     /* 
					  *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));
				        	//TC and PC will stop and TCR[0]= 0 if MR matches the TC.					
				       *tmr_mcr_ptr[timer_ch_id] |= (1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id))); 
					*/
                      *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                      // TC and PC will reset and TCR[0]= 0 if MR matches the TC.					
                      *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));						
                 }
                 else
                 {                 
                     *tmr_mcr_ptr[timer_ch_id] &= ~(1 << (2 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                      // TC and PC will reset and TCR[0]= 0 if MR matches the TC.					
                     *tmr_mcr_ptr[timer_ch_id] |= (1 << (1 + (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id)));	
                 }
			     *tmr_mcr_ptr[timer_ch_id] |= (1 << (3 * cur_timer_or_counter_id_ctrl_ptr->match_or_capture_id));
			     cur_timer_or_counter_dev_status_ptr->timer_or_counter_state = TMR_OR_COUNTER_RUNNING_STATE;
			     *tmr_tcr_ptr[timer_ch_id] = (1 << 0);  		  // timer run
	         }
	 }
	 return SUCCESS;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_All_Timer_Chs

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 12.10

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Reset_All_Timer_Chs(const uint8_t timer_reset_type)
{
	uint8_t timer_ch_id;
	
	if(timer_reset_type != RESET_WHOLE && timer_reset_type != RESET_MOD_STATUS)
	{
		system_status_flag = ERR_FORMAT_INVALID;
	    Error_or_Warning_Proc("12.10.01", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	for(timer_ch_id = 0; timer_ch_id < NUM_TIMER_DEV_ID_CHS; ++timer_ch_id)
	{
		Reset_Timer_Ch(timer_ch_id, timer_reset_type);
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Timer_Ch

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 12.11

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Reset_Timer_Ch(const uint8_t timer_ch_id, const uint8_t timer_reset_type)
{
	volatile timer_or_counter_dev_status_t *cur_timer_or_counter_dev_status_ptr;
	dev_id_t timer_dev_id;
	uint8_t timer_intp_req_src_id;
	
	if(timer_ch_id >= NUM_TIMER_DEV_ID_CHS)
	{
		  system_status_flag = ERR_DEV_CH_ID_EXCEEDS;
		  Error_or_Warning_Proc("12.11.01", ERROR_OCCURED, system_status_flag);
		  return system_status_flag;
	}
	if(timer_reset_type != RESET_WHOLE && timer_reset_type != RESET_MOD_STATUS)
	{
		system_status_flag = ERR_FORMAT_INVALID;
	    Error_or_Warning_Proc("12.11.02", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	if((Disable_Timer_Interrupt(timer_ch_id)) != SUCCESS)
	{
		system_status_flag = ERR_DISABLE_INTERRUPT;
	    Error_or_Warning_Proc("12.11.03", ERROR_OCCURED, system_status_flag);
	    return system_status_flag;
	}
	cur_timer_or_counter_dev_status_ptr = timer_or_counter_dev_status + timer_ch_id;
	timer_dev_id.dev_id = IO_FUNC_TIMER;
	timer_dev_id.dev_ch_id = timer_ch_id;
	if((Dev_ID_To_Interrupt_Src_ID_Conv(&timer_intp_req_src_id, &timer_dev_id)) != SUCCESS)
	{
		system_status_flag = ERR_DEV_ID_TO_INTP_ID_CONV;
		Error_or_Warning_Proc("12.11.04", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if((Delete_Src_Interrupt_Data_Arr_By_Src(timer_intp_req_src_id)) != SUCCESS)
	{
		system_status_flag = ERR_DELETE_DATA_BY_INTP_SRC_PROC;
		Error_or_Warning_Proc("12.11.05", ERROR_OCCURED, system_status_flag);
		return system_status_flag;
	}
	if(timer_reset_type == RESET_WHOLE)
	{
		cur_timer_or_counter_dev_status_ptr->timer_or_counter_cur_run_id = TMR_OR_COUNTER_STOP_ID;
		cur_timer_or_counter_dev_status_ptr->timer_or_counter_last_run_id = TMR_OR_COUNTER_STOP_ID;
		switch(timer_ch_id)
		{
			case CH_ID_00:
			   timer_0_ctrl.appl_proc_exec_time_interval_in_ms = 0;
			   timer_0_ctrl.match_ch_id = TIMER_MATCH_REG_NA;
			break;
			case CH_ID_01:
			   Reset_Timer_Data_ID(NUM_TIMER_AND_COUNTER_IDS);
			break;   
		}
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Timer_Data_ID

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 12.12

BUGS           :    
-*------------------------------------------------------------*/
uint16_t Reset_Timer_Data_ID(const uint8_t timer_data_id)
{
	uint8_t i;
	
	if(timer_data_id > NUM_TIMER_AND_COUNTER_IDS)
	{
		 system_status_flag = ERR_TIMER_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.12.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	if(timer_data_id == NUM_TIMER_AND_COUNTER_IDS)
	{
		for(i = 0; i < NUM_TIMER_AND_COUNTER_IDS; ++i)
		{
			Reset_Timer_Data_Specific_ID(i);
		}
	}
	else
	{
		Reset_Timer_Data_Specific_ID(timer_data_id);
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Reset_Timer_Data_Specific_ID

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 12.12

BUGS           :    
-*------------------------------------------------------------*/
static uint16_t Reset_Timer_Data_Specific_ID(const uint8_t timer_data_id)
{
	volatile timer_or_counter_id_status_t *cur_timer_or_counter_id_status_ptr;
	uint8_t i;
	
	if(timer_data_id >= NUM_TIMER_AND_COUNTER_IDS)
	{
		 system_status_flag = ERR_TIMER_ID_EXCEEDS;
		 Error_or_Warning_Proc("12.12.01", ERROR_OCCURED, system_status_flag);
		 return system_status_flag;
	}
	cur_timer_or_counter_id_status_ptr = timer_or_counter_id_status + timer_data_id;
	cur_timer_or_counter_id_status_ptr->actual_timeout_in_ms_or_max_count = 0;
	cur_timer_or_counter_id_status_ptr->elapsed_time_in_ms_or_count = 0;
	cur_timer_or_counter_id_status_ptr->max_count_per_timeout = 0;
	cur_timer_or_counter_id_status_ptr->elapsed_count_per_timeout = 0;
	cur_timer_or_counter_id_status_ptr->elapsed_num_timeouts = 0;
	cur_timer_or_counter_id_status_ptr->timer_max_num_timeout_occured_flag = STATE_NO;	
	return SUCCESS;
}

#endif

#ifdef WATCHDOG_TIMER_MOD_ENABLE
/*------------------------------------------------------------*
FUNCTION NAME  : ISR_Watch_Dog_Timer

DESCRIPTION    :
								
INPUT          : 

OUTPUT         : 

NOTE           : XTAL_FREQ = 12MHz, CCLK = 60 MHz, PCLK = 15MHz 

Func ID        : 12.13

BUGS           :    
-*------------------------------------------------------------*/
void ISR_Watch_Dog_Timer(void) __irq
{
	VICVectAddr = 0x0;
}

#endif 

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
