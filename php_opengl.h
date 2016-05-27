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

#define PHP_OPENGL_VERSION "0.9.0"

#include <php.h>
#include <ext/standard/info.h>

extern zend_module_entry opengl_module_entry;
#define opengl_module_ptr &opengl_module_entry
#define phpext_opengl_ptr opengl_module_ptr

#ifdef ZTS
#include "TSRM.h"
#endif

/* Functions */

PHP_MINIT_FUNCTION(opengl);
PHP_RINIT_FUNCTION(opengl);
PHP_RSHUTDOWN_FUNCTION(opengl);
PHP_MINFO_FUNCTION(opengl);

PHP_FUNCTION(glbindtexture);
PHP_FUNCTION(glblendfunc);
PHP_FUNCTION(glclear);
PHP_FUNCTION(glclearcolor);
PHP_FUNCTION(glcleardepth);
PHP_FUNCTION(glclearstencil);
PHP_FUNCTION(glcolormask);
PHP_FUNCTION(glcolormaterial);
PHP_FUNCTION(glcolorpointer);
PHP_FUNCTION(glcopyteximage1d);
PHP_FUNCTION(glcopyteximage2d);
PHP_FUNCTION(glcopytexsubimage1d);
PHP_FUNCTION(glcopytexsubimage2d);
PHP_FUNCTION(glcullface);
PHP_FUNCTION(gldeletelists);
PHP_FUNCTION(gldeletetextures);
PHP_FUNCTION(gldepthfunc);
PHP_FUNCTION(gldepthmask);
PHP_FUNCTION(gldepthrange);
PHP_FUNCTION(gldisable);
PHP_FUNCTION(gldisableclientstate);
PHP_FUNCTION(gldrawarrays);
PHP_FUNCTION(gldrawbuffer);
PHP_FUNCTION(gldrawelements);
PHP_FUNCTION(gledgeflag);
PHP_FUNCTION(gledgeflagpointer);
PHP_FUNCTION(gledgeflagv);
PHP_FUNCTION(glenable);
PHP_FUNCTION(glenableclientstate);
PHP_FUNCTION(glevalpoint1);
PHP_FUNCTION(glevalpoint2);
PHP_FUNCTION(glfeedbackbuffer);
PHP_FUNCTION(glfinish);
PHP_FUNCTION(glflush);
PHP_FUNCTION(glfogf);
PHP_FUNCTION(glfogfv);
PHP_FUNCTION(glfogi);
PHP_FUNCTION(glfogiv);
PHP_FUNCTION(glfrontface);
PHP_FUNCTION(glgenlists);
PHP_FUNCTION(glgentextures);
PHP_FUNCTION(glgetbooleanv);
PHP_FUNCTION(glgetclipplane);
PHP_FUNCTION(glgetdoublev);
PHP_FUNCTION(glgeterror);
PHP_FUNCTION(glgetfloatv);
PHP_FUNCTION(glgetintegerv);
PHP_FUNCTION(glgetmapdv);
PHP_FUNCTION(glgetmapfv);
PHP_FUNCTION(glgetmapiv);
PHP_FUNCTION(glgetmaterialfv);
PHP_FUNCTION(glgetmaterialiv);
PHP_FUNCTION(glgetpixelmapfv);
PHP_FUNCTION(glgetpixelmapuiv);
PHP_FUNCTION(glgetpixelmapusv);
PHP_FUNCTION(glgetpointerv);
PHP_FUNCTION(glgetpolygonstipple);
PHP_FUNCTION(glgetstring);
PHP_FUNCTION(glgettexenvfv);
PHP_FUNCTION(glgettexenviv);
PHP_FUNCTION(glgettexgendv);
PHP_FUNCTION(glgettexgenfv);
PHP_FUNCTION(glgettexgeniv);
PHP_FUNCTION(glgetteximage);
PHP_FUNCTION(glgettexlevelparameterfv);
PHP_FUNCTION(glgettexlevelparameteriv);
PHP_FUNCTION(glgettexparameterfv);
PHP_FUNCTION(glgettexparameteriv);
PHP_FUNCTION(glhint);
PHP_FUNCTION(glindexmask);
PHP_FUNCTION(glindexpointer);
PHP_FUNCTION(glinitnames);
PHP_FUNCTION(glinterleavedarrays);
PHP_FUNCTION(glisenabled);
PHP_FUNCTION(glislist);
PHP_FUNCTION(glistexture);
PHP_FUNCTION(gllightmodelf);
PHP_FUNCTION(gllightmodelfv);
PHP_FUNCTION(gllightmodeli);
PHP_FUNCTION(gllightmodeliv);
PHP_FUNCTION(gllightf);
PHP_FUNCTION(gllightfv);
PHP_FUNCTION(gllighti);
PHP_FUNCTION(gllightiv);
PHP_FUNCTION(gllinestipple);
PHP_FUNCTION(gllinewidth);
PHP_FUNCTION(gllistbase);
PHP_FUNCTION(glloadidentity);
PHP_FUNCTION(glloadmatrixd);
PHP_FUNCTION(glloadmatrixf);
PHP_FUNCTION(glloadname);
PHP_FUNCTION(gllogicop);
PHP_FUNCTION(glmapgrid1d);
PHP_FUNCTION(glmapgrid1f);
PHP_FUNCTION(glmaterialf);
PHP_FUNCTION(glmaterialfv);
PHP_FUNCTION(glmateriali);
PHP_FUNCTION(glmaterialiv);
PHP_FUNCTION(glmatrixmode);
PHP_FUNCTION(glmultmatrixd);
PHP_FUNCTION(glmultmatrixf);
PHP_FUNCTION(glnewlist);
PHP_FUNCTION(glnormalpointer);
PHP_FUNCTION(glpassthrough);
PHP_FUNCTION(glpixelmapfv);
PHP_FUNCTION(glpixelmapuiv);
PHP_FUNCTION(glpixelmapusv);
PHP_FUNCTION(glpixelstoref);
PHP_FUNCTION(glpixelstorei);
PHP_FUNCTION(glpixeltransferf);
PHP_FUNCTION(glpixeltransferi);
PHP_FUNCTION(glpixelzoom);
PHP_FUNCTION(glpointsize);
PHP_FUNCTION(glpolygonmode);
PHP_FUNCTION(glpolygonoffset);
PHP_FUNCTION(glpolygonstipple);
PHP_FUNCTION(glpopattrib);
PHP_FUNCTION(glpopclientattrib);
PHP_FUNCTION(glpopmatrix);
PHP_FUNCTION(glpopname);
PHP_FUNCTION(glprioritizetextures);
PHP_FUNCTION(glpushattrib);
PHP_FUNCTION(glpushclientattrib);
PHP_FUNCTION(glpushmatrix);
PHP_FUNCTION(glpushname);
PHP_FUNCTION(glreadbuffer);
PHP_FUNCTION(glreadpixels);
PHP_FUNCTION(glrendermode);
PHP_FUNCTION(glrotated);
PHP_FUNCTION(glrotatef);
PHP_FUNCTION(glscaled);
PHP_FUNCTION(glscalef);
PHP_FUNCTION(glscissor);
PHP_FUNCTION(glselectbuffer);
PHP_FUNCTION(glshademodel);
PHP_FUNCTION(glstencilfunc);
PHP_FUNCTION(glstencilmask);
PHP_FUNCTION(glstencilop);
PHP_FUNCTION(gltexcoordpointer);
PHP_FUNCTION(gltexenvf);
PHP_FUNCTION(gltexenvfv);
PHP_FUNCTION(gltexenvi);
PHP_FUNCTION(gltexenviv);
PHP_FUNCTION(gltexgend);
PHP_FUNCTION(gltexgendv);
PHP_FUNCTION(gltexgenf);
PHP_FUNCTION(gltexgenfv);
PHP_FUNCTION(gltexgeni);
PHP_FUNCTION(gltexgeniv);
PHP_FUNCTION(glteximage1d);
PHP_FUNCTION(glteximage2d);
PHP_FUNCTION(gltexparameterf);
PHP_FUNCTION(gltexparameterfv);
PHP_FUNCTION(gltexparameteri);
PHP_FUNCTION(gltexparameteriv);
PHP_FUNCTION(gltexsubimage1d);
PHP_FUNCTION(gltexsubimage2d);
PHP_FUNCTION(gltranslated);
PHP_FUNCTION(gltranslatef);
PHP_FUNCTION(glvertexpointer);
PHP_FUNCTION(glviewport);
PHP_FUNCTION(glgenvertexarrays);
PHP_FUNCTION(glbindvertexarray);
PHP_FUNCTION(glgenbuffers);
PHP_FUNCTION(glbindbuffer);
PHP_FUNCTION(glbufferdata);
PHP_FUNCTION(glcreateshader);
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
PHP_FUNCTION(glvertexattribpointer);
PHP_FUNCTION(glGetUniformLocation);
PHP_FUNCTION(glActiveTexture);
PHP_FUNCTION(glUniformMatrix4fv);

#endif
