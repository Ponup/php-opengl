<?php

require 'bootstrap.php';
require 'vendor/autoload.php';

// Shader sources
$vertexSource = <<<GLSL
#version 150 core
in vec2 position;
in vec3 color;
in vec2 texcoord;
uniform float fade;
out vec3 Color;
out vec2 Texcoord;
out float Fade;
void main()
{
Fade = fade;
    Color = color;
    Texcoord = texcoord;
    gl_Position = vec4(position, 0.0, 1.0);
}
GLSL;
$fragmentSource = <<<GLSL
#version 150 core
in vec3 Color;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texKitten;
uniform sampler2D texPuppy;
in float Fade;
uniform float time;
void main()
{
    float factor = (sin(time * 3.0) + 1.0) / 2.0;
    outColor = mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), factor);
        if (Texcoord.y < 0.5)
            outColor = texture(texKitten, Texcoord);
        else
            outColor = texture(texKitten,
                vec2(Texcoord.x + sin(Texcoord.y * 60.0 + time * 2.0) / 30.0, 1.0 - Texcoord.y)
            ) * vec4(0.7, 0.7, 1.0, 1.0);
}
GLSL;

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("Shader effect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
SDL_GL_CreateContext($window);

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

    $fade = 0.0;

    $start = microtime(true);

	$event = new SDL_Event;

	while(true) {
        // Clear the screen to black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        static $a = 0;
        $a += 0.0001;
        $fade += 0.0001;

        // Draw a rectangle from the 2 triangles using 6 indices
        glUniform1f(glGetUniformLocation($shaderProgram, "fade"), $fade);
        glUniform1f(glGetUniformLocation($shaderProgram, "time"), $a);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
		SDL_GL_SwapWindow($window);
		SDL_PollEvent($event);
		if($event->type == SDL_KEYDOWN) break;

		SDL_Delay(100);
    }


glDeleteTextures(2, $textures);

glDeleteProgram($shaderProgram);
glDeleteShader($fragmentShader);
glDeleteShader($vertexShader);

glDeleteBuffers(1, $ebo);
glDeleteBuffers(1, $vbo);

glDeleteVertexArrays(1, $vao);

