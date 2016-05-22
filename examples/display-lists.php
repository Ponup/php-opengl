<?php

require 'bootstrap.php';
require 'glu.php';

/*
 * * The instructions to rotate each teapot, according to the mouse
 */

function compile_rotate_list() {
    global $rotate_list_id, $rotate_x, $rotate_y;
    glNewList($rotate_list_id, GL_COMPILE);
    glRotatef($rotate_y, 1, 0, 0);
    glRotatef($rotate_x, 0, 1, 0);
    glEndList();
}

/*
 * * The instructions to rotate each teapot, according to the mouse
 */

function compile_rotate_teapot_list() {
    global $rotate_teapot_list_id, $beta, $alpha;
    glNewList($rotate_teapot_list_id, GL_COMPILE);
    glRotatef($beta, 1, 0, 0);
    glRotatef($alpha, 0, 1, 0);
    glEndList();
}

/*
 * * The instructions to render a teapot
 */

function compile_teapot_list() {
    global $teapot_list_id, $rotate_teapot_list_id;
    glNewList($teapot_list_id, GL_COMPILE);
    glPushMatrix();

    /* Nested call to another display list */
    glCallList($rotate_teapot_list_id);

    glutWireTeapot(0.3);

    glPopMatrix();
    glEndList();
}

/*
 * * Function called to update rendering
 */
$displayCallback = function() {
    global $rotate_list_id, $teapot_list_id;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0, 0, -10.);

    glCallList($rotate_list_id);

    for ($i = 0; $i < 3; ++$i)
        for ($j = 0; $j < 3; ++$j)
            for ($k = 0; $k < 3; ++$k) {
                glPushMatrix();
                glTranslatef($i - 1, $j - 1, $k - 1);
                glColor3f($i / 2., $j / 2., $k / 2.);

                glCallList($teapot_list_id);

                glPopMatrix();
            }

    /* End */
    glFlush();
    glutSwapBuffers();
};

/*
 * * Function called when the window is created or resized
 */
$ReshapeFunc = function($width, $height) {
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluPerspective(20, $width / (float) $height, 5, 15);
    glViewport(0, 0, $width, $height);

    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
};

/*
 * * Function called when a key is hit
 */
$KeyboardFunc = function($key, $x, $y) use(&$xold, &$yold) {
    global $teapot_list_id, $rotate_teapot_list_id, $rotate_list_id;
    $xold = $x; /* Has no effect: just to avoid a warning */
    $yold = $y;
    if ('q' == $key || 'Q' == $key || 27 == $key) {
        glDeleteLists($rotate_list_id, 1);
        glDeleteLists($rotate_teapot_list_id, 1);
        glDeleteLists($teapot_list_id, 1);
        exit(0);
    }
};

/*
 * * Function called when a mouse button is hit
 */
$MouseFunc = function($button, $state, $x, $y) use(&$left_click, &$right_click, $xold, $yold) {
    if (GLUT_LEFT_BUTTON == $button)
        $left_click = $state;
    if (GLUT_RIGHT_BUTTON == $button)
        $right_click = $state;
    $xold = $x;
    $yold = $y;
    $GLOBALS['xold'] = $x;
    $GLOBALS['yold'] = $y;
};

/*
 * * Function called when the mouse is moved
 */
$MotionFunc = function($x, $y) use(&$rotate_x, &$rotate_y, &$xold, &$yold, &$beta, &$alpha) {
    global $left_click, $right_click;
    if (GLUT_DOWN == $left_click) {
        $rotate_y = $rotate_y + ($y - $yold) / 5;
        $rotate_x = $rotate_x + ($x - $xold) / 5;
        if ($rotate_y > 90)
            $rotate_y = 90;
        if ($rotate_y < -90)
            $rotate_y = -90;

        /* Update the rotations */
        compile_rotate_list();

        glutPostRedisplay();
    }
    if (GLUT_DOWN == $right_click) {
        $beta = $beta + ($y - $yold) / 2;
        $alpha = $alpha + ($x - $xold) / 2;

        /* Update the rotations */
        compile_rotate_teapot_list();

        glutPostRedisplay();
    }
    $xold = $x;
    $yold = $y;
};

$left_click = GLUT_UP;
$right_click = GLUT_UP;
$xold = $yold = 0;
$rotate_x = -30;
$rotate_y = 15;
$alpha = 0;
$beta = 10;

/* Creation of the window */
glutInit($argc, $argv);
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
glutInitWindowSize(500, 500);
glutCreateWindow("PHP-OpenGL: Display lists example");

/* OpenGL settings */
glClearColor(0, 0, 0, 0);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glEnable(GL_DEPTH_TEST);

/* Compilation of the instructions lists */
$rotate_list_id = glGenLists(1);
$rotate_teapot_list_id = glGenLists(1);
$teapot_list_id = glGenLists(1);
compile_rotate_list();
compile_rotate_teapot_list();
compile_teapot_list();

/* Declaration of the callbacks */
glutDisplayFunc($displayCallback);
glutReshapeFunc($ReshapeFunc);
glutKeyboardFunc($KeyboardFunc);
glutMouseFunc($MouseFunc);
glutMotionFunc($MotionFunc);

/* Loop */
glutMainLoop();

