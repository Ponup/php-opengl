<?php

require 'bootstrap.php';
require 'glu.php';

class Camera {

    private $m_x, $m_y, $m_z;   // Position
    private $m_lx, $m_ly, $m_lz; // Direction vector of where we are looking at
    private $m_yaw, $m_pitch; // Various rotation angles
    private $m_strafe_lx, $m_strafe_lz; // Always 90 degree to direction vector

    public function __construct() {
        $this->m_yaw = 0.0;
        $this->m_pitch = 0.0;

        $this->m_x = 0;
        $this->m_y = 0;
        $this->m_z = 0;
    }

    public function Refresh() {
        // Camera parameter according to Riegl's co-ordinate system
        // x/y for flat, z for height
        $this->m_lx = cos($this->m_yaw) * cos($this->m_pitch);
        $this->m_ly = sin($this->m_pitch);
        $this->m_lz = sin($this->m_yaw) * cos($this->m_pitch);

        $this->m_strafe_lx = cos($this->m_yaw - M_PI_2);
        $this->m_strafe_lz = sin($this->m_yaw - M_PI_2);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt($this->m_x, $this->m_y, $this->m_z, $this->m_x + $this->m_lx, $this->m_y + $this->m_ly, $this->m_z + $this->m_lz, 0.0, 1.0, 0.0);
    }

    public function Move($incr) {
        $lx = cos($this->m_yaw) * cos($this->m_pitch);
        $ly = sin($this->m_pitch);
        $lz = sin($this->m_yaw) * cos($this->m_pitch);

        $this->m_x = $this->m_x + $incr * $lx;
        $this->m_y = $this->m_y + $incr * $ly;
        $this->m_z = $this->m_z + $incr * $lz;
    }

    public function Strafe($incr) {
        $this->m_x = $this->m_x + $incr * $this->m_strafe_lx;
        $this->m_z = $this->m_z + $incr * $this->m_strafe_lz;
    }

    public function Fly($incr) {
        $this->m_y = $this->m_y + $incr;
    }

    public function RotateYaw($angle) {
        $this->m_yaw += $angle;
    }

    public function RotatePitch($angle) {
        $limit = 89.0 * M_PI / 180.0;

        $this->m_pitch += $angle;

        if ($this->m_pitch < -$limit)
            $this->m_pitch = -$limit;

        if ($this->m_pitch > $limit)
            $this->m_pitch = $limit;
    }

}

$camera = new Camera;
$keyStatus = array(
    'a' => false,
    's' => false,
    'd' => false,
    'w' => false,
);
$navigationEnabled = false;
$viewportWidth = 0;
$viewportHeight = 0;
$mouseLeftDown = false;
$mouseRightDown = false;

// Movement settings
$translationSpeed = 0.05;
$rotationSpeed = M_PI / 180 * 0.2;

function onMouseMotion($x, $y, $passive) {
    global $navigationEnabled, $rotationSpeed, $camera, $viewportHeight, $viewportWidth;

    if ($navigationEnabled) {
        $dx = (float) $x - $viewportWidth / 2;
        $dy = (float) $y - $viewportHeight / 2;

        if ($dx) {
            $camera->RotateYaw($rotationSpeed * $dx);
            $camera->Refresh();
        }
        if ($dy) {
            $camera->RotatePitch($rotationSpeed * $dy);
            $camera->Refresh();
        }

        glutWarpPointer($viewportWidth / 2, $viewportHeight / 2);
    }
}

$Mouse = function( $button, $state, $x, $y ) use( &$mouseRightDown, &$mouseLeftDown ) {
    if ($state == GLUT_DOWN) {
        if ($button == GLUT_LEFT_BUTTON) {
            $mouseLeftDown = true;
        } else if ($button == GLUT_RIGHT_BUTTON) {
            $mouseRightDown = true;
        }
    } else if ($state == GLUT_UP) {
        if ($button == GLUT_LEFT_BUTTON) {
            $mouseLeftDown = false;
        } else if ($button == GLUT_RIGHT_BUTTON) {
            $mouseRightDown = false;
        }
    }
};


$Keyboard = function($key, $x, $y) use( &$keyStatus, &$navigationEnabled ) {
    global $viewportHeight, $viewportWidth;
    if (ord($key) == 27) {
        exit(0);
    }

    if ($key == ' ') {
        $navigationEnabled = !$navigationEnabled;

        if ($navigationEnabled) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer($viewportWidth / 2, $viewportHeight / 2);
        } else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    $keyStatus[$key] = true;
};

$KeyboardUp = function( $key, $x, $y ) use( &$keyStatus ) {
    usleep(4000);
    $keyStatus[$key] = false;
};

$Display = function() use( $camera ) {
    glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();

    $camera->Refresh();

    glColor3f(0, 1, 0);

    glutWireTeapot(0.5);
    Grid();

    glutSwapBuffers(); //swap the buffers
};

$Reshape = function($w, $h) use( &$viewportHeight, &$viewportWidth ) {
    $viewportWidth = $w;
    $viewportHeight = $h;

    glViewport(0, 0, $w, $h); //set the viewport to the current window specifications
    glMatrixMode(GL_PROJECTION); //set the matrix to projection

    glLoadIdentity();
    gluPerspective(60, $w / $h, 0.1, 100.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode(GL_MODELVIEW); //set the matrix back to model
};

$Timer = function($value) {
    global $keyStatus, $camera, $navigationEnabled, $translationSpeed, $mouseRightDown, $mouseLeftDown;
    if (false === $navigationEnabled) {
        return;
    }
    if ($keyStatus['w']) {
        $camera->Move($translationSpeed);
    } else if ($keyStatus['s']) {
        $camera->Move(-$translationSpeed);
    } else if ($keyStatus['a']) {
        $camera->Strafe($translationSpeed);
    } else if ($keyStatus['d']) {
        $camera->Strafe(-$translationSpeed);
    } else if ($mouseLeftDown) {
        $camera->Fly(-$translationSpeed);
    } else if ($mouseRightDown) {
        $camera->Fly($translationSpeed);
    }
    $camera->Refresh();

    //glutTimerFunc(1, $Timer, 0);
};


$Idle = function() use( $Display, $Timer ) {
    $Timer(1);
    $Display();
};


glutInit($argc, $argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
glutInitWindowSize(640, 480);
glutCreateWindow("Free navigation");
glutIgnoreKeyRepeat(1);
glutDisplayFunc($Display);
glutIdleFunc($Display);
glutMouseFunc($Mouse);
glutMotionFunc(function( $x, $y ) {
    onMouseMotion($x, $y, false);
});
//glutPassiveMotionFunc(function( $x, $y ) { onMouseMotion( $x, $y, true ); });
glutKeyboardFunc($Keyboard);
glutKeyboardUpFunc($KeyboardUp);
glutIdleFunc($Idle);
glutReshapeFunc($Reshape);
//glutTimerFunc(1, $Timer, 0);

glutMainLoop();

function Grid() {
    glPushMatrix();
    glColor3f(1, 1, 1);

    for ($i = -50; $i < 50; $i++) {
        glBegin(GL_LINES);
        glVertex3f($i, 0, -50);
        glVertex3f($i, 0, 50);
        glEnd();
    }

    for ($i = -50; $i < 50; $i++) {
        glBegin(GL_LINES);
        glVertex3f(-50, 0, $i);
        glVertex3f(50, 0, $i);
        glEnd();
    }

    glPopMatrix();
}
