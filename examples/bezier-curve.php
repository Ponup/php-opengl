<?php
/**
 * Example of how to draw a bezier curve on the screen using the PHP-OpenGL extension.
 *
 * @author Santiago Lizardo <santiagolizardo@php.net>
 */

require 'bootstrap.php';

$controlPoints = array( -4.0, -4.0, 0.0, -2.0, 4.0, 0.0, 2.0, -4.0, 0.0, 4.0, 4.0, 0.0 );

function init( array $controlPoints )
{
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glShadeModel( GL_FLAT );
	glMap1f( GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, $controlPoints );
	glEnable( GL_MAP1_VERTEX_3 );
}

$displayCallback = function() use( $controlPoints )
{
	glClear( GL_COLOR_BUFFER_BIT );
	glColor3f( 1.0, 1.0, 1.0 );
	glBegin( GL_LINE_STRIP );
		for( $i = 0; $i <= 30; $i++ )
			glEvalCoord1f( $i / 30.0 );
	glEnd();
	glPointSize( 5.0 );
	glColor3f( 1.0, 1.0, 0.0 );
	glBegin( GL_POINTS );
		for( $i = 0; $i < 4; $i++ )
		{
			$array = array_slice( $controlPoints, $i * 3,3 );
			glVertex3fv( $array );
		}
	glEnd();
	glFlush();
};

$reshapeCallback = function( $w, $h )
{
	glViewport( 0, 0, $w, $h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	if( $w <= $h )
		glOrtho( -5.0, 5.0, -5.0 * $h / $w,  5.0 * $h / $w, -5.0, 5.0 );
	else
		glOrtho( -5.0 * $w / $h, 5.0 * $w / $h, -5.0, 5.0, -5.0, 5.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};

$keyboardCallback = function( $key, $x, $y )
{
	if( 27 === ord( $key ) )
		exit( 0 );
};

glutInit( $argc, $argv );
glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
glutInitWindowSize( 500, 500 );
glutInitWindowPosition( 100, 100 );
glutCreateWindow( 'PHP-OpenGL: Bezier curve example' );

init( $controlPoints );

glutDisplayFunc( $displayCallback );
glutReshapeFunc( $reshapeCallback );
glutKeyboardFunc( $keyboardCallback );
glutMainLoop();

