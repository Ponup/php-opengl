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

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("Basic shader example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
SDL_GL_CreateContext($window);

// https://open.gl/content/code/c2_triangle_elements.txt

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
glBufferData(GL_ARRAY_BUFFER, count($vertices) * 4, $vertices, GL_STATIC_DRAW);

// Create an element array
glGenBuffers(1, $ebos);
$ebo = array_pop($ebos);

$elements = [
    0, 1, 2,
    2, 3, 0
];

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, $ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof($elements) * 4, $elements, GL_STATIC_DRAW);

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
glEnableVertexAttribArray($posAttrib);
glVertexAttribPointer($posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * 4, 0);

$colAttrib = glGetAttribLocation($shaderProgram, "color");
glEnableVertexAttribArray($colAttrib);
glVertexAttribPointer($colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * 4, 2 * 4);

$event = new SDL_Event;

while(true) {
    // Clear the screen to black
    glClearColor(0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle from the 2 triangles using 6 indices
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null);

	SDL_GL_SwapWindow($window);

	SDL_PollEvent($event);
	if($event->type == SDL_KEYDOWN) break;

	SDL_Delay(50);
}


glDeleteProgram($shaderProgram);
glDeleteShader($fragmentShader);
glDeleteShader($vertexShader);

glDeleteBuffers(1, $ebo);
glDeleteBuffers(1, $vbo);

glDeleteVertexArrays(1, $vao);

