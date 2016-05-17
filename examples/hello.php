<?php

require 'bootstrap.php';

$displayFunc = function() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
};

$idleFunc = function() {
};

function makeResources() {
}

glutInit($argc, $argv);
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
glutInitWindowSize(400, 300);
glutCreateWindow('Hello world');
glutDisplayFunc($displayFunc);
glutIdleFunc($idleFunc);

makeResources();

glutMainLoop();

