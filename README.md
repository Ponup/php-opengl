
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

```

## License


