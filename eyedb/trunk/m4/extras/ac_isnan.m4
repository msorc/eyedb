dnl checking for isnan
AC_DEFUN([AC_ISNAN],[
AC_MSG_CHECKING(for isnan)
AC_LANG_ASSERT(C++)
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[#include <math.h>]],[[int i = isnan( 3.141592);]])],
	[ac_isnan="yes"],
	[ac_isnan="no"]
)
if test "$ac_isnan" = "yes"; then
	AC_DEFINE(HAVE_ISNAN, 1, [Define if isnan exists])
	AC_MSG_RESULT(isnan)
else
	AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#include <math.h>]],[[int i = std::isnan( 3.141592);]])],
		[ac_std_isnan="yes"],
		[ac_std_isnan="no"]
	)
	if test "$ac_std_isnan" =" yes"; then
		AC_DEFINE(HAVE_STD_ISNAN, 1, [Define if std::isnan exists])
		AC_MSG_RESULT(std::isnan)
	else
		AC_MSG_ERROR(isnan is not defined)
	fi
fi
])


