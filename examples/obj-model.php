<?php

require 'bootstrap.php';

require 'vendor/autoload.php';

use \Mammoth\Graphic\Shader;
use Mammoth\Graphic\WavefrontObjLoader;
use Mammoth\Math\Matrix;
use Mammoth\Math\Transform;
use Mammoth\Math\Vector;

ini_set('memory_limit', '3096M');

SDL_Init(SDL_INIT_EVERYTHING);

SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

$window = SDL_CreateWindow("Fixed pipeline example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
SDL_GL_CreateContext($window);

$line = false;

$modelName = $argc === 2 ? $argv[1] : 'models/suzanne.obj';

$objLoader = new WavefrontObjLoader;
$obj = $objLoader->load($modelName);
echo $obj, PHP_EOL;

$verticesObjects = $obj->getVertices();
$vertices = [];
foreach($verticesObjects as $vertexObject) {
    $vertices[] = $vertexObject->x;
    $vertices[] = $vertexObject->y;
    $vertices[] = $vertexObject->z;
}

$indices = $obj->getVertexFaces(); 
$indices = array_map(function($index) { return $index - 1; }, $indices);

$normalObjects = $obj->getVertexNormals();
$normals = [];
foreach($normalObjects as $index => $normalObject) {
    if(!($normalObject instanceof Vector)) {
        echo "INDEX: $index\n";
        var_dump($normalObject);
    }
     else {
        $normals[] = $normalObject->x;
        $normals[] = $normalObject->y;
        $normals[] = $normalObject->z;
    }
}

$vertexSource = file_get_contents('shaders/basic_lighting.vs');
$fragmentSource = file_get_contents('shaders/basic_lighting.frag');

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
$viewLoc = glGetUniformLocation($shaderProgram, "view");
$modelLoc = glGetUniformLocation($shaderProgram, "model");
$projLoc = glGetUniformLocation($shaderProgram, "projection");

$position = new Vector(0, 0, 10);
$lampShader = null;
$lightVAO = null;
$lightPos = null;

$lightPos = new Vector(0, 10, 10.0);
$objectColorLoc = glGetUniformLocation($shaderProgram, "objectColor");
$lightColorLoc  = glGetUniformLocation($shaderProgram, "lightColor");
$lightPosLoc    = glGetUniformLocation($shaderProgram, "lightPos");
$viewPosLoc     = glGetUniformLocation($shaderProgram, "viewPos");
glUniform3f($objectColorLoc, 1.0, 0.5, 0.31);
glUniform3f($lightColorLoc,  1.0, 1.0, 1.0);
glUniform3f($lightPosLoc, $lightPos->x, $lightPos->y, $lightPos->z);
glUniform3f($viewPosLoc, $position->x, $position->y, $position->z);

$view = Transform::lookAt($position, new Vector(0, 0, 0), new Vector(0,1,0));
$proj = Transform::perspective(45, (float)800/(float)600, 0.1, 100);

$model = new Matrix();
//$model = \glm\rotate($model, 40, new Vector(1, 0, 0));
glUniformMatrix4fv($modelLoc, 1, GL_FALSE, $model->toRowVector());
glUniformMatrix4fv($viewLoc, 1, GL_FALSE, $view->toRowVector());
glUniformMatrix4fv($projLoc, 1, GL_FALSE, $proj->toRowVector());

glGenVertexArrays(1, $vaos);
$vao = $vaos[0];

glBindVertexArray($vao);

glGenBuffers(1, $vbos); $vbo = $vbos[0];
glBindBuffer(GL_ARRAY_BUFFER, $vbo);
glBufferData(GL_ARRAY_BUFFER, count($vertices) * 4, $vertices, GL_STATIC_DRAW);

glGenBuffers(1, $vbo_normals); $vbo_normal = $vbo_normals[0];
glBindBuffer(GL_ARRAY_BUFFER, $vbo_normal);
glBufferData(GL_ARRAY_BUFFER, count($normals) * 4, $normals, GL_STATIC_DRAW);

glGenBuffers(1, $ebos); $ebo = $ebos[0];
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, $ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, count($indices) * 4, $indices, GL_STATIC_DRAW);

// Vertex attribute
glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, $vbo);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

// Normal attribute
glEnableVertexAttribArray(1);
glBindBuffer(GL_ARRAY_BUFFER, $vbo_normal);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

glBindVertexArray(0);

// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))

$lampShader = new Shader\Program();
$lampShader->add(new Shader\Vertex('shaders/lamp.vs'));
$lampShader->add(new Shader\Fragment('shaders/lamp.frag'));
$lampShader->compile();
$lampShader->link();


glGenVertexArrays(1, $lightVAOS); $lightVAO = $lightVAOS[0];
glBindVertexArray($lightVAO);
// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
$lightVertices2 = [ 
  (new Vector(-0.1, -0.1, -0.1, 0.0)),
  (new Vector( 0.1, -0.1, -0.1, 0.0)),
  (new Vector( 0.1,  0.1, -0.1, 0.0)),
  (new Vector(-0.1,  0.1, -0.1, 0.0)),
  (new Vector(-0.1, -0.1,  0.1, 0.0)),
  (new Vector( 0.1, -0.1,  0.1, 0.0)),
  (new Vector( 0.1,  0.1,  0.1, 0.0)),
  (new Vector(-0.1,  0.1,  0.1, 0.0)),
];
$lightVertices = [];
foreach($lightVertices2 as $l) {
    $lightVertices[] = $l->x;
    $lightVertices[] = $l->y;
    $lightVertices[] = $l->z;
}

glGenBuffers(1, $vbo_lights); $vbo_light = $vbo_lights[0];
glBindBuffer(GL_ARRAY_BUFFER, $vbo_light);
glBufferData(GL_ARRAY_BUFFER, count($lightVertices) * 4, $lightVertices, GL_STATIC_DRAW);
// Set the vertex attributes (only position data for the lamp))
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * 4, 0); // Note that we skip over the normal vectors
glEnableVertexAttribArray(0);
glBindVertexArray(0);
glPolygonMode(GL_FRONT_AND_BACK, $line ? GL_LINE : GL_FILL);

$event = new SDL_Event;
while(true) {
    glClearColor(.2, .3, .3, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram($shaderProgram);

    glBindVertexArray($vao);
        $numFaces = count($indices);
        glDrawElements(GL_TRIANGLES, $numFaces, GL_UNSIGNED_INT, null);
    glBindVertexArray(0);

        $lampShader->Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        $modelLoc = glGetUniformLocation($lampShader->getId(), "model");
        $viewLoc  = glGetUniformLocation($lampShader->getId(), "view");
        $projLoc  = glGetUniformLocation($lampShader->getId(), "projection");
        // Set matrices
        glUniformMatrix4fv($viewLoc, 1, GL_FALSE, $view->toRowVector());
        glUniformMatrix4fv($projLoc, 1, GL_FALSE, $proj->toRowVector());
        $model = new Matrix();
        $model = Transform::translate($model, $lightPos);
        glUniformMatrix4fv($modelLoc, 1, GL_FALSE, $model->toRowVector());
    // Draw the light object (using light's vertex attributes)
    glBindVertexArray($lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); //8*4
    glBindVertexArray(0);
	SDL_GL_SwapWindow($window);
	SDL_Delay(10);
	SDL_PollEvent($event);
	if($event->type == SDL_KEYDOWN) {
        $line = !$line;
        
        glPolygonMode(GL_FRONT_AND_BACK, $line ? GL_LINE : GL_FILL);
	}
}

glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_DEPTH_TEST);

