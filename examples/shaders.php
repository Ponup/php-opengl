<?php

require 'bootstrap.php';

$vertexSource = <<<SHADER
#version 130

in vec2 position;
in vec3 color;
out vec3 Color;

void main() {
    Color = color;
    gl_Position = vec4(position, 0.0, 1.0);
}
SHADER;

$fragmentSource = <<<SHADER
#version 130

in vec3 Color;
out vec4 outColor;

void main() {
    outColor = vec4(Color, 1.0);
}
SHADER;


// https://open.gl/content/code/c2_triangle_elements.txt
/*
  glDeleteProgram($shaderProgram);
  glDeleteShader($fragmentShader);
  glDeleteShader($vertexShader);

  glDeleteBuffers(1, $ebo);
  glDeleteBuffers(1, $vbo);

  glDeleteVertexArrays(1, $vao);
 */
glutInit($argc, $argv);
//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
//glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(800, 600);
glutCreateWindow("PHP-OpenGL vertex arrays example");

/*
  glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glReadBuffer(GL_BACK);
  glDrawBuffer(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(TRUE);
  glDisable(GL_STENCIL_TEST);
  glStencilMask(0xFFFFFFFF);
  glStencilFunc(GL_EQUAL, 0x00000000, 0x00000001);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glClearColor(1.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0);
  glClearStencil(0);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_DITHER); */
//glActiveTexture(GL_TEXTURE0);
glEnable(GL_DEPTH_TEST);

$major = array();
$minor = array();
glGetIntegerv(GL_MAJOR_VERSION, $major, 1);
glGetIntegerv(GL_MINOR_VERSION, $minor, 1);

// Create Vertex Array Object
glGenVertexArrays(1, $vaos);
$vao = array_pop($vaos);
glBindVertexArray($vao);

// Create a Vertex Buffer Object and copy the vertex data to it;
glGenBuffers(1, $vbos);
$vbo = array_pop($vbos);

$vertices = [
    -0.5, 0.5, 1.0, 0.0, 0.0, // Top-left
    0.5, 0.5, 0.0, 1.0, 0.0, // Top-right
    0.5, -0.5, 0.0, 0.0, 1.0, // Bottom-right
    -0.5, -0.5, 1.0, 1.0, 1.0  // Bottom-left
];

glBindBuffer(GL_ARRAY_BUFFER, $vbo);
glBufferData(GL_ARRAY_BUFFER, count($vertices), $vertices, GL_STATIC_DRAW);

// Create an element array
glGenBuffers(1, $ebos);
$ebo = array_pop($ebos);

$elements = [
    0, 1, 2,
    2, 3, 0
];

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, $ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof($elements), $elements, GL_STATIC_DRAW);

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
glAttachShader($shaderProgram, $vertexShader);
glAttachShader($shaderProgram, $fragmentShader);
glBindFragDataLocation($shaderProgram, 0, "outColor");
glLinkProgram($shaderProgram);

$displayCallback = function() use ($shaderProgram, $vao) {
    echo '.';
    // Clear the screen to black
    glClearColor(0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram($shaderProgram);
    // Specify the layout of the vertex data
    $posAttrib = glGetAttribLocation($shaderProgram, "position");
    glEnableVertexAttribArray($posAttrib);
    glVertexAttribPointer($posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * 1, 0);

    $colAttrib = glGetAttribLocation($shaderProgram, "color");
    glEnableVertexAttribArray($colAttrib);
    glVertexAttribPointer($colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * 1, 6);

    glBindVertexArray($vao);

    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    // Swap buffers
glFLush();
//    glutSwapBuffers();
};
glutDisplayFunc($displayCallback);

$reshapeCallback = function($w, $h) {
    glViewport(0, 0, $w, $h);
};
glutReshapeFunc($reshapeCallback);

glutMainLoop();

