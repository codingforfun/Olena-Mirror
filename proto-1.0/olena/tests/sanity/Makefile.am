EXTRA_DIST = includes.test

check:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
        TOP_BUILDDIR=$(top_builddir) \
        PATH_TO_OLN_SRCDIR=$(OLN_LOCAL_SRC) \
	PATH_TO_OLN_BUILDDIR=$(OLN_LOCAL_BUILD) \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	  $(SHELL) $(srcdir)/includes.test

CLEANFILES = failures incltest.*
