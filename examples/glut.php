<?php

require 'bootstrap.php';
require 'glu.php';

define('DRAFT', 0);
define('MEDIUM', 1);
define('BEST', 2);

$drawquality = DRAFT;
$spincamera = true;
$cameradirection = 1;
$updownrotate = 60.0;
$ballbounce = true;
$ballspeed = 2.0;

foreach (array('OVALID', 'SPHEREID', 'BOXID', 'PLANEID', 'TEXTID') as $i => $const)
    define($const, $i + 1);

for ($i = 1; $i < $argc; $i++) {
    if (strstr($argv[$i], "-h") != NULL)
        GiveUsage($argv[0]);
    if (strstr($argv[$i], "-q") != NULL) {
        if ($i + 1 >= $argc)
            GiveUsage($argv[0]);
        $drawquality = $argv[$i + 1];
        if ($drawquality < DRAFT)
            $drawquality = DRAFT;
        if ($drawquality > BEST)
            $drawquality = BEST;
        $i++;
    }
}

/*
  This is where global settings are made, that is,
  things that will not change in time
 */

function CreateEnvironment() {
    global $drawquality;

    glEnable(GL_DEPTH_TEST);

    if ($drawquality == DRAFT)
        glShadeModel(GL_FLAT);

    if ($drawquality == MEDIUM)
        glShadeModel(GL_SMOOTH);

    if ($drawquality == BEST) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glShadeModel(GL_SMOOTH);
        glDisable(GL_DITHER);         /* Assume RGBA capabilities */
    }

    glLineWidth(1.0);
    glPointSize(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFrontFace(GL_CW);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0);         /* Background colour */
    glEnable(GL_COLOR_MATERIAL);
}

/*
  This is the basic display callback routine
  It creates the geometry, lighting, and viewing position
  In this case it rotates the camera around the scene
 */
$display = function() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    MakeCamera(FALSE, 0, 0);
    MakeLighting();
    MakeGeometry();
    glPopMatrix();

    /* glFlush(); This isn't necessary for double buffers */
    glutSwapBuffers();
};

/*
  Create the geometry
 */

function MakeGeometry() {
    global $drawquality, $ballbounce, $ballspeed;
    static $theta = 0.0;

    $radius = 0.5;
    $mshin1 = array(5.0);               /* For the sphere */
    $mspec1 = array(0.5, 0.5, 0.5, 1.0);
    $mdiff1 = array(0.6, 0.0, 0.6, 1.0);
    $mamb1 = array(0.1, 0.0, 0.1, 1.0);
    $mdiff2 = array(0.0, 1.0, 0.0, 1.0);   /* Green plane */
    $mamb2 = array(0.0, 0.2, 0.0, 1.0);
    $mdiff3 = array(0.5, 0.5, 0.5, 1.0);  /* Grey boxes */
    $mamb3 = array(0.2, 0.2, 0.2, 1.0);

    /* Create a green ground plane */
    glLoadName(PLANEID);
    if ($drawquality > DRAFT)
        glBegin(GL_POLYGON);
    else
        glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 1.0, 0.0);
    if ($drawquality > DRAFT) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, $mdiff2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, $mamb2);
    }
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(2.0, 0.0, 2.0);
    glVertex3f(2.0, 0.0, -2.0);
    glVertex3f(-2.0, 0.0, -2.0);
    glVertex3f(-2.0, 0.0, 2.0);
    glEnd();

    /* Place a few grey boxes around the place */
    glLoadName(BOXID);
    glColor3f(0.5, 0.5, 0.5);
    if ($drawquality > DRAFT) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, $mdiff3);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, $mamb3);
    }
    glPushMatrix();
    glTranslatef(1.8, 0.2, 1.8);
    if ($drawquality > DRAFT)
        glutSolidCube(0.4);
    else
        glutWireCube(0.4);
    glTranslatef(-3.6, 0.0, 0.0);
    if ($drawquality > DRAFT)
        glutSolidCube(0.4);
    else
        glutWireCube(0.4);
    glPopMatrix();

    /* Create a magenta bouncing ball */
    glLoadName(SPHEREID);
    glColor3f(0.6, 0.0, 0.6);
    if ($drawquality > DRAFT) {
        glMaterialfv(GL_FRONT, GL_SHININESS, $mshin1);
        glMaterialfv(GL_FRONT, GL_SPECULAR, $mspec1);
    }
    glPushMatrix();
    glTranslatef(0.0, $radius + 0.5 * (1 + sin(M_PI * $theta / 180)), 0.0);
    glScalef($radius, $radius, $radius);
    if ($ballbounce)
        $theta += $ballspeed;
    if ($drawquality > DRAFT)
        glutSolidSphere(1.0, 16, 16);
    else
        glutWireSphere(1.0, 32, 32);
    glPopMatrix();

    /* Create a white half-oval loop at one end */
    glLoadName(OVALID);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 1.0, 1.0);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    for ($i = 0; $i <= 180; $i++)
        glVertex3f(2 * cos($i * M_PI / 180), 1.5 * sin($i * M_PI / 180), -2.0);
    glEnd();

    /* Write some text */
    glLoadName(TEXTID);
    DrawTextXY(-2.0, -0.25, 2.0, 0.002, "PHP OpenGL Demo");
}

/*
  Set up the lighing environment
 */

function MakeLighting() {
    global $drawquality;

    $globalambient = array(0.3, 0.3, 0.3, 1.0);

    /* The specifications for 3 light sources */
    $pos0 = array(1.0, 1.0, 0.0, 0.0);      /* w = 0 == infinite distance */
    $dif0 = array(0.8, 0.8, 0.8, 1.0);

    $pos1 = array(5.0, -5.0, 0.0, 0.0);   /* Light from below */
    $dif1 = array(0.4, 0.4, 0.4, 1.0);      /* Fainter */

    if ($drawquality > DRAFT) {

        /* Set ambient globally, default ambient for light sources is 0 */
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, $globalambient);

        glLightfv(GL_LIGHT0, GL_POSITION, $pos0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, $dif0);

        glLightfv(GL_LIGHT1, GL_POSITION, $pos1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, $dif1);

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
    }
}

/*
  Set up the camera
  Optionally creating a small viewport about
  the mouse click point for object selection
 */

function MakeCamera($pickmode, $x, $y) {
    global $updownrotate, $spincamera, $cameradirection;
    static $theta = 0;

    $viewport = array();

    /* Camera setup */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if ($pickmode == TRUE) {
        glGetIntegerv(GL_VIEWPORT, $viewport, 4); /* Get the viewport bounds */
        gluPickatrix($x, $viewport[3] - $y, 3.0, 3.0, $viewport);
    }
    gluPerspective(70.0, /* Field of view */ 1.0, /* aspect ratio  */ 0.1, 1000.0);  /* near and far  */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5 * cos($theta * M_PI / 180) * sin($updownrotate * M_PI / 180), 5 * cos($updownrotate * M_PI / 180), 5 * sin($theta * M_PI / 180) * sin($updownrotate * M_PI / 180), 0.0, 0.0, 0.0, /* Focus    */ 0.0, 1.0, 0.0);                                  /* Up       */
    if ($spincamera)
        $theta += ($cameradirection * 0.2);
}

/*
  Deal with plain key strokes
 */
$handleKeyboard = function( $key, $x, $y ) use( &$spincamera, &$ballbounce ) {
    switch (ord($key)) {
        case 27: /* ESC */
        case ord('Q'):
        case ord('q'): exit();
            break;
        case ord('s'):
        case ord('S'): $spincamera = !$spincamera;
            break;
        case ord('b'):
        case ord('B'): $ballbounce = !$ballbounce;
            break;
    }
};

/*
  Deal with special key strokes
 */
$HandleSpecialKeyboard = function( $key, $x, $y ) use( &$updownrotate, &$cameradirection ) {
    switch ($key) {
        case GLUT_KEY_LEFT: $cameradirection = -1;
            break;
        case GLUT_KEY_RIGHT: $cameradirection = 1;
            break;
        case GLUT_KEY_UP: $updownrotate -= 2;
            break;
        case GLUT_KEY_DOWN: $updownrotate += 2;
            break;
    }
};

/*
  Handle mouse events
 */
$handleMouse = function( $button, $state, $x, $y ) {
    $maxselect = 100;
    $nhits = 0;
    $selectlist = array();

    if (true) {//$state == GLUT_DOWN) {
        glSelectBuffer($maxselect, $selectlist);
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(-1);

        glPushMatrix();
        MakeCamera(TRUE, $x, $y);
        MakeGeometry();
        glPopMatrix();
        $nhits = glRenderMode(GL_RENDER);

        if ($button == GLUT_LEFT_BUTTON) {
            
        } else if ($button == GLUT_MIDDLE_BUTTON) {
            
        } /* Right button events are passed to menu handlers */

        if ($nhits == -1)
            error_log("Select buffer overflow");

        /*
          $selectlist = glGetSelectBuffer();
          if ($nhits > 0) {
          echo "\tPicked $nhits objects: ";
          for ($i=0;$i<$nhits;$i++)
          echo $selectlist[4*$i+3] . " ";
          echo "\n";
          }
         */
    }
};

/*
  Handle the main menu
 */
$handleMainMenuCallback = function($whichone) use( &$spincamera, &$ballbounce ) {
    switch ($whichone) {
        case 1: $spincamera = !$spincamera;
            break;
        case 2: $ballbounce = !$ballbounce;
            break;
        case 100: exit();
            break;
    }
};

/*
  Handle the ball speed sub menu
 */
$handleSpeedMenuCallback = function($whichone) use( &$ballspeed ) {
    var_dump($whichone, $ballspeed);
    switch ($whichone) {
        case 10: $ballspeed = 0.5;
            break;
        case 20: $ballspeed = 2;
            break;
        case 30: $ballspeed = 10;
            break;
    }
};

/*
  What to do on an idle event
 */
$handleIdle = function() {

    glutPostRedisplay();
};

/*
  How to handle visibility
 */
$handleVisibility = function($visible) use( $handleIdle ) {

    if ($visible == GLUT_VISIBLE)
        glutIdleFunc($handleIdle);
    else
        glutIdleFunc(array());
};

/*
  Draw text in the x-y plane
  The x,y,z coordinate is the bottom left corner (looking down -ve z axis)
 */

function DrawTextXY($x, $y, $z, $scale, $s) {

    glPushMatrix();
    glTranslatef($x, $y, $z);
    glScalef($scale, $scale, $scale);
    for ($i = 0; $i < strlen($s); $i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, $s[$i]);
    glPopMatrix();
}

/*
  Display the program usage information
 */

function GiveUsage($cmd) {

    echo <<<USAGE
Usage:    $cmd [-h] [-q n]
          -h   this text
          -q n quality, 0,1,2

Key Strokes and Menus:
           q - quit
           s - toggle camera spin
           b - toggle ball bounce
  left arrow - change rotation direction
 right arrow - change rotation direction
  down arrow - rotate camera down
    up arrow - rotate camera up

USAGE;
    exit;
}

/* Set things up and go */
glutInit($argc, $argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glutCreateWindow("OpenGL Demo");
glutDisplayFunc($display);
glutVisibilityFunc($handleVisibility);
glutKeyboardFunc($handleKeyboard);
glutSpecialFunc($HandleSpecialKeyboard);
glutMouseFunc($handleMouse);
CreateEnvironment();

/* Set up some menus */
$speedmenu = glutCreateMenu($handleSpeedMenuCallback);
glutAddMenuEntry("Slow", 10);
glutAddMenuEntry("Medium", 20);
glutAddMenuEntry("Fast", 30);

$mainmenu = glutCreateMenu($handleMainMenuCallback);
glutAddMenuEntry("Toggle camera spin", 1);
glutAddMenuEntry("Toggle ball bounce", 2);
glutAddSubMenu("Ball speed", $speedmenu);
glutAddMenuEntry("Quit", 100);
glutAttachMenu(GLUT_RIGHT_BUTTON);

glutMainLoop();

