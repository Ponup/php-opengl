<?php

function gluPerspective($fovY, $aspect, $zNear, $zFar) {
    $fH = tan( $fovY / 360 * M_PI ) * $zNear;
    $fW = $fH * $aspect;

    glFrustum(-$fW, $fW, -$fH, $fH, $zNear, $zFar);
}

function gluLookAt() {
}

function gluOrtho2D() {
}

