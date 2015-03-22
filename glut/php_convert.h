/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Brad Lafountain                                              |
  | Author: Santiago Lizardo <santiagolizardo@php.net>                   |
  +----------------------------------------------------------------------+
*/


#define C_INT_TO_PHP_LONG 1
#define C_LONG_TO_PHP_LONG 2
#define C_FLOAT_TO_PHP_DOUBLE 3
#define C_DOUBLE_TO_PHP_DOUBLE 4
#define C_BOOLEAN_TO_PHP_BOOLEAN 5
#define C_CHAR_TO_PHP_LONG 6
#define C_UINT_TO_PHP_LONG 7
#define C_USHORT_TO_PHP_LONG 8

#define TO_C_LONG 1
#define TO_C_DOUBLE 2
#define TO_C_INT 3
#define TO_C_FLOAT 4
#define TO_C_UCHAR 5
#define TO_C_SCHAR 6
#define TO_C_USHORT 7
#define TO_C_SHORT 8
#define TO_C_UINT 9
#define TO_C_STRING 10
#define TO_C_BYTE 11
#define TO_C_UBYTE 12

void call_user_callback(HashTable *callbacks,int call_type,int num_params,zval *params[]);

void c_array_to_php_array(void *c_array,int num,zval *php_array,int type);
#define int_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_INT_TO_PHP_LONG)
#define uint_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_UINT_TO_PHP_LONG)
#define long_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_LONG_TO_PHP_LONG)
#define float_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_FLOAT_TO_PHP_DOUBLE)
#define boolean_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_BOOLEAN_TO_PHP_BOOLEAN)
#define double_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_DOUBLE_TO_PHP_DOUBLE)
#define ushort_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_USHORT_TO_PHP_LONG)
#define char_array_to_php_array(a,i,z) c_array_to_php_array(a,i,z,C_CHAR_TO_PHP_LONG)

void *php_array_to_c_array(zval *param,int type,int size,int *array_size);
#define php_array_to_string_array(z) (char **)php_array_to_c_array(z,TO_C_STRING,sizeof(char *),NULL)
#define php_array_to_byte_array(z) (char *)php_array_to_c_array(z,TO_C_BYTE,sizeof(char),NULL)
#define php_array_to_ubyte_array(z) (unsigned char*)php_array_to_c_array(z,TO_C_UBYTE,sizeof(unsigned char),NULL)
#define php_array_to_long_array(z) (long *)php_array_to_c_array(z,TO_C_LONG,sizeof(long),NULL)
#define php_array_to_double_array(z) (double *)php_array_to_c_array(z,TO_C_DOUBLE,sizeof(double),NULL)
#define php_array_to_int_array(z) (int *)php_array_to_c_array(z,TO_C_INT,sizeof(int),NULL)
#define php_array_to_float_array(z) (float *)php_array_to_c_array(z,TO_C_FLOAT,sizeof(float),NULL)
#define php_array_to_uchar_array(z) (unsigned char *)php_array_to_c_array(z,TO_C_UCHAR,sizeof(unsigned char),NULL)
#define php_array_to_schar_array(z) (signed char *)php_array_to_c_array(z,TO_C_SCHAR,sizeof(signed char),NULL)
#define php_array_to_ushort_array(z) (unsigned short *)php_array_to_c_array(z,TO_C_USHORT,sizeof(unsigned short),NULL)
#define php_array_to_short_array(z) (short *)php_array_to_c_array(z,TO_C_SHORT,sizeof(short),NULL)
#define php_array_to_uint_array(z) (unsigned int *)php_array_to_c_array(z,TO_C_UINT,sizeof(unsigned int),NULL)

int gl_pixel_size(GLenum format);
int gl_type_size(GLenum type);

#define THREE_PARAM(p,p1,p2) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &p, &p1, &p2) == FAILURE) \
		WRONG_PARAM_COUNT; 

#define FOUR_PARAM(p,p1,p2,p4) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzz", &p, &p1, &p2, &p4) == FAILURE) \
		WRONG_PARAM_COUNT; 


