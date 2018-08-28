<?php
//http://learnopengl.com/code_viewer.php?code=getting-started/transformations
require 'bootstrap.php';
require 'vendor/autoload.php';

use \glm\vec3;
use \glm\mat4;
use \Ponup\ddd\Shader;

define('WIDTH', 800);
define('HEIGHT', 600);

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("3.2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,                
                640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);                                                                                               

$context = SDL_GL_CreateContext($window);    

// Define the viewport dimensions
glViewport(0, 0, WIDTH, HEIGHT);

// Build and compile our shader program
$shaderProgram = new Shader\Program;
$shaderProgram->add(new Shader\Vertex("shaders/transformations.vert"));
$shaderProgram->add(new Shader\Fragment("shaders/transformations.frag"));
$shaderProgram->compile();
$shaderProgram->link();
$shaderProgram->use();

// Set up vertex data (and buffer(s)) and attribute pointers
$vertices = [
    // Positions          // Colors           // Texture Coords
     0.5,  0.5, 0.0,   1.0, 0.0, 0.0,   1.0, 1.0, // Top Right
     0.5, -0.5, 0.0,   0.0, 1.0, 0.0,   1.0, 0.0, // Bottom Right
    -0.5, -0.5, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0, // Bottom Left
    -0.5,  0.5, 0.0,   1.0, 1.0, 0.0,   0.0, 1.0  // Top Left 
];
$indices = [  // Note that we start from 0!
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
];
glGenVertexArrays(1, $VAOS); $VAO=$VAOS[0];
glGenBuffers(1, $VBOS); $VBO=$VBOS[0];
glGenBuffers(1, $EBOS); $EBO=$EBOS[0];

glBindVertexArray($VAO);

glBindBuffer(GL_ARRAY_BUFFER, $VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof($vertices) * 4, $vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, $EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof($indices) * 4, $indices, GL_STATIC_DRAW);

// Position attribute
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * 4, 0);
glEnableVertexAttribArray(0);
// Color attribute
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * 4, (3 * 4));
glEnableVertexAttribArray(1);
// TexCoord attribute
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * 4, (6 * 4));
glEnableVertexAttribArray(2);

glBindVertexArray(0); // Unbind VAO

$imageLoader = new \Ponup\GlLoaders\ImageLoader;

// Load and create a texture 
// ====================
// Texture 1
// ====================
$textures =[];
glGenTextures(1, $textures); $texture1=$textures[0];
glBindTexture(GL_TEXTURE_2D, $texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
// Set our texture parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // Set texture wrapping to GL_REPEAT
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// Set texture filtering
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Load, create texture and generate mipmaps
$image = $imageLoader->load("textures/container.jpg", $width, $height);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, $width, $height, 0, GL_RGBA, GL_UNSIGNED_BYTE, $image);
glGenerateMipmap(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
// ===================
// Texture 2
// ===================
$textures2 = [];
glGenTextures(1, $textures2); $texture2=$textures2[0];
glBindTexture(GL_TEXTURE_2D, $texture2);
// Set our texture parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// Set texture filtering
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Load, create texture and generate mipmaps
$image = $imageLoader->load("textures/awesomeface.png", $width, $height);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, $width, $height, 0, GL_BGRA, GL_UNSIGNED_BYTE, $image);
glGenerateMipmap(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, 0);

	$event = new SDL_Event;
while(true) {
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

    // Render
    // Clear the colorbuffer
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind Textures using texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, $texture1);
    glUniform1i(glGetUniformLocation($shaderProgram->getId(), "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, $texture2);
    glUniform1i(glGetUniformLocation($shaderProgram->getId(), "ourTexture2"), 1);

    // Activate shader
    $shaderProgram->Use();       

    // Create transformations
    static $a = 0;
    $a -= 0.01;
    $transform = new mat4;
    $transform = \glm\translate($transform, new vec3(0.5, -0.5, 0.0));
    $transform = \glm\rotate($transform, ($a * 50.0), new vec3(0.0, 0.0, 1.0));

    // Get matrix's uniform location and set matrix
    $transformLoc = glGetUniformLocation($shaderProgram->getId(), "transform");
    glUniformMatrix4fv($transformLoc, 1, GL_FALSE, \glm\value_ptr($transform));
    
    // Draw container
    glBindVertexArray($VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap the screen buffers
    SDL_GL_SwapWindow($window);
	SDL_PollEvent($event);
	if($event->type == SDL_KEYDOWN) break;

	SDL_Delay(5);
}
// Properly de-allocate all resources once they've outlived their purpose
glDeleteVertexArrays(1, $VAO);
glDeleteBuffers(1, $VBO);
glDeleteBuffers(1, $EBO);

