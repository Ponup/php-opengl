<?php

require 'bootstrap.php';

define( 'TORUS', 0 );
define( 'TEAPOT', 1 );
define( 'DOD', 2 );
define( 'TET', 3 );
define( 'ISO', 4 );
define( 'QUIT', 5 );

$spin = 0;
$obj = TORUS;
$begin = 0;

function output($x,$y,$format)
{
	glPushMatrix();
	glTranslatef($x, $y, 0);
	for($p = 0; $p < strlen($format); $p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, $format[$p]);
	glPopMatrix();
}

$menu_select = function($item) use (&$obj)
{
	if ($item == QUIT)
		exit();
	$obj = $item;
	glutPostRedisplay();
};

$movelight = function($button,$state,$x,$y) use( $begin )
{
	if($button == GLUT_LEFT_BUTTON )//&& $state == GLUT_DOWN)
	{
		$begin = $x;
	}
};

$motion = function($x, $y)
{
  global $begin;
  global $spin;
  $spin = ($spin + ($x - $begin)) % 360;
  $begin = $x;
  glutPostRedisplay();
};

function myinit()
{
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
}

$display = function()
{
  global $spin;
  global $obj;
  
  $position = array(0.0, 0.0, 1.5, 1.0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(0.0, 0.0, -5.0);

  glPushMatrix();
  glRotated($spin, 0.0, 1.0, 0.0);
  glRotated(0.0, 1.0, 0.0, 0.0);
  glLightfv(GL_LIGHT0, GL_POSITION, $position);

  glTranslated(0.0, 0.0, 1.5);
  glDisable(GL_LIGHTING);
  glColor3f(0.0, 1.0, 1.0);
  glutWireCube(0.1);
  glEnable(GL_LIGHTING);
  glPopMatrix();

  switch ($obj) {
  case TORUS:
    glutSolidTorus(0.275, 0.85, 20, 20);
    break;
  case TEAPOT:
    glutSolidTeapot(1.0);
    break;
  case DOD:
    glPushMatrix();
    glScalef(.5, .5, .5);
    glutSolidDodecahedron();
    glPopMatrix();
    break;
  case TET:
    glutSolidTetrahedron();
    break;
  case ISO:
    glutSolidIcosahedron();
    break;
  }

  glPopMatrix();
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 3000, 0, 3000);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  output(80, 2800, "Welcome to movelight.");
  output(80, 2650, "Right mouse button for menu.");
  output(80, 400, "Hold down the left mouse button");
  output(80, 250, "and move the mouse horizontally");
  output(80, 100, "to change the light position.");
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
  glutSwapBuffers();
};

$myReshape = function($w, $h)
{
  glViewport(0, 0, $w, $h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, $w / $h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
};

function tmotion($x, $y)
{
  echo sprintf("Tablet motion x = %d, y = %d\n", $x, $y);
}

function tbutton($b, $s, $x, $y)
{
  echo sprintf("b = %d, s = %d, x = %d, y = %d\n", $b, $s, $x, $y);
}

function smotion($x, $y, $z)
{
  echo sprintf("Spaceball motion %d %d %d\n", $x, $y, $z);
}

function rmotion($x, $y, $z)
{
  echo sprintf("Spaceball rotate %d %d %d\n", $x, $y, $z);
}

function sbutton($button, $state)
{
  echo sprintf("Spaceball button %d is %s\n",$button, $state == GLUT_UP ? "up" : "down");
}

function dials($dial, $value)
{
  echo sprintf("Dial %d is %d\n", $dial, $value);
  $spin = $value % 360;
  glutPostRedisplay();
}

$buttons = function($button, $state)
{
  printf("Button %d is %s\n", $button,$state == GLUT_UP ? "up" : "down");
};

glutInit($argc, $argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(500, 500);
glutCreateWindow($argv[0]);
myinit();
glutMouseFunc($movelight);
glutDisplayFunc($display);
glutReshapeFunc($myReshape);
glutMotionFunc($motion);
//glutButtonBoxFunc($buttons);
//glutCreateMenu($menu_select);
/*
glutTabletMotionFunc(tmotion);
glutTabletButtonFunc(tbutton);
glutSpaceballMotionFunc(smotion);
glutSpaceballRotateFunc(rmotion);
glutSpaceballButtonFunc(sbutton);
glutDialsFunc(dials);
glutAddMenuEntry("Torus", TORUS);
glutAddMenuEntry("Teapot", TEAPOT);
glutAddMenuEntry("Dodecahedron", DOD);
glutAddMenuEntry("Tetrahedron", TET);
glutAddMenuEntry("Icosahedron", ISO);
glutAddMenuEntry("Quit", QUIT);
glutAttachMenu(GLUT_RIGHT_BUTTON);
 */
glutMainLoop();

