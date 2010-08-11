# Build Qt apps with the autotools (Autoconf/Automake).
# M4 macros.
# This file is part of AutoTroll.
# Copyright (C) 2006  Benoit Sigoure <benoit.sigoure@lrde.epita.fr>
#
# AutoTroll is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
# USA.
#
# In addition, as a special exception, the copyright holders of AutoTroll
# give you unlimited permission to copy, distribute and modify the configure
# scripts that are the output of Autoconf when processing the macros of
# AutoTroll.  You need not follow the terms of the GNU General Public License
# when using or distributing such scripts, even though portions of the text of
# AutoTroll appear in them. The GNU General Public License (GPL) does govern
# all other use of the material that constitutes AutoTroll.
#
# This special exception to the GPL applies to versions of AutoTroll
# released by the copyright holders of AutoTroll.  Note that people who make
# modified versions of AutoTroll are not obligated to grant this special
# exception for their modified versions; it is their choice whether to do so.
# The GNU General Public License gives permission to release a modified version
# without this exception; this exception also makes it possible to release a
# modified version which carries forward this exception.

 # ------------- #
 # DOCUMENTATION #
 # ------------- #

# Disclaimer: Never tested with anything else than Qt 4.2! Feedback welcome.
# Simply invoke AT_WITH_QT in your configure.ac. AT_WITH_QT can take
# arguments which are documented in depth below. The default arguments are
# equivalent to the default .pro file generated by qmake.
#
# Invoking AT_WITH_QT will do the following:
#  - Add a --with-qt option to your configure
#  - Find qmake, moc and uic and save them in the make variables $(QMAKE),
#    $(MOC), $(UIC).
#  - Save the path to Qt in $(QT_PATH)
#  - Find the flags to use Qt, that is:
#     * $(QT_DEFINES): -D's defined by qmake.
#     * $(QT_CFLAGS): CFLAGS as defined by qmake (C?!)
#     * $(QT_CXXFLAGS): CXXFLAGS as defined by qmake.
#     * $(QT_INCPATH): -I's defined by qmake.
#     * $(QT_CPPFLAGS): Same as $(QT_DEFINES) + $(QT_INCPATH)
#     * $(QT_LFLAGS): LFLAGS defined by qmake.
#     * $(QT_LDFLAGS): Same thing as $(QT_LFLAGS).
#     * $(QT_LIBS): LIBS defined by qmake.
#
# You *MUST* invoke $(MOC) and/or $(UIC) where necessary. AutoTroll provides
# you with Makerules to ease this, here is a sample Makefile.am to use with
# AutoTroll which builds the code given in the chapter 7 of the Qt Tutorial:
# http://doc.trolltech.com/4.2/tutorial-t7.html
#
# -------------------------------------------------------------------------
# include $(top_srcdir)/build-aux/autotroll.mk
#
# ACLOCAL_AMFLAGS = -I build-aux
#
# bin_PROGRAMS = lcdrange
# lcdrange_SOURCES =  $(BUILT_SOURCES) lcdrange.cpp lcdrange.h main.cpp
# lcdrange_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)
# lcdrange_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
# lcdrange_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
# lcdrange_LDADD    = $(QT_LIBS) $(LDADD)
#
# BUILT_SOURCES = lcdrange.moc.cpp
# -------------------------------------------------------------------------
#
# Note that your MOC, UIC and QRC files *MUST* be listed manually in
# BUILT_SOURCES. If you name them properly (eg: .moc.cc, .qrc.cc, .ui.cc -- of
# course you can use .cpp or .cxx or .C rather than .cc) AutoTroll will build
# them automagically for you (using implicit rules defined in autotroll.mk).

m4_define([_AUTOTROLL_SERIAL], [m4_translit([
# serial 4
], [#
], [])])


m4_ifdef([AX_INSTEAD_IF], [],
[AC_DEFUN([AX_INSTEAD_IF],
  [m4_ifval([$1],
    [AC_MSG_WARN([$2]); [$1]],
    [AC_MSG_ERROR([$2])])])])

m4_pattern_forbid([^AT_])dnl
m4_pattern_forbid([^_AT_])dnl

# AT_WITH_QT([QT_modules], [QT_config], [QT_misc], [RUN-IF-FAILED], [RUN-IF-OK])
# ------------------------------------------------------------------------------
# Enable Qt support and add an option --with-qt to the configure script.
#
# The QT_modules argument is optional and defines extra modules to enable or
# disable (it's equivalent to the QT variable in .pro files). Modules can be
# specified as follows:
#
# AT_WITH_QT   => No argument -> No QT value.
#                                Qmake sets it to "core gui" by default.
# AT_WITH_QT([xml])   => QT += xml
# AT_WITH_QT([+xml])  => QT += xml
# AT_WITH_QT([-gui])  => QT -= gui
# AT_WITH_QT([xml -gui +sql svg])  => QT += xml sql svg
#                                     QT -= gui
#
# The QT_config argument is also optional and follows the same convention as
# QT_modules. Instead of changing the QT variable, it changes the CONFIG
# variable, which is used to tweak configuration and compiler options.
#
# The last argument, QT_misc (also optional) will be copied as-is the .pro
# file used to guess how to compile Qt apps. You may use it to further tweak
# the build process of Qt apps if tweaking the QT or CONFIG variables isn't
# enough for you.
#
# RUN-IF-FAILED is arbitrary code to execute if Qt cannot be found or if any
# problem happens.  If this argument is omitted, then AC_MSG_ERROR will be
# called.  RUN-IF-OK is arbitrary code to execute if Qt was successfully found.
AC_DEFUN([AT_WITH_QT],
[AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_CANONICAL_BUILD])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
echo "$as_me: this is autotroll.m4[]_AUTOTROLL_SERIAL" >&AS_MESSAGE_LOG_FD

  test x"$TROLL" != x && echo 'ViM rox emacs.'

dnl Memo: AC_ARG_WITH(package, help-string, [if-given], [if-not-given])
  AC_ARG_WITH([qt],
              [AS_HELP_STRING([--with-qt],
                 [Path to Qt @<:@Look in PATH and /usr/local/Trolltech@:>@])],
              [QT_PATH=$withval])

  # this is a hack to get decent flow control with 'break'
  for _qt_ignored in once; do

  # Find Qt.
  AC_ARG_VAR([QT_PATH], [Path to the Qt installation])
  if test -d /usr/local/Trolltech; then
    # Try to find the latest version.
    tmp_qt_paths=`echo /usr/local/Trolltech/*/bin | tr ' ' '\n' | sort -nr \
                                              | xargs | sed 's/  */:/g'`
  fi
  # Path to which recent MacPorts (~v1.7) install Qt4.
  test -d /opt/local/libexec/qt4-mac/bin \
    && tmp_qt_paths="$tmp_qt_paths:/opt/local/libexec/qt4-mac/bin"

  # Find qmake.
  AC_ARG_VAR([QMAKE], [Qt Makefile generator command])
  AC_PATH_PROGS([QMAKE], [qmake qmake-qt4 qmake-qt3], [missing],
                [$QT_DIR:$QT_PATH:$PATH:$tmp_qt_paths])
  if test x"$QMAKE" = xmissing; then
    AX_INSTEAD_IF([$4], [Cannot find qmake in your PATH. Try using --with-qt.])
    break
  fi

  # Find moc (Meta Object Compiler).
  AC_ARG_VAR([MOC], [Qt Meta Object Compiler command])
  AC_PATH_PROGS([MOC], [moc moc-qt4 moc-qt3], [missing],
                [$QT_PATH:$PATH:$tmp_qt_paths])
  if test x"$MOC" = xmissing; then
    AX_INSTEAD_IF([$4],
   [Cannot find moc (Meta Object Compiler) in your PATH. Try using --with-qt.])
    break
  fi

  # Find uic (User Interface Compiler).
  AC_ARG_VAR([UIC], [Qt User Interface Compiler command])
  AC_PATH_PROGS([UIC], [uic uic-qt4 uic-qt3 uic3], [missing],
                [$QT_PATH:$PATH:$tmp_qt_paths])
  if test x"$UIC" = xmissing; then
    AX_INSTEAD_IF([$4],
[Cannot find uic (User Interface Compiler) in your PATH. Try using --with-qt.])
    break
  fi

  # Find rcc (Qt Resource Compiler).
  AC_ARG_VAR([RCC], [Qt Resource Compiler command])
  AC_PATH_PROGS([RCC], [rcc], [false], [$QT_PATH:$PATH:$tmp_qt_paths])
  if test x"$UIC" = xfalse; then
    AC_MSG_WARN([Cannot find rcc (Qt Resource Compiler) in your PATH.\
  Try using --with-qt.])
  fi

  AC_MSG_CHECKING([whether host operating system is Darwin])
  at_darwin=no
  at_qmake_args=
  case $host_os in
    darwin*)
      at_darwin=yes
      at_qmake_args='-spec macx-g++'
      ;;
  esac
  AC_MSG_RESULT([$at_darwin])

  # If we don't know the path to Qt, guess it from the path to qmake.
  if test x"$QT_PATH" = x; then
    QT_PATH=`dirname "$QMAKE"`
  fi
  if test x"$QT_PATH" = x; then
    AX_INSTEAD_IF([$4],
                  [Cannot find the path to your Qt install. Use --with-qt.])
    break
  fi
  AC_SUBST([QT_PATH])

  # Get ready to build a test-app with Qt.
  if mkdir conftest.dir && cd conftest.dir; then :; else
    AX_INSTEAD_IF([$4], [Cannot mkdir conftest.dir or cd to that directory.])
    break
  fi

  cat >conftest.h <<_ASEOF
#include <QObject>

class Foo: public QObject
{
  Q_OBJECT;
public:
  Foo();
  ~Foo() {}
public Q_SLOTS:
  void setValue(int value);
Q_SIGNALS:
  void valueChanged(int newValue);
private:
  int value_;
};
_ASEOF

  cat >conftest.cpp <<_ASEOF
#include "conftest.h"
Foo::Foo()
  : value_ (42)
{
  connect(this, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
}

void Foo::setValue(int value)
{
  value_ = value;
}

int main()
{
  Foo f;
}
_ASEOF
  if $QMAKE -project; then :; else
    AX_INSTEAD_IF([$4], [Calling $QMAKE -project failed.])
    break
  fi

  # Find the .pro file generated by qmake.
  pro_file='conftest.dir.pro'
  test -f $pro_file || pro_file=`echo *.pro`
  if test -f "$pro_file"; then :; else
    AX_INSTEAD_IF([$4], [Can't find the .pro file generated by Qmake.])
    break
  fi

dnl Tweak the value of QT in the .pro if have been the 1st arg.
m4_ifval([$1], [_AT_TWEAK_PRO_FILE([QT], [$1])])

dnl Tweak the value of CONFIG in the .pro if have been given a 2nd arg.
m4_ifval([$2], [_AT_TWEAK_PRO_FILE([CONFIG], [$2])])

m4_ifval([$3],
[ # Add the extra-settings the user wants to set in the .pro
  echo "$3" >>"$pro_file"
])

  echo "$as_me:$LINENO: Invoking $QMAKE on $pro_file" >&AS_MESSAGE_LOG_FD
  sed 's/^/| /' "$pro_file" >&AS_MESSAGE_LOG_FD

  if $QMAKE $at_qmake_args; then :; else
    AX_INSTEAD_IF([$4], [Calling $QMAKE $at_qmake_args failed.])
    break
  fi

  # QMake has a very annoying misfeature: sometimes it generates Makefiles
  # where all the references to the files from the Qt installation are
  # relative.  We can't use them as-is because if we take, say, a
  # -I../../usr/include/Qt from that Makefile, the flag is invalid as soon
  # as we use it in another (sub) directory.  So what this perl pass does is
  # that it rewrite all relative paths to absolute paths.  Another problem
  # when building on Cygwin is that QMake mixes paths with blackslashes and
  # forward slashes and paths must be handled with extra care because of the
  # stupid Windows drive letters.
  echo "$as_me:$LINENO: fixing the Makefiles:" Makefile* >&AS_MESSAGE_LOG_FD
  cat >fixmk.pl <<\EOF
[use strict;
use Cwd qw(cwd abs_path);
# This variable is useful on Cygwin for the following reason: Say that you are
# in `/' (that is, in fact you are in C:/cygwin, or something like that) if you
# `cd ..' then obviously you remain in `/' (that is in C:/cygwin).  QMake
# generates paths that are relative to C:/ (or another driver letter, whatever)
# so the trick to get the `..' resolved properly is to prepend the absolute
# path of the current working directory in a Windows-style.  C:/cygwin/../ will
# properly become C:/.
my $d = "";
my $r2a = 0;
my $b2f = 0;

my $cygwin = 0;
if ($^O eq "cygwin") {
  $cygwin = 1;
  $d = cwd();
  $d = `cygpath --mixed '$d'`;
  chomp($d);
  $d .= "/";
}

sub rel2abs($)
{
  my $p = $d . shift;
  # print "r2a p=$p";
  -e $p || return $p;
  if ($cygwin) {
    $p = `cygpath --mixed '$p'`;
    chomp($p);
  }
  else {
    # Do not use abs_path on Cygwin: it incorrectly resolves the paths that are
    # relative to C:/ rather than `/'.
    $p = abs_path($p);
  }
  # print " -> $p\n";
  ++$r2a;
  return $p;
}

# Only useful on Cygwin.
sub back2forward($)
{
  my $p = shift;
  # print "b2f p=$p";
  -e $p || return $p;
  $p = `cygpath --mixed '$p'`;
  chomp($p);
  # print " -> $p\n";
  ++$b2f;
  return $p;
}

foreach my $mk (@ARGV)
{
  next if $mk =~ /~$/;
  open(MK, $mk) or die("open $mk: $!");
  # print "mk=$mk\n";
  my $file = join("", <MK>);
  close(MK) or die("close $mk: $!");
  rename $mk, $mk . "~" or die("rename $mk: $!");
  $file =~ s{(?:\.\.[\\/])+(?:[^"'\s:]+)}{rel2abs($&)}gse;
  $file =~ s{(?:[a-zA-Z]:[\\/])?(?:[^"\s]+\\[^"\s:]+)+}
            {back2forward($&)}gse if $cygwin;
  open(MK, ">", $mk) or die("open >$mk: $!");
  print MK $file;
  close(MK) or die("close >$mk: $!");
  print "$mk: updated $r2a relative paths and $b2f backslash-style paths\n";
  $r2a = 0;
  $b2f = 0;
}]
EOF

  perl >&AS_MESSAGE_LOG_FD -w fixmk.pl Makefile* ||
  AC_MSG_WARN([failed to fix the Makefiles generated by $QMAKE])
  rm -f fixmk.pl

  # Try to compile a simple Qt app.
  AC_CACHE_CHECK([whether we can build a simple Qt app], [at_cv_qt_build],
  [at_cv_qt_build=ko
  : ${MAKE=make}

  if $MAKE >&AS_MESSAGE_LOG_FD 2>&1; then
    at_cv_qt_build='ok, looks like Qt 4'
  else
    echo "$as_me:$LINENO: Build failed, trying to #include <qobject.h> \
instead" >&AS_MESSAGE_LOG_FD
    sed 's/<QObject>/<qobject.h>/' conftest.h > tmp.h && mv tmp.h conftest.h
    if $MAKE >&AS_MESSAGE_LOG_FD 2>&1; then
      at_cv_qt_build='ok, looks like Qt 3'
    else
      # Sometimes (such as on Debian) build will fail because Qt hasn't been
      # installed in debug mode and qmake tries (by default) to build apps in
      # debug mode => Try again in release mode.
      echo "$as_me:$LINENO: Build failed, trying to enforce release mode" \
            >&AS_MESSAGE_LOG_FD

      _AT_TWEAK_PRO_FILE([CONFIG], [+release])

      sed 's/<qobject.h>/<QObject>/' conftest.h > tmp.h && mv tmp.h conftest.h
      if $MAKE >&AS_MESSAGE_LOG_FD 2>&1; then
        at_cv_qt_build='ok, looks like Qt 4, release mode forced'
      else
        echo "$as_me:$LINENO: Build failed, trying to #include <qobject.h> \
instead" >&AS_MESSAGE_LOG_FD
        sed 's/<QObject>/<qobject.h>/' conftest.h >tmp.h && mv tmp.h conftest.h
        if $MAKE >&AS_MESSAGE_LOG_FD 2>&1; then
          at_cv_qt_build='ok, looks like Qt 3, release mode forced'
        else
          at_cv_qt_build=ko
          echo "$as_me:$LINENO: failed program was:" >&AS_MESSAGE_LOG_FD
          sed 's/^/| /' conftest.h >&AS_MESSAGE_LOG_FD
          echo "$as_me:$LINENO: failed program was:" >&AS_MESSAGE_LOG_FD
          sed 's/^/| /' conftest.cpp >&AS_MESSAGE_LOG_FD
        fi # if make with Qt3-style #include and release mode forced.
      fi # if make with Qt4-style #include and release mode forced.
    fi # if make with Qt3-style #include.
  fi # if make with Qt4-style #include.
  ])dnl end: AC_CACHE_CHECK(at_cv_qt_build)

  if test x"$at_cv_qt_build" = xko; then
    AX_INSTEAD_IF([$4], [Cannot build a test Qt program])
    cd ..
    break
  fi
  QT_VERSION_MAJOR=`echo "$at_cv_qt_build" | sed 's/[[^0-9]]*//g'`
  AC_SUBST([QT_VERSION_MAJOR])

  # This sed filter is applied after an expression of the form: /^FOO.*=/!d;
  # It starts by removing the beginning of the line, removing references to
  # SUBLIBS, removing unnecessary whitespaces at the beginning, and prefixes
  # all variable uses by QT_.
  qt_sed_filter='s///;
                 s/$(SUBLIBS)//g;
                 s/^ *//;
                 s/\$(\(@<:@A-Z_@:>@@<:@A-Z_@:>@*\))/$(QT_\1)/g'

  # Find the Makefile (qmake happens to generate a fake Makefile which invokes
  # a Makefile.Debug or Makefile.Release). We we have both, we'll pick the
  # Makefile.Release. The reason is that the main difference is that release
  # uses -Os and debug -g. We can override -Os by passing another -O but we
  # usually don't override -g.
  if test -f Makefile.Release; then
    at_mfile='Makefile.Release'
  else
    at_mfile='Makefile'
  fi
  if test -f $at_mfile; then :; else
    AX_INSTEAD_IF([$4], [Cannot find the Makefile generated by qmake.])
    cd ..
    break
  fi

  # Find the DEFINES of Qt (should have been named CPPFLAGS).
  AC_CACHE_CHECK([for the DEFINES to use with Qt], [at_cv_env_QT_DEFINES],
  [at_cv_env_QT_DEFINES=`sed "/^DEFINES@<:@^A-Z=@:>@*=/!d;$qt_sed_filter" $at_mfile`])
  AC_SUBST([QT_DEFINES], [$at_cv_env_QT_DEFINES])

  # Find the CFLAGS of Qt (We can use Qt in C?!)
  AC_CACHE_CHECK([for the CFLAGS to use with Qt], [at_cv_env_QT_CFLAGS],
  [at_cv_env_QT_CFLAGS=`sed "/^CFLAGS@<:@^A-Z=@:>@*=/!d;$qt_sed_filter" $at_mfile`])
  AC_SUBST([QT_CFLAGS], [$at_cv_env_QT_CFLAGS])

  # Find the CXXFLAGS of Qt.
  AC_CACHE_CHECK([for the CXXFLAGS to use with Qt], [at_cv_env_QT_CXXFLAGS],
  [at_cv_env_QT_CXXFLAGS=`sed "/^CXXFLAGS@<:@^A-Z=@:>@*=/!d;$qt_sed_filter" $at_mfile`])
  AC_SUBST([QT_CXXFLAGS], [$at_cv_env_QT_CXXFLAGS])

  # Find the INCPATH of Qt.
  AC_CACHE_CHECK([for the INCPATH to use with Qt], [at_cv_env_QT_INCPATH],
  [at_cv_env_QT_INCPATH=`sed "/^INCPATH@<:@^A-Z=@:>@*=/!d;$qt_sed_filter" $at_mfile`])
  AC_SUBST([QT_INCPATH], [$at_cv_env_QT_INCPATH])

  AC_SUBST([QT_CPPFLAGS], ["$at_cv_env_QT_DEFINES $at_cv_env_QT_INCPATH"])

  # Find the LFLAGS of Qt (Should have been named LDFLAGS)
  AC_CACHE_CHECK([for the LDFLAGS to use with Qt], [at_cv_env_QT_LDFLAGS],
  [at_cv_env_QT_LDFLAGS=`sed "/^LFLAGS@<:@^A-Z=@:>@*=/!d;$qt_sed_filter" $at_mfile`])
  AC_SUBST([QT_LFLAGS], [$at_cv_env_QT_LDFLAGS])
  AC_SUBST([QT_LDFLAGS], [$at_cv_env_QT_LDFLAGS])

  # Find the LIBS of Qt.
  AC_CACHE_CHECK([for the LIBS to use with Qt], [at_cv_env_QT_LIBS],
  [at_cv_env_QT_LIBS=`sed "/^LIBS@<:@^A-Z@:>@*=/!d;$qt_sed_filter" $at_mfile`
   if test x$at_darwin = xyes; then
     # Fix QT_LIBS: as of today Libtool (GNU Libtool 1.5.23a) doesn't handle
     # -F properly. The "bug" has been fixed on 22 October 2006
     # by Peter O'Gorman but we provide backward compatibility here.
     at_cv_env_QT_LIBS=`echo "$at_cv_env_QT_LIBS" \
                             | sed 's/^-F/-Wl,-F/;s/ -F/ -Wl,-F/g'`
   fi
  ])
  AC_SUBST([QT_LIBS], [$at_cv_env_QT_LIBS])

  cd .. && rm -rf conftest.dir

  # Run the user code
  $5

  done  # end hack (useless for to be able to use break)
])

# AT_REQUIRE_QT_VERSION(QT_version, RUN-IF-FAILED, RUN-IF-OK)
# -----------------------------------------------------------
# Check (using qmake) that Qt's version "matches" QT_version.
# Must be run AFTER AT_WITH_QT. Requires autoconf 2.60.
#
# RUN-IF-FAILED is arbitrary code to execute if Qt cannot be found or if any
# problem happens.  If this argument is omitted, then AC_MSG_ERROR will be
# called.  RUN-IF-OK is arbitrary code to execute if Qt was successfully found.
AC_DEFUN([AT_REQUIRE_QT_VERSION],
[ AC_PREREQ([2.60])
  # this is a hack to get decent flow control with 'break'
  for _qt_ignored in once; do

  if test x"$QMAKE" = x; then
    AX_INSTEAD_IF([$2],
                  [\$QMAKE is empty.\
  Did you invoke AT@&t@_WITH_QT before AT@&t@_REQUIRE_QT_VERSION?])
    break
  fi
  AC_CACHE_CHECK([for Qt's version], [at_cv_QT_VERSION],
  [echo "$as_me:$LINENO: Running $QMAKE --version:" >&AS_MESSAGE_LOG_FD
  $QMAKE --version >&AS_MESSAGE_LOG_FD 2>&1
  qmake_version_sed=['/^.*\([0-9]\.[0-9]\.[0-9]\).*$/!d;s//\1/']
  at_cv_QT_VERSION=`$QMAKE --version 2>&1 | sed "$qmake_version_sed"`])
  if test x"$at_cv_QT_VERSION" = x; then
    AX_INSTEAD_IF([$2], [Cannot detect Qt's version.])
    break
  fi
  AC_SUBST([QT_VERSION], [$at_cv_QT_VERSION])
  AS_VERSION_COMPARE([$QT_VERSION], [$1],
    [AX_INSTEAD_IF([$2; break; ], [This package requires Qt $1 or above.])])

  # Run the user code
  $3

  done  # end hack (useless for to be able to use break)
])

# _AT_TWEAK_PRO_FILE(QT_VAR, VALUE)
# ---------------------------
# @internal. Tweak the variable QT_VAR in the .pro.
# VALUE is an IFS-separated list of value and each value is rewritten
# as follows:
#   +value  => QT_VAR += value
#   -value  => QT_VAR -= value
#    value  => QT_VAR += value
AC_DEFUN([_AT_TWEAK_PRO_FILE],
[ # Tweak the value of $1 in the .pro file for $2.

  qt_conf=''
  for at_mod in $2; do
    at_mod=`echo "$at_mod" | sed 's/^-//; tough
                                  s/^+//; beef
                                  :ough
                                  s/^/$1 -= /;n
                                  :eef
                                  s/^/$1 += /'`
    qt_conf="$qt_conf
$at_mod"
  done
  echo "$qt_conf" | sed 1d >>"$pro_file"
])
