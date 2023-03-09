
/**************************************************************************
   FILE          :    conf_port.h
 
   PURPOSE       :    port header - define port and its pin assignment.
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :    port header 
 To avoid this header file to be included more than once, conditional directive is used  
	
  CHANGE LOGS     :  
	   
 **************************************************************************/
#ifndef _CONF_PORT_H

#define _CONF_PORT_H   

#include "constants.h"

/* ---------------------- macro defination ------------------------------------------------ */

/* ---------------------- data type defination -------------------------------------------- */

typedef enum 
{
 INTERNAL_ERROR_LED_IO_CH = (IO_CH_08), WATER_PUMP_CTRL_IO_CH  = (IO_CH_25)	 
} io_ch_map_op_ch_t; 

 
 #ifdef SW_MOD_ENABLE
 
typedef enum 
{
 	IO_CH_SW_CH_00 = (IO_CH_02), WATER_LVL_100_SENSOR_IO_CH = IO_CH_SW_CH_00,      IO_CH_SW_CH_01 = (IO_CH_04), WATER_LVL_080_SENSOR_IO_CH = IO_CH_SW_CH_01,
	IO_CH_SW_CH_02 = (IO_CH_05), WATER_LVL_060_SENSOR_IO_CH = IO_CH_SW_CH_02,      IO_CH_SW_CH_03 = (IO_CH_16), WATER_LVL_040_SENSOR_IO_CH = IO_CH_SW_CH_03,
	IO_CH_SW_CH_04 = (IO_CH_17), WATER_LVL_020_SENSOR_IO_CH = IO_CH_SW_CH_04,      IO_CH_SW_CH_05 = (IO_CH_18), WATER_LVL_000_SENSOR_IO_CH = IO_CH_SW_CH_05,
	IO_CH_SW_CH_06 = (IO_CH_23), FUNC_SW_IO_CH = IO_CH_SW_CH_06,                   IO_CH_SW_CH_07 = (IO_CH_31), MODE_SW_IO_CH = IO_CH_SW_CH_07    
} io_ch_map_sw_ch_t; 

#define NUM_SWS_IO_CH_MAP			(6)

#endif

#ifdef LCD_MOD_ENABLE  
typedef enum 
{
     LCD_CH_00_IO_CH_CMD_RS	 = (IO_CH_50), LCD_CH_00_IO_CH_CMD_RW = (IO_CH_51), LCD_CH_00_IO_CH_CMD_E = (IO_CH_52), LCD_CH_00_IO_CH_D0 = (IO_CH_INVALID),
	 LCD_CH_00_IO_CH_D1 = (IO_CH_INVALID), LCD_CH_00_IO_CH_D2 = (IO_CH_INVALID),  LCD_CH_00_IO_CH_D3 = (IO_CH_INVALID),  LCD_CH_00_IO_CH_D4 = (IO_CH_58),
	 LCD_CH_00_IO_CH_D5 = (IO_CH_59),  LCD_CH_00_IO_CH_D6 = (IO_CH_60), LCD_CH_00_IO_CH_D7 = (IO_CH_61)
} io_ch_map_lcd_ch_t;
							  
#endif

/* -------------------- public variable declaration --------------------------------------- */
 
/* -------------------- public function declaration --------------------------------------- */
 
#endif 

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
