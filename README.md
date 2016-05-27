
# PHP-OpenGL

PHP bindings of the OpenGL library.

## Requirements

- PHP7
- OpenGL library
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

