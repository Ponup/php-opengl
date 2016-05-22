<?php


class Shader
{
    public $Program;
    // Constructor generates the shader on the fly
    public function __construct($vertexPath, $fragmentPath)
    {
        $vShaderCode = file_get_contents($vertexPath);
        $fShaderCode = file_get_contents($fragmentPath);
        // Vertex Shader
        $vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource($vertex, 1, $vShaderCode, NULL);
        glCompileShader($vertex);
        // Fragment Shader
        $fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource($fragment, 1, $fShaderCode, NULL);
        glCompileShader($fragment);
        // If geometry shader is given, compile geometry shader
        // Shader Program
        $this->Program = glCreateProgram();
        glAttachShader($this->Program, $vertex);
        glAttachShader($this->Program, $fragment);
        glLinkProgram($this->Program);
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader($vertex);
        glDeleteShader($fragment);

    }
    public function Use() { glUseProgram($this->Program); }
}

