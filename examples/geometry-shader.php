<?php

require 'bootstrap.php';
require 'vendor/autoload.php';

use \Ponup\ddd\Shader;

const WindowWidth = 1280;
const WindowHeight = 720;

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("Geometry shader example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
SDL_GL_CreateContext($window);

// Define the viewport dimensions
glViewport(0, 0, WindowWidth, WindowHeight);

glEnable(GL_DEPTH_TEST);

$shaderProgram = new Shader\Program;
$shaderProgram->add(new Shader\Vertex("shaders/geometry-shader.vs"));
$shaderProgram->add(new Shader\Fragment("shaders/geometry-shader.fs"));
$shaderProgram->add(new Shader\Geometry("shaders/geometry-shader.gs"));
$shaderProgram->compile();
$shaderProgram->link();

    // set up vertex data (and buffer(s)) and configure vertex attributes
$points = [
    -0.5,  0.5, 1.0, 0.0, 0.0, // top-left
     0.5,  0.5, 0.0, 1.0, 0.0, // top-right
     0.5, -0.5, 0.0, 0.0, 1.0, // bottom-right
    -0.5, -0.5, 1.0, 1.0, 0.0  // bottom-left
];
const SizeOfFloat = 4;

glGenBuffers(1, $VBOS);
$VBO = $VBOS[0];
glGenVertexArrays(1, $VAOS);
$VAO = $VAOS[0];

glBindVertexArray($VAO);
glBindBuffer(GL_ARRAY_BUFFER, $VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof($points), $points, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * SizeOfFloat, 0);
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * SizeOfFloat, (2 * SizeOfFloat));
glBindVertexArray(0);

$shaderProgram->use();

$quit = false;
$event = new SDL_Event;
while (!$quit) {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray($VAO);
    glDrawArrays(GL_POINTS, 0, 4);

    SDL_GL_SwapWindow($window);

    SDL_Delay(10);
    while(SDL_PollEvent($event)) {
        if($event->type == SDL_QUIT) $quit = true;
    }
}

glDeleteVertexArrays(1, $VAO);
glDeleteBuffers(1, $VBO);
