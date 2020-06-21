<?php

require 'bootstrap.php';
require 'vendor/autoload.php';

use \Mammoth\Graphic\Camera;
use \Mammoth\Graphic\Shader;
use Mammoth\Math\Matrix;
use Mammoth\Math\Transform;
use Mammoth\Math\Vector;

// Window dimensions
define('WIDTH', 800);
define('HEIGHT', 600);

// Camera
$camera = new Camera(new Vector(0, 0, 3));
$lastX  =  WIDTH  / 2.0;
$lastY  =  HEIGHT / 2.0;
$keys = array_fill_keys(range('a', 'z'), false);


// Light attributes
$lightPos = new Vector(1.2, 1.0, 2.0);

// Deltatime
$deltaTime = 0.0;   // Time between current frame and last frame
$lastFrame = 0.0;   // Time of last frame

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("Fixed pipeline example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
SDL_GL_CreateContext($window);

// Define the viewport dimensions
glViewport(0, 0, WIDTH, HEIGHT);

// OpenGL options
glEnable(GL_DEPTH_TEST);

$lightingShader = new Shader\Program;
$lightingShader->add(new Shader\Vertex("shaders/basic_lighting.vs"));
$lightingShader->add(new Shader\Fragment("shaders/basic_lighting.frag"));
$lightingShader->compile();
$lightingShader->link();

$lampShader = new shader\Program;
$lampShader->add(new Shader\Vertex("shaders/lamp.vs"));
$lampShader->add(new Shader\Fragment("shaders/lamp.frag"));
$lampShader->compile();
$lampShader->link();

$mouse_callback = function($xpos, $ypos)
{
    global $camera;
    $lastX = &$GLOBALS['lastX'];
    $lastY = &$GLOBALS['lastY'];
    static $firstMouse = true;
    if ($firstMouse)
    {
        $lastX = $xpos;
        $lastY = $ypos;
        $firstMouse = false;
    }

    $xoffset = $xpos - $lastX;
    $yoffset = $lastY - $ypos;  // Reversed since y-coordinates go from bottom to left

    $lastX = $xpos;
    $lastY = $ypos;

    $camera->ProcessMouseMovement($xoffset, $yoffset);
};

// Set up vertex data (and buffer(s)) and attribute pointers
$vertices = [
    -0.5, -0.5, -0.5,  0.0,  0.0, -1.0,
     0.5, -0.5, -0.5,  0.0,  0.0, -1.0,
     0.5,  0.5, -0.5,  0.0,  0.0, -1.0,
     0.5,  0.5, -0.5,  0.0,  0.0, -1.0,
    -0.5,  0.5, -0.5,  0.0,  0.0, -1.0,
    -0.5, -0.5, -0.5,  0.0,  0.0, -1.0,

    -0.5, -0.5,  0.5,  0.0,  0.0,  1.0,
     0.5, -0.5,  0.5,  0.0,  0.0,  1.0,
     0.5,  0.5,  0.5,  0.0,  0.0,  1.0,
     0.5,  0.5,  0.5,  0.0,  0.0,  1.0,
    -0.5,  0.5,  0.5,  0.0,  0.0,  1.0,
    -0.5, -0.5,  0.5,  0.0,  0.0,  1.0,

    -0.5,  0.5,  0.5, -1.0,  0.0,  0.0,
    -0.5,  0.5, -0.5, -1.0,  0.0,  0.0,
    -0.5, -0.5, -0.5, -1.0,  0.0,  0.0,
    -0.5, -0.5, -0.5, -1.0,  0.0,  0.0,
    -0.5, -0.5,  0.5, -1.0,  0.0,  0.0,
    -0.5,  0.5,  0.5, -1.0,  0.0,  0.0,

     0.5,  0.5,  0.5,  1.0,  0.0,  0.0,
     0.5,  0.5, -0.5,  1.0,  0.0,  0.0,
     0.5, -0.5, -0.5,  1.0,  0.0,  0.0,
     0.5, -0.5, -0.5,  1.0,  0.0,  0.0,
     0.5, -0.5,  0.5,  1.0,  0.0,  0.0,
     0.5,  0.5,  0.5,  1.0,  0.0,  0.0,

    -0.5, -0.5, -0.5,  0.0, -1.0,  0.0,
     0.5, -0.5, -0.5,  0.0, -1.0,  0.0,
     0.5, -0.5,  0.5,  0.0, -1.0,  0.0,
     0.5, -0.5,  0.5,  0.0, -1.0,  0.0,
    -0.5, -0.5,  0.5,  0.0, -1.0,  0.0,
    -0.5, -0.5, -0.5,  0.0, -1.0,  0.0,

    -0.5,  0.5, -0.5,  0.0,  1.0,  0.0,
     0.5,  0.5, -0.5,  0.0,  1.0,  0.0,
     0.5,  0.5,  0.5,  0.0,  1.0,  0.0,
     0.5,  0.5,  0.5,  0.0,  1.0,  0.0,
    -0.5,  0.5,  0.5,  0.0,  1.0,  0.0,
    -0.5,  0.5, -0.5,  0.0,  1.0,  0.0
];
// First, set the container's VAO (and VBO)
glGenVertexArrays(1, $containerVAOS); $containerVAO = $containerVAOS[0];
glGenBuffers(1, $VBOS); $VBO = $VBOS[0];

glBindBuffer(GL_ARRAY_BUFFER, $VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof($vertices) * 4, $vertices, GL_STATIC_DRAW);

glBindVertexArray($containerVAO);
// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * 4, 0);
glEnableVertexAttribArray(0);
// Normal attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * 4, (3 * 4));
glEnableVertexAttribArray(1);
glBindVertexArray(0);

// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
glGenVertexArrays(1, $lightVAOS); $lightVAO = $lightVAOS[0];
glBindVertexArray($lightVAO);
// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
glBindBuffer(GL_ARRAY_BUFFER, $VBO);
// Set the vertex attributes (only position data for the lamp))
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * 4, 0); // Note that we skip over the normal vectors
glEnableVertexAttribArray(0);
glBindVertexArray(0);

$event = new SDL_Event;

while(true) {
        // Calculate deltatime of current frame
    $currentFrame = microtime(true);
    $deltaTime = $currentFrame - $lastFrame;
    $lastFrame = $currentFrame;

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    do_movement();

    // Clear the colorbuffer
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use cooresponding shader when setting uniforms/drawing objects
    $lightingShader->Use();
    $objectColorLoc = glGetUniformLocation($lightingShader->getId(), "objectColor");
    glUniform3f($objectColorLoc,  1.0, .5, 0.31);
    $lightColorLoc  = glGetUniformLocation($lightingShader->getId(), "lightColor");
    glUniform3f($lightColorLoc,  1.0, 1.0, 1.0);
    $lightPosLoc    = glGetUniformLocation($lightingShader->getId(), "lightPos");
    glUniform3f($lightPosLoc,    $lightPos->x, $lightPos->y, $lightPos->z);
    $viewPosLoc     = glGetUniformLocation($lightingShader->getId(), "viewPos");
    glUniform3f($viewPosLoc,     $camera->position->x, $camera->position->y, $camera->position->z);

    // Create camera transformations
    $view = new Matrix();
    $view = $camera->GetViewMatrix();
    $projection = Transform::perspective($camera->zoom, floatval(WIDTH / HEIGHT), 0.1, 100.0);
    // Get the uniform locations
    $modelLoc = glGetUniformLocation($lightingShader->getId(), "model");
    $viewLoc  = glGetUniformLocation($lightingShader->getId(),  "view");
    $projLoc  = glGetUniformLocation($lightingShader->getId(),  "projection");
    // Pass the matrices to the shader
    glUniformMatrix4fv($viewLoc, 1, GL_FALSE, $view->toRowVector());
    glUniformMatrix4fv($projLoc, 1, GL_FALSE, $projection->toRowVector());

    // Draw the container (using container's vertex attributes)
    glBindVertexArray($containerVAO);
    $model = new Matrix();
    glUniformMatrix4fv($modelLoc, 1, GL_FALSE, $model->toRowVector());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Also draw the lamp object, again binding the appropriate shader
    $lampShader->Use();
    // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
    $modelLoc = glGetUniformLocation($lampShader->getId(), "model");
    $viewLoc  = glGetUniformLocation($lampShader->getId(), "view");
    $projLoc  = glGetUniformLocation($lampShader->getId(), "projection");
    // Set matrices
    glUniformMatrix4fv($viewLoc, 1, GL_FALSE, $view->toRowVector());
    glUniformMatrix4fv($projLoc, 1, GL_FALSE, $projection->toRowVector());
    $model = new Matrix;
    $model = Transform::translate($model, $lightPos);
    $model = $model->scale(0.2); // Make it a smaller cube
    glUniformMatrix4fv($modelLoc, 1, GL_FALSE, $model->toRowVector());
    // Draw the light object (using light's vertex attributes)
    glBindVertexArray($lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Swap the screen buffers
    SDL_GL_SwapWindow($window);
	while(SDL_PollEvent($event)) {
	switch($event->type) {
	case SDL_MOUSEMOTION:
		$mouse_callback($event->motion->x, $event->motion->y);
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
	}
	}


	SDL_Delay(10);
}

function do_movement()
{
    global $deltaTime;
    $camera = &$GLOBALS['camera'];
    $keys = &$GLOBALS['keys'];
    // Camera controls
    if ($keys['w']) {
        $camera->ProcessKeyboard(Camera::FORWARD, $deltaTime);
    }
    if ($keys['s']) {
        $camera->ProcessKeyboard(Camera::BACKWARD, $deltaTime);
    }
    if ($keys['a']) {
        $camera->ProcessKeyboard(Camera::LEFT, $deltaTime);
    }
    if ($keys['d']) {
        $camera->ProcessKeyboard(Camera::RIGHT, $deltaTime);
    }
}

