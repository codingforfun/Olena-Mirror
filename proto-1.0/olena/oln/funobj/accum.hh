// Copyright (C) 2001, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_FUNOBJ_ACCUM_HH
# define OLENA_CORE_FUNOBJ_ACCUM_HH

# include <mlc/contract.hh>


namespace oln
{

  namespace funobj
  {

    /// Max accumulator.

    template <class T>
    struct max_accumulator
    {
  
      max_accumulator() :
	ok_(false) 
      {}
      
      void operator()(const T& t)
      {
	if (not ok_)
	  {
	    ok_ = true;
	    acc_ = t;
	    return;
	  }
	if (acc_ < t)
	  acc_ = t;
      }
      
      operator T() const
      {
	precondition(ok_);
	return acc_;
      }
      
    private:
      
      bool ok_;
      T acc_;

    };


    // FIXME: ... Max accumulator with initialization value.



    /// Min accumulator.

    template <class T>
    struct min_accumulator
    {
  
      min_accumulator() :
	ok_(false) 
      {}
      
      void operator()(const T& t)
      {
	if (not ok_)
	  {
	    ok_ = true;
	    acc_ = t;
	    return;
	  }
	if (t < acc_)
	  acc_ = t;
      }
      
      operator T() const
      {
	precondition(ok_);
	return acc_;
      }
      
    private:
      
      bool ok_;
      T acc_;

    };


    /// Min accumulator with initialization value.

    template <class T>
    struct min_accumulator_init
    {

      template <typename U>
      min_accumulator_init(const U& init) :
	acc_(init) 
      {}
      
      void operator()(T t)
      {
	if (t < acc_)
	  acc_ = t;
      }
      
      operator T() const
      {
	return acc_;
      }
      
    private:
      
      T acc_;

    };


    /// Max accumulator with initialization value.

    template <class T>
    struct max_accumulator_init
    {

      template <typename U>
      max_accumulator_init(const U& init) :
	acc_(init) 
      {}
      
      void operator()(T t)
      {
	if (acc_ < t)
	  acc_ = t;
      }
      
      operator T() const
      {
	return acc_;
      }
      
    private:
      
      T acc_;

    };


  } // end of namespace oln::funobj

} // end of namespace oln


#endif // ! OLENA_CORE_FUNOBJ_ACCUM_HH
