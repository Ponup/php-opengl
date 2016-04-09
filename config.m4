idnl config.m4 for extension opengl

PHP_ARG_WITH(opengl, for opengl support,
[  --with-opengl=DIR       Include OpenGL support])

CFLAGS="$CFLAGS -pedantic -Wall -Wfatal-errors -framework GLUT -framework OpenGL"

if false
then

  if test -r $PHP_GLUT/include/GL/glut.h; then
    GLUT_DIR=$PHP_GLUT
  else
    AC_MSG_CHECKING(for GLUT development files in default path)
    for i in /usr/local/Cellar/freeglut/2.8.1 /System/Library/Frameworks/OpenGL.framework/Headers /usr/local /usr
    do
      echo $i
      if test -r $i/include/GL/glut.h; then
        GLUT_DIR=$i
        AC_MSG_RESULT(found in $i)
        break
      fi
      if test -r $i/glut.h ; then
        GLUT_DIR=$i
        AC_MSG_RESULT(found in $i)
        break
      fi
    done

  fi

  if test -z "$GLUT_DIR"; then
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Please make sure GLUT is properly installed - glut.h should be in <glut-dir>/include/GL/)
  fi

  saved_LIBS="$LIBS"
  AC_MSG_CHECKING(if  GLUT links properly)
  failed=yes
  for additional_libs in "" "-lglut" "-lGL" "-lGL -lGLU" "-lGL -lGLU -lm"; do
    if test ! -z "$additional_libs"; then
      AC_MSG_CHECKING(with additional library $additional_libs)
    fi
    LIBS="$saved_LIBS -lglut $additional_libs"
    AC_TRY_LINK( ,[glutMainLoop();], have_glut=yes, have_glut=no)
    AC_MSG_RESULT($have_glut)
    LIBS="$saved_LIBS"

    if test "$have_glut" != "no"; then
      PHP_ADD_LIBRARY_WITH_PATH(glut, $GLUT_DIR/lib, GLUT_SHARED_LIBADD)
      if test ! -z "$additional_libs"; then
        PHP_EVAL_LIBLINE($additional_libs, GLUT_SHARED_LIBADD)
      fi
      failed=no
      break
    fi
  done
  unset additional_libs

  PHP_ADD_INCLUDE(/System/Library/Frameworks/OpenGL.framework/Headers)
  PHP_ADD_INCLUDE($GLUT_DIR/include)

  PHP_SUBST(GLUT_SHARED_LIBADD)
fi

if test "$PHP_OPENGL" != "no"; then

  if test -r $PHP_OPENGL/include/GL/gl.h; then
    OPENGL_DIR=$PHP_OPENGL
  else
    AC_MSG_CHECKING(for OpenGL development files in default path)
    for i in /usr/X11R6 /usr/local /usr /System/Library/Frameworks/OpenGL.framework/Headers; do
      if test -r $i/include/GL/gl.h ; then
        OPENGL_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
      if test -r $i/gl.h ; then
        OPENGL_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi

    done
  fi

  if test -z "$OPENGL_DIR"; then
    AC_MSG_RESULT(not found)
    AC_MSG_ERROR(Please make sure OpenGL is properly installed - gl.h should be in <opengl-dir>/include/GL/)
  fi

  saved_LIBS="$LIBS"

  failed=yes
  for additional_libs in "" -lm; do
    if test ! -z "$additional_libs"; then
      AC_MSG_RESULT(with additional library $additional_libs)
    fi
    AC_MSG_CHECKING(if  OpenGL links properly)
    LIBS="$saved_LIBS -lGL $additional_libs"
    AC_TRY_LINK( ,[char glBegin(); glBegin(); ], have_GL=yes, have_GL=no)
    AC_MSG_RESULT($have_GL)

    if test "$have_GL" = "no"; then
      AC_MSG_CHECKING(if  MesaGL links properly)
      LIBS="$saved_LIBS -lMesaGL $additional_libs"
      AC_TRY_LINK( ,[char glBegin(); glBegin(); ], have_MesaGL=yes, have_MesaGL=no)
      AC_MSG_RESULT($have_MesaGL)
  
      if test "$have_MesaGL" = "no"; then
        AC_MSG_CHECKING(if  MesaGL with pthreads links properly)
        LIBS="$saved_LIBS -L$OPENGL_DIR/lib -lMesaGL -lpthread $additional_libs"
        AC_TRY_LINK( ,[char glBegin(); glBegin(); ], have_MesaGL_pthreads=yes, have_MesaGL_pthreads=no)
        AC_MSG_RESULT($have_MesaGL_pthreads)
  
        if test "$have_MesaGL_pthreads" != "no"; then
          LIBS="$saved_LIBS"
          PHP_ADD_LIBRARY_WITH_PATH(pthread, $OPENGL_DIR/lib, OPENGL_SHARED_LIBADD)
          PHP_ADD_LIBRARY_WITH_PATH(MesaGL, $OPENGL_DIR/lib, OPENGL_SHARED_LIBADD)
          PHP_ADD_LIBRARY_WITH_PATH(MesaGLU, $OPENGL_DIR/lib, OPENGL_SHARED_LIBADD)
          failed=no
        fi
      else
        LIBS="$saved_LIBS"
        PHP_ADD_LIBRARY_WITH_PATH(MesaGL, $OPENGL_DIR/lib, OPENGL_SHARED_LIBADD)
        PHP_ADD_LIBRARY_WITH_PATH(MesaGLU, $OPENGL_DIR/lib, OPENGL_SHARED_LIBADD)
        failed=no
      fi
    else
      LIBS="$saved_LIBS"
      PHP_ADD_LIBRARY_WITH_PATH(GL, $OPENGL_DIR/lib, OPENGL_SHARED_LIBADD)
      PHP_ADD_LIBRARY_WITH_PATH(GLU, $OPENGL_DIR/lib, OPENGL_SHARED_LIBADD)
      failed=no
    fi
    if test "$failed" = "no"; then
      if test ! -z "$additional_libs"; then
        PHP_EVAL_LIBLINE($additional_libs, OPENGL_SHARED_LIBADD)
      fi
      break
    fi
  done
  if test "$failed" = "yeso"; then
    AC_MSG_ERROR(Can't find OpenGL nor MesaGL)
  fi
  unset additional_libs

  PHP_ADD_INCLUDE($OPENGL_DIR/include)
  PHP_ADD_INCLUDE(/System/Library/Frameworks/OpenGL.framework/Headers)

  PHP_SUBST(OPENGL_SHARED_LIBADD)
  PHP_NEW_EXTENSION(opengl, php_convert.c php_glu.c php_glut.c php_opengl.c, $ext_shared)
fi

