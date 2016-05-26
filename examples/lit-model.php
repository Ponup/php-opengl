<?php

require 'bootstrap.php';
require 'vendor/autoload.php';

use \glm\vec3;
use \glm\mat4;
use \Ponup\ddd\Camera;
use \Ponup\ddd\Shader;

// Window dimensions
define('WIDTH', 800);
define('HEIGHT', 600);

// Camera
$camera = new Camera(new vec3(0, 0, 3));
$lastX  =  WIDTH  / 2.0;
$lastY  =  HEIGHT / 2.0;
$keys = array_fill_keys(range('a', 'z'), false);


// Light attributes
$lightPos = new vec3(1.2, 1.0, 2.0);

// Deltatime
$deltaTime = 0.0;   // Time between current frame and last frame
$lastFrame = 0.0;   // Time of last frame

// The MAIN function, from here we start the application and run the game loop
    // Init GLFW
glutInit($argc, $argv);
glutInitContextVersion(3, 3);
glutInitContextProfile(GLUT_CORE_PROFILE);
glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
glutInitWindowSize(WIDTH, HEIGHT);
glutCreateWindow('LearnOpengl');

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    $lightingShader = new Shader;
    $lightingShader->compileFromPath("shaders/basic_lighting.vs", "shaders/basic_lighting.frag");
    $lampShader = new shader;
    $lampShader->compileFromPath("shaders/lamp.vs", "shaders/lamp.frag");

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
    

    $displayCallback = function() use($lightPos, $camera, $containerVAO, $lightVAO) {
            // Calculate deltatime of current frame
        $lightingShader = &$GLOBALS['lightingShader'];
        $lampShader = &$GLOBALS['lampShader'];
        $lastFrame = &$GLOBALS['lastFrame'];
        $deltaTime = &$GLOBALS['deltaTime'];
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
        $objectColorLoc = glGetUniformLocation($lightingShader->programId, "objectColor");
        glUniform3f($objectColorLoc,  1.0, .5, 0.31);
        $lightColorLoc  = glGetUniformLocation($lightingShader->programId, "lightColor");
        glUniform3f($lightColorLoc,  1.0, 1.0, 1.0);
        $lightPosLoc    = glGetUniformLocation($lightingShader->programId, "lightPos");
        glUniform3f($lightPosLoc,    $lightPos->x, $lightPos->y, $lightPos->z);
        $viewPosLoc     = glGetUniformLocation($lightingShader->programId, "viewPos");
        glUniform3f($viewPosLoc,     $camera->position->x, $camera->position->y, $camera->position->z);

        // Create camera transformations
        $view = new mat4;
        $view = $camera->GetViewMatrix();
        $projection = \glm\perspective($camera->zoom, floatval(WIDTH / HEIGHT), 0.1, 100.0);
        // Get the uniform locations
        $modelLoc = glGetUniformLocation($lightingShader->programId, "model");
        $viewLoc  = glGetUniformLocation($lightingShader->programId,  "view");
        $projLoc  = glGetUniformLocation($lightingShader->programId,  "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv($viewLoc, 1, GL_FALSE, \glm\value_ptr($view));
        glUniformMatrix4fv($projLoc, 1, GL_FALSE, \glm\value_ptr($projection));

        // Draw the container (using container's vertex attributes)
        glBindVertexArray($containerVAO);
        $model = new mat4;
        glUniformMatrix4fv($modelLoc, 1, GL_FALSE, \glm\value_ptr($model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Also draw the lamp object, again binding the appropriate shader
        $lampShader->Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        $modelLoc = glGetUniformLocation($lampShader->programId, "model");
        $viewLoc  = glGetUniformLocation($lampShader->programId, "view");
        $projLoc  = glGetUniformLocation($lampShader->programId, "projection");
        // Set matrices
        glUniformMatrix4fv($viewLoc, 1, GL_FALSE, \glm\value_ptr($view));
        glUniformMatrix4fv($projLoc, 1, GL_FALSE, \glm\value_ptr($projection));
        $model = new mat4;
        $model = \glm\translate($model, $lightPos);
        $model = $model->scale(0.2); // Make it a smaller cube
        glUniformMatrix4fv($modelLoc, 1, GL_FALSE, \glm\value_ptr($model));
        // Draw the light object (using light's vertex attributes)
        glBindVertexArray($lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Swap the screen buffers
        glutSwapBuffers();
    };
    
$onKeyDownCallback= function($key, $x, $y) 
{
    global $keys;
    if ($key >= 0 && $key < 1024)
    {
        $keys[ $key ] = true;
    }
};
$onKeyUpCallback= function($key, $x, $y) 
{
    global $keys;
    if ($key >= 0 && $key < 1024)
    {
        $keys[ $key ] = false;
    }
};

$mouse_callback= function($xpos, $ypos)
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

glutPassiveMotionFunc($mouse_callback);
glutKeyboardFunc($onKeyDownCallback);
glutKeyboardUpFunc($onKeyUpCallback);

    // Terminate GLFW, clearing any resources allocated by GLFW.

    // Set the required callback functions
glutDisplayFunc($displayCallback);
$idleFunc = function() {
    glutPostRedisplay();
};
glutIdleFunc($idleFunc);
//$displayCallback();
glutMainLoop();

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



/*


// Is called whenever a key is pressed/released via GLFW
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
*/
