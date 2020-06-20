FROM ponup/php-sdl

RUN apt-get install -y mesa-common-dev libgl1 libglvnd-dev
COPY . /opt/php-opengl
WORKDIR /opt/php-opengl
RUN phpize && ./configure && make install
RUN echo extension=opengl.so >> /etc/php/7.4/cli/php.ini

ENTRYPOINT ["/opt/php-opengl/docker-entrypoint.sh"]

