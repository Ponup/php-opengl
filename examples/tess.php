<?php
	/* OpenGL redbook tess */

require 'bootstrap.php';

$startList = 0;

function display() {
	global $startList;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glCallList($startList);
	glCallList($startList + 1);
	glFlush();
}

function beginCallback($which) {
	glBegin($which);
}

function errorCallback($errorCode) {
	echo "Tessellation Error: " . gluErrorString($errorCode) . "\n";
	exit();
}

function endCallback() {
	glEnd();
}

function vertexCallback($vertex) {
	glColor3dv(array_slice($vertex, 3));
	glVertex3dv($vertex);
}

function combineCallback($coords, $vertex_data, $weight, &$dataOut) {
	$vertex[0] = $coords[0];
	$vertex[1] = $coords[1];
	$vertex[2] = $coords[2];
	for ($i = 3; $i < 6; $i++)
		$vertex[$i] = $weight[0] * $vertex_data[0][$i]
					+ $weight[1] * $vertex_data[1][$i]
					+ $weight[2] * $vertex_data[2][$i]
					+ $weight[3] * $vertex_data[3][$i];
	$dataOut = $vertex;
}

function init() {
	global $startList;

	$rect = array(
		array(50.0, 50.0, 0.0),
		array(200.0, 50.0, 0.0),
		array(200.0, 200.0, 0.0),
		array(50.0, 200.0, 0.0)
	);
	$tri = array(
		array(75.0, 75.0, 0.0),
		array(125.0, 175.0, 0.0),
		array(175.0, 75.0, 0.0)
	);
	$star = array(
		array(250.0, 50.0, 0.0, 1.0, 0.0, 1.0),
		array(325.0, 200.0, 0.0, 1.0, 1.0, 0.0),
		array(400.0, 50.0, 0.0, 0.0, 1.0, 1.0),
		array(250.0, 150.0, 0.0, 1.0, 0.0, 0.0),
		array(400.0, 150.0, 0.0, 0.0, 1.0, 0.0)
	);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	$startList = glGenLists(2);
	$tobj = gluNewTess();
	gluTessCallback($tobj, GLU_TESS_VERTEX, 'glVertex3dv');
	gluTessCallback($tobj, GLU_TESS_BEGIN, 'beginCallback');
	gluTessCallback($tobj, GLU_TESS_END, 'endCallback');
	gluTessCallback($tobj, GLU_TESS_ERROR, 'errorCallback');

	glNewList($startList, GL_COMPILE);
	glShadeModel(GL_FLAT);
	gluTessBeginPolygon($tobj, NULL);
		gluTessBeginContour($tobj);
			gluTessVertex($tobj, $rect[0], $rect[0]);
			gluTessVertex($tobj, $rect[1], $rect[1]);
			gluTessVertex($tobj, $rect[2], $rect[2]);
			gluTessVertex($tobj, $rect[3], $rect[3]);
		gluTessEndContour($tobj);
		gluTessBeginContour($tobj);
			gluTessVertex($tobj, $tri[0], $tri[0]);
			gluTessVertex($tobj, $tri[1], $tri[1]);
			gluTessVertex($tobj, $tri[2], $tri[2]);
		gluTessEndContour($tobj);
	gluTessEndPolygon($tobj);
	glEndList();

	gluTessCallback($tobj, GLU_TESS_VERTEX, 'vertexCallback');
	gluTessCallback($tobj, GLU_TESS_BEGIN, 'beginCallback');
	gluTessCallback($tobj, GLU_TESS_END, 'endCallback');
	gluTessCallback($tobj, GLU_TESS_ERROR, 'errorCallback');
	gluTessCallback($tobj, GLU_TESS_COMBINE, 'combineCallback');

	glNewList($startList + 1, GL_COMPILE);
	glShadeModel(GL_SMOOTH);
	gluTessProperty($tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);
	gluTessBeginPolygon($tobj, NULL);
		gluTessBeginContour($tobj);
			gluTessVertex($tobj, $star[0], $star[0]);
			gluTessVertex($tobj, $star[1], $star[1]);
			gluTessVertex($tobj, $star[2], $star[2]);
			gluTessVertex($tobj, $star[3], $star[3]);
			gluTessVertex($tobj, $star[4], $star[4]);
		gluTessEndContour($tobj);
	gluTessEndPolygon($tobj);
	glEndList();
	gluDeleteTess($tobj);
}

function reshape($w, $h) {
	glViewport(0, 0, $w, $h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, $w, 0.0, $h);
}

glutInit($argc, $argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(500, 500);
glutCreateWindow($argv[0]);
init();
glutDisplayFunc('display');
glutReshapeFunc('reshape');
glutMainLoop();

