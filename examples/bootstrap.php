<?php

// Set strict mode and show all PHP warnings
error_reporting(E_ALL);

// Load OpenGL extension if not already loaded
extension_loaded('opengl') || dl('opengl.' . PHP_SHLIB_SUFFIX);

