<?php

class Vec3 { public $x, $y, $z; }
class Vec2 { public $x, $y; }

class Obj {

    public $vertices, $verticesIndices;
    public $uvs, $normals;

    public function __construct() {
        $this->vertices = [];
        $this->verticesIndices = [];
        $this->uvs = [];
        $this->normals = [];
    }

    public function getVerticesAsFloatArray() {
        $result = [];
        foreach($this->vertices as $vertex) {
            $result[] = $vertex->x;
            $result[] = $vertex->y;
            $result[] = $vertex->z;
        }
        return $result;
    }

    public function getIndicesAsFloatArray() {
        $result = [];
        foreach($this->verticesIndices as $vertex) {
            $result[] = $vertex->x;
            $result[] = $vertex->y;
            $result[] = $vertex->z;
        }

        return $result;
    }

    public function ood() {
        $ll = $this->getVerticesAsFloatArray();
        $result = [];
        foreach($this->verticesIndices as $vi) {
            $result[] = $ll[ $vi->x - 1 ];
            $result[] = $ll[ $vi->y - 1 ];
            $result[] = $ll[ $vi->z - 1 ];
        }
        return $result;
    }
};

function load($path) {
    if(!file_exists($path)) {
        throw new Exception('Unable to read file: ' . $path);
    }

    $obj = new Obj;

    $file = new SplFileObject($path);
    $file->setFlags(SplFileObject::DROP_NEW_LINE);

    while(!$file->eof()) {
        $line = $file->fgets();
        if(empty($line)) {
            continue;
        }
        if($line[0] == '#') {
            continue;
        }
        $tokens = explode(' ', $line);
        $type = $tokens[0];

        switch($type) {
            case 'v':
                $vertex = new Vec3;
                list($vertex->x, $vertex->y, $vertex->z) = sscanf($line, 'v %f %f %f');
                $vertex->x /= 2;
                $vertex->y /= 2;
                $vertex->z /= 2;
                $obj->vertices[] = $vertex;
            break;
            case 'vt':
                $uv = new Vec2;
                list($uv->x, $uv->y) = sscanf($line, 'vt %f %f');
                $obj->uvs[] = $uv;
            break;
            case 'vn':
                $vertex = new Vec3;
                list($vertex->x, $vertex->y, $vertex->z) = sscanf($line, 'vn %f %f %f');
                $obj->normals[] = $vertex;
            break;
            case 'f':
                $vertin = new Vec3;
                $uvin = new Vec3;
                $normin = new Vec3;
                list(
                    $vertin->x, $vertin->y, $vertin->z,
                    $uvin->x, $uvin->y, $uvin->z,
                    $normin->x, $normin->y, $normin->z)
                     = sscanf($line, 'f %d/%d/%d %d/%d/%d %d/%d/%d');
                $obj->verticesIndices[] = $vertin;
                break;
        }
    }

    return $obj;
}

