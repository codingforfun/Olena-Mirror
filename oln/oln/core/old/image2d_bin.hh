// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_IMAGE2D_BIN_HH
# define OLENA_CORE_IMAGE2D_BIN_HH

# include <oln/core/pred_image.hh>
# include <oln/value/bin.hh>


// FIXME: code should be factored in a super-class of image2d!


namespace oln {


  template<class I>
  class image2d<bin,I> : public image< image2d<bin,I> >,
			 public pred_image< image2d<bin,I> >
  {
  public:
    typedef I inferior;
    typedef image2d<bin,I> self;

    enum { dim = 2 };
    typedef point2d point;
    typedef dpoint2d dpoint;
    typedef fwd_iter2d iter;
    typedef fwd_iter2d fwd_iter;
    typedef bkd_iter2d bkd_iter;
    typedef image2d_size image_size;

    typedef bin value;
    template<class U> struct mute { typedef image2d<U> ret; };

    image2d();
    image2d(coord nrows, coord ncols, coord border = 2);
    image2d(const image2d_size& size, coord border = 2);

    image2d<bin,I> clone() const;            // deep copy
    image2d(image2d<bin,I>& rhs);              // shallow copy
    image2d<bin,I>& operator=(image2d<bin,I> rhs); // shallow assignment

    image2d(const image2d<bin,I>& rhs);        // w/o impl

    // io
//     image2d(const io::internal::anything& rhs) : image2d() { operator=(rhs); }
//     image2d<bin,I>& operator=(const io::internal::anything& rhs)
//     { return rhs.assign(*this); }

    ~image2d();
    void clear();

    coord nrows() const;
    coord ncols() const;
    coord border() const;
    image2d_size size() const;
    size_t npoints() const { return size_t(nrows()) * size_t(ncols()); }

    bool hold(const point2d& p) const;
    bool hold(coord row, coord col) const;

    bin operator[](const point2d& p) const;
    bin& operator[](const point2d& p);

    bin operator()(coord row, coord col) const;
    bin& operator()(coord row, coord col);

    const internal::image2d_data<bin>* data() const;
    internal::image2d_data<bin>* data();

    static std::string name() { return std::string("image2d<") + bin::name() + ">"; }

  private:

    internal::image2d_data<bin>* _data;

  };


} // end of oln



#include <oln/core/image2d_bin.hxx>


#endif // ! OLENA_CORE_IMAGE2D_BIN_HH
