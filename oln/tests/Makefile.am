# Process current dir first to built the data.
# Process the check directory next, because it contains functions
# needed by other directories.
SUBDIRS = check @OLN_TESTS_SUBDIRS@

ACLOCAL_AMFLAGS = -I ..
CONFIGURE_DEPENDENCIES = $(srcdir)/../oln.m4 $(srcdir)/../../oln-version.m4
