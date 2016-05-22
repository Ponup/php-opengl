<?php

require 'bootstrap.php';
require 'glu.php';

$left_click = GLUT_UP;
$right_click = GLUT_UP;
$xold;
$yold;
$width;
$height;
$wh;
$hw;
$rotate_x = 30;
$rotate_y = 15;
$alpha = 0;
$beta = 0;

/*
 * * Just a teapot and its frame
 */

function Teapot() {
    global $beta, $alpha;
    /* Axis */
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(1, -1, -1);
    glColor3f(0, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, 1, -1);
    glColor3f(0, 0, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);
    glEnd();

    glRotatef($beta, 1, 0, 0);
    glRotatef($alpha, 0, 1, 0);
    glColor3f(1, 1, 1);

    /* Teapot itself */
    glutWireTeapot(0.5);
}

/*
 * * Function called to update rendering
 */
$displayCallback = function() {
    global $width, $height, $wh, $hw, $rotate_x, $rotate_y;
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    /* Perspective view */
    glViewport(0, 0, $width / 2, $height / 2);
    glPushMatrix();
    glTranslatef(0, 0, -10);
    glRotatef($rotate_y, 1, 0, 0);
    glRotatef($rotate_x, 0, 1, 0);
    Teapot();
    glPopMatrix();

    /* Orthogonal projection */
    glMatrixMode(GL_PROJECTION);
    /*
     * * Note how the projection matrix is pushed, to save the perspective
     * * computed in ReshapeFunc
     */
    glPushMatrix();

    glLoadIdentity();
    if ($height > $width)
        glOrtho(-1.2, 1.2, -1.2 * $hw, 1.2 * $hw, -1.2, 1.2);
    else
        glOrtho(-1.2 * $wh, 1.2 * $wh, -1.2, 1.2, -1.2, 1.2);
    glMatrixMode(GL_MODELVIEW);

    /* Right view */
    glViewport(0, $height / 2 + 1, $width / 2 + 1, $height / 2);
    glPushMatrix();
    glRotatef(90, 0, -1, 0);
    Teapot();
    glPopMatrix();

    /* Face view */
    glViewport($width / 2 + 1, $height / 2 + 1, $width / 2, $height / 2);
    glPushMatrix();
    Teapot();
    glPopMatrix();

    /* Top view */
    glViewport($width / 2 + 1, 0, $width / 2, $height / 2);
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    Teapot();
    glPopMatrix();

    /* Projection matrix is restaured */
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    /* End */
    glFlush();
    glutSwapBuffers();
};

/*
 * * Function called when the window is created or resized
 */
$ReshapeFunc = function($new_width, $new_height) use( &$width, &$height ) {
    $width = $new_width;
    $height = $new_height;

    $hw = $height / (float) $width;
    $wh = $width / (float) $height;

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(20 * sqrt(1 + $hw * $hw), $wh, 8, 12);

    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
};

/*
 * * Function called when a key is hit
 */
$KeyboardFunc = function($key, $x, $y) {
    if ('q' == $key || 'Q' == $key || 27 == $key) {
        exit(0);
    }
};

/*
 * * Function called when a mouse button is hit
 */
$MouseFunc = function($button, $state, $x, $y) use(&$xold, &$yold, &$left_click, &$right_click) {
    if (GLUT_LEFT_BUTTON == $button)
        $left_click = $state;
    if (GLUT_RIGHT_BUTTON == $button)
        $right_click = $state;
    $xold = $x;
    $yold = $y;
};

/*
 * * Function called when the mouse is moved
 */
$MotionFunc = function($x, $y) use( &$xold, &$yold, &$rotate_x, &$rotate_y ) {
    global $beta, $alpha, $left_click, $right_click;
    if (GLUT_DOWN == $left_click) {
        $rotate_y = $rotate_y + ($y - $yold) / 5;
        $rotate_x = $rotate_x + ($x - $xold) / 5;
        if ($rotate_y > 90)
            $rotate_y = 90;
        if ($rotate_y < -90)
            $rotate_y = -90;
        glutPostRedisplay();
    }
    if (GLUT_DOWN == $right_click) {
        $beta = $beta + ($y - $yold) / 2;
        $alpha = $alpha + ($x - $xold) / 2;
        glutPostRedisplay();
    }
    $xold = $x;
    $yold = $y;
};


/* Creation of the window */
glutInit($argc, $argv);
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
glutInitWindowSize(500, 500);
glutCreateWindow("Viewport");

/* OpenGL settings */
glClearColor(0, 0, 0, 0);
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

/* Declaration of the callbacks */
glutDisplayFunc($displayCallback);
glutReshapeFunc($ReshapeFunc);
glutKeyboardFunc($KeyboardFunc);
glutMouseFunc($MouseFunc);
glutMotionFunc($MotionFunc);

glutMainLoop();

