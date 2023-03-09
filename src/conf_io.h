/**************************************************************************
   FILE          :    conf_io.h
 
   PURPOSE       :   main peripherial configuration Header
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _CONF_IO_H
 #define _CONF_IO_H  (4)
 
 #include "constants.h"
 
/* -------------------------------- OSC Freq conf -------------------------------------*/
 #define _XTAL_FREQ                              (12000000)
 
 /* ------------------------------ Number of Device Channels ----------------------------- */
#define NUM_COMM_DEV_ID_UART_CHS            (2)
#define NUM_COMM_DEV_ID_I2C_CHS             (2)
#define NUM_COMM_DEV_ID_SPI_CHS             (2)
#define NUM_OUTPUT_DEV_ID_PWM_CHS           (1)
#define NUM_TIMER_DEV_ID_CHS                (2)
#define NUM_EXT_INTERRUPT_DEV_ID_CHS        (4)
#define NUM_INPUT_DEV_ID_ADC_CHS            (2) 
#define NUM_WATCH_DOG_TIMER_CHS             (1) 
#define NUM_PLL_DEV_ID_CHS                  (1)
#define NUM_RTC_DEV_ID_CHS                  (1)
#define NUM_BROWN_OUT_DETECT_DEV_ID_CHS     (1)
#define NUM_ARM_CORE_DEV_ID_CHS             (2) 
#define NUM_OUTPUT_DEV_ID_DAC_CHS           (1)
#define NUM_INPUT_DEV_ID_KEYBOARD_CHS       (1)
#define NUM_INPUT_DEV_ID_SW_CHS             (8)
#define NUM_OUTPUT_DEV_ID_LED_MATRIX_CHS    (1)
#define NUM_OUTPUT_DEV_ID_LCD_CHS           (1)
#define NUM_OUTPUT_DEV_ID_SEG7_CHS          (1)

/* --------------------------------- module enable conf ---------------------------------*/ 
// #define WATCHDOG_TIMER_MOD_ENABLE               (1)
 #define TIMER_MOD_ENABLE                        (2)
// #define CAPTURE_MOD_ENABLE                      (3)
// #define RTC_MOD_ENABLE                          (4)
// #define PWM_MOD_ENABLE                          (5)
// #define ADC_MOD_ENABLE                          (6)
// #define DAC_MOD_ENABLE                          (7) 
#define EXT_INTERRUPT_MOD_ENABLE                (8)
 
 #define LCD_MOD_ENABLE                         (9)
// #define LED_MATRIX_MOD_ENABLE                  (10) 
// #define SEG7_MOD_ENABLE                        (11)

 #define SW_MOD_ENABLE                          (12)
// #define KEYBOARD_MOD_ENABLE                    (13)

 #define UART_MOD_ENABLE                        (14)
// #define SPI_MOD_ENABLE                         (15)
// #define I2C_MOD_ENABLE                         (16) 

// #define GSM_MOD_ENABLE                         (17)

#ifdef KEYBOARD_MOD_ENABLE 
  #ifndef SW_MOD_ENABLE
    #define SW_MOD_ENABLE                          (12)    
  #endif
  #include "keyboard.h"    
#endif

#ifdef SW_MOD_ENABLE 
   #include "sw_lib.h"
#endif

#ifdef SEG7_MOD_ENABLE
  #include "seg7_lib.h"
#endif
 
#ifdef LED_MATRIX_MOD_ENABLE
  #include "led_matrix.h"
#endif

#ifdef LCD_MOD_ENABLE   
    #include "lcd.h"	
#endif
 
#ifdef UART_MOD_ENABLE
  #include "uart.h"
#endif
 
#ifdef TIMER_MOD_ENABLE
  #include "timer.h"
#endif
 
/* ------------------------------------ INTERRUPT Conf ----------------------------------*/

#define MAX_NUM_INTP_PROCESS_TO_EXEC                  (10) 

/*------------------------------------ IO File Stream Conf ------------------------------*/             
#define MAX_STREAM_BUFFER_SIZE                       (50)
/* the PRINT_BUF_LEN should be enough for 32 bit int */
#define PRINT_BUF_LEN                                (12)

#define MAX_WIDTH_SPECIFIER                         (PRINT_BUF_LEN - 1)     
#define MAX_PREC_SPECIFIER                          (9)

/* ----------------------------------- Queue Conf   -------------------------------------*/
#define MAX_ELEMENTS_CIRC_QUEUE_ARR                  (20)

/* --------------------------------- Data Conv Conf -------------------------------------*/
#define NUM_CONVERT_BUFFER_LEN                       (15)

/* ----------------------------------- General Dev Conf ---------------------------------*/
#define  DEV_ID_BIT_SIZE               (5)
#define  DEV_CH_ID_BIT_SIZE            (5)

#define MAX_DATA_INPUT_DEVS                 (2)
#define MAX_DATA_OUTPUT_DEVS                (2)
#define MAX_DATA_RECEIVE_DEVS               (1) 
#define MAX_DATA_TRANSMIT_DEVS              (1)
  
/* ------------------------------- LCD oper conf ---------------------------------------*/ 
#ifdef LCD_MOD_ENABLE

#define NUM_CHARS_TRACE_CODE                      (8)
#define MAX_LCD_DIGITS                           (10) 
 
#define REQ_TIME_DELAY_LCD_WRITE_CMD_IN_MS        (5)
#define REQ_TIME_DELAY_LCD_HIDDEN_DISP_IN_MS   (1500) 
#define REQ_TIME_DELAY_LCD_VSS_4500mV_IN_MS      (20)  
#define REQ_TIME_DELAY_LCD_1_FONT_IN_MS          (10) 
#define REQ_TIME_DELAY_LCD_2_FONT_IN_MS           (1)
#define REQ_TIME_DELAY_LCD_CLEAR_SCR_IN_MS        (1) 
#define REQ_TIME_DELAY_LCD_ADDR_SETUP_IN_MS       (1)
#define REQ_TIME_DELAY_LCD_ENABLE_PULSE_IN_MS     (1)
#define REQ_TIME_DELAY_LCD_DATA_SETUP_IN_MS       (1)
#define REQ_TIME_DELAY_LCD_OTHER_INST_EXEC_IN_MS  (1)  
    
#define MAX_ICOUNT_LCD_1500MS              (2000)
#define MAX_JCOUNT_LCD_1500MS              (2000)
 
#define MAX_ICOUNT_LCD_10MS              (400)
#define MAX_JCOUNT_LCD_10MS              (400)

#define MAX_ICOUNT_LCD_1MS               (100) 
#define MAX_JCOUNT_LCD_1MS               (100)


// for JHD162A series LCD parameters
#define LCD_WRITE_ENABLE_PULSE_IN_USEC       (1)
#define LCD_ADDR_SETUP_IN_USEC               (1)
#define LCD_ENABLE_PULSE_BEFORE_WRITE_IN_USEC (1)
#define LCD_DATA_SETUP_IN_USEC               (1)
#define LCD_WRITE_ENABLE_PULSE_DELAY_IN_USEC (1)
#define LCD_CLEAR_EXEC_IN_USEC             (1650)
#define LCD_CURSOR_RETURN_EXEC_IN_USEC     (1650)
#define LCD_OTHER_INST_EXEC_IN_USEC          (40) 

#endif

/* -------------------------------- timer oper conf ------------------------------------*/
#ifdef TIMER_MOD_ENABLE
 
#define TIMER_SET_TIME_DELAY_IN_MULTIPLE                       TIMER_PRESET_TIME_DELAY_IN_MULTIPLE
#define TIMER_0_TICK_FOR_1_MILLI_SEC                                (15U)  
//in depends on Osc freq
#define TIMER_1_TICK_FOR_1_MILLI_SEC                               (15U)  
//in depends on Osc freq
#define TIMER_0_MIN_TIMEOUT_IN_MS                                    (50)
#define TIMER_1_MIN_TIMEOUT_IN_MS                                  (1000)

#endif
  
/* ------------------------------- uart oper conf -------------------------------------- */
#ifdef UART_MOD_ENABLE

#define MAX_UART_DIGITS                          (31) 
#define REQ_TIME_DELAY_UART_HIDDEN_DISP_IN_MS  (1500)

#ifdef LCD_MOD_ENABLE

#define LCD_BEGIN_LOC_ECHO_UART_RCVD_STR      (BEGIN_LOC_LINE_NUM_02)

#endif

#endif

/* ------------------------------- 7 segment LED oper conf ------------------------------*/
#ifdef  SEG7_MOD_ENABLE

#define MAX_SEG7_DIGITS                   SEG7_6_DIGIT 
#define REQ_CNT_SEG7_REPEAT_NUM                  (1)
#define REQ_TIME_DELAY_SEG7_DIGITS_IN_MS         (1)

#define MAX_ICOUNT_7SEG_SW_DELAY                 (100)
#define MAX_JCOUNT_7SEG_SW_DELAY                 (100)

#endif

/* ------------------------------- LED DOT MATRIX oper conf -----------------------------*/
#ifdef LED_MATRIX_MOD_ENABLE

#define REG_CNT_LED_MATRIX_CHAR                    (1) 
#define REQ_TIME_DELAY_LED_MATRIX_SCAN_IN_MS       (1)

#define MAX_ICOUNT_LED_MATRIX_SCAN_DELAY          (12) 
#define MAX_JCOUNT_LED_MATRIX_SCAN_DELAY          (12)
 

#endif

/* ------------------------------------ SW oper conf ------------------------------------*/
#ifdef SW_MOD_ENABLE

#define REQ_TIME_SW_DEBOUNCE_IN_MS                 (3)

#define MAX_ICOUNT_SW_DEBOUNCE_SW_DELAY           (100) 
#define MAX_JCOUNT_SW_DEBOUNCE_SW_DELAY           (100)

#endif


#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
