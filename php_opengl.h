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

#ifndef PHP_OPENGL_H
#define PHP_OPENGL_H

#define PHP_OPENGL_VERSION "0.9.10"

#include "php.h"
#include "ext/standard/info.h"

extern zend_module_entry opengl_module_entry;
#define opengl_module_ptr &opengl_module_entry
#define phpext_opengl_ptr opengl_module_ptr

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(opengl);
PHP_MINFO_FUNCTION(opengl);

PHP_FUNCTION(glbindtexture);
PHP_FUNCTION(glblendfunc);
PHP_FUNCTION(glclear);
PHP_FUNCTION(glclearcolor);
PHP_FUNCTION(glcleardepth);
PHP_FUNCTION(glclearstencil);
PHP_FUNCTION(glcolormask);
PHP_FUNCTION(glcopyteximage1d);
PHP_FUNCTION(glcopytexsubimage1d);
PHP_FUNCTION(glcullface);
PHP_FUNCTION(gldeletetextures);
PHP_FUNCTION(gldepthfunc);
PHP_FUNCTION(gldepthmask);
PHP_FUNCTION(gldepthrange);
PHP_FUNCTION(gldisable);
PHP_FUNCTION(gldrawarrays);
PHP_FUNCTION(gldrawbuffer);
PHP_FUNCTION(gldrawelements);
PHP_FUNCTION(glenable);
PHP_FUNCTION(glfinish);
PHP_FUNCTION(glflush);
PHP_FUNCTION(glfrontface);
PHP_FUNCTION(glgentextures);
PHP_FUNCTION(glgetbooleanv);
PHP_FUNCTION(glgetdoublev);
PHP_FUNCTION(glgeterror);
PHP_FUNCTION(glgetfloatv);
PHP_FUNCTION(glgetintegerv);
PHP_FUNCTION(glGetString);
PHP_FUNCTION(glgetteximage);
PHP_FUNCTION(glgettexlevelparameterfv);
PHP_FUNCTION(glgettexlevelparameteriv);
PHP_FUNCTION(glgettexparameterfv);
PHP_FUNCTION(glgettexparameteriv);
PHP_FUNCTION(glhint);
PHP_FUNCTION(glisenabled);
PHP_FUNCTION(glistexture);
PHP_FUNCTION(gllinewidth);
PHP_FUNCTION(gllogicop);
PHP_FUNCTION(glpixelstoref);
PHP_FUNCTION(glpixelstorei);
PHP_FUNCTION(glpointsize);
PHP_FUNCTION(glpolygonmode);
PHP_FUNCTION(glpolygonoffset);
PHP_FUNCTION(glpolygonstipple);
PHP_FUNCTION(glreadbuffer);
PHP_FUNCTION(glreadpixels);
PHP_FUNCTION(glscissor);
PHP_FUNCTION(glstencilfunc);
PHP_FUNCTION(glstencilmask);
PHP_FUNCTION(glstencilop);
PHP_FUNCTION(glteximage2d);
PHP_FUNCTION(gltexparameterf);
PHP_FUNCTION(gltexparameterfv);
PHP_FUNCTION(gltexparameteri);
PHP_FUNCTION(gltexparameteriv);
PHP_FUNCTION(gltexsubimage1d);
PHP_FUNCTION(gltexsubimage2d);
PHP_FUNCTION(glviewport);
PHP_FUNCTION(glGenVertexArrays);
PHP_FUNCTION(glBindVertexArray);
PHP_FUNCTION(glGenBuffers);
PHP_FUNCTION(glBindBuffer);
PHP_FUNCTION(glbufferdata);
PHP_FUNCTION(glCreateShader);
PHP_FUNCTION(gldeletevertexarrays);
PHP_FUNCTION(gldeletebuffers);
PHP_FUNCTION(gldeleteshader);
PHP_FUNCTION(gldeleteprogram);
PHP_FUNCTION(glshadersource);
PHP_FUNCTION(glcompileshader);
PHP_FUNCTION(glattachshader);
PHP_FUNCTION(glbindfragdatalocation);
PHP_FUNCTION(gllinkprogram);
PHP_FUNCTION(gluseprogram);
PHP_FUNCTION(glcreateprogram);
PHP_FUNCTION(glgetattriblocation);
PHP_FUNCTION(glenablevertexattribarray);
PHP_FUNCTION(gldisablevertexattribarray);
PHP_FUNCTION(glvertexattribpointer);
PHP_FUNCTION(glGetUniformLocation);
PHP_FUNCTION(glActiveTexture);
PHP_FUNCTION(glUniformMatrix4fv);
PHP_FUNCTION(glUniform1i);
PHP_FUNCTION(glUniform3f);

#endif
