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
