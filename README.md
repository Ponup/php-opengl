
# PHP-OpenGL

PHP bindings of the OpenGL and GLUT libraries.

Not all of the previously mentioned API is available on this extension. In order to keep the code small, maintainable and hopefully less buggy, it was decided to support only these subsets:

- OpenGL: All but the functions that are part of the compatibility profile. (glRotate, glBegin, glLight, ...). Compatibility profile usage is discouraged in other platforms and this extension only let you use the modern (core) GL profile. 
- GLUT: Only the API related to window management, input handling and main loop are available. Other functions (such as the one to render popup menus) are not available for the same reasons given before.

## Requirements

- PHP7
- OpenGL and FreeGLUT libraries
- Linux/MacOS (Windows support coming soon)

## Installation

### Linux

```sh
pecl install phpopengl
```

Or

```sh
git clone git://github.com/phpopengl/extension.git --recursive phpopengl
cd phpopengl
phpize
./configure --enable-opengl
make
sudo make install
echo extension=opengl.so | sudo tee /etc/php5/conf.d/opengl.ini
```

## Examples

```php
<?php
glutInit($argc, $argv);

glutInitWindowSize(800, 600);
glutInitWindowPosition(300, 100);
glutCreateWindow('Basic PHP-OpenGL example');

echo glGetString(GL_VENDOR), PHP_EOL;
echo glGetString(GL_RENDERER), PHP_EOL;

glutDisplayFunc(function() {
    glClearColor(0, 0, .2, 1); 
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
});
glutMainLoop();
```

## License

GPL-3.0+

