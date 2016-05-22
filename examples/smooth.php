<?php

require 'bootstrap.php';
require 'glu.php';

/*
 * smooth.c
 * This program demonstrates smooth shading.
 * A smooth shaded polygon is drawn in a 2-D projection.
 */

function init() {
    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_SMOOTH);
}

function triangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(5.0, 5.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(25.0, 5.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(5.0, 25.0);
    glEnd();
}

$display = function() {
    glClear(GL_COLOR_BUFFER_BIT);
    triangle();
    glFlush();
};

$reshape = function($w, $h) {
    glViewport(0, 0, $w, $h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if ($w <= $h)
        gluOrtho2D(0.0, 30.0, 0.0, 30.0 * $h / $w);
    else
        gluOrtho2D(0.0, 30.0 * $w / $h, 0.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
};

$keyboard = function($key, $x, $y) {
    if (ord($key) === 27) {
        exit(0);
    }
};

glutInit($argc, $argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(500, 500);
glutInitWindowPosition(100, 100);
glutCreateWindow($argv[0]);
init();
glutDisplayFunc($display);
glutReshapeFunc($reshape);
glutKeyboardFunc($keyboard);
glutMainLoop();

