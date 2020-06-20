
# PHP-OpenGL

PHP bindings of the OpenGL library.

Not all of the previously mentioned API is available on this extension. In order to keep the code small, maintainable and hopefully less buggy, it was decided to support only these subsets:

- OpenGL: All but the functions that are part of the compatibility profile. (glRotate, glBegin, glLight, ...). Compatibility profile usage is discouraged in other platforms and this extension only let you use the modern (core) GL profile. 

## Requirements

- PHP7.2
- [SDL extension for PHP](https://github.com/Ponup/phpsdl)
- OpenGL library/framework
- Linux/MacOS (Windows support coming soon)
- The PHP GD extension is required to run some of the examples.

## Installation

### Linux

```bash
pecl install phpopengl
```

Or

```bash
$ git clone git://github.com/phpopengl/extension.git --recursive phpopengl
$ cd phpopengl
$ phpize
$ ./configure --enable-opengl
$ make
$ sudo make install
```

## Examples

```php
<?php
SDL_Init(SDL_INIT_EVERYTHING);

$window = SDL_CreateWindow("Fixed pipeline example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,                
                640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);                                                                                               
SDL_GL_CreateContext($window);    

glClearColor(0, 0, .2, 1); 
glClear(GL_COLOR_BUFFER_BIT);
SDL_GL_SwapWindow($window);

$event = new SDL_Event;
while(true) {
	SDL_PollEvent($event);
	if($event->type == SDL_KEYDOWN) break;
	SDL_Delay(50);
}

SDL_DestroyWindow($window);
```

## License

GPL-3.0+

