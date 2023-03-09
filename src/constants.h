/**************************************************************************
   FILE          :    constants.h
 
   PURPOSE       :     
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
	
  CHANGE LOGS     :
	   
 **************************************************************************/
 
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _CONSTANTS_H
#define _CONSTANTS_H   (7)

/* ---------------------- macro defination ------------------------------------------------ */

#define NULL_DATA_PTR                                  ((void *)0)
#define EOF                                            (-1)

#define LED_OFF                                       (0)
#define LED_ON                                        (1) 

#define SUCCESS                                 (0)
#define FAILURE                                 (1)
 
#define BACKSPACE_CHAR                        ('\b')
#define ENTER_CHAR                            ('\r')
#define NEW_LINE_CHAR                         ('\n')
#define HORIZONTAL_TAB                        ('\t')
#define VERTICAL_TAB                          ('\v')
#define NULL_CHAR                             ('\0')   
#define NUM_0_CHAR                            ('0')
#define NUM_9_CHAR                            ('9')
#define ENGLISH_SMALL_ALPHA_a_CHAR            ('a')
#define ENGLISH_SMALL_ALPHA_z_CHAR            ('z')
#define ENGLISH_BIG_ALPHA_A_CHAR              ('A')
#define ENGLISH_BIG_ALPHA_Z_CHAR              ('Z')
#define BEGIN_CTRL_CHARS_ASCII_CODE           ('\x0')
#define END_CTRL_CHARS_ASCII_CODE             ('\x1F')                                  
#define ASCII_MAX_CHAR_CODE                   ('\x7F')
#define CHARS_RCV_TILL_TERMINATOR_CHAR_MODE        (0U)

#define TIME_UNIT_1_SEC_IN_MILLI_SEC                   (1000)

#define DATA_NA                                 (0)
#define DATA_INVALID                            (1)

/* ---------------------- data type defination -------------------------------------------- */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

typedef char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int64_t;

typedef unsigned int u_quad_t;
typedef int quad_t;
typedef long double real_t;
typedef void *  uintptr_t;

typedef union 
{
	uint64_t val_uint_64; 
	int64_t val_int_64; 
	uint32_t val_uint_32[2];
	int32_t val_int_32[2];
	uint16_t val_uint_16[4];
	int16_t val_int_16[4];
	uint8_t val_uint_8[8];
	int8_t val_int_8[8];
	struct 
	{
	      	uint32_t bit_00: 1;
	    	uint32_t bit_01: 1;
		    uint32_t bit_02: 1;
		    uint32_t bit_03: 1;
	    	uint32_t bit_04: 1;
		    uint32_t bit_05: 1;
		    uint32_t bit_06: 1;
	    	uint32_t bit_07: 1;
		    uint32_t bit_08: 1;
		    uint32_t bit_09: 1;
	    	uint32_t bit_10: 1;
		    uint32_t bit_11: 1;
			uint32_t bit_12: 1;
	    	uint32_t bit_13: 1;
		    uint32_t bit_14: 1;
		    uint32_t bit_15: 1;
	    	uint32_t bit_16: 1;
		    uint32_t bit_17: 1;
		    uint32_t bit_18: 1;
	    	uint32_t bit_19: 1;
		    uint32_t bit_20: 1;
		    uint32_t bit_21: 1;
	    	uint32_t bit_22: 1;
		    uint32_t bit_23: 1;
			uint32_t bit_24: 1;
		    uint32_t bit_25: 1;
		    uint32_t bit_26: 1;
	    	uint32_t bit_27: 1;
		    uint32_t bit_28: 1;
		    uint32_t bit_29: 1;
	    	uint32_t bit_30: 1;
		    uint32_t bit_31: 1;
	} val_in_bits[2];
} num_value_t;

typedef enum 
{
	KEY_PRESSED = 0, KEY_RELEASED = 1, STATE_KEY_TRANSITION_TO_PRESSED = 0, STATE_KEY_PRESSED, STATE_KEY_TRANSITION_TO_RELEASED, STATE_KEY_RELEASED
} key_states_t;

// DONT_CHANGE : BEGIN,  MUST NOT CHANGE STATE_OFF, STATE_ON, STATE_NO, STATE_YES, STATE_LOW and STATE_HIGH defined value

typedef enum 
{
	STATE_OFF = 0, STATE_ON = 1, STATE_NO = 0, STATE_YES = 1, STATE_NA, STATE_LOW = 0, STATE_HIGH = 1, STATE_TOGGLE, STATE_UNKNOWN, STATE_INVALID, STATE_VALID,
	STATE_YES_IN_CHAR = 'Y', STATE_NO_IN_CHAR ='N' , STATE_INVALID_IN_CHAR = 'X', STATE_VALID_IN_CHAR = 'V', STATE_UNKNOWN_IN_CHAR = 'U'
} gen_states_type_t;

// DONT_CHANGE : END

typedef enum 
{
	EVENT_INTERRUPT, EVENT_POLLING, EVENT_INVALID, EVENT_NA
} event_type_t;

typedef struct 
{
	   uint8_t sign_flag                 : 1;
	   uint8_t least_digits_flag         : 1; // if 1, then take num_digits from Least significant digit(from unit digits to num_digits)
	   uint8_t left_format_flag          : 1; // if 1, then left to right format 
	   uint8_t num_digits                : 5;
} disp_num_fmt_t;

typedef enum
{
	DATA_TYPE_IN_INVALID, DATA_TYPE_NO_DATA, DATA_TYPE_IN_STR, DATA_TYPE_IN_BINARY, DATA_TYPE_IN_HEXA, DATA_TYPE_IN_DECIMAL, DATA_TYPE_IN_CHAR, DATA_TYPE_IN_APPL_DEFINED
} data_type_t;

typedef enum
{ 
    DEV_DEINIT_OPER, DEV_INIT_OPER, DEV_NO_ACCESS_OPER, DEV_ALLOW_ACCESS_OPER, DEV_DISABLE_OPER, DEV_ENABLE_OPER, 
	DEV_RECEIVE_OPER, DEV_TRANSMIT_OPER, DEV_READ_OPER, DEV_WRITE_OPER, DEV_OPEN_OPER, DEV_CLOSE_OPER, NUM_DEV_OPERS 
} dev_oper_t;

#define COMM_TRANSMIT_FUNC              (NUM_DEV_OPERS)
#define COMM_RECEIVE_FUNC               (NUM_DEV_OPERS + 1)

typedef enum
{
	COMM_NO_FLOW_CTRL, COMM_HW_FLOW_CTRL, COMM_SW_FLOW_CTRL
} comm_flow_ctrl_t;

typedef enum
{
	NO_ERROR_CHECK, ODD_PARITY, EVEN_PARITY
} comm_error_check_t;


/* -------------------------- data type declaration --------------------------------------- */
typedef struct file_t file_t;
typedef struct dev_id_t dev_id_t;

/* -------------------- public variable declaration --------------------------------------- */


/* -------------------- public function declaration --------------------------------------- */

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
