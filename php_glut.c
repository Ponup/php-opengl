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

#define QUOTE(s) #s
#define STRINGIFY(s) QUOTE(s)

#include "php.h"
#include "php_glut.h"
#include <GL/gl.h>
#include <GL/freeglut.h>
#include "php_convert.h"

#ifdef PHP_WIN32
#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl3.h>
#else
//#include <GL/gl.h>
#endif

//#include <ext/standard/info.h>
//#include "php_convert.h"

static HashTable *call_backs;

static HashTable *menu_entry_callbacks;

/* GLUT_DISPLAY_CALLBACK */
zend_fcall_info display_fci;
zend_fcall_info_cache display_fci_cache;

/* GLUT_RESHAPE_CALLBACK */
zend_fcall_info reshape_fci;
zend_fcall_info_cache reshape_fci_cache;

/* */
zend_fcall_info keyboard_fci;
zend_fcall_info_cache keyboard_fci_cache;

zend_fcall_info keyboardup_fci;
zend_fcall_info_cache keyboardup_fci_cache;

zend_fcall_info createmenu_fci;
zend_fcall_info_cache createmenu_fci_cache;

zend_fcall_info special_fci;
zend_fcall_info_cache special_fci_cache;

/* GLUT_MOTION_CALLBACK */
zend_fcall_info motion_fci;
zend_fcall_info_cache motion_fci_cache;
zend_fcall_info passive_motion_fci;
zend_fcall_info_cache passive_motion_fci_cache;


/* GLUT_IDLE_CALLBACK */
zend_fcall_info idle_fci;
zend_fcall_info_cache idle_fci_cache;

/* GLUT_VISIBILITY_CALLBACK */
zend_fcall_info visibility_fci;
zend_fcall_info_cache visibility_fci_cache;

/* GLUT_MOUSE_CALLBACK */
zend_fcall_info mouse_fci;
zend_fcall_info_cache mouse_fci_cache;

zend_fcall_info mousewheel_fci;
zend_fcall_info_cache mousewheel_fci_cache;

ZEND_BEGIN_ARG_INFO_EX(arginfo_glutinitcontextversion, 0, 0, 2)
    ZEND_ARG_INFO(0, major)
    ZEND_ARG_INFO(0, minor)
ZEND_END_ARG_INFO()

PHP_FUNCTION(glutinitcontextversion)
{
    zend_long major, minor;
    
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &major, &minor) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    
    glutInitContextVersion(major, minor);
}

PHP_FUNCTION(glutInitContextProfile)
{
    zend_long profile;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &profile) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    glutInitContextProfile(profile);
}

PHP_FUNCTION(glutInitContextFlags)
{
    zend_long flags;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &flags) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    glutInitContextFlags(flags);
}

/* Glut Init Functions */
/* {{{ void glutinit(long argc, array argv) */
PHP_FUNCTION(glutinit)
{
	long argc;
	zval *argv_arg;
	char **argv;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &argc, &argv_arg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	argv = php_array_to_string_array(argv_arg);
	glutInit((int *)&argc, argv);
}
/* }}} */


/* {{{ void glutinitwindowsize(long width, long height) */
PHP_FUNCTION(glutinitwindowsize)
{
	zend_long width, height;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &width, &height) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutInitWindowSize(width, height);
}
/* }}} */

/* {{{ void glutinitwindowposition(long x, long y) */
PHP_FUNCTION(glutinitwindowposition)
{
	long x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutInitWindowPosition(x, y);
}
/* }}} */

/* {{{ void glutinitdisplaymode(long disp) */
PHP_FUNCTION(glutinitdisplaymode)
{
	zend_long disp;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &disp) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutInitDisplayMode((unsigned int)disp);
}
/* }}} */


/* {{{ void glutmainloop() */
PHP_FUNCTION(glutmainloop)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutMainLoop();
}
/* }}} */


/* {{{ long glutcreatewindow(string name) */
PHP_FUNCTION(glutcreatewindow)
{
	zend_string* name;
	int win;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &name) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	win = glutCreateWindow(name->val);
	RETURN_LONG(win);
}
/* }}} */


/* {{{ void glutcreatesubwindow(long win, long x, long y, long width, long height) */
PHP_FUNCTION(glutcreatesubwindow)
{
	long win, x, y, width, height;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllll", &win , &x, &y, &width, &height ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutCreateSubWindow((win),(x),(y),(width),(height));
}
/* }}} */


/* {{{ void glutsetwindow(long win) */
PHP_FUNCTION(glutsetwindow)
{
	long win;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &win) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glutSetWindow(win);
}
/* }}} */


/* {{{ long glutgetwindow() */
PHP_FUNCTION(glutgetwindow)
{
	RETURN_LONG(glutGetWindow());
}
/* }}} */


/* {{{ void glutdestroywindow(long win) */
PHP_FUNCTION(glutdestroywindow)
{
	long win;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &win) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glutDestroyWindow(win);
}
/* }}} */

/* {{{ void glutpostredisplay() */
PHP_FUNCTION(glutpostredisplay)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutPostRedisplay();
}
/* }}} */


/* {{{ void glutswapbuffers() */
PHP_FUNCTION(glutswapbuffers)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutSwapBuffers();
}
/* }}} */


/* {{{ void glutpositionwindow(long x, long y) */
PHP_FUNCTION(glutpositionwindow)
{
	zend_long x, y;
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutPositionWindow(x,y);
}
/* }}} */


/* {{{ void glutreshapewindow(long width, long height) */
PHP_FUNCTION(glutreshapewindow)
{
	long width, height;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &width, &height) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutPositionWindow(width,height);
}
/* }}} */


/* {{{ void glutfullscreen() */
PHP_FUNCTION(glutfullscreen)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutFullScreen();
}
/* }}} */


/* {{{ void glutpopwindow() */
PHP_FUNCTION(glutpopwindow)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutPopWindow();
}
/* }}} */


/* {{{ void glutpushwindow() */
PHP_FUNCTION(glutpushwindow)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutPushWindow();
}
/* }}} */


/* {{{ void glutshowwindow() */
PHP_FUNCTION(glutshowwindow)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutShowWindow();
}
/* }}} */


/* {{{ void gluthidewindow() */
PHP_FUNCTION(gluthidewindow)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutHideWindow();
}
/* }}} */


/* {{{ void gluticonifywindow() */
PHP_FUNCTION(gluticonifywindow)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutIconifyWindow();
}
/* }}} */


/* {{{ void glutsetwindowtitle(string name) */
PHP_FUNCTION(glutsetwindowtitle)
{
	char *name = NULL;
	size_t name_len;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glutSetWindowTitle(name);
}
/* }}} */

/* {{{ void glutseticontitle(string name) */
PHP_FUNCTION(glutseticontitle)
{
	char *name = NULL;
	size_t name_len;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutSetIconTitle(name);
}
/* }}} */

/* {{{ void glutsetcursor(long cursor) */
PHP_FUNCTION(glutsetcursor)
{
	long cursor;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &cursor) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutSetCursor(cursor);
}
/* }}} */

/* {{{ void glutestablishoverlay() */
PHP_FUNCTION(glutestablishoverlay)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutEstablishOverlay();
}
/* }}} */


/* {{{ void glutuselayer(long layer) */
PHP_FUNCTION(glutuselayer)
{
	long layer;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &layer) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutUseLayer(layer);
}
/* }}} */

/* {{{ void glutremoveoverlay() */
PHP_FUNCTION(glutremoveoverlay)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutRemoveOverlay();
}
/* }}} */


/* {{{ void glutpostoverlayredisplay() */
PHP_FUNCTION(glutpostoverlayredisplay)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutPostOverlayRedisplay();
}
/* }}} */


/* {{{ void glutshowoverlay() */
PHP_FUNCTION(glutshowoverlay)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutShowOverlay();
}
/* }}} */


/* {{{ void gluthideoverlay() */
PHP_FUNCTION(gluthideoverlay)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutHideOverlay();
}
/* }}} */


void menu_callback(int selection)
{
	zval *z_menu_entry, **z_menu_id, **z_menu_parameter, *z_menu_function = NULL, params[1];
	zval retval;
	TSRMLS_FETCH();

	if (!(z_menu_entry = zend_hash_index_find(menu_entry_callbacks, selection))) {
		php_error(E_WARNING, "unknown menu entry callback %d", selection);
		return;
	}
	if (!(z_menu_id = zend_hash_index_find(Z_ARRVAL_P(z_menu_entry), 0))) {
		php_error(E_WARNING, "can't find menu_id");
		return;
	}
	if (!(z_menu_parameter = zend_hash_index_find(Z_ARRVAL_P(z_menu_entry), 1))) {
		php_error(E_WARNING, "can't find menu_parameter");
		return;
	}
	ZVAL_LONG(&params[0], Z_LVAL_PP(z_menu_parameter));
	if (call_user_function(CG(function_table), NULL, z_menu_function, &retval, 1, params TSRMLS_CC) != SUCCESS)
		zend_error(E_ERROR, "Function call failed");
}

#define HASH_CALLBACK(callback,param_num,hash_key) \
	{ \
		zval_add_ref(&callback); \
		zend_hash_index_update(call_backs, hash_key, callback); \
	}

void glutcreatemenu_callback(int value)
{
	zval params[1];
	zval retval;

	ZVAL_LONG(&params[0], value);

	createmenu_fci.param_count = 1;
	createmenu_fci.params = params;
	createmenu_fci.retval = &retval;

	if (zend_call_function(&createmenu_fci, &createmenu_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ long glutcreatemenu(mixed menu_callback) */
PHP_FUNCTION(glutcreatemenu)
{
	int menu_id;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &createmenu_fci, &createmenu_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	menu_id = glutCreateMenu(glutcreatemenu_callback);
	RETURN_LONG(menu_id);
}
/* }}} */

/* {{{ void glutsetmenu(long menu) */
PHP_FUNCTION(glutsetmenu)
{
	long menu;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &menu) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutSetMenu(menu);
}
/* }}} */

/* {{{ long glutgetmenu() */
PHP_FUNCTION(glutgetmenu)
{
	int menu;
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	menu = glutGetMenu();
	RETURN_LONG(menu);
}
/* }}} */


/* {{{ void glutdestroymenu(long menu) */
PHP_FUNCTION(glutdestroymenu)
{
	long menu;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &menu) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutDestroyMenu(menu);
}
/* }}} */

/* {{{ void glutIgnoreKeyRepeat(long menu) */
PHP_FUNCTION(glutignorekeyrepeat)
{
	long menu;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &menu) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutIgnoreKeyRepeat(menu);
}
/* }}} */



/* {{{ bool glutaddmenuentry(string name, long value) */
PHP_FUNCTION(glutaddmenuentry)
{
	char *name = NULL;
	size_t name_len;
	long value;

	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &name, &name_len, &value ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutAddMenuEntry(name, (int)value);
}
/* }}} */


/* {{{ void glutaddsubmenu(string name, long value) */
PHP_FUNCTION(glutaddsubmenu)
{
	char *name = NULL;
	size_t name_len;
	long value;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &name, &name_len, &value) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutAddSubMenu(name,value);
}
/* }}} */

/* {{{ void glutaddsubmenu(string name, long value) */
PHP_FUNCTION(glutwarppointer)
{
	long x, y;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &x, &y) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutWarpPointer(x,y);
}
/* }}} */




/* {{{ void glutchangetomenuentry(long entry, string name, long value) */
PHP_FUNCTION(glutchangetomenuentry)
{
	long entry, value;
	char *name;
	size_t name_len;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lsl", &entry, &name, &name_len, &value) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutChangeToMenuEntry((entry),(name),(value));
}
/* }}} */


/* {{{ void glutchangetosubmenu(long entry, string name, long value) */
PHP_FUNCTION(glutchangetosubmenu)
{
	long entry, value;
	char *name;
	size_t name_len;	
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lsl", &entry, &name, &name_len, &value) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutChangeToSubMenu((entry),(name),(value));
}
/* }}} */


/* {{{ void glutremovemenuitem(long entry) */
PHP_FUNCTION(glutremovemenuitem)
{
	long entry;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &entry) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutRemoveMenuItem(entry);
}
/* }}} */


/* {{{ void glutattachmenu(long button) */
PHP_FUNCTION(glutattachmenu)
{
	long button;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &button) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutAttachMenu(button);
}
/* }}} */


/* {{{ void glutdetachmenu(long button) */
PHP_FUNCTION(glutdetachmenu)
{
	long button;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &button) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutDetachMenu(button);
}
/* }}} */

void glutdisplayfunction_callback()
{
	zval retval;

	display_fci.param_count = 0;
	display_fci.params = NULL;
	display_fci.retval = &retval;

	if (zend_call_function(&display_fci, &display_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
		return;
	}
}

/* {{{ bool glutdisplayfunc(mixed callback) */
PHP_FUNCTION(glutdisplayfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &display_fci, &display_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutDisplayFunc(glutdisplayfunction_callback);
}
/* }}} */


void glutoverlaydisplayfunc_callback()
{
	call_user_callback(call_backs,GLUT_OVERLAY_DISPLAY_CALLBACK,0,NULL);
}

/* {{{ bool glutoverlaydisplayfunc(mixed callback) */
PHP_FUNCTION(glutoverlaydisplayfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_OVERLAY_DISPLAY_CALLBACK);
	glutOverlayDisplayFunc(glutoverlaydisplayfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutreshapefunc_callback(int width, int height)
{
	zval params[2];
	zval retval;

	ZVAL_LONG(&params[0], width);
	ZVAL_LONG(&params[1], height);

	reshape_fci.param_count = 2;
	reshape_fci.params = params;
	reshape_fci.retval = &retval;

	if (zend_call_function(&reshape_fci, &reshape_fci_cache) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutreshapefunc(mixed callback) */
PHP_FUNCTION(glutreshapefunc)
{
	reshape_fci.param_count = 2;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &reshape_fci, &reshape_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutReshapeFunc(glutreshapefunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutkeyboardfunc_callback(unsigned char key, int x, int y)
{
	zval params[3];
	zval retval;
	char str[2];

    str[0] = key;
    str[1] = '\0';

	ZVAL_STRING(&params[0], str);
	ZVAL_LONG(&params[1], x);
	ZVAL_LONG(&params[2], y);

	keyboard_fci.param_count = 3;
	keyboard_fci.params = params;
	keyboard_fci.retval = &retval;

	if (zend_call_function(&keyboard_fci, &keyboard_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
		return;
	}

	//zval_ptr_dtor(&retval);
}

/* {{{ bool glutkeyboardfunc(mixed callback) */
PHP_FUNCTION(glutkeyboardfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &keyboard_fci, &keyboard_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutKeyboardFunc(glutkeyboardfunc_callback);
}
/* }}} */

void glutkeyboardupfunc_callback(unsigned char key,int x,int y)
{
	zval params[3];
	zval retval;
	char str[2];

	sprintf(str,"%c",key);

	ZVAL_STRING(&params[0], str);
	ZVAL_LONG(&params[1], x);
	ZVAL_LONG(&params[2], y);

	keyboardup_fci.param_count = 3;
	keyboardup_fci.params = params;
	keyboardup_fci.retval = &retval;

	if (zend_call_function(&keyboardup_fci, &keyboardup_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}


/* {{{ bool glutkeyboardfunc(mixed callback) */
PHP_FUNCTION(glutkeyboardupfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &keyboardup_fci, &keyboardup_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutKeyboardUpFunc(glutkeyboardupfunc_callback);
	RETURN_TRUE;
}
/* }}} */

void glutmousewheelfunc_callback(int wheel, int direction, int x, int y)
{
	zval params[4];
	zval retval;

	ZVAL_LONG(&params[0], wheel);
	ZVAL_LONG(&params[1], direction);
	ZVAL_LONG(&params[2], x);
	ZVAL_LONG(&params[3], y);

	mousewheel_fci.param_count = 4;
	mousewheel_fci.params = params;
	mousewheel_fci.retval = &retval;

	if (zend_call_function(&mousewheel_fci, &mousewheel_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}

}

ZEND_BEGIN_ARG_INFO_EX(arginfo_glutMouseWheelFunc, 0, 0, 1)
    ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

/* {{{ bool glutmousefunc(mixed callback) */
PHP_FUNCTION(glutMouseWheelFunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &mousewheel_fci, &mousewheel_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutMouseWheelFunc(glutmousewheelfunc_callback);
	RETURN_TRUE;
}

void glutmousefunc_callback(int button, int state, int x, int y)
{
	zval params[4];
	zval retval;

	ZVAL_LONG(&params[0], button);
	ZVAL_LONG(&params[1], state);
	ZVAL_LONG(&params[2], x);
	ZVAL_LONG(&params[3], y);

	mouse_fci.param_count = 4;
	mouse_fci.params = params;
	mouse_fci.retval = &retval;

	if (zend_call_function(&mouse_fci, &mouse_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}

}

/* {{{ bool glutmousefunc(mixed callback) */
PHP_FUNCTION(glutmousefunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &mouse_fci, &mouse_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutMouseFunc(glutmousefunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutmotionfunc_callback(int x,int y)
{
	zval params[2];
	zval retval;

	ZVAL_LONG(&params[0], x);
	ZVAL_LONG(&params[1], y);

	motion_fci.param_count = 2;
	motion_fci.params = params;
	motion_fci.retval = &retval;

	if (zend_call_function(&motion_fci, &motion_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutmotionfunc(mixed callback) */
PHP_FUNCTION(glutmotionfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &motion_fci, &motion_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutMotionFunc(glutmotionfunc_callback);
	RETURN_TRUE;
}
/* }}} */

void glutpassivemotionfunc_callback(int x, int y)
{
	zval params[2];
    zval retval;

	ZVAL_LONG(&params[0], x);
	ZVAL_LONG(&params[1], y);

	passive_motion_fci.param_count = 2;
	passive_motion_fci.params = params;
	passive_motion_fci.retval = &retval;

	if (zend_call_function(&passive_motion_fci, &passive_motion_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutpassivemotionfunc(mixed callback) */
PHP_FUNCTION(glutpassivemotionfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &passive_motion_fci, &passive_motion_fci_cache) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutPassiveMotionFunc(glutpassivemotionfunc_callback);
}
/* }}} */


void glutvisibilityfunc_callback(int state)
{
	zval params[1];
	zval retval;

	ZVAL_LONG(&params[0], state);

	visibility_fci.param_count = 1;
	visibility_fci.params = params;
	visibility_fci.retval = &retval;

	if (zend_call_function(&visibility_fci, &visibility_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutvisibilityfunc(mixed callback) */
PHP_FUNCTION(glutvisibilityfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &visibility_fci, &visibility_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutVisibilityFunc(glutvisibilityfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutentryfunc_callback(int state)
{
	zval **params[1];
	params[0] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],state);
	call_user_callback(call_backs,GLUT_ENTRY_CALLBACK,1,params);
}

/* {{{ bool glutentryfunc(mixed callback) */
PHP_FUNCTION(glutentryfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_ENTRY_CALLBACK);
	glutEntryFunc(glutentryfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutspecialfunc_callback(int key,int x,int y)
{
	zval params[3];
	zval retval;
	char str[2];

	sprintf(str,"%c",key);

	ZVAL_LONG(&params[0], key);
	ZVAL_LONG(&params[1], x);
	ZVAL_LONG(&params[2], y);

	special_fci.param_count = 3;
	special_fci.params = params;
	special_fci.retval = &retval;

	if (zend_call_function(&special_fci, &special_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutspecialfunc(mixed callback) */
PHP_FUNCTION(glutspecialfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &special_fci, &special_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutSpecialFunc(glutspecialfunc_callback);
	RETURN_TRUE;
}
/* }}} */

void glutspaceballmotionfunc_callback(int x,int y,int z)
{
	zval *params[3];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	params[2] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],x);
	ZVAL_LONG(params[1],y);
	ZVAL_LONG(params[2],z);
	call_user_callback(call_backs,GLUT_SPACEBALLMOTION_CALLBACK,3,params);
}

/* {{{ bool glutspaceballmotionfunc(mixed callback) */
PHP_FUNCTION(glutspaceballmotionfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_SPACEBALLMOTION_CALLBACK);
	glutSpaceballMotionFunc(glutspaceballmotionfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutspaceballrotatefunc_callback(int x,int y,int z)
{
	zval *params[3];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	params[2] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],x);
	ZVAL_LONG(params[1],y);
	ZVAL_LONG(params[2],z);
	call_user_callback(call_backs,GLUT_SPACEBALLROTATE_CALLBACK,3,params);
}

/* {{{ bool glutspaceballrotatefunc(mixed callback) */
PHP_FUNCTION(glutspaceballrotatefunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_SPACEBALLROTATE_CALLBACK);
	glutSpaceballRotateFunc(glutspaceballmotionfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutspaceballbuttonfunc_callback(int button,int state)
{
	zval *params[2];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],button);
	ZVAL_LONG(params[1],state);
	call_user_callback(call_backs,GLUT_SPACEBALLBUTTON_CALLBACK,2,params);
}

/* {{{ bool glutspaceballbuttonfunc(mixed callback) */
PHP_FUNCTION(glutspaceballbuttonfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_SPACEBALLBUTTON_CALLBACK);
	glutSpaceballButtonFunc(glutspaceballbuttonfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutbuttonboxfunc_callback(int button,int state)
{
	zval *params[2];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],button);
	ZVAL_LONG(params[1],state);
	call_user_callback(call_backs,GLUT_BUTTONBOX_CALLBACK,2,params);
}

/* {{{ bool glutbuttonboxfunc(mixed callback) */
PHP_FUNCTION(glutbuttonboxfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_BUTTONBOX_CALLBACK);
	glutButtonBoxFunc(glutbuttonboxfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutdialsfunc_callback(int dial,int value)
{
	zval *params[2];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],dial);
	ZVAL_LONG(params[1],value);
	call_user_callback(call_backs,GLUT_DIALS_CALLBACK,2,params);
}

/* {{{ bool glutdialsfunc(mixed callback) */
PHP_FUNCTION(glutdialsfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_DIALS_CALLBACK);
	glutDialsFunc(glutdialsfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void gluttabletmotionfunc_callback(int x,int y)
{
	zval *params[2];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],x);
	ZVAL_LONG(params[1],y);
	call_user_callback(call_backs,GLUT_TABLETMOTION_CALLBACK,2,params);
}

/* {{{ bool gluttabletmotionfunc(mixed callback) */
PHP_FUNCTION(gluttabletmotionfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_TABLETMOTION_CALLBACK);
	glutTabletMotionFunc(gluttabletmotionfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void gluttabletbuttonfunc_callback(int button,int state,int x,int y)
{
	zval *params[4];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	params[2] = (zval *)emalloc(sizeof(zval));
	params[3] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],button);
	ZVAL_LONG(params[1],state);
	ZVAL_LONG(params[2],x);
	ZVAL_LONG(params[3],y);
	call_user_callback(call_backs,GLUT_TABLETBUTTON_CALLBACK,4,params);
}

/* {{{ bool gluttabletbuttonfunc(mixed callback) */
PHP_FUNCTION(gluttabletbuttonfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_TABLETBUTTON_CALLBACK);
	glutTabletButtonFunc(gluttabletbuttonfunc_callback);
	RETURN_TRUE;
}
/* }}} */



void glutmenustatusfunc_callback(int state,int x,int y)
{
	zval *params[3];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	params[2] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],state);
	ZVAL_LONG(params[1],x);
	ZVAL_LONG(params[2],y);
	call_user_callback(call_backs,GLUT_MENUSTATUS_CALLBACK,3,params);
}

/* {{{ bool glutmenustatusfunc(mixed callback) */
PHP_FUNCTION(glutmenustatusfunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_MENUSTATUS_CALLBACK);
	glutMenuStatusFunc(glutmenustatusfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutmenustatefunc_callback(int state)
{
	zval *params[1];
	params[0] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],state);
	call_user_callback(call_backs,GLUT_MENUSTATE_CALLBACK,1,params);
}

/* {{{ bool glutmenustatefunc(mixed callback) */
PHP_FUNCTION(glutmenustatefunc)
{
	zval *callback;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callback) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 1, GLUT_MENUSTATE_CALLBACK);
	glutMenuStateFunc(glutmenustatefunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutidlefunc_callback()
{
	zval retval;

	idle_fci.param_count = 0;
	idle_fci.params = NULL;
	idle_fci.retval = &retval;

	if (zend_call_function(&idle_fci, &idle_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutidlefunc(mixed callback) */
PHP_FUNCTION(glutidlefunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &idle_fci, &idle_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutIdleFunc(glutidlefunc_callback);
	RETURN_TRUE;
}
/* }}} */


void gluttimerfunc_callback(int value)
{
	zval *params[1];
	params[0] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],value);
	call_user_callback(call_backs,GLUT_TIMER_CALLBACK,1,params);
}

/* {{{ void gluttimerfunc(long seconds, mixed callback, long value) */
PHP_FUNCTION(gluttimerfunc)
{
	zval *callback;
	long seconds, value;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lzl", &seconds, &callback, &value) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	HASH_CALLBACK(callback, 2, GLUT_VISIBILITY_CALLBACK);
	glutTimerFunc((seconds),gluttimerfunc_callback,(value));
}
/* }}} */


/* {{{ void glutsetcolor(long cell, double red, double green, double blue) */
PHP_FUNCTION(glutsetcolor)
{
	long cell, red, green, blue;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &cell, &red, &green, &blue) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutSetColor(cell,(red),(green),(blue));
}
/* }}} */


/* {{{ double glutgetcolor(long cell, long component) */
PHP_FUNCTION(glutgetcolor)
{
	long cell, component;
	GLfloat color;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &cell, &component) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	color = glutGetColor(cell,component);
	RETURN_DOUBLE(color);
}
/* }}} */


/* {{{ void glutcopycolormap(long win) */
PHP_FUNCTION(glutcopycolormap)
{
	long win;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &win) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutCopyColormap(win);
}
/* }}} */


/* {{{ void glutget(long state) */
PHP_FUNCTION(glutget)
{
	long state;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &state) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutGet(state);
}
/* }}} */


/* {{{ void glutlayerget(long info) */
PHP_FUNCTION(glutlayerget)
{
	long info;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &info) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutLayerGet(info);
}
/* }}} */


/* {{{ void glutdeviceget(long info) */
PHP_FUNCTION(glutdeviceget)
{
	long info;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &info) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutDeviceGet(info);
}
/* }}} */


/* {{{ long glutgetmodifiers() */
PHP_FUNCTION(glutgetmodifiers)
{
	int mod;
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	mod = glutGetModifiers();
	RETURN_LONG(mod);
}
/* }}} */


/* {{{ long glutextensionsupported(string ext) */
PHP_FUNCTION(glutextensionsupported)
{
	char *ext = NULL;
	size_t ext_len;
	int ret;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ext, &ext_len) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	ret = glutExtensionSupported(ext);
	RETURN_LONG(ret);
}
/* }}} */


/* {{{ void glutbitmapcharacter(long font, string chr) */
PHP_FUNCTION(glutbitmapcharacter)
{
	long font;
	char *chr = NULL;
	size_t chr_len;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &font, &chr, &chr_len) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}
	glutBitmapCharacter((void*)font,chr[0]);
}
/* }}} */


/* {{{ long glutbitmapwidth(long font, string chr) */
PHP_FUNCTION(glutbitmapwidth)
{
	int width;
	long font;
	char *chr = NULL;
	size_t chr_len;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &font, &chr, &chr_len) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	width = glutBitmapWidth((void*)font,chr[0]);
	RETURN_LONG(width);
}
/* }}} */


/* {{{ void glutstrokecharacter(long font, string chr) */
PHP_FUNCTION(glutstrokecharacter)
{
	long font;
	zend_string* chr;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lS", &font, &chr) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutStrokeCharacter((void*)font,chr->val[0]);
}
/* }}} */


/* {{{ long glutstrokewidth(long font, string chr) */
PHP_FUNCTION(glutstrokewidth)
{
	int ret;
	long font;
	char *chr = NULL;
	size_t chr_len;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &font, &chr, &chr_len) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ret = glutStrokeWidth((void*)font,chr[0]);
	RETURN_LONG(ret);
}
/* }}} */


/* {{{ void glutsolidsphere(double radius, long slices, long stacks) */
PHP_FUNCTION(glutsolidsphere)
{
	double radius;
	long slices, stacks;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll", &radius, &slices, &stacks) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutSolidSphere((radius),(slices),(stacks));
}
/* }}} */


/* {{{ void glutwiresphere(double radius, long slices, long stacks) */
PHP_FUNCTION(glutwiresphere)
{
	double radius;
	long slices, stacks;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll", &radius, &slices, &stacks) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutWireSphere((radius),(slices),(stacks));
}
/* }}} */


/* {{{ void glutsolidcube(double size) */
PHP_FUNCTION(glutsolidcube)
{
	double size;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &size) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutSolidCube(size);
}
/* }}} */


/* {{{ void glutwirecube(double size) */
PHP_FUNCTION(glutwirecube)
{
	double size;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &size) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutWireCube(size);
}
/* }}} */


/* {{{ void glutsolidcone(double base, doable height, long slices, long stacks) */
PHP_FUNCTION(glutsolidcone)
{
	double base, height;
	long slices, stacks;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddll", &base , &height, &slices, &stacks) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutSolidCone((base),(height),(slices),(stacks));
}
/* }}} */


/* {{{ void glutwirecone(double base, double height, long slices, long stacks) */
PHP_FUNCTION(glutwirecone)
{
	double base, height;
	long slices, stacks;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddll", &base, &height, &slices, &stacks) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutWireCone((base),(height),(slices),(stacks));
}
/* }}} */


/* {{{ void glutsolidtorus(double innerRadius, double outerRadius, long nsides, long rings) */
PHP_FUNCTION(glutsolidtorus)
{
	double innerRadius, outerRadius;
	long nsides, rings;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddll", &innerRadius, &outerRadius, &nsides, &rings) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutSolidTorus((innerRadius),(outerRadius),(nsides),(rings));
}
/* }}} */


/* {{{ void glutwiretorus(double innerRadius, double outerRadius, long nsides, long rings) */
PHP_FUNCTION(glutwiretorus)
{
	double innerRadius, outerRadius;
	long nsides, rings;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddll", &innerRadius, &outerRadius, &nsides, &rings) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	glutWireTorus((innerRadius),(outerRadius),(nsides),(rings));
}
/* }}} */


/* {{{ void glutsoliddodecahedron() */
PHP_FUNCTION(glutsoliddodecahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutSolidDodecahedron();
}
/* }}} */


/* {{{ void glutwiredodecahedron() */
PHP_FUNCTION(glutwiredodecahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutWireDodecahedron();
}
/* }}} */


/* {{{ void glutsolidoctahedron() */
PHP_FUNCTION(glutsolidoctahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutSolidOctahedron();
}
/* }}} */


/* {{{ void glutwireoctahedron() */
PHP_FUNCTION(glutwireoctahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutWireOctahedron();
}
/* }}} */


/* {{{ void glutsolidtetrahedron() */
PHP_FUNCTION(glutsolidtetrahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutSolidTetrahedron();
}
/* }}} */


/* {{{ void glutwiretetrahedron() */
PHP_FUNCTION(glutwiretetrahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutWireTetrahedron();
}
/* }}} */


/* {{{ void glutsolidicosahedron() */
PHP_FUNCTION(glutsolidicosahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutSolidIcosahedron();
}
/* }}} */


/* {{{ void glutwireicosahedron() */
PHP_FUNCTION(glutwireicosahedron)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	glutWireIcosahedron();
}
/* }}} */


/* {{{ void glutsolidteapot(double size) */
PHP_FUNCTION(glutsolidteapot)
{
	double size;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &size) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutSolidTeapot(size);
}
/* }}} */


/* {{{ void glutwireteapot(double size) */
PHP_FUNCTION(glutwireteapot)
{
	double size;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &size) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

	glutWireTeapot(size);
}
/* }}} */

/* {{{ void glutwireteapot(double size) */
PHP_FUNCTION(glutSetOption)
{
    zend_long what, value;
	if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &what, &value) == FAILURE ) {
		WRONG_PARAM_COUNT;
	}

    glutSetOption(what, value);
}

/* {{{ void glutwireteapot(double size) */
PHP_FUNCTION(glutLeaveMainLoop)
{
	if( zend_parse_parameters_none() == FAILURE) {
		WRONG_PARAM_COUNT;
	}

    glutLeaveMainLoop(); 
}


const zend_function_entry glut_functions[] = {
    ZEND_FE(glutinitcontextversion, arginfo_glutinitcontextversion)
    ZEND_FE(glutInitContextProfile, NULL)
    ZEND_FE(glutInitContextFlags, NULL)
	ZEND_FE(glutinit,NULL)
	ZEND_FE(glutinitwindowsize,NULL)
	ZEND_FE(glutinitwindowposition,NULL)
	ZEND_FE(glutinitdisplaymode,NULL)
	ZEND_FE(glutmainloop,NULL)
	ZEND_FE(glutcreatewindow,NULL)
	ZEND_FE(glutcreatesubwindow,NULL)
	ZEND_FE(glutsetwindow,NULL)
	ZEND_FE(glutgetwindow,NULL)
	ZEND_FE(glutdisplayfunc,NULL)
	ZEND_FE(glutdestroywindow,NULL)
	ZEND_FE(glutpostredisplay,NULL)
	ZEND_FE(glutswapbuffers,NULL)
	ZEND_FE(glutpositionwindow,NULL)
	ZEND_FE(glutreshapewindow,NULL)
	ZEND_FE(glutfullscreen,NULL)
	ZEND_FE(glutpopwindow,NULL)
	ZEND_FE(glutpushwindow,NULL)
	ZEND_FE(glutshowwindow,NULL)
	ZEND_FE(gluthidewindow,NULL)
	ZEND_FE(gluticonifywindow,NULL)
	ZEND_FE(glutsetwindowtitle,NULL)
	ZEND_FE(glutseticontitle,NULL)
	ZEND_FE(glutsetcursor,NULL)
	ZEND_FE(glutwarppointer,NULL)
	ZEND_FE(glutestablishoverlay,NULL)
	ZEND_FE(glutuselayer,NULL)
	ZEND_FE(glutremoveoverlay,NULL)
	ZEND_FE(glutpostoverlayredisplay,NULL)
	ZEND_FE(glutcreatemenu,NULL)
	ZEND_FE(glutaddmenuentry,NULL)
	ZEND_FE(glutsetmenu,NULL)
	ZEND_FE(glutgetmenu,NULL)
	ZEND_FE(glutdestroymenu,NULL)
	ZEND_FE(glutaddsubmenu,NULL)
	ZEND_FE(glutchangetomenuentry,NULL)
	ZEND_FE(glutchangetosubmenu,NULL)
	ZEND_FE(glutremovemenuitem,NULL)
	ZEND_FE(glutattachmenu,NULL)
	ZEND_FE(glutdetachmenu,NULL)
	ZEND_FE(glutoverlaydisplayfunc,NULL)
	ZEND_FE(glutreshapefunc,NULL)
	ZEND_FE(glutkeyboardfunc,NULL)
	ZEND_FE(glutkeyboardupfunc,NULL)
	ZEND_FE(glutmousefunc,NULL)
	ZEND_FE(glutmotionfunc,NULL)
	ZEND_FE(glutignorekeyrepeat,NULL)
	ZEND_FE(glutpassivemotionfunc,NULL)
	ZEND_FE(glutvisibilityfunc,NULL)
	ZEND_FE(glutentryfunc,NULL)
	ZEND_FE(glutspecialfunc,NULL)
	ZEND_FE(glutspaceballmotionfunc,NULL)
	ZEND_FE(glutspaceballrotatefunc,NULL)
	ZEND_FE(glutspaceballbuttonfunc,NULL)
	ZEND_FE(glutbuttonboxfunc,NULL)
	ZEND_FE(glutdialsfunc,NULL)
	ZEND_FE(gluttabletmotionfunc,NULL)
	ZEND_FE(gluttabletbuttonfunc,NULL)
	ZEND_FE(glutidlefunc,NULL)
	ZEND_FE(glutsetcolor,NULL)
	ZEND_FE(glutgetcolor,NULL)
	ZEND_FE(glutcopycolormap,NULL)
	ZEND_FE(glutget,NULL)
	ZEND_FE(glutlayerget,NULL)
	ZEND_FE(glutdeviceget,NULL)
	ZEND_FE(glutgetmodifiers,NULL)
	ZEND_FE(glutextensionsupported,NULL)
	ZEND_FE(glutbitmapcharacter,NULL)
	ZEND_FE(glutbitmapwidth,NULL)
	ZEND_FE(glutstrokecharacter,NULL)
	ZEND_FE(glutstrokewidth,NULL)
	ZEND_FE(glutsolidsphere,NULL)
	ZEND_FE(glutwiresphere,NULL)
	ZEND_FE(glutsolidcube,NULL)
	ZEND_FE(glutwirecube,NULL)
	ZEND_FE(glutsolidcone,NULL)
	ZEND_FE(glutwirecone,NULL)
	ZEND_FE(glutsolidtorus,NULL)
	ZEND_FE(glutwiretorus,NULL)
	ZEND_FE(glutsoliddodecahedron,NULL)
	ZEND_FE(glutwiredodecahedron,NULL)
	ZEND_FE(glutsolidoctahedron,NULL)
	ZEND_FE(glutwireoctahedron,NULL)
	ZEND_FE(glutsolidtetrahedron,NULL)
	ZEND_FE(glutwiretetrahedron,NULL)
	ZEND_FE(glutsolidicosahedron,NULL)
	ZEND_FE(glutwireicosahedron,NULL)
	ZEND_FE(glutsolidteapot,NULL)
	ZEND_FE(glutwireteapot,NULL)
	ZEND_FE(glutSetOption,NULL)
	ZEND_FE(glutLeaveMainLoop,NULL)
    ZEND_FE(glutMouseWheelFunc, arginfo_glutMouseWheelFunc)
	ZEND_FE_END
};

//glutMainLoopEvent

#ifdef COMPILE_DL_GLUT
ZEND_GET_MODULE(glut)
#endif

PHP_MINIT_FUNCTION(glut)
{
	/*Glut Init Constants */
	REGISTER_LONG_CONSTANT("GLUT_RGBA", GLUT_RGBA, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_RGB", GLUT_RGB, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_INDEX", GLUT_INDEX, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_SINGLE", GLUT_SINGLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_DOUBLE", GLUT_DOUBLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_ACCUM", GLUT_ACCUM, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_ALPHA", GLUT_ALPHA, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_DEPTH", GLUT_DEPTH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_STENCIL", GLUT_STENCIL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_MULTISAMPLE", GLUT_MULTISAMPLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_STEREO", GLUT_STEREO, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_LUMINANCE", GLUT_LUMINANCE, CONST_CS | CONST_PERSISTENT);
	/*Glut Cursor Constants */
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_RIGHT_ARROW", GLUT_CURSOR_RIGHT_ARROW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_LEFT_ARROW", GLUT_CURSOR_LEFT_ARROW, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_INFO", GLUT_CURSOR_INFO, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_DESTROY", GLUT_CURSOR_DESTROY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_HELP", GLUT_CURSOR_HELP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_CYCLE", GLUT_CURSOR_CYCLE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_SPRAY", GLUT_CURSOR_SPRAY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_WAIT", GLUT_CURSOR_WAIT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_TEXT", GLUT_CURSOR_TEXT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_CROSSHAIR", GLUT_CURSOR_CROSSHAIR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_UP_DOWN", GLUT_CURSOR_UP_DOWN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_LEFT_RIGHT", GLUT_CURSOR_LEFT_RIGHT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_TOP_SIDE", GLUT_CURSOR_TOP_SIDE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_BOTTOM_SIDE", GLUT_CURSOR_BOTTOM_SIDE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_LEFT_SIDE", GLUT_CURSOR_LEFT_SIDE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_RIGHT_SIDE", GLUT_CURSOR_RIGHT_SIDE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_TOP_LEFT_CORNER", GLUT_CURSOR_TOP_LEFT_CORNER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_TOP_RIGHT_CORNER", GLUT_CURSOR_TOP_RIGHT_CORNER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_BOTTOM_RIGHT_CORNER", GLUT_CURSOR_BOTTOM_RIGHT_CORNER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_FULL_CROSSHAIR", GLUT_CURSOR_FULL_CROSSHAIR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_NONE", GLUT_CURSOR_NONE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_CURSOR_INHERIT", GLUT_CURSOR_INHERIT, CONST_CS | CONST_PERSISTENT);
	/*Glut Layer Constants */
	REGISTER_LONG_CONSTANT("GLUT_NORMAL", GLUT_NORMAL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_OVERLAY", GLUT_OVERLAY, CONST_CS | CONST_PERSISTENT);
	/*Glut Button Contants */
	REGISTER_LONG_CONSTANT("GLUT_LEFT_BUTTON", GLUT_LEFT_BUTTON, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_MIDDLE_BUTTON", GLUT_MIDDLE_BUTTON, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_RIGHT_BUTTON", GLUT_RIGHT_BUTTON, CONST_CS | CONST_PERSISTENT);
	/*Glut Visibilty Constants */
	REGISTER_LONG_CONSTANT("GLUT_NOT_VISIBLE", GLUT_NOT_VISIBLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_VISIBLE", GLUT_VISIBLE , CONST_CS | CONST_PERSISTENT);
	/*Glut Key Constants */
	REGISTER_LONG_CONSTANT("GLUT_KEY_F1", GLUT_KEY_F1 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F2", GLUT_KEY_F2 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F3", GLUT_KEY_F3 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F4", GLUT_KEY_F4 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F5", GLUT_KEY_F5 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F6", GLUT_KEY_F6 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F7", GLUT_KEY_F7 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F8", GLUT_KEY_F8 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F9", GLUT_KEY_F9 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F10", GLUT_KEY_F10 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F11", GLUT_KEY_F11 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_F12", GLUT_KEY_F12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_LEFT", GLUT_KEY_LEFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_UP", GLUT_KEY_UP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_RIGHT", GLUT_KEY_RIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_DOWN", GLUT_KEY_DOWN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_PAGE_UP", GLUT_KEY_PAGE_UP , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_PAGE_DOWN", GLUT_KEY_PAGE_DOWN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_HOME", GLUT_KEY_HOME , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_END", GLUT_KEY_END , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_KEY_INSERT", GLUT_KEY_INSERT , CONST_CS | CONST_PERSISTENT);
	/*Glut Menu Status Constants */
	REGISTER_LONG_CONSTANT("GLUT_MENU_IN_USE", GLUT_MENU_IN_USE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_MENU_NOT_IN_USE", GLUT_MENU_NOT_IN_USE , CONST_CS | CONST_PERSISTENT);
	/*Glut Color Constants */
	REGISTER_LONG_CONSTANT("GLUT_RED", GLUT_RED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_GREEN", GLUT_GREEN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_BLUE", GLUT_BLUE , CONST_CS | CONST_PERSISTENT);
	/*Glut Get Attribute Constants */
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_X", GLUT_WINDOW_X , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_Y", GLUT_WINDOW_Y , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_WIDTH", GLUT_WINDOW_WIDTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_HEIGHT", GLUT_WINDOW_HEIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_BUFFER_SIZE", GLUT_WINDOW_BUFFER_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_STENCIL_SIZE", GLUT_WINDOW_STENCIL_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_DEPTH_SIZE", GLUT_WINDOW_DEPTH_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_RED_SIZE", GLUT_WINDOW_RED_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_GREEN_SIZE", GLUT_WINDOW_GREEN_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_BLUE_SIZE", GLUT_WINDOW_BLUE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_ALPHA_SIZE", GLUT_WINDOW_ALPHA_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_ACCUM_RED_SIZE", GLUT_WINDOW_ACCUM_RED_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_ACCUM_GREEN_SIZE", GLUT_WINDOW_ACCUM_GREEN_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_ACCUM_BLUE_SIZE", GLUT_WINDOW_ACCUM_BLUE_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_ACCUM_ALPHA_SIZE", GLUT_WINDOW_ACCUM_ALPHA_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_DOUBLEBUFFER", GLUT_WINDOW_DOUBLEBUFFER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_RGBA", GLUT_WINDOW_RGBA , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_PARENT", GLUT_WINDOW_PARENT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_NUM_CHILDREN", GLUT_WINDOW_NUM_CHILDREN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_COLORMAP_SIZE", GLUT_WINDOW_COLORMAP_SIZE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_NUM_SAMPLES", GLUT_WINDOW_NUM_SAMPLES , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_STEREO", GLUT_WINDOW_STEREO , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_WINDOW_CURSOR", GLUT_WINDOW_CURSOR , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_SCREEN_WIDTH", GLUT_SCREEN_WIDTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_SCREEN_HEIGHT", GLUT_SCREEN_HEIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_SCREEN_WIDTH_MM", GLUT_SCREEN_WIDTH_MM , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_SCREEN_HEIGHT_MM", GLUT_SCREEN_HEIGHT_MM , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_MENU_NUM_ITEMS", GLUT_MENU_NUM_ITEMS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_DISPLAY_MODE_POSSIBLE", GLUT_DISPLAY_MODE_POSSIBLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_INIT_DISPLAY_MODE", GLUT_INIT_DISPLAY_MODE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_INIT_WINDOW_X", GLUT_INIT_WINDOW_X , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_INIT_WINDOW_Y", GLUT_INIT_WINDOW_Y , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_INIT_WINDOW_WIDTH", GLUT_INIT_WINDOW_WIDTH , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_INIT_WINDOW_HEIGHT", GLUT_INIT_WINDOW_HEIGHT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_ELAPSED_TIME", GLUT_ELAPSED_TIME , CONST_CS | CONST_PERSISTENT);
	/*Glut Layer Constants */
	REGISTER_LONG_CONSTANT("GLUT_OVERLAY_POSSIBLE", GLUT_OVERLAY_POSSIBLE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_LAYER_IN_USE", GLUT_LAYER_IN_USE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_HAS_OVERLAY", GLUT_HAS_OVERLAY , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_TRANSPARENT_INDEX", GLUT_TRANSPARENT_INDEX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_NORMAL_DAMAGED", GLUT_NORMAL_DAMAGED , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_OVERLAY_DAMAGED", GLUT_OVERLAY_DAMAGED , CONST_CS | CONST_PERSISTENT);
	/*Glut Device Constants */
	REGISTER_LONG_CONSTANT("GLUT_DOWN", GLUT_DOWN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_UP", GLUT_UP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_HAS_KEYBOARD", GLUT_HAS_KEYBOARD , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_HAS_MOUSE", GLUT_HAS_MOUSE , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_HAS_SPACEBALL", GLUT_HAS_SPACEBALL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_HAS_DIAL_AND_BUTTON_BOX", GLUT_HAS_DIAL_AND_BUTTON_BOX , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_HAS_TABLET", GLUT_HAS_TABLET , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_NUM_MOUSE_BUTTONS", GLUT_NUM_MOUSE_BUTTONS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_NUM_SPACEBALL_BUTTONS", GLUT_NUM_SPACEBALL_BUTTONS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_NUM_BUTTON_BOX_BUTTONS", GLUT_NUM_BUTTON_BOX_BUTTONS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_NUM_DIALS", GLUT_NUM_DIALS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_NUM_TABLET_BUTTONS", GLUT_NUM_TABLET_BUTTONS , CONST_CS | CONST_PERSISTENT);
	/*Glut Modifyer Constants */
	REGISTER_LONG_CONSTANT("GLUT_ACTIVE_SHIFT", GLUT_ACTIVE_SHIFT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_ACTIVE_CTRL", GLUT_ACTIVE_CTRL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_ACTIVE_ALT", GLUT_ACTIVE_ALT , CONST_CS | CONST_PERSISTENT);
	/*Glut Font Constants */
	REGISTER_LONG_CONSTANT("GLUT_BITMAP_8_BY_13", (long)GLUT_BITMAP_8_BY_13 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_BITMAP_9_BY_15", (long)GLUT_BITMAP_9_BY_15 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_BITMAP_TIMES_ROMAN_10", (long)GLUT_BITMAP_TIMES_ROMAN_10 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_BITMAP_TIMES_ROMAN_24", (long)GLUT_BITMAP_TIMES_ROMAN_24 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_BITMAP_HELVETICA_10", (long)GLUT_BITMAP_HELVETICA_10 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_BITMAP_HELVETICA_12", (long)GLUT_BITMAP_HELVETICA_12 , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_BITMAP_HELVETICA_18", (long)GLUT_BITMAP_HELVETICA_18 , CONST_CS | CONST_PERSISTENT);
	/*Glut Stroke Constants */
	REGISTER_LONG_CONSTANT("GLUT_STROKE_ROMAN", (long)GLUT_STROKE_ROMAN , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GLUT_STROKE_MONO_ROMAN", (long)GLUT_STROKE_MONO_ROMAN , CONST_CS | CONST_PERSISTENT);
#ifdef GLUT_3_2_CORE_PROFILE
	REGISTER_LONG_CONSTANT("GLUT_3_2_CORE_PROFILE", (long)GLUT_3_2_CORE_PROFILE, CONST_CS | CONST_PERSISTENT);
#endif
        
#ifdef GLUT_FORWARD_COMPATIBLE
        REGISTER_LONG_CONSTANT("GLUT_FORWARD_COMPATIBLE", (long)GLUT_FORWARD_COMPATIBLE , CONST_CS | CONST_PERSISTENT);
#endif
#ifdef GLUT_CORE_PROFILE
        REGISTER_LONG_CONSTANT("GLUT_CORE_PROFILE", (long)GLUT_CORE_PROFILE , CONST_CS | CONST_PERSISTENT);
#endif
    REGISTER_LONG_CONSTANT("GLUT_DEBUG", (long)GLUT_DEBUG, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GLUT_ACTION_ON_WINDOW_CLOSE", (long)GLUT_ACTION_ON_WINDOW_CLOSE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("GLUT_ACTION_GLUTMAINLOOP_RETURNS", (long)GLUT_ACTION_GLUTMAINLOOP_RETURNS, CONST_CS | CONST_PERSISTENT);

	call_backs = (HashTable*)emalloc(sizeof(HashTable));
/*	zend_hash_init(call_backs, 0, NULL, ZVAL_PTR_DTOR, 0); */
	
	ALLOC_HASHTABLE( menu_entry_callbacks  );
	zend_hash_init(menu_entry_callbacks, 0, NULL, ZVAL_PTR_DTOR, 0);

	return (zend_register_functions(NULL, glut_functions, NULL, MODULE_PERSISTENT TSRMLS_CC));
}

PHP_MINFO_FUNCTION(glut)
{
        php_info_print_table_start();
        php_info_print_table_header(2, "GLUT support", "enabled");
        php_info_print_table_row(2, "GLUT API version", STRINGIFY(GLUT_API_VERSION));
        php_info_print_table_end();
}
