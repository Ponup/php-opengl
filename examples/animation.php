<?php

// https://open.gl/transformations

require 'bootstrap.php';

require 'vendor/autoload.php';
use \glm\vec3;
use \glm\mat4;

// Shader sources
$vertexSource = <<<GLSL
    #version 150 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    out vec3 Color;
    out vec2 Texcoord;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;
    void main()
    {
        Color = color;
        Texcoord = texcoord;
        gl_Position = proj * view * model * vec4(position, 0.0, 1.0);
    }
GLSL;

$fragmentSource = <<<GLSL
    #version 150 core
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D texKitten;
    uniform sampler2D texPuppy;
    void main()
    {
        outColor = mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.5);
    }
GLSL;

    $t_start = microtime(true);

glutInit($argc, $argv);
glutInitContextVersion(3, 3);
glutInitContextProfile(GLUT_CORE_PROFILE);
glutInitWindowSize(800, 600);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

glutCreateWindow('PHP');

    // Create Vertex Array Object
    glGenVertexArrays(1, $vaos);
    $vao = $vaos[0];
    glBindVertexArray($vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    glGenBuffers(1, $vbos);
    $vbo = $vbos[0];

    $vertices = [
    //  Position      Color             Texcoords
        -0.5,  0.5, 1.0, 0.0, 0.0, 0.0, 0.0, // Top-left
         0.5,  0.5, 0.0, 1.0, 0.0, 1.0, 0.0, // Top-right
         0.5, -0.5, 0.0, 0.0, 1.0, 1.0, 1.0, // Bottom-right
        -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0  // Bottom-left
    ];

    glBindBuffer(GL_ARRAY_BUFFER, $vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof($vertices) * 4, $vertices, GL_STATIC_DRAW);

    // Create an element array
    glGenBuffers(1, $ebos);
    $ebo = $ebos[0];

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
    glVertexAttribPointer($posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * 4, 0);

    $colAttrib = glGetAttribLocation($shaderProgram, "color");
    glEnableVertexAttribArray($colAttrib);
    glVertexAttribPointer($colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * 4, (2 * 4));

    $texAttrib = glGetAttribLocation($shaderProgram, "texcoord");
    glEnableVertexAttribArray($texAttrib);
    glVertexAttribPointer($texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * 4, (5 * 4));

    $imageLoader = new \Ponup\GlLoaders\ImageLoader;

    // Load textures
    $textures = [];
    glGenTextures(2, $textures);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, $textures[0]);
    $image = $imageLoader->load("textures/sample.png", $width, $height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, $width, $height, 0, GL_RGBA, GL_UNSIGNED_BYTE, $image);
    glUniform1i(glGetUniformLocation($shaderProgram, "texKitten"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, $textures[1]);
    $image = $imageLoader->load("textures/sample2.png", $width, $height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, $width, $height, 0, GL_RGBA, GL_UNSIGNED_BYTE, $image);
    glUniform1i(glGetUniformLocation($shaderProgram, "texPuppy"), 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    $uniModel = glGetUniformLocation($shaderProgram, "model");

    // Set up projection
    $view = \glm\lookAt(
        \glm\vec3(1.2, 1.2, 1.2),
        \glm\vec3(0.0, 0.0, 0.0),
        \glm\vec3(0.0, 0.0, 1.0)
    );
    $uniView = glGetUniformLocation($shaderProgram, "view");
    glUniformMatrix4fv($uniView, 1, GL_FALSE, \glm\value_ptr($view));

    $proj = \glm\perspective((45.0), 800.0 / 600.0, 1.0, 10.0);
    $uniProj = glGetUniformLocation($shaderProgram, "proj");
    glUniformMatrix4fv($uniProj, 1, GL_FALSE, \glm\value_ptr($proj));

    $displayCallback = function() use($uniModel) {
        global $t_start;
        // Clear the screen to black
        glClearColor(0.1, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculate transformation
        $t_now = microtime(true); 
        $time = $t_now - $t_start;
        
        $model = new mat4;
        $model = \glm\rotate(
            $model,
            $time * \glm\radians(180.0),
            new vec3(0.0, 0.0, 1.0)
        );
        glUniformMatrix4fv($uniModel, 1, GL_FALSE, \glm\value_ptr($model));

        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
        glutSwapBuffers();
    };

    $idleCallback = function() {
        glutPostRedisplay();
    };

    glutDisplayFunc($displayCallback);
    glutIdleFunc($idleCallback);
    glutMainLoop();

    glDeleteTextures(2, $textures);

    glDeleteProgram($shaderProgram);
    glDeleteShader($fragmentShader);
    glDeleteShader($vertexShader);

    glDeleteBuffers(1, $ebo);
    glDeleteBuffers(1, $vbo);

    glDeleteVertexArrays(1, $vao);

    return 0;

