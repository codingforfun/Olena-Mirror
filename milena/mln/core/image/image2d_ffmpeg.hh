// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_IMAGE_IMAGE2D_FFMPEG_HH
# define MLN_CORE_IMAGE_IMAGE2D_FFMPEG_HH

/// \file
/// Definition of the basic mln::image2d_ffmpeg class.
///

# include <mln/core/internal/image_primary.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/routine/init.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>




namespace mln
{

  // Forward declaration.
  template <typename T> class image2d_ffmpeg;


  namespace internal
  {

    /*!
      \brief Data structure for \c mln::image2d_ffmpeg<T>.
    */
    template <typename T>
    struct data< image2d_ffmpeg<T> >
    {
      data(AVFrame *frame);
      ~data();

      AVFrame *frame;

      T*  buffer_;
      T** array_;

      box2d b_;  // theoretical box
    };

  } // end of namespace mln::internal

  namespace trait
  {

    template <typename T>
    struct image_< image2d_ffmpeg<T> > : default_image_< T, image2d_ffmpeg<T> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::slow    speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_access::direct           value_access;
      typedef trait::image::value_storage::one_block       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_alignment::with_grid     value_alignment;
      typedef trait::image::value_io::read_write           value_io;

      // site / domain
      typedef trait::image::pw_io::read_write        pw_io;
      typedef trait::image::localization::basic_grid localization;
      typedef trait::image::dimension::two_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::none ext_domain;
      typedef trait::image::ext_value::irrelevant    ext_value;
      typedef trait::image::ext_io::irrelevant     ext_io;
    };

  } // end of namespace mln::trait


  namespace internal
  {

    avformat_helper

    template <typename V>
    avformat_helper
    {
      typedef undefined res;
    };

  } // end of namespace mln::internal



  /// Basic 2D image class.
  ///
  /// The parameter \c T is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness around data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename T>
  class image2d_ffmpeg : public internal::image_primary< T, mln::box2d, image2d_ffmpeg<T> >
  {
    typedef internal::image_primary< T, mln::box2d, image2d_ffmpeg<T> > super_;
  public:

    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;

    typedef avformat_helper<T>::res format;


    /// Skeleton.
    typedef image2d_ffmpeg< tag::value_<T> > skeleton;


    /// Constructor without argument.
    image2d_ffmpeg();

    /// Constructor with the numbers of rows and columns and the
    /// border thickness.
    image2d_ffmpeg(AVFrame *frame);


    /// \cond INTERNAL_API
    /// Initialize an empty image.
    void init_(const box2d& b);
    /// \endcond


    /// Test if \p p is valid.
    bool has(const point2d& p) const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the bounding box domain.
    const box2d& bbox() const;

    // virtual box, i.e., box including the virtual border
    const box2d& vbbox() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point2d& p);

    // Specific methods:
    // -----------------

    /// \cond INTERNAL_API
    /// Read-only access to the image value located at (\p row, \p col).
    const T& at_(mln::def::coord row, mln::def::coord col) const;
    /// Read-write access to the image value located at (\p row, \p col).
    T& at_(mln::def::coord row, mln::def::coord col);
    /// \endcond


    /// Give the number of rows.
    unsigned nrows() const;

    /// Give the number of columns.
    unsigned ncols() const;


    // As a fastest image:
    // -------------------

    // Give the offset of a point.
    using super_::offset_of_point;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of elements (points including border ones).
    unsigned nelements() const;

    /// Read-only access to the image value located at offset \p i.
    const T& element(unsigned i) const;

    /// Read-write access to the image value located at offset \p i.
    T& element(unsigned i);

    /// Give the delta-offset corresponding to the delta-point \p dp.
    int delta_offset(const dpoint2d& dp) const;

    /// Give the point corresponding to the offset \p i.
    point2d point_at_offset(unsigned i) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();

  };



  // Forward declaration

  /// \cond INTERNAL_API

  template <typename T, typename J>
  void init_(tag::image_t, mln::image2d_ffmpeg<T>& target, const J& model);

  /// \endcond


# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T, typename J>
  inline
  void init_(tag::image_t, image2d_ffmpeg<T>& target, const J& model)
  {
    box2d b;
    init_(tag::bbox, b, model);
    target.init_(b);
  }


  // internal::data< image2d_ffmpeg<T> >

  namespace internal
  {
    template <typename T>
    inline
    data< image2d_ffmpeg<T> >::data(AVFrame *frame)
      : frame_(frame)
    {
      b_ = make::box2d(frame->height, frame->width);

      unsigned
	nr = frame->height,
	nc = frame->width;
      array_ = new T*[nr];
      buf_ = frame->data[0];
      T* buf = frame->data[0];
      for (unsigned i = 0; i < nr; ++i)
	{
	  array_[i] = buf;
	  buf += frame->linesize;
	}
    }

    template <typename T>
    inline
    data< image2d_ffmpeg<T> >::~data()
    {
      deallocate_();
      // AVFrame must be deleted by the user!
    }

    template <typename T>
    inline
    void
    data< image2d_ffmpeg<T> >::deallocate_()
    {
      if (array_)
	{
	  array_ += vb_.pmin().row();
	  delete[] array_;
	  array_ = 0;
	}
    }

  } // end of namespace mln::internal


  // image2d_ffmpeg<T>

  template <typename T>
  inline
  image2d_ffmpeg<T>::image2d_ffmpeg()
  {
  }

  template <typename T>
  inline
  image2d_ffmpeg<T>::image2d_ffmpeg(AVFrame *frame)
  {
    this->data_ = new internal::data< image2d_ffmpeg<T> >(frame);
  }

  template <typename T>
  inline
  const box2d&
  image2d_ffmpeg<T>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box2d&
  image2d_ffmpeg<T>::bbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box2d&
  image2d_ffmpeg<T>::vbbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  bool
  image2d_ffmpeg<T>::has(const point2d& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.has(p);
  }

  template <typename T>
  inline
  const T&
  image2d_ffmpeg<T>::operator()(const point2d& p) const
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <typename T>
  inline
  T&
  image2d_ffmpeg<T>::operator()(const point2d& p)
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
  }


  // Specific methods:

  template <typename T>
  inline
  const T&
  image2d_ffmpeg<T>::at_(mln::def::coord row, mln::def::coord col) const
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  inline
  T&
  image2d_ffmpeg<T>::at_(mln::def::coord row, mln::def::coord col)
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename T>
  inline
  unsigned
  image2d_ffmpeg<T>::nrows() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename T>
  inline
  unsigned
  image2d_ffmpeg<T>::ncols() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(1);
  }


  // As a fastest image:

  template <typename T>
  inline
  unsigned
  image2d_ffmpeg<T>::border() const
  {
    mln_precondition(this->is_valid());
    return 0;
  }

  template <typename T>
  inline
  unsigned
  image2d_ffmpeg<T>::nelements() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.nsites();
  }

  template <typename T>
  inline
  const T&
  image2d_ffmpeg<T>::element(unsigned i) const
  {
    mln_precondition(i < nelements());
    return *(this->data_->buffer_ + i);
  }

  template <typename T>
  inline
  T&
  image2d_ffmpeg<T>::element(unsigned i)
  {
    mln_precondition(i < nelements());
    return *(this->data_->buffer_ + i);
  }

  template <typename T>
  inline
  const T*
  image2d_ffmpeg<T>::buffer() const
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  T*
  image2d_ffmpeg<T>::buffer()
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  // template <typename T>
  // inline
  // int
  // image2d_ffmpeg<T>::delta_offset(const dpoint2d& dp) const
  // {
  //   mln_precondition(this->is_valid());
  //   int o = dp[0] * this->data_->frame->linesize + dp[1];
  //   return o;
  // }

  // template <typename T>
  // inline
  // point2d
  // image2d_ffmpeg<T>::point_at_offset(unsigned i) const
  // {
  //   mln_precondition(i < nelements());
  //   def::coord
  //     row = static_cast<def::coord>(i / this->data_->vb_.len(1) + this->data_->vb_.min_row()),
  //     col = static_cast<def::coord>(i % this->data_->vb_.len(1) + this->data_->vb_.min_col());
  //   point2d p = point2d(row, col);
  //   mln_postcondition(& this->operator()(p) == this->data_->buffer_ + i);
  //   return p;
  // }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
// # include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image2d_ffmpeg<T> >
    {
      typedef fwd_pixter2d< image2d_ffmpeg<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image2d_ffmpeg<T> >
    {
      typedef fwd_pixter2d< const image2d_ffmpeg<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image2d_ffmpeg<T> >
    {
      typedef bkd_pixter2d< image2d_ffmpeg<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< const image2d_ffmpeg<T> >
    {
      typedef bkd_pixter2d< const image2d_ffmpeg<T> > ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image2d_ffmpeg<T>, W >
    {
      typedef dpoints_fwd_pixter< image2d_ffmpeg<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image2d_ffmpeg<T>, W >
    {
      typedef dpoints_fwd_pixter< const image2d_ffmpeg<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image2d_ffmpeg<T>, W >
    {
      typedef dpoints_bkd_pixter< image2d_ffmpeg<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< const image2d_ffmpeg<T>, W >
    {
      typedef dpoints_bkd_pixter< const image2d_ffmpeg<T> > ret;
    };

    // nixter

    template <typename T, typename N>
    struct fwd_nixter< image2d_ffmpeg<T>, N >
    {
      typedef dpoints_fwd_pixter< image2d_ffmpeg<T> > ret;
    };

    template <typename T, typename N>
    struct fwd_nixter< const image2d_ffmpeg<T>, N >
    {
      typedef dpoints_fwd_pixter< const image2d_ffmpeg<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< image2d_ffmpeg<T>, N >
    {
      typedef dpoints_bkd_pixter< image2d_ffmpeg<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< const image2d_ffmpeg<T>, N >
    {
      typedef dpoints_bkd_pixter< const image2d_ffmpeg<T> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMAGE2D_FFMPEG_HH
