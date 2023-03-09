/**************************************************************************
   FILE          :    conf_appl.h
 
   PURPOSE       :   main application configuration Header
 
   AUTHOR        :    K.M. Arun Kumar
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _CONF_APPL_H
 #define _CONF_APPL_H   (2)
 
 #include "constants.h"
  
/* -------------------------------- Debug Conf -----------------------------------------*/
//#define TRACE                                 (1U)
#define TRACE_ERROR                             (2U)
#define TRACE_REQ                               (3U)
#define TRACE_INFO                              (4U)
//#define TRACE_DATA                            (5U)
//#define TRACE_FLOW                            (6U)

#define TRACE_LCD_CH_ID                             (CH_ID_00)
#define TRACE_UART_CH_ID                            (CH_ID_00) 

/* -------------------------------  Macro Conf ------------------------------------------*/
//#define SHOULD_REMOVE                 (7)
#define SHOULD_CALL                     (8)
//#define TEST                          (9)                                                  
//#define SW_PRESS_HOLD_CPU             (10)
//#define DMA_DATA_ID_STR               (11)
//#define STR_TO_NUM_FROM_RIGHT_TO_LEFT (12)
 
//#define BUG_PRINT                       (13)

/* ------------------------------ Appl Data Types --------------------------------------- */

#define MAX_DATA_INPUT_DEVS                 (2)
#define MAX_DATA_OUTPUT_DEVS                (2)
#define MAX_DATA_RECEIVE_DEVS               (1) 
#define MAX_DATA_TRANSMIT_DEVS              (1)

typedef enum 
{
   DATA_ID_WATER_LEVEL_CTRL_NORMAL, NUM_DATA_IDS    	
} appl_data_id_t;

/*------------------------------- LCD Disp Conf ------------------------------------------*/
#ifdef LCD_MOD_ENABLE
   #define RESET_LCD_DISP_LOC       (BEGIN_LOC_LINE_NUM_01)
   #define RESET_LCD_INPUT_LOC      (BEGIN_LOC_LINE_NUM_02)
   #define ERROR_LINE_NUM           (LINE_NUM_04)  
   
  //WATER LEVEL Indicated in %, so Num of digits are 3
#define LCD_WATER_LVL_DATA_NUM_DIGITS            (3U)
//String Len of "PUMP : "
#define LCD_DISP_PUMP_STATE_COL_NUM              (7U)
// start of column in LCD to display after display of "PUMP :  ON " or "PUMP : OFF " "MANUAL" or "AUTO" 
#define LCD_DISP_MODE_COL_NUM                    (LCD_DISP_PUMP_STATE_COL_NUM + 4) 
// string len of "Pre Water Lvl: "
#define LCD_DISP_PRESET_WATER_LVL_COL_NUM        (16U)
#define LCD_DISP_PRESET_WATER_LVL_PERC_COL_NUM   (LCD_DISP_PRESET_WATER_LVL_COL_NUM + LCD_WATER_LVL_DATA_NUM_DIGITS)    
//string length of "Water Level: "
#define LCD_DISP_CUR_WATER_LVL_COL_NUM           (14U)
#define LCD_DISP_CUR_WATER_LVL_PERC_COL_NUM      (LCD_DISP_CUR_WATER_LVL_COL_NUM + LCD_WATER_LVL_DATA_NUM_DIGITS)
 
#endif

/* -------------------------------Timer State Conf ---------------------------------------*/
#ifdef TIMER_MOD_ENABLE
typedef enum
  { 
      TIMER_ID_DRY_RUN, TIMER_ID_PRESET_WATER_LVL_LOCK, NUM_TIMER_AND_COUNTER_IDS
  } timer_and_counter_id_t;
  
 #define TIMEOUT_NEXT_SENSOR_WATER_LVL_IN_MS           (5000U)
 #define TIMEOUT_PRESET_WATER_LVL_LOCK_IN_MS           (6000U)

#endif

/*------------------------------- Ext Interrupt Conf ------------------------------------------*/
#ifdef EXT_INTERRUPT_MOD_ENABLE

#define RESET_SW_EXT_INTP_CH_ID   (CH_ID_03)

#endif

/* ------------------------------ ADC input signal val Conf ------------------------------*/
#ifdef ADC_MOD_ENABLE

#define ADC_VREF_IN_MV                     (3300)

#endif

/* ------------------------------- Application Conf --------------------------------------*/
#define HIDDEN_KEY_DISP_CHAR                      ('X') 
#define DATA_MAX_NUM_ALLOCATED_CHARS              (11) 
#define HIDDEN_CHAR_LCD_DISP_TIME_IN_MS          (1000)
#define MAX_ICOUNT_HIDDEN_CHAR_DISP_1500MS       (1500)
#define MAX_JCOUNT_HIDDEN_CHAR_DISP_1500MS       (1500)

#define NUM_CHARS_INPUTDATA        GIVEN_CHARS_MAX_CONFIG_LINES_AND_COLS

#define DEFAULT_PRESET_WATER_LVL                 (20U)
#define MIN_PRESET_WATER_LVL                     DEFAULT_PRESET_WATER_LVL 
#define MAX_PRESET_WATER_LVL                     (80U) 
#define PRESET_WATER_LVL_INCREMENT_VAL           (1U) 
 
#define MAX_WATER_LVL_TRIG_TURN_OFF_PUMP         (99U)
#define MIN_WATER_LVL_TRIG_TURN_ON_PUMP          (0U) 
#define MAX_WATER_LVL_TRIG_TURN_ON_PUMP          (90U)
#define WATER_LVL_INVALID                        (110U)
#define MAX_WATER_LVL_SENSOR                     (100U)
#define WATER_LVL_RESOLUTION                     (20)

#define WATER_LVL_SENSORS_MAP_INVALID            (0xFF)            


#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
