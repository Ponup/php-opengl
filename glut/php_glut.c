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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <GL/glut.h>

#include "php.h"
#include "php_glut.h"
#include "php_convert.h"

static HashTable *call_backs;

static HashTable *menu_callbacks;
static HashTable *menu_entry_callbacks;
static unsigned long unique_menu_entry_id = 0;

/* Callback functions (TODO: Make an static array with all possible callbacks) */

/* GLUT_DISPLAY_CALLBACK */
zend_fcall_info display_fci;
zend_fcall_info_cache display_fci_cache;

/* GLUT_RESHAPE_CALLBACK */
zend_fcall_info reshape_fci;
zend_fcall_info_cache reshape_fci_cache;

/* */
zend_fcall_info keyboard_fci;
zend_fcall_info_cache keyboard_fci_cache;
	
/* GLUT_IDLE_CALLBACK */
zend_fcall_info idle_fci;
zend_fcall_info_cache idle_fci_cache;

/* GLUT_VISIBILITY_CALLBACK */
zend_fcall_info visibility_fci;
zend_fcall_info_cache visibility_fci_cache;

const zend_function_entry glut_functions[] = {
	PHP_FE(glutinit,NULL)
	PHP_FE(glutinitwindowsize,NULL)
	PHP_FE(glutinitwindowposition,NULL)
	PHP_FE(glutinitdisplaymode,NULL)
	PHP_FE(glutmainloop,NULL)
	PHP_FE(glutcreatewindow,NULL)
	PHP_FE(glutcreatesubwindow,NULL)
	PHP_FE(glutsetwindow,NULL)
	PHP_FE(glutgetwindow,NULL)
	PHP_FE(glutdisplayfunc,NULL)
	PHP_FE(glutdestroywindow,NULL)
	PHP_FE(glutpostredisplay,NULL)
	PHP_FE(glutswapbuffers,NULL)
	PHP_FE(glutpositionwindow,NULL)
	PHP_FE(glutreshapewindow,NULL)
	PHP_FE(glutfullscreen,NULL)
	PHP_FE(glutpopwindow,NULL)
	PHP_FE(glutpushwindow,NULL)
	PHP_FE(glutshowwindow,NULL)
	PHP_FE(gluthidewindow,NULL)
	PHP_FE(gluticonifywindow,NULL)
	PHP_FE(glutsetwindowtitle,NULL)
	PHP_FE(glutseticontitle,NULL)
	PHP_FE(glutsetcursor,NULL)
	PHP_FE(glutestablishoverlay,NULL)
	PHP_FE(glutuselayer,NULL)
	PHP_FE(glutremoveoverlay,NULL)
	PHP_FE(glutpostoverlayredisplay,NULL)
	PHP_FE(glutcreatemenu,NULL)
	PHP_FE(glutaddmenuentry,NULL)
	PHP_FE(glutsetmenu,NULL)
	PHP_FE(glutgetmenu,NULL)
	PHP_FE(glutdestroymenu,NULL)
	PHP_FE(glutaddsubmenu,NULL)
	PHP_FE(glutchangetomenuentry,NULL)
	PHP_FE(glutchangetosubmenu,NULL)
	PHP_FE(glutremovemenuitem,NULL)
	PHP_FE(glutattachmenu,NULL)
	PHP_FE(glutdetachmenu,NULL)
	PHP_FE(glutoverlaydisplayfunc,NULL)
	PHP_FE(glutreshapefunc,NULL)
	PHP_FE(glutkeyboardfunc,NULL)
	PHP_FE(glutmousefunc,NULL)
	PHP_FE(glutmotionfunc,NULL)
	PHP_FE(glutpassivemotionfunc,NULL)
	PHP_FE(glutvisibilityfunc,NULL)
	PHP_FE(glutentryfunc,NULL)
	PHP_FE(glutspecialfunc,NULL)
	PHP_FE(glutspaceballmotionfunc,NULL)
	PHP_FE(glutspaceballrotatefunc,NULL)
	PHP_FE(glutspaceballbuttonfunc,NULL)
	PHP_FE(glutbuttonboxfunc,NULL)
	PHP_FE(glutdialsfunc,NULL)
	PHP_FE(gluttabletmotionfunc,NULL)
	PHP_FE(gluttabletbuttonfunc,NULL)
	PHP_FE(glutidlefunc,NULL)
	PHP_FE(glutsetcolor,NULL)
	PHP_FE(glutgetcolor,NULL)
	PHP_FE(glutcopycolormap,NULL)
	PHP_FE(glutget,NULL)
	PHP_FE(glutlayerget,NULL)
	PHP_FE(glutdeviceget,NULL)
	PHP_FE(glutgetmodifiers,NULL)
	PHP_FE(glutextensionsupported,NULL)
	PHP_FE(glutbitmapcharacter,NULL)
	PHP_FE(glutbitmapwidth,NULL)
	PHP_FE(glutstrokecharacter,NULL)
	PHP_FE(glutstrokewidth,NULL)
	PHP_FE(glutsolidsphere,NULL)
	PHP_FE(glutwiresphere,NULL)
	PHP_FE(glutsolidcube,NULL)
	PHP_FE(glutwirecube,NULL)
	PHP_FE(glutsolidcone,NULL)
	PHP_FE(glutwirecone,NULL)
	PHP_FE(glutsolidtorus,NULL)
	PHP_FE(glutwiretorus,NULL)
	PHP_FE(glutsoliddodecahedron,NULL)
	PHP_FE(glutwiredodecahedron,NULL)
	PHP_FE(glutsolidoctahedron,NULL)
	PHP_FE(glutwireoctahedron,NULL)
	PHP_FE(glutsolidtetrahedron,NULL)
	PHP_FE(glutwiretetrahedron,NULL)
	PHP_FE(glutsolidicosahedron,NULL)
	PHP_FE(glutwireicosahedron,NULL)
	PHP_FE(glutsolidteapot,NULL)
	PHP_FE(glutwireteapot,NULL)

	{NULL, NULL, NULL}
};


zend_module_entry glut_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  "Glut functions", 
  glut_functions, 
  PHP_MINIT(glut), 
  NULL,
  NULL,
  NULL,
  PHP_MINFO(glut),
#if ZEND_MODULE_API_NO >= 20010901
  PHP_GLUT_VERSION,
#endif
  STANDARD_MODULE_PROPERTIES,
};

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
		
	call_backs = (HashTable*)emalloc(sizeof(HashTable));
	zend_hash_init(call_backs, 0, NULL, ZVAL_PTR_DTOR, 0);
	
	menu_callbacks = (HashTable*)emalloc(sizeof(HashTable));
	zend_hash_init(menu_callbacks, 0, NULL, ZVAL_PTR_DTOR, 0);
	
	ALLOC_HASHTABLE( menu_entry_callbacks  );
	zend_hash_init(menu_entry_callbacks, 0, NULL, ZVAL_PTR_DTOR, 0);

	return SUCCESS;
}

PHP_MINFO_FUNCTION(glut)
{
	php_printf("Glut support enabled");
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
	glutInit(&argc, argv);
}
/* }}} */


/* {{{ void glutinitwindowsize(long width, long height) */
PHP_FUNCTION(glutinitwindowsize)
{
	zval *width,*height;
	TWO_PARAM(width,height);
	convert_to_long(width);
	convert_to_long(height);
	glutInitWindowSize(Z_LVAL_P(width),Z_LVAL_P(height));
}
/* }}} */



/* {{{ void glutinitwindowposition(long x, long y) */
PHP_FUNCTION(glutinitwindowposition)
{
	zval *x,*y;
	TWO_PARAM(x,y);
	convert_to_long(x);
	convert_to_long(y);
	glutInitWindowPosition(Z_LVAL_P(x),Z_LVAL_P(y));
}
/* }}} */


/* {{{ void glutinitdisplaymode(long disp) */
PHP_FUNCTION(glutinitdisplaymode)
{
	zval *disp;
	ONE_PARAM(disp);
	convert_to_long(disp);
	glutInitDisplayMode((unsigned int)Z_LVAL_P(disp));
}
/* }}} */


/* {{{ void glutmainloop() */
PHP_FUNCTION(glutmainloop)
{
	ZERO_PARAM();
	glutMainLoop();
}
/* }}} */


/* {{{ long glutcreatewindow(string name) */
PHP_FUNCTION(glutcreatewindow)
{
	zval *name;
	int win;
	ONE_PARAM(name);
	convert_to_string(name);
	win = glutCreateWindow(Z_STRVAL_P(name));
	RETURN_LONG(win);
}
/* }}} */


/* {{{ void glutcreatesubwindow(long win, long x, long y, long width, long height) */
PHP_FUNCTION(glutcreatesubwindow)
{
	zval *win,*x,*y,*width,*height;
	FIVE_PARAM(win,x,y,width,height);
	convert_to_long(win);
	convert_to_long(x);
	convert_to_long(y);
	convert_to_long(width);
	convert_to_long(height);
	glutCreateSubWindow(Z_LVAL_P(win),Z_LVAL_P(x),Z_LVAL_P(y),Z_LVAL_P(width),Z_LVAL_P(height));
}
/* }}} */


/* {{{ void glutsetwindow(long win) */
PHP_FUNCTION(glutsetwindow)
{
	zval *win;
	ONE_PARAM(win);
	convert_to_long(win);
	glutSetWindow(Z_LVAL_P(win));
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
	zval *win;
	ONE_PARAM(win);
	convert_to_long(win);
	glutDestroyWindow(Z_LVAL_P(win));
}
/* }}} */


/* {{{ void glutpostredisplay() */
PHP_FUNCTION(glutpostredisplay)
{
	ZERO_PARAM();
	glutPostRedisplay();
}
/* }}} */


/* {{{ void glutswapbuffers() */
PHP_FUNCTION(glutswapbuffers)
{
	ZERO_PARAM();
	glutSwapBuffers();
}
/* }}} */


/* {{{ void glutpositionwindow(long x, long y) */
PHP_FUNCTION(glutpositionwindow)
{
	zval *x,*y;
	TWO_PARAM(x,y);
	convert_to_long(x);
	convert_to_long(y);
	glutPositionWindow(Z_LVAL_P(x),Z_LVAL_P(y));
}
/* }}} */


/* {{{ void glutreshapewindow(long width, long height) */
PHP_FUNCTION(glutreshapewindow)
{
	zval *width,*height;
	TWO_PARAM(width,height);
	convert_to_long(width);
	convert_to_long(height);
	glutPositionWindow(Z_LVAL_P(width),Z_LVAL_P(height));
}
/* }}} */


/* {{{ void glutfullscreen() */
PHP_FUNCTION(glutfullscreen)
{
	ZERO_PARAM();
	glutFullScreen();
}
/* }}} */


/* {{{ void glutpopwindow() */
PHP_FUNCTION(glutpopwindow)
{
	ZERO_PARAM();
	glutPopWindow();
}
/* }}} */


/* {{{ void glutpushwindow() */
PHP_FUNCTION(glutpushwindow)
{
	ZERO_PARAM();
	glutPushWindow();
}
/* }}} */


/* {{{ void glutshowwindow() */
PHP_FUNCTION(glutshowwindow)
{
	ZERO_PARAM();
	glutShowWindow();
}
/* }}} */


/* {{{ void gluthidewindow() */
PHP_FUNCTION(gluthidewindow)
{
	ZERO_PARAM();
	glutHideWindow();
}
/* }}} */


/* {{{ void gluticonifywindow() */
PHP_FUNCTION(gluticonifywindow)
{
	ZERO_PARAM();
	glutIconifyWindow();
}
/* }}} */


/* {{{ void glutsetwindowtitle(string name) */
PHP_FUNCTION(glutsetwindowtitle)
{
	zval *name;
	ONE_PARAM(name);
	convert_to_string(name);
	glutSetWindowTitle(Z_STRVAL_P(name));
}
/* }}} */


/* {{{ void glutseticontitle(string name) */
PHP_FUNCTION(glutseticontitle)
{
	zval *name;
	ONE_PARAM(name);
	convert_to_string(name);
	glutSetIconTitle(Z_STRVAL_P(name));
}
/* }}} */


/* {{{ void glutsetcursor(long cursor) */
PHP_FUNCTION(glutsetcursor)
{
	zval *cursor;
	ONE_PARAM(cursor);
	convert_to_long(cursor);
	glutSetCursor(Z_LVAL_P(cursor));
}
/* }}} */


/* {{{ void glutestablishoverlay() */
PHP_FUNCTION(glutestablishoverlay)
{
	ZERO_PARAM();
	glutEstablishOverlay();
}
/* }}} */


/* {{{ void glutuselayer(long layer) */
PHP_FUNCTION(glutuselayer)
{
	zval *layer;
	ONE_PARAM(layer);
	convert_to_long(layer);
	glutUseLayer(Z_LVAL_P(layer));
}
/* }}} */


/* {{{ void glutremoveoverlay() */
PHP_FUNCTION(glutremoveoverlay)
{
	ZERO_PARAM();
	glutRemoveOverlay();
}
/* }}} */


/* {{{ void glutpostoverlayredisplay() */
PHP_FUNCTION(glutpostoverlayredisplay)
{
	ZERO_PARAM();
	glutPostOverlayRedisplay();
}
/* }}} */


/* {{{ void glutshowoverlay() */
PHP_FUNCTION(glutshowoverlay)
{
	ZERO_PARAM();
	glutShowOverlay();
}
/* }}} */


/* {{{ void gluthideoverlay() */
PHP_FUNCTION(gluthideoverlay)
{
	ZERO_PARAM();
	glutHideOverlay();
}
/* }}} */


void menu_callback(int selection)
{
	zval *z_menu_entry, **z_menu_id, **z_menu_parameter, *z_menu_function, *params[1];
	zval retval;
	TSRMLS_FETCH();

	if (zend_hash_index_find(menu_entry_callbacks, selection, (void **)&z_menu_entry) != SUCCESS) {
		php_error(E_WARNING, "unknown menu entry callback %d", selection);
		return;
	}
	if (zend_hash_index_find(Z_ARRVAL_P(z_menu_entry), 0, (void **)&z_menu_id) != SUCCESS) {
		php_error(E_WARNING, "can't find menu_id");
		return;
	}
	if (zend_hash_index_find(Z_ARRVAL_P(z_menu_entry), 1, (void **)&z_menu_parameter) != SUCCESS) {
		php_error(E_WARNING, "can't find menu_parameter");
		return;
	}
	if (zend_hash_index_find(menu_callbacks, Z_LVAL_PP(z_menu_id), (void **)&z_menu_function) != SUCCESS) {
		php_error(E_WARNING, "unknown menu callback %d", z_menu_id);
		return;
	}
	MAKE_STD_ZVAL(params[0]);
	ZVAL_LONG(params[0], Z_LVAL_PP(z_menu_parameter));
	if (call_user_function(CG(function_table), NULL, z_menu_function, &retval, 1, params TSRMLS_CC) != SUCCESS)
		zend_error(E_ERROR, "Function call failed");
	efree(params[0]);
}

/* {{{ long glutcreatemenu(mixed menu_callback) */
PHP_FUNCTION(glutcreatemenu)
{
	zval *callback;
	int menu_id;
	ONE_PARAM(callback);
	IS_CALLBACK(callback, 1);
	HASH_MENU_CALLBACK(callback, 1, menu_id = glutCreateMenu(menu_callback));
	RETURN_LONG(menu_id);
}
/* }}} */


/* {{{ void glutsetmenu(long menu) */
PHP_FUNCTION(glutsetmenu)
{
	zval *menu;
	ONE_PARAM(menu);
	convert_to_long(menu);
	glutSetMenu(Z_LVAL_P(menu));
}
/* }}} */


/* {{{ long glutgetmenu() */
PHP_FUNCTION(glutgetmenu)
{
	int menu;
	ZERO_PARAM();
	menu = glutGetMenu();
	RETURN_LONG(menu);
}
/* }}} */


/* {{{ void glutdestroymenu(long menu) */
PHP_FUNCTION(glutdestroymenu)
{
	zval *menu;
	ONE_PARAM(menu);
	convert_to_long(menu);
	glutDestroyMenu(Z_LVAL_P(menu));
}
/* }}} */


/* {{{ bool glutaddmenuentry(string name, long value) */
PHP_FUNCTION(glutaddmenuentry)
{
	zval *name, *value, *menu_entry;
	int current_menu;

	TWO_PARAM(name,value);
	convert_to_string(name);
	convert_to_long(value);

	if (0 == ( current_menu = glutGetMenu())) {
		php_error(E_WARNING, "%s() no current glutMenu; call glutcreatemenu() first",
				get_active_function_name(TSRMLS_C));
		RETURN_FALSE;
	}
	MAKE_STD_ZVAL(menu_entry);
	array_init(menu_entry);
	add_index_long(menu_entry, 0, current_menu);
	add_index_long(menu_entry, 1, Z_LVAL_P(value));
	zend_hash_index_update(menu_entry_callbacks, unique_menu_entry_id, menu_entry, sizeof(zval), NULL);
	glutAddMenuEntry(Z_STRVAL_P(name), unique_menu_entry_id++);
	RETURN_TRUE;
}
/* }}} */


/* {{{ void glutaddsubmenu(string name, long value) */
PHP_FUNCTION(glutaddsubmenu)
{
	zval *name,*value;
	TWO_PARAM(name,value);
	convert_to_string(name);
	convert_to_long(value);
	glutAddSubMenu(Z_STRVAL_P(name),Z_LVAL_P(value));
}
/* }}} */


/* {{{ void glutchangetomenuentry(long entry, string name, long value) */
PHP_FUNCTION(glutchangetomenuentry)
{
	zval *entry,*name,*value;
	THREE_PARAM(entry,name,value);
	convert_to_long(entry);
	convert_to_string(name);
	convert_to_long(value);
	glutChangeToMenuEntry(Z_LVAL_P(entry),Z_STRVAL_P(name),Z_LVAL_P(value));
}
/* }}} */


/* {{{ void glutchangetosubmenu(long entry, string name, long value) */
PHP_FUNCTION(glutchangetosubmenu)
{
	zval *entry,*name,*value;
	THREE_PARAM(entry,name,value);
	convert_to_long(entry);
	convert_to_string(name);
	convert_to_long(value);
	glutChangeToSubMenu(Z_LVAL_P(entry),Z_STRVAL_P(name),Z_LVAL_P(value));
}
/* }}} */


/* {{{ void glutremovemenuitem(long entry) */
PHP_FUNCTION(glutremovemenuitem)
{
	zval *entry;
	ONE_PARAM(entry);
	convert_to_long(entry);
	glutRemoveMenuItem(Z_LVAL_P(entry));
}
/* }}} */


/* {{{ void glutattachmenu(long button) */
PHP_FUNCTION(glutattachmenu)
{
	zval *button;
	ONE_PARAM(button);
	convert_to_long(button);
	glutAttachMenu(Z_LVAL_P(button));
}
/* }}} */


/* {{{ void glutdetachmenu(long button) */
PHP_FUNCTION(glutdetachmenu)
{
	zval *button;
	ONE_PARAM(button);
	convert_to_long(button);
	glutDetachMenu(Z_LVAL_P(button));
}
/* }}} */


void glutdisplayfunction_callback()
{
	void ***tsrm_ls = ts_resource_ex(0, 0);
	zval *retval;

	display_fci.param_count = 0;
	display_fci.params = NULL;
	display_fci.retval_ptr_ptr = &retval;

	if (zend_call_function(&display_fci, &display_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutdisplayfunc(mixed callback) */
PHP_FUNCTION(glutdisplayfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &display_fci, &display_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutDisplayFunc((void*)glutdisplayfunction_callback);
	RETURN_TRUE;
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
	ONE_PARAM(callback);
	HASH_CALLBACK(callback, 1, GLUT_OVERLAY_DISPLAY_CALLBACK);
	glutOverlayDisplayFunc((void*)glutoverlaydisplayfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutreshapefunc_callback(int width,int height)
{
	void ***tsrm_ls = ts_resource_ex(0, 0);
	zval ***params;
	zval *retval;

	params = (zval***)safe_emalloc(2, sizeof(zval**), 0);

	params[0] = (zval *)emalloc(sizeof(zval*));
	MAKE_STD_ZVAL(*params[0]);
	ZVAL_LONG(*params[0], width);
	
	params[1] = (zval *)emalloc(sizeof(zval*));
	MAKE_STD_ZVAL(*params[1]);
	ZVAL_LONG(*params[1], height);

	reshape_fci.param_count = 2;
	reshape_fci.params = params;
	reshape_fci.retval_ptr_ptr = &retval;

	if (zend_call_function(&reshape_fci, &reshape_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutreshapefunc(mixed callback) */
PHP_FUNCTION(glutreshapefunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &reshape_fci, &reshape_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutReshapeFunc(glutreshapefunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutkeyboardfunc_callback(unsigned char key,int x,int y)
{
	void ***tsrm_ls = ts_resource_ex(0, 0);
	zval ***params;
	zval *retval;
	char *str;

	params = (zval***)safe_emalloc(3, sizeof(zval**), 0);
	params[0] = (zval *)emalloc(sizeof(zval*));
	params[1] = (zval *)emalloc(sizeof(zval*));
	params[2] = (zval *)emalloc(sizeof(zval*));
	str = (char *)emalloc(sizeof(char)*2);
	sprintf(str,"%c",key);

	MAKE_STD_ZVAL(*params[0]);
	MAKE_STD_ZVAL(*params[1]);
	MAKE_STD_ZVAL(*params[2]);

	ZVAL_STRING(*params[0], str, 1);
	ZVAL_LONG(*params[1], x);
	ZVAL_LONG(*params[2], y);

	keyboard_fci.param_count = 3;
	keyboard_fci.params = params;
	keyboard_fci.retval_ptr_ptr = &retval;

	if (zend_call_function(&keyboard_fci, &keyboard_fci_cache TSRMLS_CC) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the callback");
	}
}

/* {{{ bool glutkeyboardfunc(mixed callback) */
PHP_FUNCTION(glutkeyboardfunc)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f", &keyboard_fci, &keyboard_fci_cache) == FAILURE)
	{
		WRONG_PARAM_COUNT;
	}

	glutKeyboardFunc(glutkeyboardfunc_callback);
	RETURN_TRUE;
}
/* }}} */

void glutmousefunc_callback(int button, int state, int x, int y)
{
	zval *params[4];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	params[2] = (zval *)emalloc(sizeof(zval));
	params[3] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],button);
	ZVAL_LONG(params[1],state);
	ZVAL_LONG(params[2],x);
	ZVAL_LONG(params[2],y);
	call_user_callback(call_backs,GLUT_MOUSE_CALLBACK,4,params);
}

/* {{{ bool glutmousefunc(mixed callback) */
PHP_FUNCTION(glutmousefunc)
{
	zval *callback;
	ONE_PARAM(callback);
	HASH_CALLBACK(callback, 1, GLUT_MOUSE_CALLBACK);
	glutMouseFunc(glutmousefunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutmotionfunc_callback(int x,int y)
{
	zval *params[2];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],x);
	ZVAL_LONG(params[1],y);
	call_user_callback(call_backs,GLUT_MOTION_CALLBACK,2,params);
}

/* {{{ bool glutmotionfunc(mixed callback) */
PHP_FUNCTION(glutmotionfunc)
{
	zval *callback;
	ONE_PARAM(callback);
	HASH_CALLBACK(callback, 1, GLUT_MOTION_CALLBACK);
	glutMotionFunc(glutmotionfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutpassivemotionfunc_callback(int x,int y)
{
	zval *params[2];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],x);
	ZVAL_LONG(params[1],y);
	call_user_callback(call_backs,GLUT_PASSIVE_MOTION_CALLBACK,2,params);
}

/* {{{ bool glutpassivemotionfunc(mixed callback) */
PHP_FUNCTION(glutpassivemotionfunc)
{
	zval *callback;
	ONE_PARAM(callback);
	HASH_CALLBACK(callback, 1, GLUT_PASSIVE_MOTION_CALLBACK);
	glutPassiveMotionFunc(glutpassivemotionfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutvisibilityfunc_callback(int state)
{
	void ***tsrm_ls = ts_resource_ex(0, 0);
	zval ***params;
	zval *retval;

	params = (zval***)safe_emalloc(1, sizeof(zval**), 0);
	params[0] = (zval *)emalloc(sizeof(zval*));

	MAKE_STD_ZVAL(*params[0]);
	ZVAL_LONG(*params[0], state);

	visibility_fci.param_count = 1;
	visibility_fci.params = params;
	visibility_fci.retval_ptr_ptr = &retval;

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
	zval *params[1];
	params[0] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],state);
	call_user_callback(call_backs,GLUT_ENTRY_CALLBACK,1,params);
}

/* {{{ bool glutentryfunc(mixed callback) */
PHP_FUNCTION(glutentryfunc)
{
	zval *callback;
	ONE_PARAM(callback);
	HASH_CALLBACK(callback, 1, GLUT_ENTRY_CALLBACK);
	glutEntryFunc(glutentryfunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutspecialfunc_callback(int key,int x,int y)
{
	zval *params[3];
	params[0] = (zval *)emalloc(sizeof(zval));
	params[1] = (zval *)emalloc(sizeof(zval));
	params[2] = (zval *)emalloc(sizeof(zval));
	ZVAL_LONG(params[0],key);
	ZVAL_LONG(params[1],x);
	ZVAL_LONG(params[2],y);
	call_user_callback(call_backs,GLUT_SPECIAL_CALLBACK,3,params);
}

/* {{{ bool glutspecialfunc(mixed callback) */
PHP_FUNCTION(glutspecialfunc)
{
	zval *callback;
	ONE_PARAM(callback);
	HASH_CALLBACK(callback, 1, GLUT_SPECIAL_CALLBACK);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
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
	ONE_PARAM(callback);
	HASH_CALLBACK(callback, 1, GLUT_MENUSTATE_CALLBACK);
	glutMenuStateFunc(glutmenustatefunc_callback);
	RETURN_TRUE;
}
/* }}} */


void glutidlefunc_callback()
{
	void ***tsrm_ls = ts_resource_ex(0, 0);
	zval *retval;

	idle_fci.param_count = 0;
	idle_fci.params = NULL;
	idle_fci.retval_ptr_ptr = &retval;

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

	glutIdleFunc((void*)glutidlefunc_callback);
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
	zval *callback,*seconds,*value;
	THREE_PARAM(seconds,callback,value);
	HASH_CALLBACK(callback, 2, GLUT_VISIBILITY_CALLBACK);
	convert_to_long(seconds);
	convert_to_long(value);
	glutTimerFunc(Z_LVAL_P(seconds),gluttimerfunc_callback,Z_LVAL_P(value));
}
/* }}} */


/* {{{ void glutsetcolor(long cell, double red, double green, double blue) */
PHP_FUNCTION(glutsetcolor)
{
	zval *cell,*red,*green,*blue;
	FOUR_PARAM(cell,red,green,blue);
	convert_to_long(cell);
	convert_to_double(red);
	convert_to_double(green);
	convert_to_double(blue);
	glutSetColor(Z_LVAL_P(cell),Z_DVAL_P(red),Z_DVAL_P(green),Z_DVAL_P(blue));
}
/* }}} */


/* {{{ double glutgetcolor(long cell, long component) */
PHP_FUNCTION(glutgetcolor)
{
	zval *cell,*component;
	GLfloat color;
	TWO_PARAM(cell,component);
	convert_to_long(cell);
	convert_to_long(component);
	color = glutGetColor(Z_LVAL_P(cell),Z_LVAL_P(component));
	RETURN_DOUBLE(color);
}
/* }}} */


/* {{{ void glutcopycolormap(long win) */
PHP_FUNCTION(glutcopycolormap)
{
	zval *win;
	ONE_PARAM(win);
	convert_to_long(win);
	glutCopyColormap(Z_LVAL_P(win));
}
/* }}} */


/* {{{ void glutget(long state) */
PHP_FUNCTION(glutget)
{
	zval *state;
	ONE_PARAM(state);
	convert_to_long(state);
	glutGet(Z_LVAL_P(state));
}
/* }}} */


/* {{{ void glutlayerget(long info) */
PHP_FUNCTION(glutlayerget)
{
	zval *info;
	ONE_PARAM(info);
	convert_to_long(info);
	glutLayerGet(Z_LVAL_P(info));
}
/* }}} */


/* {{{ void glutdeviceget(long info) */
PHP_FUNCTION(glutdeviceget)
{
	zval *info;
	ONE_PARAM(info);
	convert_to_long(info);
	glutDeviceGet(Z_LVAL_P(info));
}
/* }}} */


/* {{{ long glutgetmodifiers() */
PHP_FUNCTION(glutgetmodifiers)
{
	int mod;
	ZERO_PARAM();
	mod = glutGetModifiers();
	RETURN_LONG(mod);
}
/* }}} */


/* {{{ long glutextensionsupported(string ext) */
PHP_FUNCTION(glutextensionsupported)
{
	zval *ext;
	int ret;
	ONE_PARAM(ext);
	convert_to_string(ext);
	ret = glutExtensionSupported(Z_STRVAL_P(ext));
	RETURN_LONG(ret);
}
/* }}} */


/* {{{ void glutbitmapcharacter(long font, string chr) */
PHP_FUNCTION(glutbitmapcharacter)
{
	zval *font,*chr;
	TWO_PARAM(font,chr);
	convert_to_long(font);
	convert_to_string(chr);
	glutBitmapCharacter((void*)Z_LVAL_P(font),Z_STRVAL_P(chr)[0]);
}
/* }}} */


/* {{{ long glutbitmapwidth(long font, string chr) */
PHP_FUNCTION(glutbitmapwidth)
{
	int width;
	zval *font,*chr;
	TWO_PARAM(font,chr);
	convert_to_long(font);
	convert_to_string(chr);
	width = glutBitmapWidth((void*)Z_LVAL_P(font),Z_STRVAL_P(chr)[0]);
	RETURN_LONG(width);
}
/* }}} */


/* {{{ void glutstrokecharacter(long font, string chr) */
PHP_FUNCTION(glutstrokecharacter)
{
	zval *font,*chr;
	TWO_PARAM(font,chr);
	convert_to_long(font);
	convert_to_string(chr);
	glutStrokeCharacter((void*)Z_LVAL_P(font),Z_STRVAL_P(chr)[0]);
}
/* }}} */


/* {{{ long glutstrokewidth(long font, string chr) */
PHP_FUNCTION(glutstrokewidth)
{
	int ret;
	zval *font,*chr;
	TWO_PARAM(font,chr);
	convert_to_long(font);
	convert_to_string(chr);
	ret = glutStrokeWidth((void*)Z_LVAL_P(font),Z_STRVAL_P(chr)[0]);
	RETURN_LONG(ret);
}
/* }}} */


/* {{{ void glutsolidsphere(double radius, long slices, long stacks) */
PHP_FUNCTION(glutsolidsphere)
{
	zval *radius,*slices,*stacks;
	THREE_PARAM(radius,slices,stacks);
	convert_to_double(radius);
	convert_to_long(slices);
	convert_to_long(stacks);
	glutSolidSphere(Z_DVAL_P(radius),Z_LVAL_P(slices),Z_LVAL_P(stacks));
}
/* }}} */


/* {{{ void glutwiresphere(double radius, long slices, long stacks) */
PHP_FUNCTION(glutwiresphere)
{
	zval *radius,*slices,*stacks;
	THREE_PARAM(radius,slices,stacks);
	convert_to_double(radius);
	convert_to_long(slices);
	convert_to_long(stacks);
	glutWireSphere(Z_DVAL_P(radius),Z_LVAL_P(slices),Z_LVAL_P(stacks));
}
/* }}} */


/* {{{ void glutsolidcube(double size) */
PHP_FUNCTION(glutsolidcube)
{
	zval *size;
	ONE_PARAM(size);
	convert_to_double(size);
	glutSolidCube(Z_DVAL_P(size));
}
/* }}} */


/* {{{ void glutwirecube(double size) */
PHP_FUNCTION(glutwirecube)
{
	zval *size;
	ONE_PARAM(size);
	convert_to_double(size);
	glutWireCube(Z_DVAL_P(size));
}
/* }}} */


/* {{{ void glutsolidcone(double base, doable height, long slices, long stacks) */
PHP_FUNCTION(glutsolidcone)
{
	zval *base,*height,*slices,*stacks;
	FOUR_PARAM(base,height,slices,stacks);
	convert_to_double(base);
	convert_to_double(height);
	convert_to_long(slices);
	convert_to_long(stacks);
	glutSolidCone(Z_DVAL_P(base),Z_DVAL_P(height),Z_LVAL_P(slices),Z_LVAL_P(stacks));
}
/* }}} */


/* {{{ void glutwirecone(double base, double height, long slices, long stacks) */
PHP_FUNCTION(glutwirecone)
{
	zval *base,*height,*slices,*stacks;
	FOUR_PARAM(base,height,slices,stacks);
	convert_to_double(base);
	convert_to_double(height);
	convert_to_long(slices);
	convert_to_long(stacks);
	glutWireCone(Z_DVAL_P(base),Z_DVAL_P(height),Z_LVAL_P(slices),Z_LVAL_P(stacks));
}
/* }}} */


/* {{{ void glutsolidtorus(double innerRadius, double outerRadius, long nsides, long rings) */
PHP_FUNCTION(glutsolidtorus)
{
	zval *innerRadius,*outerRadius,*nsides,*rings;
	FOUR_PARAM(innerRadius,outerRadius,nsides,rings);
	convert_to_double(innerRadius);
	convert_to_double(outerRadius);
	convert_to_long(nsides);
	convert_to_long(rings);
	glutSolidTorus(Z_DVAL_P(innerRadius),Z_DVAL_P(outerRadius),Z_LVAL_P(nsides),Z_LVAL_P(rings));
}
/* }}} */


/* {{{ void glutwiretorus(double innerRadius, double outerRadius, long nsides, long rings) */
PHP_FUNCTION(glutwiretorus)
{
	zval *innerRadius,*outerRadius,*nsides,*rings;
	FOUR_PARAM(innerRadius,outerRadius,nsides,rings);
	convert_to_double(innerRadius);
	convert_to_double(outerRadius);
	convert_to_long(nsides);
	convert_to_long(rings);
	glutWireTorus(Z_DVAL_P(innerRadius),Z_DVAL_P(outerRadius),Z_LVAL_P(nsides),Z_LVAL_P(rings));
}
/* }}} */


/* {{{ void glutsoliddodecahedron() */
PHP_FUNCTION(glutsoliddodecahedron)
{
	ZERO_PARAM();
	glutSolidDodecahedron();
}
/* }}} */


/* {{{ void glutwiredodecahedron() */
PHP_FUNCTION(glutwiredodecahedron)
{
	ZERO_PARAM();
	glutWireDodecahedron();
}
/* }}} */


/* {{{ void glutsolidoctahedron() */
PHP_FUNCTION(glutsolidoctahedron)
{
	ZERO_PARAM();
	glutSolidOctahedron();
}
/* }}} */


/* {{{ void glutwireoctahedron() */
PHP_FUNCTION(glutwireoctahedron)
{
	ZERO_PARAM();
	glutWireOctahedron();
}
/* }}} */


/* {{{ void glutsolidtetrahedron() */
PHP_FUNCTION(glutsolidtetrahedron)
{
	ZERO_PARAM();
	glutSolidTetrahedron();
}
/* }}} */


/* {{{ void glutwiretetrahedron() */
PHP_FUNCTION(glutwiretetrahedron)
{
	ZERO_PARAM();
	glutWireTetrahedron();
}
/* }}} */


/* {{{ void glutsolidicosahedron() */
PHP_FUNCTION(glutsolidicosahedron)
{
	ZERO_PARAM();
	glutSolidIcosahedron();
}
/* }}} */


/* {{{ void glutwireicosahedron() */
PHP_FUNCTION(glutwireicosahedron)
{
	ZERO_PARAM();
	glutWireIcosahedron();
}
/* }}} */


/* {{{ void glutsolidteapot(double size) */
PHP_FUNCTION(glutsolidteapot)
{
	zval *size;
	ONE_PARAM(size);
	convert_to_double(size);
	glutSolidTeapot(Z_DVAL_P(size));
}
/* }}} */


/* {{{ void glutwireteapot(double size) */
PHP_FUNCTION(glutwireteapot)
{
	zval *size;
	ONE_PARAM(size);
	convert_to_double(size);
	glutWireTeapot(Z_DVAL_P(size));
}
/* }}} */

