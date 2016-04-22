<?php

require 'bootstrap.php';

/*
 * transparency and alpha values
 * http://wisdom.sakura.ne.jp/system/opengl/gl17.html
 */
$width = 0;
$height = 0;

function Draw() {
    global $width, $height;
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.9, -0.9);
    glVertex2f(0.9, -0.9);
    glVertex2f(0.0, 0.9);
    glEnd();
    glViewport(0, 0, $width, $height);
}

$disp = function() {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glColor4f(1, 0, 0, 0.7);
    glTranslatef(-0.5, 0, 0);
    Draw();
    glPopMatrix();
    glPushMatrix();
    glColor4f(0, 0, 1, 0.5);
    glTranslatef(0.5, 0, 0);
    Draw();
    glPopMatrix();
    glFlush();
};
$reshape = function($w, $h) use($disp) {
    global $width, $height;
    $width = $w;
    $height = $h;
    $disp();
};
glutInit($argc, $argv);
glutInitWindowSize(400, 300);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
glutCreateWindow("Kitty on your lap");
glutDisplayFunc($disp);
glutReshapeFunc($reshape);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE);
glutMainLoop();
