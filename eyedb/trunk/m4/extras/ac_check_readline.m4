AC_DEFUN([AC_CHECK_READLINE],[
ac_save_LIBS=$LIBS
case "$1" in
readline)
	AC_MSG_CHECKING(for readline)
	LIBS="$LIBS -lreadline"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM([[#include <readline/readline.h>]],[[readline("");]])],
		ac_cv_check_readline="yes",
		ac_cv_check_readline="no"
	)
	if test "$ac_cv_check_readline" = "yes" ; then
	   AC_MSG_RESULT(yes)
	   AC_DEFINE(HAVE_LIBREADLINE, 1, [Define if you have GNU readline library])
	else
	   AC_MSG_RESULT(no)
	   LIBS="$ac_save_LIBS"
	fi
;;
editline)
	AC_MSG_CHECKING(for editline)
	LIBS="$LIBS -leditline"
	AC_LINK_IFELSE(
		[AC_LANG_PROGRAM([[extern "C" { extern char *readline(char *);}]],[[readline("> ");]])],
		ac_cv_check_editline="yes",
		ac_cv_check_editline="no"
	)
	if test "$ac_cv_check_editline" = "yes" ; then
	   AC_MSG_RESULT(yes)
	   AC_DEFINE(HAVE_LIBEDITLINE, 1, [Define if you have BSD editline library])
	else
	   AC_MSG_RESULT(no)
	   LIBS="$ac_save_LIBS"
	fi
;;
esac
])
