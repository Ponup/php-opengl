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

#ifndef PHP_GLUT_H
#define PHP_GLUT_H

#define GLUT_MENU_CALLBACK 0
#define GLUT_DISPLAY_CALLBACK 1
#define GLUT_OVERLAY_DISPLAY_CALLBACK 2
#define GLUT_KEYBOARD_CALLBACK 3
#define GLUT_MOUSE_CALLBACK 4
#define GLUT_MOTION_CALLBACK 5
#define GLUT_PASSIVE_MOTION_CALLBACK 6
#define GLUT_VISIBILITY_CALLBACK 7
#define GLUT_ENTRY_CALLBACK 8
#define GLUT_SPECIAL_CALLBACK 9
#define GLUT_SPACEBALLMOTION_CALLBACK 10
#define GLUT_SPACEBALLROTATE_CALLBACK 11
#define GLUT_SPACEBALLBUTTON_CALLBACK 12
#define GLUT_BUTTONBOX_CALLBACK 13
#define GLUT_DIALS_CALLBACK 14
#define GLUT_TABLETMOTION_CALLBACK 15
#define GLUT_TABLETBUTTON_CALLBACK 16
#define GLUT_MENUSTATUS_CALLBACK 17
#define GLUT_MENUSTATE_CALLBACK 18
#define GLUT_IDLE_CALLBACK 19
#define GLUT_TIMER_CALLBACK 20
#define GLUT_RESHAPE_CALLBACK 21

PHP_MINIT_FUNCTION(glut);
PHP_RINIT_FUNCTION(glut);
PHP_RSHUTDOWN_FUNCTION(glut);
PHP_MINFO_FUNCTION(glut);

PHP_FUNCTION(glutinit);
PHP_FUNCTION(glutinitwindowsize);
PHP_FUNCTION(glutinitwindowposition);
PHP_FUNCTION(glutinitdisplaymode);
PHP_FUNCTION(glutmainloop);
PHP_FUNCTION(glutcreatewindow);
PHP_FUNCTION(glutcreatesubwindow);
PHP_FUNCTION(glutdisplayfunc);
PHP_FUNCTION(glutsetwindow);
PHP_FUNCTION(glutgetwindow);
PHP_FUNCTION(glutdestroywindow);
PHP_FUNCTION(glutpostredisplay);
PHP_FUNCTION(glutswapbuffers);
PHP_FUNCTION(glutpositionwindow);
PHP_FUNCTION(glutreshapewindow);
PHP_FUNCTION(glutfullscreen);
PHP_FUNCTION(glutpopwindow);
PHP_FUNCTION(glutpushwindow);
PHP_FUNCTION(glutshowwindow);
PHP_FUNCTION(gluthidewindow);
PHP_FUNCTION(gluticonifywindow);
PHP_FUNCTION(glutsetwindowtitle);
PHP_FUNCTION(glutseticontitle);
PHP_FUNCTION(glutsetcursor);
PHP_FUNCTION(glutestablishoverlay);
PHP_FUNCTION(glutuselayer);
PHP_FUNCTION(glutremoveoverlay);
PHP_FUNCTION(glutpostoverlayredisplay);
PHP_FUNCTION(glutshowoverlay);
PHP_FUNCTION(gluthideoverlay);
PHP_FUNCTION(glutcreatemenu);
PHP_FUNCTION(glutaddmenuentry);
PHP_FUNCTION(glutattachmenu);
PHP_FUNCTION(glutdetachmenu);
PHP_FUNCTION(glutsetmenu);
PHP_FUNCTION(glutgetmenu);
PHP_FUNCTION(glutdestroymenu);
PHP_FUNCTION(glutaddsubmenu);
PHP_FUNCTION(glutchangetomenuentry);
PHP_FUNCTION(glutchangetosubmenu);
PHP_FUNCTION(glutremovemenuitem);
PHP_FUNCTION(glutoverlaydisplayfunc);
PHP_FUNCTION(glutreshapefunc);
PHP_FUNCTION(glutwarppointer);
PHP_FUNCTION(glutignorekeyrepeat);
PHP_FUNCTION(glutkeyboardfunc);
PHP_FUNCTION(glutkeyboardupfunc);
PHP_FUNCTION(glutmousefunc);
PHP_FUNCTION(glutmotionfunc);
PHP_FUNCTION(glutpassivemotionfunc);
PHP_FUNCTION(glutvisibilityfunc);
PHP_FUNCTION(glutentryfunc);
PHP_FUNCTION(glutspecialfunc);
PHP_FUNCTION(glutspaceballmotionfunc);
PHP_FUNCTION(glutspaceballrotatefunc);
PHP_FUNCTION(glutspaceballbuttonfunc);
PHP_FUNCTION(glutbuttonboxfunc);
PHP_FUNCTION(glutdialsfunc);
PHP_FUNCTION(gluttabletmotionfunc);
PHP_FUNCTION(gluttabletbuttonfunc);
PHP_FUNCTION(glutmenustatusfunc);
PHP_FUNCTION(glutmenustatefunc);
PHP_FUNCTION(glutidlefunc);
PHP_FUNCTION(gluttimerfunc);
PHP_FUNCTION(glutsetcolor);
PHP_FUNCTION(glutgetcolor);
PHP_FUNCTION(glutcopycolormap);
PHP_FUNCTION(glutsolidcube);
PHP_FUNCTION(glutwirecube);
PHP_FUNCTION(glutget);
PHP_FUNCTION(glutlayerget);
PHP_FUNCTION(glutdeviceget);
PHP_FUNCTION(glutgetmodifiers);
PHP_FUNCTION(glutextensionsupported);
PHP_FUNCTION(glutbitmapcharacter);
PHP_FUNCTION(glutbitmapwidth);
PHP_FUNCTION(glutstrokecharacter);
PHP_FUNCTION(glutstrokewidth);
PHP_FUNCTION(glutsolidteapot);
PHP_FUNCTION(glutwireteapot);
PHP_FUNCTION(glutsolidicosahedron);
PHP_FUNCTION(glutwireicosahedron);
PHP_FUNCTION(glutsolidtetrahedron);
PHP_FUNCTION(glutwiretetrahedron);
PHP_FUNCTION(glutsolidoctahedron);
PHP_FUNCTION(glutwireoctahedron);
PHP_FUNCTION(glutsoliddodecahedron);
PHP_FUNCTION(glutwiredodecahedron);
PHP_FUNCTION(glutsolidtorus);
PHP_FUNCTION(glutwiretorus);
PHP_FUNCTION(glutsolidcone);
PHP_FUNCTION(glutwirecone);
PHP_FUNCTION(glutsolidsphere);
PHP_FUNCTION(glutwiresphere);
PHP_FUNCTION(glutinitcontextversion);
PHP_FUNCTION(glutInitContextProfile);
PHP_FUNCTION(glutInitContextFlags);
PHP_FUNCTION(glutSetOption);
PHP_FUNCTION(glutLeaveMainLoop);

void glutdisplayfunction_callback();
void glutoverlaydisplayfunc_callback();
void glutreshapefunc_callback(int width,int height);
void glutkeyboardfunc_callback(unsigned char key,int x,int y);
void glutmotionfunc_callback(int x,int y);
void glutpassivemotionfunc_callback(int x,int y);
void glutvisibilityfunc_callback(int state);
void glutentryfunc_callback(int state);
void glutspecialfunc_callback(int key,int x,int y);
void glutspaceballmotionfunc_callback(int x,int y,int z);
void glutspaceballrotatefunc_callback(int x,int y,int z);
void glutspaceballbuttonfunc_callback(int button,int state);
void glutbuttonboxfunc_callback(int button,int state);
void glutdialsfunc_callback(int dial,int value);
void gluttabletmotionfunc_callback(int x,int y);
void gluttabletbuttonfunc_callback(int button,int state,int x,int y);
void glutmenustatusfunc_callback(int state,int x,int y);
void glutmenustatefunc_callback(int state);
void glutidlefunc_callback();
void gluttimerfunc_callback(int value);

#endif
