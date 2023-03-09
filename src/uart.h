/**************************************************************************
   FILE          :    uart.h
 
   PURPOSE       :      
 
   AUTHOR        :    K.M. Arun Kumar
 
  KNOWN BUGS     :
	
  NOTE           :   
	
  CHANGE LOGS     :
	   
 **************************************************************************/
 
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _UART_H
#define _UART_H  (20)

#include "constants.h"

/* ---------------------- macro defination ------------------------------------------------ */

/* ---------------------- data type defination -------------------------------------------- */

typedef enum
{
	STOP_BIT_1, STOP_BIT_1_5_OR_2
} stop_select_t;

typedef struct
{
  uint16_t baud_rate;
  uint8_t access_flag               : 1;
  uint8_t enable_flag               : 1;
  uint8_t transmit_enable_flag      : 1;
  uint8_t receive_enable_flag       : 1; 
  uint8_t echo_receive_char         : 1;
  uint8_t stop_select               : 1; 
  uint8_t                           : 2; 
  uint8_t data_bit_len              : 3;  // FOR 8 bit data bit len, then, data_bit_len = 8 - 1.  
  uint8_t error_check               : 2;   
  uint8_t                           : 3;
} uart_ctrl_t; 

typedef struct
{
	uint8_t num_chars_transmitted;
    uint8_t	num_chars_received;
	uint8_t max_num_chars_transmitted;
	uint8_t max_num_chars_received; 
	uint8_t transmit_till_end_point   : 1;
    uint8_t receive_till_end_point    : 1;
	uint8_t                           : 6;
} uart_status_t;

typedef struct
{
	disp_num_fmt_t  disp_num_fmt;
	char *uart_disp_str;
	int32_t uart_disp_num;
	uint8_t use_data_status_flag       :  1;
	uint8_t data_till_end_point_flag   :  1;
	uint8_t                            :  6;
} uart_transmit_ctrl_t;

typedef enum
{
	UART_RESET_STATUS_INIT, UART_RESET_STATUS_RECEIVE_INIT, UART_RESET_STATUS_RECEIVE_CHARS, UART_RESET_STATUS_TRANSMIT_INIT, UART_RESET_STATUS_TRANSMIT_CHARS
} uart_reset_status_t;

typedef enum 
{
	UART_SET_STATUS_RECEIVE_END_POINT_FLAG, UART_SET_STATUS_MAX_RECEIVE_CHARS, UART_SET_STATUS_TRANSMIT_END_POINT_FLAG, UART_RESET_STATUS_MAX_TRANSMIT_CHARS
} uart_set_status_t;

/* -------------------- public variable declaration --------------------------------------- */
extern uart_ctrl_t uart_ctrl[];
extern uart_status_t uart_status[];
extern file_t stdout_vt; 
extern const uint8_t internal_error_led_io_ch;


/* -------------------- public function declaration --------------------------------------- */
uint16_t UART_Init(const uint8_t uart_ch_id, const uint8_t init_mode);
uint16_t UART_Transmit_Char(const uint8_t uart_ch_id, const unsigned char uart_tx_char);
uint16_t UART_Receive_Char(const uint8_t uart_ch_id, void *const dev_input_ctrl_para_ptr);
uint16_t UART_Transmit_Num(const uint8_t uart_ch_id, const uint8_t num_data_type, const disp_num_fmt_t disp_num_fmt, const int32_t uart_num);
uint16_t UART_Transmit_Str(const uint8_t uart_ch_id, const char *const transmit_str);
uint16_t UART_Disable(const uint8_t uart_ch_id);
uint16_t UART_Enable(const uint8_t uart_ch_id);
uint16_t Disable_All_UARTs(void);
uint16_t UART_Allow_Access(const uint8_t uart_ch_id);
uint16_t UART_No_Access(const uint8_t uart_ch_id);
uint16_t UART_DeInit(const uint8_t uart_ch_id, const uint8_t deinit_mode);
uint16_t UART_Transmit_Enable_Ctrl(const uint8_t uart_ch_id, const uint8_t transmit_enable_flag);
uint16_t UART_Receive_Enable_Ctrl(const uint8_t uart_ch_id, const uint8_t receive_enable_flag);
uint16_t UART_Transmit(const uint8_t uart_ch_id, const void *const uart_transmit_ctrl_para_ptr);
uint16_t UART_Transmit_Ctrl_Proc(const uint8_t uart_ch_id, const uint8_t uart_ctrl_format, const char uart_transmit_char );
uint16_t UART_Reset_Status(const uint8_t uart_ch_id, const uint8_t uart_reset_status_type);
uint16_t UART_Set_Status(const uint8_t uart_ch_id, const uint8_t uart_set_status_type, const uint8_t set_data );
int16_t Print(const char* const format_str,...);
char Put_Char(const char to_disp_char);
int16_t Put_Str(const char *const to_disp_str);
uint16_t Error_Or_Warning_UART_Proc(const uint8_t uart_ch_id, const char *const error_trace_str, const uint8_t warn_or_error_format);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
