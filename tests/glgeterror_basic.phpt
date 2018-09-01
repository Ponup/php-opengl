--TEST--
Tests the glGetError function returns errors correctly.
--FILE--
<?php
$constants = [ 'GL_NO_ERROR', 'GL_INVALID_ENUM', 'GL_INVALID_VALUE', 'GL_INVALID_OPERATION', 'GL_INVALID_FRAMEBUFFER_OPERATION', 'GL_OUT_OF_MEMORY', 'GL_STACK_UNDERFLOW', 'GL_STACK_OVERFLOW' ];
foreach($constants as $constant) {
    echo $constant, defined($constant) ? ' is defined' : 'is undefined', PHP_EOL;
}

$window = SDL_CreateWindow('Test window', SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
$context = SDL_GL_CreateContext($window);

var_dump(glGetError() === GL_NO_ERROR);

SDL_GL_DeleteContext($context);
SDL_DestroyWindow($window);
--EXPECT--
GL_NO_ERROR is defined
GL_INVALID_ENUM is defined
GL_INVALID_VALUE is defined
GL_INVALID_OPERATION is defined
GL_INVALID_FRAMEBUFFER_OPERATIONis undefined
GL_OUT_OF_MEMORY is defined
GL_STACK_UNDERFLOW is defined
GL_STACK_OVERFLOW is defined
bool(true)
