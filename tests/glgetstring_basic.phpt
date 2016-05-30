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
echo gettype(glGetString(GL_VENDOR)), PHP_EOL;
glutInit($argc, $argv);
glutCreateWindow('Title');
echo gettype(glGetString(GL_VENDOR)), PHP_EOL;
--EXPECT--
GL_VENDOR is defined
GL_RENDERER is defined
GL_VERSION is defined
GL_SHADING_LANGUAGE_VERSION is defined
NULL
string
