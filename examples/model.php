<?php

require 'bootstrap.php';
require 'obj_loader.php';

glutInit($argc, $argv);
glutInitContextVersion(3, 3);
glutInitContextProfile(GLUT_CORE_PROFILE);

glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
glutInitWindowSize(800, 600);

glutCreateWindow('Rectangle');

$line = false;

$obj = load('cube2.obj');

$vertices = $obj->getVerticesAsFloatArray();
$vertices = $obj->ood();
$indices = $obj->getIndicesAsFloatArray(); 

    $vertexSource = <<<PPP
#version 330 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) in vec3 position;
uniform float gScale;
void main() {
    gl_Position = vec4(gScale*position.x, gScale*position.y, position.z, 1.0);
}
PPP;

    $fragmentSource = <<<PPP
#version 330 core
out vec4 color;
void main() {
    color = vec4(1.0, 0.5, 0.5, 1.0);
}
PPP;

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

glUseProgram($shaderProgram);
$gScaleLocation = glGetUniformLocation($shaderProgram, "gScale");
if($gScaleLocation == -1) {
    die('Unable to fin variable glGetUniformLocation');
}

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

glPolygonMode(GL_FRONT_AND_BACK, $line ? GL_LINE : GL_FILL);

$displayFunc = function() use($shaderProgram, $vao, $indices, $gScaleLocation) {
    glClearColor(.2, .3, .3, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram($shaderProgram);

    glBindVertexArray($vao);
    $aaa = count($indices)/3;
    glDrawElements(GL_TRIANGLES, $aaa, GL_UNSIGNED_INT, 0);  

    glBindVertexArray(0);

    glutSwapBuffers();
};

$idleFunc = function() use($gScaleLocation) {
    static $Scale = 0.0;
    $Scale += 0.001;
    $uni = sprintf('%0.1f', sin($Scale));
    glUniform1f($gScaleLocation, floatval($uni));
    glutPostRedisplay();
};

glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

$keyboardFunc = function($key, $x, $y) use(&$line) {
    if(ord($key) === 27) {
        glutLeaveMainLoop();
    }
    elseif(ord($key) === 32) {
        $line = !$line;
        
        glPolygonMode(GL_FRONT_AND_BACK, $line ? GL_LINE : GL_FILL);
        glutPostRedisplay();
    }
};

glutDisplayFunc($displayFunc);
glutIdleFunc($idleFunc);
glutKeyboardFunc($keyboardFunc);
glutMainLoop();

echo 'Cleainnig resources';

