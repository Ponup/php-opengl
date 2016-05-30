--TEST--
Extension opengl is loaded.
--DESCRIPTION--
This test checks if the compiled extension is loaded.
--FILE--
<?php
extension_loaded('opengl') && print('OpenGL is loaded');
--EXPECT--
OpenGL is loaded
