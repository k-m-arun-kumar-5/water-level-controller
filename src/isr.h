/**************************************************************************
   FILE          :    isr.h
 
   PURPOSE       :    interrupt service routine header.  
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
	
  CHANGE LOGS     :
	   
 **************************************************************************/
 
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _ISR_H
#define _ISR_H    (11)
 
#include "constants.h"

/* ---------------------- macro defination ------------------------------------------------ */

#define VIC_VECTOR_ADDR(slot)           (VICVectAddr##slot) 
#define VIC_VECTOR_CTRL(slot)           (VICVectCntl##slot)

#define ISR_EXT_INTERRUPT(ext_interrupt_id) (uint32_t) (ISR_Ext_Interrupt_##ext_interrupt_id)
  
/* ---------------------- data type defination -------------------------------------------- */

/* DONT_CHANGE : BEGIN - dont change interrupt_request_src_t enum datas for LPC2138, as enum value is related 
                         to bit pos in peripherial interrupt request */

typedef enum 
{ 
   INTP_REQ_WDT, INTP_REQ_RESERVED_SW_INTP, INTP_REQ_ARM_CORE_CH_0, INTP_REQ_ARM_CORE_CH_1, INTP_REQ_TMR_CH_0, INTP_REQ_TMR_CH_1, INTP_REQ_UART_CH_0, INTP_REQ_UART_CH_1,
   INTP_REQ_PWM_CH_0, INTP_REQ_I2C_CH_0, INTP_REQ_SPI_CH_0, INTP_REQ_SPI_CH_1, INTP_REQ_SSP = INTP_REQ_SPI_CH_1, INTP_REQ_PLL, INTP_REQ_RTC, INTP_REQ_EXT_INTP_CH_0, 
   INTP_REQ_EXT_INTP_CH_1, INTP_REQ_EXT_INTP_CH_2, INTP_REQ_EXT_INTP_CH_3, INTP_REQ_ADC_CH_0, INTP_REQ_I2C_CH_1, INTP_REQ_BOD, INTP_REQ_ADC_CH_1, NUM_INTP_REQ_SRCS, INTP_REQ_SRC_INVALID 
} interrupt_request_src_t;

/* DONT_CHANGE : END */


typedef enum 
{ 
   VECTOR_SLOT_00,  VECTOR_SLOT_01,  VECTOR_SLOT_02,  VECTOR_SLOT_03,  VECTOR_SLOT_04,  VECTOR_SLOT_05, VECTOR_SLOT_06, VECTOR_SLOT_07, 
   VECTOR_SLOT_08,  VECTOR_SLOT_09,  VECTOR_SLOT_10,  VECTOR_SLOT_11,  VECTOR_SLOT_12,  VECTOR_SLOT_13, VECTOR_SLOT_14, VECTOR_SLOT_15,
   NUM_VECTOR_SLOTS, VECTOR_SLOT_NA   
} vector_slot_id_t;

typedef enum 
{
	EXT_INTERRUPT_LOW_LEVEL, EXT_INTERRUPT_HIGH_LEVEL, EXT_INTERRUPT_FALLING_EDGE, EXT_INTERRUPT_RISING_EDGE, EXT_INTERRUPT_SENSE_NA 
} ext_interrupt_trigger_state_t;

typedef enum 
{
	INTERRUPT_REQ_VECTOR_IRQ, INTERRUPT_REQ_FIQ, INTERRUPT_REQ_NON_VECTOR_IRQ, INTERRUPT_REQ_NA
} interrupt_req_type_t;

typedef struct 
{
	uint32_t isr_vector_irq; 
	uint16_t  enable_interrupt_request_src            : 5;    
	uint16_t  interrupt_type                          : 2;
	uint16_t  vector_irq_slot                         : 5;
    uint16_t                                          : 4;	
} interrupt_request_ctrl_t;

typedef struct 
{
  volatile void *src_interrupt_data_ptr;
  uint16_t process_interrupt_status;
  uint8_t src_interrupt_dev;  
} src_interrupt_dev_data_t;

typedef struct
{
	uint8_t ext_interrupt_src                          : 2;
  uint8_t ext_interrupt_sense                        : 3;
  uint8_t ext_interrupt_func_mode                    : 3;
	uint8_t next_triggered_irqs_while_proc_isr_flag    : 1;  // Process ISRs triggered by next successive valid vectored IRQ sources other than current processing ISR while processing ISR
	uint8_t same_triggered_irqs_while_proc_isr_flag    : 1;  // Process ISRs triggered by next successive valid vectored IRQ sources while processing ISR	
	uint8_t                                            : 6;    
} ext_interrupt_ctrl_t;

typedef struct
{
	uint8_t ext_interrupt_io_ch;	
} ext_interrupt_status_t;

/* -------------------- public variable declaration --------------------------------------- */
extern interrupt_request_ctrl_t  interrupt_request_ctrl[NUM_INTP_REQ_SRCS];
extern ext_interrupt_ctrl_t  ext_interrupt_ctrl[NUM_EXT_INTERRUPT_DEV_ID_CHS];
extern ext_interrupt_status_t  ext_interrupt_status[NUM_EXT_INTERRUPT_DEV_ID_CHS];
extern volatile src_interrupt_dev_data_t src_interrupt_dev_data[];

/* -------------------- public function declaration --------------------------------------- */
uint16_t Enable_Interrupt_Src(const uint8_t enable_interrupt_request_src); 
uint16_t Enable_Ext_Interrupt(const uint8_t ext_interrupt_id);
uint16_t Ext_Interrupt_0_Proc(void);
uint16_t Ext_Interrupt_1_Proc(void);
uint16_t Ext_Interrupt_2_Proc(void);
uint16_t Ext_Interrupt_3_Proc(void);
uint16_t Disable_Interrupt_Src(const uint8_t disable_interrupt_request_src);
uint16_t Disable_Ext_Interrupt(const uint8_t ext_interrupt_ch_id);
uint16_t Reset_All_Interrupts(const uint8_t interrupt_reset_type);
uint16_t Reset_Src_Interrupt_Data_Arr(const uint8_t src_interrupt_data_arr_index);
uint16_t Search_Src_Interrupt_Data_Arr_Free_Index(uint8_t *const src_interrupt_data_arr_free_index_ptr);
uint16_t Search_Src_Interrupt_Data_Arr_By_Src(const uint8_t search_interrupt_req_src_id, uint8_t *const src_interrupt_data_arr_search_index_ptr);
uint16_t Delete_Src_Interrupt_Data_Arr_By_Src(const uint8_t delete_interrupt_req_src_id);
uint16_t Ext_Interrupt_Retrieve_Data_Arr(const uint8_t ext_intp_ch_id);
uint16_t Dev_ID_To_Interrupt_Src_ID_Conv(uint8_t *const dev_in_interrupt_id_ptr, const dev_id_t *const io_func_dev_id_ptr);
uint16_t Interrupt_Src_ID_To_Dev_ID_Conv(dev_id_t *const dev_id_ptr, const uint8_t conv_interrupt_req_src_id);
void ISR_Non_Vector_IRQ(void) __irq;
void ISR_Timer_0(void) __irq;
void ISR_Timer_1(void) __irq;
void ISR_Watch_Dog_Timer(void) __irq;
void ISR_UART_0(void) __irq;
void ISR_UART_1(void) __irq;
void ISR_Ext_Interrupt_0(void) __irq;
void ISR_Ext_Interrupt_1(void) __irq;
void ISR_Ext_Interrupt_2(void) __irq;
void ISR_Ext_Interrupt_3(void) __irq;
void ISR_I2C_0(void) __irq;
void ISR_I2C_1(void) __irq;
void ISR_SPI_0(void) __irq;
void ISR_SPI_1(void) __irq;
void ISR_ADC_0(void) __irq;
void ISR_ADC_1(void) __irq;
void ISR_PWM_0(void) __irq;
void ISR_PLL(void) __irq;
void ISR_RTC(void) __irq;
void ISR_Brown_Out_Detect(void) __irq;

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
