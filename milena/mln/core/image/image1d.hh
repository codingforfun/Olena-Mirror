// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMAGE1D_HH
# define MLN_CORE_IMAGE_IMAGE1D_HH

/// \file
///
/// Definition of the basic mln::image1d class.
///
/// \todo Rewrite from_to(histo, image1d) after Etienne's work.

# include <mln/core/internal/fixme.hh>
# include <mln/core/internal/image_primary.hh>
# include <mln/core/alias/box1d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/v2v/all_to.hh>


// FIXME:

// # include <mln/core/pixter1d.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Forward declaration.
  template <typename V> struct image1d;


  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::image1d<V>.
    */
    template <typename V>
    struct data< image1d<V> >
    {
      data(const box1d& b, unsigned bdr);
      ~data();

      V*  buffer_;
      V* array_;

      box1d b_;  // theoretical box
      unsigned bdr_;
      box1d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
      void swap_ (data< image1d<V> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename V>
    struct image_< image1d<V> > : default_image_< V, image1d<V> >
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
      typedef trait::image::dimension::one_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_write     ext_io;
    };

  } // end of namespace mln::trait



  // Forward declaration.
  template <typename V> struct image1d;


  /// Basic 1D image class.
  ///
  /// The parameter \c V is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness before and after data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename V>
  struct image1d :
    public internal::image_primary< V, box1d, image1d<V> >
  {
    typedef internal::image_primary< V, mln::box1d, image1d<V> > super_;

    /// Value associated type.
    typedef V         value;

    /// Return type of read-only access.
    typedef const V& rvalue;

    /// Return type of read-write access.
    typedef V&       lvalue;

    /// Skeleton.
    typedef image1d< tag::value_<V> > skeleton;


    /// Constructor without argument.
    image1d();

    /// Constructor with the number of indices and the border
    /// thickness.
    image1d(unsigned ninds, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness.
    image1d(const box1d& b, unsigned bdr = border::thickness);


    /// \cond INTERNAL_API
    /// Initialize an empty image.
    void init_(const box1d& b, unsigned bdr = border::thickness);
    /// \endcond


    /// Test if \p p is valid.
    bool has(const point1d& p) const;

    /// Give the definition domain.
    const box1d& domain() const;

    /// Give the bounding box domain.
    const box1d& bbox() const;

    /// virtual box, i.e., box including the virtual border
    const box1d& vbbox() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Read-only access to the image value located at point \p p.
    const V& operator()(const point1d& p) const;

    /// Read-write access to the image value located at point \p p.
    V& operator()(const point1d& p);


    // Specific methods:
    // -----------------

    /// \cond INTERNAL_API

    /// Read-only access to the image value located at (\p offset).
    const V& at_(def::coord offset) const;

    /// Read-write access to the image value located at (\p offset).
    V& at_(def::coord offset);

     /// Give the number of offsets.
    unsigned ninds() const;

    /// \endcond


    /// Fast Image method

    // Give the offset of a point.
    using super_::offset_of_point;

    /// Give the offset corresponding to the delta-point \p dp.
    int delta_offset(const dpoint1d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point1d point_at_offset(unsigned i) const;

    /// Give a hook to the value buffer.
    const V* buffer() const;

    /// Give a hook to the value buffer.
    V* buffer();

    /// Read-only access to the \p i-th image value (including the
    /// border).
    const V& element(unsigned i) const;

    /// Read-write access to the \p i-th image value (including the
    /// border).
    V& element(unsigned i);

    /// Give the number of cells (points including border ones).
    unsigned nelements() const;


    /// \cond INTERNAL_API
    /// Resize image border with new_border.
    void resize_(unsigned new_border);
    /// \endcond

  };

  template <typename V, typename J>
  void init_(tag::image_t, mln::image1d<V>& target, const J& model);

# ifndef MLN_INCLUDE_ONLY

  // init_
  template <typename V>
  inline
  void init_(tag::border_t, unsigned& b, const image1d<V>& model)
  {
    b = model.border();
  }

  template <typename V, typename J>
  inline
  void init_(tag::image_t, image1d<V>& target, const J& model)
  {
    box1d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }

  // internal::data< image1d<V> >

  namespace internal
  {

    template <typename V>
    inline
    data< image1d<V> >::data(const box1d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename V>
    inline
    data< image1d<V> >::~data()
    {
      deallocate_();
    }

    template <typename V>
    inline
    void
    data< image1d<V> >::update_vb_()
    {
      dpoint1d dp(all_to(bdr_));

      vb_.pmin() = b_.pmin() - dp;
      vb_.pmax() = b_.pmax() + dp;
    }

    template <typename V>
    inline
    void
    data< image1d<V> >::allocate_()
    {
      update_vb_();
      unsigned
	ni = vb_.len(0);
      buffer_ = new V[ni];
      array_ = buffer_ - vb_.pmin().ind();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
    }

    template <typename V>
    inline
    void
    data< image1d<V> >::deallocate_()
    {
      if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
    }


    template <typename V>
    inline
    void
    data< image1d<V> >::swap_(data< image1d<V> >& other_)
    {

      data< image1d<V> > self_ = *this;
      *this = other_;
      other_ = self_;

    }

    template <typename V>
    inline
    void
    data< image1d<V> >::reallocate_(unsigned new_border)
    {
      data< image1d<V> >& tmp = *(new data< image1d<V> >(this->b_, new_border));
      this->swap_(tmp);
    }

  } // end of namespace mln::internal


  // image1d<V>

  template <typename V>
  inline
  image1d<V>::image1d()
  {
  }

  template <typename V>
  inline
  image1d<V>::image1d(const box1d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename V>
  inline
  image1d<V>::image1d(unsigned ninds, unsigned bdr)
  {
    mln_precondition(ninds != 0);
    init_(make::box1d(ninds), bdr);
  }

  template <typename V>
  inline
  void
  image1d<V>::init_(const box1d& b, unsigned bdr)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< image1d<V> >(b, bdr);
  }

  template <typename V>
  inline
  const box1d&
  image1d<V>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename V>
  inline
  const box1d&
  image1d<V>::bbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename V>
  inline
  const box1d&
  image1d<V>::vbbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_;
  }

  template <typename V>
  inline
  unsigned
  image1d<V>::border() const
  {
    mln_precondition(this->is_valid());
    return this->data_->bdr_;
  }

  template <typename V>
  inline
  unsigned
  image1d<V>::nelements() const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_.nsites();
  }

  template <typename V>
  inline
  bool
  image1d<V>::has(const point1d& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_.has(p);
  }

  template <typename V>
  inline
  const V&
  image1d<V>::operator()(const point1d& p) const
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.ind()];
  }

  template <typename V>
  inline
  V&
  image1d<V>::operator()(const point1d& p)
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.ind()];
  }

  template <typename V>
  inline
  const V&
  image1d<V>::at_(def::coord offset) const
  {
    mln_precondition(this->has(point1d(offset)));
    return this->data_->array_[offset];
  }

  template <typename V>
  inline
  unsigned
  image1d<V>::ninds() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename V>
  inline
  V&
  image1d<V>::at_(def::coord offset)
  {
    mln_precondition(this->has(point1d(offset)));
    return this->data_->array_[offset];
  }


  template <typename V>
  inline
  const V&
  image1d<V>::element(unsigned i) const
  {
    mln_precondition(i < nelements());
    return this->data_->buffer_[i];
  }

  template <typename V>
  inline
  V&
  image1d<V>::element(unsigned i)
  {
    mln_precondition(i < nelements());
    return this->data_->buffer_[i];
  }

  template <typename V>
  inline
  const V*
  image1d<V>::buffer() const
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename V>
  inline
  V*
  image1d<V>::buffer()
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename V>
  inline
  int
  image1d<V>::delta_offset(const dpoint1d& dp) const
  {
    mln_precondition(this->is_valid());
    int o = dp[0];
    return o;
  }

  template <typename V>
  inline
  point1d
  image1d<V>::point_at_offset(unsigned i) const
  {
    mln_precondition(i < nelements());
    def::coord ind = static_cast<def::coord>(i + this->data_->vb_.min_ind());
    point1d p = point1d(ind);
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + i);
    return p;
  }

  template <typename V>
  inline
  void
  image1d<V>::resize_(unsigned new_border)
  {
    this->data_->reallocate_(new_border);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter1d.hh>
# include <mln/core/w_window.hh>



namespace mln
{

  namespace trait
  {

    // pixter

    template <typename V>
    struct fwd_pixter< image1d<V> >
    {
      typedef fwd_pixter1d< image1d<V> > ret;
    };

    template <typename V>
    struct fwd_pixter< const image1d<V> >
    {
      typedef fwd_pixter1d< const image1d<V> > ret;
    };

    template <typename V>
    struct bkd_pixter< image1d<V> >
    {
      typedef bkd_pixter1d< image1d<V> > ret;
    };

    template <typename V>
    struct bkd_pixter< const image1d<V> >
    {
      typedef bkd_pixter1d< const image1d<V> > ret;
    };

    // qixter

    template <typename V, typename W>
    struct fwd_qixter< image1d<V>, W >
    {
      typedef dpoints_fwd_pixter< image1d<V> > ret;
    };

    template <typename V, typename W>
    struct fwd_qixter< const image1d<V>, W >
    {
      typedef dpoints_fwd_pixter< const image1d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_qixter< image1d<V>, W >
    {
      typedef dpoints_bkd_pixter< image1d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_qixter< const image1d<V>, W >
    {
      typedef dpoints_bkd_pixter< const image1d<V> > ret;
    };

    // nixter

    template <typename V, typename W>
    struct fwd_nixter< image1d<V>, W >
    {
      typedef dpoints_fwd_pixter< image1d<V> > ret;
    };

    template <typename V, typename W>
    struct fwd_nixter< const image1d<V>, W >
    {
      typedef dpoints_fwd_pixter< const image1d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_nixter< image1d<V>, W >
    {
      typedef dpoints_bkd_pixter< image1d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_nixter< const image1d<V>, W >
    {
      typedef dpoints_bkd_pixter< const image1d<V> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln

# include <mln/make/image.hh>

#endif // ! MLN_CORE_IMAGE_IMAGE1D_HH
