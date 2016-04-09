<?php

require 'bootstrap.php';

$displayCallback = function() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glutSolidTeapot(1);
	glFlush();
};

$reshapeCallback = function($w, $h) {
	glViewport(0, 0, $w, $h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-.5, .5, -.5, .5, 1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
};

glutInit($argc, $argv);
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowPosition(0, 0);
glutInitWindowSize(800, 600);
glutCreateWindow('Simple model');
glutDisplayFunc($displayCallback);
glutReshapeFunc($reshapeCallback);
glutMainLoop();

