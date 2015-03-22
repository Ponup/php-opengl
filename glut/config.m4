dnl config.m4 for extension glut

CFLAGS="$CFLAGS -pedantic -Wall -Wfatal-errors"

PHP_ARG_WITH(glut, for glut support,
[  --with-glut=DIR         Include GLUT support])

if test "$PHP_GLUT" != "no"; then

  if test -r $PHP_GLUT/include/GL/glut.h; then
    GLUT_DIR=$PHP_GLUT
  else
    AC_MSG_CHECKING(for GLUT development files in default path)
    for i in /usr/local /usr; do
      if test -r $i/include/GL/glut.h; then
        GLUT_DIR=$i
        AC_MSG_RESULT(found in $i)
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
  for additional_libs in "" "-lGL" "-lGL -lGLU" "-lGL -lGLU -lm"; do
    if test ! -z "$additional_libs"; then
      AC_MSG_CHECKING(checking with additional library $additional_libs)
    fi
    LIBS="$saved_LIBS -L$GLUT_DIR/lib -lglut $additional_libs"
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
  if test "$failed" = "yes"; then
    AC_MSG_ERROR(Something went wrong - Please check config.log.)
  fi
  unset additional_libs

  PHP_ADD_INCLUDE($GLUT_DIR/include)

  PHP_SUBST(GLUT_SHARED_LIBADD)
  PHP_EXTENSION(glut,$ext_shared)
fi
