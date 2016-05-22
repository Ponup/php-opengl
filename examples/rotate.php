<?php
//http://learnopengl.com/code_viewer.php?code=getting-started/transformations
require 'bootstrap.php';
require 'shader.php';

require 'vendor/autoload.php';

use \glm\vec3;
use \glm\mat4;

define('WIDTH', 800);
define('HEIGHT', 600);

glutInit($argc, $argv);
glutInitContextVersion(3, 3);
glutInitContextProfile(GLUT_CORE_PROFILE);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
glutInitWindowSize(WIDTH, HEIGHT);
glutCreateWindow('Php');

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Build and compile our shader program
    $ourShader = new Shader("shaders/transformations.vert", "shaders/transformations.frag");

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

    // Game loop
    $displayFunc = function() use($texture1, $texture2, $ourShader, $VAO) {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

        // Render
        // Clear the colorbuffer
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, $texture1);
        glUniform1i(glGetUniformLocation($ourShader->Program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, $texture2);
        glUniform1i(glGetUniformLocation($ourShader->Program, "ourTexture2"), 1);

        // Activate shader
        $ourShader->Use();       

        // Create transformations
        static $a = 0;
        $a -= 0.01;
        $transform = new mat4;
        $transform = \glm\translate($transform, new vec3(0.5, -0.5, 0.0));
        $transform = \glm\rotate($transform, ($a * 50.0), new vec3(0.0, 0.0, 1.0));

        // Get matrix's uniform location and set matrix
        $transformLoc = glGetUniformLocation($ourShader->Program, "transform");
        glUniformMatrix4fv($transformLoc, 1, GL_FALSE, \glm\value_ptr($transform));
        
        // Draw container
        glBindVertexArray($VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glutSwapBuffers();
    };

    $idleFunc = function() {
        glutPostRedisplay();
    };

    glutDisplayFunc($displayFunc);
    glutIdleFunc($idleFunc);
    glutMainLoop();
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, $VAO);
    glDeleteBuffers(1, $VBO);
    glDeleteBuffers(1, $EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.

