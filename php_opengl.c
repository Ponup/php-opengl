/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef PHP_WIN32
#include <windows.h>
#endif

#define PHP_OSX defined(__APPLE__) && defined(__MACH__)

#if PHP_OSX 
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "php.h"
#include "php_opengl.h"
#include "php_glu.h"
#include "php_glut.h"
#include "php_convert.h"

extern int le_nurb;
extern int le_tess;
extern int le_quad;

const zend_function_entry opengl_functions[] = {
	/* GL Functions */
	ZEND_FE(glaccum,NULL)
	ZEND_FE(glalphafunc,NULL)
	ZEND_FE(glaretexturesresident,NULL)
	ZEND_FE(glarrayelement,NULL)
	ZEND_FE(glbegin,NULL)
	ZEND_FE(glbindtexture,NULL)
	ZEND_FE(glbitmap,NULL)
	ZEND_FE(glblendfunc,NULL)
	ZEND_FE(glcalllist,NULL)
	ZEND_FE(glcalllists,NULL)
	ZEND_FE(glclear,NULL)
	ZEND_FE(glclearaccum,NULL)
	ZEND_FE(glclearcolor,NULL)
	ZEND_FE(glcleardepth,NULL)
	ZEND_FE(glclearindex,NULL)
	ZEND_FE(glclearstencil,NULL)
	ZEND_FE(glclipplane,NULL)
	ZEND_FE(glcolor3b,NULL)
	ZEND_FE(glcolor3bv,NULL)
	ZEND_FE(glcolor3d,NULL)
	ZEND_FE(glcolor3dv,NULL)
	ZEND_FE(glcolor3f,NULL)
	ZEND_FE(glcolor3fv,NULL)
	ZEND_FE(glcolor3i,NULL)
	ZEND_FE(glcolor3iv,NULL)
	ZEND_FE(glcolor3s,NULL)
	ZEND_FE(glcolor3sv,NULL)
	ZEND_FE(glcolor3ub,NULL)
	ZEND_FE(glcolor3ubv,NULL)
	ZEND_FE(glcolor3ui,NULL)
	ZEND_FE(glcolor3uiv,NULL)
	ZEND_FE(glcolor3us,NULL)
	ZEND_FE(glcolor3usv,NULL)
	ZEND_FE(glcolor4b,NULL)
	ZEND_FE(glcolor4bv,NULL)
	ZEND_FE(glcolor4d,NULL)
	ZEND_FE(glcolor4dv,NULL)
	ZEND_FE(glcolor4f,NULL)
	ZEND_FE(glcolor4fv,NULL)
	ZEND_FE(glcolor4i,NULL)
	ZEND_FE(glcolor4iv,NULL)
	ZEND_FE(glcolor4s,NULL)
	ZEND_FE(glcolor4sv,NULL)
	ZEND_FE(glcolor4ub,NULL)
	ZEND_FE(glcolor4ubv,NULL)
	ZEND_FE(glcolor4ui,NULL)
	ZEND_FE(glcolor4uiv,NULL)
	ZEND_FE(glcolor4us,NULL)
	ZEND_FE(glcolor4usv,NULL)
	ZEND_FE(glcolormask,NULL)
	ZEND_FE(glcolormaterial,NULL)
	ZEND_FE(glcolorpointer,NULL)
	ZEND_FE(glcopypixels,NULL)
	ZEND_FE(glcopyteximage1d,NULL)
	ZEND_FE(glcopyteximage2d,NULL)
	ZEND_FE(glcopytexsubimage1d,NULL)
	ZEND_FE(glcopytexsubimage2d,NULL)
	ZEND_FE(glcullface,NULL)
	ZEND_FE(gldeletelists,NULL)
	ZEND_FE(gldeletetextures,NULL)
	ZEND_FE(gldepthfunc,NULL)
	ZEND_FE(gldepthmask,NULL)
	ZEND_FE(gldepthrange,NULL)
	ZEND_FE(gldisable,NULL)
	ZEND_FE(gldisableclientstate,NULL)
	ZEND_FE(gldrawarrays,NULL)
	ZEND_FE(gldrawbuffer,NULL)
	ZEND_FE(gldrawelements,NULL)
	ZEND_FE(gldrawpixels,NULL)
	ZEND_FE(gledgeflag,NULL)
	ZEND_FE(gledgeflagpointer,NULL)
	ZEND_FE(gledgeflagv,NULL)
	ZEND_FE(glenable,NULL)
	ZEND_FE(glenableclientstate,NULL)
	ZEND_FE(glend,NULL)
	ZEND_FE(glendlist,NULL)
	ZEND_FE(glevalcoord1d,NULL)
	ZEND_FE(glevalcoord1dv,NULL)
	ZEND_FE(glevalcoord1f,NULL)
	ZEND_FE(glevalcoord1fv,NULL)
	ZEND_FE(glevalcoord2d,NULL)
	ZEND_FE(glevalcoord2dv,NULL)
	ZEND_FE(glevalcoord2f,NULL)
	ZEND_FE(glevalcoord2fv,NULL)
	ZEND_FE(glevalmesh1,NULL)
	ZEND_FE(glevalmesh2,NULL)
	ZEND_FE(glevalpoint1,NULL)
	ZEND_FE(glevalpoint2,NULL)
	ZEND_FE(glfeedbackbuffer,NULL)
	ZEND_FE(glfinish,NULL)
	ZEND_FE(glflush,NULL)
	ZEND_FE(glfogf,NULL)
	ZEND_FE(glfogfv,NULL)
	ZEND_FE(glfogi,NULL)
	ZEND_FE(glfogiv,NULL)
	ZEND_FE(glfrontface,NULL)
	ZEND_FE(glfrustum,NULL)
	ZEND_FE(glgenlists,NULL)
	ZEND_FE(glgentextures,NULL)
	ZEND_FE(glgetbooleanv,NULL)
	ZEND_FE(glgetclipplane,NULL)
	ZEND_FE(glgetdoublev,NULL)
	ZEND_FE(glgeterror,NULL)
	ZEND_FE(glgetfloatv,NULL)
	ZEND_FE(glgetintegerv,NULL)
	ZEND_FE(glgetlightfv,NULL)
	ZEND_FE(glgetlightiv,NULL)
	ZEND_FE(glgetmapdv,NULL)
	ZEND_FE(glgetmapfv,NULL)
	ZEND_FE(glgetmapiv,NULL)
	ZEND_FE(glgetmaterialfv,NULL)
	ZEND_FE(glgetmaterialiv,NULL)
	ZEND_FE(glgetpixelmapfv,NULL)
	ZEND_FE(glgetpixelmapuiv,NULL)
	ZEND_FE(glgetpixelmapusv,NULL)
	ZEND_FE(glgetpointerv,NULL)
	ZEND_FE(glgetpolygonstipple,NULL)
	ZEND_FE(glgetstring,NULL)
	ZEND_FE(glgettexenvfv,NULL)
	ZEND_FE(glgettexenviv,NULL)
	ZEND_FE(glgettexgendv,NULL)
	ZEND_FE(glgettexgenfv,NULL)
	ZEND_FE(glgettexgeniv,NULL)
	ZEND_FE(glgetteximage,NULL)
	ZEND_FE(glgettexlevelparameterfv,NULL)
	ZEND_FE(glgettexlevelparameteriv,NULL)
	ZEND_FE(glgettexparameterfv,NULL)
	ZEND_FE(glgettexparameteriv,NULL)
	ZEND_FE(glhint,NULL)
	ZEND_FE(glindexmask,NULL)
	ZEND_FE(glindexpointer,NULL)
	ZEND_FE(glindexd,NULL)
	ZEND_FE(glindexdv,NULL)
	ZEND_FE(glindexf,NULL)
	ZEND_FE(glindexfv,NULL)
	ZEND_FE(glindexi,NULL)
	ZEND_FE(glindexiv,NULL)
	ZEND_FE(glindexs,NULL)
	ZEND_FE(glindexsv,NULL)
	ZEND_FE(glindexub,NULL)
	ZEND_FE(glindexubv,NULL)
	ZEND_FE(glinitnames,NULL)
	ZEND_FE(glinterleavedarrays,NULL)
	ZEND_FE(glisenabled,NULL)
	ZEND_FE(glislist,NULL)
	ZEND_FE(glistexture,NULL)
	ZEND_FE(gllightmodelf,NULL)
	ZEND_FE(gllightmodelfv,NULL)
	ZEND_FE(gllightmodeli,NULL)
	ZEND_FE(gllightmodeliv,NULL)
	ZEND_FE(gllightf,NULL)
	ZEND_FE(gllightfv,NULL)
	ZEND_FE(gllighti,NULL)
	ZEND_FE(gllightiv,NULL)
	ZEND_FE(gllinestipple,NULL)
	ZEND_FE(gllinewidth,NULL)
	ZEND_FE(gllistbase,NULL)
	ZEND_FE(glloadidentity,NULL)
	ZEND_FE(glloadmatrixd,NULL)
	ZEND_FE(glloadmatrixf,NULL)
	ZEND_FE(glloadname,NULL)
	ZEND_FE(gllogicop,NULL)
	ZEND_FE(glmap1d,NULL)
	ZEND_FE(glmap1f,NULL)
	ZEND_FE(glmap2d,NULL)
	ZEND_FE(glmap2f,NULL)
	ZEND_FE(glmapgrid1d,NULL)
	ZEND_FE(glmapgrid1f,NULL)
	ZEND_FE(glmapgrid2d,NULL)
	ZEND_FE(glmapgrid2f,NULL)
	ZEND_FE(glmaterialf,NULL)
	ZEND_FE(glmaterialfv,NULL)
	ZEND_FE(glmateriali,NULL)
	ZEND_FE(glmaterialiv,NULL)
	ZEND_FE(glmatrixmode,NULL)
	ZEND_FE(glmultmatrixd,NULL)
	ZEND_FE(glmultmatrixf,NULL)
	ZEND_FE(glnewlist,NULL)
	ZEND_FE(glnormal3b,NULL)
	ZEND_FE(glnormal3bv,NULL)
	ZEND_FE(glnormal3d,NULL)
	ZEND_FE(glnormal3dv,NULL)
	ZEND_FE(glnormal3f,NULL)
	ZEND_FE(glnormal3fv,NULL)
	ZEND_FE(glnormal3i,NULL)
	ZEND_FE(glnormal3iv,NULL)
	ZEND_FE(glnormal3s,NULL)
	ZEND_FE(glnormal3sv,NULL)
	ZEND_FE(glnormalpointer,NULL)
	ZEND_FE(glortho,NULL)
	ZEND_FE(glpassthrough,NULL)
	ZEND_FE(glpixelmapfv,NULL)
	ZEND_FE(glpixelmapuiv,NULL)
	ZEND_FE(glpixelmapusv,NULL)
	ZEND_FE(glpixelstoref,NULL)
	ZEND_FE(glpixelstorei,NULL)
	ZEND_FE(glpixeltransferf,NULL)
	ZEND_FE(glpixeltransferi,NULL)
	ZEND_FE(glpixelzoom,NULL)
	ZEND_FE(glpointsize,NULL)
	ZEND_FE(glpolygonmode,NULL)
	ZEND_FE(glpolygonoffset,NULL)
	ZEND_FE(glpolygonstipple,NULL)
	ZEND_FE(glpopattrib,NULL)
	ZEND_FE(glpopclientattrib,NULL)
	ZEND_FE(glpopmatrix,NULL)
	ZEND_FE(glpopname,NULL)
	ZEND_FE(glprioritizetextures,NULL)
	ZEND_FE(glpushattrib,NULL)
	ZEND_FE(glpushclientattrib,NULL)
	ZEND_FE(glpushmatrix,NULL)
	ZEND_FE(glpushname,NULL)
	ZEND_FE(glrasterpos2d,NULL)
	ZEND_FE(glrasterpos2dv,NULL)
	ZEND_FE(glrasterpos2f,NULL)
	ZEND_FE(glrasterpos2fv,NULL)
	ZEND_FE(glrasterpos2i,NULL)
	ZEND_FE(glrasterpos2iv,NULL)
	ZEND_FE(glrasterpos2s,NULL)
	ZEND_FE(glrasterpos2sv,NULL)
	ZEND_FE(glrasterpos3d,NULL)
	ZEND_FE(glrasterpos3dv,NULL)
	ZEND_FE(glrasterpos3f,NULL)
	ZEND_FE(glrasterpos3fv,NULL)
	ZEND_FE(glrasterpos3i,NULL)
	ZEND_FE(glrasterpos3iv,NULL)
	ZEND_FE(glrasterpos3s,NULL)
	ZEND_FE(glrasterpos3sv,NULL)
	ZEND_FE(glrasterpos4d,NULL)
	ZEND_FE(glrasterpos4dv,NULL)
	ZEND_FE(glrasterpos4f,NULL)
	ZEND_FE(glrasterpos4fv,NULL)
	ZEND_FE(glrasterpos4i,NULL)
	ZEND_FE(glrasterpos4iv,NULL)
	ZEND_FE(glrasterpos4s,NULL)
	ZEND_FE(glrasterpos4sv,NULL)
	ZEND_FE(glreadbuffer,NULL)
	ZEND_FE(glreadpixels,NULL)
	ZEND_FE(glrectd,NULL)
	ZEND_FE(glrectdv,NULL)
	ZEND_FE(glrectf,NULL)
	ZEND_FE(glrectfv,NULL)
	ZEND_FE(glrecti,NULL)
	ZEND_FE(glrectiv,NULL)
	ZEND_FE(glrects,NULL)
	ZEND_FE(glrectsv,NULL)
	ZEND_FE(glrendermode,NULL)
	ZEND_FE(glrotated,NULL)
	ZEND_FE(glrotatef,NULL)
	ZEND_FE(glscaled,NULL)
	ZEND_FE(glscalef,NULL)
	ZEND_FE(glscissor,NULL)
	ZEND_FE(glselectbuffer,NULL)
	ZEND_FE(glshademodel,NULL)
	ZEND_FE(glstencilfunc,NULL)
	ZEND_FE(glstencilmask,NULL)
	ZEND_FE(glstencilop,NULL)
	ZEND_FE(gltexcoord1d,NULL)
	ZEND_FE(gltexcoord1dv,NULL)
	ZEND_FE(gltexcoord1f,NULL)
	ZEND_FE(gltexcoord1fv,NULL)
	ZEND_FE(gltexcoord1i,NULL)
	ZEND_FE(gltexcoord1iv,NULL)
	ZEND_FE(gltexcoord1s,NULL)
	ZEND_FE(gltexcoord1sv,NULL)
	ZEND_FE(gltexcoord2d,NULL)
	ZEND_FE(gltexcoord2dv,NULL)
	ZEND_FE(gltexcoord2f,NULL)
	ZEND_FE(gltexcoord2fv,NULL)
	ZEND_FE(gltexcoord2i,NULL)
	ZEND_FE(gltexcoord2iv,NULL)
	ZEND_FE(gltexcoord2s,NULL)
	ZEND_FE(gltexcoord2sv,NULL)
	ZEND_FE(gltexcoord3d,NULL)
	ZEND_FE(gltexcoord3dv,NULL)
	ZEND_FE(gltexcoord3f,NULL)
	ZEND_FE(gltexcoord3fv,NULL)
	ZEND_FE(gltexcoord3i,NULL)
	ZEND_FE(gltexcoord3iv,NULL)
	ZEND_FE(gltexcoord3s,NULL)
	ZEND_FE(gltexcoord3sv,NULL)
	ZEND_FE(gltexcoord4d,NULL)
	ZEND_FE(gltexcoord4dv,NULL)
	ZEND_FE(gltexcoord4f,NULL)
	ZEND_FE(gltexcoord4fv,NULL)
	ZEND_FE(gltexcoord4i,NULL)
	ZEND_FE(gltexcoord4iv,NULL)
	ZEND_FE(gltexcoord4s,NULL)
	ZEND_FE(gltexcoord4sv,NULL)
	ZEND_FE(gltexcoordpointer,NULL)
	ZEND_FE(gltexenvf,NULL)
	ZEND_FE(gltexenvfv,NULL)
	ZEND_FE(gltexenvi,NULL)
	ZEND_FE(gltexenviv,NULL)
	ZEND_FE(gltexgend,NULL)
	ZEND_FE(gltexgendv,NULL)
	ZEND_FE(gltexgenf,NULL)
	ZEND_FE(gltexgenfv,NULL)
	ZEND_FE(gltexgeni,NULL)
	ZEND_FE(gltexgeniv,NULL)
	ZEND_FE(glteximage1d,NULL)
	ZEND_FE(glteximage2d,NULL)
	ZEND_FE(gltexparameterf,NULL)
	ZEND_FE(gltexparameterfv,NULL)
	ZEND_FE(gltexparameteri,NULL)
	ZEND_FE(gltexparameteriv,NULL)
	ZEND_FE(gltexsubimage1d,NULL)
	ZEND_FE(gltexsubimage2d,NULL)
	ZEND_FE(gltranslated,NULL)
	ZEND_FE(gltranslatef,NULL)
	ZEND_FE(glvertex2d,NULL)
	ZEND_FE(glvertex2dv,NULL)
	ZEND_FE(glvertex2f,NULL)
	ZEND_FE(glvertex2fv,NULL)
	ZEND_FE(glvertex2i,NULL)
	ZEND_FE(glvertex2iv,NULL)
	ZEND_FE(glvertex2s,NULL)
	ZEND_FE(glvertex2sv,NULL)
	ZEND_FE(glvertex3d,NULL)
	ZEND_FE(glvertex3dv,NULL)
	ZEND_FE(glvertex3f,NULL)
	ZEND_FE(glvertex3fv,NULL)
	ZEND_FE(glvertex3i,NULL)
	ZEND_FE(glvertex3iv,NULL)
	ZEND_FE(glvertex3s,NULL)
	ZEND_FE(glvertex3sv,NULL)
	ZEND_FE(glvertex4d,NULL)
	ZEND_FE(glvertex4dv,NULL)
	ZEND_FE(glvertex4f,NULL)
	ZEND_FE(glvertex4fv,NULL)
	ZEND_FE(glvertex4i,NULL)
	ZEND_FE(glvertex4iv,NULL)
	ZEND_FE(glvertex4s,NULL)
	ZEND_FE(glvertex4sv,NULL)
	ZEND_FE(glvertexpointer,NULL)
	ZEND_FE(glviewport,NULL)

	/* GLU Functions */
	ZEND_FE(gluerrorstring,NULL)
	ZEND_FE(gluerrorunicodestringext,NULL)
	ZEND_FE(glugetstring,NULL)
	ZEND_FE(gluortho2d,NULL)
	ZEND_FE(gluperspective,NULL)
	ZEND_FE(glupickatrix,NULL)
	ZEND_FE(gluproject,NULL)
	ZEND_FE(glunuproject,NULL)
	//ZEND_FE(gluscaleimage,NULL)
	ZEND_FE(glulookat,NULL)
	ZEND_FE(glubuild1dmipmaps,NULL)
	ZEND_FE(glubuild2dmipmaps,NULL)
	ZEND_FE(glunewquadric,NULL)
	ZEND_FE(gludeletequadric,NULL)
	ZEND_FE(gluquadricnormals,NULL)
	ZEND_FE(gluquadrictexture,NULL)
	ZEND_FE(gluquadricorientation,NULL)
	ZEND_FE(gluquadricdrawstyle,NULL)
	ZEND_FE(glucylinder,NULL)
	ZEND_FE(gludisk,NULL)
	ZEND_FE(glupartialdisk,NULL)
	ZEND_FE(glusphere,NULL)
	ZEND_FE(glunewtess,NULL)
	ZEND_FE(gludeletetess,NULL)
	ZEND_FE(glutessbeginpolygon,NULL)
	ZEND_FE(glutessbegincontour,NULL)
	ZEND_FE(glutessvertex,NULL)
	ZEND_FE(glutessendcontour,NULL)
	ZEND_FE(glutessendpolygon,NULL)
	ZEND_FE(glutessproperty,NULL)
	ZEND_FE(glutessnormal,NULL)
	ZEND_FE(glugettessproperty,NULL)
	ZEND_FE(glunewnurbsrenderer,NULL)
	ZEND_FE(gludeletenurbsrenderer,NULL)
	ZEND_FE(glubeginsurface,NULL)
	ZEND_FE(glubegincurve,NULL)
	ZEND_FE(gluendcurve,NULL)
	ZEND_FE(gluendsurface,NULL)
	ZEND_FE(glubegintrim,NULL)
	ZEND_FE(gluendtrim,NULL)
	ZEND_FE(glupwlcurve,NULL)
	ZEND_FE(glunurbscurve,NULL)
	ZEND_FE(glunurbssurface,NULL)
	ZEND_FE(gluloadsamplingmatrices,NULL)
	ZEND_FE(gluNurbsProperty,NULL)
	ZEND_FE(glugetnurbsproperty,NULL)
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

PHP_MINFO_FUNCTION(opengl)
{
        php_info_print_table_start();
        php_info_print_table_header(2, "OpenGL support", "enabled");
        php_info_print_table_end();
}

/* {{{ void glaccum(long op, double value) */
PHP_FUNCTION(glaccum)
{
	long op;
	double value;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &op, &value) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glAccum((int)op,(float)value);
}
/* }}} */

/* {{{ void glalphafunc(long func, double ref) */
PHP_FUNCTION(glalphafunc)
{
	long func;
	double ref;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &func, &ref) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glAlphaFunc((int)func,(float)ref);
}
/* }}} */

/* {{{ bool glaretexturesresident(long n, array textures, array residences) */
PHP_FUNCTION(glaretexturesresident)
{
	zval *n, *textures, *residences;
	GLboolean return_int;
	GLuint *v_textures;
	GLboolean *v_residences;
	THREE_PARAM(n, textures, residences);
	convert_to_long(n);
	convert_to_array(textures);
	convert_to_array(residences);
	v_textures = php_array_to_uint_array(textures);
	v_residences = (GLboolean *)emalloc(sizeof(GLboolean) * Z_LVAL_P(n));
	return_int = glAreTexturesResident((int)Z_LVAL_P(n),v_textures,v_residences);
	boolean_array_to_php_array(v_residences,Z_LVAL_P(n),residences);
	efree(v_residences);
	RETURN_BOOL(return_int);
}
/* }}} */

/* {{{ void glarrayelement(long i) */
PHP_FUNCTION(glarrayelement)
{
	long i;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &i) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glArrayElement((int)i);
}
/* }}} */

/* {{{ void glbegin(long mode) */
PHP_FUNCTION(glbegin)
{
	long mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glBegin((int)mode);
}
/* }}} */

/* {{{ void glbindtexture(long target, long texture) */
PHP_FUNCTION(glbindtexture)
{
	long target, texture;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &target, &texture) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glBindTexture((int)target,(unsigned int)texture);
}
/* }}} */

/* {{{ void glbitmap(long width, long height, double xorig, double yorig, double xmove, double ymove, string bitmap) */
PHP_FUNCTION(glbitmap)
{
	zval *width, *height, *xorig, *yorig, *xmove, *ymove, *bitmap;
	SEVEN_PARAM(width, height, xorig, yorig, xmove, ymove, bitmap);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_double(xorig);
	convert_to_double(yorig);
	convert_to_double(xmove);
	convert_to_double(ymove);
	convert_to_string(bitmap);
	glBitmap((int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(float)Z_DVAL_P(xorig),(float)Z_DVAL_P(yorig),(float)Z_DVAL_P(xmove),(float)Z_DVAL_P(ymove),Z_STRVAL_P(bitmap));
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

/* {{{ void glcalllist(long list) */
PHP_FUNCTION(glcalllist)
{
	long list;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &list) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glCallList((unsigned int)list);
}
/* }}} */

/* {{{ null glcalllists(long n, long type, array lists) */
PHP_FUNCTION(glcalllists)
{
	zval *n, *type, *lists;
	GLvoid *v_lists;
	THREE_PARAM(n, type, lists);
	convert_to_long(n);
	convert_to_long(type);
	convert_to_array(lists);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_lists = php_array_to_short_array(lists);
		break;
	case GL_UNSIGNED_SHORT:
		v_lists = php_array_to_ushort_array(lists);
		break;
	case GL_INT:
		v_lists = php_array_to_int_array(lists);
		break;
	case GL_UNSIGNED_INT:
		v_lists = php_array_to_uint_array(lists);
		break;
	case GL_FLOAT:
		v_lists = php_array_to_float_array(lists);
		break;
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
	case GL_2_BYTES:
	case GL_3_BYTES:
	case GL_4_BYTES:
		zend_error(E_ERROR,"These types are not supported GL_BYTE, GL_UNSIGNED_BYTE, GL_2_BYTES, GL_3_BYTES or GL_4_BYTES");
		RETURN_NULL();
		break;
	}
	glCallLists((int)Z_LVAL_P(n),(int)Z_LVAL_P(type),v_lists);
}
/* }}} */

/* {{{ void glclear(long mask) */
PHP_FUNCTION(glclear)
{
	long mask;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glClear(mask);
}
/* }}} */

/* {{{ void glclearaccum(double red, double green, double blue, double alpha) */
PHP_FUNCTION(glclearaccum)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glClearAccum((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue),(float)Z_DVAL_P(alpha));
}
/* }}} */

/* {{{ void glclearcolor(double red, double green, double blue, double alpha) */
PHP_FUNCTION(glclearcolor)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glClearColor((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue),(float)Z_DVAL_P(alpha));
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

/* {{{ void glclearindex(double c) */
PHP_FUNCTION(glclearindex)
{
	double c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glClearIndex((float)c);
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

/* {{{ void glclipplane(long plane, array equation) */
PHP_FUNCTION(glclipplane)
{
	long plane;
	zval *equation;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &plane, &equation) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	GLdouble *v_equation;
	convert_to_array(equation);
	v_equation = php_array_to_double_array(equation);
	glClipPlane((int)plane,v_equation);
}
/* }}} */

/* {{{ void glcolor3b(long red, long green, long blue) */
PHP_FUNCTION(glcolor3b)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3b((signed char)Z_LVAL_P(red),(signed char)Z_LVAL_P(green),(signed char)Z_LVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3bv(array v) */
PHP_FUNCTION(glcolor3bv)
{
	zval *v;
	GLbyte *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	convert_to_array(v);
	v_v = php_array_to_schar_array(v);
	glColor3bv(v_v);
}
/* }}} */

/* {{{ void glcolor3d(double red, double green, double blue) */
PHP_FUNCTION(glcolor3d)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	glColor3d(Z_DVAL_P(red),Z_DVAL_P(green),Z_DVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3dv(array v) */
PHP_FUNCTION(glcolor3dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	convert_to_array(v);
	v_v = php_array_to_double_array(v);
	glColor3dv(v_v);
}
/* }}} */

/* {{{ void glcolor3f(double red, double green, double blue) */
PHP_FUNCTION(glcolor3f)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	glColor3f((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3fv(array v) */
PHP_FUNCTION(glcolor3fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	convert_to_array(v);
	v_v = php_array_to_float_array(v);
	glColor3fv(v_v);
}
/* }}} */

/* {{{ void glcolor3i(long red, long green, long blue) */
PHP_FUNCTION(glcolor3i)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3i((int)Z_LVAL_P(red),(int)Z_LVAL_P(green),(int)Z_LVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3iv(array v) */
PHP_FUNCTION(glcolor3iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glColor3iv(v_v);
}
/* }}} */

/* {{{ void glcolor3s(long red, long green, long blue) */
PHP_FUNCTION(glcolor3s)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3s((short)Z_LVAL_P(red),(short)Z_LVAL_P(green),(short)Z_LVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3sv(array v) */
PHP_FUNCTION(glcolor3sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glColor3sv(v_v);
}
/* }}} */

/* {{{ void glcolor3ub(long red, long green, long blue) */
PHP_FUNCTION(glcolor3ub)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3ub((unsigned char)Z_LVAL_P(red),(unsigned char)Z_LVAL_P(green),(unsigned char)Z_LVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3ubv(array v) */
PHP_FUNCTION(glcolor3ubv)
{
	zval *v;
	GLubyte *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_uchar_array(v);
	glColor3ubv(v_v);
}
/* }}} */

/* {{{ void glcolor3ui(long red, long green, long blue) */
PHP_FUNCTION(glcolor3ui)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3ui((unsigned int)Z_LVAL_P(red),(unsigned int)Z_LVAL_P(green),(unsigned int)Z_LVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3uiv(array v) */
PHP_FUNCTION(glcolor3uiv)
{
	zval *v;
	GLuint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_uint_array(v);
	glColor3uiv(v_v);
}
/* }}} */

/* {{{ void glcolor3us(long red, long green, long blue) */
PHP_FUNCTION(glcolor3us)
{
	zval *red, *green, *blue;
	THREE_PARAM(red, green, blue);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	glColor3us((unsigned short)Z_LVAL_P(red),(unsigned short)Z_LVAL_P(green),(unsigned short)Z_LVAL_P(blue));
}
/* }}} */

/* {{{ void glcolor3usv(array v) */
PHP_FUNCTION(glcolor3usv)
{
	zval *v;
	GLushort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_ushort_array(v);
	glColor3usv(v_v);
}
/* }}} */

/* {{{ void glcolor4b(long red, long green, long blue, long alpha) */
PHP_FUNCTION(glcolor4b)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4b((signed char)Z_LVAL_P(red),(signed char)Z_LVAL_P(green),(signed char)Z_LVAL_P(blue),(signed char)Z_LVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4bv(array v) */
PHP_FUNCTION(glcolor4bv)
{
	zval *v;
	GLbyte *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_schar_array(v);
	glColor4bv(v_v);
}
/* }}} */

/* {{{ void glcolor4d(double red, double green, double blue, double alpha) */
PHP_FUNCTION(glcolor4d)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glColor4d(Z_DVAL_P(red),Z_DVAL_P(green),Z_DVAL_P(blue),Z_DVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4dv(array v) */
PHP_FUNCTION(glcolor4dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glColor4dv(v_v);
}
/* }}} */

/* {{{ void glcolor4f(double red, double green, double blue, double alpha) */
PHP_FUNCTION(glcolor4f)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	convert_to_double(alpha);
	glColor4f((float)Z_DVAL_P(red),(float)Z_DVAL_P(green),(float)Z_DVAL_P(blue),(float)Z_DVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4fv(array v) */
PHP_FUNCTION(glcolor4fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glColor4fv(v_v);
}
/* }}} */

/* {{{ void glcolor4i(long red, long green, long blue, long alpha) */
PHP_FUNCTION(glcolor4i)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4i((int)Z_LVAL_P(red),(int)Z_LVAL_P(green),(int)Z_LVAL_P(blue),(int)Z_LVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4iv(array v) */
PHP_FUNCTION(glcolor4iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glColor4iv(v_v);
}
/* }}} */

/* {{{ void glcolor4s(long red, long green, long blue, long alpha) */
PHP_FUNCTION(glcolor4s)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4s((short)Z_LVAL_P(red),(short)Z_LVAL_P(green),(short)Z_LVAL_P(blue),(short)Z_LVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4sv(array v) */
PHP_FUNCTION(glcolor4sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glColor4sv(v_v);
}
/* }}} */

/* {{{ void glcolor4ub(long red, long green, long blue, long alpha) */
PHP_FUNCTION(glcolor4ub)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4ub((unsigned char)Z_LVAL_P(red),(unsigned char)Z_LVAL_P(green),(unsigned char)Z_LVAL_P(blue),(unsigned char)Z_LVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4ubv(array v) */
PHP_FUNCTION(glcolor4ubv)
{
	zval *v;
	GLubyte *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_uchar_array(v);
	glColor4ubv(v_v);
}
/* }}} */

/* {{{ void glcolor4ui(long red, long green, long blue, long alpha) */
PHP_FUNCTION(glcolor4ui)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4ui((unsigned int)Z_LVAL_P(red),(unsigned int)Z_LVAL_P(green),(unsigned int)Z_LVAL_P(blue),(unsigned int)Z_LVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4uiv(array v) */
PHP_FUNCTION(glcolor4uiv)
{
	zval *v;
	GLuint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_uint_array(v);
	glColor4uiv(v_v);
}
/* }}} */

/* {{{ void glcolor4us(long red, long green, long blue, long alpha) */
PHP_FUNCTION(glcolor4us)
{
	zval *red, *green, *blue, *alpha;
	FOUR_PARAM(red, green, blue, alpha);
	convert_to_long(red);
	convert_to_long(green);
	convert_to_long(blue);
	convert_to_long(alpha);
	glColor4us((unsigned short)Z_LVAL_P(red),(unsigned short)Z_LVAL_P(green),(unsigned short)Z_LVAL_P(blue),(unsigned short)Z_LVAL_P(alpha));
}
/* }}} */

/* {{{ void glcolor4usv(array v) */
PHP_FUNCTION(glcolor4usv)
{
	zval *v;
	GLushort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_ushort_array(v);
	glColor4usv(v_v);
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

/* {{{ void glcolormaterial(long face, long mode) */
PHP_FUNCTION(glcolormaterial)
{
	long face, mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &face, &mode) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glColorMaterial((int)face,(int)mode);
}
/* }}} */

/* {{{ null glcolorpointer(long size, long type, long stride, array pointer) */
PHP_FUNCTION(glcolorpointer)
{
	zval *size, *type, *stride, *pointer;
	GLvoid *v_pointer;
	FOUR_PARAM(size, type, stride, pointer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);

	switch(Z_LVAL_P(type))
	{
	case GL_BYTE:
		v_pointer = php_array_to_byte_array(pointer);
		break;
	case GL_UNSIGNED_BYTE:
		v_pointer = php_array_to_ubyte_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_UNSIGNED_SHORT:
		v_pointer = php_array_to_ushort_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_UNSIGNED_INT:
		v_pointer = php_array_to_uint_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	default:
		zend_error(E_ERROR,"Wrong data type");
		RETURN_NULL();
		break;
	}
	glColorPointer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
}
/* }}} */

/* {{{ void glcopypixels(long x, long y, long width, long height, long type) */
PHP_FUNCTION(glcopypixels)
{
	zval *x, *y, *width, *height, *type;
	FIVE_PARAM(x, y, width, height, type);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(type);
	glCopyPixels((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(type));
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
	zval *target, *level, *xoffset, *x, *y, *width;
	SIX_PARAM(target, level, xoffset, x, y, width);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	glCopyTexSubImage1D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(width));
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

/* {{{ void gldeletelists(long list, long range) */
PHP_FUNCTION(gldeletelists)
{
	long list, range;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &list, &range) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glDeleteLists((unsigned int)list,(int)range);
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

/* {{{ void gldisableclientstate(long array) */
PHP_FUNCTION(gldisableclientstate)
{
	long array;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &array) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glDisableClientState((int)array);
}
/* }}} */

/* {{{ void gldrawarrays(long mode, long first, long count) */
PHP_FUNCTION(gldrawarrays)
{
	zval *mode, *first, *count;
	THREE_PARAM(mode, first, count);
	convert_to_long(mode);
	convert_to_long(first);
	convert_to_long(count);
	glDrawArrays((int)Z_LVAL_P(mode),(int)Z_LVAL_P(first),(int)Z_LVAL_P(count));
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

/* {{{ void gldrawelements(long mode, long count, long type, array indices) */
PHP_FUNCTION(gldrawelements)
{
	zval *mode, *count, *type, *indices;
	GLvoid *v_indices;
	FOUR_PARAM(mode, count, type, indices);
	convert_to_long(mode);
	convert_to_long(count);
	convert_to_long(type);
	convert_to_array(indices);
	switch(Z_LVAL_P(type))
	{
	case GL_UNSIGNED_BYTE:
		v_indices = php_array_to_ubyte_array(indices);
		break;
	case GL_UNSIGNED_SHORT:
		v_indices = php_array_to_ushort_array(indices);
		break;
	case GL_UNSIGNED_INT:
		v_indices = php_array_to_uint_array(indices);
		break;
	}
	glDrawElements((int)Z_LVAL_P(mode),(int)Z_LVAL_P(count),(int)Z_LVAL_P(type),v_indices);
}
/* }}} */

/* {{{ void gldrawpixels(long width, long height, long format, long type, array pixels) */
PHP_FUNCTION(gldrawpixels)
{
	zval *width, *height, *format, *type, *pixels;
	GLvoid *v_pixels;
	FIVE_PARAM(width, height, format, type, pixels);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glDrawPixels((int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
}
/* }}} */

/* {{{ void gledgeflag(boolean flag) */
PHP_FUNCTION(gledgeflag)
{
	zend_bool flag;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &flag) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEdgeFlag(flag);
}
/* }}} */

/* {{{ void gledgeflagpointer(long stride, array pointer) */
PHP_FUNCTION(gledgeflagpointer)
{
	long stride;
	zval *pointer;
	GLvoid *v_pointer;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &stride, &pointer) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	v_pointer = php_array_to_uchar_array(pointer);
	glEdgeFlagPointer((int)(stride),v_pointer);
}
/* }}} */

/* {{{ void gledgeflagv(array flag) */
PHP_FUNCTION(gledgeflagv)
{
	zval *flag;
	GLboolean *v_flag;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &flag) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_flag = php_array_to_uchar_array(flag);
	glEdgeFlagv(v_flag);
}
/* }}} */

/* {{{ void glenable(long cap) */
PHP_FUNCTION(glenable)
{
	long cap;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &cap) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEnable((int)cap);
}
/* }}} */

/* {{{ void glenableclientstate(long array) */
PHP_FUNCTION(glenableclientstate)
{
	long array;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &array) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEnableClientState((int)array);
}
/* }}} */

/* {{{ void glend() */
PHP_FUNCTION(glend)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEnd();
}
/* }}} */

/* {{{ void glendlist() */
PHP_FUNCTION(glendlist)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEndList();
}
/* }}} */

/* {{{ void glevalcoord1d(double u) */
PHP_FUNCTION(glevalcoord1d)
{
	double u;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &u) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEvalCoord1d(u);
}
/* }}} */

/* {{{ void glevalcoord1dv(array u) */
PHP_FUNCTION(glevalcoord1dv)
{
	zval *u;
	GLdouble *v_u;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &u) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_u = php_array_to_double_array(u);
	glEvalCoord1dv(v_u);
}
/* }}} */

/* {{{ void glevalcoord1f(double u) */
PHP_FUNCTION(glevalcoord1f)
{
	double u;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &u) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEvalCoord1f((float)u);
}
/* }}} */

/* {{{ void glevalcoord1fv(array u) */
PHP_FUNCTION(glevalcoord1fv)
{
	zval *u;
	GLfloat *v_u;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &u) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_u = php_array_to_float_array(u);
	glEvalCoord1fv(v_u);
}
/* }}} */

/* {{{ void glevalcoord2d(double u, double v) */
PHP_FUNCTION(glevalcoord2d)
{
	double u, v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &u, &v) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glEvalCoord2d(u,v);
}
/* }}} */

/* {{{ void glevalcoord2dv(array u) */
PHP_FUNCTION(glevalcoord2dv)
{
	zval *u;
	GLdouble *v_u;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &u) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_u = php_array_to_double_array(u);
	glEvalCoord2dv(v_u);
}
/* }}} */

/* {{{ void glevalcoord2f(double u, double v) */
PHP_FUNCTION(glevalcoord2f)
{
	double u, v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &u, &v) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glEvalCoord2f((float)u,(float)v);
}
/* }}} */

/* {{{ void glevalcoord2fv(array u) */
PHP_FUNCTION(glevalcoord2fv)
{
	zval *u;
	GLfloat *v_u;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &u) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_u = php_array_to_float_array(u);
	glEvalCoord2fv(v_u);
}
/* }}} */

/* {{{ void glevalmesh1(long mode, long i1, long i2) */
PHP_FUNCTION(glevalmesh1)
{
	zval *mode, *i1, *i2;
	THREE_PARAM(mode, i1, i2);
	convert_to_long(mode);
	convert_to_long(i1);
	convert_to_long(i2);
	glEvalMesh1((int)Z_LVAL_P(mode),(int)Z_LVAL_P(i1),(int)Z_LVAL_P(i2));
}
/* }}} */

/* {{{ void glevalmesh2(long mode, long i1, long i2, long j1, long j2) */
PHP_FUNCTION(glevalmesh2)
{
	zval *mode, *i1, *i2, *j1, *j2;
	FIVE_PARAM(mode, i1, i2, j1, j2);
	convert_to_long(mode);
	convert_to_long(i1);
	convert_to_long(i2);
	convert_to_long(j1);
	convert_to_long(j2);
	glEvalMesh2((int)Z_LVAL_P(mode),(int)Z_LVAL_P(i1),(int)Z_LVAL_P(i2),(int)Z_LVAL_P(j1),(int)Z_LVAL_P(j2));
}
/* }}} */

/* {{{ void glevalpoint1(long i) */
PHP_FUNCTION(glevalpoint1)
{
	long i;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &i) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glEvalPoint1((int)i);
}
/* }}} */

/* {{{ void glevalpoint2(long i, long j) */
PHP_FUNCTION(glevalpoint2)
{
	long i, j;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &i, &j) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glEvalPoint2((int)i,(int)j);
}
/* }}} */

/* {{{ void glfeedbackbuffer(long size, long type, array buffer) */
PHP_FUNCTION(glfeedbackbuffer)
{
	zval *size, *type, *buffer;
	GLfloat *v_buffer;
	THREE_PARAM(size, type, buffer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_array(buffer);
	v_buffer = (GLfloat *)emalloc(sizeof(GLfloat) * Z_LVAL_P(size));
	glFeedbackBuffer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),v_buffer);
	float_array_to_php_array(v_buffer,Z_LVAL_P(size),buffer);
	efree(v_buffer);
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

/* {{{ void glfogf(long pname, double param) */
PHP_FUNCTION(glfogf)
{
	long pname;
	double param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glFogf((int)(pname),(float)(param));
}
/* }}} */

/* {{{ void glfogfv(long pname, array params) */
PHP_FUNCTION(glfogfv)
{
	long pname;
	zval *params;
	GLfloat *v_params;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &pname, &params) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	v_params = php_array_to_float_array(params);
	glFogfv((int)(pname),v_params);
}
/* }}} */

/* {{{ void glfogi(long pname, long param) */
PHP_FUNCTION(glfogi)
{
	long pname, param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glFogi((int)(pname),(int)(param));
}
/* }}} */

/* {{{ void glfogiv(long pname, array params) */
PHP_FUNCTION(glfogiv)
{
	long pname;
	zval *params;
	GLint *v_params;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &pname, &params) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	v_params = php_array_to_int_array(params);
	glFogiv((int)(pname),v_params);
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

/* {{{ void glfrustum(double left, double right, double bottom, double top, double zNear, double zFar) */
PHP_FUNCTION(glfrustum)
{
	zval *left, *right, *bottom, *top, *zNear, *zFar;
	SIX_PARAM(left, right, bottom, top, zNear, zFar);
	convert_to_double(left);
	convert_to_double(right);
	convert_to_double(bottom);
	convert_to_double(top);
	convert_to_double(zNear);
	convert_to_double(zFar);
	glFrustum(Z_DVAL_P(left),Z_DVAL_P(right),Z_DVAL_P(bottom),Z_DVAL_P(top),Z_DVAL_P(zNear),Z_DVAL_P(zFar));
}
/* }}} */

/* {{{ long glgenlists(long range) */
PHP_FUNCTION(glgenlists)
{
	long range;
	unsigned int return_unsigned_int;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &range) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	return_unsigned_int = glGenLists((int)range);
	RETURN_LONG(return_unsigned_int);
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

/* {{{ void glgetclipplane(long plane, array equation) */
PHP_FUNCTION(glgetclipplane)
{
	long plane;
	zval *equation;
	GLdouble v_equation[4];
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &plane, &equation ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glGetClipPlane((int)(plane),v_equation);
	double_array_to_php_array(v_equation,4,equation);
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
	int return_int;
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	return_int = glGetError();
	RETURN_LONG(return_int);
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

/* {{{ void glgetlightfv(long light, long pname, array params, long num) */
PHP_FUNCTION(glgetlightfv)
{
	zval *light, *pname, *params, *num;
	GLfloat *v_params;
	FOUR_PARAM(light, pname, params, num);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLfloat *)emalloc(Z_LVAL_P(num) * sizeof(GLfloat));
	glGetLightfv(Z_LVAL_P(light),Z_LVAL_P(pname),v_params);
	float_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetlightiv(long light, long pname, array params, long num) */
PHP_FUNCTION(glgetlightiv)
{
	zval *light, *pname, *params, *num;
	GLint *v_params;
	FOUR_PARAM(light, pname, params, num);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLint *)emalloc(Z_LVAL_P(num) * sizeof(GLint));
	glGetLightiv((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetmapdv(long target, long query, array v, long num) */
PHP_FUNCTION(glgetmapdv)
{
	zval *target, *query, *v, *num;
	GLdouble *v_v;
	FOUR_PARAM(target, query, v, num);
	convert_to_long(target);
	convert_to_long(query);
	convert_to_array(v);
	convert_to_long(num);
	v_v = (GLdouble *)emalloc(Z_LVAL_P(num) * sizeof(GLdouble));
	glGetMapdv((int)Z_LVAL_P(target),(int)Z_LVAL_P(query),v_v);
	double_array_to_php_array(v_v,Z_LVAL_P(num),v);
	efree(v_v);
}
/* }}} */

/* {{{ void glgetmapfv(long target, long query, array v, long num) */
PHP_FUNCTION(glgetmapfv)
{
	zval *target, *query, *v, *num;
	GLfloat *v_v;
	FOUR_PARAM(target, query, v, num);
	convert_to_long(target);
	convert_to_long(query);
	convert_to_array(v);
	convert_to_long(num);
	v_v = (GLfloat *)emalloc(Z_LVAL_P(num) * sizeof(GLfloat));
	glGetMapfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(query),v_v);
	float_array_to_php_array(v_v,Z_LVAL_P(num),v);
	efree(v_v);
}
/* }}} */

/* {{{ void glgetmapiv(long target, long query, array v, long num) */
PHP_FUNCTION(glgetmapiv)
{
	zval *target, *query, *v, *num;
	GLint *v_v;
	FOUR_PARAM(target, query, v, num);
	convert_to_long(target);
	convert_to_long(query);
	convert_to_array(v);
	convert_to_long(num);
	v_v = (GLint *)emalloc(Z_LVAL_P(num) * sizeof(GLint));
	glGetMapiv((int)Z_LVAL_P(target),(int)Z_LVAL_P(query),v_v);
	int_array_to_php_array(v_v,Z_LVAL_P(num),v);
	efree(v_v);
}
/* }}} */

/* {{{ void glgetmaterialfv(long face, long pname, array params, long num) */
PHP_FUNCTION(glgetmaterialfv)
{
	zval *face, *pname, *params, *num;
	GLfloat *v_params;
	FOUR_PARAM(face, pname, params, num);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLfloat *)emalloc(Z_LVAL_P(num) * sizeof(GLfloat));
	glGetMaterialfv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
	float_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetmaterialiv(long face, long pname, array params, long num) */
PHP_FUNCTION(glgetmaterialiv)
{
	zval *face, *pname, *params, *num;
	GLint *v_params;
	FOUR_PARAM(face, pname, params, num);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLint *)emalloc(Z_LVAL_P(num) * sizeof(GLint));
	glGetMaterialiv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetpixelmapfv(long map, array values) */
PHP_FUNCTION(glgetpixelmapfv)
{
	long map;
	zval *values;
	GLfloat *v_values;
	int size;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &map, &values) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glGetIntegerv((map)+0x040,&size);
	v_values = (GLfloat *)emalloc(size * sizeof(GLfloat));
	glGetPixelMapfv((map),v_values);
	float_array_to_php_array(v_values,size,values);
	efree(v_values);
}
/* }}} */


/* {{{ void glgetpixelmapuiv(long map, array values) */
PHP_FUNCTION(glgetpixelmapuiv)
{
	long map;
	zval *values;
	GLuint *v_values;
	int size;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &map, &values) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glGetIntegerv((map)+0x040,&size);
	v_values = (GLuint *)emalloc(size * sizeof(GLuint));
	glGetPixelMapuiv((map),v_values);
	uint_array_to_php_array(v_values,size,values);
	efree(v_values);
}
/* }}} */

/* {{{ void glgetpixelmapusv(long map, array values) */
PHP_FUNCTION(glgetpixelmapusv)
{
	long map;
	zval *values;
	GLushort *v_values;
	int size;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &map, &values) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glGetIntegerv((map)+0x040,&size);
	v_values = (GLushort *)emalloc(size * sizeof(GLushort));
	glGetPixelMapusv((map),v_values);
	ushort_array_to_php_array(v_values,size,values);
	efree(v_values);
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

/* {{{ void glgetpolygonstipple(string mask) */
PHP_FUNCTION(glgetpolygonstipple)
{
	zval *mask;
	GLubyte *v_mask;
	v_mask = (GLubyte *)emalloc(32 * 32);
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glGetPolygonStipple(v_mask);
	ZVAL_STRING(mask,v_mask);
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

/* {{{ void glgettexenvfv(long target, long pname, array params, long num) */
PHP_FUNCTION(glgettexenvfv)
{
	zval *target, *pname, *params, *num;
	GLfloat *v_params;
	FOUR_PARAM(target, pname, params, num);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLfloat*)emalloc(sizeof(GLfloat) * Z_LVAL_P(num));
	glGetTexEnvfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	float_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgettexenviv(long target, long pname, array params, long num) */
PHP_FUNCTION(glgettexenviv)
{
	zval *target, *pname, *params, *num;
	GLint *v_params;
	FOUR_PARAM(target, pname, params, num);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLint*)emalloc(sizeof(GLint) * Z_LVAL_P(num));
	glGetTexEnviv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgettexgendv(long coord, long pname, array params, long num) */
PHP_FUNCTION(glgettexgendv)
{
	zval *coord, *pname, *params, *num;
	GLdouble *v_params;
	FOUR_PARAM(coord, pname, params, num);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLdouble*)emalloc(sizeof(GLdouble) * Z_LVAL_P(num));
	glGetTexGendv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	double_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgettexgenfv(long coord, long pname, array params, long num) */
PHP_FUNCTION(glgettexgenfv)
{
	zval *coord, *pname, *params, *num;
	GLfloat *v_params;
	FOUR_PARAM(coord, pname, params, num);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLfloat*)emalloc(sizeof(GLfloat) * Z_LVAL_P(num));
	glGetTexGenfv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	double_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgettexgeniv(long coord, long pname, array params, long num) */
PHP_FUNCTION(glgettexgeniv)
{
	zval *coord, *pname, *params, *num;
	GLint *v_params;
	FOUR_PARAM(coord, pname, params, num);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_long(num);
	convert_to_array(params);
	v_params = (GLint*)emalloc(sizeof(GLint) * Z_LVAL_P(num));
	glGetTexGeniv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
	int_array_to_php_array(v_params,Z_LVAL_P(num),params);
	efree(v_params);
}
/* }}} */

/* {{{ void glgetteximage(long target, long level, long format, long type, array pixels) */
PHP_FUNCTION(glgetteximage)
{
	zval *target, *level, *format, *type, *pixels;
	GLvoid *v_pixels;
	FIVE_PARAM(target, level, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glGetTexImage((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
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

/* {{{ void glindexmask(long mask) */
PHP_FUNCTION(glindexmask)
{
	long mask;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glIndexMask((unsigned int)mask);
}
/* }}} */

/* {{{ void glindexpointer(long type, long stride, array pointer) */
PHP_FUNCTION(glindexpointer)
{
	zval *type, *stride, *pointer;
	GLvoid *v_pointer;
	THREE_PARAM(type, stride, pointer);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glIndexPointer((int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
}
/* }}} */

/* {{{ void glindexd(double c) */
PHP_FUNCTION(glindexd)
{
	double c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glIndexd(c);
}
/* }}} */

/* {{{ void glindexdv(array c) */
PHP_FUNCTION(glindexdv)
{
	zval *c;
	GLdouble *v_c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_c = php_array_to_double_array(c);
	glIndexdv(v_c);
}
/* }}} */

/* {{{ void glindexf(double c) */
PHP_FUNCTION(glindexf)
{
	double c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glIndexf((float)c);
}
/* }}} */

/* {{{ void glindexfv(array c) */
PHP_FUNCTION(glindexfv)
{
	zval *c;
	GLfloat *v_c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_c = php_array_to_float_array(c);
	glIndexfv(v_c);
}
/* }}} */

/* {{{ void glindexi(long c) */
PHP_FUNCTION(glindexi)
{
	long c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glIndexi((int)c);
}
/* }}} */

/* {{{ void glindexiv(array c) */
PHP_FUNCTION(glindexiv)
{
	zval *c;
	GLint *v_c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_c = php_array_to_int_array(c);
	glIndexiv(v_c);
}
/* }}} */

/* {{{ void glindexs(long c) */
PHP_FUNCTION(glindexs)
{
	long c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glIndexs((short)c);
}
/* }}} */

/* {{{ void glindexsv(array c) */
PHP_FUNCTION(glindexsv)
{
	zval *c;
	GLshort *v_c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_c = php_array_to_short_array(c);
	glIndexsv(v_c);
}
/* }}} */

/* {{{ void glindexub(long c) */
PHP_FUNCTION(glindexub)
{
	long c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glIndexub((unsigned char)c);
}
/* }}} */

/* {{{ void glindexubv(array c) */
PHP_FUNCTION(glindexubv)
{
	zval *c;
	GLubyte *v_c;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &c) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_c = php_array_to_uchar_array(c);
	glIndexubv(v_c);
}
/* }}} */

/* {{{ void glinitnames() */
PHP_FUNCTION(glinitnames)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glInitNames();
}
/* }}} */

/* {{{ void glinterleavedarrays(long format, long stride, array pointer) */
PHP_FUNCTION(glinterleavedarrays)
{
	/*TODO: Do this function */
	/*zval *format, *stride, *pointer;
	GLvoid *v_pointer;
	THREE_PARAM(format, stride, pointer);
	convert_to_long(format);
	convert_to_long(stride);
	convert_to_array(pointer);
	v_pointer = php_array_to_long_array(pointer);
	glInterleavedArrays((int)Z_LVAL_P(format),(int)Z_LVAL_P(stride),v_pointer);*/
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

/* {{{ bool glislist(long list) */
PHP_FUNCTION(glislist)
{
	long list;
	int return_int;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &list) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	return_int = glIsList((unsigned int)list);
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

/* {{{ void gllightmodelf(long pname, double param) */
PHP_FUNCTION(gllightmodelf)
{
	long pname;
	double param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glLightModelf((int)(pname),(float)(param));
}
/* }}} */

/* {{{ void gllightmodelfv(long pname, array params) */
PHP_FUNCTION(gllightmodelfv)
{
	long pname;
	zval *params;
	GLfloat *v_params;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &pname, &params) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	v_params = php_array_to_float_array(params);
	glLightModelfv((int)(pname),v_params);
}
/* }}} */

/* {{{ void gllightmodeli(long pname, long param) */
PHP_FUNCTION(gllightmodeli)
{
	long pname, param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glLightModeli((int)(pname),(int)(param));
}
/* }}} */

/* {{{ void gllightmodeliv(long pname, array params) */
PHP_FUNCTION(gllightmodeliv)
{
	long pname;
	zval *params;
	GLint *v_params;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &pname, &params) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	v_params = php_array_to_int_array(params);
	glLightModeliv((int)(pname),v_params);
}
/* }}} */

/* {{{ void gllightf(long light, long pname, double param) */
PHP_FUNCTION(gllightf)
{
	zval *light, *pname, *param;
	THREE_PARAM(light, pname, param);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_double(param);
	glLightf((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
/* }}} */

/* {{{ void gllightfv(long light, long pname, array params) */
PHP_FUNCTION(gllightfv)
{
	zval *light, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(light, pname, params);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glLightfv((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gllighti(long light, long pname, long param) */
PHP_FUNCTION(gllighti)
{
	zval *light, *pname, *param;
	THREE_PARAM(light, pname, param);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_long(param);
	glLighti((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
/* }}} */

/* {{{ void gllightiv(long light, long pname, array params) */
PHP_FUNCTION(gllightiv)
{
	zval *light, *pname, *params;
	GLint *v_params;
	THREE_PARAM(light, pname, params);
	convert_to_long(light);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glLightiv((int)Z_LVAL_P(light),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gllinestipple(long factor, long pattern) */
PHP_FUNCTION(gllinestipple)
{
	long factor, pattern;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &factor, &pattern) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glLineStipple((int)(factor),(unsigned short)(pattern));
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

/* {{{ void gllistbase(long base) */
PHP_FUNCTION(gllistbase)
{
	long base;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &base) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glListBase((unsigned int)base);
}
/* }}} */

/* {{{ void glloadidentity() */
PHP_FUNCTION(glloadidentity)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glLoadIdentity();
}
/* }}} */

/* {{{ void glloadmatrixd(array m) */
PHP_FUNCTION(glloadmatrixd)
{
	zval *m;
	GLdouble *v_m;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &m) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_m = php_array_to_double_array(m);
	glLoadMatrixd(v_m);
}
/* }}} */

/* {{{ void glloadmatrixf(array m) */
PHP_FUNCTION(glloadmatrixf)
{
	zval *m;
	GLfloat *v_m;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &m) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_m = php_array_to_float_array(m);
	glLoadMatrixf(v_m);
}
/* }}} */

/* {{{ void glloadname(long name) */
PHP_FUNCTION(glloadname)
{
	long name;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &name) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glLoadName((unsigned int)name);
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

/* {{{ void glmap1d(long target, double u1, double u2, long stride, long order, array points) */
PHP_FUNCTION(glmap1d)
{
	zval *target, *u1, *u2, *stride, *order, *points;
	GLdouble *v_points;
	SIX_PARAM(target, u1, u2, stride, order, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(stride);
	convert_to_long(order);
	convert_to_array(points);
	v_points = php_array_to_double_array(points);
	glMap1d((int)Z_LVAL_P(target),Z_DVAL_P(u1),Z_DVAL_P(u2),(int)Z_LVAL_P(stride),(int)Z_LVAL_P(order),v_points);
}
/* }}} */

/* {{{ void glmap1f(long target, double u1, double u2, long stride, long order, array points) */
PHP_FUNCTION(glmap1f)
{
	zval *target, *u1, *u2, *stride, *order, *points;
	GLfloat *v_points;
	SIX_PARAM(target, u1, u2, stride, order, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(stride);
	convert_to_long(order);
	convert_to_array(points);
	v_points = php_array_to_float_array(points);
	glMap1f((int)Z_LVAL_P(target),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2),(int)Z_LVAL_P(stride),(int)Z_LVAL_P(order),v_points);
}
/* }}} */

/* {{{ void glmap2d(long target, double u1, double u2, long ustride, long uorder, double v1, double v2, long vstride, long vorder, array points) */
PHP_FUNCTION(glmap2d)
{
	zval *target, *u1, *u2, *ustride, *uorder, *v1, *v2, *vstride, *vorder, *points;
	GLdouble *v_points;
	TEN_PARAM(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(ustride);
	convert_to_long(uorder);
	convert_to_double(v1);
	convert_to_double(v2);
	convert_to_long(vstride);
	convert_to_long(vorder);
	convert_to_array(points);
	v_points = php_array_to_double_array(points);
	glMap2d((int)Z_LVAL_P(target),Z_DVAL_P(u1),Z_DVAL_P(u2),(int)Z_LVAL_P(ustride),(int)Z_LVAL_P(uorder),Z_DVAL_P(v1),Z_DVAL_P(v2),(int)Z_LVAL_P(vstride),(int)Z_LVAL_P(vorder),v_points);
}
/* }}} */

/* {{{ void glmap2f(long target, double u1, double u2, long ustride, long uorder, double v1, double v2, long vstride, long vorder, array points) */
PHP_FUNCTION(glmap2f)
{
	zval *target, *u1, *u2, *ustride, *uorder, *v1, *v2, *vstride, *vorder, *points;
	GLfloat *v_points;
	TEN_PARAM(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
	convert_to_long(target);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(ustride);
	convert_to_long(uorder);
	convert_to_double(v1);
	convert_to_double(v2);
	convert_to_long(vstride);
	convert_to_long(vorder);
	convert_to_array(points);
	v_points = php_array_to_float_array(points);
	glMap2f((int)Z_LVAL_P(target),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2),(int)Z_LVAL_P(ustride),(int)Z_LVAL_P(uorder),(float)Z_DVAL_P(v1),(float)Z_DVAL_P(v2),(int)Z_LVAL_P(vstride),(int)Z_LVAL_P(vorder),v_points);
}
/* }}} */

/* {{{ void glmapgrid1d(long un, double u1, double u2) */
PHP_FUNCTION(glmapgrid1d)
{
	zval *un, *u1, *u2;
	THREE_PARAM(un, u1, u2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	glMapGrid1d((int)Z_LVAL_P(un),Z_DVAL_P(u1),Z_DVAL_P(u2));
}
/* }}} */

/* {{{ void glmapgrid1f(long un, double u1, double u2) */
PHP_FUNCTION(glmapgrid1f)
{
	zval *un, *u1, *u2;
	THREE_PARAM(un, u1, u2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	glMapGrid1f((int)Z_LVAL_P(un),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2));
}
/* }}} */

/* {{{ void glmapgrid2d(long un, double u1, double u2, long vn, double v1, double v2) */
PHP_FUNCTION(glmapgrid2d)
{
	zval *un, *u1, *u2, *vn, *v1, *v2;
	SIX_PARAM(un, u1, u2, vn, v1, v2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(vn);
	convert_to_double(v1);
	convert_to_double(v2);
	glMapGrid2d((int)Z_LVAL_P(un),Z_DVAL_P(u1),Z_DVAL_P(u2),(int)Z_LVAL_P(vn),Z_DVAL_P(v1),Z_DVAL_P(v2));
}
/* }}} */

/* {{{ void glmapgrid2f(long un, double u1, double u2, long vn, double v1, double v2) */
PHP_FUNCTION(glmapgrid2f)
{
	zval *un, *u1, *u2, *vn, *v1, *v2;
	SIX_PARAM(un, u1, u2, vn, v1, v2);
	convert_to_long(un);
	convert_to_double(u1);
	convert_to_double(u2);
	convert_to_long(vn);
	convert_to_double(v1);
	convert_to_double(v2);
	glMapGrid2f((int)Z_LVAL_P(un),(float)Z_DVAL_P(u1),(float)Z_DVAL_P(u2),(int)Z_LVAL_P(vn),(float)Z_DVAL_P(v1),(float)Z_DVAL_P(v2));
}
/* }}} */

/* {{{ void glmaterialf(long face, long pname, double param) */
PHP_FUNCTION(glmaterialf)
{
	zval *face, *pname, *param;
	THREE_PARAM(face, pname, param);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_double(param);
	glMaterialf((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
/* }}} */

/* {{{ void glmaterialfv(long face, long pname, array params) */
PHP_FUNCTION(glmaterialfv)
{
	zval *face, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(face, pname, params);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glMaterialfv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void glmateriali(long face, long pname, long param) */
PHP_FUNCTION(glmateriali)
{
	zval *face, *pname, *param;
	THREE_PARAM(face, pname, param);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_long(param);
	glMateriali((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
/* }}} */

/* {{{ void glmaterialiv(long face, long pname, array params) */
PHP_FUNCTION(glmaterialiv)
{
	zval *face, *pname, *params;
	GLint *v_params;
	THREE_PARAM(face, pname, params);
	convert_to_long(face);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glMaterialiv((int)Z_LVAL_P(face),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void glmatrixmode(long mode) */
PHP_FUNCTION(glmatrixmode)
{
	long mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glMatrixMode((int)mode);
}
/* }}} */

/* {{{ void glmultmatrixd(array m) */
PHP_FUNCTION(glmultmatrixd)
{
	zval *m;
	GLdouble *v_m;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &m) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_m = php_array_to_double_array(m);
	glMultMatrixd(v_m);
}
/* }}} */

/* {{{ void glmultmatrixf(array m) */
PHP_FUNCTION(glmultmatrixf)
{
	zval *m;
	GLfloat *v_m;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &m) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_m = php_array_to_float_array(m);
	glMultMatrixf(v_m);
}
/* }}} */

/* {{{ void glnewlist(long list, long mode) */
PHP_FUNCTION(glnewlist)
{
	long list, mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &list, &mode) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glNewList((unsigned int)(list),(int)(mode));
}
/* }}} */

/* {{{ void glnormal3b(long nx, long ny, long nz) */
PHP_FUNCTION(glnormal3b)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_long(nx);
	convert_to_long(ny);
	convert_to_long(nz);
	glNormal3b((signed char)Z_LVAL_P(nx),(signed char)Z_LVAL_P(ny),(signed char)Z_LVAL_P(nz));
}
/* }}} */

/* {{{ void glnormal3bv(array v) */
PHP_FUNCTION(glnormal3bv)
{
	zval *v;
	GLbyte *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_schar_array(v);
	glNormal3bv(v_v);
}
/* }}} */

/* {{{ void glnormal3d(double nx, double ny, double nz) */
PHP_FUNCTION(glnormal3d)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_double(nx);
	convert_to_double(ny);
	convert_to_double(nz);
	glNormal3d(Z_DVAL_P(nx),Z_DVAL_P(ny),Z_DVAL_P(nz));
}
/* }}} */

/* {{{ void glnormal3dv(array v) */
PHP_FUNCTION(glnormal3dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glNormal3dv(v_v);
}
/* }}} */

/* {{{ void glnormal3f(double nx, double ny, double nz) */
PHP_FUNCTION(glnormal3f)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_double(nx);
	convert_to_double(ny);
	convert_to_double(nz);
	glNormal3f((float)Z_DVAL_P(nx),(float)Z_DVAL_P(ny),(float)Z_DVAL_P(nz));
}
/* }}} */

/* {{{ void glnormal3fv(array v) */
PHP_FUNCTION(glnormal3fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glNormal3fv(v_v);
}
/* }}} */

/* {{{ void glnormal3i(long nx, long ny, long nz) */
PHP_FUNCTION(glnormal3i)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_long(nx);
	convert_to_long(ny);
	convert_to_long(nz);
	glNormal3i((int)Z_LVAL_P(nx),(int)Z_LVAL_P(ny),(int)Z_LVAL_P(nz));
}
/* }}} */

/* {{{ void glnormal3iv(array v) */
PHP_FUNCTION(glnormal3iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glNormal3iv(v_v);
}
/* }}} */

/* {{{ void glnormal3s(long nx, long ny, long nz) */
PHP_FUNCTION(glnormal3s)
{
	zval *nx, *ny, *nz;
	THREE_PARAM(nx, ny, nz);
	convert_to_long(nx);
	convert_to_long(ny);
	convert_to_long(nz);
	glNormal3s((short)Z_LVAL_P(nx),(short)Z_LVAL_P(ny),(short)Z_LVAL_P(nz));
}
/* }}} */

/* {{{ void glnormal3sv(array v) */
PHP_FUNCTION(glnormal3sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glNormal3sv(v_v);
}
/* }}} */

/* {{{ void glnormalpointer(long type, long stride, array pointer) */
PHP_FUNCTION(glnormalpointer)
{
	zval *type, *stride, *pointer;
	GLvoid *v_pointer;
	THREE_PARAM(type, stride, pointer);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_BYTE:
		v_pointer = php_array_to_byte_array(pointer);
		break;
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glNormalPointer((int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
}
/* }}} */

/* {{{ void glortho(double left, double right, double bottom, double top, double zNear, double zFar) */
PHP_FUNCTION(glortho)
{
	zval *left, *right, *bottom, *top, *zNear, *zFar;
	SIX_PARAM(left, right, bottom, top, zNear, zFar);
	convert_to_double(left);
	convert_to_double(right);
	convert_to_double(bottom);
	convert_to_double(top);
	convert_to_double(zNear);
	convert_to_double(zFar);
	glOrtho(Z_DVAL_P(left),Z_DVAL_P(right),Z_DVAL_P(bottom),Z_DVAL_P(top),Z_DVAL_P(zNear),Z_DVAL_P(zFar));
}
/* }}} */

/* {{{ void glpassthrough(double token) */
PHP_FUNCTION(glpassthrough)
{
	double token;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &token) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPassThrough((float)token);
}
/* }}} */

/* {{{ void glpixelmapfv(long map, long mapsize, array values) */
PHP_FUNCTION(glpixelmapfv)
{
	zval *map, *mapsize, *values;
	GLfloat *v_values;
	THREE_PARAM(map, mapsize, values);
	convert_to_long(map);
	convert_to_long(mapsize);
	convert_to_array(values);
	v_values = php_array_to_float_array(values);
	glPixelMapfv((int)Z_LVAL_P(map),(int)Z_LVAL_P(mapsize),v_values);
}
/* }}} */

/* {{{ void glpixelmapuiv(long map, long mapsize, array values) */
PHP_FUNCTION(glpixelmapuiv)
{
	zval *map, *mapsize, *values;
	GLuint *v_values;
	THREE_PARAM(map, mapsize, values);
	convert_to_long(map);
	convert_to_long(mapsize);
	convert_to_array(values);
	v_values = php_array_to_uint_array(values);
	glPixelMapuiv((int)Z_LVAL_P(map),(int)Z_LVAL_P(mapsize),v_values);
}
/* }}} */

/* {{{ void glpixelmapusv(long map, long mapsize, array values) */
PHP_FUNCTION(glpixelmapusv)
{
	zval *map, *mapsize, *values;
	GLushort *v_values;
	THREE_PARAM(map, mapsize, values);
	convert_to_long(map);
	convert_to_long(mapsize);
	convert_to_array(values);
	v_values = php_array_to_ushort_array(values);
	glPixelMapusv((int)Z_LVAL_P(map),(int)Z_LVAL_P(mapsize),v_values);
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

/* {{{ void glpixeltransferf(long pname, double param) */
PHP_FUNCTION(glpixeltransferf)
{
	long pname;
	double param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glPixelTransferf((int)(pname),(float)(param));
}
/* }}} */

/* {{{ void glpixeltransferi(long pname, long param) */
PHP_FUNCTION(glpixeltransferi)
{
	long pname, param;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &pname, &param) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glPixelTransferi((int)(pname),(int)(param));
}
/* }}} */

/* {{{ void glpixelzoom(double xfactor, double yfactor) */
PHP_FUNCTION(glpixelzoom)
{
	double xfactor, yfactor;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &xfactor, &yfactor) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glPixelZoom((float)(xfactor),(float)(yfactor));
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

/* {{{ void glpopattrib() */
PHP_FUNCTION(glpopattrib)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPopAttrib();
}
/* }}} */

/* {{{ void glpopclientattrib() */
PHP_FUNCTION(glpopclientattrib)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPopClientAttrib();
}
/* }}} */

/* {{{ void glpopmatrix() */
PHP_FUNCTION(glpopmatrix)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPopMatrix();
}
/* }}} */

/* {{{ void glpopname() */
PHP_FUNCTION(glpopname)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPopName();
}
/* }}} */

/* {{{ void glprioritizetextures(long n, array textures, array priorities) */
PHP_FUNCTION(glprioritizetextures)
{
	zval *n, *textures, *priorities;
	GLuint *v_textures;
	GLclampf *v_priorities;
	THREE_PARAM(n, textures, priorities);
	convert_to_long(n);
	convert_to_array(textures);
	convert_to_array(priorities);
	v_textures = php_array_to_uint_array(textures);
	v_priorities = php_array_to_float_array(priorities);
	glPrioritizeTextures((int)Z_LVAL_P(n),v_textures,v_priorities);
}
/* }}} */

/* {{{ void glpushattrib(long mask) */
PHP_FUNCTION(glpushattrib)
{
	long mask;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPushAttrib(mask);
}
/* }}} */

/* {{{ void glpushclientattrib(long mask) */
PHP_FUNCTION(glpushclientattrib)
{
	long mask;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mask) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPushClientAttrib(mask);
}
/* }}} */

/* {{{ void glpushmatrix() */
PHP_FUNCTION(glpushmatrix)
{
	if( zend_parse_parameters_none() == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPushMatrix();
}
/* }}} */

/* {{{ void glpushname(long name) */
PHP_FUNCTION(glpushname)
{
	long name;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &name) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glPushName((unsigned int)name);
}
/* }}} */

/* {{{ void glrasterpos2d(double x, double y) */
PHP_FUNCTION(glrasterpos2d)
{
	double x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glRasterPos2d((x),(y));
}
/* }}} */

/* {{{ void glrasterpos2dv(array v) */
PHP_FUNCTION(glrasterpos2dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glRasterPos2dv(v_v);
}
/* }}} */

/* {{{ void glrasterpos2f(double x, double y) */
PHP_FUNCTION(glrasterpos2f)
{
	double x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "xy", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glRasterPos2f((float)(x),(float)(y));
}
/* }}} */

/* {{{ void glrasterpos2fv(array v) */
PHP_FUNCTION(glrasterpos2fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glRasterPos2fv(v_v);
}
/* }}} */

/* {{{ void glrasterpos2i(long x, long y) */
PHP_FUNCTION(glrasterpos2i)
{
	long x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glRasterPos2i((int)(x),(int)(y));
}
/* }}} */

/* {{{ void glrasterpos2iv(array v) */
PHP_FUNCTION(glrasterpos2iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glRasterPos2iv(v_v);
}
/* }}} */

/* {{{ void glrasterpos2s(long x, long y) */
PHP_FUNCTION(glrasterpos2s)
{
	long x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glRasterPos2s((short)(x),(short)(y));
}
/* }}} */

/* {{{ void glrasterpos2sv(array v) */
PHP_FUNCTION(glrasterpos2sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glRasterPos2sv(v_v);
}
/* }}} */

/* {{{ void glrasterpos3d(double x, double y, double z) */
PHP_FUNCTION(glrasterpos3d)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRasterPos3d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
/* }}} */

/* {{{ void glrasterpos3dv(array v) */
PHP_FUNCTION(glrasterpos3dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glRasterPos3dv(v_v);
}
/* }}} */

/* {{{ void glrasterpos3f(double x, double y, double z) */
PHP_FUNCTION(glrasterpos3f)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRasterPos3f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
/* }}} */

/* {{{ void glrasterpos3fv(array v) */
PHP_FUNCTION(glrasterpos3fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glRasterPos3fv(v_v);
}
/* }}} */

/* {{{ void glrasterpos3i(long x, long y, long z) */
PHP_FUNCTION(glrasterpos3i)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glRasterPos3i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z));
}
/* }}} */

/* {{{ void glrasterpos3iv(array v) */
PHP_FUNCTION(glrasterpos3iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glRasterPos3iv(v_v);
}
/* }}} */

/* {{{ void glrasterpos3s(long x, long y, long z) */
PHP_FUNCTION(glrasterpos3s)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glRasterPos3s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z));
}
/* }}} */

/* {{{ void glrasterpos3sv(array v) */
PHP_FUNCTION(glrasterpos3sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glRasterPos3sv(v_v);
}
/* }}} */

/* {{{ void glrasterpos4d(double x, double y, double z, double w) */
PHP_FUNCTION(glrasterpos4d)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glRasterPos4d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z),Z_DVAL_P(w));
}
/* }}} */

/* {{{ void glrasterpos4dv(array v) */
PHP_FUNCTION(glrasterpos4dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glRasterPos4dv(v_v);
}
/* }}} */

/* {{{ void glrasterpos4f(double x, double y, double z, double w) */
PHP_FUNCTION(glrasterpos4f)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glRasterPos4f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z),(float)Z_DVAL_P(w));
}
/* }}} */

/* {{{ void glrasterpos4fv(array v) */
PHP_FUNCTION(glrasterpos4fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glRasterPos4fv(v_v);
}
/* }}} */

/* {{{ void glrasterpos4i(long x, long y, long z, long w) */
PHP_FUNCTION(glrasterpos4i)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glRasterPos4i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z),(int)Z_LVAL_P(w));
}
/* }}} */

/* {{{ void glrasterpos4iv(array v) */
PHP_FUNCTION(glrasterpos4iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glRasterPos4iv(v_v);
}
/* }}} */

/* {{{ void glrasterpos4s(long x, long y, long z, long w) */
PHP_FUNCTION(glrasterpos4s)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glRasterPos4s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z),(short)Z_LVAL_P(w));
}
/* }}} */

/* {{{ void glrasterpos4sv(array v) */
PHP_FUNCTION(glrasterpos4sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glRasterPos4sv(v_v);
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

/* {{{ void glrectd(double x1, double y1, double x2, double y2) */
PHP_FUNCTION(glrectd)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_double(x1);
	convert_to_double(y1);
	convert_to_double(x2);
	convert_to_double(y2);
	glRectd(Z_DVAL_P(x1),Z_DVAL_P(y1),Z_DVAL_P(x2),Z_DVAL_P(y2));
}
/* }}} */

/* {{{ void glrectdv(array v1, array v2) */
PHP_FUNCTION(glrectdv)
{
	zval *v1, *v2;
	GLdouble *v_v1;
	GLdouble *v_v2;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &v1, &v2) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_double_array(v1);
	v_v2 = php_array_to_double_array(v2);
	glRectdv(v_v1,v_v2);
}
/* }}} */

/* {{{ void glrectf(double x1, double y1, double x2, double y2) */
PHP_FUNCTION(glrectf)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_double(x1);
	convert_to_double(y1);
	convert_to_double(x2);
	convert_to_double(y2);
	glRectf((float)Z_DVAL_P(x1),(float)Z_DVAL_P(y1),(float)Z_DVAL_P(x2),(float)Z_DVAL_P(y2));
}
/* }}} */

/* {{{ void glrectfv(array v1, array v2) */
PHP_FUNCTION(glrectfv)
{
	zval *v1, *v2;
	GLfloat *v_v1;
	GLfloat *v_v2;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &v1, &v2) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_float_array(v1);
	v_v2 = php_array_to_float_array(v2);
	glRectfv(v_v1,v_v2);
}
/* }}} */

/* {{{ void glrecti(long x1, long y1, long x2, long y2) */
PHP_FUNCTION(glrecti)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_long(x1);
	convert_to_long(y1);
	convert_to_long(x2);
	convert_to_long(y2);
	glRecti((int)Z_LVAL_P(x1),(int)Z_LVAL_P(y1),(int)Z_LVAL_P(x2),(int)Z_LVAL_P(y2));
}
/* }}} */

/* {{{ void glrectiv(array v1, array v2) */
PHP_FUNCTION(glrectiv)
{
	zval *v1, *v2;
	GLint *v_v1;
	GLint *v_v2;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &v1, &v2) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_int_array(v1);
	v_v2 = php_array_to_int_array(v2);
	glRectiv(v_v1,v_v2);
}
/* }}} */

/* {{{ void glrects(long x1, long y1, long x2, long y2) */
PHP_FUNCTION(glrects)
{
	zval *x1, *y1, *x2, *y2;
	FOUR_PARAM(x1, y1, x2, y2);
	convert_to_long(x1);
	convert_to_long(y1);
	convert_to_long(x2);
	convert_to_long(y2);
	glRects((short)Z_LVAL_P(x1),(short)Z_LVAL_P(y1),(short)Z_LVAL_P(x2),(short)Z_LVAL_P(y2));
}
/* }}} */

/* {{{ void glrectsv(array v1, array v2) */
PHP_FUNCTION(glrectsv)
{
	zval *v1, *v2;
	GLshort *v_v1;
	GLshort *v_v2;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &v1, &v2 ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_array(v1);
	convert_to_array(v2);
	v_v1 = php_array_to_short_array(v1);
	v_v2 = php_array_to_short_array(v2);
	glRectsv(v_v1,v_v2);
}
/* }}} */

/* {{{ long glrendermode(long mode) */
PHP_FUNCTION(glrendermode)
{
	long mode;
	int return_int;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	return_int = glRenderMode((int)mode);
	RETURN_LONG(return_int);
}
/* }}} */

/* {{{ void glrotated(double angle, double x, double y, double z) */
PHP_FUNCTION(glrotated)
{
	zval *angle, *x, *y, *z;
	FOUR_PARAM(angle, x, y, z);
	convert_to_double(angle);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRotated(Z_DVAL_P(angle),Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
/* }}} */

/* {{{ void glrotatef(double angle, double x, double y, double z) */
PHP_FUNCTION(glrotatef)
{
	zval *angle, *x, *y, *z;
	FOUR_PARAM(angle, x, y, z);
	convert_to_double(angle);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glRotatef((float)Z_DVAL_P(angle),(float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
/* }}} */

/* {{{ void glscaled(double x, double y, double z) */
PHP_FUNCTION(glscaled)
{
	double x, y, z;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd", &x, &y, &z) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glScaled(x,y,z);
}
/* }}} */

/* {{{ void glscalef(double x, double y, double z) */
PHP_FUNCTION(glscalef)
{
	double x, y, z;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd", &x, &y, &z) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glScalef((float)x,(float)y,(float)z);
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

/* {{{ void glselectbuffer(long size, array buffer) */
PHP_FUNCTION(glselectbuffer)
{
	long size;
	zval *buffer;
	GLuint *v_buffer;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &size, &buffer) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	convert_to_array(buffer);
	v_buffer = php_array_to_uint_array(buffer);
	glSelectBuffer((int)(size),v_buffer);
}
/* }}} */

/* {{{ void glshademodel(long mode) */
PHP_FUNCTION(glshademodel)
{
	long mode;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glShadeModel((int)mode);
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

/* {{{ void gltexcoord1d(double s) */
PHP_FUNCTION(gltexcoord1d)
{
	double s;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &s) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glTexCoord1d(s);
}
/* }}} */

/* {{{ void gltexcoord1dv(array v) */
PHP_FUNCTION(gltexcoord1dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glTexCoord1dv(v_v);
}
/* }}} */

/* {{{ void gltexcoord1f(double s) */
PHP_FUNCTION(gltexcoord1f)
{
	double s;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &s) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glTexCoord1f((float)s);
}
/* }}} */

/* {{{ void gltexcoord1fv(array v) */
PHP_FUNCTION(gltexcoord1fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glTexCoord1fv(v_v);
}
/* }}} */

/* {{{ void gltexcoord1i(long s) */
PHP_FUNCTION(gltexcoord1i)
{
	long s;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &s) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glTexCoord1i((int)s);
}
/* }}} */

/* {{{ void gltexcoord1iv(array v) */
PHP_FUNCTION(gltexcoord1iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glTexCoord1iv(v_v);
}
/* }}} */

/* {{{ void gltexcoord1s(long s) */
PHP_FUNCTION(gltexcoord1s)
{
	long s;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &s) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glTexCoord1s((short)s);
}
/* }}} */

/* {{{ void gltexcoord1sv(array v) */
PHP_FUNCTION(gltexcoord1sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glTexCoord1sv(v_v);
}
/* }}} */

/* {{{ void gltexcoord2d(double s, double t) */
PHP_FUNCTION(gltexcoord2d)
{
	double s, t;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &s, &t) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glTexCoord2d((s),(t));
}
/* }}} */

/* {{{ void gltexcoord2dv(array v) */
PHP_FUNCTION(gltexcoord2dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glTexCoord2dv(v_v);
}
/* }}} */

/* {{{ void gltexcoord2f(double s, double t) */
PHP_FUNCTION(gltexcoord2f)
{
	double s, t;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &s, &t) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glTexCoord2f((float)(s),(float)(t));
}
/* }}} */

/* {{{ void gltexcoord2fv(array v) */
PHP_FUNCTION(gltexcoord2fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glTexCoord2fv(v_v);
}
/* }}} */

/* {{{ void gltexcoord2i(long s, long t) */
PHP_FUNCTION(gltexcoord2i)
{
	long s, t;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &s, &t) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glTexCoord2i((int)(s),(int)(t));
}
/* }}} */

/* {{{ void gltexcoord2iv(array v) */
PHP_FUNCTION(gltexcoord2iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glTexCoord2iv(v_v);
}
/* }}} */

/* {{{ void gltexcoord2s(long s, long t) */
PHP_FUNCTION(gltexcoord2s)
{
	long s, t;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &s, &t) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glTexCoord2s((short)(s),(short)(t));
}
/* }}} */

/* {{{ void gltexcoord2sv(array v) */
PHP_FUNCTION(gltexcoord2sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glTexCoord2sv(v_v);
}
/* }}} */

/* {{{ void gltexcoord3d(double s, double t, double r) */
PHP_FUNCTION(gltexcoord3d)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	glTexCoord3d(Z_DVAL_P(s),Z_DVAL_P(t),Z_DVAL_P(r));
}
/* }}} */

/* {{{ void gltexcoord3dv(array v) */
PHP_FUNCTION(gltexcoord3dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glTexCoord3dv(v_v);
}
/* }}} */

/* {{{ void gltexcoord3f(double s, double t, double r) */
PHP_FUNCTION(gltexcoord3f)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	glTexCoord3f((float)Z_DVAL_P(s),(float)Z_DVAL_P(t),(float)Z_DVAL_P(r));
}
/* }}} */

/* {{{ void gltexcoord3fv(array v) */
PHP_FUNCTION(gltexcoord3fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glTexCoord3fv(v_v);
}
/* }}} */

/* {{{ void gltexcoord3i(long s, long t, long r) */
PHP_FUNCTION(gltexcoord3i)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	glTexCoord3i((int)Z_LVAL_P(s),(int)Z_LVAL_P(t),(int)Z_LVAL_P(r));
}
/* }}} */

/* {{{ void gltexcoord3iv(array v) */
PHP_FUNCTION(gltexcoord3iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glTexCoord3iv(v_v);
}
/* }}} */

/* {{{ void gltexcoord3s(long s, long t, long r) */
PHP_FUNCTION(gltexcoord3s)
{
	zval *s, *t, *r;
	THREE_PARAM(s, t, r);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	glTexCoord3s((short)Z_LVAL_P(s),(short)Z_LVAL_P(t),(short)Z_LVAL_P(r));
}
/* }}} */

/* {{{ void gltexcoord3sv(array v) */
PHP_FUNCTION(gltexcoord3sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glTexCoord3sv(v_v);
}
/* }}} */

/* {{{ void gltexcoord4d(double s, double t, double r, double q) */
PHP_FUNCTION(gltexcoord4d)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	convert_to_double(q);
	glTexCoord4d(Z_DVAL_P(s),Z_DVAL_P(t),Z_DVAL_P(r),Z_DVAL_P(q));
}
/* }}} */

/* {{{ void gltexcoord4dv(array v) */
PHP_FUNCTION(gltexcoord4dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glTexCoord4dv(v_v);
}
/* }}} */

/* {{{ void gltexcoord4f(double s, double t, double r, double q) */
PHP_FUNCTION(gltexcoord4f)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_double(s);
	convert_to_double(t);
	convert_to_double(r);
	convert_to_double(q);
	glTexCoord4f((float)Z_DVAL_P(s),(float)Z_DVAL_P(t),(float)Z_DVAL_P(r),(float)Z_DVAL_P(q));
}
/* }}} */

/* {{{ void gltexcoord4fv(array v) */
PHP_FUNCTION(gltexcoord4fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glTexCoord4fv(v_v);
}
/* }}} */

/* {{{ void gltexcoord4i(long s, long t, long r, long q) */
PHP_FUNCTION(gltexcoord4i)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	convert_to_long(q);
	glTexCoord4i((int)Z_LVAL_P(s),(int)Z_LVAL_P(t),(int)Z_LVAL_P(r),(int)Z_LVAL_P(q));
}
/* }}} */

/* {{{ void gltexcoord4iv(array v) */
PHP_FUNCTION(gltexcoord4iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glTexCoord4iv(v_v);
}
/* }}} */

/* {{{ void gltexcoord4s(long s, long t, long r, long q) */
PHP_FUNCTION(gltexcoord4s)
{
	zval *s, *t, *r, *q;
	FOUR_PARAM(s, t, r, q);
	convert_to_long(s);
	convert_to_long(t);
	convert_to_long(r);
	convert_to_long(q);
	glTexCoord4s((short)Z_LVAL_P(s),(short)Z_LVAL_P(t),(short)Z_LVAL_P(r),(short)Z_LVAL_P(q));
}
/* }}} */

/* {{{ void gltexcoord4sv(array v) */
PHP_FUNCTION(gltexcoord4sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glTexCoord4sv(v_v);
}
/* }}} */

/* {{{ void gltexcoordpointer(long size, long type, long stride, array pointer) */
PHP_FUNCTION(gltexcoordpointer)
{
	zval *size, *type, *stride, *pointer;
	GLvoid *v_pointer;
	FOUR_PARAM(size, type, stride, pointer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glTexCoordPointer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
}
/* }}} */

/* {{{ void gltexenvf(long target, long pname, double param) */
PHP_FUNCTION(gltexenvf)
{
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_double(param);
	glTexEnvf((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
/* }}} */

/* {{{ void gltexenvfv(long target, long pname, array params) */
PHP_FUNCTION(gltexenvfv)
{
	zval *target, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glTexEnvfv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gltexenvi(long target, long pname, long param) */
PHP_FUNCTION(gltexenvi)
{
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_long(param);
	glTexEnvi((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
/* }}} */

/* {{{ void gltexenviv(long target, long pname, array params) */
PHP_FUNCTION(gltexenviv)
{
	zval *target, *pname, *params;
	GLint *v_params;
	THREE_PARAM(target, pname, params);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glTexEnviv((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gltexgend(long coord, long pname, double param) */
PHP_FUNCTION(gltexgend)
{
	zval *coord, *pname, *param;
	THREE_PARAM(coord, pname, param);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_double(param);
	glTexGend((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),Z_DVAL_P(param));
}
/* }}} */

/* {{{ void gltexgendv(long coord, long pname, array params) */
PHP_FUNCTION(gltexgendv)
{
	zval *coord, *pname, *params;
	GLdouble *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_double_array(params);
	glTexGendv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gltexgenf(long coord, long pname, double param) */
PHP_FUNCTION(gltexgenf)
{
	zval *coord, *pname, *param;
	THREE_PARAM(coord, pname, param);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_double(param);
	glTexGenf((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),(float)Z_DVAL_P(param));
}
/* }}} */

/* {{{ void gltexgenfv(long coord, long pname, array params) */
PHP_FUNCTION(gltexgenfv)
{
	zval *coord, *pname, *params;
	GLfloat *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_float_array(params);
	glTexGenfv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
}
/* }}} */

/* {{{ void gltexgeni(long coord, long pname, long param) */
PHP_FUNCTION(gltexgeni)
{
	zval *coord, *pname, *param;
	THREE_PARAM(coord, pname, param);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_long(param);
	glTexGeni((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
}
/* }}} */

/* {{{ void gltexgeniv(long coord, long pname, array params) */
PHP_FUNCTION(gltexgeniv)
{
	zval *coord, *pname, *params;
	GLint *v_params;
	THREE_PARAM(coord, pname, params);
	convert_to_long(coord);
	convert_to_long(pname);
	convert_to_array(params);
	v_params = php_array_to_int_array(params);
	glTexGeniv((int)Z_LVAL_P(coord),(int)Z_LVAL_P(pname),v_params);
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
	zval *target, *level, *internalformat, *width, *height, *border, *format, *type, *pixels;
	GLvoid *v_pixels;
	NINE_PARAM(target, level, internalformat, width, height, border, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(internalformat);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(border);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(internalformat),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(border),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
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
	zval *target, *pname, *param;
	THREE_PARAM(target, pname, param);
	convert_to_long(target);
	convert_to_long(pname);
	convert_to_long(param);
	glTexParameteri((int)Z_LVAL_P(target),(int)Z_LVAL_P(pname),(int)Z_LVAL_P(param));
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
	zval *target, *level, *xoffset, *yoffset, *width, *height, *format, *type, *pixels;
	GLvoid *v_pixels;
	NINE_PARAM(target, level, xoffset, yoffset, width, height, format, type, pixels);
	convert_to_long(target);
	convert_to_long(level);
	convert_to_long(xoffset);
	convert_to_long(yoffset);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_array(pixels);
	v_pixels = php_array_to_long_array(pixels);
	glTexSubImage2D((int)Z_LVAL_P(target),(int)Z_LVAL_P(level),(int)Z_LVAL_P(xoffset),(int)Z_LVAL_P(yoffset),(int)Z_LVAL_P(width),(int)Z_LVAL_P(height),(int)Z_LVAL_P(format),(int)Z_LVAL_P(type),v_pixels);
}
/* }}} */

/* {{{ void gltranslated(double x, double y, double z) */
PHP_FUNCTION(gltranslated)
{
	double x, y, z;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd", &x, &y, &z) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glTranslated(x,y,z);
}
/* }}} */

/* {{{ void gltranslatef(double x, double y, double z) */
PHP_FUNCTION(gltranslatef)
{
	double x, y, z;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd", &x, &y, &z) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glTranslatef((float)x,(float)y,(float)z);
}
/* }}} */

/* {{{ void glvertex2d(double x, double y) */
PHP_FUNCTION(glvertex2d)
{
	double x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glVertex2d((x),(y));
}
/* }}} */

/* {{{ void glvertex2dv(array v) */
PHP_FUNCTION(glvertex2dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glVertex2dv(v_v);
}
/* }}} */

/* {{{ void glvertex2f(double x, double y) */
PHP_FUNCTION(glvertex2f)
{
	double x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glVertex2f((float)(x),(float)(y));
}
/* }}} */

/* {{{ void glvertex2fv(array v) */
PHP_FUNCTION(glvertex2fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glVertex2fv(v_v);
}
/* }}} */

/* {{{ void glvertex2i(long x, long y) */
PHP_FUNCTION(glvertex2i)
{
	long x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glVertex2i((int)(x),(int)(y));
}
/* }}} */

/* {{{ void glvertex2iv(array v) */
PHP_FUNCTION(glvertex2iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glVertex2iv(v_v);
}
/* }}} */

/* {{{ void glvertex2s(long x, long y) */
PHP_FUNCTION(glvertex2s)
{
	long x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glVertex2s((short)(x),(short)(y));
}
/* }}} */

/* {{{ void glvertex2sv(array v) */
PHP_FUNCTION(glvertex2sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glVertex2sv(v_v);
}
/* }}} */

/* {{{ void glvertex3d(double x, double y, double z) */
PHP_FUNCTION(glvertex3d)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glVertex3d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
}
/* }}} */

/* {{{ void glvertex3dv(array v) */
PHP_FUNCTION(glvertex3dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glVertex3dv(v_v);
}
/* }}} */

/* {{{ void glvertex3f(double x, double y, double z) */
PHP_FUNCTION(glvertex3f)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	glVertex3f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z));
}
/* }}} */

/* {{{ void glvertex3fv(array v) */
PHP_FUNCTION(glvertex3fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glVertex3fv(v_v);
}
/* }}} */

/* {{{ void glvertex3i(long x, long y, long z) */
PHP_FUNCTION(glvertex3i)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glVertex3i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z));
}
/* }}} */

/* {{{ void glvertex3iv(array v) */
PHP_FUNCTION(glvertex3iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glVertex3iv(v_v);
}
/* }}} */

/* {{{ void glvertex3s(long x, long y, long z) */
PHP_FUNCTION(glvertex3s)
{
	zval *x, *y, *z;
	THREE_PARAM(x, y, z);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	glVertex3s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z));
}
/* }}} */

/* {{{ void glvertex3sv(array v) */
PHP_FUNCTION(glvertex3sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glVertex3sv(v_v);
}
/* }}} */

/* {{{ void glvertex4d(double x, double y, double z, double w) */
PHP_FUNCTION(glvertex4d)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glVertex4d(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z),Z_DVAL_P(w));
}
/* }}} */

/* {{{ void glvertex4dv(array v) */
PHP_FUNCTION(glvertex4dv)
{
	zval *v;
	GLdouble *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_double_array(v);
	glVertex4dv(v_v);
}
/* }}} */

/* {{{ void glvertex4f(double x, double y, double z, double w) */
PHP_FUNCTION(glvertex4f)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(z);
	convert_to_double(w);
	glVertex4f((float)Z_DVAL_P(x),(float)Z_DVAL_P(y),(float)Z_DVAL_P(z),(float)Z_DVAL_P(w));
}
/* }}} */

/* {{{ void glvertex4fv(array v) */
PHP_FUNCTION(glvertex4fv)
{
	zval *v;
	GLfloat *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_float_array(v);
	glVertex4fv(v_v);
}
/* }}} */

/* {{{ void glvertex4i(long x, long y, long z, long w) */
PHP_FUNCTION(glvertex4i)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glVertex4i((int)Z_LVAL_P(x),(int)Z_LVAL_P(y),(int)Z_LVAL_P(z),(int)Z_LVAL_P(w));
}
/* }}} */

/* {{{ void glvertex4iv(array v) */
PHP_FUNCTION(glvertex4iv)
{
	zval *v;
	GLint *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_int_array(v);
	glVertex4iv(v_v);
}
/* }}} */

/* {{{ void glvertex4s(long x, long y, long z, long w) */
PHP_FUNCTION(glvertex4s)
{
	zval *x, *y, *z, *w;
	FOUR_PARAM(x, y, z, w);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(z);
	convert_to_long(w);
	glVertex4s((short)Z_LVAL_P(x),(short)Z_LVAL_P(y),(short)Z_LVAL_P(z),(short)Z_LVAL_P(w));
}
/* }}} */

/* {{{ void glvertex4sv(array v) */
PHP_FUNCTION(glvertex4sv)
{
	zval *v;
	GLshort *v_v;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &v) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	v_v = php_array_to_short_array(v);
	glVertex4sv(v_v);
}
/* }}} */

/* {{{ void glvertexpointer(long size, long type, long stride, array pointer) */
PHP_FUNCTION(glvertexpointer)
{
	zval *size, *type, *stride, *pointer;
	GLvoid *v_pointer;
	FOUR_PARAM(size, type, stride, pointer);
	convert_to_long(size);
	convert_to_long(type);
	convert_to_long(stride);
	convert_to_array(pointer);
	switch(Z_LVAL_P(type))
	{
	case GL_SHORT:
		v_pointer = php_array_to_short_array(pointer);
		break;
	case GL_INT:
		v_pointer = php_array_to_int_array(pointer);
		break;
	case GL_FLOAT:
		v_pointer = php_array_to_float_array(pointer);
		break;
	case GL_DOUBLE:
		v_pointer = php_array_to_double_array(pointer);
		break;
	}
	glVertexPointer((int)Z_LVAL_P(size),(int)Z_LVAL_P(type),(int)Z_LVAL_P(stride),v_pointer);
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


PHP_MINIT_FUNCTION(opengl)
{
	//le_quad = zend_register_list_destructors_ex(gluquadric_destructor, NULL, "GLUquadric", module_number);
	//le_nurb = zend_register_list_destructors_ex(glunurbs_destructor, NULL, "GLUnurbs", module_number);
	//le_tess = zend_register_list_destructors_ex(glutesselator_destructor, NULL, "GLUtesselator", module_number);

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

	PHP_MINIT(glut)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}

