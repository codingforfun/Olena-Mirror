// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_TAGS_HH
# define OLENA_CORE_TAGS_HH

# include <oln/core/abstract/image_constness.hh>
# include <oln/core/abstract/image_dimension.hh>

#define oln_tag_decl(TAG)			\
						\
struct TAG					\
{						\
protected:					\
  TAG() {}					\
}


#define oln_tag_decl_case(MTAG, TAG, MCLASS)		\
							\
struct TAG : public MTAG				\
{							\
protected:						\
  TAG() {}						\
};							\
							\
namespace internal {					\
							\
  template <typename E>					\
  struct image_case < TAG, E > : public MCLASS <E>	\
  {							\
  protected:						\
    image_case() {}					\
  };							\
}


#define oln_tag_decl_retrieve_from_props(TAG)		\
							\
template <typename E>					\
struct retrieve < tag::TAG, E >				\
{							\
  typedef typename props<cat::image,E>::TAG##_tag ret; /* FIXME: ok? */		\
  /* static test if ret derives from tag::TAG */	\
}



namespace oln {


  namespace tag {


    namespace internal {

      template <typename TAG, typename E> struct image_case;
      template <typename TAG, typename E> struct retrieve;

    } // end of namespace tag::internal


    oln_tag_decl ( constness );
    oln_tag_decl_case( constness, readonly,  abstract::readonly_image )
    oln_tag_decl_case( constness, readwrite, abstract::readwrite_image )

    oln_tag_decl ( dimension );
    oln_tag_decl_case( dimension, dimension1, abstract::image1d)
    oln_tag_decl_case( dimension, dimension2, abstract::image2d)
    oln_tag_decl_case( dimension, dimension3, abstract::image3d)

    namespace internal {

      template <typename E>
      struct retrieve < tag::constness, E >
      {
	typedef typename props<cat::image,E>::constness_tag ret; // FIXME: ok?
	// test if ret derives from constness_tag
      };

      template <typename E>
      struct retrieve < tag::dimension, E >
      {
	typedef typename props<cat::image,E>::dimension_tag ret; // FIXME: ok?
	// test if ret derives from constness_tag
      };
 

      template <typename TAG, typename E>
      struct image_switch : 
              public image_case< typename retrieve<TAG,E>::ret, E>
      {
      protected:
	image_switch() {}
      };


    } // end of namespace tag::internal

  } // end of namespace tag


  namespace abstract {

    template <typename E>
    struct image_entry : 
            public tag::internal::image_switch < tag::constness, E >,
            public tag::internal::image_switch < tag::dimension, E >
            // ...
    {
    protected:
      image_entry() {}
    };

  }

} // end of namespace oln


#endif // ! OLENA_CORE_TAGS_HH
