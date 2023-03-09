/**************************************************************************
   FILE          :    sw_lib.h
 
   PURPOSE       :    Switch Library header 
 
   AUTHOR        :     K.M. Arun Kumar
 
  KNOWN BUGS     :
	
  NOTE           :    
	
  CHANGE LOGS     :  
	   
 **************************************************************************/
#ifndef _SW_LIB_H
#define _SW_LIB_H   (17)

 #include "constants.h"
 
 /* ---------------------- macro defination ------------------------------------------------ */
  
/* ---------------------- data type defination -------------------------------------------- */
 typedef struct 
 {
	 uint8_t io_ch;
	 uint8_t access_flag        : 1;
     uint8_t enable_flag        : 1;      	 
	 uint8_t sw_pressed_state   : 1;
	 uint8_t sw_type            : 2;	
	 uint8_t sw_cur_state       : 2; 
	 uint8_t sw_read_data       : 1;     
 } sw_para_t;
 
 typedef enum 
 {
	 PUSH_BUTTON_SW_TYPE, TOGGLE_SW_TYPE
 } sw_type_t;
 
 /* -------------------- public variable declaration --------------------------------------- */
  extern sw_para_t sw_para[]; 
  extern uint8_t sws_io_ch_map_datas[];
  
/* -------------------- public function declaration --------------------------------------- */
uint16_t IO_Ch_To_SW_Ch( const uint8_t io_ch, uint8_t *const sw_ch_id_ptr);
uint16_t Reset_SW_Ch(const uint8_t sw_ch_id);
void Reset_SW_Chs(void);
uint16_t SW_Read_By_IO_Ch( const uint8_t io_ch, uint8_t *const sw_read_data_ptr); 
uint16_t SW_Read(const uint8_t sw_ch_id, uint8_t *const sw_read_data_ptr);
uint16_t SW_Press_By_IO_Ch_Proc(const uint8_t sw_io_ch);
uint16_t SW_Press_Proc(const uint8_t sw_ch_id, void *const dummy_ptr);
uint16_t SW_Disable(const uint8_t sw_ch_id);
uint16_t SW_Enable(const uint8_t sw_ch_id);
uint16_t Disable_All_SWs(void);
uint16_t SW_Allow_Access(const uint8_t sw_ch_id);
uint16_t SW_No_Access(const uint8_t sw_ch_id);
uint16_t SW_Init(const uint8_t sw_ch_id, const uint8_t init_mode);
uint16_t SW_DeInit(const uint8_t sw_ch_id, const uint8_t deinit_mode);
uint16_t SW_Ch_ID_Check_And_Alloc(const uint8_t ch_id_alloc_type, uint8_t *const base_sw_ch_id_ptr, const uint8_t num_sw_chs);
uint16_t SW_Ch_UnConfig(const uint8_t base_sw_ch_id, const uint8_t num_sw_chs);
uint16_t SW_Oper(const uint8_t sw_io_ch, const uint8_t sw_dev_oper);
uint16_t SW_Para_Init(const uint8_t base_sw_ch_id, const uint8_t base_sw_io_ch, const uint8_t num_sw_chs, const uint8_t pressed_state);
uint16_t SW_Present_State_By_IO_Ch(const uint8_t sw_io_ch, uint8_t *const is_sw_pressed_ptr);

#endif

/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
