<?php

require 'bootstrap.php';
require 'glu.php';

define('MENU_STARTOVER', 0);
define('MENU_STOP_RINGS', 1);
define('MENU_STOP_FADE', 2);
define('MENU_START_RINGS', 3);
define('MENU_START_FADE', 4);
define('MENU_ZOOM_OUT', 6);
define('MENU_QUIT', 5);
define('NOTALLOWED', 0);
define('CONE', 1);
define('TORUS', 2);
define('INNERMAT', 3);
define('OUTTERMAT', 4);
define('STEPS', 30);

$Fade = 1;   /* Start moving out */

$Axis = 0;
$AxisInc = (2.0 * M_PI / STEPS);

function myInit() {
    global $Fade, $Axis, $AxisInc, $InnerRad, $OutterRad, $Tilt, $Trans, $TransCone, $Dist;
    $InnerRad = 0.70;
    $OutterRad = 5.0;
    $Tilt = 15;
    $Dist = 30;

    $sinoftilt = sin($Tilt * M_PI * 2 / 360);

    $Trans = (2 * $OutterRad + $InnerRad) * $sinoftilt + $InnerRad +
            ((1 - $sinoftilt) * $InnerRad) - ($InnerRad * 1 / 10);

    $TransCone = $Trans + ($OutterRad * $sinoftilt + $InnerRad);
}

function myglInit() {
    global $Fade, $Axis, $AxisInc, $InnerRad, $OutterRad, $Tilt, $Trans, $TransCone, $Dist;
    $light0_position = array(1.0, 0.2, 1.0, 0.0);
    $light1_position = array(0.0, 0.0, 0.0, 1.0);
    $light1_diffuse = array(1.0, 1.0, 1.0, 1.0);
    $light1_specular = array(1.0, 1.0, 1.0, 1.0);
    $lm_ambient = array(0.2, 0.2, 0.2, 1.0);

    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, $light0_position);
    glLightfv(GL_LIGHT1, GL_POSITION, $light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, $light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, $light1_specular);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, $lm_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glFlush();
}

$reshapeCallback = function($w, $h) {
    glViewport(0, 0, $w, $h);
    glFlush();
};

function MakeDisplayLists() {
    global $Fade, $Axis, $AxisInc, $InnerRad, $OutterRad, $Tilt, $Trans, $TransCone, $Dist;
    $cone_diffuse = array(0.0, 0.7, 0.7, 1.0);
    $mat1_ambient = array(1.0, 1.0, 1.0, 1.0);
    $mat2_ambient = array(0.0, 0.0, 0.0, 0.0);
    $torus1_diffuse = array(0.7, 0.7, 0.0, 1.0);
    $torus2_diffuse = array(0.3, 0.0, 0.0, 1.0);
    $mat1_specular = array(1.0, 1.0, 1.0, 1.0);
    $mat2_specular = array(0.5, 0.5, 0.5, 1.0);

    glNewList(INNERMAT, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, $mat1_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, $torus1_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, $mat1_ambient);
    glEndList();

    glNewList(OUTTERMAT, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, $mat2_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, $torus2_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, $mat2_ambient);
    glEndList();

    glNewList(CONE, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, $cone_diffuse);
    glPushMatrix();
    glTranslatef(0, -$TransCone, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidCone($OutterRad, 10, 8, 8);
    glPopMatrix();
    glEndList();

    glNewList(TORUS, GL_COMPILE);
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glutSolidTorus($InnerRad, $OutterRad, 15, 25);
    glPopMatrix();
    glEndList();
}

function DrawRings($axis) {
    global $Fade, $Axis, $AxisInc, $InnerRad, $OutterRad, $Tilt, $Trans, $TransCone, $Dist;
    $x = sin($axis);
    $y = cos($axis);

    glPushMatrix();
    glTranslatef(0, $Trans, 0);
    glRotatef($Tilt, $x, 0, $y);
    glCallList(TORUS);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-$Tilt, $x, 0, $y);
    glCallList(TORUS);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -$Trans, 0);
    glRotatef($Tilt, $x, 0, $y);
    glCallList(TORUS);
    glPopMatrix();
}

/* Draw the inner thing, then glScale and draw 3 huge rings */
$mydisplay = function() {
    global $Fade, $Axis, $AxisInc, $InnerRad, $OutterRad, $Tilt, $Trans, $TransCone, $Dist;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 10, 1000);
    gluLookAt(0, 0, $Dist, 0, 0, 0, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);

    glCallList(INNERMAT);
    DrawRings($Axis);
    glCallList(CONE);

    glCallList(OUTTERMAT);
    glPushMatrix();
    glScalef(10, 10, 10);
    DrawRings($Axis / 3);
    glPopMatrix();

    glutSwapBuffers();
    glFlush();
};

/* rotate the axis and adjust position if nec. */
$myidle = function() use( $mydisplay ) {
    global $Fade, $Axis, $AxisInc, $InnerRad, $OutterRad, $Tilt, $Trans, $TransCone, $Dist;
    $Axis += $AxisInc;

    if ($Dist < 15 && $Fade) /* start slow */
        $Dist += 0.1;
    else if ($Dist < 800 && $Fade) /* don't go back too far */
        $Dist *= 1.005;

    $mydisplay();
};

/* nothing fancy */
$handlemenu = function($value) {
    global $Fade, $Axis, $AxisInc, $InnerRad, $OutterRad, $Tilt, $Trans, $TransCone, $Dist;
    switch ($value) {
        case MENU_STARTOVER:
            $Dist = 10;
            $Axis = 0;
            $Fade = 1;
            $AxisInc = (2.0 * M_PI / STEPS);
            glutChangeToMenuEntry(3, "Stop rings", MENU_STOP_RINGS);
            glutChangeToMenuEntry(4, "Stop fade", MENU_STOP_FADE);
            break;
        case MENU_ZOOM_OUT:
            $Dist = 800;
            break;
        case MENU_STOP_RINGS:
            $AxisInc = 0;
            glutChangeToMenuEntry(3, "Start rings", MENU_START_RINGS);
            break;
        case MENU_START_RINGS:
            $AxisInc = (2.0 * M_PI / STEPS);
            glutChangeToMenuEntry(3, "Stop rings", MENU_STOP_RINGS);
            break;
        case MENU_STOP_FADE:
            $Fade = 0;
            glutChangeToMenuEntry(4, "Start fade", MENU_START_FADE);
            break;
        case MENU_START_FADE:
            $Fade = 1;
            glutChangeToMenuEntry(4, "Stop fade", MENU_STOP_FADE);
            break;
        case MENU_QUIT:
            exit();
            break;
    }
};

function MenuInit() {
    global $handlemenu;
    glutCreateMenu($handlemenu);
    glutAddMenuEntry("Start Over", MENU_STARTOVER);
    glutAddMenuEntry("Zoom Out", MENU_ZOOM_OUT);
    glutAddMenuEntry("Stop rings", MENU_STOP_RINGS);
    glutAddMenuEntry("Stop fade", MENU_STOP_FADE);
    glutAddMenuEntry("Quit", MENU_QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

$vis = function($visible) use( $myidle ) {
    if (1 || $visible == GLUT_VISIBLE) {
        glutIdleFunc($myidle);
    } else {
        glutIdleFunc(NULL);
    }
};

glutInit($argc, $argv);
glutInitWindowSize(512, 512);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glutCreateWindow("rings");

myInit();
myglInit();

MakeDisplayLists();
MenuInit();

glutReshapeFunc($reshapeCallback);
glutDisplayFunc($mydisplay);
glutVisibilityFunc($vis);

glutMainLoop();

