EXTRA_DIST = includes.test

maintainer-check:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
        TOP_BUILDDIR=$(top_builddir) \
        PATH_TO_NTG_SRCDIR=$(NTG_LOCAL_SRC) \
	PATH_TO_NTG_BUILDDIR=$(NTG_LOCAL_BUILD) \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	  $(SHELL) $(srcdir)/includes.test

CLEANFILES = failures incltest.*
