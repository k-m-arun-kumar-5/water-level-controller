/**************************************************************************
   FILE          :    lcd.h
 
   PURPOSE       :    LCD Header
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :  
 
  CHANGE LOGS    :
	   
 **************************************************************************/
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _LCD_H
#define _LCD_H   (13)

 #include "constants.h"
 
/* ---------------------- macro defination ------------------------------------------------ */                           

#define COL_NUM_01                   (1)

/* DONT_CHANGE: BEGIN - macro values must not be changed */

#define MAX_DISP_NUM_LINES_ALL    (1)
#define MAX_DISP_NUM_LINES_1      (0) 

#define DATA_INTERFACE_8_BITS       (1) 
#define DATA_INTERFACE_4_BITS       (0)

#define CHAR_FONT_5_10_DOTS        (1)
#define CHAR_FONT_5_8_DOTS         (0)

/*for 20 * 4 LCD disp */ 
/* num cols = num of chars in a line */
#define MAX_AVAIL_NUM_COLS                    (20)
#define MAX_AVAIL_NUM_LINES                   (4)
                            
#define BEGIN_LOC_LINE_NUM_01                      (0x80)
#define BEGIN_LOC_LINE_NUM_02                      (0xC0)
// (0x94)
#define BEGIN_LOC_LINE_NUM_03                     (BEGIN_LOC_LINE_NUM_01 + MAX_AVAIL_NUM_COLS) 
//(0xD4) 
#define BEGIN_LOC_LINE_NUM_04                     (BEGIN_LOC_LINE_NUM_02  + MAX_AVAIL_NUM_COLS) 
// (0x93)
#define END_LOC_LINE_NUM_01                       (BEGIN_LOC_LINE_NUM_01 + MAX_AVAIL_NUM_COLS - 1)
//(0xD3)
#define END_LOC_LINE_NUM_02                       (BEGIN_LOC_LINE_NUM_02  + MAX_AVAIL_NUM_COLS - 1)
//(0xA7)
#define END_LOC_LINE_NUM_03                       (BEGIN_LOC_LINE_NUM_03  + MAX_AVAIL_NUM_COLS - 1) 
//(0xE7)
#define END_LOC_LINE_NUM_04                       (BEGIN_LOC_LINE_NUM_04 + MAX_AVAIL_NUM_COLS - 1)

#define LCD_LOC_INVALID                     (0x00)
#define LCD_LINE_NUM_OR_INDEX_INVALID       (MAX_AVAIL_NUM_LINES + 1) 
#define LCD_COL_NUM_OR_INDEX_INVALID        (MAX_AVAIL_NUM_COLS + 1)     
            
/* DONT_CHANGE: END */

#define LCD_CTRL_LOC_TYPE_DATA_POS       (7)

/* ---------------------- data type defination -------------------------------------------- */
typedef enum
{ 
  GIVEN_CHARS_MAX_CONFIG_LINES_AND_COLS  = 1, GIVEN_XY_MAX_CONFIG_LINES_AND_COLS, GIVEN_CHARS_MAX_XY
} lcd_config_chars_format_ctrl_t;  

typedef enum 
{
 	NO_CHECK_BUSY_FLAG, CHECK_BUSY_FLAG  
} lcd_busy_flag_ctrl_t;  

typedef enum 
{
 	ALL_LINES, LINE_NUM_01 = 1, LINE_NUM_02, LINE_NUM_03, LINE_NUM_04 
} lcd_line_num_t;
	
/* should status be displayed on or off for error, warning and current time left and its count of long key press, no key press, 
  time within process should succeed or failed  */
typedef enum 
{
   STATUS_DISP_OFF, STATUS_DISP_ON
} lcd_status_disp_ctrl_t;

typedef enum
{
 	LCD_INPUT_LOC, LCD_OUTPUT_LOC
} lcd_loc_ctrl_t;

typedef struct
 {
	uint8_t access_flag                 : 1;
    uint8_t enable_flag                 : 1; 	
	uint8_t interface                   : 1;
	uint8_t font                        : 1;
    uint8_t check_bf                    : 1;
	uint8_t max_avail_lines_in_index    : 2;   
	uint8_t                             : 1;  
    uint8_t max_config_lines_in_index   : 2;
	uint8_t max_avail_cols_in_index     : 5;	
	uint8_t                             : 1;
	uint8_t max_config_cols_in_index    : 5;
	uint8_t                             : 3;	
} lcd_ctrl_t;

typedef struct
{
	uint8_t line_num_or_index     : 3;
    uint8_t col_num_or_index      : 5;	
} lcd_point_t;
 
typedef struct
{
	lcd_point_t cur_data_start_point_num;
	lcd_point_t next_data_start_point_num;
	uint8_t max_num_chars_config;
} lcd_point_config_t;

typedef struct
{
	lcd_point_t	start_point_num;
	disp_num_fmt_t  disp_num_fmt;
	char *lcd_disp_str;
	int32_t lcd_disp_num;
	uint8_t use_start_point_flag       :  1;
	uint8_t use_data_status_flag       :  1;
	uint8_t data_till_end_point_flag   :  1;
	uint8_t                            :  5;
} lcd_disp_ctrl_t;

typedef struct
{
	lcd_point_t start_point_num;
	lcd_point_t end_point_num;
	uint8_t start_loc;
	uint8_t end_loc;
	uint8_t max_num_chars_alloc;		
} cur_data_lcd_para_t;	

typedef struct
{
	cur_data_lcd_para_t cur_data_lcd_para;
	uint8_t lcd_data_or_cmd;
	uint8_t lcd_cur_disp_loc;
	uint8_t lcd_cur_input_loc;
	/* currently displayed data in each line starts from 1 ie use array index as line num for us. index 0 can be used as all lines */
    uint8_t cur_line_disp_data[MAX_AVAIL_NUM_LINES + 1];	
	uint8_t data_till_end_point_flag   : 1;
	uint8_t lcd_input_loc_flag         : 2;
	uint8_t                            : 5;
} lcd_status_t;

typedef enum
{
    ERROR_DISP, WARNING_DISP, FATAL_DISP, BLANK_LINE_DISP, NUM_LCD_GNRL_DISP_DATAS 
} lcd_general_disp_data_t;

/* -------------------- public variable declaration --------------------------------------- */
extern lcd_ctrl_t lcd_ctrl[NUM_OUTPUT_DEV_ID_LCD_CHS]; 
extern lcd_status_t lcd_status[NUM_OUTPUT_DEV_ID_LCD_CHS];
extern const uint8_t lcd_io_ch_ctrl[][3 + 8];

/* -------------------- public prototype declaration --------------------------------------- */
uint16_t LCD_Init(const uint8_t lcd_ch_id, const uint8_t init_mode);
uint16_t LCD_Clear_Screen(const uint8_t lcd_ch_id);
uint16_t LCD_Write_Command_NO_BF(const uint8_t lcd_ch_id, const uint8_t lcd_cmd);
uint16_t LCD_Write_Command(const uint8_t lcd_ch_id, const uint8_t lcd_cmd);
uint16_t LCD_Disp_Char(const uint8_t lcd_ch_id, const char lcd_data);
uint16_t LCD_Disp_Str(const uint8_t lcd_ch_id, const char *const lcd_disp_str);
uint16_t LCD_Disp_Num(const uint8_t lcd_ch_id, const uint8_t num_data_type, const disp_num_fmt_t disp_num_fmt, const int32_t lcd_num);
uint16_t LCD_Disp_Goto_XY(const uint8_t lcd_ch_id, const uint8_t start_line_num, const uint8_t start_col_num);
uint16_t LCD_Input_Goto_XY(const uint8_t lcd_ch_id, const uint8_t start_line_num, const uint8_t start_col_num);
uint16_t LCD_From_XY_To_Loc(const uint8_t lcd_ch_id, const lcd_point_t *const start_lcd_point, uint8_t *const lcd_loc_ptr);
uint16_t LCD_From_Loc_to_XY(const uint8_t lcd_ch_id, const uint8_t loc_lcd, lcd_point_t *const lcd_point_num_ptr);
uint16_t LCD_Config_Cur_Loc(const uint8_t lcd_ch_id, const uint8_t set_loc_ctrl_flag, const uint8_t set_input_loc, const uint8_t set_disp_loc);
uint16_t LCD_Write(const uint8_t lcd_ch_id, const void *const lcd_rcv_disp_num_ptr);
uint16_t LCD_Disable(const uint8_t lcd_ch_id);
uint16_t LCD_Enable(const uint8_t lcd_ch_id);
uint16_t Disable_All_LCDs(void);
uint16_t LCD_Allow_Access(const uint8_t lcd_ch_id);
uint16_t LCD_No_Access(const uint8_t lcd_ch_id);
uint16_t LCD_DeInit(const uint8_t lcd_ch_id, const uint8_t deinit_mode);
uint16_t LCD_Cur_Data_By_NumChars_Calc_Para(const uint8_t lcd_ch_id, const lcd_point_config_t *const lcd_point_config_ptr);
uint16_t LCD_Cur_Data_By_XY_Calc_Para(const uint8_t lcd_ch_id, const lcd_point_config_t *const lcd_point_config_ptr);
uint16_t LCD_Cur_Data_By_NumChars_and_XY_Calc_Para(const uint8_t lcd_ch_id, const lcd_point_config_t *const lcd_point_config_ptr);
uint16_t Reset_LCD_Cur_Data_Status_Para(uint8_t lcd_ch_id);
uint16_t LCD_Loc_Ctrl_Proc(const uint8_t lcd_ch_id, const uint8_t lcd_ctrl_format, const char lcd_rcvd_char );
uint16_t Reset_All_LCD_Chs(const uint8_t lcd_reset_type);
uint16_t Reset_LCD_Ch(const uint8_t lcd_ch_id, const uint8_t lcd_reset_type);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
