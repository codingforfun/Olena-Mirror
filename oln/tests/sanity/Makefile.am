EXTRA_DIST = includes.test

check-local:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
        TOP_BUILDDIR=$(top_builddir) \
        PATH_TO_OLN_SRCDIR=$(OLN_LOCAL_SRC) \
	PATH_TO_OLN_BUILDDIR=$(OLN_LOCAL_BUILD) \
	  $(SHELL) $(srcdir)/includes.test

CLEANFILES = failures incltest.*
