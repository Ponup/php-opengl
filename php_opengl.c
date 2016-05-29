/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
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
  | Author: Santiago Lizardo <santiagolizardo@php.net>                   |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PHP_WIN32
#include <windows.h>
#endif

#include "php_opengl.h"

#define PHP_OSX defined(__APPLE__) && defined(__MACH__)

#if PHP_OSX 
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/freeglut.h>
#endif

#include "php_convert.h"


/*
#include "php_glut.h"
*/

void convert_gluint_array_to_zval(GLuint* array, GLsizei n, zval* val) {
    GLsizei i;

    zval_dtor(val);
    array_init(val);
    for(i = 0; i < n; i++) {
        add_index_long(val, i, array[i]);
    }
}

int* convert_php_array_to_void_pointer(zend_array* array, int size) {
    int* data;
    
    data = (int*)emalloc(sizeof(int) * size);
    
    return data;
}

extern int le_nurb;
extern int le_tess;
extern int le_quad;

ZEND_BEGIN_ARG_INFO_EX(arginfo_glgenvertexarrays, 0, 0, 2)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(1, arrays)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glgenvertexarrays)
{
    zval *z_arrays;
    zend_long n;
    GLuint* arrays = NULL;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz/", &n, &z_arrays) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    arrays = (GLuint*)emalloc(sizeof(GLuint) * n);
    if(arrays == NULL) {
        RETURN_FALSE;
    }

    glGenVertexArrays((GLsizei)n, arrays);
    
    convert_gluint_array_to_zval(arrays, n, z_arrays);
    
    efree(arrays);
    
    RETURN_TRUE;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glbindvertexarray, 0, 0, 1)
    ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glbindvertexarray)
{
    zend_long array;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &array) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glBindVertexArray((GLuint)array);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glgenbuffers, 0, 0, 2)
    ZEND_ARG_INFO(0, n)
    ZEND_ARG_INFO(1, buffers)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glgenbuffers)
{
    zval *z_buffers;
    zend_long n;
    GLuint *buffers = NULL;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz/", &n, &z_buffers) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    buffers = (GLuint*)emalloc(sizeof(GLuint) * n);
    if(buffers == NULL) {
        RETURN_FALSE;
    }
    
    glGenBuffers((GLsizei)n, buffers);
    
    convert_gluint_array_to_zval(buffers, n, z_buffers);
    
    efree(buffers);
    
    RETURN_TRUE;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glbindbuffer, 0, 0, 2)
    ZEND_ARG_INFO(0, target)
    ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glbindbuffer)
{
    zend_long target, buffer;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &target, &buffer) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glBindBuffer(target, buffer);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glbufferdata, 0, 0, 4)
    ZEND_ARG_INFO(0, target)
    ZEND_ARG_INFO(0, size)
    ZEND_ARG_INFO(0, data)
    ZEND_ARG_INFO(0, usage)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glbufferdata)
{
    int array_type;
    zend_long target, size, usage;
    zval *z_data, *z_first;
    void* data;
    zend_array *param_ht;
    int tmp_size;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llal", &target, &size, &z_data, &usage) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    param_ht = HASH_OF(z_data);
    tmp_size = zend_hash_num_elements(param_ht);

    if(tmp_size > 0) {
        z_first = zend_hash_index_find(param_ht, 0);
        array_type = Z_TYPE_P(z_first) == IS_DOUBLE ? TO_C_FLOAT : TO_C_INT;
        
        data = php_array_to_c_array(z_data, array_type, tmp_size, NULL);
        
        glBufferData((GLenum)target, (GLsizeiptr)size, data, (GLenum)usage);
    } else {
        zend_error(E_WARNING, "glBufferData received an empty array");
    }
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glDeleteVertexArrays, 0, 0, 0)
    ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glDeleteVertexArrays)
{
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glDeleteBuffers, 0, 0, 0)
    ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glDeleteBuffers)
{
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glDeleteShader, 0, 0, 0)
    ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glDeleteShader)
{
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glDeleteProgram, 0, 0, 0)
    ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glDeleteProgram)
{
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glshadersource, 0, 0, 4)
    ZEND_ARG_INFO(0, shader)
    ZEND_ARG_INFO(0, count)
    ZEND_ARG_INFO(0, string)
    ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glshadersource)
{
    zend_long shader, count, length;
    char* string;
    size_t string_len;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llsl", &shader, &count, &string, &string_len, &length) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glShaderSource(shader, count, &string, length == 0 ? NULL : &length);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glcompileshader, 0, 0, 1)
    ZEND_ARG_INFO(0, shader)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glcompileshader)
{
    zend_long shader;
    GLint isCompiled = 0;
    GLint maxLength = 0;
    GLchar* error_msg = NULL;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &shader) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glCompileShader((GLuint)shader);
    
    glGetShaderiv((GLuint)shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            error_msg = (char*)emalloc(sizeof(GLchar)*maxLength);
            memset(error_msg, 0, sizeof(GLchar)*maxLength);
            glGetShaderInfoLog((GLuint)shader, maxLength, &maxLength, error_msg);

            zend_error(E_ERROR, error_msg);
            
            efree(error_msg);

            glDeleteShader(shader); // Don't leak the shader.
    }

}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glattachshader, 0, 0, 2)
    ZEND_ARG_INFO(0, program)
    ZEND_ARG_INFO(0, shader)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glattachshader)
{
    zend_long program, shader;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &program, &shader) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glAttachShader((GLuint)program, (GLuint)shader);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glbindfragdatalocation, 0, 0, 3)
    ZEND_ARG_INFO(0, program)
    ZEND_ARG_INFO(0, colorNumber)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glbindfragdatalocation)
{
    zend_long program, program_number;
    char *name;
    size_t name_len;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls", &program, &program_number, &name, &name_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glBindFragDataLocation(program, program_number, name);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_gllinkprogram, 0, 0, 1)
    ZEND_ARG_INFO(0, program)
ZEND_END_ARG_INFO()

PHP_FUNCTION(gllinkprogram)
{
    zend_long program;
        GLint IsLinked;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &program) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glLinkProgram((GLuint)program);
    
    glGetProgramiv(program, GL_LINK_STATUS, (GLint *)&IsLinked);
    if(IsLinked==GL_FALSE)
    {
            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            if(maxLength>0)
            {
                    char *pLinkInfoLog = (char*)emalloc(sizeof(char)*maxLength);
                    glGetProgramInfoLog(program, maxLength, &maxLength, pLinkInfoLog);
                    zend_error(E_ERROR, pLinkInfoLog);
            }
    }
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_gluseprogram, 0, 0, 1)
    ZEND_ARG_INFO(0, program)
ZEND_END_ARG_INFO()

PHP_FUNCTION(gluseprogram)
{
    zend_long program;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &program) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glUseProgram((GLuint)program);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glcreateprogram, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glcreateprogram)
{
    GLuint program = glCreateProgram();
    RETURN_LONG((long)program);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glgetattriblocation, 0, 0, 2)
    ZEND_ARG_INFO(0, program)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glgetattriblocation)
{
    zend_long program;
    char *name;
    size_t name_len;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &program, &name, &name_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    RETURN_LONG(glGetAttribLocation(program, name));
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glenablevertexattribarray, 0, 0, 1)
    ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glenablevertexattribarray)
{
    zend_long index;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &index) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glEnableVertexAttribArray((GLuint)index);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glvertexattribpointer, 0, 0, 6)
    ZEND_ARG_INFO(0, index)
    ZEND_ARG_INFO(0, size)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, normalized)
    ZEND_ARG_INFO(0, stride)
    ZEND_ARG_INFO(0, pointer)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glvertexattribpointer)
{
    zend_long index, size, type, normalized, stride, pointer;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllll", &index, &size, &type, &normalized, &stride, &pointer) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glVertexAttribPointer((GLuint)index, (GLint)size, (GLenum)type, (GLboolean)normalized, (GLsizei)stride, pointer > 0 ? (void*)pointer : 0);
}

/*
ZEND_BEGIN_ARG_INFO_EX(arginfo_, 0, 0, 2)
    ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()
*/        

PHP_MINFO_FUNCTION(opengl)
{
        php_info_print_table_start();
        php_info_print_table_header(2, "OpenGL support", "enabled");
        php_info_print_table_end();
}

/* {{{ void glbindtexture(long target, long texture) */
PHP_FUNCTION(glbindtexture)
{
	zend_long target, texture;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &target, &texture) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glBindTexture((GLenum)target, (GLuint)texture);
}
/* }}} */

/* {{{ void glblendfunc(long sfactor, long dfactor) */
PHP_FUNCTION(glblendfunc)
{
	long sfactor, dfactor;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &sfactor, &dfactor) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glBlendFunc((int)sfactor,(int)dfactor);
}
/* }}} */

/* {{{ void glclear(long mask) */
PHP_FUNCTION(glclear)
{
	zend_long mask;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glClear((GLbitfield)mask);
}
/* }}} */

/* {{{ void glclearcolor(double red, double green, double blue, double alpha) */
PHP_FUNCTION(glclearcolor)
{
    double red, green, blue, alpha;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &red, &green, &blue, &alpha) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

	glClearColor((GLclampf)red, (GLclampf)green, (GLclampf)blue, (GLclampf)alpha);
}
/* }}} */

/* {{{ void glcleardepth(double depth) */
PHP_FUNCTION(glcleardepth)
{
	double depth;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &depth) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glClearDepth(depth);
}
/* }}} */

/* {{{ void glclearstencil(long s) */
PHP_FUNCTION(glclearstencil)
{
	long s;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &s) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glClearStencil((int)s);
}
/* }}} */

/* {{{ void glcolormask(boolean red, boolean green, boolean blue, boolean alpha) */
PHP_FUNCTION(glcolormask)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_boolean(red);
	convert_to_boolean(green);
	convert_to_boolean(blue);
	convert_to_boolean(alpha);
	glColorMask(Z_BVAL_P(red),Z_BVAL_P(green),Z_BVAL_P(blue),Z_BVAL_P(alpha));
}
/* }}} */

/* {{{ void glcopyteximage1d(long target, long level, long internalFormat, long x, long y, long width, long border) */
PHP_FUNCTION(glcopyteximage1d)
{
	zval *target, *level, *internalFormat, *x, *y, *width, *border;
	SEVEN_PARAM(target, level, internalFormat, x, y, width, border);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalFormat);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(border);
	glCopyTexImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalFormat),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(border));
}
/* }}} */

/* {{{ void glcopyteximage2d(long target, long level, long internalFormat, long x, long y, long width, long height, long border) */
PHP_FUNCTION(glcopyteximage2d)
{
	zval *target, *level, *internalFormat, *x, *y, *width, *height, *border;
	EIGHT_PARAM(target, level, internalFormat, x, y, width, height, border);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalFormat);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(border);
	glCopyTexImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalFormat),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(border));
}
/* }}} */

/* {{{ void glcopytexsubimage1d(long target, long level, long xoffset, long x, long y, long width) */
PHP_FUNCTION(glcopytexsubimage1d)
{
    zend_long target, level, xoffset, x, y, width;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllll", &target, &level, &xoffset, &x, &y, &width) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	glCopyTexSubImage1D((int)target, (int)level, (int)xoffset, (int)x, (int)y, (int)width);
}
/* }}} */

/* {{{ void glcopytexsubimage2d(long target, long level, long xoffset, long yoffset, long x, long y, long width, long height) */
PHP_FUNCTION(glcopytexsubimage2d)
{
	zval *target, *level, *xoffset, *yoffset, *x, *y, *width, *height;
	EIGHT_PARAM(target, level, xoffset, yoffset, x, y, width, height);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(yoffset);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	glCopyTexSubImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(yoffset),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height));
}
/* }}} */

/* {{{ void glcullface(long mode) */
PHP_FUNCTION(glcullface)
{
	long mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glCullFace((int)mode);
}
/* }}} */

/* {{{ void gldeletetextures(long n, array textures) */
PHP_FUNCTION(gldeletetextures)
{
	long n;
	zval *textures;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &n, &textures) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	GLuint *v_textures;
	v_textures = php_array_to_uint_array(textures);
	glDeleteTextures((int)(n),v_textures);
}
/* }}} */

/* {{{ void gldepthfunc(long func) */
PHP_FUNCTION(gldepthfunc)
{
	long func;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &func) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glDepthFunc((int)func);
}
/* }}} */

/* {{{ void gldepthmask(boolean flag) */
PHP_FUNCTION(gldepthmask)
{
	zend_bool flag;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &flag) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glDepthMask(flag);
}
/* }}} */

/* {{{ void gldepthrange(double zNear, double zFar) */
PHP_FUNCTION(gldepthrange)
{
	double zNear, zFar;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &zNear, &zFar) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glDepthRange(zNear,zFar);
}
/* }}} */

/* {{{ void gldisable(long cap) */
PHP_FUNCTION(gldisable)
{
	long cap;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &cap) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glDisable((int)cap);
}
/* }}} */

/* {{{ void gldrawarrays(long mode, long first, long count) */
PHP_FUNCTION(gldrawarrays)
{
    zend_long mode, first, count;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &mode, &first, &count) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	glDrawArrays((GLenum)mode, (GLint)first, (GLsizei)count);
}
/* }}} */

/* {{{ void gldrawbuffer(long mode) */
PHP_FUNCTION(gldrawbuffer)
{
	long mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glDrawBuffer((int)mode);
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_glDrawElements, 0, 0, 4)
    ZEND_ARG_INFO(0, mode)
    ZEND_ARG_INFO(0, count)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, indices)
ZEND_END_ARG_INFO()

/* {{{ void gldrawelements(long mode, long count, long type, array indices) */
PHP_FUNCTION(gldrawelements)
{
    zend_long mode, count, type;
    zval indices;
    GLvoid *v_indices = NULL;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllz!", &mode, &count, &type, &indices) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    if(!ZVAL_IS_NULL(&indices)) {
        convert_to_array(&indices);
        switch(type)
        {
        case GL_UNSIGNED_BYTE:
                v_indices = php_array_to_ubyte_array(&indices);
                break;
        case GL_UNSIGNED_SHORT:
                v_indices = php_array_to_ushort_array(&indices);
                break;
        case GL_UNSIGNED_INT:
                v_indices = php_array_to_uint_array(&indices);
                break;
        }
    }
    glDrawElements((GLenum)mode, (GLsizei)count, (GLenum)type, NULL);
}
/* }}} */

/* {{{ void glenable(long cap) */
PHP_FUNCTION(glenable)
{
	zend_long cap;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &cap) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEnable((GLenum)cap);
}
/* }}} */

/* {{{ void glfinish() */
PHP_FUNCTION(glfinish)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glFinish();
}
/* }}} */

/* {{{ void glflush() */
PHP_FUNCTION(glflush)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glFlush();
}
/* }}} */

/* {{{ void glfrontface(long mode) */
PHP_FUNCTION(glfrontface)
{
	long mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glFrontFace((int)mode);
}
/* }}} */

/* {{{ void glgentextures(long n, array textures) */
PHP_FUNCTION(glgentextures)
{
	long n;
	zval *textures;
	GLuint *v_textures;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &n, &textures) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	v_textures = (GLuint *)emalloc(sizeof(GLuint) * (n));
	glGenTextures((int)(n),v_textures);
	uint_array_to_php_array(v_textures,(n),textures);
	efree(v_textures);
}
/* }}} */

/* {{{ void glgetbooleanv(long pname, array params, long num) */
PHP_FUNCTION(glgetbooleanv)
{
	zval *pname, *params, *num;
	GLboolean *v_params;
	THREE_PARAM(pname, params, num);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLboolean *)emalloc(Z_LVAL_P(num) * sizeof(GLboolean));
	glGetBooleanv((unsigned int)Z_LVAL_P(pname),v_params);
	boolean_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetdoublev(long pname, array params, long num) */
PHP_FUNCTION(glgetdoublev)
{
	zval *pname, *params, *num;
	GLdouble *v_params;
	THREE_PARAM(pname, params, num);
	convert_to_long(pname);
	convert_to_array(params);
	convert_to_long(num);
	v_params = (GLdouble *)emalloc(Z_LVAL_P(num) * sizeof(GLdouble));
	glGetDoublev((int)Z_LVAL_P(pname),v_params);
	double_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ long glgeterror() */
PHP_FUNCTION(glgeterror)
{
	GLenum error_code;
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	error_code = glGetError();
	RETURN_LONG(error_code);
}
/* }}} */

/* {{{ void glgetfloatv(long pname, array params, long num) */
PHP_FUNCTION(glgetfloatv)
{
	zval *pname, *params, *num;
	GLfloat *v_params;
	THREE_PARAM(pname, params, num);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLfloat *)emalloc(Z_LVAL_P(num) * sizeof(GLint));
	glGetFloatv((unsigned int)Z_LVAL_P(pname),v_params);
	float_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetintegerv(long pname, array params, long num) */
PHP_FUNCTION(glgetintegerv)
{
	zval *pname, *params, *num;
	GLint *v_params;
	THREE_PARAM(pname, params, num);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLint *)emalloc(Z_LVAL_P(num) * sizeof(GLint));
	glGetIntegerv((unsigned int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetpointerv(long pname, array params) */
PHP_FUNCTION(glgetpointerv)
{
	long pname;
	zval *params;
	/*TODO: do this function */
	/*
	GLvoid* *v_params;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &pname, &params) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_array(params);
	v_params = php_array_to_long_array(params);
	glGetPointerv((int)(pname),v_params);*/
}
/* }}} */

/* {{{ string glgetstring(long name) */
PHP_FUNCTION(glgetstring)
{
	long name;
	GLubyte *ret;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &name) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	ret = (GLubyte*)glGetString(name);
	RETURN_STRING((char*)ret);
}
/* }}} */

/* {{{ void glgetteximage(long target, long level, long format, long type, array pixels) */
PHP_FUNCTION(glgetteximage)
{
    zend_long target, level, format, type;
	zval *pixels;
	GLvoid *v_pixels;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllz", &target, &level, &format, &type) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glGetTexImage((int)target, (int)level, (int)format, (int)type, v_pixels);
}
/* }}} */

/* {{{ void glgettexlevelparameterfv(long target, long level, long pname, array params) */
PHP_FUNCTION(glgettexlevelparameterfv)
{
	zval *target, *level, *pname, *params;
	GLfloat *v_params;
	FOUR_PARAM(target, level, pname, params);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glGetTexLevelParameterfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void glgettexlevelparameteriv(long target, long level, long pname, array params) */
PHP_FUNCTION(glgettexlevelparameteriv)
{
	zval *target, *level, *pname, *params;
	GLint *v_params;
	FOUR_PARAM(target, level, pname, params);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glGetTexLevelParameteriv((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void glgettexparameterfv(long target, long pname, array params) */
PHP_FUNCTION(glgettexparameterfv)
{
	zval *target, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glGetTexParameterfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void glgettexparameteriv(long target, long pname, array params) */
PHP_FUNCTION(glgettexparameteriv)
{
	zval *target, *pname, *params;
	GLint *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glGetTexParameteriv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void glhint(long target, long mode) */
PHP_FUNCTION(glhint)
{
	long target, mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &target, &mode) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glHint((int)(target),(int)(mode));
}
/* }}} */

/* {{{ bool glisenabled(long cap) */
PHP_FUNCTION(glisenabled)
{
	long cap;
	int return_int;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &cap) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	return_int = glIsEnabled((int)cap);
	RETURN_BOOL(return_int);
}
/* }}} */

/* {{{ bool glistexture(long texture) */
PHP_FUNCTION(glistexture)
{
	long texture;
	int return_int;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &texture) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	return_int = glIsTexture((unsigned int)texture);
	RETURN_BOOL(return_int);
}
/* }}} */

/* {{{ void gllinewidth(double width) */
PHP_FUNCTION(gllinewidth)
{
	double width;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &width) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glLineWidth((float)width);
}
/* }}} */

/* {{{ void gllogicop(long opcode) */
PHP_FUNCTION(gllogicop)
{
	long opcode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &opcode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glLogicOp((int)opcode);
}
/* }}} */

/* {{{ void glpixelstoref(long pname, double param) */
PHP_FUNCTION(glpixelstoref)
{
	long pname;
	double param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glPixelStoref((int)(pname),(float)(param));
}
/* }}} */

/* {{{ void glpixelstorei(long pname, long param) */
PHP_FUNCTION(glpixelstorei)
{
	long pname, param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glPixelStorei((int)(pname),(int)(param));
}
/* }}} */

/* {{{ void glpointsize(double size) */
PHP_FUNCTION(glpointsize)
{
	double size;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &size) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPointSize((float)size);
}
/* }}} */

/* {{{ void glpolygonmode(long face, long mode) */
PHP_FUNCTION(glpolygonmode)
{
	long face, mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &face, &mode) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glPolygonMode((int)(face),(int)(mode));
}
/* }}} */

/* {{{ void glpolygonoffset(double factor, double units) */
PHP_FUNCTION(glpolygonoffset)
{
	double factor, units;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &factor, &units) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glPolygonOffset((float)(factor),(float)(units));
}
/* }}} */

/* {{{ void glpolygonstipple(array mask) */
PHP_FUNCTION(glpolygonstipple)
{
	zval *mask;
	GLubyte *v_mask;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_mask = php_array_to_uchar_array(mask);
	glPolygonStipple(v_mask);
}
/* }}} */

/* {{{ void glreadbuffer(long mode) */
PHP_FUNCTION(glreadbuffer)
{
	long mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glReadBuffer((int)mode);
}
/* }}} */

/* {{{ void glreadpixels(long x, long y, long width, long height, long format, long type, array pixels) */
PHP_FUNCTION(glreadpixels)
{
	zval *x, *y, *width, *height, *format, *type, *pixels;
	GLvoid *v_pixels;
	SEVEN_PARAM(x, y, width, height, format, type, pixels);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glReadPixels((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
}
/* }}} */

/* {{{ void glscissor(long x, long y, long width, long height) */
PHP_FUNCTION(glscissor)
{
	zval *x, *y, *width, *height;
	FOUR_PARAM(x, y, width, height);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	glScissor((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height));
}
/* }}} */

/* {{{ void glstencilfunc(long func, long ref, long mask) */
PHP_FUNCTION(glstencilfunc)
{
	zval *func, *ref, *mask;
	THREE_PARAM(func, ref, mask);
	convert_to_long(func);
	convert_to_long(ref);
	convert_to_long(mask);
	glStencilFunc((int)Z_LVAL_P(func),(int)Z_LVAL_P(ref),(unsigned int)Z_LVAL_P(mask));
}
/* }}} */

/* {{{ void glstencilmask(long mask) */
PHP_FUNCTION(glstencilmask)
{
	long mask;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glStencilMask((unsigned int)mask);
}
/* }}} */

/* {{{ void glstencilop(long fail, long zfail, long zpass) */
PHP_FUNCTION(glstencilop)
{
	zval *fail, *zfail, *zpass;
	THREE_PARAM(fail, zfail, zpass);
	convert_to_long(fail);
	convert_to_long(zfail);
	convert_to_long(zpass);
	glStencilOp((int)Z_LVAL_P(fail),(int)Z_LVAL_P(zfail),(int)Z_LVAL_P(zpass));
}
/* }}} */

/* {{{ void glteximage1d(long target, long level, long internalformat, long width, long border, long format, long type, array pixels) */
PHP_FUNCTION(glteximage1d)
{
	zval *target, *level, *internalformat, *width, *border, *format, *type, *pixels;
	GLvoid *v_pixels;
	EIGHT_PARAM(target, level, internalformat, width, border, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalformat);
	convert_to_long(width);
	convert_to_long(border);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalformat),(int)Z_LVAL_P(width),(int)Z_LVAL_P(border),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
}
/* }}} */

/* {{{ void glteximage2d(long target, long level, long internalformat, long width, long height, long border, long format, long type, array pixels) */
PHP_FUNCTION(glteximage2d)
{
    zend_long target, level, internal_format, width, height, border, format, type;
    zval *pixels;
	GLvoid *v_pixels;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllllllz", &target, &level, &internal_format, &width, &height, &border, &format, &type, &pixels) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	convert_to_array(pixels);
	v_pixels = php_array_to_int_array(pixels);
	glTexImage2D((GLenum)target, (GLint)level, (GLint)internal_format, (GLsizei)width, (GLsizei)height, (GLint)border, (GLenum)format, (GLenum)type, v_pixels);
}
/* }}} */

/* {{{ void gltexparameterf(long target, long pname, double param) */
PHP_FUNCTION(gltexparameterf)
{
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_double(param);
	glTexParameterf((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
/* }}} */

/* {{{ void gltexparameterfv(long target, long pname, array params) */
PHP_FUNCTION(gltexparameterfv)
{
	zval *target, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glTexParameterfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gltexparameteri(long target, long pname, long param) */
PHP_FUNCTION(gltexparameteri)
{
	zend_long target, pname, param;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &target, &pname, &param) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	glTexParameteri((GLenum)target, (GLenum)pname, (GLint)param);
}
/* }}} */

/* {{{ void gltexparameteriv(long target, long pname, array params) */
PHP_FUNCTION(gltexparameteriv)
{
	zval *target, *pname, *params;
	GLint *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glTexParameteriv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gltexsubimage1d(long target, long level, long xoffset, long width, long format, long type, array pixels) */
PHP_FUNCTION(gltexsubimage1d)
{
	zval *target, *level, *xoffset, *width, *format, *type, *pixels;
	GLvoid *v_pixels;
	SEVEN_PARAM(target, level, xoffset, width, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(width);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexSubImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(width),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
}
/* }}} */

/* {{{ void gltexsubimage2d(long target, long level, long xoffset, long yoffset, long width, long height, long format, long type, array pixels) */
PHP_FUNCTION(gltexsubimage2d)
{
	zend_long target, level, xoffset, yoffset, width, height, format, type;
    zval* pixels;
	GLvoid *v_pixels;
	if(zend_parse_parameters(ZEND_NUM_ARGS(), "llllllllz", &target, &level, &xoffset, &yoffset, &width, &height, &format, &type, &pixels) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexSubImage2D((int)(target),(int)(level),(int)(xoffset),(int)(yoffset),(int)(width),(int)(height),(int)(format),(int)(type),v_pixels);
}
/* }}} */

/* {{{ void glviewport(long x, long y, long width, long height) */
PHP_FUNCTION(glviewport)
{
	zend_long x, y, width, height;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &x, &y, &width, &height) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glViewport((int)x, (int)y, (int)width, (int)height);
}
/* }}} */

/* {{{ void glviewport(long x, long y, long width, long height) */
PHP_FUNCTION(glcreateshader)
{
    GLuint return_code;
    zend_long shader_type;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &shader_type) == FAILURE)
    {
            WRONG_PARAM_COUNT;
    }

    return_code = glCreateShader((GLenum)shader_type);
    RETURN_LONG(return_code);
}
/* }}} */

PHP_FUNCTION(glGetUniformLocation)
{
    zend_long program;
    char* name;
    size_t name_len;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &program, &name, &name_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    RETURN_LONG(glGetUniformLocation(program, name));
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glUniform1f, 0, 0, 2)
    ZEND_ARG_INFO(0, location)
    ZEND_ARG_INFO(0, v0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glUniform1f)
{
    zend_long location;
    double v0;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &location, &v0) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    glUniform1f((GLint)location, (GLfloat)v0);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glUniform3f, 0, 0, 4)
    ZEND_ARG_INFO(0, location)
    ZEND_ARG_INFO(0, v0)
    ZEND_ARG_INFO(0, v1)
    ZEND_ARG_INFO(0, v2)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glUniform3f)
{
    zend_long location;
    double v0, v1, v2;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lddd", &location, &v0, &v1, &v2) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    glUniform3f((GLint)location, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2);
}


ZEND_BEGIN_ARG_INFO_EX(arginfo_glUniformMatrix4fv, 0, 0, 4)
    ZEND_ARG_INFO(0, location)
    ZEND_ARG_INFO(0, count)
    ZEND_ARG_INFO(0, transpose)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glUniformMatrix4fv)
{
    zend_long location, count;
    zend_bool transpose;
    zend_array* z_value;
    GLfloat* value;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llba", &location, &count, &transpose, &z_value) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

	convert_to_array(z_value);
    value = php_array_to_float_array(z_value);

    glUniformMatrix4fv((GLint)location, (GLsizei)count, (GLboolean)transpose, value);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glGenerateMipmap, 0, 0, 1)
    ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glGenerateMipmap)
{
    zend_long target;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &target) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    glGenerateMipmap((GLenum)target);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glUniform1i, 0, 0, 2)
    ZEND_ARG_INFO(0, location)
    ZEND_ARG_INFO(0, v0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glUniform1i)
{
    zend_long location, v0;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &location, &v0) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    glUniform1i((GLint)location, (GLint)v0);
}


PHP_FUNCTION(glActiveTexture)
{
    zend_long texture;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &texture) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    glActiveTexture((GLenum)texture);
}

const zend_function_entry opengl_functions[] = {
	ZEND_FE(glbindtexture,NULL)
	ZEND_FE(glblendfunc,NULL)
	ZEND_FE(glclear,NULL)
	ZEND_FE(glclearcolor,NULL)
	ZEND_FE(glcleardepth,NULL)
	ZEND_FE(glclearstencil,NULL)
	ZEND_FE(glcolormask,NULL)
	ZEND_FE(glcopyteximage1d,NULL)
	ZEND_FE(glcopyteximage2d,NULL)
	ZEND_FE(glcopytexsubimage1d,NULL)
	ZEND_FE(glcopytexsubimage2d,NULL)
	ZEND_FE(glcullface,NULL)
	ZEND_FE(gldeletetextures,NULL)
	ZEND_FE(gldepthfunc,NULL)
	ZEND_FE(gldepthmask,NULL)
	ZEND_FE(gldepthrange,NULL)
	ZEND_FE(gldisable,NULL)
	ZEND_FE(gldrawarrays,NULL)
	ZEND_FE(gldrawbuffer,NULL)
	ZEND_FE(gldrawelements, arginfo_glDrawElements)
	ZEND_FE(glenable,NULL)
	ZEND_FE(glfinish,NULL)
	ZEND_FE(glflush,NULL)
	ZEND_FE(glfrontface,NULL)
	ZEND_FE(glgentextures,NULL)
	ZEND_FE(glgetbooleanv,NULL)
	ZEND_FE(glgetdoublev,NULL)
	ZEND_FE(glgeterror,NULL)
	ZEND_FE(glgetfloatv,NULL)
	ZEND_FE(glgetintegerv,NULL)
	ZEND_FE(glgetpointerv,NULL)
	ZEND_FE(glgetstring,NULL)
	ZEND_FE(glgetteximage,NULL)
	ZEND_FE(glgettexlevelparameterfv,NULL)
	ZEND_FE(glgettexlevelparameteriv,NULL)
	ZEND_FE(glgettexparameterfv,NULL)
	ZEND_FE(glgettexparameteriv,NULL)
	ZEND_FE(glhint,NULL)
	ZEND_FE(glisenabled,NULL)
	ZEND_FE(glistexture,NULL)
	ZEND_FE(gllinewidth,NULL)
	ZEND_FE(gllogicop,NULL)
	ZEND_FE(glpixelstoref,NULL)
	ZEND_FE(glpixelstorei,NULL)
	ZEND_FE(glpointsize,NULL)
	ZEND_FE(glpolygonmode,NULL)
	ZEND_FE(glpolygonoffset,NULL)
	ZEND_FE(glpolygonstipple,NULL)
	ZEND_FE(glreadbuffer,NULL)
	ZEND_FE(glreadpixels,NULL)
	ZEND_FE(glscissor,NULL)
	ZEND_FE(glstencilfunc,NULL)
	ZEND_FE(glstencilmask,NULL)
	ZEND_FE(glstencilop,NULL)
	ZEND_FE(glteximage1d,NULL)
	ZEND_FE(glteximage2d,NULL)
	ZEND_FE(gltexparameterf,NULL)
	ZEND_FE(gltexparameterfv,NULL)
	ZEND_FE(gltexparameteri,NULL)
	ZEND_FE(gltexparameteriv,NULL)
	ZEND_FE(gltexsubimage1d,NULL)
	ZEND_FE(gltexsubimage2d,NULL)
	ZEND_FE(glviewport,NULL)
	ZEND_FE(glcreateshader, NULL)
    ZEND_FE(glgenvertexarrays, arginfo_glgenvertexarrays)
    ZEND_FE(glbindvertexarray, arginfo_glbindvertexarray)
    ZEND_FE(glgenbuffers, arginfo_glgenbuffers)
    ZEND_FE(glbindbuffer, arginfo_glbindbuffer)
    ZEND_FE(glbufferdata, arginfo_glbufferdata)
    ZEND_FE(glDeleteVertexArrays, arginfo_glDeleteVertexArrays)
    ZEND_FE(glDeleteBuffers, arginfo_glDeleteBuffers)
    ZEND_FE(glDeleteShader, arginfo_glDeleteShader)
    ZEND_FE(glDeleteProgram, arginfo_glDeleteProgram)
    ZEND_FE(glshadersource, arginfo_glshadersource)
    ZEND_FE(glcompileshader, arginfo_glcompileshader)
    ZEND_FE(glattachshader, arginfo_glattachshader)
    ZEND_FE(glbindfragdatalocation, arginfo_glbindfragdatalocation)
    ZEND_FE(gllinkprogram, arginfo_gllinkprogram)
    ZEND_FE(gluseprogram, arginfo_gluseprogram)
    ZEND_FE(glcreateprogram, arginfo_glcreateprogram)
    ZEND_FE(glgetattriblocation, arginfo_glgetattriblocation)
    ZEND_FE(glenablevertexattribarray, arginfo_glenablevertexattribarray)
    ZEND_FE(glvertexattribpointer, arginfo_glvertexattribpointer)
    ZEND_FE(glGetUniformLocation, NULL)
    ZEND_FE(glUniform1f, arginfo_glUniform1f)
    ZEND_FE(glUniform3f, arginfo_glUniform3f)
    ZEND_FE(glUniform1i, arginfo_glUniform1i)
    ZEND_FE(glActiveTexture, NULL)
    ZEND_FE(glUniformMatrix4fv, arginfo_glUniformMatrix4fv)
    ZEND_FE(glGenerateMipmap, arginfo_glGenerateMipmap)

	ZEND_FE_END
};

zend_module_entry opengl_module_entry = {
#if ZEND_MODULE_API_NO > 20010901
  STANDARD_MODULE_HEADER,
#endif
  "OpenGL", 
  opengl_functions, 
  PHP_MINIT(opengl), 
  NULL,
  NULL,
  NULL,
  PHP_MINFO(opengl),
#if ZEND_MODULE_API_NO > 20010901
  PHP_OPENGL_VERSION,
#endif
  STANDARD_MODULE_PROPERTIES,
};

#ifdef COMPILE_DL_OPENGL
ZEND_GET_MODULE(opengl)
#endif

PHP_MINIT_FUNCTION(opengl)
{
	REGISTER_LONG_CONSTANT("GL_ACCUM", GL_ACCUM , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LOAD", GL_LOAD , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RETURN", GL_RETURN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MULT", GL_MULT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ADD", GL_ADD , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEVER", GL_NEVER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LESS", GL_LESS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EQUAL", GL_EQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LEQUAL", GL_LEQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREATER", GL_GREATER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NOTEQUAL", GL_NOTEQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GEQUAL", GL_GEQUAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALWAYS", GL_ALWAYS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_BIT", GL_CURRENT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_BIT", GL_POINT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_BIT", GL_LINE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_BIT", GL_POLYGON_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_STIPPLE_BIT", GL_POLYGON_STIPPLE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MODE_BIT", GL_PIXEL_MODE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHTING_BIT", GL_LIGHTING_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_BIT", GL_FOG_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_BUFFER_BIT", GL_DEPTH_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_BUFFER_BIT", GL_ACCUM_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_BUFFER_BIT", GL_STENCIL_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VIEWPORT_BIT", GL_VIEWPORT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRANSFORM_BIT", GL_TRANSFORM_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ENABLE_BIT", GL_ENABLE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_HINT_BIT", GL_HINT_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EVAL_BIT", GL_EVAL_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_BIT", GL_LIST_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BIT", GL_TEXTURE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SCISSOR_BIT", GL_SCISSOR_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALL_ATTRIB_BITS", GL_ALL_ATTRIB_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINTS", GL_POINTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINES", GL_LINES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_LOOP", GL_LINE_LOOP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STRIP", GL_LINE_STRIP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRIANGLES", GL_TRIANGLES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRIANGLE_STRIP", GL_TRIANGLE_STRIP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRIANGLE_FAN", GL_TRIANGLE_FAN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_QUADS", GL_QUADS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_QUAD_STRIP", GL_QUAD_STRIP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON", GL_POLYGON , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ZERO", GL_ZERO , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE", GL_ONE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SRC_COLOR", GL_SRC_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SRC_ALPHA", GL_SRC_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DST_ALPHA", GL_DST_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_DST_ALPHA", GL_ONE_MINUS_DST_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DST_COLOR", GL_DST_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SRC_ALPHA_SATURATE", GL_SRC_ALPHA_SATURATE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TRUE", GL_TRUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FALSE", GL_FALSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE0", GL_CLIP_PLANE0 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE1", GL_CLIP_PLANE1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE2", GL_CLIP_PLANE2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE3", GL_CLIP_PLANE3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE4", GL_CLIP_PLANE4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIP_PLANE5", GL_CLIP_PLANE5 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BYTE", GL_BYTE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNSIGNED_BYTE", GL_UNSIGNED_BYTE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SHORT", GL_SHORT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNSIGNED_SHORT", GL_UNSIGNED_SHORT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INT", GL_INT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNSIGNED_INT", GL_UNSIGNED_INT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FLOAT", GL_FLOAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_2_BYTES", GL_2_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3_BYTES", GL_3_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_4_BYTES", GL_4_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DOUBLE", GL_DOUBLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NONE", GL_NONE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_LEFT", GL_FRONT_LEFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_RIGHT", GL_FRONT_RIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BACK_LEFT", GL_BACK_LEFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BACK_RIGHT", GL_BACK_RIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT", GL_FRONT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BACK", GL_BACK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LEFT", GL_LEFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RIGHT", GL_RIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_AND_BACK", GL_FRONT_AND_BACK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX0", GL_AUX0 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX1", GL_AUX1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX2", GL_AUX2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX3", GL_AUX3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NO_ERROR", GL_NO_ERROR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVALID_ENUM", GL_INVALID_ENUM , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVALID_VALUE", GL_INVALID_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVALID_OPERATION", GL_INVALID_OPERATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STACK_OVERFLOW", GL_STACK_OVERFLOW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STACK_UNDERFLOW", GL_STACK_UNDERFLOW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OUT_OF_MEMORY", GL_OUT_OF_MEMORY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_2D", GL_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3D", GL_3D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3D_COLOR", GL_3D_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_3D_COLOR_TEXTURE", GL_3D_COLOR_TEXTURE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_4D_COLOR_TEXTURE", GL_4D_COLOR_TEXTURE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PASS_THROUGH_TOKEN", GL_PASS_THROUGH_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_TOKEN", GL_POINT_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_TOKEN", GL_LINE_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_TOKEN", GL_POLYGON_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BITMAP_TOKEN", GL_BITMAP_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DRAW_PIXEL_TOKEN", GL_DRAW_PIXEL_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COPY_PIXEL_TOKEN", GL_COPY_PIXEL_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_RESET_TOKEN", GL_LINE_RESET_TOKEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXP", GL_EXP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXP2", GL_EXP2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CW", GL_CW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CCW", GL_CCW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COEFF", GL_COEFF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ORDER", GL_ORDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DOMAIN", GL_DOMAIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_COLOR", GL_CURRENT_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_INDEX", GL_CURRENT_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_NORMAL", GL_CURRENT_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_TEXTURE_COORDS", GL_CURRENT_TEXTURE_COORDS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_COLOR", GL_CURRENT_RASTER_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_INDEX", GL_CURRENT_RASTER_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_TEXTURE_COORDS", GL_CURRENT_RASTER_TEXTURE_COORDS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_POSITION", GL_CURRENT_RASTER_POSITION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_POSITION_VALID", GL_CURRENT_RASTER_POSITION_VALID , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CURRENT_RASTER_DISTANCE", GL_CURRENT_RASTER_DISTANCE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SMOOTH", GL_POINT_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SIZE", GL_POINT_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SIZE_RANGE", GL_POINT_SIZE_RANGE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SIZE_GRANULARITY", GL_POINT_SIZE_GRANULARITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_SMOOTH", GL_LINE_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_WIDTH", GL_LINE_WIDTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_WIDTH_RANGE", GL_LINE_WIDTH_RANGE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_WIDTH_GRANULARITY", GL_LINE_WIDTH_GRANULARITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STIPPLE", GL_LINE_STIPPLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STIPPLE_PATTERN", GL_LINE_STIPPLE_PATTERN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_STIPPLE_REPEAT", GL_LINE_STIPPLE_REPEAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_MODE", GL_LIST_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_LIST_NESTING", GL_MAX_LIST_NESTING , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_BASE", GL_LIST_BASE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIST_INDEX", GL_LIST_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_MODE", GL_POLYGON_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_SMOOTH", GL_POLYGON_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_STIPPLE", GL_POLYGON_STIPPLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG", GL_EDGE_FLAG , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CULL_FACE", GL_CULL_FACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CULL_FACE_MODE", GL_CULL_FACE_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FRONT_FACE", GL_FRONT_FACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHTING", GL_LIGHTING , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT_MODEL_LOCAL_VIEWER", GL_LIGHT_MODEL_LOCAL_VIEWER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT_MODEL_TWO_SIDE", GL_LIGHT_MODEL_TWO_SIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT_MODEL_AMBIENT", GL_LIGHT_MODEL_AMBIENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SHADE_MODEL", GL_SHADE_MODEL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_MATERIAL_FACE", GL_COLOR_MATERIAL_FACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_MATERIAL_PARAMETER", GL_COLOR_MATERIAL_PARAMETER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_MATERIAL", GL_COLOR_MATERIAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG", GL_FOG , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_INDEX", GL_FOG_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_DENSITY", GL_FOG_DENSITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_START", GL_FOG_START , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_END", GL_FOG_END , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_MODE", GL_FOG_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_COLOR", GL_FOG_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_RANGE", GL_DEPTH_RANGE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_TEST", GL_DEPTH_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_WRITEMASK", GL_DEPTH_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_CLEAR_VALUE", GL_DEPTH_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_FUNC", GL_DEPTH_FUNC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_CLEAR_VALUE", GL_ACCUM_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_TEST", GL_STENCIL_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_CLEAR_VALUE", GL_STENCIL_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_FUNC", GL_STENCIL_FUNC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_VALUE_MASK", GL_STENCIL_VALUE_MASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_FAIL", GL_STENCIL_FAIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_PASS_DEPTH_FAIL", GL_STENCIL_PASS_DEPTH_FAIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_PASS_DEPTH_PASS", GL_STENCIL_PASS_DEPTH_PASS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_REF", GL_STENCIL_REF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_WRITEMASK", GL_STENCIL_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MATRIX_MODE", GL_MATRIX_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMALIZE", GL_NORMALIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VIEWPORT", GL_VIEWPORT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODELVIEW_STACK_DEPTH", GL_MODELVIEW_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROJECTION_STACK_DEPTH", GL_PROJECTION_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_STACK_DEPTH", GL_TEXTURE_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODELVIEW_MATRIX", GL_MODELVIEW_MATRIX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROJECTION_MATRIX", GL_PROJECTION_MATRIX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_MATRIX", GL_TEXTURE_MATRIX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ATTRIB_STACK_DEPTH", GL_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_ATTRIB_STACK_DEPTH", GL_CLIENT_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_TEST", GL_ALPHA_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_TEST_FUNC", GL_ALPHA_TEST_FUNC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_TEST_REF", GL_ALPHA_TEST_REF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DITHER", GL_DITHER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLEND_DST", GL_BLEND_DST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLEND_SRC", GL_BLEND_SRC , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLEND", GL_BLEND , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LOGIC_OP_MODE", GL_LOGIC_OP_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_LOGIC_OP", GL_INDEX_LOGIC_OP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_LOGIC_OP", GL_COLOR_LOGIC_OP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUX_BUFFERS", GL_AUX_BUFFERS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DRAW_BUFFER", GL_DRAW_BUFFER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_READ_BUFFER", GL_READ_BUFFER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SCISSOR_BOX", GL_SCISSOR_BOX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SCISSOR_TEST", GL_SCISSOR_TEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_CLEAR_VALUE", GL_INDEX_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_WRITEMASK", GL_INDEX_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_CLEAR_VALUE", GL_COLOR_CLEAR_VALUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_WRITEMASK", GL_COLOR_WRITEMASK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_MODE", GL_INDEX_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA_MODE", GL_RGBA_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DOUBLEBUFFER", GL_DOUBLEBUFFER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STEREO", GL_STEREO , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RENDER_MODE", GL_RENDER_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PERSPECTIVE_CORRECTION_HINT", GL_PERSPECTIVE_CORRECTION_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT_SMOOTH_HINT", GL_POINT_SMOOTH_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE_SMOOTH_HINT", GL_LINE_SMOOTH_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_SMOOTH_HINT", GL_POLYGON_SMOOTH_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_HINT", GL_FOG_HINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_S", GL_TEXTURE_GEN_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_T", GL_TEXTURE_GEN_T , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_R", GL_TEXTURE_GEN_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_Q", GL_TEXTURE_GEN_Q , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_I", GL_PIXEL_MAP_I_TO_I , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_S_TO_S", GL_PIXEL_MAP_S_TO_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_R", GL_PIXEL_MAP_I_TO_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_G", GL_PIXEL_MAP_I_TO_G , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_B", GL_PIXEL_MAP_I_TO_B , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_A", GL_PIXEL_MAP_I_TO_A , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_R_TO_R", GL_PIXEL_MAP_R_TO_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_G_TO_G", GL_PIXEL_MAP_G_TO_G , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_B_TO_B", GL_PIXEL_MAP_B_TO_B , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_A_TO_A", GL_PIXEL_MAP_A_TO_A , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_I_SIZE", GL_PIXEL_MAP_I_TO_I_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_S_TO_S_SIZE", GL_PIXEL_MAP_S_TO_S_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_R_SIZE", GL_PIXEL_MAP_I_TO_R_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_G_SIZE", GL_PIXEL_MAP_I_TO_G_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_B_SIZE", GL_PIXEL_MAP_I_TO_B_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_I_TO_A_SIZE", GL_PIXEL_MAP_I_TO_A_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_R_TO_R_SIZE", GL_PIXEL_MAP_R_TO_R_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_G_TO_G_SIZE", GL_PIXEL_MAP_G_TO_G_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_B_TO_B_SIZE", GL_PIXEL_MAP_B_TO_B_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PIXEL_MAP_A_TO_A_SIZE", GL_PIXEL_MAP_A_TO_A_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_SWAP_BYTES", GL_UNPACK_SWAP_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_LSB_FIRST", GL_UNPACK_LSB_FIRST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_ROW_LENGTH", GL_UNPACK_ROW_LENGTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_SKIP_ROWS", GL_UNPACK_SKIP_ROWS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_SKIP_PIXELS", GL_UNPACK_SKIP_PIXELS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_UNPACK_ALIGNMENT", GL_UNPACK_ALIGNMENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_SWAP_BYTES", GL_PACK_SWAP_BYTES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_LSB_FIRST", GL_PACK_LSB_FIRST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_ROW_LENGTH", GL_PACK_ROW_LENGTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_SKIP_ROWS", GL_PACK_SKIP_ROWS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_SKIP_PIXELS", GL_PACK_SKIP_PIXELS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PACK_ALIGNMENT", GL_PACK_ALIGNMENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP_COLOR", GL_MAP_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP_STENCIL", GL_MAP_STENCIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_SHIFT", GL_INDEX_SHIFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_OFFSET", GL_INDEX_OFFSET , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED_SCALE", GL_RED_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED_BIAS", GL_RED_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ZOOM_X", GL_ZOOM_X , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ZOOM_Y", GL_ZOOM_Y , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN_SCALE", GL_GREEN_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN_BIAS", GL_GREEN_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE_SCALE", GL_BLUE_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE_BIAS", GL_BLUE_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_SCALE", GL_ALPHA_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_BIAS", GL_ALPHA_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_SCALE", GL_DEPTH_SCALE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_BIAS", GL_DEPTH_BIAS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_EVAL_ORDER", GL_MAX_EVAL_ORDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_LIGHTS", GL_MAX_LIGHTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_CLIP_PLANES", GL_MAX_CLIP_PLANES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_TEXTURE_SIZE", GL_MAX_TEXTURE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_PIXEL_MAP_TABLE", GL_MAX_PIXEL_MAP_TABLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_ATTRIB_STACK_DEPTH", GL_MAX_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_MODELVIEW_STACK_DEPTH", GL_MAX_MODELVIEW_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_NAME_STACK_DEPTH", GL_MAX_NAME_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_PROJECTION_STACK_DEPTH", GL_MAX_PROJECTION_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_TEXTURE_STACK_DEPTH", GL_MAX_TEXTURE_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_VIEWPORT_DIMS", GL_MAX_VIEWPORT_DIMS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_CLIENT_ATTRIB_STACK_DEPTH", GL_MAX_CLIENT_ATTRIB_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SUBPIXEL_BITS", GL_SUBPIXEL_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_BITS", GL_INDEX_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED_BITS", GL_RED_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN_BITS", GL_GREEN_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE_BITS", GL_BLUE_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA_BITS", GL_ALPHA_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_BITS", GL_DEPTH_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_BITS", GL_STENCIL_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_RED_BITS", GL_ACCUM_RED_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_GREEN_BITS", GL_ACCUM_GREEN_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_BLUE_BITS", GL_ACCUM_BLUE_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ACCUM_ALPHA_BITS", GL_ACCUM_ALPHA_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NAME_STACK_DEPTH", GL_NAME_STACK_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AUTO_NORMAL", GL_AUTO_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_COLOR_4", GL_MAP1_COLOR_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_INDEX", GL_MAP1_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_NORMAL", GL_MAP1_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_1", GL_MAP1_TEXTURE_COORD_1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_2", GL_MAP1_TEXTURE_COORD_2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_3", GL_MAP1_TEXTURE_COORD_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_TEXTURE_COORD_4", GL_MAP1_TEXTURE_COORD_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_VERTEX_3", GL_MAP1_VERTEX_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_VERTEX_4", GL_MAP1_VERTEX_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_COLOR_4", GL_MAP2_COLOR_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_INDEX", GL_MAP2_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_NORMAL", GL_MAP2_NORMAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_1", GL_MAP2_TEXTURE_COORD_1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_2", GL_MAP2_TEXTURE_COORD_2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_3", GL_MAP2_TEXTURE_COORD_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_TEXTURE_COORD_4", GL_MAP2_TEXTURE_COORD_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_VERTEX_3", GL_MAP2_VERTEX_3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_VERTEX_4", GL_MAP2_VERTEX_4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_GRID_DOMAIN", GL_MAP1_GRID_DOMAIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP1_GRID_SEGMENTS", GL_MAP1_GRID_SEGMENTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_GRID_DOMAIN", GL_MAP2_GRID_DOMAIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAP2_GRID_SEGMENTS", GL_MAP2_GRID_SEGMENTS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_1D", GL_TEXTURE_1D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_2D", GL_TEXTURE_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK_BUFFER_POINTER", GL_FEEDBACK_BUFFER_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK_BUFFER_SIZE", GL_FEEDBACK_BUFFER_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK_BUFFER_TYPE", GL_FEEDBACK_BUFFER_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SELECTION_BUFFER_POINTER", GL_SELECTION_BUFFER_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SELECTION_BUFFER_SIZE", GL_SELECTION_BUFFER_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_WIDTH", GL_TEXTURE_WIDTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_HEIGHT", GL_TEXTURE_HEIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_INTERNAL_FORMAT", GL_TEXTURE_INTERNAL_FORMAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BORDER_COLOR", GL_TEXTURE_BORDER_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BORDER", GL_TEXTURE_BORDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DONT_CARE", GL_DONT_CARE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FASTEST", GL_FASTEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NICEST", GL_NICEST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT0", GL_LIGHT0 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT1", GL_LIGHT1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT2", GL_LIGHT2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT3", GL_LIGHT3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT4", GL_LIGHT4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT5", GL_LIGHT5 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT6", GL_LIGHT6 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LIGHT7", GL_LIGHT7 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AMBIENT", GL_AMBIENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DIFFUSE", GL_DIFFUSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPECULAR", GL_SPECULAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POSITION", GL_POSITION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPOT_DIRECTION", GL_SPOT_DIRECTION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPOT_EXPONENT", GL_SPOT_EXPONENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPOT_CUTOFF", GL_SPOT_CUTOFF , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CONSTANT_ATTENUATION", GL_CONSTANT_ATTENUATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR_ATTENUATION", GL_LINEAR_ATTENUATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_QUADRATIC_ATTENUATION", GL_QUADRATIC_ATTENUATION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COMPILE", GL_COMPILE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COMPILE_AND_EXECUTE", GL_COMPILE_AND_EXECUTE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLEAR", GL_CLEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AND", GL_AND , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AND_REVERSE", GL_AND_REVERSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COPY", GL_COPY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AND_INVERTED", GL_AND_INVERTED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NOOP", GL_NOOP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_XOR", GL_XOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OR", GL_OR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NOR", GL_NOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EQUIV", GL_EQUIV , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INVERT", GL_INVERT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OR_REVERSE", GL_OR_REVERSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COPY_INVERTED", GL_COPY_INVERTED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OR_INVERTED", GL_OR_INVERTED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NAND", GL_NAND , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SET", GL_SET , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EMISSION", GL_EMISSION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SHININESS", GL_SHININESS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_AMBIENT_AND_DIFFUSE", GL_AMBIENT_AND_DIFFUSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEXES", GL_COLOR_INDEXES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODELVIEW", GL_MODELVIEW , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROJECTION", GL_PROJECTION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE", GL_TEXTURE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR", GL_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH", GL_DEPTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL", GL_STENCIL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX", GL_COLOR_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_STENCIL_INDEX", GL_STENCIL_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DEPTH_COMPONENT", GL_DEPTH_COMPONENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RED", GL_RED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_GREEN", GL_GREEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BLUE", GL_BLUE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA", GL_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB", GL_RGB , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA", GL_RGBA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BGR", GL_BGR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BGRA", GL_BGRA, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB_INTEGER", GL_RGB_INTEGER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE", GL_LUMINANCE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE_ALPHA", GL_LUMINANCE_ALPHA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BITMAP", GL_BITMAP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POINT", GL_POINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINE", GL_LINE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FILL", GL_FILL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RENDER", GL_RENDER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FEEDBACK", GL_FEEDBACK , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SELECT", GL_SELECT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FLAT", GL_FLAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SMOOTH", GL_SMOOTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_KEEP", GL_KEEP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_REPLACE", GL_REPLACE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INCR", GL_INCR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DECR", GL_DECR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VENDOR", GL_VENDOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RENDERER", GL_RENDERER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERSION", GL_VERSION , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXTENSIONS", GL_EXTENSIONS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_S", GL_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T", GL_T , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_R", GL_R , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_Q", GL_Q , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MODULATE", GL_MODULATE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_DECAL", GL_DECAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ENV_MODE", GL_TEXTURE_ENV_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ENV_COLOR", GL_TEXTURE_ENV_COLOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ENV", GL_TEXTURE_ENV , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EYE_LINEAR", GL_EYE_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OBJECT_LINEAR", GL_OBJECT_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_SPHERE_MAP", GL_SPHERE_MAP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GEN_MODE", GL_TEXTURE_GEN_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_OBJECT_PLANE", GL_OBJECT_PLANE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EYE_PLANE", GL_EYE_PLANE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEAREST", GL_NEAREST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR", GL_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_MAG_FILTER", GL_TEXTURE_MAG_FILTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_MIN_FILTER", GL_TEXTURE_MIN_FILTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_WRAP_S", GL_TEXTURE_WRAP_S , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_WRAP_T", GL_TEXTURE_WRAP_T , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLAMP", GL_CLAMP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_REPEAT", GL_REPEAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_PIXEL_STORE_BIT", GL_CLIENT_PIXEL_STORE_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_VERTEX_ARRAY_BIT", GL_CLIENT_VERTEX_ARRAY_BIT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_FACTOR", GL_POLYGON_OFFSET_FACTOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_UNITS", GL_POLYGON_OFFSET_UNITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_POINT", GL_POLYGON_OFFSET_POINT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_LINE", GL_POLYGON_OFFSET_LINE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_POLYGON_OFFSET_FILL", GL_POLYGON_OFFSET_FILL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA4", GL_ALPHA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA8", GL_ALPHA8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA12", GL_ALPHA12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_ALPHA16", GL_ALPHA16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE4", GL_LUMINANCE4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE8", GL_LUMINANCE8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE12", GL_LUMINANCE12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE16", GL_LUMINANCE16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE4_ALPHA4", GL_LUMINANCE4_ALPHA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE6_ALPHA2", GL_LUMINANCE6_ALPHA2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE8_ALPHA8", GL_LUMINANCE8_ALPHA8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE12_ALPHA4", GL_LUMINANCE12_ALPHA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE12_ALPHA12", GL_LUMINANCE12_ALPHA12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_LUMINANCE16_ALPHA16", GL_LUMINANCE16_ALPHA16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY", GL_INTENSITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY4", GL_INTENSITY4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY8", GL_INTENSITY8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY12", GL_INTENSITY12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INTENSITY16", GL_INTENSITY16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_R3_G3_B2", GL_R3_G3_B2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB4", GL_RGB4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB5", GL_RGB5 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB8", GL_RGB8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB10", GL_RGB10 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB12", GL_RGB12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB16", GL_RGB16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA2", GL_RGBA2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA4", GL_RGBA4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB5_A1", GL_RGB5_A1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA8", GL_RGBA8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGB10_A2", GL_RGB10_A2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA12", GL_RGBA12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_RGBA16", GL_RGBA16 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_RED_SIZE", GL_TEXTURE_RED_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_GREEN_SIZE", GL_TEXTURE_GREEN_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BLUE_SIZE", GL_TEXTURE_BLUE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_ALPHA_SIZE", GL_TEXTURE_ALPHA_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_LUMINANCE_SIZE", GL_TEXTURE_LUMINANCE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_INTENSITY_SIZE", GL_TEXTURE_INTENSITY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROXY_TEXTURE_1D", GL_PROXY_TEXTURE_1D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PROXY_TEXTURE_2D", GL_PROXY_TEXTURE_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_PRIORITY", GL_TEXTURE_PRIORITY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_RESIDENT", GL_TEXTURE_RESIDENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BINDING_1D", GL_TEXTURE_BINDING_1D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_BINDING_2D", GL_TEXTURE_BINDING_2D , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY", GL_VERTEX_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY", GL_NORMAL_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY", GL_COLOR_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY", GL_INDEX_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY", GL_TEXTURE_COORD_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY", GL_EDGE_FLAG_ARRAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_SIZE", GL_VERTEX_ARRAY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_TYPE", GL_VERTEX_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_STRIDE", GL_VERTEX_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_TYPE", GL_NORMAL_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_STRIDE", GL_NORMAL_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_SIZE", GL_COLOR_ARRAY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_TYPE", GL_COLOR_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_STRIDE", GL_COLOR_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_TYPE", GL_INDEX_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_STRIDE", GL_INDEX_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_SIZE", GL_TEXTURE_COORD_ARRAY_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_TYPE", GL_TEXTURE_COORD_ARRAY_TYPE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_STRIDE", GL_TEXTURE_COORD_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_STRIDE", GL_EDGE_FLAG_ARRAY_STRIDE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_POINTER", GL_VERTEX_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_POINTER", GL_NORMAL_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_POINTER", GL_COLOR_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_POINTER", GL_INDEX_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_POINTER", GL_TEXTURE_COORD_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_POINTER", GL_EDGE_FLAG_ARRAY_POINTER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_V2F", GL_V2F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_V3F", GL_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C4UB_V2F", GL_C4UB_V2F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C4UB_V3F", GL_C4UB_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C3F_V3F", GL_C3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_N3F_V3F", GL_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_C4F_N3F_V3F", GL_C4F_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_V3F", GL_T2F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T4F_V4F", GL_T4F_V4F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_C4UB_V3F", GL_T2F_C4UB_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_C3F_V3F", GL_T2F_C3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_N3F_V3F", GL_T2F_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T2F_C4F_N3F_V3F", GL_T2F_C4F_N3F_V3F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_T4F_C4F_N3F_V4F", GL_T4F_C4F_N3F_V4F , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EXT_bgra", GL_EXT_bgra , CONST_CS | CONST_PERSISTENT);

#ifdef GL_VERTEX_ARRAY_EXT
	REGISTER_LONG_CONSTANT("GL_EXT_vertex_array", GL_EXT_vertex_array, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_EXT", GL_VERTEX_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_SIZE_EXT", GL_VERTEX_ARRAY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_TYPE_EXT", GL_VERTEX_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_STRIDE_EXT", GL_VERTEX_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_COUNT_EXT", GL_VERTEX_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_VERTEX_ARRAY_POINTER_EXT", GL_VERTEX_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
#endif
#ifdef GL_NORMAL_ARRAY_EXT
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_EXT", GL_NORMAL_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_TYPE_EXT", GL_NORMAL_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_STRIDE_EXT", GL_NORMAL_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_COUNT_EXT", GL_NORMAL_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_NORMAL_ARRAY_POINTER_EXT", GL_NORMAL_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
#endif
#ifdef GL_COLOR_ARRAY_EXT
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_EXT", GL_COLOR_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_SIZE_EXT", GL_COLOR_ARRAY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_TYPE_EXT", GL_COLOR_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_STRIDE_EXT", GL_COLOR_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_COUNT_EXT", GL_COLOR_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_ARRAY_POINTER_EXT", GL_COLOR_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
#endif
#ifdef GL_INDEX_ARRAY_EXT
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_EXT", GL_INDEX_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_TYPE_EXT", GL_INDEX_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_STRIDE_EXT", GL_INDEX_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_COUNT_EXT", GL_INDEX_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_INDEX_ARRAY_POINTER_EXT", GL_INDEX_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
#endif
#ifdef GL_TEXTURE_COORD_ARRAY_EXT
	REGISTER_LONG_CONSTANT("GL_EXT_paletted_texture", GL_EXT_paletted_texture , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_EXT", GL_TEXTURE_COORD_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_SIZE_EXT", GL_TEXTURE_COORD_ARRAY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_TYPE_EXT", GL_TEXTURE_COORD_ARRAY_TYPE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_STRIDE_EXT", GL_TEXTURE_COORD_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_COUNT_EXT", GL_TEXTURE_COORD_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COORD_ARRAY_POINTER_EXT", GL_TEXTURE_COORD_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
#endif
#ifdef GL_EDGE_FLAG_ARRAY_EXT
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_EXT", GL_EDGE_FLAG_ARRAY_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_STRIDE_EXT", GL_EDGE_FLAG_ARRAY_STRIDE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_COUNT_EXT", GL_EDGE_FLAG_ARRAY_COUNT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_EDGE_FLAG_ARRAY_POINTER_EXT", GL_EDGE_FLAG_ARRAY_POINTER_EXT , CONST_CS | CONST_PERSISTENT);
#endif
	REGISTER_LONG_CONSTANT("GL_BGR_EXT", GL_BGR_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_BGRA_EXT", GL_BGRA_EXT , CONST_CS | CONST_PERSISTENT);
	/*
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_FORMAT_EXT", GL_COLOR_TABLE_FORMAT_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_WIDTH_EXT", GL_COLOR_TABLE_WIDTH_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_RED_SIZE_EXT", GL_COLOR_TABLE_RED_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_GREEN_SIZE_EXT", GL_COLOR_TABLE_GREEN_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_BLUE_SIZE_EXT", GL_COLOR_TABLE_BLUE_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_ALPHA_SIZE_EXT", GL_COLOR_TABLE_ALPHA_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_LUMINANCE_SIZE_EXT", GL_COLOR_TABLE_LUMINANCE_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_TABLE_INTENSITY_SIZE_EXT", GL_COLOR_TABLE_INTENSITY_SIZE_EXT , CONST_CS | CONST_PERSISTENT);
	*/
#if !(PHP_OSX)
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX1_EXT", GL_COLOR_INDEX1_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX2_EXT", GL_COLOR_INDEX2_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX4_EXT", GL_COLOR_INDEX4_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX8_EXT", GL_COLOR_INDEX8_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX12_EXT", GL_COLOR_INDEX12_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_COLOR_INDEX16_EXT", GL_COLOR_INDEX16_EXT , CONST_CS | CONST_PERSISTENT);
#endif
#ifdef PHP_WIN32
	REGISTER_LONG_CONSTANT("GL_DOUBLE_EXT", GL_DOUBLE_EXT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_CLIENT_ALL_ATTRIB_BITS", GL_CLIENT_ALL_ATTRIB_BITS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_WIN_swap_hint", GL_WIN_swap_hint , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_WIN_draw_range_elements", GL_WIN_draw_range_elements , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_ELEMENTS_VERTICES_WIN", GL_MAX_ELEMENTS_VERTICES_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_MAX_ELEMENTS_INDICES_WIN", GL_MAX_ELEMENTS_INDICES_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PHONG_WIN", GL_PHONG_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_PHONG_HINT_WIN", GL_PHONG_HINT_WIN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_FOG_SPECULAR_TEXTURE_WIN", GL_FOG_SPECULAR_TEXTURE_WIN , CONST_CS | CONST_PERSISTENT);
#endif
	REGISTER_LONG_CONSTANT("GL_LOGIC_OP", GL_LOGIC_OP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GL_TEXTURE_COMPONENTS", GL_TEXTURE_COMPONENTS , CONST_CS | CONST_PERSISTENT);

    REGISTER_LONG_CONSTANT("GL_ARRAY_BUFFER", GL_ARRAY_BUFFER, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GL_STATIC_DRAW", GL_STATIC_DRAW, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GL_ELEMENT_ARRAY_BUFFER", GL_ELEMENT_ARRAY_BUFFER, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GL_VERTEX_SHADER", GL_VERTEX_SHADER, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GL_FRAGMENT_SHADER", GL_FRAGMENT_SHADER, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GL_SHADING_LANGUAGE_VERSION", GL_SHADING_LANGUAGE_VERSION, CONST_CS | CONST_PERSISTENT);

    REGISTER_LONG_CONSTANT("GL_MINOR_VERSION", GL_MINOR_VERSION, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GL_MAJOR_VERSION", GL_MAJOR_VERSION, CONST_CS | CONST_PERSISTENT);
    
    REGISTER_LONG_CONSTANT("GL_TEXTURE0", GL_TEXTURE0, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GL_TEXTURE1", GL_TEXTURE1, CONST_CS | CONST_PERSISTENT);
    
    REGISTER_LONG_CONSTANT("GL_CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE, CONST_CS | CONST_PERSISTENT);
        
	PHP_MINIT(glut)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}

