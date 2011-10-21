// Copyright (C) 2007, 2011 EPITA Research and Development Laboratory
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

#ifndef __IMAGEGIMP_HH__
# define __IMAGEGIMP_HH__

# include <libgimp/gimp.h>
# include <libgimp/gimpui.h>

# include <mln/core/concept/proxy.hh>
# include <mln/core/internal/image_primary.hh>

# include <mln/core/alias/box2d.hh>
# include <mln/value/set.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>
# include <mln/literal/all.hh>

# include <iostream>
# include <sstream>

namespace mln
{

  // Fwd decl.
  template <GimpImageType TI> struct gimp_image;

  template <typename T1, typename T2>
  struct my_shell
    : public Proxy< my_shell<T1,T2> >,
      public mln::internal::proxy_impl< T1, my_shell<T1,T2> >
  {
    // Equiv
    typedef T1 equiv;

    T1 subj_()
    {
      return val_;
    }

    my_shell(GimpPixelRgn* rgn, const point2d& p)
      : rgn_(rgn), p_(p)
    {
      gimp_pixel_rgn_get_pixel(rgn_,
			       (guchar *) &val_,
			       p_.col(), p_.row());
    }

    my_shell<T1, T2>& operator=(const T1& nval)
    {
      val_ = nval;
      gimp_pixel_rgn_set_pixel(rgn_,
			       (guchar *) &val_,
			       p_.col(), p_.row());
    }


    T1 val_;
    GimpPixelRgn* rgn_;
    point2d p_;

  };

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
      data(GimpPixelRgn* rgn);
      /// }

      /// Destructor.
      ~data();

      /// Gimp region.
      GimpPixelRgn* rgn_;

      guchar *rdata_;

      // Columns pointers
      //guchar** array_;

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
      typedef trait::image::speed::fast       speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_access::computed           value_access;
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



  /*! \brief Basic 2D image class.
   *
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
    typedef my_shell<T,guchar> rvalue;

    /// Return type of read-write access.
    typedef my_shell<T,guchar> lvalue;


    /// Skeleton.
    typedef gimp_image<TI> skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<value> vset;


    /// Constructor without argument.
    gimp_image();

    /// Constructor with a gimp region.
    gimp_image(GimpPixelRgn* rgn);

    /// Constructor with a box2d.
    gimp_image(box2d box);

    /// Constructor with with the numbers of rows and columns.
    gimp_image(int nrows, int ncols);

    /// Initialise an image from a gimp region.
    void init_(GimpPixelRgn* rgn);

    // /// Initialise an image from a box2d.
    // void init_(box2d box);

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

    // // Give the index of a point.
    // unsigned index_of_point(const point& p) const;


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
    // point2d point_at_index(unsigned i) const;

    /// Give a hook to the value buffer.
    const guchar* buffer() const;

    /// Give a hook to the value buffer.
    guchar* buffer();


    /// Gimp Image method

    /// Give a hook to the GimpPixelRgn.
    GimpPixelRgn* gimp_region();

    /// Give a hook to the GimpDrawable.
    GimpDrawable* gimp_drawable();
  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <GimpImageType TI>
    data< gimp_image<TI> >::data(GimpPixelRgn* rgn)
      : rgn_(rgn)
    {
      mln_assertion(rgn != NULL);

      rgn_ = rgn;

      rdata_ = rgn->data;
      mln_assertion(rdata_ != 0);

      // unsigned
      // 	nr = b_.height(),
      // 	nc = b_.width();
      // array_ = new guchar*[nc];
      // guchar* buf = rgn_->data;
      // for (unsigned i = 0; i < nc; ++i)
      // {
      // 	array_[i] = buf;
      // 	buf += nr * sizeof(T);
      // }

      b_ = make::box2d(rgn->y, rgn->x,
		       rgn->y + rgn->h - 1,
		       rgn->x + rgn->w - 1);
    }

    template <GimpImageType TI>
    data< gimp_image<TI> >::~data()
    {
      // FIXME : Flush!
    }

  } // end of namespace mln::internal

  // gimp_image<TI>


  template <GimpImageType TI>
  gimp_image<TI>::gimp_image()
  {
  }

  template <GimpImageType TI>
  gimp_image<TI>::gimp_image(GimpPixelRgn* rgn)
  {
    init_(rgn);
    gimp_tile_cache_ntiles(3 * (this->data_->rgn_->w / gimp_tile_width()));
  }

  template <GimpImageType TI>
  void
  gimp_image<TI>::init_(GimpPixelRgn* rgn)
  {
    this->data_ = new internal::data< gimp_image<TI> >(rgn);
  }

  template <GimpImageType TI>
  const box2d&
  gimp_image<TI>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  // template <GimpImageType TI>
  // unsigned
  // gimp_image<TI>::index_of_point(const point& p) const
  // {
  //   mln_precondition(this->is_valid());
  //   mln_precondition(this->has(p));

  //   unsigned i = (this->operator()(p).address_() - this->buffer()) / sizeof(T);
  //   mln_postcondition(p == this->point_at_index(i));
  //   return i;
  // }


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

  // template <GimpImageType TI>
  // inline
  // typename gimp_image<TI>::rvalue
  // gimp_image<TI>::element(unsigned i) const
  // {
  //   mln_precondition(i < nelements());
  //   return rvalue(this->data_->rdata_ + 3 * i);
  // }

  // template <GimpImageType TI>
  // inline
  // typename gimp_image<TI>::lvalue
  // gimp_image<TI>::element(unsigned i)
  // {
  //   mln_precondition(i < nelements());
  //   return lvalue(this->data_->rdata_ + 3 * i);
  // }


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
    // return rvalue(&this->data_->array_[p.col()][p.row() * 3]);
    // return rvalue(this->data_->rdata_ + 3 * (p.row() * this->data_->b_.width() + p.col()));
    // static T c;
    // gimp_pixel_rgn_get_pixel(this->data_->rgn_,
    // 			     (guchar *) &c,
    // 			     p.col(), p.row());
    // return c;
    return rvalue(this->data_->rgn_, p);
 }

  template <GimpImageType TI>
  typename gimp_image<TI>::lvalue
  gimp_image<TI>::operator()(const point& p)
  {
    mln_precondition(this->has(p));
    return rvalue(this->data_->rgn_, p);
//    return lvalue(&this->data_->array_[p.col()][p.row() * 3]);
    //return rvalue(this->data_->rdata_ + 3 * (p.row() * this->data_->b_.width() + p.col()));
  }

  // template <GimpImageType TI>
  // inline
  // int
  // gimp_image<TI>::delta_index(const dpoint2d& dp) const
  // {
  //   mln_precondition(this->is_valid());
  //   int o = dp[0] * this->data_->b_.width() * 3 + dp[1] * 3;
  //   return o;
  // }

  // template <GimpImageType TI>
  // inline
  // point2d
  // gimp_image<TI>::point_at_index(unsigned i) const
  // {
  //   mln_precondition(i < nelements());
  //   def::coord
  //     row = static_cast<def::coord>(i / this->data_->b_.height() + this->data_->b_.min_col()),
  //     col = static_cast<def::coord>(i % this->data_->b_.height() + this->data_->b_.min_row());
  //   point2d p = point2d(row, col);
  //   mln_postcondition(this->operator()(p).address_() == this->data_->rdata_ + sizeof(T) * i);
  //   return p;
  // }

  // template <GimpImageType TI>
  // const guchar*
  // gimp_image<TI>::buffer() const
  // {
  //   return this->data_->rdata_;
  // }


  // template <GimpImageType TI>
  // guchar*
  // gimp_image<TI>::buffer()
  // {
  //   return this->data_->rdata_;
  // }


  template <GimpImageType TI>
  GimpPixelRgn*
  gimp_image<TI>::gimp_region()
  {
    return this->data_->rgn_;
  }

  template <GimpImageType TI>
  GimpDrawable*
  gimp_image<TI>::gimp_drawable()
  {
    return this->data_->rgn_->drawable;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
# include <mln/core/w_window.hh>


namespace mln
{

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

} // end of namespace mln


#endif // ! __GIMP_IMAGE_HH__
