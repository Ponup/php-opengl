<?php

require 'bootstrap.php';

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("Fixed pipeline example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
SDL_GL_CreateContext($window);

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

$vertexSource = <<<SHADER
#version 330 core
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
SHADER;

$fragmentSource = <<<SHADER
#version 330 core
out vec4 color;
void main() {
    color = vec4(1.0, 0.5, 0.5, 1.0);
}
SHADER;

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

$event = new SDL_Event;

while(true) {
    glClearColor(.2, .3, .3, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram($shaderProgram);
    glBindVertexArray($vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
    glBindVertexArray(0);
	SDL_GL_SwapWindow($window);

	SDL_Delay(50);
	SDL_PollEvent($event);
	if($event->type == SDL_KEYDOWN) break; 
}

SDL_DestroyWindow($window);

