# PKG_CHECK_VAR(VARIABLE-PREFIX, MODULE, VARIABLE-NAME, PKG-CONFIG-VARIABLE-NAME)

AC_DEFUN([PKG_CHECK_VAR],
[AC_REQUIRE([PKG_PROG_PKG_CONFIG])dnl
AC_ARG_VAR([$1][_$3], [$3 variable for $1, overriding pkg-config])dnl

pkg_failed=no
AC_CACHE_CHECK([for $1][_$3], [pkg_cv_][$1][_$3],
	[_PKG_CONFIG([$1][_$3], variable=[$4], [$2])])

if test $pkg_failed = yes; then
	$1[]_PKG_ERRORS=`$PKG_CONFIG --errors-to-stdout --print-errors "$2"`
	# Put the nasty error message in config.log where it belongs
	echo "$$1[]_PKG_ERRORS" 1>&AS_MESSAGE_LOG_FD

	AC_MSG_ERROR(dnl
[Package requirements ($2) were not met.
Consider adjusting the PKG_CONFIG_PATH environment variable if you
installed software in a non-standard prefix.

Alternatively you may set the $1_$3 environment variable
to avoid the need to call pkg-config.  See the pkg-config man page for
more details.])

elif test $pkg_failed = untried; then
	AC_MSG_FAILURE(dnl
[The pkg-config script could not be found or is too old.  Make sure it
is in your PATH or set the PKG_CONFIG environment variable to the full
path to pkg-config.

Alternatively you may set the $1_$3 environment variable
to avoid the need to call pkg-config.  See the pkg-config man page for
more details.

To get pkg-config, see <http://www.freedesktop.org/software/pkgconfig>.])
else
	$1[]_$3=$pkg_cv_[]$1[]_$3
fi[]dnl
])# PKG_CHECK_VAR
