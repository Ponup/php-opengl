<?php

require 'bootstrap.php';

glutInit($argc, $argv);
glutInitContextVersion(3, 3);
glutInitContextProfile(GLUT_CORE_PROFILE);

glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
glutInitWindowSize(800, 600);

glutCreateWindow('Rectangle');

   $vertices = [
        +0.5, +.5, 0, // top right
        +0.5, -.5, 0, // bottom right
        -0.5, -.5, 0, // bottom left
        -0.5, +.5, 0, // top left
    ];
    $indices = [
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    ];

    $vertexSource = <<<PPP
#version 330 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
PPP;

    $fragmentSource = <<<PPP
#version 330 core
out vec4 color;
void main() {
    color = vec4(1.0, 0.5, 0.5, 1.0);
}
PPP;

    $vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource($vertexShader, 1, $vertexSource, NULL);
    glCompileShader($vertexShader);

    $fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource($fragmentShader, 1, $fragmentSource, NULL);
    glCompileShader($fragmentShader);

    $shaderProgram = glCreateProgram();
    glAttachShader($shaderProgram, $vertexShader);
    glAttachShader($shaderProgram, $fragmentShader);
    glLinkProgram($shaderProgram);

        glGenVertexArrays(1, $vaos);
        $vao = $vaos[0];
        glGenBuffers(1, $vbos);
        $vbo = $vbos[0];
        glGenBuffers(1, $ebos);
        $ebo = $ebos[0];

    glBindVertexArray($vao);

        glBindBuffer(GL_ARRAY_BUFFER, $vbo);
        glBufferData(GL_ARRAY_BUFFER, count($vertices) * 4, $vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, $ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count($indices) * 4, $indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

$displayFunc = function() use($shaderProgram, $vao) {
    glClearColor(.2, .3, .3, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram($shaderProgram);
    glBindVertexArray($vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
    glBindVertexArray(0);

    glutSwapBuffers();
};

glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

$keyboardFunc = function($key, $x, $y) {
    if(ord($key) === 27) {
        glutLeaveMainLoop();
    }
};

glutDisplayFunc($displayFunc);
glutKeyboardFunc($keyboardFunc);
glutMainLoop();

echo 'Cleainnig resources';

