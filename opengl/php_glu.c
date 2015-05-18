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
  | Author: Santiago Lizardo <http://www.santiagolizardo.com>            |
  +----------------------------------------------------------------------+
*/


#ifdef PHP_WIN32
#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "php.h"
#include "php_glu.h"
#include "php_opengl.h"
#include "php_convert.h"

int le_quad = 0;
int le_nurb = 0;
int le_tess = 0;


int quadric_callback = 0;

/* {{{ string gluerrorstring(long errCode) */
PHP_FUNCTION(gluerrorstring)
{
	long errCode;
	GLubyte *str;
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &errCode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	str = (GLubyte *)gluErrorString(errCode);
	RETURN_STRING(str,1);
}
/* }}} */


/* {{{ string gluerrorunicodestringext(long errCode) */
PHP_FUNCTION(gluerrorunicodestringext)
{
	/*zval *errCode;
	wchar_t *str;
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &errCode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(errCode);
	str = (wchar_t *)gluErrorUnicodeStringEXT(Z_LVAL_P(errCode));
	RETURN_STRING(str,1);*/
}
/* }}} */


/* {{{ string glugetstring(long errCode) */
PHP_FUNCTION(glugetstring)
{
	long errCode;
	GLubyte *str;
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &errCode) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	str = (GLubyte *)gluGetString(errCode);
	RETURN_STRING(str,1);
}
/* }}} */


/* {{{ void gluortho2d(double left, double right, double bottom, double top) */
PHP_FUNCTION(gluortho2d)
{
	zval *left,*right,*bottom,*top;
	FOUR_PARAM(left,right,bottom,top);
	convert_to_double(left);
	convert_to_double(right);
	convert_to_double(bottom);
	convert_to_double(top);
	gluOrtho2D(Z_DVAL_P(left),Z_DVAL_P(right),Z_DVAL_P(bottom),Z_DVAL_P(top));
}
/* }}} */


/* {{{ void gluperspective(double degree, double ratio, double neara, double fara) */
PHP_FUNCTION(gluperspective)
{
	double degree, ratio, neara, fara;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &degree, &ratio, &neara, &fara) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	gluPerspective((degree),(ratio),(neara),(fara));
}
/* }}} */


/* {{{ void glupickatrix(double x, double y, double width, double height, array viewport) */
PHP_FUNCTION(glupickatrix)
{
	zval *x,*y,*width,*height,*viewport;
	GLint *v_viewport;
	FIVE_PARAM(x,y,width,height,viewport);
	convert_to_double(x);
	convert_to_double(y);
	convert_to_double(width);
	convert_to_double(height);
	convert_to_array(viewport);
	v_viewport = php_array_to_int_array(viewport);
	gluPickMatrix(Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(width),Z_DVAL_P(height),v_viewport);
}
/* }}} */


/* {{{ void gluproject(double objx, double objy, double objz, array modelMatrix, array projMatrix, array viewport, double winx, double winy, double winz) */
PHP_FUNCTION(gluproject)
{
	zval *objx,*objy,*objz,*modelMatrix,*projMatrix,*viewport,*winx,*winy,*winz;
	GLdouble *v_model,*v_proj,v_winx,v_winy,v_winz;
	GLint *v_viewport;

	NINE_PARAM(objx,objy,objz,modelMatrix,projMatrix,viewport,winx,winy,winz);
	convert_to_double(objx);
	convert_to_double(objy);
	convert_to_double(objz);
	convert_to_array(modelMatrix);
	convert_to_array(projMatrix);
	convert_to_array(viewport);
	convert_to_double(winx);
	convert_to_double(winy);
	convert_to_double(winz);
	v_model = php_array_to_double_array(modelMatrix);
	v_proj = php_array_to_double_array(projMatrix);
	v_viewport = php_array_to_int_array(viewport);
	gluProject(Z_DVAL_P(objx),Z_DVAL_P(objy),Z_DVAL_P(objz),v_model,v_proj,v_viewport,&v_winx,&v_winy,&v_winz);
	ZVAL_DOUBLE(winx,v_winx);ZVAL_DOUBLE(winy,v_winy);ZVAL_DOUBLE(winz,v_winz);
}
/* }}} */


/* {{{ void glunuproject(double objx, double objy, double objz, array modelMatrix, array projMatrix, array viewport, double winx, double winy, double winz) */
PHP_FUNCTION(glunuproject)
{
	zval *objx,*objy,*objz,*modelMatrix,*projMatrix,*viewport,*winx,*winy,*winz;
	GLdouble *v_model,*v_proj,v_winx,v_winy,v_winz;
	GLint *v_viewport;

	NINE_PARAM(objx,objy,objz,modelMatrix,projMatrix,viewport,winx,winy,winz);
	convert_to_double(objx);
	convert_to_double(objy);
	convert_to_double(objz);
	convert_to_array(modelMatrix);
	convert_to_array(projMatrix);
	convert_to_array(viewport);
	convert_to_double(winx);
	convert_to_double(winy);
	convert_to_double(winz);
	v_model = php_array_to_double_array(modelMatrix);
	v_proj = php_array_to_double_array(projMatrix);
	v_viewport = php_array_to_int_array(viewport);
	gluUnProject(Z_DVAL_P(objx),Z_DVAL_P(objy),Z_DVAL_P(objz),v_model,v_proj,v_viewport,&v_winx,&v_winy,&v_winz);
	ZVAL_DOUBLE(winx,v_winx);ZVAL_DOUBLE(winy,v_winy);ZVAL_DOUBLE(winz,v_winz);
}
/* }}} */


/* {{{ void gluscaleimage(long format, long widthin, long heightin, long typein, string datain, long widthout, long heightout, long typeout, string dataout) */
PHP_FUNCTION(gluscaleimage)
{
	zval *format,*widthin,*heightin,*typein,*datain,*widthout,*heightout,*typeout,*dataout;
	void *v_dataout;
	int size;

	NINE_PARAM(format,widthin,heightin,typein,datain,widthout,heightout,typeout,dataout);
	convert_to_long(format);
	convert_to_long(widthin);
	convert_to_long(heightin);
	convert_to_long(typein);
	convert_to_long(widthout);
	convert_to_long(heightout);
	convert_to_long(typeout);
	convert_to_string(dataout);
	convert_to_string(datain);

	size = Z_LVAL_P(widthout) * Z_LVAL_P(heightout) * gl_pixel_size(Z_LVAL_P(format)) * gl_type_size(Z_LVAL_P(typeout));
	v_dataout = emalloc(size);

	gluScaleImage(Z_LVAL_P(format),Z_LVAL_P(widthin),Z_LVAL_P(heightin),Z_LVAL_P(typein),Z_STRVAL_P(datain),Z_LVAL_P(widthout),Z_LVAL_P(heightout),Z_LVAL_P(typeout),v_dataout);

	Z_STRVAL_P(dataout) = v_dataout;
	Z_STRLEN_P(dataout) = size;
}
/* }}} */


/* {{{ void glulookat(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) */
PHP_FUNCTION(glulookat)
{
	zval *x1,*y1,*z1,*x2,*y2,*z2,*x3,*y3,*z3;
	NINE_PARAM(x1,y1,z1,x2,y2,z2,x3,y3,z3);
	convert_to_double(x1);
	convert_to_double(y1);
	convert_to_double(z1);
	convert_to_double(x2);
	convert_to_double(y2);
	convert_to_double(z2);
	convert_to_double(x3);
	convert_to_double(y3);
	convert_to_double(z3);
	gluLookAt(Z_DVAL_P(x1),Z_DVAL_P(y1),Z_DVAL_P(z1),Z_DVAL_P(x2),Z_DVAL_P(y2),Z_DVAL_P(z2),Z_DVAL_P(x3),Z_DVAL_P(y3),Z_DVAL_P(z3));
}
/* }}} */


/* {{{ long glubuild1dmipmaps(long target, long components, long width, long format, long type, string data) */
PHP_FUNCTION(glubuild1dmipmaps)
{
	long target, components, width, format, type;
	char *data;
	int data_len;
	int ret;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllls", &target, &components, &width, &format, &type, &data, &data_len) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	ret = gluBuild1DMipmaps((target),(components),(width),(format),(type),(data));
	RETURN_LONG(ret);
}
/* }}} */


/* {{{ long glubuild2dmipmaps(long target, long components, long width, long height, long format, long type, string data) */
PHP_FUNCTION(glubuild2dmipmaps)
{
	zval *target,*components,*width,*height,*format,*type,*data;
	int ret;
	SEVEN_PARAM(target,components,width,height,format,type,data);
	convert_to_long(target);
	convert_to_long(components);
	convert_to_long(width);
	convert_to_long(height);
	convert_to_long(format);
	convert_to_long(type);
	convert_to_string(data);
	ret = gluBuild2DMipmaps(Z_LVAL_P(target),Z_LVAL_P(components),Z_LVAL_P(width),Z_LVAL_P(height),Z_LVAL_P(format),Z_LVAL_P(type),Z_STRVAL_P(data));
	RETURN_LONG(ret);
}
/* }}} */


/* {{{ resource glunewquadric() */
PHP_FUNCTION(glunewquadric)
{
	int ret;
	GLUquadric *quad = gluNewQuadric();
#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 3) || (PHP_MAJOR_VERSION > 5)
	ret = zend_list_insert(quad, le_quad TSRMLS_CC);
#else
	ret = zend_list_insert(quad, le_quad);
#endif
	RETURN_RESOURCE(ret);
}
/* }}} */


/* {{{ void gludeletequadric(resource quad) */
PHP_FUNCTION(gludeletequadric)
{
	zval *quad;
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &quad) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(quad) == IS_RESOURCE)
	{
		zend_list_delete(Z_RESVAL_P(quad));
	}
}
/* }}} */


/* {{{ void gluquadricnormals(resource quad, long normals) */
PHP_FUNCTION(gluquadricnormals)
{
	zval *quad;
	long normals;
	GLUquadric *gluquad;
	int type;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &quad, &normals) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
	if(gluquad != NULL && type == le_quad)
	{
		gluQuadricNormals(gluquad,normals);
	}
}
/* }}} */


/* {{{ void gluquadrictexture(resource quad, boolean textcords) */
PHP_FUNCTION(gluquadrictexture)
{
	zval *quad;
	zend_bool textrecords;
	GLUquadric *gluquad;
	int type;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rb", &quad, &textrecords) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
	if(gluquad != NULL && type == le_quad)
	{
		gluQuadricTexture(gluquad,textrecords);
	}
}
/* }}} */


/* {{{ void gluquadricorientation(resource quad, long orientation) */
PHP_FUNCTION(gluquadricorientation)
{
	zval *quad;
	long orientation;
	GLUquadric *gluquad;
	int type;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &quad, &orientation) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
	if(gluquad != NULL && type == le_quad)
	{
		gluQuadricOrientation(gluquad,orientation);
	}
}
/* }}} */


/* {{{ void gluquadricdrawstyle(resource quad, long drawStyle) */
PHP_FUNCTION(gluquadricdrawstyle)
{
	zval *quad;
	long drawStyle;
	GLUquadric *gluquad;
	int type;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &quad, &drawStyle) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
	if(gluquad != NULL && type == le_quad)
	{
		gluQuadricDrawStyle(gluquad,drawStyle);
	}
}
/* }}} */


/* {{{ void glucylinder(resource quad, double baseRadius, double topRadius, double height, long slices, long stacks) */
PHP_FUNCTION(glucylinder)
{
	zval *quad;
	double baseRadius, topRadius, height;
	long slices, stacks;
	GLUquadric *gluquad;
	int type;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdddll", &quad, &baseRadius, &topRadius, &height, &slices, &stacks) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
	if(gluquad != NULL && type == le_quad)
	{
		gluCylinder(gluquad,(baseRadius),(topRadius),(height),(slices),(stacks));
	}
}
/* }}} */


/* {{{ void gludisk(resource quad, double innerRadius, double outerRadius, long slices, long stacks) */
PHP_FUNCTION(gludisk)
{
	zval *quad,*innerRadius,*outerRadius,*slices,*stacks;
	GLUquadric *gluquad;
	int type;

	FIVE_PARAM(quad,innerRadius,outerRadius,slices,stacks);
	if(Z_TYPE_P(quad) == IS_RESOURCE)
	{
		convert_to_double(innerRadius);
		convert_to_double(outerRadius);
		convert_to_long(slices);
		convert_to_long(stacks);
		gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
		if(gluquad != NULL && type == le_quad)
		{
			gluDisk(gluquad,Z_DVAL_P(innerRadius),Z_DVAL_P(outerRadius),Z_LVAL_P(slices),Z_LVAL_P(stacks));
		}
	}
}
/* }}} */


/* {{{ void glupartialdisk(resource quad, double innerRadius, double outerRadius, long slices, long stacks, double startAngle, double sweepAngle) */
PHP_FUNCTION(glupartialdisk)
{
	zval *quad,*innerRadius,*outerRadius,*slices,*stacks,*startAngle,*sweepAngle;
	GLUquadric *gluquad;
	int type;

	SEVEN_PARAM(quad,innerRadius,outerRadius,slices,stacks,startAngle,sweepAngle);
	if(Z_TYPE_P(quad) == IS_RESOURCE)
	{
		convert_to_double(innerRadius);
		convert_to_double(outerRadius);
		convert_to_long(slices);
		convert_to_long(stacks);
		convert_to_double(startAngle);
		convert_to_double(sweepAngle);
		gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
		if(gluquad != NULL && type == le_quad)
		{
			gluPartialDisk(gluquad,Z_DVAL_P(innerRadius),Z_DVAL_P(outerRadius),Z_LVAL_P(slices),Z_LVAL_P(stacks),Z_DVAL_P(startAngle),Z_DVAL_P(sweepAngle));
		}
	}
}
/* }}} */


/* {{{ void glusphere(resource quad, double radius, long slices, long stacks) */
PHP_FUNCTION(glusphere)
{
	zval *quad,*radius,*slices,*stacks;
	GLUquadric *gluquad;
	int type;

	FOUR_PARAM(quad,radius,slices,stacks);
	if(Z_TYPE_P(quad) == IS_RESOURCE)
	{
		convert_to_double(radius);
		convert_to_long(slices);
		convert_to_long(stacks);
		gluquad = zend_list_find(Z_RESVAL_P(quad),&type);
		if(gluquad != NULL && type == le_quad)
		{
			gluSphere(gluquad,Z_DVAL_P(radius),Z_LVAL_P(slices),Z_LVAL_P(stacks));
		}
	}
}
/* }}} */


/* {{{ resource glunewtess() */
PHP_FUNCTION(glunewtess)
{
	int ret;
	GLUtesselator *tess = gluNewTess();
#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 3) || (PHP_MAJOR_VERSION > 5)
	ret = zend_list_insert(tess, le_tess TSRMLS_CC);
#else
	ret = zend_list_insert(tess, le_tess);
#endif
	RETURN_RESOURCE(ret);
}
/* }}} */


/* {{{ void gludeletetess(resource tess) */
PHP_FUNCTION(gludeletetess)
{
	zval *tess;
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &tess) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		zend_list_delete(Z_RESVAL_P(tess));
	}
}
/* }}} */


/* {{{ void glutessbeginpolygon(resource tess, unknown data) */
PHP_FUNCTION(glutessbeginpolygon)
{
	zval *tess,*data;
	GLUtesselator *glutess;
	int type;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &tess, &data) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutess = zend_list_find(Z_RESVAL_P(tess),&type);
	if(glutess != NULL && type == le_tess)
	{
		gluTessBeginPolygon(glutess,NULL);
	}
}
/* }}} */


/* {{{ void glutessbegincontour(resource tess) */
PHP_FUNCTION(glutessbegincontour)
{
	zval *tess;
	GLUtesselator *glutess;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &tess) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		glutess = zend_list_find(Z_RESVAL_P(tess),&type);
		if(glutess != NULL && type == le_tess)
		{
			gluTessBeginContour(glutess);
		}
	}
}
/* }}} */


/* {{{ void glutessvertex(resource tess, array cords, unknown data) */
PHP_FUNCTION(glutessvertex)
{
	zval *tess,*cords,*data;
	GLUtesselator *glutess;
	int type;
	GLdouble *v_cords;

	THREE_PARAM(tess,cords,data);
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		convert_to_array(cords);
		v_cords = php_array_to_double_array(cords);
		glutess = zend_list_find(Z_RESVAL_P(tess),&type);
		if(glutess != NULL && type == le_tess)
		{
			gluTessVertex(glutess,v_cords,NULL);
		}
	}
}
/* }}} */


/* {{{ void glutessendcontour(resource tess) */
PHP_FUNCTION(glutessendcontour)
{
	zval *tess;
	GLUtesselator *glutess;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &tess) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		glutess = zend_list_find(Z_RESVAL_P(tess),&type);
		if(glutess != NULL && type == le_tess)
		{
			gluTessEndContour(glutess);
		}
	}
}
/* }}} */


/* {{{ void glutessendpolygon(resource tess) */
PHP_FUNCTION(glutessendpolygon)
{
	zval *tess;
	GLUtesselator *glutess;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &tess) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		glutess = zend_list_find(Z_RESVAL_P(tess),&type);
		if(glutess != NULL && type == le_tess)
		{
			gluTessEndPolygon(glutess);
		}
	}
}
/* }}} */


/* {{{ void glutessproperty(resource tess, long which, double value) */
PHP_FUNCTION(glutessproperty)
{
	zval *tess,*which,*value;
	GLUtesselator *glutess;
	int type;

	THREE_PARAM(tess,which,value);
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		convert_to_long(which);
		convert_to_double(value);
		glutess = zend_list_find(Z_RESVAL_P(tess),&type);
		if(glutess != NULL && type == le_tess)
		{
			gluTessProperty(glutess,Z_LVAL_P(which),Z_DVAL_P(value));
		}
	}
}
/* }}} */


/* {{{ void glutessnormal(resource tess, double x, double y, double z) */
PHP_FUNCTION(glutessnormal)
{
	zval *tess,*x,*y,*z;
	GLUtesselator *glutess;
	int type;

	FOUR_PARAM(tess,x,y,z);
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		convert_to_double(x);
		convert_to_double(y);
		convert_to_double(z);
		glutess = zend_list_find(Z_RESVAL_P(tess),&type);
		if(glutess != NULL && type == le_tess)
		{
			gluTessNormal(glutess,Z_DVAL_P(x),Z_DVAL_P(y),Z_DVAL_P(z));
		}
	}
}
/* }}} */


/* {{{ void glugettessproperty(resource tess, long which, double value) */
PHP_FUNCTION(glugettessproperty)
{
	zval *tess,*which,*value;
	GLUtesselator *glutess;
	int type;

	THREE_PARAM(tess,which,value);
	if(Z_TYPE_P(tess) == IS_RESOURCE)
	{
		convert_to_long(which);
		convert_to_double(value);
		glutess = zend_list_find(Z_RESVAL_P(tess),&type);
		if(glutess != NULL && type == le_tess)
		{
			gluGetTessProperty(glutess,Z_LVAL_P(which),&Z_DVAL_P(value));
		}
	}
}
/* }}} */


/* {{{ resource glunewnurbsrenderer() */
PHP_FUNCTION(glunewnurbsrenderer)
{
	int ret;
	GLUnurbs *nurb = gluNewNurbsRenderer();
#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 3) || (PHP_MAJOR_VERSION > 5)
	ret = zend_list_insert(nurb, le_nurb TSRMLS_CC);
#else
	ret = zend_list_insert(nurb, le_nurb);
#endif
	RETURN_RESOURCE(ret);
}
/* }}} */


/* {{{ void gludeletenurbsrenderer(resource nurb) */
PHP_FUNCTION(gludeletenurbsrenderer)
{
	zval *nurb;
	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &nurb) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		zend_list_delete(Z_RESVAL_P(nurb));
	}
}
/* }}} */


/* {{{ void glubeginsurface(resource nurb) */
PHP_FUNCTION(glubeginsurface)
{
	zval *nurb;
	GLUnurbs *glunurb;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &nurb) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&type);
		if(glunurb != NULL && type == le_nurb)
		{
			gluBeginSurface(glunurb);
		}
	}
}
/* }}} */


/* {{{ void glubegincurve(resource nurb) */
PHP_FUNCTION(glubegincurve)
{
	zval *nurb;
	GLUnurbs *glunurb;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &nurb) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&type);
		if(glunurb != NULL && type == le_nurb)
		{
			gluBeginCurve(glunurb);
		}
	}
}
/* }}} */


/* {{{ void gluendcurve(resource nurb) */
PHP_FUNCTION(gluendcurve)
{
	zval *nurb;
	GLUnurbs *glunurb;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "l", &nurb) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&type);
		if(glunurb != NULL && type == le_nurb)
		{
			gluEndCurve(glunurb);
		}
	}
}
/* }}} */


/* {{{ void gluendsurface(resource nurb) */
PHP_FUNCTION(gluendsurface)
{
	zval *nurb;
	GLUnurbs *glunurb;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &nurb) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&type);
		if(glunurb != NULL && type == le_nurb)
		{
			gluEndSurface(glunurb);
		}
	}
}
/* }}} */


/* {{{ void glubegintrim(resource nurb) */
PHP_FUNCTION(glubegintrim)
{
	zval *nurb;
	GLUnurbs *glunurb;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &nurb) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&type);
		if(glunurb != NULL && type == le_nurb)
		{
			gluBeginTrim(glunurb);
		}
	}
}
/* }}} */


/* {{{ void gluendtrim(resource nurb) */
PHP_FUNCTION(gluendtrim)
{
	zval *nurb;
	GLUnurbs *glunurb;
	int type;

	if( zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "z", &nurb) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&type);
		if(glunurb != NULL && type == le_nurb)
		{
			gluEndTrim(glunurb);
		}
	}
}
/* }}} */


/* {{{ void glupwlcurve(resource nurb, long count, array array, long stride, long type) */
PHP_FUNCTION(glupwlcurve)
{
	zval *nurb,*count,*array,*stride,*type;
	GLUnurbs *glunurb;
	int ctype;
	float *v_array;

	FIVE_PARAM(nurb,count,array,stride,type);
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		convert_to_long(count);
		convert_to_long(stride);
		convert_to_long(type);
		convert_to_array(array);
		v_array = php_array_to_float_array(array);
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&ctype);
		if(glunurb != NULL && ctype == le_nurb)
		{
			gluPwlCurve(glunurb,Z_LVAL_P(count),v_array,Z_LVAL_P(stride),Z_LVAL_P(type));
		}
	}
}
/* }}} */


/* {{{ void glunurbscurve(resource nurb, long nknots, array knot, long stride, array ctlarray, long order, long type) */
PHP_FUNCTION(glunurbscurve)
{
	zval *nurb,*nknots,*knot,*stride,*ctlarray,*order,*type;
	GLUnurbs *glunurb;
	int ctype;
	float *v_knot,*v_ctlarray;

	SEVEN_PARAM(nurb,nknots,knot,stride,ctlarray,order,type);
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		convert_to_long(nknots);
		convert_to_long(stride);
		convert_to_long(type);
		convert_to_long(order);
		convert_to_array(knot);
		convert_to_array(ctlarray);
		v_knot = php_array_to_float_array(knot);
		v_ctlarray = php_array_to_float_array(ctlarray);
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&ctype);
		if(glunurb != NULL && ctype == le_nurb)
		{
			gluNurbsCurve(glunurb,Z_LVAL_P(nknots),v_knot,Z_LVAL_P(stride),v_ctlarray,Z_LVAL_P(order),Z_LVAL_P(type));
		}
	}
}
/* }}} */


/* {{{ void glunurbssurface(resource nurb, long sknot_count, array sknot, long tknot_count, array tknot, long s_stride, long t_stride, array ctlarray, long sorder, long torder, long type) */
PHP_FUNCTION(glunurbssurface)
{
	zval *nurb, *sknot, *tknot, *ctlarray;
	long sknot_count, tknot_count, s_stride, t_stride, sorder, torder, type; 
	GLUnurbs *glunurb;
	int ctype;
	float *v_sknot,*v_ctlarray,*v_tknot;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zlalallalll", &nurb, &sknot_count, &sknot, &tknot_count, &tknot, &s_stride, &t_stride, &ctlarray, &sorder, &torder, &type) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	v_sknot = php_array_to_float_array(sknot);
	v_tknot = php_array_to_float_array(tknot);
	v_ctlarray = php_array_to_float_array(ctlarray);
	glunurb = zend_list_find(Z_RESVAL_P(nurb),&ctype);
	if(glunurb != NULL && ctype == le_nurb)
	{
		gluNurbsSurface(glunurb,(sknot_count),v_sknot,(tknot_count),v_tknot,(s_stride),(t_stride),v_ctlarray,(sorder),(torder),(type));
	}
}
/* }}} */


/* {{{ void gluloadsamplingmatrices(resource nurb, array modelMatrix, array projMatrix, array viewport) */
PHP_FUNCTION(gluloadsamplingmatrices)
{
	zval *nurb,*modelMatrix,*projMatrix,*viewport;
	GLUnurbs *glunurb;
	int ctype;
	float *v_modelMatrix,*v_projMatrix;
	int *v_viewport;

	FOUR_PARAM(nurb,modelMatrix,projMatrix,viewport);
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		convert_to_array(modelMatrix);
		convert_to_array(projMatrix);
		convert_to_array(viewport);
		v_modelMatrix = php_array_to_float_array(modelMatrix);
		v_projMatrix = php_array_to_float_array(projMatrix);
		v_viewport = php_array_to_int_array(viewport);
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&ctype);
		if(glunurb != NULL && ctype == le_nurb)
		{
			gluLoadSamplingMatrices(glunurb,v_modelMatrix,v_projMatrix,v_viewport);
		}
	}
}
/* }}} */


/* {{{ void gluNurbsProperty(resource nurb, long property, double value) */
PHP_FUNCTION(gluNurbsProperty)
{
	zval *nurb,*property,*value;
	GLUnurbs *glunurb;
	int ctype;

	THREE_PARAM(nurb,property,value);
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		convert_to_long(property);
		convert_to_double(value);
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&ctype);
		if(glunurb != NULL && ctype == le_nurb)
		{
			gluNurbsProperty(glunurb,Z_LVAL_P(property),(float)Z_DVAL_P(value));
		}
	}
}
/* }}} */


/* {{{ void glugetnurbsproperty(resource nurb, long property, double value) */
PHP_FUNCTION(glugetnurbsproperty)
{
	zval *nurb,*property,*value;
	GLUnurbs *glunurb;
	int ctype;
        float f_property;

	THREE_PARAM(nurb,property,value);
	if(Z_TYPE_P(nurb) == IS_RESOURCE)
	{
		convert_to_long(property);
		convert_to_double(value);
		glunurb = zend_list_find(Z_RESVAL_P(nurb),&ctype);
		if(glunurb != NULL && ctype == le_nurb)
		{
			gluGetNurbsProperty(glunurb,Z_LVAL_P(property),&f_property);
			Z_DVAL_P(value) = f_property;
		}
	}
}
/* }}} */


void gluquadric_destructor(zend_rsrc_list_entry *quad TSRMLS_DC)
{
	gluDeleteQuadric(quad->ptr);
}

void glunurbs_destructor(zend_rsrc_list_entry *nurbs TSRMLS_DC)
{
	gluDeleteNurbsRenderer(nurbs->ptr);
}

void glutesselator_destructor(zend_rsrc_list_entry *tesselator TSRMLS_DC)
{
	gluDeleteTess(tesselator->ptr);
}
