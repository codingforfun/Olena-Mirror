#ifndef OLENA__TESTS__CHECK__SRCDIR__H
# define OLENA__TESTS__CHECK__SRCDIR__H

# include <string>

# ifdef HAVE_CONFIG_H
#  include <config.h>
# endif

extern std::string srcdir;

void srcdir_setup ();
std::string Isrcdir (const std::string& arg);

#endif // OLENA__TESTS__CHECK__SRCDIR__H
