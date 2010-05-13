# M4 Macros for using ymlibs
# author: Yusuke Matsunaga (松永 裕介)
#
# $Id: ymlibs.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2009 Yusuke Matsunaga
# All rights reserved.

#==================================================================
# YM_CHECK_YMLIBS
#
# Description: check for 'ymlibs'
#==================================================================
AC_DEFUN([YM_CHECK_YMLIBS], [
AC_MSG_CHECKING([for ymlibs])
ym_tmp_found=no
ym_tmp_use_src=no
ym_tmp_ymlibsdir=../ymlibs
AC_ARG_WITH(ymlibs,
            AS_HELP_STRING([--with-ymlibs=DIR],
                           [ymConfig.sh is in DIR]),
[ #--with-ymlibs specified
if test "x$withval" != x && test -f $withval/ymConfig.sh; then
  #found
  ym_tmp_dir=$withval
  ym_tmp_found=yes
fi],
[ #not specified
if test -f $ym_tmp_ymlibsdir/ymConfig.sh; then
  #found source tree
  ym_tmp_dir=$ym_tmp_ymlibsdir
  ym_tmp_found=yes
  ym_tmp_use_src=yes
else
  arch=`uname -p`
  if test "X$arch" = "Xx86_64"; then
    ym_tmp_lib="lib64"
  else
    ym_tmp_lib="lib"
  fi
  ym_tmp_dirlist="$prefix/$ym_tmp_lib $ac_default_prefix/$ym_tmp_lib /usr/$ym_tmp_lib /usr/local/$ym_tmp_lib"
  ym_tmp_subdirlist="ymlibs1.0 ymlibs"
  for dir in $ym_tmp_dirlist
  do
    if test -f $dir/ymConfig.sh; then
      ym_tmp_dir=$dir
      ym_tmp_found=yes
    else
      for subdir in $ym_tmp_subdirlist
      do
        if test -f $dir/$subdir/ymConfig.sh; then
          ym_tmp_dir=$dir/$subdir
          ym_tmp_found=yes
          break
        fi
      done
    fi
    if test $ym_tmp_found = yes; then break; fi
  done
fi
])#end-of AC_ARG_WITH

if test $ym_tmp_found = yes; then
  #read config file
  . $ym_tmp_dir/ymConfig.sh

  if test $ym_tmp_use_src = yes; then
    YM_INCLUDES="\$(GMP_INCLUDES) -I\$(top_srcdir)/../ymlibs/include -I\$(top_builddir)/../ymlibs"
    ym_tmp_libdir='\$\(top_builddir\)/../ymlibs'
    for module in ${YM_LIBS_MODULES}
    do
      MODULE=`echo $module | sed -e y/$as_cr_letters/$as_cr_LETTERS/`
      modvar=`eval echo "$"YM_LIBS_LIBYM_${MODULE}`
      if test "x$modvar" != x; then
        eval YM_LIBS_LIBYM_${MODULE}="${ym_tmp_libdir}/libym_${module}/libym_${module}.la"
      fi
    done
    AC_MSG_RESULT([use source tree])
  else
    AC_MSG_RESULT([$ym_tmp_dir])
  fi
  AC_SUBST([YM_INCLUDES])
else
  AC_MSG_ERROR([not found])
fi
])


#==================================================================
# YM_CHECK_LIBS(libname)
#
#==================================================================
AC_DEFUN([YM_CHECK_LIBS], [
__YM_CHECK_LIBS_SUB($1, m4_toupper($1))
])

AC_DEFUN([__YM_CHECK_LIBS_SUB],[
ym_tmp_found=yes
AC_MSG_CHECKING([for $1])
if test "x${YM_LIBS_$2}" = x; then
  ym_tmp_found=no
  AC_MSG_ERROR([not found])
else
  AC_SUBST([YM_LIBS_$2])
  AC_MSG_RESULT([$YM_LIBS_$2])
fi
])
]