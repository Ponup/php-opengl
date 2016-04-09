<?php
/**
 * Example of how to draw a bezier mesh on the screen using the PHP-OpenGL extension.
 *
 * @author Santiago Lizardo <santiagolizardo@php.net>
 */

require 'bootstrap.php';

$gear_profile = array(
	array( 'rad' => 0.000, 'wid' => 0.0 ),
	array( 'rad' => 0.300, 'wid' => 7.0 ),
	array( 'rad' => 0.340, 'wid' => 0.4 ),
	array( 'rad' => 0.550, 'wid' => 0.64 ),
	array( 'rad' => 0.600, 'wid' => 0.4 ),
	array( 'rad' => 0.950, 'wid' => 1.0 ),
);

$a1 = 27.0;
$a2 = 67.0;
$a3 = 47.0;
$a4 = 87.0;
$i1 = 1.2;
$i2 = 3.1;
$i3 = 2.3;
$i4 = 1.1;

$d_near = 1.0;
$d_far = 2000;
$poo = 0;

function gear($nt,$wd,$ir,$or,$tp,$tip,$ns, array $ip)
{
  global $circle_subdiv,$a1,$a2,$a3,$a4,$i1,$i2,$i3,$i4,$d_near,$d_far,$poo;
  if ($nt <= 0)
    $circle_subdiv = 64;
  else {
    $circle_subdiv = $nt;
    while ($circle_subdiv < 64)
      $circle_subdiv += $nt;
  }

  if ($ns <= 0) {
    flat_face(0.0, $ir, $wd);
  } else {
    if ($ip[0]['rad'] > 0.0) {
      flat_face(0.0, $ip[0]['rad'] * $ir, $wd);
      $prev = $wd;
      $t = 0;
    } else {
      flat_face(0.0, $ip[1]['rad'] * $ir, $ip[0]['wid'] * $wd);
      $prev = $ip[0]['wid'];
      $t = 1;
    }
    for ($k = $t; $k < $ns; $k++) {
      if ($prev < $ip[$k]['wid']) {
        draw_inside($prev * $wd, $ip[$k]['wid'] * $wd, $ip[$k]['rad'] * $ir);
      } else {
        draw_outside($prev * $wd, $ip[$k]['wid'] * $wd, $ip[$k]['rad'] * $ir);
      }
      $prev = $ip[$k]['wid'];
      /* - draw to edge of wheel, add final face if needed - */
      if ($k == $ns - 1) {
        flat_face($ip[$k]['rad'] * $ir, $ir, $ip[$k]['wid'] * $wd);

        /* now draw side to match tooth rim */
        if ($ip[$k]['wid'] < 1.0) {
          draw_inside($ip[$k]['wid'] * $wd, $wd, $ir);
        } else {
          draw_outside($ip[$k]['wid'] * $wd, $wd, $ir);
        }
      } else {
        flat_face($ip[$k]['rad'] * $ir, $ip[$k + 1]['rad'] * $ir, $ip[$k]['wid'] * $wd);
      }
    }
  }

  /* --- tooth side faces --- */
  tooth_side($nt, $ir, $or, $tp, $tip, $wd);
  /* --- tooth hill surface --- */
}

function tooth_side($nt,$ir,$or,$tp,$tip,$wd)
{
  global $a1,$a2,$a3,$a4,$i1,$i2,$i3,$i4,$d_near,$d_far,$poo;
  $end = 2.0 * M_PI / $nt;
  $or = $or * $ir;         /* or is really a ratio of ir */
  for ($i = 0; $i < 2.0 * M_PI - $end / 4.0; $i += $end) {

    $c[0] = cos($i);
    $s[0] = sin($i);
    $c[1] = cos($i + $end * (0.5 - $tip / 2));
    $s[1] = sin($i + $end * (0.5 - $tip / 2));
    $c[2] = cos($i + $end * (0.5 + $tp / 2));
    $s[2] = sin($i + $end * (0.5 + $tp / 2));

    $x[0] = $ir * $c[0];
    $y[0] = $ir * $s[0];
    $x[5] = $ir * cos($i + $end);
    $y[5] = $ir * sin($i + $end);
    /* ---treat veritices 1,4 special to match strait edge of
       face */
    $x[1] = $x[0] + ($x[5] - $x[0]) * (0.5 - $tp / 2);
    $y[1] = $y[0] + ($y[5] - $y[0]) * (0.5 - $tp / 2);
    $x[4] = $x[0] + ($x[5] - $x[0]) * (0.5 + $tp / 2);
    $y[4] = $y[0] + ($y[5] - $y[0]) * (0.5 + $tp / 2);
    $x[2] = $or * cos($i + $end * (0.5 - $tip / 2));
    $y[2] = $or * sin($i + $end * (0.5 - $tip / 2));
    $x[3] = $or * cos($i + $end * (0.5 + $tip / 2));
    $y[3] = $or * sin($i + $end * (0.5 + $tip / 2));

    /* draw face trapezoids as 2 tmesh */
    glNormal3f(0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f($x[2], $y[2], $wd / 2);
    glVertex3f($x[1], $y[1], $wd / 2);
    glVertex3f($x[3], $y[3], $wd / 2);
    glVertex3f($x[4], $y[4], $wd / 2);
    glEnd();

    glNormal3f(0.0, 0.0, -1.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f($x[2], $y[2], -$wd / 2);
    glVertex3f($x[1], $y[1], -$wd / 2);
    glVertex3f($x[3], $y[3], -$wd / 2);
    glVertex3f($x[4], $y[4], -$wd / 2);
    glEnd();

    /* draw inside rim pieces */
    glNormal3f($c[0], $s[0], 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f($x[0], $y[0], -$wd / 2);
    glVertex3f($x[1], $y[1], -$wd / 2);
    glVertex3f($x[0], $y[0], $wd / 2);
    glVertex3f($x[1], $y[1], $wd / 2);
    glEnd();

    /* draw up hill side */
    {
      /* calculate normal of face */
      $aa = $x[2] - $x[1];
      $ba = $y[2] - $y[1];
      $na = 1.0 / sqrt($aa * $aa + $ba * $ba);
      $aa = $aa * $na;
      $ba = $ba * $na;
      glNormal3f($ba, -$aa, 0.0);
    }
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f($x[1], $y[1], -$wd / 2);
    glVertex3f($x[2], $y[2], -$wd / 2);
    glVertex3f($x[1], $y[1], $wd / 2);
    glVertex3f($x[2], $y[2], $wd / 2);
    glEnd();
    /* draw top of hill */
    glNormal3f($c[1], $s[1], 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f($x[2], $y[2], -$wd / 2);
    glVertex3f($x[3], $y[3], -$wd / 2);
    glVertex3f($x[2], $y[2], $wd / 2);
    glVertex3f($x[3], $y[3], $wd / 2);
    glEnd();

    /* draw down hill side */
    {
      /* calculate normal of face */
      $aa = $x[4] - $x[3];
      $ba = $y[4] - $y[3];
      $ca = 1.0 / sqrt($aa * $aa + $ba * $ba);
      $aa = $aa * $ca;
      $ba = $ba * $ca;
      glNormal3f($ba, -$aa, 0.0);
    }
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f($x[3], $y[3], -$wd / 2);
    glVertex3f($x[4], $y[4], -$wd / 2);
    glVertex3f($x[3], $y[3], $wd / 2);
    glVertex3f($x[4], $y[4], $wd / 2);
    glEnd();
    /* inside rim part */
    glNormal3f($c[2], $s[2], 0.0);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f($x[4], $y[4], -$wd / 2);
    glVertex3f($x[5], $y[5], -$wd / 2);
    glVertex3f($x[4], $y[4], $wd / 2);
    glVertex3f($x[5], $y[5], $wd / 2);
    glEnd();
  }
}

function flat_face($ir,$or,$wd)
{
  global $circle_subdiv,$a1,$a2,$a3,$a4,$i1,$i2,$i3,$i4,$d_near,$d_far,$poo;
  if ($poo)
    printf("Face  : %f..%f wid=%f\n", $ir, $or, $wd);
  if ($wd == 0.0)
    return;
  for ($w = $wd / 2; $w > -$wd; $w -= $wd) {
    if ($w > 0.0)
      glNormal3f(0.0, 0.0, 1.0);
    else
      glNormal3f(0.0, 0.0, -1.0);

    if ($ir == 0.0) {
      /* draw as t-fan */
      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0.0, 0.0, $w);  /* center */
      glVertex3f($or, 0.0, $w);
      for ($i = 1; $i < $circle_subdiv; $i++) {
        glVertex3f(cos(2.0 * M_PI * $i / $circle_subdiv) * $or,
          sin(2.0 * M_PI * $i / vcircle_subdiv) * $or,
          $w);
      }
      glVertex3f($or, 0.0, $w);
      glEnd();
    } else {
      /* draw as tmesh */
      glBegin(GL_TRIANGLE_STRIP);
      glVertex3f($or, 0.0, $w);
      glVertex3f($ir, 0.0, $w);
      for ($i = 1; $i < $circle_subdiv; $i++) {
        glVertex3f(cos(2.0 * M_PI * $i / $circle_subdiv) * $or,
          sin(2.0 * M_PI * $i / $circle_subdiv) * $or,
          $w);
        glVertex3f(cos(2.0 * M_PI * $i / $circle_subdiv) * $ir,
          sin(2.0 * M_PI * $i / $circle_subdiv) * $ir,
          $w);
      }
      glVertex3f($or, 0.0, $w);
      glVertex3f($ir, 0.0, $w);
      glEnd();

    }
  }
}

function draw_inside($w1,$w2,$rad)
{
  global $circle_subdiv,$a1,$a2,$a3,$a4,$i1,$i2,$i3,$i4,$d_near,$d_far,$poo;
  if ($poo)
    printf("Inside: wid=%f..%f rad=%f\n", $w1, $w2, $rad);
  if ($w1 == $w2)
    return;

  $w1 = $w1 / 2;
  $w2 = $w2 / 2;
  for ($j = 0; $j < 2; $j++) {
    if ($j == 1) {
      $w1 = -$w1;
      $w2 = -$w2;
    }
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f($rad, 0.0, $w1);
    glVertex3f($rad, 0.0, $w2);
    for ($i = 1; $i < $circle_subdiv; $i++) {
      $c = cos(2.0 * M_PI * $i / $circle_subdiv);
      $s = sin(2.0 * M_PI * $i / $circle_subdiv);
      glNormal3f(-$c, -$s, 0.0);
      glVertex3f($c * $rad,
        $s * $rad,
        $w1);
      glVertex3f($c * $rad,
        $s * $rad,
        $w2);
    }
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f($rad, 0.0, $w1);
    glVertex3f($rad, 0.0, $w2);
    glEnd();
  }
}

function draw_outside($w1,$w2,$rad)
{
	global $circle_subdiv,$a1,$a2,$a3,$a4,$i1,$i2,$i3,$i4,$d_near,$d_far,$poo;
	if ($poo)
		printf("Outsid: wid=%f..%f rad=%f\n", $w1, $w2, $rad);
	if ($w1 == $w2)
		return;

  $w1 = $w1 / 2;
  $w2 = $w2 / 2;
  for ($j = 0; $j < 2; $j++) {
    if ($j == 1) {
      $w1 = -$w1;
      $w2 = -$w2;
    }
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f($rad, 0.0, $w1);
    glVertex3f($rad, 0.0, $w2);
    for ($i = 1; $i < $circle_subdiv; $i++) {
      $c = cos(2.0 * M_PI * $i / $circle_subdiv);
      $s = sin(2.0 * M_PI * $i / $circle_subdiv);
      glNormal3f($c, $s, 0.0);
      glVertex3f($c * $rad,
        $s * $rad,
        $w1);
      glVertex3f($c * $rad,
        $s * $rad,
        $w2);
    }
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f($rad, 0.0, $w1);
    glVertex3f($rad, 0.0, $w2);
    glEnd();
  }
}


$display = function()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
};

$myReshape = function( $w,$h )
{
	global $d_near,$d_far;
	glViewport( 0, 0, $w, $h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -1.0, 1.0, -1.0, 1.0, $d_near, $d_far );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
};

$oneFrame = function()
{
	global $gear_profile, $a1, $a2, $a3, $a4, $i1, $i2, $i3, $i4, $d_near, $d_far, $poo;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -4.0);
	glRotatef($a3, 1.0, 1.0, 1.0);
	glRotatef($a4, 0.0, 0.0, -1.0);
	glTranslatef(0.14, 0.2, 0.0);
	gear(76, 0.4, 2.0, 1.1,0.4, 0.04, 6, $gear_profile);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, 0.2, -3.8);
	glRotatef($a2, -4.0, 2.0, -1.0);
	glRotatef($a1, 1.0, -3.0, 1.0);
	glTranslatef(0.0, -0.2, 0.0);
	gear(36, 0.4, 2.0, 1.1, 0.7, 0.2, 6, $gear_profile);
	glPopMatrix();

	$a1 += $i1;
	if ($a1 > 360.0)
		$a1 -= 360.0;
	if ($a1 < 0.0)
		$a1 -= 360.0;

	$a2 += $i2;
	if ($a2 > 360.0)
		$a2 -= 360.0;
	if ($a2 < 0.0)
		$a2 -= 360.0;

	$a3 += $i3;
	if ($a3 > 360.0)
		$a3 -= 360.0;
	if ($a3 < 0.0)
		$a3 -= 360.0;

	$a4 += $i4;
	if ($a4 > 360.0)
		$a4 -= 360.0;
	if ($a4 < 0.0)
		$a4 -= 360.0;
	glutSwapBuffers();
};

$visibility = function($status) use( $oneFrame )
{
	if( $status == GLUT_VISIBLE )
	{
		glutIdleFunc( $oneFrame );
	}
	else
	{
		glutIdleFunc( null );
	}
};

function initGraphics()
{
	global $myReshape;
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	$myReshape( 640, 480 );
	glEnable( GL_DEPTH_TEST );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_LIGHTING );

	glLightf( GL_LIGHT0, GL_SHININESS, 1.0 );
	glLightfv( GL_LIGHT0, GL_POSITION, array( 1.3, 1.3, -3.3, 1 ) );

	$f = array( .8, 1, .83, 1 );
	glLightfv( GL_LIGHT0, GL_SPECULAR, $f );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, $f );
	glEnable( GL_LIGHT0 );

	glLightf( GL_LIGHT1, GL_SHININESS, 1.0 );
	glLightfv( GL_LIGHT1, GL_POSITION, array( -2.3, .3, -7.3, 1 ) );

	$f = array( 1, .8, .93, 1 );
	glLightfv( GL_LIGHT1, GL_SPECULAR, $f );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, $f );
	glEnable( GL_LIGHT1 );

	/* Gear material */
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, array( .1, .15, .2, 1 ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, array( .9, .3, .3, 1 ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, array( .4, .9, .6, 1 ) );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 4 );
}

$keys = function( $c, $x, $y )
{
	if( ord( $c ) == 27 )
		exit();
};

glutInit( $argc, $argv );

glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
glutInitWindowPosition( 100, 100 );
glutInitWindowSize( 640, 480 );
glutCreateWindow( 'PHP-OpenGL: Gears example' );

initGraphics();

glutReshapeFunc( $myReshape );
glutDisplayFunc( $display );
glutKeyboardFunc( $keys );
glutVisibilityFunc( $visibility );
glutPostRedisplay();
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
glutMainLoop();

