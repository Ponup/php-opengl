<?php

// Set strict mode and show all PHP warnings
error_reporting(E_ALL);
ini_set('display_errors', true);

// Load OpenGL extension if not already loaded
extension_loaded('opengl') || dl('opengl.' . PHP_SHLIB_SUFFIX);

function gluPerspective($fovY, $aspect, $zNear, $zFar) {
    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    $fH = tan( $fovY / 360 * M_PI ) * $zNear;
    $fW = $fH * $aspect;

    glFrustum( -$fW, $fW, -$fH, $fH, $zNear, $zFar );
}
    function SOIL_load_image($path, &$width, &$height) {
        $img = imagecreatefrompng($path);
        $width = imagesx($img); 
        $height = imagesy($img); 
        $colors = [];
        for ($y = 0; $y < $height; $y++)
        {
            for ($x = 0; $x < $width; $x++)
            {
                $rgb = imagecolorat($img, $x, $y);
                $r = ($rgb >> 16) & 0xFF;
                $g = ($rgb >> 8) & 0xFF;
                $b = $rgb & 0xFF;
                $colors[] = $rgb;
            } 
        }
        return $colors;
    }
 
