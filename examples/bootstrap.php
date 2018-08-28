<?php

// Set strict mode and show all PHP warnings
error_reporting(E_ALL);
ini_set('display_errors', true);

// Load SDL and OpenGL extensions if they not already loaded
extension_loaded('sdl') || dl('sdl.' . PHP_SHLIB_SUFFIX);
extension_loaded('opengl') || dl('opengl.' . PHP_SHLIB_SUFFIX);


