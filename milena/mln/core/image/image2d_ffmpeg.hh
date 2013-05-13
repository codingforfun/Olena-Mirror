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




namespace mln
{

  // Forward declaration.
  template <typename V> class image2d_ffmpeg;


  namespace internal
  {

    /*!
      \brief Data structure for \c mln::image2d_ffmpeg<V>.
    */
    template <typename V>
    struct data< image2d_ffmpeg<V> >
    {
      data(AVFrame *frame);
      ~data();

      AVFrame *frame_;

      V*  buffer_;
      V** array_;

      box2d b_;  // theoretical box

      void deallocate_();

      // FIXME: we would like to get border information from ffmpeg.
      //box2d vb_;
    };

  } // end of namespace mln::internal

  namespace trait
  {

    template <typename V>
    struct image_< image2d_ffmpeg<V> > : default_image_< V, image2d_ffmpeg<V> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::fastest    speed;
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

      // FIXME: wrt to the image format, it may have a border... We
      // should check how to handle that.
      //
      // extended domain
      typedef trait::image::ext_domain::none ext_domain;
      typedef trait::image::ext_value::irrelevant    ext_value;
      typedef trait::image::ext_io::irrelevant     ext_io;
    };



    // ch_value trait

    namespace impl
    {
      template < typename V, typename V2>
      struct ch_value_< image2d_ffmpeg<tag::value_<V> >, V2>
      {
	typedef image2d<V2> ret;
      };

    }

  } // end of namespace mln::trait


  /// Basic 2D image class.
  ///
  /// The parameter \c V is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness around data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename V>
  class image2d_ffmpeg : public internal::image_primary< V, mln::box2d, image2d_ffmpeg<V> >
  {
    typedef internal::image_primary< V, mln::box2d, image2d_ffmpeg<V> > super_;
  public:

    /// Value associated type.
    typedef V         value;

    /// Return type of read-only access.
    typedef const V& rvalue;

    /// Return type of read-write access.
    typedef V&       lvalue;

    /// Skeleton.
    typedef image2d_ffmpeg< tag::value_<V> > skeleton;


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
    const V& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    V& operator()(const point2d& p);

    // Specific methods:
    // -----------------

    /// \cond INTERNAL_API
    /// Read-only access to the image value located at (\p row, \p col).
    const V& at_(mln::def::coord row, mln::def::coord col) const;
    /// Read-write access to the image value located at (\p row, \p col).
    V& at_(mln::def::coord row, mln::def::coord col);
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
    const V& element(unsigned i) const;

    /// Read-write access to the image value located at offset \p i.
    V& element(unsigned i);

    /// Give the delta-offset corresponding to the delta-point \p dp.
    int delta_offset(const dpoint2d& dp) const;

    /// Give the point corresponding to the offset \p i.
    point2d point_at_offset(unsigned i) const;

    /// Give a hook to the value buffer.
    const V* buffer() const;

    /// Give a hook to the value buffer.
    V* buffer();

  };



  // Forward declaration

  /// \cond INTERNAL_API

  template <typename V, typename J>
  void init_(tag::image_t, mln::image2d_ffmpeg<V>& target, const J& model);

  /// \endcond


# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename V, typename J>
  inline
  void init_(tag::image_t, image2d_ffmpeg<V>& target, const J& model)
  {
    std::cerr << "Not implemented" << std::endl;
    abort();
  }


  // internal::data< image2d_ffmpeg<V> >

  namespace internal
  {
    template <typename V>
    inline
    data< image2d_ffmpeg<V> >::data(AVFrame *frame)
      : frame_(frame)
    {
       b_ = make::box2d(frame->height, frame->width); // frame->width ?
      unsigned
	nr = frame->height,
	nc = frame->width;
      (void) nc;
      array_ = new V*[nr];
      buffer_ = static_cast<V*>((void *)frame->data[0]);
      V* buf = static_cast<V*>((void *)frame->data[0]);
      for (unsigned i = 0; i < nr; ++i)
      {
	array_[i] = buf;
	buf += frame->linesize[0] / 3;
      }

      // FIXME: Not supporting padding for the moment!
      int padding = frame->linesize[0] / 3 % frame->width;
      if (padding)
      {
	std::cout << "This frame uses padding for memory alignment... "
		  << "Not supported for the moment!" <<  << std::endl;
	abort();
      }
    }

    template <typename V>
    inline
    data< image2d_ffmpeg<V> >::~data()
    {
      deallocate_();
      // AVFrame must be deleted by the user!
    }

    template <typename V>
    inline
    void
    data< image2d_ffmpeg<V> >::deallocate_()
    {
      if (array_)
	{
	  delete[] array_;
	  array_ = 0;
	}
    }

  } // end of namespace mln::internal


  // image2d_ffmpeg<V>

  template <typename V>
  inline
  image2d_ffmpeg<V>::image2d_ffmpeg()
  {
  }

  template <typename V>
  inline
  image2d_ffmpeg<V>::image2d_ffmpeg(AVFrame *frame)
  {
    this->data_ = new internal::data< image2d_ffmpeg<V> >(frame);
  }

  template <typename V>
  inline
  void
  image2d_ffmpeg<V>::init_(const box2d&)
  {
    abort();
  }

  template <typename V>
  inline
  const box2d&
  image2d_ffmpeg<V>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename V>
  inline
  const box2d&
  image2d_ffmpeg<V>::bbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename V>
  inline
  const box2d&
  image2d_ffmpeg<V>::vbbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename V>
  inline
  bool
  image2d_ffmpeg<V>::has(const point2d& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.has(p);
  }

  template <typename V>
  inline
  const V&
  image2d_ffmpeg<V>::operator()(const point2d& p) const
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <typename V>
  inline
  V&
  image2d_ffmpeg<V>::operator()(const point2d& p)
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
  }


  // Specific methods:

  template <typename V>
  inline
  const V&
  image2d_ffmpeg<V>::at_(mln::def::coord row, mln::def::coord col) const
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename V>
  inline
  V&
  image2d_ffmpeg<V>::at_(mln::def::coord row, mln::def::coord col)
  {
    mln_precondition(this->has(point2d(row, col)));
    return this->data_->array_[row][col];
  }

  template <typename V>
  inline
  unsigned
  image2d_ffmpeg<V>::nrows() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename V>
  inline
  unsigned
  image2d_ffmpeg<V>::ncols() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(1);
  }


  // As a fastest image:

  template <typename V>
  inline
  unsigned
  image2d_ffmpeg<V>::border() const
  {
    mln_precondition(this->is_valid());
    return 0;
  }

  template <typename V>
  inline
  unsigned
  image2d_ffmpeg<V>::nelements() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.nsites();
  }

  template <typename V>
  inline
  const V&
  image2d_ffmpeg<V>::element(unsigned i) const
  {
    mln_precondition(i < nelements());
    return this->data_->buffer_[i];
  }

  template <typename V>
  inline
  V&
  image2d_ffmpeg<V>::element(unsigned i)
  {
    mln_precondition(i < nelements());
    return this->data_->buffer_[i];
  }

  template <typename V>
  inline
  const V*
  image2d_ffmpeg<V>::buffer() const
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename V>
  inline
  V*
  image2d_ffmpeg<V>::buffer()
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename V>
  inline
  int
  image2d_ffmpeg<V>::delta_offset(const dpoint2d& dp) const
  {
    mln_precondition(this->is_valid());
    int o = dp[0] * this->data_->frame_->linesize[0] / 3 + dp[1];
    return o;
  }

  template <typename V>
  inline
  point2d
  image2d_ffmpeg<V>::point_at_offset(unsigned i) const
  {
    mln_precondition(i < nelements());
    def::coord
      row = static_cast<def::coord>(i / this->data_->b_.len(1) + this->data_->b_.min_row()),
      col = static_cast<def::coord>(i % this->data_->b_.len(1) + this->data_->b_.min_col());
    point2d p = point2d(row, col);
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + i);
    return p;
  }



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

    template <typename V>
    struct fwd_pixter< image2d_ffmpeg<V> >
    {
      typedef fwd_pixter2d< image2d_ffmpeg<V> > ret;
    };

    template <typename V>
    struct fwd_pixter< const image2d_ffmpeg<V> >
    {
      typedef fwd_pixter2d< const image2d_ffmpeg<V> > ret;
    };

    template <typename V>
    struct bkd_pixter< image2d_ffmpeg<V> >
    {
      typedef bkd_pixter2d< image2d_ffmpeg<V> > ret;
    };

    template <typename V>
    struct bkd_pixter< const image2d_ffmpeg<V> >
    {
      typedef bkd_pixter2d< const image2d_ffmpeg<V> > ret;
    };

    // qixter

    template <typename V, typename W>
    struct fwd_qixter< image2d_ffmpeg<V>, W >
    {
      typedef dpoints_fwd_pixter< image2d_ffmpeg<V> > ret;
    };

    template <typename V, typename W>
    struct fwd_qixter< const image2d_ffmpeg<V>, W >
    {
      typedef dpoints_fwd_pixter< const image2d_ffmpeg<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_qixter< image2d_ffmpeg<V>, W >
    {
      typedef dpoints_bkd_pixter< image2d_ffmpeg<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_qixter< const image2d_ffmpeg<V>, W >
    {
      typedef dpoints_bkd_pixter< const image2d_ffmpeg<V> > ret;
    };

    // nixter

    template <typename V, typename N>
    struct fwd_nixter< image2d_ffmpeg<V>, N >
    {
      typedef dpoints_fwd_pixter< image2d_ffmpeg<V> > ret;
    };

    template <typename V, typename N>
    struct fwd_nixter< const image2d_ffmpeg<V>, N >
    {
      typedef dpoints_fwd_pixter< const image2d_ffmpeg<V> > ret;
    };

    template <typename V, typename N>
    struct bkd_nixter< image2d_ffmpeg<V>, N >
    {
      typedef dpoints_bkd_pixter< image2d_ffmpeg<V> > ret;
    };

    template <typename V, typename N>
    struct bkd_nixter< const image2d_ffmpeg<V>, N >
    {
      typedef dpoints_bkd_pixter< const image2d_ffmpeg<V> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMAGE2D_FFMPEG_HH
