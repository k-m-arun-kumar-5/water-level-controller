/* ********************************************************************
FILE                   : gen_std_lib_part_2.c

PURPOSE                : 
                     									 	 
AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            : 

NOTE                  : 
                       
CHANGE LOGS           : 

FILE ID               : 15 

*****************************************************************************/

#include "main.h"
#include <stdarg.h>
/* ------------------------------ macro defination ------------------------------ */

/* ----------------------------- global variable defination --------------------- */


/* ----------------------------- global variable declaration -------------------- */


/* ----------------------------- global function declaration -------------------- */
int16_t VFile_Scan(file_t *const fp, const char *const fmt, va_list *const scan_arg_list_ptr);
int16_t VFile_Print(file_t *const fp, const char *const fmt, va_list *const print_va_list_ptr);
static int16_t Scan_Data(const char *inp, const char  *const fmt0, va_list *const scan_va_list_ptr);
static int16_t Print_Data(va_list *const print_va_list_ptr, file_t *const file_ptr, const char *const format_ptr);
static char Print_Char(char **const out_str_ptr, const char print_char);
static int16_t Print_Str(char **const out_str_ptr, const char *print_str, const uint8_t conf_width_spec, const uint8_t pad_format);
static int16_t Print_Num(char **const out_str_ptr, const int32_t print_num, const uint8_t conf_width_spec, const uint16_t ctrl_flag );
static int16_t Print_Float(char **const out_str_ptr, const float print_float, const uint8_t conf_width_spec, const uint8_t conf_num_digits_after_point, const uint16_t ctrl_flag );

/*------------------------------------------------------------*
FUNCTION NAME  : Real_Val_To_IEEE_754_Float

DESCRIPTION    : convert a real value to IEEE 754 floating point representaion 
								
INPUT          : 

OUTPUT         : 

NOTE           : single precision IEEE 754 floating point rep

Func ID        : 15.01 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Real_Val_To_IEEE_754_Float(const float src_float_num, float_ieee_754_format_t *const result_float_ptr)
{
	int16_t float_bit_index;
     
	if(result_float_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.01.01", ERROR_OCCURED, system_status_flag); 
		return system_status_flag; 
	}
   memset(result_float_ptr, NULL_CHAR, 1);
	result_float_ptr->raw_ieee_754_format.float_num = src_float_num;
	result_float_ptr->ieee_754_format = 0;
    float_bit_index = FLOATBITS - 1;
	//sign_bit part
    PUT_BIT((*result_float_ptr), float_bit_index);
    result_float_ptr->raw_ieee_754_format.raw.sign  = (result_float_ptr->ieee_754_format >> 31) & 0x01;
   // exponient part
    for(float_bit_index--; float_bit_index >= 23; float_bit_index--)
	{
        PUT_BIT((*result_float_ptr), float_bit_index);
    }
	result_float_ptr->raw_ieee_754_format.raw.exponient = (result_float_ptr->ieee_754_format >> 23) & 0xFF;
	//mantissa part
    for(; float_bit_index  >= 0; float_bit_index--)
	{
        PUT_BIT((*result_float_ptr), float_bit_index);
    }
	result_float_ptr->raw_ieee_754_format.raw.mantissa = (result_float_ptr->ieee_754_format) & 0x7FFFFF;
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : IEEE_754_Float_Raw_To_Int

DESCRIPTION    : convert a raw IEEE float rep to the corresponding integer
								
INPUT          : 

OUTPUT         : 

NOTE           : single precision IEEE 754 floating point rep

Func ID        : 15.02 

Bugs           :  
-*------------------------------------------------------------*/
uint32_t IEEE_754_Float_Raw_To_Int(const uint32_t ieee_754_float_format, const uint8_t low, const uint8_t high) 
{ 
    uint32_t float_to_int = 0, i; 
	
    for (i = high; i >= low; i--)
	{ 
        float_to_int = float_to_int + ((ieee_754_float_format >> (31 - i)) & 0x01) * Power_Of(BASE_02, high - i); 
    } 
    return float_to_int; 
} 

/*------------------------------------------------------------*
FUNCTION NAME  : IEEE_754_Float_To_Real_Val

DESCRIPTION    : convert IEEE 754 floating point representation into real value 
								
INPUT          : 

OUTPUT         : 

NOTE           : single precision IEEE 754 floating point rep

Func ID        : 15.03 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t IEEE_754_Float_To_Real_Val(const float_ieee_754_format_t *const src_float_ptr, float *const result_float_num_ptr)
{
	float_ieee_754_format_t result_float;
	uint32_t float_in_int;
	
	if(src_float_ptr == NULL_DATA_PTR || result_float_num_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.03.01", ERROR_OCCURED, system_status_flag); 
		return system_status_flag;
	}
    // Convert mantissa part (23 bits) to corresponding decimal integer 
     float_in_int = IEEE_754_Float_Raw_To_Int(src_float_ptr->ieee_754_format, 9, 31); 
  
    // Assign integer representation of mantissa 
    result_float.raw_ieee_754_format.raw.mantissa = float_in_int; 
  
    // Convert the exponent part (8 bits) to a corresponding decimal integer 
    float_in_int = IEEE_754_Float_Raw_To_Int(src_float_ptr->ieee_754_format, 1, 8); 
  
    // Assign integer representation of the exponent 
    result_float.raw_ieee_754_format.raw.exponient = float_in_int; 
  
    // Assign sign bit 
    result_float.raw_ieee_754_format.raw.sign = ((src_float_ptr->ieee_754_format >> 31) & 0x01 ); 
    memcpy(result_float_num_ptr, &result_float.raw_ieee_754_format.float_num, sizeof(float));
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : IEEE_754_Float_Data_Status

DESCRIPTION    :   
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 15.04 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t IEEE_754_Float_Data_Status(const float_ieee_754_format_t float_ieee_754_format, uint8_t *const float_data_status_ptr) 
{	
     if(float_data_status_ptr == NULL_DATA_PTR)
	 {
		 system_status_flag =  ERR_NULL_PTR;
		 Error_or_Warning_Proc("15.07.01", ERROR_OCCURED, system_status_flag); 
         return system_status_flag;		 
	 }
	 if(float_ieee_754_format.raw_ieee_754_format.raw.exponient == 255)		 
	 {
		 if(float_ieee_754_format.raw_ieee_754_format.raw.mantissa == 0)
		 {
		    if(float_ieee_754_format.raw_ieee_754_format.raw.sign == 1)
		    {
			   *float_data_status_ptr = REAL_VAL_IS_NEGATIVE_INFINITY;
			   return SUCCESS;
		    }
		    *float_data_status_ptr = REAL_VAL_IS_POSITIVE_INFINITY;
		    return SUCCESS;
		 }
		 else
		 {
			if(float_ieee_754_format.raw_ieee_754_format.raw.sign == 1)
		    {
			   *float_data_status_ptr = REAL_VAL_IS_NEGATIVE_NOT_A_NUM;
			   return SUCCESS;
		    }
		    *float_data_status_ptr = REAL_VAL_IS_POSITIVE_NOT_A_NUM;
		    return SUCCESS;
		 }
	 }
	 if(float_ieee_754_format.raw_ieee_754_format.raw.exponient == 0)  
	 {
		 if(float_ieee_754_format.raw_ieee_754_format.raw.mantissa == 0)
		 {
		     if(float_ieee_754_format.raw_ieee_754_format.raw.sign == 1)
		     {
			   *float_data_status_ptr = REAL_VAL_IS_NEGATIVE_ZERO;
			   return SUCCESS;
		     }
		     *float_data_status_ptr = REAL_VAL_IS_POSITIVE_ZERO;
		     return SUCCESS;
		 }
		 else
		 {
			 if(float_ieee_754_format.raw_ieee_754_format.raw.sign == 1)
		     {
			   *float_data_status_ptr = REAL_VAL_IS_NEGATIVE_DENORMALISED;
			   return SUCCESS;
		     }
		     *float_data_status_ptr = REAL_VAL_IS_POSITIVE_DENORMALISED;
		     return SUCCESS;
		 }
	 }
	 if(float_ieee_754_format.raw_ieee_754_format.raw.sign == 1)
	 {
		 *float_data_status_ptr = REAL_VAL_IS_NEGATIVE_NORMALISED;
		 return SUCCESS;
	 }
	 *float_data_status_ptr = REAL_VAL_IS_POSITIVE_NORMALISED;
	 return SUCCESS;
} 

/*------------------------------------------------------------*
FUNCTION NAME  : Int_To_Str

DESCRIPTION    : Converts a given integer num to string str_ptr. 
                 req_num_digits is the number of digits required in output. 
				 If req_num_digits is more than the number of digits in num, 
				 then 0s are added at the beginning. 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 15.05 

Bugs           :  
-*------------------------------------------------------------*/
int16_t Int_To_Str(const int32_t num, char *const str_ptr, const uint8_t req_num_digits) 
{ 
    uint32_t cur_num; 
    uint8_t str_len = 0; 
	
	if(str_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.05.01", ERROR_OCCURED, system_status_flag); 
		return EOF; 
	}
	if(num < 0)
	{
		cur_num = -num;		
	}
	else
	{
		cur_num = num;
	}
    while (cur_num) 
    { 
        str_ptr[str_len++] = (cur_num % BASE_10) + NUM_0_CHAR; 
        cur_num = cur_num / BASE_10; 
    } 
    // If number of digits required is more, then add 0s at the beginning 
    while (str_len < req_num_digits)
	{		
        str_ptr[str_len++] = NUM_0_CHAR; 
	}
    if(num < 0)
    {
     	str_ptr[str_len++] = '-';
    }
    if((Reverse_Str_Use_Same_Str(str_ptr, str_len)) != SUCCESS)
	{
		system_status_flag =  ERR_STR_REVERSE_PROC;
		Error_or_Warning_Proc("15.05.02", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}			
    str_ptr[str_len] = NULL_CHAR; 
    return str_len; 
}
 
/*------------------------------------------------------------*
FUNCTION NAME  : Double_To_Str_Conv

DESCRIPTION    :  Converts a floating point number to string. 
								
INPUT          : 

OUTPUT         : 

NOTE           : Double_To_Str_Conv() function is similar to ftoa()

Func ID        : 15.06 

Bugs           :  
-*------------------------------------------------------------*/
uint16_t Double_To_Str_Conv(const double double_num, char *const double_to_str, uint8_t num_digits_after_point) 
{ 
    // Extract integer part 
     int32_t int_part = (int32_t)double_num; 
  
    // Extract frac part 
    double frac_part = double_num - (double)int_part; 
  
    // convert integer part to string 
    int16_t int_part_str_len, frac_part_str_len ;      
	
  	if(double_to_str == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.06.01", ERROR_OCCURED, system_status_flag); 
		return system_status_flag;
	}
	memset(double_to_str, NULL_CHAR, 1);
	if(num_digits_after_point > MAX_PREC_SPECIFIER)
	{
		num_digits_after_point = MAX_PREC_SPECIFIER;
	}
	if((int_part_str_len = Int_To_Str(int_part, double_to_str, 0)) <= 0)
	{		
		system_status_flag =  ERR_DATA_CONV_PROC;
		Error_or_Warning_Proc("15.06.02", ERROR_OCCURED, system_status_flag); 
		return system_status_flag;
	}
	// check for display option after point 
    if(num_digits_after_point != 0) 
    { 
        double_to_str[int_part_str_len] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. of points after dot.
        // The third parameter is needed to handle cases like 233.007 
        if(frac_part < 0)
        {
        	frac_part = -frac_part;
		}
		
        frac_part = frac_part * (double)Power_Of(BASE_10, num_digits_after_point); 
        if((frac_part_str_len = Int_To_Str((int32_t)frac_part, double_to_str + int_part_str_len + 1, num_digits_after_point)) <= 0)
		{
			system_status_flag =  ERR_DATA_CONV_PROC;
		    Error_or_Warning_Proc("15.06.03", ERROR_OCCURED, system_status_flag); 
		    return system_status_flag;
		}
    } 
    return SUCCESS;
} 

/*------------------------------------------------------------*
FUNCTION NAME  : Str_To_Double_Conv

DESCRIPTION    :  Converts a string to floating point number. 
								
INPUT          : 

OUTPUT         : 

NOTE           : Str_To_Double_Conv() function is similar to atof().
                 float in str is in base 10 and converted float is in base 10.  

Func ID        : 15.07 

Bugs           :  
-*------------------------------------------------------------*/
double Str_To_Double_Conv(const char *const double_to_str) 
{
     double integerPart = 0;
     double fractionPart = 0;
	 int32_t divisorForFraction = 1;
     int16_t sign = 1;
     uint8_t inFraction = STATE_NO;
	 const char *cur_float_str_pos_ptr  = double_to_str;
	 
	 if(cur_float_str_pos_ptr == NULL_DATA_PTR)
	 {
		 system_status_flag =  ERR_NULL_PTR;
		 Error_or_Warning_Proc("15.07.01", ERROR_OCCURED, system_status_flag); 
         return 0.0;
	 }		 
     /*Take care of +/- sign*/
     if (*cur_float_str_pos_ptr == '-')
     {
         ++cur_float_str_pos_ptr;
         sign = -1;
     }
     else if (*cur_float_str_pos_ptr == '+')
     {
         ++cur_float_str_pos_ptr;
     }
     while (*cur_float_str_pos_ptr != NULL_CHAR)
     {
         if (*cur_float_str_pos_ptr >= NUM_0_CHAR && *cur_float_str_pos_ptr <= NUM_9_CHAR)
         {
             if (inFraction == STATE_YES)
             {
                 /*See how are we converting a character to integer*/
                 fractionPart = fractionPart * BASE_10 + (*cur_float_str_pos_ptr - NUM_0_CHAR);
                 divisorForFraction *= BASE_10;
             }
             else
             {
                 integerPart = integerPart * BASE_10 + (*cur_float_str_pos_ptr - NUM_0_CHAR);
             }
         }
         else if (*cur_float_str_pos_ptr == '.')
         {
             if (inFraction == STATE_YES)
                 return sign * (integerPart + fractionPart/divisorForFraction);
             else
                 inFraction = STATE_YES;
         }
         else
         {
             return sign * (integerPart + fractionPart/divisorForFraction);
         }
         ++cur_float_str_pos_ptr;
     }
     return sign * (integerPart + fractionPart/divisorForFraction);
} 

/*------------------------------------------------------------*
FUNCTION NAME  : File_Get_Char

DESCRIPTION    : get char from stream
								
INPUT          : 

OUTPUT         : 

NOTE           : File_Get_Char operation is similar to fgetc()

Func ID        : 15.08 

Bugs           :  
-*------------------------------------------------------------*/
int16_t File_Get_Char(file_t *const fp)
{
	char get_char_arr[2];
	
	if(fp == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.08.01", ERROR_OCCURED, system_status_flag); 
		return (EOF);
	}	
	if(Read_Oper(fp, get_char_arr, 1) == EOF)
	{
		system_status_flag = ERR_DEV_OPER_READ_FUNC ;
		Error_or_Warning_Proc("15.08.02", ERROR_OCCURED, system_status_flag); 
		return (EOF);
	}
	return ((int16_t)get_char_arr[0]);
}

/*------------------------------------------------------------*
FUNCTION NAME  : Read_Oper

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : Read operation is similar to read() in linux

Func ID        : 15.09 

Bugs           :  
-*------------------------------------------------------------*/
int16_t Read_Oper(file_t *const fp, char *const out_buf_ptr, const uint8_t max_num_chars_to_read)
{
	char *out_buf_pos_ptr = out_buf_ptr;
	uint8_t num_chars_read = 0, stream_buf_before_proc_pos;
	
	if(out_buf_ptr == NULL_DATA_PTR || fp == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.09.01", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
    if(fp->stream_io_type != STREAM_IO_INPUT && fp->stream_io_type != STREAM_IO_BOTH)
	{
		system_status_flag =  ERR_FORMAT_INVALID;
		Error_or_Warning_Proc("15.09.02", ERROR_OCCURED, system_status_flag);
		*out_buf_pos_ptr = NULL_CHAR;	
		return EOF;
	}
	if(fp->stream_buf_cur_pos + fp->num_chars_stream_buf >= MAX_STREAM_BUFFER_SIZE)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE ;
		Error_or_Warning_Proc("15.09.03", ERROR_OCCURED, system_status_flag); 
		*out_buf_pos_ptr = NULL_CHAR;	
	    return EOF;
    }
	stream_buf_before_proc_pos = fp->stream_buf_cur_pos;
	if(fp->file_desp == STDIN_FILE_DESP)
	{
		// stdin_keyboard
		#ifdef KEYBOARD_MOD_ENABLE
		   out_buf_pos_ptr =  Get_Str(out_buf_ptr);
		   if(out_buf_pos_ptr == NULL_DATA_PTR)
		   {
			  system_status_flag = ERR_DEV_OPER_READ_FUNC ;
		      Error_or_Warning_Proc("15.09.04", ERROR_OCCURED, system_status_flag); 			  
			  return EOF;
		  }
		  num_chars_read = out_buf_pos_ptr - out_buf_ptr;		 
		#else
           system_status_flag = ERR_MOD_NOT_ENABLED;
		   Error_or_Warning_Proc("15.09.05", ERROR_OCCURED, system_status_flag); 
		   *out_buf_pos_ptr = NULL_CHAR;	
		   return EOF;			
		#endif
	}
	else
	{
	   for(num_chars_read = 0; num_chars_read < max_num_chars_to_read; ++num_chars_read)
	   {
	      if(fp->stream_buf_cur_pos == fp->num_chars_stream_buf || fp->stream_buf[fp->stream_buf_cur_pos] == NULL_CHAR || fp->stream_buf[fp->stream_buf_cur_pos] == ENTER_CHAR)
		  {
		    	break;
		  }
		  *out_buf_pos_ptr++ = fp->stream_buf[fp->stream_buf_cur_pos++];		 
	   }	  
	}	
	*out_buf_pos_ptr = NULL_CHAR;	
    return num_chars_read;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : File_Get_Str

DESCRIPTION    : get string from stream
								
INPUT          : 

OUTPUT         : 

NOTE           : File_Get_Str() operation is similiar to fgets()

Func ID        : 15.10 

Bugs           :  
-*------------------------------------------------------------*/
char *File_Get_Str(char *const dst_str_ptr,  file_t *const fp, const uint8_t max_chars_to_get)
{
	char *char_ptr;
	int16_t num_chars_read;
	
	if(fp == NULL_DATA_PTR || dst_str_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.10.01", ERROR_OCCURED, system_status_flag); 
		return (NULL_DATA_PTR);
	}
	char_ptr = dst_str_ptr;
	if((num_chars_read = Read_Oper(fp, dst_str_ptr, max_chars_to_get)) == EOF)
	{
		system_status_flag = ERR_DEV_OPER_READ_FUNC ;
		Error_or_Warning_Proc("15.10.02", ERROR_OCCURED, system_status_flag); 
		return (NULL_DATA_PTR);
	}	
	char_ptr += num_chars_read;
	return (char_ptr);
}

/*------------------------------------------------------------*
FUNCTION NAME  : VFile_Scan

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : VFile_Scan() operation is similar to vfscanf()

Func ID        : 15.11 

Bugs           :  
-*------------------------------------------------------------*/
int16_t VFile_Scan(file_t *const fp, const char *const fmt, va_list *const scan_arg_list_ptr)
{
    char     in_buf[MAX_STREAM_BUFFER_SIZE + 1];
    int16_t    count;
	
	if(scan_arg_list_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.11.01", ERROR_OCCURED, system_status_flag);
	    return (EOF);
	}
    if (File_Get_Str(in_buf, fp, MAX_STREAM_BUFFER_SIZE) == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_DEV_OPER_READ_FUNC;
		Error_or_Warning_Proc("15.11.02", ERROR_OCCURED, system_status_flag);
	    return (EOF);
	}	
    count = Scan_Data(in_buf, fmt, scan_arg_list_ptr);
	if(count == EOF)
	{
		system_status_flag =  ERR_DEV_OPER_READ_FUNC;
		Error_or_Warning_Proc("15.11.03", ERROR_OCCURED, system_status_flag);
	}
	return (count);
}

/*------------------------------------------------------------*
FUNCTION NAME  : File_Scan

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : File_Scan() operation is similar to fscanf()

Func ID        : 15.12 

Bugs           :  
-*------------------------------------------------------------*/
int16_t File_Scan(file_t *const fp, const char *const fmt, ...)
{
  va_list ap;
  int16_t    count;
	
	if(fmt == NULL_DATA_PTR || fp == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.12.01", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	if(*fmt == NULL_CHAR)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("15.12.02", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
    va_start(ap, fmt);
	count = VFile_Scan(fp, fmt, &ap);
	va_end(ap);
	if(count == EOF)
	{
		system_status_flag =  ERR_DEV_OPER_READ_FUNC;
		Error_or_Warning_Proc("15.12.03", ERROR_OCCURED, system_status_flag); 
	}
	return(count);
}

/*------------------------------------------------------------*
FUNCTION NAME  : Str_Scan

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : Str_Scan() operation is similar to sscanf()

Func ID        : 15.13 

Bugs           :  
-*------------------------------------------------------------*/		    			    
int16_t Str_Scan(const char *const ibuf, const char *const fmt, ...)
{
	va_list ap;
	int16_t count;
	
	if(ibuf == NULL_DATA_PTR )
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.13.01", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	if(fmt == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.13.02", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	if(*ibuf == NULL_CHAR)
	{
		system_status_flag =  ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("15.13.03", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	if(*fmt == NULL_CHAR)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("15.13.04", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
	va_start(ap, fmt);
	count = Scan_Data(ibuf, fmt, &ap);
	va_end(ap);
	if(count == EOF)
	{
		system_status_flag =  ERR_DEV_OPER_READ_FUNC;
		Error_or_Warning_Proc("15.13.05", ERROR_OCCURED, system_status_flag); 
	}
	return(count);
}

/*------------------------------------------------------------*
FUNCTION NAME  : Scan_Data

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : Scan_Data() operation is similar to vsscanf()

Func ID        : 15.14 

Bugs           :  
-*------------------------------------------------------------*/
static int16_t Scan_Data(const char *inp, const char  *const fmt0, va_list *const scan_va_list_ptr)
{
	static const char *Scan_Input_Limit_Chars(char *tab, const char *fmt);
	size_t sum = 0;
	int inr;
	const char *fmt = (const char *)fmt0;
	int c;			/* character from format, or conversion */
	size_t width;		/* field width, or 0 */
	char *p;		/* points into all kinds of strings */
	int n;			/* handy integer */
	int flags;		/* flags as defined above */
	char *p0;		/* saves original value of p when necessary */
	int nassigned;		/* number of fields assigned */
	int nconversions;	/* number of conversions */
	int nread;		/* number of characters consumed from fp */
	int base;		/* base argument to conversion function */
	char ccltab[256];	/* character class table for %[...] */
	char buf[MAX_STREAM_BUFFER_SIZE];		/* buffer for numeric conversions */     
	/* `basefix' is used to avoid `if' tests in the integer scanner */
	static short basefix[17] =
		{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int8_t num_digits_after_point, num_digits_before_point, num_chars_in_buf = 0; 
	
	if(scan_va_list_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.14.01", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	inr = Str_Len(inp);
	if(inr == 0)
	{
		system_status_flag =  ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("15.14.02", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	nassigned = 0;
	nconversions = 0;
	nread = 0;
	base = 0;		/* XXX just to keep gcc happy */
	for (;;)
	{
		c = *fmt++;
		if (c == NULL_CHAR)
			return (nassigned);
		if (isspace(c))
		{
			while (inr > 0 && isspace(*inp))
			{
				nread++, inr--, inp++;
			}
			continue;
		}
		if (c != '%')
			goto literal;
		width = 0;
		flags = 0;
		/*
		 * switch on the format.  continue if done;
		 * break once format type is derived.
		 */
again:		c = *fmt++;
		switch (c)
		{
		case '%':
literal:
			if (inr <= 0)
				goto input_failure;
			if (*inp != c)
				goto match_failure;
			inr--, inp++;
			nread++;
			continue;

		case '*':
			flags |= SUPPRESS;
			goto again;
		case 'l':
			if (flags & LONG)
			{
				flags &= ~LONG;
				flags |= LONGLONG;
			}
			else
				flags |= LONG;
			goto again;
		case 'L':
        	flags |= LONGLONG;	
			goto again;	
		case 'q':
			flags |= LONGLONG;	/* not quite */
			goto again;
		case 'h':
			if (flags & SHORT)
			{
				flags &= ~SHORT;
				flags |= SHORTSHORT;
			}
			else
				flags |= SHORT;
			goto again;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			width = width * 10 + c - '0';
			goto again;

		/*
		 * Conversions.
		 */
		case 'd':
			c = CT_INT;
			base = BASE_10;
			break;

		case 'i':
			c = CT_INT;
			base = 0;
			break;

		case 'o':
			c = CT_INT;
			flags |= UNSIGNED;
			base = BASE_08;
			break;

		case 'u':
			c = CT_INT;
			flags |= UNSIGNED;
			base = BASE_10;
			break;

		case 'X':
		case 'x':
			flags |= PFXOK;	/* enable 0x prefixing */
			c = CT_INT;
			flags |= UNSIGNED;
			base = BASE_16;
			break;
		case 's':
			c = CT_STRING;
			break;
        case 'f':
		    c = CT_REAL;
			flags |= REAL;
			base = BASE_10;			
        break;		
		case '[':
			fmt = Scan_Input_Limit_Chars(ccltab, fmt);
			flags |= NOSKIP;
			c = CT_CCL;
			break;

		case 'c':
			flags |= NOSKIP;
			c = CT_CHAR;
			break;

		case 'p':	/* pointer format is like hex */
			flags |= POINTER | PFXOK;
			c = CT_INT;
			flags |= UNSIGNED;
			base = BASE_16;
			break;

		case 'n':
			nconversions++;
			if (flags & SUPPRESS)	/* ??? */
				continue;
			if (flags & SHORTSHORT)
				*va_arg(*scan_va_list_ptr, char *) = nread;
			else if (flags & SHORT)
				*va_arg(*scan_va_list_ptr, short *) = nread;
			else if (flags & LONG)
				*va_arg(*scan_va_list_ptr, long *) = nread;
			else if (flags & LONGLONG)
				*va_arg(*scan_va_list_ptr, long long *) = nread;
			else
				*va_arg(*scan_va_list_ptr, int *) = nread;
			continue;
		}

		/*
		 * We have a conversion that requires input.
		 */
		if (inr <= 0)
			goto input_failure;

		/*
		 * Consume leading white space, except for formats
		 * that suppress this.
		 */
		if ((flags & NOSKIP) == 0)
		{
			while (isspace(*inp))
			{
				nread++;
				if (--inr > 0)
					inp++;
				else 
					goto input_failure;
			}
			/*
			 * Note that there is at least one character in
			 * the buffer, so conversions that do not set NOSKIP
			 * can no longer result in an input failure.
			 */
		}

		/*
		 * Do the conversion.
		 */
		switch (c)
		{
		   case CT_CHAR:
			/* scan arbitrary characters (sets NOSKIP) */
			if (width == 0)
				width = 1;
			if (flags & SUPPRESS)
			{
      
				memset(va_arg(*scan_va_list_ptr, char *), NULL_CHAR , 1);								
				for (;;)
				{
					if ((n = inr) < (int)width)
					{
						sum += n;
						width -= n;
						inp += n;
						if (sum == 0)
							goto input_failure;
						break;
					} 
					else
					{
						sum += width;
						inr -= width;
						inp += width;
						break;
					}
				}
				nread += sum;
			} 
			else
			{
				memcpy(va_arg(*scan_va_list_ptr, char *), inp, width);
				inr -= width;
				inp += width;
				nread += width;
				nassigned++;
			}
			nconversions++;
			break;

		case CT_CCL:
			/* scan a (nonempty) character class (sets NOSKIP) */
	    	/*	
			   if (width == 0)
				width = (size_t)~0; // size is infinity 
			*/
			if (width == 0 || width > sizeof(buf) - 1)
				width = sizeof(buf) - 1;
			/* take only those things in the class */
			if (flags & SUPPRESS)
			{
				n = 0;	
				memset(va_arg(*scan_va_list_ptr, char *), NULL_CHAR, 1);			
				while (ccltab[(unsigned char)*inp])
				{
					n++, inr--, inp++;
					if (--width == 0)
					{
						break;
					}
					if (inr <= 0)
					{
						if (n == 0)
							goto input_failure;					    						   
						break;
					}
				}
				if (n == 0)
					goto match_failure;					
			}
			else
			{
				p0 = p = va_arg(*scan_va_list_ptr, char *);
				while (ccltab[(unsigned char)*inp])
				{
					inr--;
					*p++ = *inp++;
					if (--width == 0)
						break;
					if (inr <= 0)
					{
						if (p == p0)
							goto input_failure;
						break;
					}
				}
				n = p - p0;
				if (n == 0)
					goto match_failure;
				*p = 0;
				nassigned++;
			}
			nread += n;
			nconversions++;
			break;
		case CT_STRING:
			/* like CCL, but zero-length string OK, & no NOSKIP */
	 	/*	
		    if (width == 0)
				width = (size_t)~0; // size is infinity
		*/
		   	if (width == 0 || width > sizeof(buf) - 1)
				width = sizeof(buf) - 1;	
			if (flags & SUPPRESS)
			{
				n = 0;
				memset(va_arg(*scan_va_list_ptr, char *), NULL_CHAR, 1);
				while (!isspace(*inp))
				{
					n++, inr--, inp++;
					if (--width == 0)
						break;
					if (inr <= 0)
						break;
				}
				nread += n;
			}
			else
			{
				p0 = p = va_arg(*scan_va_list_ptr, char *);
				while (!isspace(*inp))
				{
					inr--;
					*p++ = *inp++;
					if (--width == 0)
						break;
					if (inr <= 0)
						break;
				}
				*p = NULL_CHAR;
				nread += p - p0;
				nassigned++;
			}
			nconversions++;
			continue;
			
		case CT_INT:
			/* scan an integer as if by the conversion function */
/*#ifdef hardway
			if (width == 0 || width > sizeof(buf) - 1)
				width = sizeof(buf) - 1;
#else
			// size_t is unsigned, hence this optimisation 
			if (--width > sizeof(buf) - 2)
				width = sizeof(buf) - 2;
			width++;
#endif */
           if (width == 0 || width > MAX_WIDTH_SPECIFIER)
				width = MAX_WIDTH_SPECIFIER;
			memset(buf, NULL_CHAR, MAX_STREAM_BUFFER_SIZE);
           	flags |= SIGNOK | NDIGITS | NZDIGITS;
			for (p = buf; width; width--)
			{
				c = *inp;
				/*
				 * Switch on the character; `goto int_ok'
				 * if we accept it as a part of number.
				 */
				switch (c)
				{

				/*
				 * The digit 0 is always legal, but is
				 * special.  For %i conversions, if no
				 * digits (zero or nonzero) have been
				 * scanned (only signs), we will have
				 * base==0.  In that case, we should set
				 * it to 8 and enable 0x prefixing.
				 * Also, if we have not scanned zero digits
				 * before this, do not turn off prefixing
				 * (someone else will turn it off if we
				 * have scanned any nonzero digits).
				 */
				case '0':
					if (base == 0)
					{
						base = BASE_08;
						flags |= PFXOK;
					}
					if (flags & NZDIGITS)
					    flags &= ~(SIGNOK|NZDIGITS|NDIGITS);
					else
					    flags &= ~(SIGNOK|PFXOK|NDIGITS);
					goto int_ok;

				/* 1 through 7 always legal */
				case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					base = basefix[base];
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto int_ok;

				/* digits 8 and 9 ok if decimal or hex */
				case '8': case '9':
					base = basefix[base];
					if (base <= BASE_08)
						break;	/* not legal here */
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto int_ok;

				/* letters ok if hex */
				case 'A': case 'B': case 'C':
				case 'D': case 'E': case 'F':
				case 'a': case 'b': case 'c':
				case 'd': case 'e': case 'f':
					/* no need to fix base here */
					if (base <= BASE_10)
						break;	/* not legal here */
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto int_ok;

				/* sign ok only as first character */
				case '+': case '-':
					if (flags & SIGNOK)
					 {
						flags &= ~SIGNOK;
						goto int_ok;
					}
					break;

				/* x ok if flag still set & 2nd char */
				case 'x': case 'X':
					if (flags & PFXOK && p == buf + 1)
					 {
						base = BASE_16;	/* if %i */
						flags &= ~PFXOK;
						goto int_ok;
					}
					break;
				}

				/*
				 * If we got here, c is not a legal character
				 * for a number.  Stop accumulating digits.
				 */
				break;
		int_ok:
				/*
				 * c is legal: store it and look at the next.
				 */
				*p++ = c;
				if (--inr > 0)
					inp++;
				else 
					break;		/* end of input */
			}
			/*
			 * If we had only a sign, it is no good; push
			 * back the sign.  If the number ends in `x',
			 * it was [sign] '0' 'x', so push back the x
			 * and treat it as [sign] '0'.
			 */
			if (flags & NDIGITS)
			 {
				if (p > buf)
				{
					inp--;
					inr++;
				}
				goto match_failure;
			}
			c = ((char *)p)[-1];
			if (c == 'x' || c == 'X')
			 {
				--p;
				inp--;
				inr++;
			}
			if (flags & SUPPRESS)
			{
				if (flags & POINTER)
					*va_arg(*scan_va_list_ptr, void **) = NULL_DATA_PTR;
				else if (flags & SHORTSHORT)
					*va_arg(*scan_va_list_ptr, char *) = NULL_CHAR;
				else if (flags & SHORT)
					*va_arg(*scan_va_list_ptr, short *) = 0;
				else if (flags & LONG)
					*va_arg(*scan_va_list_ptr, long *) = 0;
				else if (flags & LONGLONG)
					*va_arg(*scan_va_list_ptr, long long *) = 0;
				else
					*va_arg(*scan_va_list_ptr, int *) = 0;
				
			}
			else
			{
			    u_quad_t res;

				*p = NULL_CHAR;
				if (flags & UNSIGNED) 
					 res = strtoul(buf, (char **)NULL_DATA_PTR, base);				   
				else
				    res = strtoq(buf, (char **)NULL_DATA_PTR, base);
				if (flags & POINTER)
					*va_arg(*scan_va_list_ptr, void **) = (void *)(uintptr_t)res;
				else if (flags & SHORTSHORT)
					*va_arg(*scan_va_list_ptr, char *) = res;
				else if (flags & SHORT)
					*va_arg(*scan_va_list_ptr, short *) = res;
				else if (flags & LONG)
					*va_arg(*scan_va_list_ptr, long *) = res;
				else if (flags & LONGLONG)
					*va_arg(*scan_va_list_ptr, long long *) = res;
				else
					*va_arg(*scan_va_list_ptr, int *) = res;
				nassigned++; 	
			}
			nread += p - buf;
			nconversions++;
		break;
        case CT_REAL:
		     if (width == 0 || width > MAX_WIDTH_SPECIFIER + 1 + MAX_PREC_SPECIFIER)				 
				width = MAX_WIDTH_SPECIFIER +  1 + MAX_PREC_SPECIFIER;
			num_digits_after_point = 0;
			num_digits_before_point = 0;
			num_chars_in_buf = 0;
			memset(buf, NULL_CHAR, MAX_STREAM_BUFFER_SIZE);
			flags |= SIGNOK | DPTOK |NDIGITS| NZDIGITS;
			for (p = buf; width; width--)
			{
				c = *inp;
				switch(c)
				{
					case '0': case '1':	case '2': case '3':
				    case '4': case '5': case '6': case '7':	
                    case '8': case '9':	
					  if(flags & NZDIGITS)
					  {
					      flags &= ~(SIGNOK | NDIGITS | NZDIGITS);						  
					  }
					  if(flags & DPTOK)
					  {
					  	 if(++num_digits_before_point > MAX_WIDTH_SPECIFIER)
						 {
							  goto match_failure;  							  
						 }
                         else
						 {
							  buf[num_chars_in_buf++] = c; 
						 }						  
					  }
					  else
					  {
						  if(++num_digits_after_point > MAX_PREC_SPECIFIER)
						  {
							  buf[num_chars_in_buf] = NULL_CHAR;
							  //discard extra numeric data if precison exceeds limit
						  }
                          else
						  {
							 buf[num_chars_in_buf++] = c; 
						  }	
					  }
					  goto real_ok;
				   //break;
				   case '+': 
				   case '-':
					 if (flags & SIGNOK )
					 {
						flags &= ~(SIGNOK);
						if(c == '-')
						{
					    	buf[num_chars_in_buf++] = '-';							
						}
						goto real_ok;
					 }
				   break;
				   case '.':
				     if (flags & DPTOK)
					 {
						flags &= ~(DPTOK);
						buf[num_chars_in_buf++] = '.';
						goto real_ok;
					 }
				   break; 
                   			
				}
				/*
				 * If we got here, c is not a legal character
				 * for a number.  Stop accumulating digits.
				 */
				break;
		real_ok:
				/*
				 * c is legal: store it and look at the next.
				 */
				*p++ = c;
				if (--inr > 0)
					inp++;
				else 
					break;		/* end of input */
			}	
			//If we had only a sign, it is no good;
			if (flags & NDIGITS)
			{
			     goto match_failure;
			}
			if(flags & SUPPRESS)
			{
				if (flags & LONG)
					*va_arg(*scan_va_list_ptr, double *) = 0.0;
				else if (flags & LONGLONG)
					*va_arg(*scan_va_list_ptr, long double *) = 0.0;
				else
					*va_arg(*scan_va_list_ptr, float *) = 0.0;				
			}
			else
			{
				real_t res;
			    *p = NULL_CHAR;
				res = Str_To_Double_Conv(buf);
				if (flags & LONG)
					*va_arg(*scan_va_list_ptr, double *) = res;
				else if (flags & LONGLONG)
					*va_arg(*scan_va_list_ptr, long double *) = res ; 
				else
					*va_arg(*scan_va_list_ptr, float *) = res;
				nassigned++;
			}
			nread += p - buf;
			nconversions++;
		break;
		}
	}
input_failure:
	return (nconversions != 0 ? nassigned : EOF);
match_failure:
	return (nassigned);
}

/*------------------------------------------------------------*
FUNCTION NAME  : Scan_Input_Limit_Chars

DESCRIPTION    : Fill in the given table from the scanset at the given format
                 (just after `[').  Return a pointer to the character past the
                  closing `]'.  The table has a 1 wherever characters should be
                 considered part of the scanset.
								
INPUT          : 

OUTPUT         : 

NOTE           : Scan_Input_Limit_Chars operation is similar to __sccl()

Func ID        : 15.15 

Bugs           :  
-*------------------------------------------------------------*/
 static const char *Scan_Input_Limit_Chars(char *tab, const char *fmt)
{
	int c, n, v;

	/* first `clear' the whole table */
	c = *fmt++;		/* first char hat => negated scanset */
	if (c == '^')
    {
		v = 1;		/* default => accept */
		c = *fmt++;	/* get new first char */
	} 
	else
	{
		v = 0;		/* default => reject */
	}

	/* XXX: Will not work if sizeof(tab*) > sizeof(char) */
	(void) memset(tab, v, 256);

	if (c == 0)
	{
		return (fmt - 1);/* format ended before closing ] */
	}

	/*
	 * Now set the entries corresponding to the actual scanset
	 * to the opposite of the above.
	 *
	 * The first character may be ']' (or '-') without being special;
	 * the last character may be '-'.
	 */
	v = 1 - v;
	for (;;)
	{
		tab[c] = v;		/* take character c */
doswitch:
		n = *fmt++;		/* and examine the next */
		switch (n)
		{

		case 0:			/* format ended too soon */
			return (fmt - 1);

		case '-':
			/*
			 * A scanset of the form
			 *	[01+-]
			 * is defined as `the digit 0, the digit 1,
			 * the character +, the character -', but
			 * the effect of a scanset such as
			 *	[a-zA-Z0-9]
			 * is implementation defined.  The V7 Unix
			 * scanf treats `a-z' as `the letters a through
			 * z', but treats `a-a' as `the letter a, the
			 * character -, and the letter a'.
			 *
			 * For compatibility, the `-' is not considerd
			 * to define a range if the character following
			 * it is either a close bracket (required by ANSI)
			 * or is not numerically greater than the character
			 * we just stored in the table (c).
			 */
			n = *fmt;
			if (n == ']' || n < c)
			{
				c = '-';
				break;	/* resume the for(;;) */
			}
			fmt++;
			/* fill in the range */
			do
			{
			    tab[++c] = v;
			} while (c < n);
			c = n;
			/*
			 * Alas, the V7 Unix scanf also treats formats
			 * such as [a-c-e] as `the letters a through e'.
			 * This too is permitted by the standard....
			 */
			goto doswitch;
//		break;

		case ']':		/* end of scanset */
			return (fmt);

		default:		/* just another character */
			c = n;
			break;
		}
	}
	/* NOT REACHED */
	return NULL_DATA_PTR;
}

/*------------------------------------------------------------*
FUNCTION NAME  : File_Put_Char

DESCRIPTION    : put char into stream
								
INPUT          : 

OUTPUT         : 

NOTE           : File_Put_Char() operation is similar to fputc()

Func ID        : 15.16 

Bugs           :  
-*------------------------------------------------------------*/
int16_t File_Put_Char(const char put_char, file_t *const fp)
{
	int16_t num_chars_written;
	char put_char_arr[2];
	
	if(fp == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.16.01", ERROR_OCCURED, system_status_flag); 
		return (EOF);
	}
	put_char_arr[0] = put_char;
	put_char_arr[1] = NULL_CHAR;
	if((num_chars_written = Write_Oper(fp, put_char_arr, 1)) == EOF)
	{
		system_status_flag = ERR_DEV_OPER_WRITE_FUNC ;
		Error_or_Warning_Proc("15.16.02", ERROR_OCCURED, system_status_flag); 
		return (EOF);
	}
	if(num_chars_written == 0)
	{
		return 0;
	}
	return ((int16_t )put_char);
}

/*------------------------------------------------------------*
FUNCTION NAME  : Write_Oper

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : Write_Oper() operation is similar to write() in linux

Func ID        : 15.17

Bugs           :  
-*------------------------------------------------------------*/
int16_t Write_Oper(file_t *const fp, const char *const in_buf_ptr, const uint8_t max_num_chars_to_write)
{
	const char *in_buf_pos_ptr = in_buf_ptr;
	int16_t num_chars_written = 0, stream_buf_before_proc_pos;
	
	if(in_buf_ptr == NULL_DATA_PTR || fp == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.17.01", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
    if(fp->stream_io_type != STREAM_IO_OUTPUT && fp->stream_io_type != STREAM_IO_BOTH)
	{
		system_status_flag =  ERR_FORMAT_INVALID;
		Error_or_Warning_Proc("15.17.02", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	stream_buf_before_proc_pos = fp->stream_buf_cur_pos;
	if(*in_buf_pos_ptr == NULL_CHAR)
	{
		system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.17.03", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
	if(fp->num_chars_stream_buf == 0)
	{
		return 0;
    }
	if(fp->file_desp == STDOUT_FILE_DESP)
	{
		// stdout_vt	
        #ifdef UART_MOD_ENABLE		
		   if((Put_Char(*in_buf_pos_ptr)) == NULL_CHAR)
		   {
		   	   system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		       Error_or_Warning_Proc("15.17.05", ERROR_OCCURED, system_status_flag); 
			   return EOF; 
		   }		   
		   ++num_chars_written;		   
		  #else
		     system_status_flag = ERR_MOD_NOT_ENABLED;
		     Error_or_Warning_Proc("15.17.06", ERROR_OCCURED, system_status_flag); 
		     return EOF; 
		  #endif
	}
	else
	{
	    for(num_chars_written = 0; num_chars_written < max_num_chars_to_write; ++num_chars_written)
	    {
	    	if((fp->stream_buf_cur_pos == fp->num_chars_stream_buf) || *in_buf_pos_ptr == NULL_CHAR)
		    {
		    	break;
		    }
		    fp->stream_buf[fp->stream_buf_cur_pos++] = *in_buf_pos_ptr++;		 
	    } 
	}
	memset(fp->stream_buf + stream_buf_before_proc_pos, NULL_CHAR, num_chars_written);
	fp->num_chars_stream_buf -= num_chars_written;	
    return num_chars_written;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : File_Put_Str

DESCRIPTION    : put string into stream
								
INPUT          : 

OUTPUT         : 

NOTE           : File_Put_Str() operation is similiar to fputs()

Func ID        : 15.18 

Bugs           :  
-*------------------------------------------------------------*/
int16_t File_Put_Str(const char *const io_buf_ptr, file_t *const fp)
{
   const char *char_ptr = io_buf_ptr;
   uint8_t num_chars_written = 0;
  
	if(fp == NULL_DATA_PTR)
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("15.18.01", ERROR_OCCURED, system_status_flag); 
		return EOF;
	}
    while(*char_ptr != NULL_CHAR)
	{
		if((File_Put_Char(*char_ptr, fp)) == EOF)
		{
			system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		    Error_or_Warning_Proc("15.18.02", ERROR_OCCURED, system_status_flag);            				
			break;
		}
		++char_ptr;
		++num_chars_written;
	}
	if(num_chars_written == 0)
	{
		return EOF;		
	}
	return (num_chars_written);
}

/*------------------------------------------------------------*
FUNCTION NAME  : VFile_Print

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : VFile_Print() operation is similar to vfprintf()

Func ID        : 15.19 

Bugs           :  
-*------------------------------------------------------------*/
int16_t VFile_Print(file_t *const fp, const char *const fmt, va_list *const print_va_list_ptr)
{
    int16_t num_chars_printed;
	    
    num_chars_printed = Print_Data(print_va_list_ptr, fp, fmt);
	if(num_chars_printed == EOF)
	{
		system_status_flag =  ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.19.01", ERROR_OCCURED, system_status_flag);
		return (EOF);
	}
	if(num_chars_printed == 0)
	{
		return 0;
	}
	if(fp->stream_buf_cur_pos + fp->num_chars_stream_buf + num_chars_printed + 1 >= MAX_STREAM_BUFFER_SIZE)
	{
		system_status_flag =  ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.19.02", ERROR_OCCURED, system_status_flag);
	    return (EOF);
	}
	fp->num_chars_stream_buf += (num_chars_printed);
	if (File_Put_Str((fp->stream_buf + fp->stream_buf_cur_pos), fp) == EOF)
	{
		system_status_flag =  ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.19.03", ERROR_OCCURED, system_status_flag);
	    return (EOF);
	}
	if(fp->file_desp == STDOUT_FILE_DESP)
	{
		if((File_Flush(fp)) != SUCCESS)
		{
			system_status_flag = ERR_FILE_FLUSH_PROC;
		    Error_or_Warning_Proc("15.19.04", ERROR_OCCURED, system_status_flag);
			return (EOF);
		}
	}		
    return (num_chars_printed);
}

/*------------------------------------------------------------*
FUNCTION NAME  : File_Print

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : File_Print() operation is similar to fprintf()

Func ID        : 15.20 

Bugs           :  
-*------------------------------------------------------------*/
int16_t File_Print(file_t *const fp, const char *const format_ptr, ...)
{
    va_list arg;
	 int16_t num_chars_printed;	
		
	if(fp == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.20.01", ERROR_OCCURED, system_status_flag);
	    return (EOF);		
	}
	if(format_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.20.02", ERROR_OCCURED, system_status_flag);
	    return (EOF);
	}
	if(*format_ptr == NULL_CHAR)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("15.20.03", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
	va_start(arg, format_ptr); 
	num_chars_printed = VFile_Print(fp, format_ptr, &arg );
	va_end(arg);
	if(num_chars_printed == EOF)
	{
		system_status_flag =  ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.20.04", ERROR_OCCURED, system_status_flag);
	    return (EOF);
	}
	return num_chars_printed;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Str_Print

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : Str_Print() operation is similar to sprintf()
                 assuming sizeof(void *) == sizeof(int)   

Func ID        : 15.21 

Bugs           :  
-*------------------------------------------------------------*/
int16_t Str_Print(char *out_str, const char *const format_ptr, ...)
{
	file_t out_str_file;
	va_list arg;
	int16_t num_chars_printed;	
		
	if(out_str == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.21.01", ERROR_OCCURED, system_status_flag);
	    return (EOF);
	}
	if(format_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.21.02", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
	if(*format_ptr == NULL_CHAR)
	{
		system_status_flag = ERR_DATA_OUT_OF_RANGE;
		Error_or_Warning_Proc("15.21.03", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
	File_Flush(&out_str_file);
	out_str_file.stream_type = STREAM_TYPE_NA;
	out_str_file.stream_io_type = STREAM_IO_OUTPUT;
	out_str_file.file_desp = STR_FILE_DESP;
	va_start(arg, format_ptr); 
	num_chars_printed = Print_Data(&arg, &out_str_file, format_ptr);
	va_end(arg);
	if(num_chars_printed == EOF)
	{
		system_status_flag =  ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.21.04", ERROR_OCCURED, system_status_flag);
		return EOF;
	}
	if(num_chars_printed == 0)
	{
		return 0;
	}
	memcpy(out_str, out_str_file.stream_buf, num_chars_printed);
	out_str[num_chars_printed + 1] = NULL_CHAR;
	return num_chars_printed;	
}

/*------------------------------------------------------------*
FUNCTION NAME  : Print_Data

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 15.22 

Bugs           :  
-*------------------------------------------------------------*/
static int16_t Print_Data(va_list *const print_va_list_ptr, file_t *const out_file_ptr, const char *const format_ptr)
{
	const char *transverse_ptr = format_ptr;
	char *print_str, *out_stream_buf_ptr;
	float float_print_data;
	uint32_t uint32_print_data; 
	int32_t int32_print_data; 
	uint16_t uint16_print_data; 
	int16_t int16_print_data, num_chars_printed = 0, ret_status;	
    uint8_t width_spec, pad_format, num_digits_after_point, proc_bit_field = 0; 
	char char_print_data;
	char char_in_str_arr[2];
    
    if(print_va_list_ptr == NULL_DATA_PTR || out_file_ptr == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.22.01", ERROR_OCCURED, system_status_flag);
		return EOF;
	}	
	out_stream_buf_ptr = out_file_ptr->stream_buf + out_file_ptr->stream_buf_cur_pos;	
	
	for (; *transverse_ptr != NULL_CHAR; ++transverse_ptr)
	{
		if (*transverse_ptr == '%')
		{
			++transverse_ptr;
			width_spec = pad_format = num_digits_after_point = 0;
			proc_bit_field &= ~(1 << 0); 
			if (*transverse_ptr == NULL_CHAR)
			{
				*out_stream_buf_ptr = NULL_CHAR;	            
				return num_chars_printed;
			}
			if (*transverse_ptr == '%')
			{
				if((Print_Char(&out_stream_buf_ptr, *transverse_ptr)) == NULL_CHAR)
				{
					*out_stream_buf_ptr = NULL_CHAR;
	                return num_chars_printed;
				}
			    ++num_chars_printed;
				continue;
			}
			if (*transverse_ptr == '-')
			{
				++transverse_ptr;
				pad_format = PAD_RIGHT;
			}
			while (*transverse_ptr == '0')
			{
				++transverse_ptr;
				pad_format |= PAD_ZERO;
			}
			for(; *transverse_ptr >= '0' && *transverse_ptr <= '9';	++transverse_ptr)
			{
			    width_spec *= 10;
			    width_spec += *transverse_ptr - '0';
			    if(width_spec > MAX_WIDTH_SPECIFIER)
			    {
			        width_spec = MAX_WIDTH_SPECIFIER;
			        for(;*transverse_ptr >= '0' && *transverse_ptr <= '9'; ++transverse_ptr);
				    break;
		    	}
			}
			
		    if(*transverse_ptr == '.')
			{
				++transverse_ptr;
				proc_bit_field |= (1 << 0); 
			}
			if((proc_bit_field & (1 << 0) ))
			{
				for(;*transverse_ptr >= '0' && *transverse_ptr <= '9';  ++transverse_ptr)	
			    {
			        num_digits_after_point *= 10;
			        num_digits_after_point += *transverse_ptr - '0';
			        if(num_digits_after_point > MAX_PREC_SPECIFIER)
			        {
			           num_digits_after_point = MAX_PREC_SPECIFIER;
			           for(;*transverse_ptr >= '0' && *transverse_ptr <= '9'; ++transverse_ptr);
				       break;
		        	}
		    	}	
			}
		    if(*transverse_ptr == 'c' )
			{
				/* char are converted to int then pushed on the stack */
				char_print_data = (char) va_arg(*print_va_list_ptr, int32_t);		
		       	char_in_str_arr[0] = char_print_data;
				char_in_str_arr[1] = NULL_CHAR;
				if((ret_status = Print_Str(&out_stream_buf_ptr, char_in_str_arr, width_spec, pad_format)) == 0)
				{
					 system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
					 Error_or_Warning_Proc("15.22.02", ERROR_OCCURED, system_status_flag);
					 *out_stream_buf_ptr = NULL_CHAR;
	                 return num_chars_printed;
				}
				num_chars_printed += ret_status;
				continue;
			}
			if(*transverse_ptr == 's')
			{		
				  print_str = va_arg(*print_va_list_ptr, char *);
				  if((ret_status = Print_Str(&out_stream_buf_ptr, print_str ? print_str: NULL_DATA_PTR, width_spec, pad_format)) == 0)
				  {
					   system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		               Error_or_Warning_Proc("15.22.03", ERROR_OCCURED, system_status_flag);
					   *out_stream_buf_ptr = NULL_CHAR;
	                   return num_chars_printed;
				  } 
				  num_chars_printed += ret_status;
				  continue;
			}
			if(*transverse_ptr == 'b' )
			{
				uint32_print_data = va_arg(*print_va_list_ptr, uint32_t); 
				if((ret_status = Print_Num(&out_stream_buf_ptr, uint32_print_data, width_spec, BASE_02 | STATE_NO << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_ALPHA_NA << ALPHA_BIT_POS)) == 0)
				{
					 system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		             Error_or_Warning_Proc("15.22.04", ERROR_OCCURED, system_status_flag);
					 *out_stream_buf_ptr = NULL_CHAR;
	                 return num_chars_printed;
				}
				num_chars_printed += ret_status;
				continue;
			}			
			if(*transverse_ptr == 'o' )
			{
				uint32_print_data = va_arg(*print_va_list_ptr, uint32_t); 
				if((ret_status = Print_Num(&out_stream_buf_ptr, uint32_print_data, width_spec, BASE_08 | STATE_NO << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_ALPHA_NA << ALPHA_BIT_POS)) == 0)
				{
					 system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
					 Error_or_Warning_Proc("15.22.05", ERROR_OCCURED, system_status_flag);
					 *out_stream_buf_ptr = NULL_CHAR;
	                 return num_chars_printed;
				}
				num_chars_printed += ret_status;
				continue;
			}
			if(*transverse_ptr == 'x')	
			{   
		        uint32_print_data = va_arg(*print_va_list_ptr, uint32_t); 
		        if((ret_status = Print_Num(&out_stream_buf_ptr, uint32_print_data, width_spec, BASE_16 | STATE_NO << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_SMALL_ALPHA << ALPHA_BIT_POS)) == 0)
				{
					 system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		             Error_or_Warning_Proc("15.22.06", ERROR_OCCURED, system_status_flag);
					 *out_stream_buf_ptr = NULL_CHAR;
	                 return num_chars_printed;
				}
				num_chars_printed += ret_status;
				continue;
			}
			if(*transverse_ptr == 'X')
			{	
		        uint32_print_data = va_arg(*print_va_list_ptr, uint32_t); 
		        if((ret_status = Print_Num(&out_stream_buf_ptr, uint32_print_data, width_spec, BASE_16 | STATE_NO << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_BIG_ALPHA << ALPHA_BIT_POS )) == 0)
				{
					 system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		             Error_or_Warning_Proc("15.22.07", ERROR_OCCURED, system_status_flag);
					 *out_stream_buf_ptr = NULL_CHAR;
	                 return num_chars_printed;
				}
				 num_chars_printed += ret_status;
				continue;
			}
			if(*transverse_ptr == 'h' )
			{
				++transverse_ptr;
			    switch(*transverse_ptr)
			    {
			        case 'u':
					   uint16_print_data = (uint16_t)va_arg(*print_va_list_ptr, uint32_t); 
				       if((ret_status = Print_Num(&out_stream_buf_ptr, uint16_print_data, width_spec, BASE_10 | STATE_NO << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_ALPHA_NA << ALPHA_BIT_POS)) == 0)
				       {
						   system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		                   Error_or_Warning_Proc("15.22.07", ERROR_OCCURED, system_status_flag);
						   *out_stream_buf_ptr = NULL_CHAR;
	                       return num_chars_printed;
				       }
					break;
					default:
				       int16_print_data = (int16_t)va_arg(*print_va_list_ptr, int32_t); 
				       if((ret_status = Print_Num(&out_stream_buf_ptr, int16_print_data, width_spec, BASE_10 | STATE_YES << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_ALPHA_NA << ALPHA_BIT_POS)) == 0)
				       {
						   system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		                   Error_or_Warning_Proc("15.22.08", ERROR_OCCURED, system_status_flag);
						   *out_stream_buf_ptr = NULL_CHAR;
	                       return num_chars_printed;
				       }
			           --transverse_ptr;
				}
				num_chars_printed += ret_status;
				continue;
			}
			if(*transverse_ptr == 'u' )
			{
				uint32_print_data = va_arg(*print_va_list_ptr, uint32_t); 
				if((ret_status = Print_Num(&out_stream_buf_ptr, uint32_print_data, width_spec, BASE_10 | STATE_NO << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_ALPHA_NA << ALPHA_BIT_POS)) == 0)
				{
					system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		            Error_or_Warning_Proc("15.22.09", ERROR_OCCURED, system_status_flag);
					*out_stream_buf_ptr = NULL_CHAR;
	                return num_chars_printed;
				}
				num_chars_printed += ret_status;
				continue;
			}
			if(*transverse_ptr == 'd' || *transverse_ptr == 'i')
			{	
                 int32_print_data = va_arg(*print_va_list_ptr, int32_t);		
                 if((ret_status = Print_Num(&out_stream_buf_ptr, int32_print_data, width_spec, BASE_10 | STATE_YES << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_ALPHA_NA << ALPHA_BIT_POS)) == 0)
				 {
					 system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		             Error_or_Warning_Proc("15.22.10", ERROR_OCCURED, system_status_flag);
					 *out_stream_buf_ptr = NULL_CHAR;
					 return num_chars_printed;
				 }					 
			  	 num_chars_printed += ret_status;
				 continue;
			}
			if(*transverse_ptr == 'f' )
			{
				float_print_data = (float)va_arg(*print_va_list_ptr, double); 
				if(((proc_bit_field & (1 << 0)) == 0))
				{
					//default max digits after point if '.'  operator for max precision spec is not specified
					num_digits_after_point = 6;
				}
				if((ret_status = Print_Float(&out_stream_buf_ptr, float_print_data, width_spec, num_digits_after_point, BASE_10 | STATE_YES << SIGN_FLAG_BIT_POS | pad_format << PAD_FORMAT_BIT_POS | NUM_CONV_ALPHA_NA << ALPHA_BIT_POS)) == 0)
				{
					system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		            Error_or_Warning_Proc("15.22.11", ERROR_OCCURED, system_status_flag);
		    		*out_stream_buf_ptr = NULL_CHAR; 
				   	return num_chars_printed;
				}
				num_chars_printed += ret_status;
				continue;
			}
		}
		else
		{
			if((Print_Char(&out_stream_buf_ptr, *transverse_ptr)) == NULL_CHAR)
			{
				system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		        Error_or_Warning_Proc("15.22.12", ERROR_OCCURED, system_status_flag);
				*out_stream_buf_ptr = NULL_CHAR;
				return num_chars_printed;
			}
			++num_chars_printed;
			if(*transverse_ptr == ENTER_CHAR)
			{
				if(out_file_ptr->file_desp == STDOUT_FILE_DESP)
				{
					if(out_file_ptr->stream_buf_cur_pos + out_file_ptr->num_chars_stream_buf + num_chars_printed + 1 >= MAX_STREAM_BUFFER_SIZE)
	                {
	                	system_status_flag =  ERR_DEV_OPER_WRITE_FUNC;
	                	Error_or_Warning_Proc("15.22.13", ERROR_OCCURED, system_status_flag);
	                    return (EOF);
	                }
	                out_file_ptr->num_chars_stream_buf += (num_chars_printed);
	                if(File_Put_Str((out_file_ptr->stream_buf + out_file_ptr->stream_buf_cur_pos), out_file_ptr) == EOF)
	                {
		               system_status_flag =  ERR_DEV_OPER_WRITE_FUNC;
		               Error_or_Warning_Proc("15.22.14", ERROR_OCCURED, system_status_flag);
	                   return (EOF);
	                }
   					if((File_Flush(out_file_ptr)) != SUCCESS)
					{
						system_status_flag = ERR_FILE_FLUSH_PROC;
		                Error_or_Warning_Proc("15.22.15", ERROR_OCCURED, system_status_flag);
				        *out_stream_buf_ptr = NULL_CHAR;
				        return num_chars_printed;
					}
					out_stream_buf_ptr = out_file_ptr->stream_buf + out_file_ptr->stream_buf_cur_pos;	
					num_chars_printed = 0;
				}
			}			
		}
	}
	*out_stream_buf_ptr = NULL_CHAR;	
	return num_chars_printed;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Print_Char

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           :   

Func ID        : 15.23 

Bugs           :  
-*------------------------------------------------------------*/
static char Print_Char(char **const out_str_ptr, const char print_char)
{
	char char_proc;
	
	if(out_str_ptr)
	{
		char_proc = print_char;
		**out_str_ptr = print_char;
		++(*out_str_ptr);
	}
	return char_proc;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Print_Str

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           :   

Func ID        : 15.24 

Bugs           :  
-*------------------------------------------------------------*/
static int16_t Print_Str(char **const out_str_ptr, const char *print_str, const uint8_t conf_width_spec, const uint8_t pad_format)
{
	int16_t num_chars_printed = 0;
	int8_t width_spec = conf_width_spec;
    char pad_char = ' ' ;
    const char *print_char_ptr = print_str;
    uint8_t str_len = 0;
	
	if(print_str == NULL_DATA_PTR)
	{
		system_status_flag =  ERR_NULL_PTR;
		Error_or_Warning_Proc("15.24.01", ERROR_OCCURED, system_status_flag);
		return 0;		
	}
	if (width_spec > 0)
	{
		str_len = Str_Len(print_str);
		if(str_len == 0)
		{
			system_status_flag =  ERR_DATA_OUT_OF_RANGE;
		    Error_or_Warning_Proc("15.24.01", ERROR_OCCURED, system_status_flag);
		    return 0;
		}
		if (str_len >= width_spec) 
		{
			width_spec = 0;
		}
		else 
		{ 
	       width_spec -= str_len;
		}
		if (pad_format & PAD_ZERO) 
		{ 
	        pad_char = '0';
		}
	}
	if (!(pad_format & PAD_RIGHT))
	{
		// pad_format left justified
		for ( ;width_spec > 0; --width_spec)
		{
			if((Print_Char(out_str_ptr, pad_char)) == NULL_CHAR)
			{
				return num_chars_printed;
			}
			++num_chars_printed;
		}
	}
	for ( ;*print_char_ptr; ++print_char_ptr)
	{
		if((Print_Char(out_str_ptr, *print_char_ptr)) == NULL_CHAR)
		{
			return num_chars_printed;
		}
		++num_chars_printed;
	}
	for ( ; width_spec > 0; --width_spec)
	{
		if((Print_Char(out_str_ptr, pad_char)) == NULL_CHAR)
		{
			return num_chars_printed;
		}
		++num_chars_printed;
	}
	return num_chars_printed;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Print_Num

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           :   

Func ID        : 15.25 

Bugs           :  
-*------------------------------------------------------------*/
static int16_t Print_Num(char **const out_str_ptr, const int32_t print_num, const uint8_t conf_width_spec, const uint16_t ctrl_flag )
{
	char print_buf[PRINT_BUF_LEN];
	char *cur_print_buf_ptr;
	uint32_t cur_unsign_print_num = (uint32_t)print_num;
	int32_t cur_digit; 
	int16_t num_chars_printed = 0, ret_status;
    char alpha_start_char;
	uint8_t num_negative_flag = STATE_NO, width_spec = conf_width_spec;
    const uint8_t base = ctrl_flag & 0x1F, sign_flag = (ctrl_flag >> SIGN_FLAG_BIT_POS) & 0x01, 
	   pad_format = (ctrl_flag >> PAD_FORMAT_BIT_POS) & 0x03, alpha_print_flag = (ctrl_flag >> ALPHA_BIT_POS ) & 0x03;
	
	if(print_num == 0)
	{
		print_buf[0] = '0';
		print_buf[1] = NULL_CHAR;
		return Print_Str(out_str_ptr, print_buf, width_spec, pad_format);
	}
	if(sign_flag == STATE_YES && base == BASE_10 && print_num < 0)
	{
		num_negative_flag = STATE_YES;
		cur_unsign_print_num = -print_num;
	}
	if(base > BASE_10)
	{
		switch(alpha_print_flag)
		{
			case NUM_CONV_BIG_ALPHA:
			   alpha_start_char = 'A';
			break;
			case NUM_CONV_SMALL_ALPHA:
			   alpha_start_char = 'a';
			break;
			default:
		    	system_status_flag =  ERR_FORMAT_INVALID;
		        Error_or_Warning_Proc("15.25.01", ERROR_OCCURED, system_status_flag);
		        return 0;
		}
	}
	cur_print_buf_ptr = print_buf + PRINT_BUF_LEN - 1;
	*cur_print_buf_ptr = NULL_CHAR;
	while(cur_unsign_print_num)
	{
		cur_digit = cur_unsign_print_num % base;
		if( cur_digit >= 10 )
		{
			cur_digit += alpha_start_char - '0' - 10;
		}
		*--cur_print_buf_ptr = cur_digit + '0';
		cur_unsign_print_num /= base;
	}
	if(num_negative_flag == STATE_YES)
	{
		if( width_spec && (pad_format & PAD_ZERO) ) 
		{
			if((Print_Char(out_str_ptr, '-')) == NULL_CHAR)
			{
				return num_chars_printed;
			}
			++num_chars_printed;
			--width_spec;
		}
		else
		{
			*--cur_print_buf_ptr = '-';
		}
	}
	if((ret_status = Print_Str(out_str_ptr, cur_print_buf_ptr, width_spec, pad_format)) == 0)
	{
		system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.25.02", ERROR_OCCURED, system_status_flag);
		return num_chars_printed;		
	}
    return num_chars_printed + ret_status;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Print_Float

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           :   

Func ID        : 15.26 

Bugs           :  
-*------------------------------------------------------------*/
static int16_t Print_Float(char **const out_str_ptr, const float print_float, const uint8_t conf_width_spec, const uint8_t conf_num_digits_after_point, const uint16_t ctrl_flag )
{
	char print_buf[PRINT_BUF_LEN];
	char *cur_print_buf_ptr;
	float_ieee_754_format_t result_float;
	float result_float_num;
	uint32_t  cur_unsign_print_num;
    int32_t int_part;	
    float frac_part; 
	int16_t num_chars_printed = 0, ret_status;
    char alpha_start_char;
	uint8_t proc_bit_field = 0, width_spec = conf_width_spec, cur_digit;
    const uint8_t base = ctrl_flag & 0x1F, sign_flag = (ctrl_flag >> SIGN_FLAG_BIT_POS) & 0x01, 
	   pad_format = (ctrl_flag >> PAD_FORMAT_BIT_POS) & 0x03, alpha_print_flag = (ctrl_flag >> ALPHA_BIT_POS ) & 0x03;
	int8_t num_digits_after_point = conf_num_digits_after_point;
	
	if(base > BASE_10)
	{
		switch(alpha_print_flag)
		{
			case NUM_CONV_BIG_ALPHA:
			   alpha_start_char = 'A';
			break;
			case NUM_CONV_SMALL_ALPHA:
			   alpha_start_char = 'a';
			break;
			default:
			   system_status_flag =  ERR_FORMAT_INVALID;
		       Error_or_Warning_Proc("15.26.01", ERROR_OCCURED, system_status_flag);
			   return 0;
		}
	}
	if((Real_Val_To_IEEE_754_Float(print_float, &result_float)) != SUCCESS)
	{
		system_status_flag = ERR_DATA_CONV_PROC ;
		Error_or_Warning_Proc("15.26.02", ERROR_OCCURED, system_status_flag);
		return 0;
	}
	if((IEEE_754_Float_To_Real_Val(&result_float, &result_float_num)) != SUCCESS)
	{
		system_status_flag = ERR_DATA_CONV_PROC ;
		Error_or_Warning_Proc("15.26.03", ERROR_OCCURED, system_status_flag);
		return 0;
	}
	memset(print_buf, NULL_CHAR, PRINT_BUF_LEN);
	cur_print_buf_ptr = print_buf + PRINT_BUF_LEN - 1;
	*cur_print_buf_ptr = NULL_CHAR;
	int_part = (int32_t)result_float_num;		
	if(num_digits_after_point != 0)
	{
		frac_part = result_float_num - (float)int_part; 
	    if(frac_part < 0)
        {
          frac_part = -frac_part;
        }
	    frac_part = frac_part * (float)Power_Of(base, num_digits_after_point); 
	    cur_unsign_print_num = (int32_t)frac_part;
    	while(num_digits_after_point != 0)
	    {
		   cur_digit = cur_unsign_print_num % base;
		   if( cur_digit >= 10 )
		   {
		    	cur_digit += alpha_start_char - '0' - 10;
		   }
		    *--cur_print_buf_ptr = cur_digit + '0';
	       cur_unsign_print_num /= base;
		   --num_digits_after_point;		   
	    }
	     *--cur_print_buf_ptr = '.';
	}
	if(sign_flag == STATE_YES && base == BASE_10 && result_float.raw_ieee_754_format.raw.sign == 1)
	{
		// negative 
		proc_bit_field |= (1 << 0);
		cur_unsign_print_num = -int_part;
	}	
    else
    {
    	cur_unsign_print_num = int_part;
	}
    if(cur_unsign_print_num == 0)
	{
		*--cur_print_buf_ptr = '0';
	}		
	while(cur_unsign_print_num)
	{
		cur_digit = cur_unsign_print_num % base;
		if( cur_digit >= 10 )
		{
			cur_digit += alpha_start_char - '0' - 10;
		}
		*--cur_print_buf_ptr = cur_digit + '0';
		cur_unsign_print_num /= base;
	}	
	if((proc_bit_field & (1 << 0)))
	{
		// negative 
		if( width_spec && (pad_format & PAD_ZERO) ) 
		{
			if((Print_Char(out_str_ptr, '-')) == NULL_CHAR)
			{
				return num_chars_printed;
			}
			++num_chars_printed;
			--width_spec;
		}
		else
		{
			*--cur_print_buf_ptr = '-';
		}
	}
	ret_status = Print_Str(out_str_ptr, cur_print_buf_ptr, width_spec, pad_format);
	if(ret_status == 0)
	{
		system_status_flag = ERR_DEV_OPER_WRITE_FUNC;
		Error_or_Warning_Proc("15.26.03", ERROR_OCCURED, system_status_flag);
		return num_chars_printed;
	}
   	return num_chars_printed + ret_status; 
}

/*------------------------------------------------------------*
FUNCTION NAME  : Str_Cpy

DESCRIPTION    : 
								
INPUT          : 

OUTPUT         : 

NOTE           : 

Func ID        : 02.21 

Bugs           :  
-*------------------------------------------------------------*/
int16_t Str_Cpy(char * const dest_str_ptr, const char* const src_str_ptr)
{
	int16_t str_len = 0; 
	uint8_t str_pos = 0;
	
	if (dest_str_ptr == NULL_DATA_PTR || src_str_ptr == NULL_DATA_PTR ||((str_len = Str_Len(src_str_ptr) ) <= 0) )
	{
		system_status_flag = ERR_NULL_PTR;
		Error_or_Warning_Proc("02.21.01", ERROR_OCCURED, system_status_flag);
		str_len = 0;
		return EOF;
	}
	for(str_pos = 0; str_pos < str_len; ++str_pos)
	{
		dest_str_ptr[str_pos] = src_str_ptr[str_pos];
	}
	dest_str_ptr[str_pos] = NULL_CHAR;
	return ( str_len);
}
	
/*------------------------------------------------------------------*-
  ------------------------ END OF FILE ------------------------------
-*------------------------------------------------------------------*/
