EXTRA_DIST = tests runtests

check-local:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
        TOP_BUILDDIR=$(top_builddir) \
        PATH_TO_OLN_SRCDIR=$(OLN_LOCAL_SRC) \
	PATH_TO_OLN_BUILDDIR=$(OLN_LOCAL_BUILD) \
	  $(SHELL) $(srcdir)/runtests

distclean-local:
	rm -rf runs
