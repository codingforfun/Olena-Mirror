// Copyright (C) 2011 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef MLN_CORE_IMAGE_GIMP_IMAGE_HH
# define MLN_CORE_IMAGE_GIMP_IMAGE_HH

/// \file
///
/// GIMP image wrapper.
/// \todo Handle mln::duplicate(gimp_image)


# include <iostream>
# include <sstream>

extern "C"
{
// Qt define this macro which collides with GTK symbols.
//# undef signals
# include <libgimp/gimp.h>
# include <libgimp/gimpui.h>
}

# include <mln/core/concept/proxy.hh>
# include <mln/core/internal/image_primary.hh>

# include <mln/core/alias/box2d.hh>
# include <mln/value/set.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>
# include <mln/literal/all.hh>

# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  // Forward declaration.
  template <GimpImageType TI> struct gimp_image;


  namespace trait
  {
    // pixter

    template <GimpImageType TI>
    struct fwd_pixter< gimp_image<TI> >
    {
      typedef fwd_pixter2d< gimp_image<TI> > ret;
    };


    template <GimpImageType TI>
    struct fwd_pixter< const gimp_image<TI> >
    {
      typedef fwd_pixter2d< const gimp_image<TI> > ret;
    };


    template <GimpImageType TI>
    struct bkd_pixter< gimp_image<TI> >
    {
      typedef bkd_pixter2d< gimp_image<TI> > ret;
    };


    template <GimpImageType TI>
    struct bkd_pixter< const gimp_image<TI> >
    {
      typedef bkd_pixter2d< const gimp_image<TI> > ret;
    };

    // qixter

    template <typename W, GimpImageType TI>
    struct fwd_qixter< gimp_image<TI>, W >
    {
      typedef dpoints_fwd_pixter< gimp_image<TI> > ret;
    };

    template <typename W, GimpImageType TI>
    struct fwd_qixter< const gimp_image<TI>, W >
    {
      typedef dpoints_fwd_pixter< const gimp_image<TI> > ret;
    };

    template <typename W, GimpImageType TI>
    struct bkd_qixter< gimp_image<TI>, W >
    {
      typedef mln::internal::fixme ret;
    };

    // nixter

    template <typename N, GimpImageType TI>
    struct fwd_nixter< gimp_image<TI>, N >
    {
      typedef dpoints_fwd_pixter< gimp_image<TI> > ret;
    };

    template <typename N, GimpImageType TI>
    struct fwd_nixter< const gimp_image<TI>, N >
    {
      typedef dpoints_fwd_pixter< const gimp_image<TI> > ret;
    };

    template <typename N, GimpImageType TI>
    struct bkd_nixter< gimp_image<TI>, N >
    {
      typedef mln::internal::fixme ret;
    };

  } // end of namespace mln::trait


  namespace internal
  {

    /// Trait that give mln type from gimp value tag.
    template <GimpImageType TI>
    struct gimp_value_tag_to_mln
    {
      typedef void type;
    };

    template <>
    struct gimp_value_tag_to_mln<GIMP_RGB_IMAGE>
    {
	typedef value::rgb8 type;
    };

    template <>
    struct gimp_value_tag_to_mln<GIMP_GRAY_IMAGE>
    {
      typedef value::int_u8 type;
    };


    template <GimpImageType TI>
    struct data< gimp_image<TI> >
    {
      typedef typename internal::gimp_value_tag_to_mln<TI>::type T;

      /// Constructor.
      /// {
      data(GimpDrawable* drawable);
      /// }

      /// Destructor.
      ~data();

      /// Gimp region.
      GimpDrawable* drawable_;

      GimpPixelRgn rgn_;

      T *rdata_;
      T** array_;

      /// theoretical box
      box2d b_;
    };

  } // end of namespace mln::internal

  namespace trait
  {

    template <GimpImageType TI>
    struct image_< gimp_image<TI> > : default_image_< mln::value::rgb8, gimp_image<TI> >
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

      // extended domain
      typedef trait::image::ext_domain::none ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_only     ext_io;
    };

  } // end of namespace mln::trait



  /*! \brief GIMP 2D image class.

    \tparam GimpImageType Can take the following values :
    GIMP_GRAY_IMAGE and GIMP_RGB_IMAGE.

   */
  template <GimpImageType TI>
  struct gimp_image
    : public internal::image_primary<typename internal::gimp_value_tag_to_mln<TI>::type,
				     box2d, gimp_image<TI> >
  {
    typedef internal::image_primary<typename internal::gimp_value_tag_to_mln<TI>::type,
				     box2d, gimp_image<TI> > super_;

    // Warning: just to make effective types appear in Doxygen:
    typedef box2d   pset;
    typedef point2d psite;
    typedef point2d point;
    typedef dpoint2d dpoint;
    typedef mln_fwd_piter(box2d) fwd_piter;
    typedef mln_bkd_piter(box2d) bkd_piter;
    // End of warning.

    typedef typename internal::gimp_value_tag_to_mln<TI>::type T;

    /// Value associated type.
    typedef T value;

    /// Return type of read-only access.
    //typedef my_shell<T,guchar> rvalue;
    typedef const T& rvalue;

    /// Return type of read-write access.
    //typedef my_shell<T,guchar> lvalue;
    typedef T& lvalue;


    /// Skeleton.
    typedef gimp_image<TI> skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<value> vset;


    /// Constructor without argument.
    gimp_image();

    /// Constructor with a gimp drawable.
    gimp_image(GimpDrawable* drawable);

    /// Constructor with a box2d.
    gimp_image(box2d box);

    /// Constructor with with the numbers of rows and columns.
    gimp_image(int nrows, int ncols);

    /// Initialise an image from a gimp drawable.
    void init_(GimpDrawable* drawable);

    /// Test if \p p is valid.
    bool has(const point2d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// const-only access to the image value located at point \p p.
    rvalue operator()(const point& p) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator()(const point& p);


    // As a fastest image:
    // -------------------

    // Give the index of a point.
    using super_::index_of_point;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of elements (points including border ones).
    unsigned nelements() const;

    /// Read-only access to the image value located at index \p i.
    rvalue element(unsigned i) const;

    /// Read-write access to the image value located at index \p i.
    lvalue element(unsigned i);

    /// Give the delta-index corresponding to the delta-point \p dp.
    int delta_index(const dpoint2d& dp) const;

    // /// Give the point corresponding to the index \p i.
    point2d point_at_index(unsigned i) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();


    /// Gimp Image method

    /// Give a hook to the GimpDrawable.
    GimpDrawable* gimp_drawable_() const;

    /// Return GimpDrawable's image ID.
    gint32 gimp_image_id_() const;
  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <GimpImageType TI>
    data< gimp_image<TI> >::data(GimpDrawable* drawable)
      : drawable_(drawable)
    {
      mln_precondition(drawable != NULL);

      gint  x1, y1, x2, y2;
      gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);

      gimp_pixel_rgn_init(&rgn_, drawable, x1, y1, x2 - x1, y2 - y1, TRUE, TRUE);

      rdata_ = new T[(x2 - x1) * (y2 - y1)];
      gimp_pixel_rgn_get_rect(&rgn_, (guchar *)rdata_, x1, y1, (x2 - x1), (y2 - y1));

      b_ = make::box2d(y1, x1, y2 - 1, x2 - 1);
      array_ = new T*[b_.height()];
      T* buf = rdata_;
      for (unsigned i = 0; i < b_.height(); ++i)
      	{
      	  array_[i] = buf;
      	  buf += b_.width();
      	}

    }

    template <GimpImageType TI>
    data< gimp_image<TI> >::~data()
    {
      gint  x1, y1, x2, y2;
      gimp_drawable_mask_bounds (drawable_->drawable_id, &x1, &y1, &x2, &y2);

      gimp_pixel_rgn_set_rect(&rgn_, (guchar *)rdata_,
			      x1, y1,
			      (x2 - x1), (y2 - y1));

      gimp_drawable_flush (drawable_);
      gimp_drawable_merge_shadow(drawable_->drawable_id, true);

      gimp_drawable_update (drawable_->drawable_id, 0, 0,
       			    drawable_->width, drawable_->height);

      gimp_displays_flush();
      gimp_drawable_detach (drawable_);

      delete[] rdata_;
      delete[] array_;
    }

  } // end of namespace mln::internal



  // gimp_image<TI>

  template <GimpImageType TI>
  gimp_image<TI>::gimp_image()
  {
  }

  template <GimpImageType TI>
  gimp_image<TI>::gimp_image(GimpDrawable* drawable)
  {
    init_(drawable);
    gimp_tile_cache_ntiles(3 * (drawable->width / gimp_tile_width()));
  }

  template <GimpImageType TI>
  void
  gimp_image<TI>::init_(GimpDrawable* drawable)
  {
    this->data_ = new internal::data< gimp_image<TI> >(drawable);
  }

  template <GimpImageType TI>
  const box2d&
  gimp_image<TI>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <GimpImageType TI>
  unsigned
  gimp_image<TI>::border() const
  {
    return 0;
  }

  template <GimpImageType TI>
  unsigned
  gimp_image<TI>::nelements() const
  {
     mln_precondition(this->is_valid());
     return this->data_->b_.nsites();
  }

  template <GimpImageType TI>
  inline
  typename gimp_image<TI>::rvalue
  gimp_image<TI>::element(unsigned i) const
  {
    mln_precondition(i < nelements());
    return *(this->data_->rdata_ + this->data_->drawable_->bpp * i);
  }

  template <GimpImageType TI>
  inline
  typename gimp_image<TI>::lvalue
  gimp_image<TI>::element(unsigned i)
  {
    mln_precondition(i < nelements());
    return *(this->data_->rdata_ + this->data_->drawable_->bpp * i);
  }


  template <GimpImageType TI>
  bool
  gimp_image<TI>::has(const point2d& p) const
  {
     mln_precondition(this->is_valid());
     return this->data_->b_.has(p);
  }

  template <GimpImageType TI>
  typename gimp_image<TI>::rvalue
  gimp_image<TI>::operator()(const point& p) const
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
 }

  template <GimpImageType TI>
  typename gimp_image<TI>::lvalue
  gimp_image<TI>::operator()(const point& p)
  {
    mln_precondition(this->has(p));
    return this->data_->array_[p.row()][p.col()];
  }

  template <GimpImageType TI>
  inline
  int
  gimp_image<TI>::delta_index(const dpoint2d& dp) const
  {
    mln_precondition(this->is_valid());
    int o = (dp[0] * this->data_->b_.width() + dp[1]) * this->data_->drawable_->bpp;
    return o;
  }

  template <GimpImageType TI>
  inline
  point2d
  gimp_image<TI>::point_at_index(unsigned i) const
  {
    mln_precondition(i < nelements());
    def::coord
      row = static_cast<def::coord>(i / this->data_->b_.width() + this->data_->b_.min_row()),
      col = static_cast<def::coord>(i % this->data_->b_.width() + this->data_->b_.min_col());
    point2d p = point2d(row, col);
    mln_postcondition(& this->operator()(p) == this->data_->rdata_ + i);
    return p;
  }

  template <GimpImageType TI>
  const typename gimp_image<TI>::T*
  gimp_image<TI>::buffer() const
  {
    return this->data_->rdata_;
  }

  template <GimpImageType TI>
  typename gimp_image<TI>::T*
  gimp_image<TI>::buffer()
  {
    return this->data_->rdata_;
  }

  template <GimpImageType TI>
  GimpDrawable*
  gimp_image<TI>::gimp_drawable_() const
  {
    return this->data_->drawable_;
  }

  template <GimpImageType TI>
  gint32
  gimp_image<TI>::gimp_image_id_() const
  {
    return gimp_drawable_get_image(this->data_->drawable_->drawable_id);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_GIMP_IMAGE_HH
