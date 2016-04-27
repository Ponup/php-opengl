<?php

require 'bootstrap.php';

//https://open.gl/content/code/c2_triangle.txt

$vertexSource = <<<SQL
#version 130
in vec2 position;
void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}
SQL;

$fragmentSource = <<<SQL
#version 130
out vec4 outColor;
void main()
{
    outColor = vec4(1.0, 1.0, 1.0, 1.0);
}
SQL;

//settings.depthBits = 24;
glutInit($argc, $argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
//glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(800, 600);
glutCreateWindow("PHP-OpenGL vertex arrays example");

glGenVertexArrays(1, $vaos);
$vao = $vaos[0];
var_dump($vao);
glBindVertexArray($vao);

// Create a Vertex Buffer Object and copy the vertex data to it
glGenBuffers(1, $vbos);
$vbo = $vbos[0];

var_dump($vbo);

$vertices = [
    0.0, 0.5,
    0.5, -0.5,
    -0.5, -0.5
];

glBindBuffer(GL_ARRAY_BUFFER, $vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof($vertices), $vertices, GL_STATIC_DRAW);

// Create and compile the vertex shader
$vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource($vertexShader, 1, $vertexSource, NULL);
glCompileShader($vertexShader);

// Create and compile the fragment shader
$fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource($fragmentShader, 1, $fragmentSource, NULL);
glCompileShader($fragmentShader);

// Link the vertex and fragment shader into a shader program
$shaderProgram = glCreateProgram();
var_dump('shaderProgram', $shaderProgram);
glAttachShader($shaderProgram, $vertexShader);
glAttachShader($shaderProgram, $fragmentShader);
glBindFragDataLocation($shaderProgram, 0, "outColor");
glLinkProgram($shaderProgram);
glUseProgram($shaderProgram);

// Specify the layout of the vertex data
$posAttrib = glGetAttribLocation($shaderProgram, "position");
var_dump('getError', glGetError());
var_dump('posAttrib', $posAttrib);
glEnableVertexAttribArray($posAttrib);
glVertexAttribPointer($posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

$displayCallback = function() use ($shaderProgram, $vao) {

    // Clear the screen to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram($shaderProgram);
    // Draw a triangle from the 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers
    glutSwapBuffers();
};
/*
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  glDeleteBuffers(1, &vbo);

  glDeleteVertexArrays(1, &vao);

  window.close();
 *
 * */


glutDisplayFunc($displayCallback);

$reshapeCallback = function($w, $h) {
    glViewport(0, 0, $w, $h);
    assert(glGetError() === 0);
};
glutReshapeFunc($reshapeCallback);

glutMainLoop();

