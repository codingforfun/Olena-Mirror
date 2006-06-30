								// -*- C++ -*-

// Workaround for ruby.hh defining some macros already defined in the
// top-level config.h (PACKAGE_BUGREPORT, PACKAGE_NAME,
// PACKAGE_STRING, PACKAGE_TARNAME, PACKAGE_VERSION).  Simply undef
// them to avoid redefinitions.

%{
#if defined(SWIGRUBY)

# ifdef PACKAGE_BUGREPORT
#  undef PACKAGE_BUGREPORT
# endif

# ifdef PACKAGE_NAME
#  undef PACKAGE_NAME
# endif

# ifdef PACKAGE_STRING
#  undef PACKAGE_STRING
# endif

# ifdef PACKAGE_TARNAME
#  undef PACKAGE_TARNAME
# endif

# ifdef PACKAGE_VERSION
#  undef PACKAGE_VERSION
# endif

#endif // SWIGRUBY
%}
