/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
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
  | Author: Santiago Lizardo <santiagolizardo@php.net>            |
  +----------------------------------------------------------------------+
*/

#include <GL/gl.h>
#include "php.h"

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

#define FIVE_PARAM(p,p1,p2,p4,p5) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzzz", &p, &p1, &p2, &p4, &p5) == FAILURE) \
		WRONG_PARAM_COUNT; 

#define SIX_PARAM(p,p1,p2,p4,p5,p6) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzzzz", &p, &p1, &p2, &p4, &p5, &p6) == FAILURE) \
		WRONG_PARAM_COUNT; 

#define SEVEN_PARAM(p,p1,p2,p4,p5,p6,p7) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzzzzz", &p, &p1, &p2, &p4, &p5, &p6, &p7) == FAILURE) \
		WRONG_PARAM_COUNT; 

#define EIGHT_PARAM(p,p1,p2,p4,p5,p6,p7,p8) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzzzzzz", &p, &p1, &p2, &p4, &p5, &p6, &p7, &p8) == FAILURE) \
		WRONG_PARAM_COUNT; 

#define NINE_PARAM(p,p1,p2,p4,p5,p6,p7,p8,p9) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzzzzzzz", &p, &p1, &p2, &p4, &p5, &p6, &p7, &p8, &p9) == FAILURE) \
		WRONG_PARAM_COUNT; 

#define TEN_PARAM(p,p1,p2,p4,p5,p6,p7,p8,p9,p10) \
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzzzzzzzz", &p, &p1, &p2, &p4, &p5, &p6, &p7, &p8, &p9, &p10) == FAILURE) \
		WRONG_PARAM_COUNT; 

#define IS_CALLBACK(callback, param_num) \
	{ \
		char *callback_name; \
		if (!zend_is_callable(callback, 0, &callback_name)) { \
			php_error(E_WARNING, "%s() requires argument %d, '%s', to be a valid callback", \
				get_active_function_name(TSRMLS_C), param_num, callback_name); \
			efree(callback_name); \
			RETURN_FALSE; \
		} \
		efree(callback_name); \
	}

#define HASH_CALLBACK(callback,param_num,hash_key) \
	{ \
		IS_CALLBACK(callback,param_num); \
		zval_add_ref(&callback); \
		zend_hash_index_update(call_backs, hash_key, callback, sizeof(zval), NULL); \
	}

#define HASH_MENU_CALLBACK(callback,param_num,menu_id) \
	{ \
		IS_CALLBACK(callback,param_num); \
		zval_add_ref(&callback); \
		zend_hash_index_update(menu_callbacks, menu_id, callback, sizeof(zval), NULL); \
	}
