// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef METALIC_OPTIONAL_HH
# define METALIC_OPTIONAL_HH

// FIXME: this code is neither tested or used anywhere. Should stay
// disabled until at least one test is written. -- Nicolas Burrus

# if 0

namespace mlc 
{

  // This class is used as a static tag for specialization of certain
  // algorithm. Usually, it is used to mention that there is nothing
  // to do.
  struct none {};

  // Option encapsulates a value in a sum type : Some t | None.
  template <class T>
  struct Option
  {
    Option(none) : t_(0)  {}
    Option(T& t) : t_(&t) {}
    T* t_;
    operator T&() { return *t_; }
  };

  // Optional is meant to enable optional static argument with variant
  // types. The 'Decl' argument must have a free template argument to
  // permit the substitution of the class 'none' to it when no
  // argument is given. 
  // example: 
  // 
  // template <class U, class T>
  // void foo(U arg1, Optional(T& arg2)) // here T is free.
  // {} 
  //
  // Then, you can write:
  // foo(1);         // T = none
  // foo(1, 2);      // T = int
  // foo(1, "bar");  // T = char *
# define Optional(T, v)  ((Option<T> v) = none())

  // Depending on the presence of optional argument, we have different
  // specialization of the algorithm. Here are several helper classes
  // and macros.
 
  // Do nothing in the general case.
  template <class Behaviour, class OptionalArgument>
  struct optional_behavior
  {
    static inline void do_it()
    {}
    
    template <class T>
    static inline void do_it(T) 
    {}

    template <class T, class U>
    static inline void do_it(T, U) 
    {}

    template <class T, class U, class V>
    static inline void do_it(T, U, V) 
    {}
  };

# define DoNothingWhenNone(Behaviour) \
  template <>					\
  struct optional_behavior<Behaviour, none>  	\
  {						\
    static inline void do_it()			\
    {}						\
    						\
    template <class T>				\
    static inline void do_it(T) 		\
    {}						\
						\
    template <class T, class U>			\
    static inline void do_it(T, U) 		\
    {}						\
						\
    template <class T, class U, class V>	\
    static inline void do_it(T, U, V) 		\
    {}						\
  };


} // end of mlc

# endif

#endif // METALIC_OPTIONAL_HH
