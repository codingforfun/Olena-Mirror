// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef METALIC_BOX_HH
# define METALIC_BOX_HH


namespace mlc 
{

  template <class T>
  struct box {

    box(T ref) :
      ref(ref)
    {}

    box(const box<T>& rhs) :
      ref(rhs.ref)
    {}

    box<T>& operator=(box<T>& rhs)
    {
      this->ref = rhs.ref;
      return *this;
    }

    // FIXME: oldies, should be removed?

    operator T() { return ref; }

    T& unbox() { return ref; }
    const T& unbox() const { return ref; }

    // FIXME: new code, better?

    const T& operator*() const { return ref; }
          T& operator*()       { return ref; }

    const T *const operator->() const { return &ref; }
          T *const operator->()       { return &ref; }

    friend class box<const T>;

  private:
    mutable T ref;
  };



  template <class T>
  struct box <const T>
  {

    box(const T& ref) :
      ref(const_cast<T&>(ref))
    {}

    box(const box<T>& rhs) :
      ref(const_cast<T&>(rhs.ref))
    {}

    box(const box<const T>& rhs) :
      ref(const_cast<T&>(rhs.ref))
    {}

    box<const T>& operator=(const box<const T>& rhs)
    {
      this->ref = const_cast<T&>(rhs.ref);
      return *this;
    }

    // FIXME: oldies, should be removed?

    operator const T() const { return ref; }
    const T& unbox() const { return ref; }

    // FIXME: new code, better?

    const T& operator*() const { return ref; } // FIXME: 'T&' or 'T'?
    const T *const operator->() const { return &ref; }

  private:
    mutable T ref;
  };



  template <class T> const T& unbox(const box<T>& b) {return b.unbox(); }
  template <class T>       T& unbox(      box<T>& b) {return b.unbox(); }


} // end of namespace mlc


#endif // ! METALIC_BOX_HH
