
SUBDIRS = $(OLN_TOOLS_SUBDIRS)

ACLOCAL_AMFLAGS = -I ../oln -I ..
CONFIGURE_DEPENDENCIES = $(srcdir)/../oln/oln.m4 \
	$(srcdir)/../oln-version.m4 \
	$(srcdir)/../component.m4

maintainer-check:
	if cppi --version >/dev/null 2>&1 then \
	  find . -name *.cc -name *.hh -name *.hxx | xargs cppi --check; \
	else \
	  echo "$@: GNU cppi is required" >&2; \
	fi
