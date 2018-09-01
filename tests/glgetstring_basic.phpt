--TEST--
Verifies glGetString returns a valid string.
--DESCRIPTION--
It also verifies all possible constants this function takes are defined.
--FILE--
<?php
$constants = [ 'GL_VENDOR', 'GL_RENDERER', 'GL_VERSION', 'GL_SHADING_LANGUAGE_VERSION' ];
foreach($constants as $constant) {
    echo $constant, defined($constant) ? ' is defined' : ' is undefined', PHP_EOL; 
}

$window = SDL_CreateWindow('Test window', SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
$context = SDL_GL_CreateContext($window);

echo gettype(glGetString(GL_VENDOR)), PHP_EOL;

SDL_GL_DeleteContext($context);
SDL_DestroyWindow($window);
--EXPECT--
GL_VENDOR is defined
GL_RENDERER is defined
GL_VERSION is defined
GL_SHADING_LANGUAGE_VERSION is defined
string
