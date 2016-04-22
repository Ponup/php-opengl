<?php
require 'bootstrap.php';

$vertexSource = <<<SHADER
#version 150 core

in vec2 position;
in vec3 color;
out vec3 Color;

void main() {
    Color = color;
    gl_Position = vec4(position, 0.0, 1.0);
}
SHADER;

$fragmentSource = <<<SHADER
#version 150 core

in vec3 Color;
out vec4 outColor;

void main() {
    outColor = vec4(Color, 1.0);
}
SHADER;

// Create Vertex Array Object
$vao;
glGenVertexArrays(1, $vao);
var_dump($vao);
glBindVertexArray($vao);

// Create a Vertex Buffer Object and copy the vertex data to it
$vbo;
glGenBuffers(1, $vbo);

$vertices = [
    -0.5, 0.5, 1.0, 0.0, 0.0, // Top-left
    0.5, 0.5, 0.0, 1.0, 0.0, // Top-right
    0.5, -0.5, 0.0, 0.0, 1.0, // Bottom-right
    -0.5, -0.5, 1.0, 1.0, 1.0  // Bottom-left
];

glBindBuffer(GL_ARRAY_BUFFER, $vbo);
glBufferData(GL_ARRAY_BUFFER, count($vertices), $vertices, GL_STATIC_DRAW);

// Create an element array
$ebo;
glGenBuffers(1, $ebo);

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
glUseProgram($shaderProgram);

// Specify the layout of the vertex data
$posAttrib = glGetAttribLocation($shaderProgram, "position");
glEnableVertexAttribArray(posAttrib);
glVertexAttribPointer($posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

$colAttrib = glGetAttribLocation($shaderProgram, "color");
glEnableVertexAttribArray($colAttrib);
glVertexAttribPointer($colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 6);

$running = true;
while ($running) {
    // Clear the screen to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap buffers
}

glDeleteProgram($shaderProgram);
glDeleteShader($fragmentShader);
glDeleteShader($vertexShader);

glDeleteBuffers(1, $ebo);
glDeleteBuffers(1, $vbo);

glDeleteVertexArrays(1, $vao);

