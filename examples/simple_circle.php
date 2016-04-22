<?php

require 'bootstrap.php';

$Draw = function() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);

    glVertex3f(0.1, 0.1, 0.0);
    glVertex3f(0.9, 0.1, 0.0);
    glVertex3f(0.8, 0.9, 0.0);
    glVertex3f(0.2, 0.9, 0.0);

    glEnd();

    DrawCircle(0.5, 0.5, 0.2, 30);

    glFlush();
};

function DrawCircle($cx, $cy, $r, $num_segments) {
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 0.0, 0.0);
    for ($ii = 0; $ii < $num_segments; $ii++) {
        $theta = 2.0 * 3.1415926 * (float) $ii / (float) $num_segments; //get the current angle

        $x = $r * cos($theta); //calculate the x component
        $y = $r * sin($theta); //calculate the y component

        glVertex2f($x + $cx, $y + $cy); //output vertex
    }
    glEnd();
}

function Initialize() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

glutInit($argc, $argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(950, 500);
glutInitWindowPosition(200, 200);
glutCreateWindow("Universum");
Initialize();
glutDisplayFunc($Draw);
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1.0, 1.0, 1.0);

glutMainLoop();
