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

extern "C"
{
# include <libgimp/gimp.h>
# include <libgimp/gimpui.h>
}

# include <mln/core/internal/image_primary.hh>

# include <mln/core/alias/box2d.hh>
# include <mln/value/set.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/all.hh>

# include <iostream>
# include <sstream>

namespace mln
{

  // Fwd decl.
  template <GimpImageType TI> struct gimp_image;

  namespace internal
  {

    template <GimpImageType TI>
    struct data< gimp_image<TI> >
    {
      /// Constructor.
      /// {
      data(GimpPixelRgn* rgn_);
      /// }

      /// Destructor.
      ~data();

      /// Gimp region.
      GimpPixelRgn* rgn_;

      ///Pointer to pixel buffer.
      mln_value(gimp_image<TI>)* buffer_;

      // FIXME: currently not initialized but may be faster! See if
      // Gimp supports such things.
      ///Pointer to an array of pointer on images lines.
      mln_value(gimp_image<TI>)** array_;

      /// theoretical box
      box2d b_;

      /// Smap data between images.
      void swap_(data< gimp_image<TI> >& other_);
    };

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
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T& lvalue;


    /// Skeleton.
    typedef gimp_image<TI> skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<value> vset;


    /// Constructor without argument.
    gimp_image();

    /// Constructor with a gimp region.
    gimp_image(GimpPixelRgn* rgn);

    /// Initialise an image from a gimp region.
    void init_(GimpPixelRgn* rgn);

    /// Test if \p p is valid.
    bool has(const point2d& p) const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t nelements() const;

    /// const-only access to the image value located at point \p p.
    const T& operator()(const point& p) const;

    // FIXME: does not work! We need a shell object for enabling writing!
    /// Read-write access to the image value located at point \p p.
    T& operator()(const point& p);

    /// Fast Image method

    /// Give a hook to the value buffer.
    // const T* buffer() const;

    // /// Give a hook to the value buffer.
    // T* buffer();

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
//      buffer_ = (typename gimp_image<TI>::value*) rgn->data;
      b_ = make::box2d(rgn->y, rgn->x,
		       rgn->y + rgn->h - 1,
		       rgn->x + rgn->w - 1);
    }

    template <GimpImageType TI>
    data< gimp_image<TI> >::~data()
    {
    }

    template <GimpImageType TI>
    void
    data< gimp_image<TI> >::swap_(data< gimp_image<TI> >& other_)
    {
      data< gimp_image<TI> > self_ = *this;
      *this = other_;
      other_ = self_;
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
  }

  template <GimpImageType TI>
  void
  gimp_image<TI>::init_(GimpPixelRgn* rgn)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< gimp_image<TI> >(rgn);
  }


  template <GimpImageType TI>
  const typename gimp_image<TI>::vset&
  gimp_image<TI>::values() const
  {
    return vset::the();
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
  std::size_t
  gimp_image<TI>::nelements() const
  {
     mln_precondition(this->is_valid());
     return this->data_->b_.nsites();
  }


  template <GimpImageType TI>
  bool
  gimp_image<TI>::has(const point2d& p) const
  {
     mln_precondition(this->is_valid());
     return this->data_->b_.has(p);
  }


  template <GimpImageType TI>
  const mln_value(gimp_image<TI>)&
  gimp_image<TI>::operator()(const point& p) const
  {
    // mln_precondition(this->has(p));
     // FIXME HERE value*) this->data_->rgn->data

    static mln::value::rgb8 c;
    gimp_pixel_rgn_get_pixel(this->data_->rgn_,
			     (guchar *) &c,
			     p[1], p[0]);
    return c;
 }

  template <GimpImageType TI>
  mln_value(gimp_image<TI>)&
  gimp_image<TI>::operator()(const point& p)
  {
    // mln_precondition(this->has(p));
    static mln::value::rgb8 c;
    gimp_pixel_rgn_get_pixel(this->data_->rgn_,
			     (guchar *) &c,
			     p[1], p[0]);
    return c;
  }

  // template <GimpImageType TI>
  // const mln_value(gimp_image<TI>)*
  // gimp_image<TI>::buffer() const
  // {
  //   mln_precondition(this->is_valid());
  //   return this->data_->buffer_;
  // }


  // template <GimpImageType TI>
  // mln_value(gimp_image<TI>)*
  // gimp_image<TI>::buffer()
  // {
  //   mln_precondition(this->is_valid());
  //   return this->data_->buffer_;
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
