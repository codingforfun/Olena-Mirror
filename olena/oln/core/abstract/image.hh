#ifndef PROTO_OLN_CORE_ABSTRACT_IMAGE_HH
# define PROTO_OLN_CORE_ABSTRACT_IMAGE_HH

# include <oln/core/coord.hh>
# include <oln/core/macros.hh>
# include <mlc/type.hh>
# include <ntg/basics.hh>

namespace oln {

  // fwd decls

  struct dim1d;
  struct dim2d;
  // ...


  namespace abstract {

    template <typename E>
    struct image : public mlc::any__best_speed<E>
    {

      const oln_image_size_type(E)& size() const
      {
	return this->exact().size_impl();
      }

      const E clone() const
      {
	return this->exact().clone_impl();
      }

      unsigned long npoints() const
      {
	return this->exact().npoints_impl();
      }

      const oln_value_type(E) operator[](const oln_point_type(E)& p) const
      {
	assert(hold_large(p));
	return this->exact().op_square_brackets_impl(p);
      }

      oln_value_type(E)& operator[](const oln_point_type(E)& p)
      {
	assert(hold_large(p));
	return this->exact().op_square_brackets_impl(p);
      }

      bool hold(const oln_point_type(E)& p) const
      {
	return this->exact().hold_impl(p);
      }

      bool hold_large(const oln_point_type(E)& p) const
      {
	return this->exact().hold_large_impl(p);
      }

    protected:

      typedef mlc::any__best_speed<E> super;

      image(E* this_ = 0) :
	super()
      {}

      virtual ~image() {}
    };


    // discriminant is linear-buffer-data-storage

    template <typename E>
    struct linbuf_image : public virtual image<E>
    {

//       const oln_value_type(E) operator[](int i) const
//       {
// 	// FIXME: assert(i from ? to ?)...
// 	return this->exact().op_square_brackets(i);
//       }

//       oln_value_type(E)& operator[](int i)
//       {
// 	// FIXME: assert(i from ? to ?)...
// 	return this->exact().op_square_brackets(i);
//       }

      oln_value_type(E)* buffer()
      {
	return this->exact().buffer_impl();
      }

      const oln_value_type(E)* buffer() const
      {
	return this->exact().buffer_impl();
      }

//       size_t buffer_size() const {
// 	return this->exact().buffer_size_impl();
//       }

    protected:
      typedef image<E> super;
      linbuf_image(E* this_ = 0) :
	super(this_)
      {}
    };


    // discriminant is dimension

    template <typename E>
    struct image1d : public virtual image<E>
    {
      coord nindices() const { return this->size().nindices(); }
      const oln_value_type(E) operator()(coord index) const
      {
	return this->operator[](oln_point_type(E)(index));
      }
      oln_value_type(E)& operator()(coord index)
      {
	return this->operator[](oln_point_type(E)(index));
      }
    protected:
      typedef image<E> super;
      image1d(E* this_ = 0) : super(this_) {}
    };

    template <typename E>
    struct image2d : public virtual image<E>
    {
      coord nrows() const { return this->size().nrows(); }
      coord ncols() const { return this->size().ncols(); }
      const oln_value_type(E) operator()(coord row, coord col) const
      {
	return this->operator[](oln_point_type(E)(row, col));
      }
      oln_value_type(E)& operator()(coord row, coord col)
      {
	return this->operator[](oln_point_type(E)(row, col));
      }
    protected:
      typedef image<E> super;
      image2d(E* this_ = 0) : super(this_) {}
    };


    // discriminant is value type tag

    template <typename E>
    struct label_image : public virtual image<E>
    {
    protected:
      typedef image<E> super;
      label_image(E* this_ = 0) : super(this_) {}
    };

    template <typename E>
    struct binary_image : public virtual label_image<E>
    {
    protected:
      typedef label_image<E> super;
      binary_image(E* this_ = 0) : super(this_) {}
    };

    template <typename E>
    struct scalar_image : public virtual image<E>
    {
    protected:
      typedef image<E> super;
      scalar_image(E* this_ = 0) : super(this_) {}
    };

    template <typename E>
    struct vector_image : public virtual image<E>
    {
    protected:
      typedef image<E> super;
      vector_image(E* this_ = 0) : super(this_) {}
    };


    // switches

    namespace internal {


      // switch over data storage (linear buffer or not)

      template <bool Linbuf_value, typename E>
      struct linbuf_switch;

      template <typename E>
      struct linbuf_switch <true, E> : public linbuf_image<E>
      {
	linbuf_switch(E* this_ = 0) : linbuf_image<E>(this_) {}
      };

      template <typename E>
      struct linbuf_switch <false, E> : public virtual image<E>
      {
	linbuf_switch(E* this_ = 0) : image<E>(this_) {}
      };

      // switch over dimension

      template <typename Dim, typename E>
      struct dim_switch;

      template <typename E>
      struct dim_switch <oln::dim1d, E> : public image1d<E>
      {
	dim_switch(E* this_ = 0) : image1d<E>(this_) {}
      };

      template <typename E>
      struct dim_switch <oln::dim2d, E> : public image2d<E>
      {
	dim_switch(E* this_ = 0) : image2d<E>(this_) {}
      };

      // switch over value type tag

      template <unsigned Tag, typename E>
      struct value_tag_switch;

      template <typename E>
      struct value_tag_switch<ntg::unknown_tag, E> : public virtual image<E>
      {
	value_tag_switch(E* this_ = 0) : image<E>(this_) {}
      };

      template <typename E>
      struct value_tag_switch<ntg::label_tag, E> : public label_image<E>
      {
	value_tag_switch(E* this_ = 0) : label_image<E>(this_) {}
      };

      template <typename E>
      struct value_tag_switch<ntg::binary_tag, E> : public binary_image<E>
      {
	value_tag_switch(E* this_ = 0) : binary_image<E>(this_) {}
      };

      template <typename E>
      struct value_tag_switch<ntg::scalar_tag, E> : public scalar_image<E>
      {
	value_tag_switch(E* this_ = 0) : scalar_image<E>(this_) {}
      };

      template <typename E>
      struct value_tag_switch<ntg::vector_tag, E> : public vector_image<E>
      {
	value_tag_switch(E* this_ = 0) : vector_image<E>(this_) {}
      };

    } // end of namespace internal




    // entry point of image hierarchy

    template <typename E>
    struct image_entry : public internal::linbuf_switch< oln_linbuf_value(E), E >,
			 public internal::dim_switch< oln_dim_type(E), E >,
			 public internal::value_tag_switch< ntg_tag_value(oln_value_type(E)), E >
    {
    protected:
      image_entry(E* this_ = 0) :
	internal::linbuf_switch< oln_linbuf_value(E), E >(this_),
	internal::dim_switch< oln_dim_type(E), E >(this_),
	internal::value_tag_switch< ntg_tag_value(oln_value_type(E)), E >(this_)
	{}
    };


  } // end of namespace abstract

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_ABSTRACT_IMAGE_HH
