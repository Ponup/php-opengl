<?php

// Set strict mode and show all PHP warnings
error_reporting(E_ALL);
ini_set('display_errors', true);

// Load OpenGL extension if not already loaded
extension_loaded('opengl') || dl('opengl.' . PHP_SHLIB_SUFFIX);


