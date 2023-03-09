/**************************************************************************
   FILE          :    appl.h
 
   PURPOSE       :    application header.  
 
   AUTHOR        :    K.M. Arun Kumar
 
  KNOWN BUGS     :
	
  NOTE           :   
	
  CHANGE LOGS     :
	   
 **************************************************************************/
 
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _APPL_H
#define _APPL_H    (1)

#include "constants.h"
#include "sys_status.h"

/* ---------------------- macro defination ------------------------------------------------ */

/* ---------------------- data type defination -------------------------------------------- */
typedef enum
{
	ERR_UART_NOT_DATA_SRC = NUM_SYS_STATUS, ERR_WATER_PUMP_INCORRECT_STATE, ERR_WATER_LVL_PROC, ERR_WATER_LVL_SENSOR_NOT_WORK,
    ERR_EXT_INTP_PROC_FOR_RESET, NUM_APPL_STATUS
} appl_status_t; 

typedef struct
{   
    uint8_t preset_water_lvl;
	uint8_t cur_water_lvl;
    // 0: water pump state OFF, 1: water pump state is ON. 
	uint8_t water_pump_state          : 1;
	// Operation state: 0: manual mode, 1: automatic mode        
	uint8_t cur_oper_mode             : 2; 
	/* 0: set preset value minumum water level (in %)for auto pump to turn ON, 1: Water Pump trig to turn OFF, 
	2: Water Pump trig to turn ON, 3: timeout due to exceeds time for water level not reaching next or last sensor, 
	4: timeout due to long press of func switch push button */
	uint8_t func_state                : 4;
	uint8_t changed_oper_mode_flag    : 1;
	uint8_t first_time_proc_flag      : 1;
    uint8_t ctrl_cur_fsm_state        : 7;	
} water_lvl_data_t;
 
typedef enum
{
	WATER_PUMP_STATE_OFF, WATER_PUMP_STATE_ON 
} water_pump_oper_state_t; 
  
typedef enum
{
	MANUAL_MODE, AUTOMATIC_MODE, OPER_MODE_INVALID 
} water_pump_mode_t; 

typedef enum
{
	SET_PRESET_WATER_LVL, WATER_PUMP_MANUAL_TRIG_OFF, WATER_PUMP_MANUAL_TRIG_ON, WATER_PUMP_AUTO_TRIG_OFF, WATER_PUMP_AUTO_TRIG_ON, 
	TIMEOUT_DRY_RUN, TRIGGERED_RESET_SW, WATER_LVL_SENSORS_ABNORMAL, INTERNAL_ERR
} water_pump_func_state_t;

typedef enum
{
	LCD_DISP_PUMP_ON, LCD_DISP_PUMP_OFF, LCD_DISP_MANUAL_MODE, LCD_DISP_AUTO_MODE, LCD_DISP_CUR_WATER_LVL, LCD_DISP_PRESET_WATER_LVL, 
	LCD_DISP_CONSTANT_DATA, LCD_DISP_ERR_LINE_BLANK, LCD_DISP_ERROR, LCD_DISP_SENSORS_ABNORMAL, LCD_DISP_DRY_RUN
} lcd_disp_data_type_t;

typedef enum
{
	RESET_WHOLE_DATAS, RESET_BY_RESET_SW, WATER_LVL_INTERNAL_ERR_PROC, WATER_LVL_SENSORS_ABNORMAL_PROC, WATER_LVL_TIMEOUT_DRY_RUN_PROC,
    WATER_LVL_PRESET_SET_LOCK, ALL_SET_DATA_LOCK, READ_ALL_SENSORS_SWS, READ_FUNC_SW, READ_MODE_SW, READ_WATER_LVL_SENSORS, READ_WATER_PUMP_STATE 	
} ctrl_water_lvl_datas_t;

typedef enum
{
	FSM_STATE_INIT, FSM_STATE_IDLE, FSM_STATE_VALIDATE_PUMP_STATE, FSM_STATE_READ_MODE_SW, FSM_STATE_READ_WATER_LVL_SENSORS, FSM_STATE_READ_FUNC_SW,  
	FSM_STATE_DISP_CUR_WATER_LVL, FSM_STATE_SET_PRESET_WATER_LVL, FSM_STATE_PUMP_TRIG_OFF, FSM_STATE_PUMP_TRIG_ON, FSM_STATE_TIMEOUT_DRY_RUN, 
	FSM_STATE_INTERNAL_ERROR, FSM_STATE_WATER_LVL_SENSORS_ABNORMAL
}  ctrl_water_lvl_fsm_state_t;

 /* -------------------- public variable declaration --------------------------------------- */
extern water_lvl_data_t water_lvl_data; 
extern uint32_t appl_status_flag;
extern uint32_t io_chs_map_data;
extern uint8_t ctrl_prev_fsm_state;

/* -------------------- public function declaration --------------------------------------- */
uint16_t Appl_Reset(const uint8_t reset_type);
uint16_t Appl_Reset_Proc(const uint8_t );
uint16_t Reset_Water_Lvl_Datas(const uint8_t reset_type);
uint16_t LCD_Data_Disp(const uint8_t lcd_disp_data);
uint16_t Ctrl_Water_Lvl_FSM_Proc(const uint8_t read_sw_or_sensor_type);
uint16_t Ctrl_Water_Lvl_Error_Proc(const uint8_t error_proc);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
