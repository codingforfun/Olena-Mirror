// Copyright (C) 2001, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_IMPL_IMAGE_ARRAY_HH
# define OLENA_CORE_IMPL_IMAGE_ARRAY_HH

# include <oln/core/internal/fake.hh>
# include <mlc/contract.hh>
# include <oln/core/coord.hh>
# include <oln/core/impl/image_impl.hh>
# include <iostream>

namespace oln {

  template<class T>
  void allocate_data_(T*& buffer, size_t s)
  {
    precondition(s > 0);
    buffer = new T[s];
  }
  
  template<class T>
  void desallocate_data_(T*& buffer, size_t s)
  {
    precondition(buffer != 0);
    delete[] buffer;
    buffer = 0; // security
  }

  namespace impl {

    template<class T, class ExactI, class E>
    class image_array: public image_impl<ExactI, E>
    {
    public:
      typedef typename image_traits<ExactI>::point_type point_type;
      typedef typename image_traits<ExactI>::iter_type iter_type;
      typedef typename image_traits<ExactI>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<ExactI>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<ExactI>::value_type value_type;
      typedef typename image_traits<ExactI>::size_type size_type;
      typedef E exact_type;


      typedef image_impl<ExactI, E > super_type;
      typedef image_array<T, ExactI, E> self_type;

      enum { dim = image_traits<ExactI>::dim };

      image_array(const size_type& s): super_type(s), buffer_(0)
      {
	allocate_data_(buffer_, len(s));
      }

      size_t len(const size_type& s) const 
      {
	return to_exact(*this).len_(s);
      }

      image_array(const self_type&);     // cpy ctor  w/o impl
      void operator=(const self_type&); // assign op w/o impl

      const T* buffer() const
      {
	invariant(buffer_ != 0);
	return buffer_;
      }

      T* buffer()
      {
	invariant(buffer_ != 0);
	return buffer_;
      }

    protected:
      ~image_array()
      {
	desallocate_data_(buffer_, len(size_));
      }
    
      T* buffer_;
        
    private:
      image_array<T, ExactI, E> clone_() const
      {
	// FIXME: implement
      }
     
    };
    
  } // end of namespace impl

} // end of namespace oln

#endif // ! OLENA_CORE_IMPL_IMAGE_ARRAY_HH
