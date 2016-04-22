<?php

/**
 * Example of how to draw a bezier mesh on the screen using the PHP-OpenGL extension.
 *
 * @author Santiago Lizardo <santiagolizardo@php.net>
 */
require 'bootstrap.php';

class Renderer {

    private $controlPoints = [
        -1.5, -1.5, 4.0, -0.5, -1.5, 2.0,
        0.5, -1.5, -1.0,
        1.5, -1.5, 2.0, -1.5, -0.5, 1.0,
        -0.5, -0.5, 3.0,
        0.5, -0.5, 0.0,
        1.5, -0.5, -1.0,
        -1.5, 0.5, 4.0,
        -0.5, 0.5, 0.0,
        0.5, 0.5, 3.0,
        1.5, 0.5, 4.0,
        -1.5, 1.5, -2.0,
        -0.5, 1.5, -2.0,
        0.5, 1.5, 0.0,
        1.5, 1.5, -1.0
    ];

    public function init() {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glEnable(GL_DEPTH_TEST);
        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, $this->controlPoints);
        glEnable(GL_MAP2_VERTEX_3);
        glEnable(GL_AUTO_NORMAL);
        glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

        $this->initLights();
    }

    public function initLights() {
        $ambient = [ 0.2, 0.2, 0.2, 1.0];
        $position = [ 0.0, 0.0, 2.0, 1.0];
        $diffuseMaterial = [ 0.6, 0.6, 0.6, 1.0];
        $specularMaterial = [ 1.0, 1.0, 1.0, 1.0];
        $shinimessMaterial = [ 50.0];

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glLightfv(GL_LIGHT0, GL_AMBIENT, $ambient);
        glLightfv(GL_LIGHT0, GL_POSITION, $position);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, $diffuseMaterial);
        glMaterialfv(GL_FRONT, GL_SPECULAR, $specularMaterial);
        glMaterialfv(GL_FRONT, GL_SHININESS, $shinimessMaterial);
    }

    public function display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glRotatef(85.0, 1.0, 1.0, 1.0);
        glEvalMesh2(GL_FILL, 0, 20, 0, 20);
        glPopMatrix();
        glFlush();
    }

    public function reshape($w, $h) {
        glViewport(0, 0, $w, $h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if ($w <= $h) {
            glOrtho(-4.0, 4.0, -4.0 * $h / $w, 4.0 * $h / $w, -4.0, 4.0);
        } else {
            glOrtho(-4.0 * $w / $h, 4.0 * $w / $h, -4.0, 4.0, -4.0, 4.0);
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    public function keyboard($key, $x, $y) {
        exit(0);
    }

}

glutInit($argc, $argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(500, 500);
glutInitWindowPosition(100, 100);
glutCreateWindow('PHP-OpenGL: Bezier mesh example');

$renderer = new Renderer;
$renderer->init();

glutReshapeFunc([ $renderer, 'reshape']);
glutDisplayFunc([ $renderer, 'display']);
glutKeyboardFunc([ $renderer, 'keyboard']);
glutMainLoop();

