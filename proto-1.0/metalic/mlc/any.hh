// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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


#ifndef METALIC_ANY_HH
# define METALIC_ANY_HH




// abbrev
#define any_mem(E) any <E, dispatch_policy::best_memory>



// 'any' classes

namespace mlc
{

  struct dispatch_policy
  {
    struct best_speed;
    struct best_memory;
    struct simple;
  };


  // any

  template <typename E, typename Dispatch_Policy = dispatch_policy::simple>
  struct any;


  // "best speed" version of 'any'

  template <typename E>
  struct any <E,
              dispatch_policy::best_speed>
  {
    E& exact() {
      precondition(exact_ptr != 0);
      return *exact_ptr;
    }
    const E& exact() const {
      precondition(exact_ptr != 0);
      return *exact_ptr;
    }
    
    /// Assignment (op=).

    any& operator=(const any& rhs)
    {
      // do nothing
      // so that "this->exact_ptr" is unchanged
      return *this;
    }

    /// Copy Ctor.

    any(const any& rhs) :
      exact_ptr(0) // safety
    {
      // this->exact_ptr is *not* copied
      // its value has to be set in subclasses copy ctors.
    }

  protected:
    any(E* exact_ptr) : exact_ptr(exact_ptr) {}
    virtual ~any() {}

    /// Only attribute.
    E* exact_ptr;
  };


  template <typename E>
  struct any__best_speed : public any<E, dispatch_policy::best_speed>
  {
  protected:
    typedef any<E, dispatch_policy::best_speed> super;
    any__best_speed(E* exact_ptr = 0) : super(exact_ptr) {}
  };

  // "best memory" version of 'any'

  template <typename E>
  struct any <E,
	      dispatch_policy::best_memory>
  {
    E& exact() {
      return *(E*)((char*)this - exact_offset);
    }
    const E& exact() const {
      return *(const E*)((const char*)this - exact_offset);
    }
    
    static const int exact_offset;
    static const E exact_obj;
    static const any_mem(E)& ref_exact_obj;
  
  protected:
    any() {}
    any(E* exact_ptr_does_not_exist_in_this_version); // safety
    virtual ~any() {} // FIXME: virtual for a "best memory" version!?
  };

  template <typename E>
  struct any__best_memory : public any<E, dispatch_policy::best_memory>
  {
  protected:
    typedef any<E, dispatch_policy::best_memory> super;
    any__best_memory() : super() {}
  };

  template <typename E> const E any_mem(E)::exact_obj = E();
  template <typename E> const any_mem(E)& any_mem(E)::ref_exact_obj = any_mem(E)::exact_obj;
  template <typename E> const int any_mem(E)::exact_offset =
                                     (const char*)(void*)(&any_mem(E)::ref_exact_obj)
                                       - (const char*)(void*)(&any_mem(E)::exact_obj);


  // "simple" version of 'any'

  template <typename E>
  struct any <E,
              dispatch_policy::simple>
  {
    E& exact() {
      return *(E*)(void*)this;
    }
    const E& exact() const {
      return *(const E*)(const void*)this;
    }
  protected:
    any() {}
    any(E* exact_ptr_does_not_exist_in_this_version); // safety
  };


} // end of namespace mlc


#endif // ! METALIC_ANY_HH
