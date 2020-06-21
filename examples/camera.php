<?php

require 'bootstrap.php';
require 'vendor/autoload.php';

use Mammoth\Graphic\ImageLoader;
use \Mammoth\Graphic\Shader;
use Mammoth\Math\Angle;
use Mammoth\Math\Matrix;
use Mammoth\Math\Transform;
use Mammoth\Math\Vector;

// http://learnopengl.com/code_viewer.php?code=getting-started/camera_zoom

const WIDTH = 1024;
const HEIGHT = 768;

// Camera
$cameraPos   = new Vector(0.0, 0.0,  3.0);
$cameraFront = new Vector(0.0, 0.0, -1.0);
$cameraUp    = new Vector(0.0, 1.0,  0.0);

$yaw    = -90.0;    // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
$pitch  =  0.0;

$lastMouseCoordinates = null;

$fov =  45.0;
$keys = array_fill_keys(range('a', 'z'), false);

// Deltatime
$deltaTime = 0.0;   // Time between current frame and last frame
$lastFrame = 0.0;   // Time of last frame

function updateCameraPosition()
{
    global $keys, $deltaTime, $cameraUp, $cameraFront;
    $cameraPos = &$GLOBALS['cameraPos'];
    // Camera controls
    $cameraSpeed = 5.0 * $deltaTime;
    if ($keys['w']) {
        $cameraPos = $cameraPos->add($cameraFront->scale($cameraSpeed));
    }
    if ($keys['s']) {
        $cameraPos = $cameraPos->substract($cameraFront->scale($cameraSpeed));
    }
    if ($keys['a']) {
        $cameraPos = $cameraPos->substract(
            $cameraFront->cross($cameraUp)->normalize()->scale($cameraSpeed)
        );
    }
    if ($keys['d']) {
        $cameraPos = $cameraPos->add(
            $cameraFront->cross($cameraUp)->normalize()->scale($cameraSpeed)
        );
    }
}
$scroll_callback = function ($wheel, $direction, $x, $y) {
    $fov = &$GLOBALS['fov'];
    if ($fov >= 1.0 && $fov <= 45.0)
        $fov -= $direction;
    if ($fov <= 1.0)
        $fov = 1.0;
    if ($fov >= 45.0)
        $fov = 45.0;
};
$motion_callback = function ($xpos, $ypos) use(&$lastMouseCoordinates) {
    $yaw = &$GLOBALS['yaw'];
    $pitch = &$GLOBALS['pitch'];

    if (null === $lastMouseCoordinates) {
        $lastMouseCoordinates = new Vector($xpos-(WIDTH>>1), $ypos+(HEIGHT>>1));
    }

    $xoffset = $xpos - $lastMouseCoordinates->x;
    $yoffset = $lastMouseCoordinates->y - $ypos; // Reversed since y-coordinates go from bottom to left
    $lastMouseCoordinates = new Vector($xpos, $ypos);

    $sensitivity = 0.1; // Change this value to your liking
    $xoffset *= $sensitivity;
    $yoffset *= $sensitivity;

    $yaw   += $xoffset;
    $pitch += $yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if ($pitch > 89.0)
        $pitch = 89.0;
    if ($pitch < -89.0)
        $pitch = -89.0;

    $front = new Vector(
            cos(Angle::toRadians($yaw)) * cos(Angle::toRadians($pitch)),
            sin(Angle::toRadians($pitch)),
            sin(Angle::toRadians($yaw)) * cos(Angle::toRadians($pitch))
        );
    $GLOBALS['cameraFront'] = $front->normalize();
};

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("Fixed pipeline example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
SDL_GL_CreateContext($window);

// Define the viewport dimensions
glViewport(0, 0, WIDTH, HEIGHT);

glEnable(GL_DEPTH_TEST);
//glDepthFunc(GL_LESS);

//glFrontFace(GL_CCW);;

// Build and compile our shader program
$shaderProgram = new Shader\Program;
$shaderProgram->add(new Shader\Vertex("shaders/camera.vert"));
$shaderProgram->add(new Shader\Fragment("shaders/camera.frag"));
$shaderProgram->compile();
$shaderProgram->link();

// Set up vertex data (and buffer(s)) and attribute pointers
$vertices = [
    -0.5, -0.5, -0.5,  0.0, 0.0,
    0.5, -0.5, -0.5,  1.0, 0.0,
    0.5,  0.5, -0.5,  1.0, 1.0,
    0.5,  0.5, -0.5,  1.0, 1.0,
    -0.5,  0.5, -0.5,  0.0, 1.0,
    -0.5, -0.5, -0.5,  0.0, 0.0,

    -0.5, -0.5,  0.5,  0.0, 0.0,
    0.5, -0.5,  0.5,  1.0, 0.0,
    0.5,  0.5,  0.5,  1.0, 1.0,
    0.5,  0.5,  0.5,  1.0, 1.0,
    -0.5,  0.5,  0.5,  0.0, 1.0,
    -0.5, -0.5,  0.5,  0.0, 0.0,

    -0.5,  0.5,  0.5,  1.0, 0.0,
    -0.5,  0.5, -0.5,  1.0, 1.0,
    -0.5, -0.5, -0.5,  0.0, 1.0,
    -0.5, -0.5, -0.5,  0.0, 1.0,
    -0.5, -0.5,  0.5,  0.0, 0.0,
    -0.5,  0.5,  0.5,  1.0, 0.0,

    0.5,  0.5,  0.5,  1.0, 0.0,
    0.5,  0.5, -0.5,  1.0, 1.0,
    0.5, -0.5, -0.5,  0.0, 1.0,
    0.5, -0.5, -0.5,  0.0, 1.0,
    0.5, -0.5,  0.5,  0.0, 0.0,
    0.5,  0.5,  0.5,  1.0, 0.0,

    -0.5, -0.5, -0.5,  0.0, 1.0,
    0.5, -0.5, -0.5,  1.0, 1.0,
    0.5, -0.5,  0.5,  1.0, 0.0,
    0.5, -0.5,  0.5,  1.0, 0.0,
    -0.5, -0.5,  0.5,  0.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, 1.0,

    -0.5,  0.5, -0.5,  0.0, 1.0,
    0.5,  0.5, -0.5,  1.0, 1.0,
    0.5,  0.5,  0.5,  1.0, 0.0,
    0.5,  0.5,  0.5,  1.0, 0.0,
    -0.5,  0.5,  0.5,  0.0, 0.0,
    -0.5,  0.5, -0.5,  0.0, 1.0
];

$cubePositions = [
    new Vector(0.0,  0.0,  0.0),
    new Vector(2.0,  5.0, -15.0),
    new Vector(-1.5, -2.2, -2.5),
    new Vector(-3.8, -2.0, -12.3),
    new Vector(2.4, -0.4, -3.5),
    new Vector(-1.7,  3.0, -7.5),
    new Vector(1.3, -2.0, -2.5),
    new Vector(1.5,  2.0, -2.5),
    new Vector(1.5,  0.2, -1.5),
    new Vector(-1.3,  1.0, -1.5)
];

glGenVertexArrays(1, $VAOS);
$VAO = $VAOS[0];
glGenBuffers(1, $VBOS);
$VBO = $VBOS[0];

glBindVertexArray($VAO);

glBindBuffer(GL_ARRAY_BUFFER, $VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof($vertices) * 4, $vertices, GL_STATIC_DRAW);

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * 4, 0);
glEnableVertexAttribArray(0);
// TexCoord attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * 4, (3 * 4));
glEnableVertexAttribArray(2);

glBindVertexArray(0); // Unbind VAO

$imageLoader = new ImageLoader;

// Load and create a texture 
$textures = [];
// ====================
// Texture 1
// ====================
glGenTextures(1, $textures);
$texture1 = $textures[0];
glBindTexture(GL_TEXTURE_2D, $texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
// Set our texture parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // Set texture wrapping to GL_REPEAT
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// Set texture filtering
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Load, create texture and generate mipmaps
$image = $imageLoader->load("textures/container.jpg", $width, $height);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, $width, $height, 0, GL_BGRA, GL_UNSIGNED_BYTE, $image);
glGenerateMipmap(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
// ===================
// Texture 2
// ===================
$textures2 = [];
glGenTextures(1, $textures2);
$texture2 = $textures2[0];
glBindTexture(GL_TEXTURE_2D, $texture2);
// Set our texture parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// Set texture filtering
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Load, create texture and generate mipmaps
$image = $imageLoader->load("textures/awesomeface.png", $width, $height);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, $width, $height, 0, GL_BGRA, GL_UNSIGNED_BYTE, $image);
glGenerateMipmap(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, 0);

$event = new SDL_Event;

$quit = false;

// Game loop
while (!$quit) {
    // Calculate deltatime of current frame
    $currentFrame = microtime(true);
    $deltaTime = $currentFrame - $lastFrame;
    $lastFrame = $currentFrame;

    while (SDL_PollEvent($event)) {
        switch ($event->type) {
            case SDL_MOUSEMOTION:
                $motion_callback($event->motion->x, $event->motion->y);
                break;
            case SDL_KEYDOWN:
                $symChar = chr($event->key->keysym->sym);
                $keys['w'] = $symChar == 'w';
                $keys['s'] = $symChar == 's';
                $keys['a'] = $symChar == 'a';
                $keys['d'] = $symChar == 'd';
                break;
            case SDL_KEYUP:
                $keys = array_fill_keys(range('a', 'z'), false);
                break;
            case SDL_QUIT:
                $quit = true;
                break;
        }
    }

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    updateCameraPosition();

    // Render
    // Clear the colorbuffer
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind Textures using texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, $texture1);
    glUniform1i(glGetUniformLocation($shaderProgram->getId(), "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, $texture2);
    glUniform1i(glGetUniformLocation($shaderProgram->getId(), "ourTexture2"), 1);

    // Activate shader
    $shaderProgram->Use();

    // Projection 
    $projection = Transform::perspective($fov, (float)WIDTH / (float)HEIGHT, 0.1, 100.0);
    // Get the uniform locations

    // Camera/View transformation
    $view = Transform::lookAt($cameraPos, $cameraPos->add($cameraFront), $cameraUp);

    $modelLoc = glGetUniformLocation($shaderProgram->getId(), "model");
    $viewLoc = glGetUniformLocation($shaderProgram->getId(), "view");
    $projLoc = glGetUniformLocation($shaderProgram->getId(), "projection");
    // Pass the matrices to the shader
    glUniformMatrix4fv($viewLoc, 1, GL_FALSE, $view->toRowVector());
    glUniformMatrix4fv($projLoc, 1, GL_FALSE, $projection->toRowVector());

    glBindVertexArray($VAO);
    for ($i = 0; $i < count($cubePositions); $i++) {
        // Calculate the model matrix for each object and pass it to shader before drawing
        $model = new Matrix();
        $model = Transform::translate($model, $cubePositions[$i]);
        $angle = 20.0 * $i;
        $model = Transform::rotate($model, $angle, new Vector(1.0, 0.3, 0.5));
        glUniformMatrix4fv($modelLoc, 1, GL_FALSE, $model->toRowVector());

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    // Swap the screen buffers
    SDL_GL_SwapWindow($window);
}

glDeleteVertexArrays(1, $VAO);
glDeleteBuffers(1, $VBO);
