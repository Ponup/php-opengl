<?php

class mat4 {

    private $data;

    public function __construct(array $data = null) {
        if($data == null) {
            $data = [
                [ 1, 0, 0, 0 ],
                [ 0, 1, 0, 0 ],
                [ 0, 0, 1, 0 ],
                [ 0, 0, 0, 1 ],
            ];
        }
        $this->data = $data;
    }

    public function toArray() {
        return $this->data;
    }
}

class vec3 {

    public $x, $y, $z;

    public function __construct($x = null, $y = null, $z = null) {
        $this->x = $x;
        $this->y = $y;
        $this->z = $z;
    }

    public function substract(vec3 $other) {
        return $this;
    }

    public function length() {
        return sqrt($this->x * 2 + $this->y * 2 + $this->z * 2);
    }

    public function normalize() {
        $length = $this->length();
        if($length == 0) return;
        $this->x = $this->x / $length;
        $this->y = $this->y / $length;
        $this->z = $this->z / $length;
    }

    public function cross(vec3 $other) {
        // https://www.mathsisfun.com/algebra/vectors-cross-product.html
        $vec3 = new vec3;
        $vec3->x = $this->y * $other->z - $this->z * $other->y;
        $vec3->y = $this->z * $other->x - $this->x * $other->z;
        $vec3->z = $this->x * $other->y - $this->y * $other->x;
        return $vec3;
    }

    public function dot(vec3 $other) {
        return (
            $this->x * $other->x
            + $this->y * $other->y
            + $this->z * $other->z
        );
    }

    public function __toString() {
        return sprintf('vec3{ x=%f, y=%f, z=%f }', $this->x, $this->y, $this->z);
    }
}

class glm {

    static function radians($degrees) {
        return deg2rad($degrees);
    }

    static function lookAt(vec3 $eye, vec3 $center, vec3 $up) {
        $mat4 = new mat4;
    //http://stackoverflow.com/questions/21830340/understanding-glmlookat
        // Create new coord system
        $Z = $eye->substract( $center ); // eye - center
        $Z->Normalize();
        $Y = $up;
        $X = $Y->cross( $Z );
        // Recompute Y = Z cross X:
        $Y = $Z->cross( $X );
        // Cross-product gives area of parallelogram, which is < 1.0 for non-perpendicular unit-length vectors; so normalize X, Y here:
        $X->Normalize();
        $Y->Normalize();
        return new mat4([
            [ $X->x, $Y->x, $Z->x, 0 ],
            [ $X->y, $Y->y, $Z->y, 0 ],
            [ $X->z, $Y->z, $Z->z, 0 ],
            [ -$X->Dot( $eye ), -$Y->dot( $eye ), -$Z->dot( $eye ), 1.0 ],
        ]);
        return $mat4;
    }

    static function perspective() {
        $mat4 = new mat4;
        return $mat4;
    }

    static function rotate(mat4 $mat) {
        return $mat;
    }

    static function vec3($x, $y, $z) {
        $vec3 = new vec3($x, $y, $z);
        return $vec3;
    }

    static function value_ptr(mat4 $x) {
        return $x->toArray();
    }
}

