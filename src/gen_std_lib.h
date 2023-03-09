/**************************************************************************
   FILE          :    gen_std_lib.h
 
   PURPOSE       :   Standard library Header
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
  
  CHANGE LOGS    :
	   
 **************************************************************************/
 
 #ifndef _GEN_STD_LIB_H
 #define _GEN_STD_LIB_H   (9)
 
  #include "constants.h"
  
 /* ---------------------- macro defination ------------------------------------------------ */ 

 /* Bit Operation macros */
     /* Set bit pos  in byte data   */
#define Set_Bit_in_Data(data_ptr , bit_pos)                         ((*(data_ptr)) |=   (1 << (bit_pos)))     
      /* Clear bit pos in byte data */ 
#define Clear_Bit_in_Data(data_ptr ,bit_pos)                         ((*(data_ptr)) &= (~(1 << (bit_pos))))      
    /* flip bit pos in byte data  */ 
#define Toggle_Bit_in_Data(data_ptr , bit_pos)                       ((*(data_ptr)) ^=   (1 << (bit_pos)))        
    /* Test if bit pos in byte data  is set   */
#define Test_Bit_Is_Set_in_Data(data_ptr ,bit_pos)                    (((*(data_ptr)) & (1 << (bit_pos))) >> (bit_pos))      
   /* Test if bit pos in byte data is clear */  
#define Test_Bit_Is_Clear_in_Data(data_ptr ,bit_pos)                  (!(((*(data_ptr)) & (1 << (bit_pos))) >> (bit_pos))) 

#define  DATA_ID_INVALID               (NUM_DATA_IDS + 1)
#define  DATA_NUM_INVALID              (0xFFFFFFFF)

#define MAX_NUM(X,Y)                      (((X) > (Y)) ? (X) : (Y))
#define MIN_NUM(X,Y)                      (((X) < (Y)) ? (X) : (Y))

#define PAD_RIGHT                     (1 << 0)
#define PAD_ZERO                      (1 << 1)

#define LITTLE_ENDIAN() ((*((const char *) &endian_test_data) == 0x01))

#define CHARBITS               (8)
#define FLOATBITS              (sizeof(float) * CHARBITS)

/* extract nth LSB from object stored in lvalue x */
#define GET_BIT(float_num, float_bit_index) ((((const char *) &float_num)[LITTLE_ENDIAN() ? (float_bit_index) / CHARBITS : sizeof(float_num) - (float_bit_index) / CHARBITS - 1] >> ((float_bit_index) % CHARBITS)) & 0x01)

#define PUT_BIT(conv_float, float_bit_index) ((GET_BIT((conv_float.raw_ieee_754_format.float_num), (float_bit_index)) ? (conv_float.ieee_754_format |= (1 << float_bit_index)) : (conv_float.ieee_754_format &= ~(1 << float_bit_index)) ))

/*
 * Flags used during scanf conversion.
 */
#define	LONG		       (0x01)	/* l: long or double */
#define	SHORT		       (0x04)	/* h: short */
#define	SUPPRESS	       (0x08)	/* *: suppress assignment */
#define	POINTER		       (0x10)	/* p: void * (as hex) */
#define	NOSKIP		       (0x20)	/* [ or c: do not skip blanks */
#define	LONGLONG	      (0x400)	/* ll: long long (+ deprecated q: quad) */
#define	SHORTSHORT	     (0x4000)	/* hh: char */
#define	UNSIGNED	     (0x8000)	/* %[oupxX] conversions */
#define REAL            (0x10000)   /* float or double */
/*
 * The following are used in numeric conversions only:
 * SIGNOK, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */
#define	SIGNOK	    	(0x40)	/* +/- is (still) legal */
#define	NDIGITS		    (0x80)	/* no digits detected */

#define	DPTOK		   (0x100)	/* (float) decimal point is still legal */
#define	EXPOK		   (0x200)	/* (float) exponent (e+3, etc) still legal */

#define	PFXOK		  (0x100)	/* 0x prefix is (still) legal */
#define	NZDIGITS	  (0x200)	/* no zero digits detected */

/*
 * scanf Conversion types.
 */
#define	CT_CHAR		(0)	/* %c conversion */
#define	CT_CCL		(1)	/* %[...] conversion */
#define	CT_STRING	(2)	/* %s conversion */
#define	CT_INT		(3)	/* %[dioupxX] conversion */
#define CT_REAL     (4)   /* [%f or %lf */
#define	UQUAD_MAX	((u_quad_t)0-1)	/* max value for a uquad_t */
					/* max value for a quad_t */
#define	QUAD_MAX	((quad_t)(UQUAD_MAX >> 1))
#define	QUAD_MIN	(-QUAD_MAX-1)	/* min value for a quad_t */

#define CIRCULAR_QUEUE_RESET_INDEX        (-1)
        
/* ---------------------- data type defination -------------------------------------------- */
typedef enum 
{ 
  COMM_ERROR_BY_LED, COMM_ERROR_BY_LED_AND_OR_LCD, COMM_ERROR_BY_LED_AND_OR_UART, COMM_ERROR_BY_LED_UART_AND_OR_LCD
} comm_error_by_dev_t;

typedef struct 
{
	uint32_t read_or_test_consucc_val;
	uint32_t write_or_config_consucc_val;
	uint8_t start_bit_pos;
	uint8_t bits_len;
} consucc_bit_t;

typedef enum
{
   DATA_ID_READ_MODE_TILL_ALLOC , DATA_ID_READ_MODE_TILL_TERMINATOR
} cur_data_input_mode_t;

typedef enum 
{
	RCVD_CHAR_DONT_DISP_FORMAT, RCVD_CHAR_PLAIN_DISP_FORMAT, RCVD_CHAR_HIDDEN_DISP_FORMAT   
}  cur_data_disp_format_t;
 
 typedef enum 
{
	DATA_ID_RESET_INIT_STATUS, DATA_ID_RESET_WHOLE_STATUS, DATA_ID_RESET_RETRY_STATUS, DATA_ID_RESET_APPEND_INPUT_STATUS_FLAG    
}  cur_data_reset_status_t;

typedef struct 
{
	 uint8_t data_id;
	 uint8_t data_read_num_char_enable_flag                  :  1;
	 uint8_t data_read_small_english_alpha_char_enable_flag  :  1;
	 uint8_t data_read_big_english_alpha_char_enable_flag    :  1;
	 uint8_t data_read_special_char_enable_flag              :  1;
	 uint8_t data_read_ctrl_char_enable_flag                 :  1;
	 uint8_t data_output_stream_format                       :  2;
	 uint8_t                                                 :  1;
	 uint8_t data_input_stream_format                        :  2;
     uint8_t data_output_disp_format                         :  2;
	 uint8_t output_data_type                                :  3;
	 uint8_t input_data_type                                 :  3;	
	 uint8_t data_tx_stream_format                           :  2;
	 uint8_t data_rcv_stream_format                          :  2;
	 uint8_t data_tx_event                                   :  2;
	 uint8_t data_rcv_event                                  :  2;                                
	 uint8_t max_num_data_read_try                           :  6;	
	 char data_read_terminator_char; 
     uint8_t max_req_data_len;		 	
} data_id_ctrl_para_t;

typedef struct
{
	num_value_t    data_num;
	#ifdef DMA_DATA_ID_STR
	   char *data_str;
	#else
	   char data_str[DATA_MAX_NUM_ALLOCATED_CHARS];	  
    #endif
	uint8_t data_str_len_or_pos;
} data_value_t;

typedef union
{
	const void  *dev_output_ctrl_para_ptr;	
	const void  *dev_transmit_ctrl_para_ptr;
} data_write_ctrl_ptr_t;

typedef struct
{
	 uint8_t read_valid_terminator_char_flag          : 1;
     uint8_t reach_max_alloc_read_chars_flag          : 1;
	 uint8_t data_read_mode                           : 1;	
	 uint8_t max_try_reached_flag                      : 1;
	 uint8_t read_and_ready_to_process_flag            : 1;
	 uint8_t last_try_read_and_ready_to_process_flag   : 1;
	 uint8_t                                           : 2;
} data_read_status_t;

typedef struct 
{
	 data_value_t data_value;
	 uint8_t data_id;
	 uint8_t max_allocated_data_len;	 
	 uint8_t data_read_num_try;
	 data_read_status_t data_read_status;
} data_id_status_para_t;

typedef enum
{
	DATA_RETRIEVE_CTRL_MAX_REQ_DATA_CHARS, DATA_RETRIEVE_CTRL_MAX_NUM_DATA_READ_TRY, DATA_RETRIEVE_CTRL_DATA_DISP_FORMAT, DATA_RETRIEVE_CTRL_OUTPUT_DATA_TYPE,
 	DATA_RETRIEVE_CTRL_INPUT_DATA_TYPE, DATA_RETRIEVE_CTRL_INPUT_DATA_STREAM, DATA_RETRIEVE_CTRL_OUTPUT_DATA_STREAM, DATA_RETRIEVE_CTRL_TRANSMIT_DATA_STREAM, 
	DATA_RETRIEVE_CTRL_RECEIVE_DATA_STREAM, DATA_RETRIEVE_CTRL_READ_NUM_CHAR_ENABLE_FLAG, DATA_RETRIEVE_CTRL_READ_SMALL_ENGLISH_ALPHA_CHAR_ENABLE_FLAG, 
	DATA_RETRIEVE_CTRL_READ_BIG_ENGLISH_ALPHA_CHAR_ENABLE_FLAG, DATA_RETRIEVE_CTRL_READ_SPECIAL_CHAR_ENABLE_FLAG, DATA_RETRIEVE_CTRL_READ_CTRL_CHAR_ENABLE_FLAG, 
	DATA_RETRIEVE_CTRL_TRANSMIT_SERVICE, DATA_RETRIEVE_CTRL_RECEIVE_SERVICE, DATA_RETRIEVE_CTRL_DATA_READ_TERMINATOR_CHAR, DATA_RETRIEVE_STATUS_NUM_CHARS_READ, 
	DATA_RETRIEVE_STATUS_DATA_STR, DATA_RETRIEVE_STATUS_DATA_CHAR, DATA_RETRIEVE_STATUS_DATA_INT32, DATA_RETRIEVE_STATUS_DATA_UINT8
} appl_data_retrieve_type_t;

typedef enum
{
	RESET_WHOLE, RESET_MOD_STATUS, RESET_DATA_IDS_AND_APPL, RESET_APPL, RESET_RECEIVE_DATAS, RESET_STATUS_RECEIVE_CHARS, RESET_TRANSMIT_DATAS, RESET_STATUS_TRANSMIT_CHARS
} reset_ctrl_t;

typedef enum 
{
	CONFIG_RECEIVE_END_POINT_FLAG, CONFIG_MAX_RECEIVE_CHARS, CONFIG_TRANSMIT_END_POINT_FLAG, CONFIG_MAX_TRANSMIT_CHARS
} config_max_tx_and_rcv_data_t;

typedef enum
{
	BASE_02 = 2, BASE_08 = 8, BASE_10 = 10, BASE_16 = 16
} base_t;

typedef enum 
{
	NUM_CONV_BIG_ALPHA, NUM_CONV_SMALL_ALPHA, NUM_CONV_ALPHA_NA
} num_conv_alpha_t;

typedef enum 
{
	GET_NUM_DIGITS, GET_POWER_OF_CUR_NUM_DIGITS, GET_POWER_OF_NEXT_NUM_DIGITS  
} get_num_digits_type_t;

typedef enum
{
	BASE_BIT_POS = 0, SIGN_FLAG_BIT_POS = 5, PAD_FORMAT_BIT_POS = 6, ALPHA_BIT_POS = 8 
} print_num_flag_bit_pos_t;

typedef union
{ 
  
    float float_num; 
    struct
    { 
        // Order is important. 
        // Here the members of the union data structure 
        // use the same memory (32 bits). 
        // The ordering is taken 
        // from the LSB to the MSB. 
        uint32_t mantissa  : 23; 
        uint32_t exponient : 8; 
        uint32_t sign      : 1; 
    } raw; 
} raw_ieee_754_format_t; 

typedef struct 
{
	raw_ieee_754_format_t raw_ieee_754_format;	
	uint32_t ieee_754_format;	
} float_ieee_754_format_t;	

typedef enum 
{
	REAL_VAL_IS_NEGATIVE_INFINITY, REAL_VAL_IS_POSITIVE_INFINITY,  REAL_VAL_IS_NEGATIVE_NOT_A_NUM, REAL_VAL_IS_POSITIVE_NOT_A_NUM, REAL_VAL_IS_NEGATIVE_ZERO,
	REAL_VAL_IS_POSITIVE_ZERO, REAL_VAL_IS_NEGATIVE_DENORMALISED, REAL_VAL_IS_POSITIVE_DENORMALISED, REAL_VAL_IS_NEGATIVE_NORMALISED, REAL_VAL_IS_POSITIVE_NORMALISED 	
} ieee_754_real_val_data_status;
 
typedef enum
{
	CHAR_DEVICE, BLOCK_DEVICE, DEVICE_DATA_NA  
} device_data_type_t;

typedef enum 
{
	STREAM_IO_INPUT, STREAM_IO_OUTPUT, STREAM_IO_BOTH, STREAM_IO_NA
} stream_io_type_t;

typedef enum 
{
	STREAM_TYPE_IO, STREAM_TYPE_FILE, STREAM_TYPE_SOCKET, STREAM_TYPE_NA
} stream_type_t ;

typedef struct file_t
{
	char stream_buf[MAX_STREAM_BUFFER_SIZE + 1];
	uint8_t file_desp;
	uint8_t num_chars_stream_buf;
	uint8_t stream_buf_cur_pos;
	uint8_t device_type	    : 2; //char or block
	uint8_t stream_type     : 2; //io_type, file or socket
	uint8_t stream_io_type  : 2; //input, output, both	
	uint8_t                 : 2;	
} file_t;

typedef enum 
{ 
    STDIN_FILE_DESP, STDOUT_FILE_DESP, STDERR_FILE_DESP, STR_FILE_DESP  
} file_desp_pre_conf_t;
  
typedef struct
{
	volatile void *associated_data_ptr;
	uint16_t related_data_type;	
} data_t;

typedef struct
{
	data_t data_arr[MAX_ELEMENTS_CIRC_QUEUE_ARR];
	int8_t front_index; //dequeue at front_index
	int8_t rear_index;  //enqueue at rear_index
} circular_queue_data_t;

/* -------------------- public variable declaration --------------------------------------- */
 extern data_id_status_para_t data_id_status_para[NUM_DATA_IDS];
 extern uint8_t cur_data_id, error_or_warning_proc_flag;
 extern const data_id_ctrl_para_t  data_id_ctrl_para[NUM_DATA_IDS]; 
 extern uint8_t internal_error_state;
 extern num_value_t disp_trace_num;
 extern disp_num_fmt_t disp_trace_num_fmt;
 extern const uint32_t endian_test_data;
 extern const uint8_t internal_error_led_io_ch;
 #ifdef LCD_MOD_ENABLE
    extern char lcd_disp_error_code[MAX_AVAIL_NUM_COLS];
 #endif

 extern volatile circular_queue_data_t comm_receive_queue;
 extern volatile circular_queue_data_t comm_transmit_queue; 


/* -------------------- public function declaration --------------------------------------- */ 
/* in gen_std_lib_part_1 */
void SW_Time_Delay(const uint32_t max_i_count, const uint32_t max_j_count);
uint16_t Config_Consucc_Bits( const uint8_t flag_consucc_bit, void *const data_ptr);
uint16_t Test_Consucc_Bits(const uint8_t flag_consucc_bit, const void *const data_ptr );
uint16_t Str_to_Num_Conv(int32_t *const num_conv_from_str, const char *const num_in_str );
uint8_t Str_Len(const char *const str);
uint16_t Reset_Process(const uint8_t reset_type);
uint16_t Reset_Data_ID_Status(const uint8_t data_id, const uint8_t set_cur_data_status_reset_type);
void Reset_Data_IDs_Status(void);
uint16_t Next_Data_Conf_Parameter(const uint8_t set_cur_data_id);
uint16_t Data_IDs_Set_Para(void);
uint16_t Data_ID_Set_Para(const uint8_t data_id, const uint8_t max_alloc_data_len);
uint32_t Power_Of(const uint8_t base, const uint8_t power);
void Delay_MS(const uint32_t max_elapsed_time_in_ms );
void Delay_US(const uint32_t max_elapsed_time_in_us );
uint16_t Write_Bit_in_Data( uint32_t *const data_ptr, const uint8_t bit_pos, const uint8_t set_bit_val );
uint16_t Error_or_Warning_Proc(const char *const error_trace_str, const uint8_t warn_or_error_format, const uint32_t warning_or_error_code);
uint16_t Comm_Error_Proc(const char *const error_trace_str, const uint8_t warn_or_error_format, const uint8_t error_comm_by_dev, const uint32_t warning_or_error_code);
uint16_t Appl_Data_Retrieve_Para(const uint8_t data_id, const uint8_t retrieve_data_form, void *const retrive_data_ptr);
uint16_t Set_Status_Reset_Data_IDs(const uint8_t set_data_status_reset_type);
char* Num_To_Str_Conv_Till_Conf_Size(const int32_t num_to_convert, const uint8_t base, const uint8_t alpha_flag, const uint8_t conv_till_conf_size );
uint16_t Init_File(file_t *const file_ptr, const uint8_t file_desp, const uint8_t device_type, const uint8_t stream_type, const uint8_t stream_io_type);
uint16_t File_Flush(file_t *const file_ptr);
quad_t strtoq(const char *const nptr, char **endptr, const uint8_t base);
unsigned long strtoul(const char *const nptr, char **endptr, const uint8_t base);
char* Num_To_Str_Conv_No_Conf_Size(const int32_t num_to_str, char *const num_in_str, const uint8_t base);
uint16_t Reverse_Str_Use_Diff_Str(const char *const str_to_reverse_ptr, char *const reversed_str_ptr, const uint8_t conf_str_to_reverse_start_pos, const uint8_t conf_str_to_reverse_len);
uint16_t Get_Num_Digits_Based_Data(const uint32_t num, int32_t *const num_digits_data_ptr, const uint8_t base, const uint8_t get_num_digits_type);
uint16_t Reverse_Str_Use_Same_Str(char *const str, const uint8_t len);
uint16_t isascii(const unsigned char test_char);
uint16_t Error_or_Warning_Occured_Proc(const uint8_t warn_or_error_format, const uint32_t warning_or_error_code);

/* in gen_std_lib_part_2 */
uint16_t Real_Val_To_IEEE_754_Float(const float src_float_num, float_ieee_754_format_t *const result_float_ptr);
uint32_t IEEE_754_Float_Raw_To_Int(const uint32_t ieee_754_float_format, const uint8_t low, const uint8_t high);
uint16_t IEEE_754_Float_To_Real_Val(const float_ieee_754_format_t *const src_float_ptr, float *const result_float_num_ptr);
uint16_t IEEE_754_Float_Data_Status(const float_ieee_754_format_t float_ieee_754_format, uint8_t *const float_data_status_ptr);
int16_t Int_To_Str(const int32_t num, char *const str_ptr, const uint8_t req_num_digits); 
uint16_t Double_To_Str_Conv(const double double_num, char *const double_to_str, uint8_t num_digits_after_point);
double Str_To_Double_Conv(const char *const double_to_str);
int16_t File_Get_Char(file_t *const fp);
int16_t Read_Oper(file_t *const fp, char *const out_buf_ptr, const uint8_t max_num_chars_to_read);
char *File_Get_Str(char *const dst_str_ptr, file_t *const fp, const uint8_t max_chars_to_get);
int16_t File_Scan(file_t *const fp, const char *const fmt, ...);
int16_t Str_Scan(const char *const ibuf, const char *const fmt, ...);
int16_t File_Put_Char(const char put_char, file_t *const fp);
int16_t Write_Oper(file_t *const fp, const char *const in_buf_ptr, const uint8_t max_num_chars_to_write);
int16_t File_Put_Str(const char *const io_buf_ptr, file_t *const fp);
int16_t File_Print(file_t *const fp, const char *const format_ptr, ...);
int16_t Str_Print(char *out_str, const char *const format_ptr, ...);
int16_t Str_Cpy(char * const dest_str_ptr, const char* const src_str_ptr);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
