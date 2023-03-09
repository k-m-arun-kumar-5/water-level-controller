/**************************************************************************
   FILE          :    seg7_lib.h
 
   PURPOSE       :   
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   
	
  CHANGE LOGS     :  
	   
 **************************************************************************/
#ifndef _SEG7_LIB_H
#define _SEG7_LIB_H   (16)

#include "constants.h"
 
/* ---------------------- macro defination ------------------------------------------------ */
#define SEG7_COMMON_CATHODE               (0)
#define SEG7_COMMON_ANODE                 (1)

#define SEG7_1_DIGIT                      (1U)
#define SEG7_2_DIGIT                      (2U)
#define SEG7_3_DIGIT                      (3U)
#define SEG7_4_DIGIT                      (4U)
#define SEG7_5_DIGIT                      (5U)
#define SEG7_6_DIGIT                      (6U)

#define SEG7_START_NON_DIGIT_CODE    ('A')
#define SEG7_MAX_NUM_HEXA_DIGITS          (10)

/* ---------------------- data type defination -------------------------------------------- */

typedef struct 
{
    uint8_t io_ch_a_led;
	uint8_t io_ch_unit_digit;
	uint8_t access_flag                   : 1; 
	uint8_t enable_flag                   : 1;
	uint8_t seg7_type                     : 1; 
    uint8_t num_digits                    : 3;
	uint8_t dot_disp_flag                 : 1;
	uint8_t digit_place_active_state      : 1;
} seg7_ctrl_t;

typedef uint8_t (*seg7_disp_num_ptr_t)(const uint8_t seg7_ch_id, const void *const seg7_rcv_disp_num_ptr);

/* -------------------- public variable declaration --------------------------------------- */
 extern seg7_ctrl_t seg7_ctrl[NUM_OUTPUT_DEV_ID_SEG7_CHS];

/* -------------------- public function declaration --------------------------------------- */
uint16_t Seg7_Write(const uint8_t seg7_ch_id, const void *const seg7_rcv_disp_num_ptr);
uint16_t Seg7_Disable(const uint8_t seg7_ch_id);
uint16_t Seg7_Enable(const uint8_t seg7_ch_id);
uint16_t Reset_All_Seg7_Chs(const uint8_t seg7_reset_type);
uint16_t Seg7_Allow_Access(const uint8_t seg7_ch_id);
uint16_t Seg7_No_Access(const uint8_t seg7_ch_id);
uint16_t Seg7_Init(const uint8_t seg7_ch_id, const uint8_t init_mode);
uint16_t Seg7_DeInit(const uint8_t seg7_ch_id, const uint8_t deinit_mode);

#endif 

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
