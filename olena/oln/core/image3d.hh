// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_IMAGE3D_HH
# define OLENA_CORE_IMAGE3D_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/dim3d.hh>
# include <oln/core/macros.hh>
# include <oln/core/image3d_size.hh>
# include <oln/core/constants.hh>
# include <oln/core/point3d.hh>

namespace oln {


  // fwd decls

  template <typename T> struct image3d;

  namespace io {
    struct data;
    template <typename T> void load(image3d<T>& target, const io::data& data);
  }


  // FIXME: for backward compatibility
  namespace border {
//     namespace impl {
//       template <typename T>
//       void set_width(image3d<T>& target, coord_t width);
//     }
//     template <typename I>
//     void set_width(const abstract::image<I>& target, coord_t width);
//     template <typename I>
//     void assign(const abstract::image<I>& target, oln_value_type(I) value);
  }


  // image3d

  template <typename T>
  struct props < image3d<T> > : public default_props <dim3d>
  {
    typedef T value_type;
    enum { linbuf_value = true };
  };

  template <typename T>
  struct image3d : public abstract::image_entry< image3d<T> >
  {
    typedef abstract::image_entry< image3d<T> > super;

    image3d() :
      super(this),
      size_(0, 0),
      border_(0),
      buffer_(0),
      array_(0)
    {
      this->exact_ptr = this;
    }

    image3d(const io::data& data) :
      super(this),
      size_(0, 0, 0),
      border_(0),
      buffer_(0),
      array_(0)
    {
      this->exact_ptr = this;
      io::load(*this, this->data.filename);
    }

    image3d(coord nslices_, coord nrows_, coord ncols_,
	    coord border_ = constant::default_border) :
      super(this),
      size_(nslices_, nrows_, ncols_),
      border_(border_),
      buffer_(0),
      array_(0)
    {
      assert(nslices > 0 &&
	     nrows_ > 0 &&
	     ncols_ > 0 &&
	     border_ >= 0);
      this->exact_ptr = this;
      allocate();
    }

    image3d(const image3d_size& size_,
	    coord border_ = constant::default_border) :
      super(this),
      size_(size_),
      border_(border_),
      buffer_(0),
      array_(0)
    {
      assert(border_ >= 0);
      this->exact_ptr = this;
      allocate();
    }


    // FIXME: hum...
    image3d(const image3d& rhs) :
      super(this),
      size_(rhs.size_),
      border_(rhs.border_),
      buffer_(rhs.buffer_),
      array_(rhs.array_)
    {
      this->exact_ptr = this;
    }

    // FIXME: hum...
    void operator=(const image3d& rhs)
    {
      this->size_ = rhs.size_;
      this->border_ = rhs.border_;
      this->buffer_ = rhs.buffer_;
      this->array_ = rhs.array_;
    }

    // FIXME: for backward compatibility
    void border_adapt_assign(coord width, T value) const
    {
//       border::set_width(*this, width);
//       border::assign(*this, value);
    }

    void resize(coord nrows_, coord ncols_,
		coord border_ = constant::default_border)
    {
      size_.nrows() = nrows_;
      size_.ncols() = ncols_;

      //FIXME: bug with g++-2.95 when abstract::image inherits from any__best_memory
//       std::cout << "size_.nrows()=" << size_.nrows() << std::endl;
//       std::cout << "size_impl().nrows()=" << this->size_impl().nrows() << std::endl;
//       std::cout << "size().nrows()=" << this->size().nrows() << std::endl;

      this->border_ = border_;
      allocate();
    }

    const image3d_size& size_impl() const {
      return size_;
    }

    const T op_square_brackets_impl(const point3d& p) const {
      assert(this->hold_large(p));
      assert(array_ != 0);
      return array_[p.slice()][p.row()][p.col()]; // FIXME: WHAT's that ?
      //Should be dependent of smth like impl_image !!!!
    }

    T& op_square_brackets_impl(const point3d& p) {
      assert(this->hold_large(p));
      assert(array_ != 0);
      return array_[p.slice()][p.row()][p.col()];
    }

    bool hold_impl(const point3d& p) const {
      return
	p.slice() >= 0 && p.slice() < size_.nslices()
	&& p.row() >= 0 && p.row() < size_.nrows()
	&& p.col() >= 0 && p.col() < size_.ncols();
    }

    bool hold_large_impl(const point3d& p) const {
      return
	p.slice() >= - border_ && p.slice() < size_.nrows() + border_
	&& p.row() >= - border_ && p.row() < size_.nrows() + border_
	&& p.col() >= - border_ && p.col() < size_.ncols() + border_;
    }

    size_t npoints_impl() const {
      return size_t(size_.nslices()) * size_t(size_.nrows()) * size_t(size_.ncols());
    }

    size_t buffer_size_impl() const {
      return size_t(size_.nslices() + 2 * border_)
	* size_t(size_.nrows() + 2 * border_)
	* size_t(size_.ncols() + 2 * border_);
    }

    // not _impl methods

    coord border() const {
      return border_;
    }

    const image3d<T> clone_impl() const
    {
      assert(array_ != 0);
      image3d<T> output(this->nslices(), this->nrows(), this->ncols(), border());
      memcpy(output.buffer(), this->buffer(),
	     this->buffer_size() * sizeof(T));
      return output; // FIXME: Should be dependant of impl_image...
    }


    // hooks (handle carefully!)

    T* buffer_impl() { return buffer_; }
    const T* buffer_impl() const { return buffer_; }

    T** array() { return array_; }
    T** const array() const { return array_; }

  protected:

    image3d_size size_;
    coord border_;
    T* buffer_;
    T** array_;

  private:

    void allocate(T*& buffer__, T**& array__,
		  coord nrows__, coord ncols__,
		  coord border__) {
      coord nrows_eff = nrows__ + 2 * border__;
      coord ncols_eff = ncols__ + 2 * border__;
      array__ = new T*[nrows_eff];
      buffer__ = new T[size_t(nrows_eff) * size_t(ncols_eff)];
      T* buf = buffer__ + border__;
      for (coord row = 0; row < nrows_eff; ++row)
	{
	  array__[row] = buf;
	  buf += ncols_eff;
	}
      array__ += border__;
    }

    void allocate() {
      allocate(this->buffer_, this->array_,
	       this->size_.nrows(), this->size_.ncols(),
	       this->border_);
//       coord_t nrows_eff = size_.nrows() + 2 * border_;
//       coord_t ncols_eff = size_.ncols() + 2 * border_;
//       array_ = new T*[nrows_eff];
//       buffer_ = new T[size_t(nrows_eff) * size_t(ncols_eff)];
//       T* buf = buffer_ + border_;
//       for (coord_t row = 0; row < nrows_eff; ++row)
// 	{
// 	  array_[row] = buf;
// 	  buf += ncols_eff;
// 	}
//       array_ += border_;
    }

    void desallocate() {
      if (buffer_ == 0 || array_ == 0)
	std::cerr << "pb" << std::endl;
      // FIXME:
      assert(buffer_ != 0 && array_ != 0);
      delete[] buffer_;
      buffer_ = 0; // safety
      array_ -= border_;
      delete[] array_;
      array_ = 0;  // safety
    }

//     template <typename U>
//     friend void border::impl::set_width(image3d<U>& target, coord width);
  };

} // end of namespace oln

// # include <oln/core/point3d.hh>
// # include <oln/core/dpoint3d.hh>
// # include <oln/core/fwd_iter3d.hh>
// # include <oln/core/bkd_iter3d.hh>
// # include <oln/core/impl/image_array3d.hh>
// # include <oln/io/readable.hh>
// # include <oln/core/image.hh>

// # include <iostream>
// # include <stdlib.h>

// namespace oln {

//   template<class T, class Exact = mlc::final>
//   class image3d; // fwd_decl

//   /*! \class image_id<image3d<T, Exact> >
//   **
//   ** Helper class used by image_traits to retrieve
//   ** the typedef associated to an image.
//   */

//   template<class T, class Exact>
//   struct image_id<image3d<T, Exact> >
//   {
//     enum{dim = 3};
//     typedef T value_type;
//     typedef typename mlc::exact_vt<image3d<T, Exact>, Exact>::ret exact_type;
//     typedef impl::image_array3d<T> impl_type;
//     typedef point3d point_type;
//     typedef image3d_size size_type;
//   };

//   /*! \class image_traits<image3d<T, Exact> >
//   **
//   ** Helper class usefull to retrieve all the type
//   ** relative to an image.
//   */

//   template<class T, class Exact>
//   struct image_traits<image3d<T, Exact> >:
//     public image_traits<image<image_id<image3d<T, Exact> >::dim,
// 			      typename image_id<image3d<T, Exact> >::value_type,
// 			      typename image_id<image3d<T, Exact> >::impl_type,
// 			      typename image_id<image3d<T, Exact> >::exact_type> >
//   {};

//   // client can use image3d; instances are real images, that is,
//   // images with data ---conversely to proxy images

//   /*! \class image3d
//   **
//   ** To instantiate an image3d with oln::rgb_8 as value_type,
//   ** one can write:\n
//   ** oln::image3d<ntg::rgb_8> t;
//   */

//   template<class T, class Exact>
//   class image3d:
//     public image<image_id<image3d<T, Exact> >::dim,
// 		 typename image_id<image3d<T, Exact> >::value_type,
// 		 typename image_id<image3d<T, Exact> >::impl_type,
// 		 typename image_id<image3d<T, Exact> >::exact_type>
//   {

//   public:

//     typedef image3d<T, Exact> self_type;
//     typedef typename image_id<image3d<T, Exact> >::value_type value_type;
//     typedef typename image_id<image3d<T, Exact> >::exact_type exact_type;
//     typedef typename image_id<image3d<T, Exact> >::impl_type impl_type;
//     typedef image<image_id<image3d<T, Exact> >::dim,
// 		  value_type,
// 		  impl_type,
// 		  exact_type> super_type;

//     friend class abstract::image<exact_type>;

//     image3d() :
//       super_type()
//     {
//       mlc_init_static_hierarchy(Exact);
//     }

//     /*! \brief Allocate memory to contain
//     ** an image3d with \a ncols column,
//     ** \a nrows rows, and \a nslices slices
//     ** plus a border width equal to 2 by default.
//     */

//     image3d(coord nslices, coord nrows, coord ncols, coord border = 2) :
//       super_type(new impl_type(image3d_size(nslices, nrows, ncols, border)))
//     {
//       mlc_init_static_hierarchy(Exact);
//     }

//     /*! \brief Allocate memory to contain an
//     ** image3d with a size equal to \a size.
//     */

//     image3d(const image3d_size& size) :
//       super_type(new impl_type(size))
//     {
//       mlc_init_static_hierarchy(Exact);
//     }

//     /*! \brief Build a new image3d by performing
//     ** a shallow copy of \a rhs, the points are
//     ** not duplicated, but shared between \a rhs
//     ** and the new image.
//     **
//     ** \see abstract::image::clone()
//     */


//     image3d(self_type& rhs) :
//       super_type(rhs)
//     {
//       mlc_init_static_hierarchy(Exact);
//     }

//     // io
//     /*! \brief Perform a shallow copy from \a r to
//     ** the new image, the points are not duplicated,
//     ** but shared between the two images.
//     **
//     ** \see abstract::image::clone()
//     */

//     image3d(const io::internal::anything& r) : super_type()
//     {
//       mlc_init_static_hierarchy(Exact);
//       r.assign(*this);
//     }

//     /*! \brief Perform a shallow copy from \a rhs to
//     ** the current image, the points are ot duplicated,
//     ** but shared between the two images.
//     **
//     ** \see abstract::image::clone()
//     */

//     image3d&
//     operator=(const io::internal::anything& r)
//     {
//       return r.assign(*this);
//     }

//     /*! \brief Perform a shallow copy from \a r to
//     ** the current image, the points are not duplicated
//     ** but shared between the two images.
//     **
//     ** \see abstract::image::clone()
//     */

//     exact_type&
//     operator=(self_type rhs)
//     {
//       return this->exact().assign(rhs.exact());
//     }

//     static std::string
//     name()
//     {
//       return
// 	std::string("image3d<")
// 	+ ntg_name(T) + ","
// 	+ Exact::name() + ">";
//     }

//     /// Define ret equal to image3d<U>.

//     template<class U>
//     struct mute
//     {
//       typedef image3d<U> ret;
//     };

//     image3d(const self_type& rhs); // w/o impl

//   protected:

//     /// Return a deep copy of the current image.
//     exact_type
//     clone_() const
//     {
//       exact_type output(this->nslices(), this->nrows(),
// 			this->ncols(), this->border());
//       clone_to(output.impl());
//       return output;
//     }

//   };

//   /*! \class dim_traits<3, T, Exact>
//   **
//   ** Define img_type equal to image3d<T, Exact>.
//   */

//   template <class T, class Exact>
//   struct dim_traits<3, T, Exact>
//   {
//     typedef image3d<T, Exact> img_type;
//   };
// } // end of oln

#endif // ! OLENA_CORE_IMAGE3D_HH
