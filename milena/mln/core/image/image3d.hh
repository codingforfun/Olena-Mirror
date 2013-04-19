// Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012, 2013 EPITA
// Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMAGE3D_HH
# define MLN_CORE_IMAGE_IMAGE3D_HH

/// \file
///
/// Definition of the basic mln::image3d class.

# include <mln/core/internal/fixme.hh>
# include <mln/core/internal/image_primary.hh>
# include <mln/core/alias/box3d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>

// # include <mln/core/line_piter.hh>

// FIXME:

// # include <mln/core/pixter3d.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Forward declaration.
  template <typename V> struct image3d;



  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::image3d<V>.
    */
    template <typename V>
    struct data< image3d<V> >
    {
      data(const box3d& b, unsigned bdr);
      ~data();

      V*  buffer_;
      V*** array_;

      box3d b_;  // theoretical box
      unsigned bdr_;
      box3d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
      void swap_ (data< image3d<V> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename V>
    struct image_< image3d<V> > : default_image_< V, image3d<V> >
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
      typedef trait::image::dimension::three_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_write     ext_io;
    };

  } // end of namespace mln::trait



  /// Basic 3D image class.
  ///
  /// The parameter \c T is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness around data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename V>
  struct image3d : public internal::image_primary< V, box3d, image3d<V> >
  {
    // Warning: just to make effective types appear in Doxygen:
    typedef box3d   pset;
    typedef point3d psite;
    typedef point3d point;
    typedef dpoint3d dpoint;
    typedef mln_fwd_piter(box3d) fwd_piter;
    typedef mln_bkd_piter(box3d) bkd_piter;
//     typedef line_piter_<point> line_piter;
    // End of warning.


    /// Super type
    typedef internal::image_primary< V, box3d, image3d<V> > super_;

    /// Value associated type.
    typedef V         value;

    /// Return type of read-only access.
    typedef const V& rvalue;

    /// Return type of read-write access.
    typedef V&       lvalue;


    /// Skeleton.
    typedef image3d< tag::value_<V> > skeleton;

    /// Constructor without argument.
    image3d();

    /// Constructor with a box and the border thickness (default is
    /// 3).
    image3d(const box3d& b, unsigned bdr = border::thickness);

    /// Constructor with the numbers of offsets and the
    /// border thickness.
    image3d(int nslis, int nrows, int ncols, unsigned bdr = border::thickness);


    /// \cond INTERNAL_API
    /// Initialize an empty image.
    void init_(const box3d& b, unsigned bdr = border::thickness);
    /// \endcond


    /// Test if \p p is valid.
    bool has(const point3d& p) const;

    /// Give the definition domain.
    const box3d& domain() const;

    /// Give the bounding box domain.
    const box3d& bbox() const;

    /// virtual box, i.e., box including the virtual border
    const box3d& vbbox() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    unsigned nelements() const;

    /// Read-only access to the image value located at point \p p.
    const V& operator()(const point3d& p) const;

    /// Read-write access to the image value located at point \p p.
    V& operator()(const point3d& p);

    /// Read-only access to the image value located at offset \p i.
    const V& element(unsigned i) const;

    /// Read-write access to the image value located at offset \p i.
    V& element(unsigned i);

    /// \cond INTERNAL_API

    /// Read-only access to the image value located at (\p sli, \p
    /// row, \p col).
    const V& at_(def::coord sli, def::coord row, def::coord col) const;

    /// Read-write access to the image value located at (\p sli, \p
    /// row, \p col).
    V& at_(def::coord sli, def::coord row, def::coord col);

    /// \endcond

    /// Give the number of slices.
    unsigned nslis() const;

    /// Give the number of rows.
    unsigned nrows() const;

    /// Give the number of columns.
    unsigned ncols() const;


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int delta_offset(const dpoint3d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point3d point_at_offset(unsigned o) const;

    /// Give a hook to the value buffer.
    const V* buffer() const;

    /// Give a hook to the value buffer.
    V* buffer();

    /// \cond INTERNAL_API

    /// To use the track pointer inherited.
    using super_::data_;


    /// Resize image border with new_border.
    void resize_(unsigned new_border);

    /// \endcond

  };

  template <typename V, typename J>
  void init_(tag::image_t, mln::image3d<V>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename V>
  inline
  void init_(tag::border_t, unsigned& b, const image3d<V>& model)
  {
    b = model.border();
  }

  template <typename V, typename J>
  inline
  void init_(tag::image_t, image3d<V>& target, const J& model)
  {
    box3d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }


  // internal::data< image3d<V> >

  namespace internal
  {

    template <typename V>
    inline
    data< image3d<V> >::data(const box3d& b, unsigned bdr)
      : buffer_(0),
	array_ (0),
	b_     (b),
	bdr_   (bdr)
    {
      allocate_();
    }

    template <typename V>
    inline
    data< image3d<V> >::~data()
    {
      deallocate_();
    }

    template <typename V>
    inline
    void
    data< image3d<V> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint3d(all_to(bdr_));
      vb_.pmax() = b_.pmax() + dpoint3d(all_to(bdr_));
    }

    template <typename V>
    inline
    void
    data< image3d<V> >::allocate_()
    {
      update_vb_();
      unsigned
	ns = vb_.len(0),
	nr = vb_.len(1),
	nc = vb_.len(2);
      buffer_ = new V[nr * nc * ns];
      array_ = new V**[ns];
      V* buf = buffer_ - vb_.pmin().col();
      for (unsigned i = 0; i < ns; ++i)
      {
	V** tmp = new V*[nr];
	array_[i] = tmp;
	for (unsigned j = 0; j < nr; ++j)
	{
	  array_[i][j] = buf;
	  buf += nc;
	}
	array_[i] -= vb_.pmin().row();
      }
      array_ -= vb_.pmin().sli();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
    }

    template <typename V>
    inline
    void
    data< image3d<V> >::deallocate_()
    {
      if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
      for (typename point3d::coord i = vb_.pmin().sli(); i <= vb_.pmax().sli(); ++i)
      {
	if (array_[i])
	{
	  array_[i] += vb_.pmin().row();
	  delete[] array_[i];
	  array_[i] = 0;
	}
      }
      if (array_)
      {
	array_ += vb_.pmin().sli();
	delete[] array_;
	array_ = 0;
      }
    }

    template <typename V>
    inline
    void
    data< image3d<V> >::swap_(data< image3d<V> >& other_)
    {
      data< image3d<V> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

    template <typename V>
    inline
    void
    data< image3d<V> >::reallocate_(unsigned new_border)
    {
      data< image3d<V> >& tmp = *(new data< image3d<V> >(this->b_, new_border));
      this->swap_(tmp);
    }


  } // end of namespace mln::internal

  // image3d<V>

  template <typename V>
  inline
  image3d<V>::image3d()
  {
  }

  template <typename V>
  inline
  image3d<V>::image3d(const box3d& b, unsigned bdr)
  {
    init_(b, bdr);
  }

  template <typename V>
  inline
  image3d<V>::image3d(int nslis, int nrows, int ncols, unsigned bdr)
  {
    init_(make::box3d(nslis, nrows, ncols), bdr);
  }

  template <typename V>
  inline
  void
  image3d<V>::init_(const box3d& b, unsigned bdr)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< image3d<V> >(b, bdr);
  }

  template <typename V>
  inline
  const box3d&
  image3d<V>::domain() const
  {
    mln_precondition(this->is_valid());
    return data_->b_;
  }

  template <typename V>
  inline
  const box3d&
  image3d<V>::bbox() const
  {
    mln_precondition(this->is_valid());
    return data_->b_;
  }

  template <typename V>
  inline
  const box3d&
  image3d<V>::vbbox() const
  {
    mln_precondition(this->is_valid());
    return data_->vb_;
  }

  template <typename V>
  inline
  unsigned
  image3d<V>::border() const
  {
    mln_precondition(this->is_valid());
    return data_->bdr_;
  }

  template <typename V>
  inline
  unsigned
  image3d<V>::nelements() const
  {
    mln_precondition(this->is_valid());
    return data_->vb_.nsites();
  }

  template <typename V>
  inline
  bool
  image3d<V>::has(const point3d& p) const
  {
    mln_precondition(this->is_valid());
    return data_->vb_.has(p);
  }

  template <typename V>
  inline
  const V&
  image3d<V>::operator()(const point3d& p) const
  {
    mln_precondition(this->has(p));
    return data_->array_[p.sli()][p.row()][p.col()];
  }

  template <typename V>
  inline
  V&
  image3d<V>::operator()(const point3d& p)
  {
    mln_precondition(this->has(p));
    return data_->array_[p.sli()][p.row()][p.col()];
  }

  template <typename V>
  inline
  const V&
  image3d<V>::element(unsigned i) const
  {
    mln_precondition(i < nelements());
    return *(data_->buffer_ + i);
  }

  template <typename V>
  inline
  V&
  image3d<V>::element(unsigned i)
  {
    mln_precondition(i < nelements());
    return *(data_->buffer_ + i);
  }

  template <typename V>
  inline
  const V&
  image3d<V>::at_(def::coord sli, def::coord row, def::coord col) const
  {
    mln_precondition(this->has(point3d(sli, row, col)));
    return data_->array_[sli][row][col];
  }

  template <typename V>
  inline
  V&
  image3d<V>::at_(def::coord sli, def::coord row, def::coord col)
  {
    mln_precondition(this->has(point3d(sli, row, col)));
    return data_->array_[sli][row][col];
  }

  template <typename V>
  inline
  unsigned
  image3d<V>::nslis() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename V>
  inline
  unsigned
  image3d<V>::nrows() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(1);
  }

  template <typename V>
  inline
  unsigned
  image3d<V>::ncols() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(2);
  }

  template <typename V>
  inline
  const V*
  image3d<V>::buffer() const
  {
    mln_precondition(this->is_valid());
    return data_->buffer_;
  }

  template <typename V>
  inline
  V*
  image3d<V>::buffer()
  {
    mln_precondition(this->is_valid());
    return data_->buffer_;
  }

  template <typename V>
  inline
  int
  image3d<V>::delta_offset(const dpoint3d& dp) const
  {
    mln_precondition(this->is_valid());
    int o = (dp[0] * this->data_->vb_.len(1)
	     + dp[1]) * this->data_->vb_.len(2) + dp[2];
    return o;
  }

  template <typename V>
  inline
  point3d
  image3d<V>::point_at_offset(unsigned o) const
  {
    mln_precondition(o < nelements());
    def::coord
      sli = static_cast<def::coord>(o / (data_->vb_.len(1) * data_->vb_.len(2)) + data_->vb_.min_sli()),
      row = static_cast<def::coord>((o % (data_->vb_.len(1) * data_->vb_.len(2))) / data_->vb_.len(2) + data_->vb_.min_row()),
      col = static_cast<def::coord>(o % data_->vb_.len(2) + data_->vb_.min_col());
    point3d p = point3d(sli, row, col);
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
    return p;
  }

  template <typename V>
  inline
  void
  image3d<V>::resize_(unsigned new_border)
  {
    this->data_->reallocate_(new_border);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter3d.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename V>
    struct fwd_pixter< image3d<V> >
    {
      typedef fwd_pixter3d< image3d<V> > ret;
    };

    template <typename V>
    struct fwd_pixter< const image3d<V> >
    {
      typedef fwd_pixter3d< const image3d<V> > ret;
    };

    template <typename V>
    struct bkd_pixter< image3d<V> >
    {
      typedef bkd_pixter3d< image3d<V> > ret;
    };

    template <typename V>
    struct bkd_pixter< const image3d<V> >
    {
      typedef bkd_pixter3d< const image3d<V> > ret;
    };

    // qixter

    template <typename V, typename W>
    struct fwd_qixter< image3d<V>, W >
    {
      typedef dpoints_fwd_pixter< image3d<V> > ret;
    };

    template <typename V, typename W>
    struct fwd_qixter< const image3d<V>, W >
    {
      typedef dpoints_fwd_pixter< const image3d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_qixter< image3d<V>, W >
    {
      typedef dpoints_bkd_pixter< image3d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_qixter< const image3d<V>, W >
    {
      typedef dpoints_bkd_pixter< const image3d<V> > ret;
    };

    // nixter

    template <typename V, typename W>
    struct fwd_nixter< image3d<V>, W >
    {
      typedef dpoints_fwd_pixter< image3d<V> > ret;
    };

    template <typename V, typename W>
    struct fwd_nixter< const image3d<V>, W >
    {
      typedef dpoints_fwd_pixter< const image3d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_nixter< image3d<V>, W >
    {
      typedef dpoints_bkd_pixter< image3d<V> > ret;
    };

    template <typename V, typename W>
    struct bkd_nixter< const image3d<V>, W >
    {
      typedef dpoints_bkd_pixter< const image3d<V> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMAGE3D_HH
