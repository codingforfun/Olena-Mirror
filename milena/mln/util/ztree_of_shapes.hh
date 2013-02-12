// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
/// \brief Tree of shapes data structure.

#ifndef MLN_UTIL_ZTREE_OF_SHAPES_HH
# define MLN_UTIL_ZTREE_OF_SHAPES_HH

# include <vector>
# include <mln/value/rgb8.hh>
# include <mln/util/map.hh>
# include <mln/core/image/image2d.hh>
# include <mln/util/tree_of_shapes.hh>
# include <mln/data/transform.hh>
# include <mln/border/resize.hh>

# include <mln/debug/println.hh>
# include <mln/util/object_id.hh>

namespace mln
{

  // Type used to refer to a pixel in one of the ztree images.
  struct face_id {};
  typedef unsigned face_id_t;


  namespace util
  {
    enum Hole { Not_hole, Is_hole, Contains_hole };
    enum Tags { None = 0, Spurious, Noise, Object, Background };

    struct attrib_t
    {
      attrib_t()
      {
	reset();
      }

      void reset()
      {
	tag = None;
	own_area = 0;
	area = 0;
	nchild = 0;
	color = literal::black;
	is_leaf = true;
	own_area_ratio = 0;
	rectangularity = 0;
	elongation = 0;
	d_color_q = 0;
	d_color_gp = 0;
	dlap = 0;
	max_abs_lap = 0;
	hole = Not_hole;
	bg_score = 0;
	fg_score = 0;
	is_valid = false;
      }

      // It is stored on any representatives.
      Tags tag;

      // Corresponds to the number of pixels of the same level as the
      // related representative.
      // It is stored on any representatives.
      unsigned own_area;

      // Corresponds to the number of pixels of all the sub-shapes and
      // at the same level.
      // It is stored on any representatives.
      unsigned area;

      // The bounding box.
      // It is stored on 0-representatives ONLY.
      box2d bbox;

      // The mean color of the shape. It relies only on values from
      // 2-faces owned by this shape.
      // It is stored on any representatives.
      value::rgb8 color;

      // // The weighted mean color of the shape. It relies only on
      // // values from 2-faces owned by this shape.  It is stored on any
      // // representatives.
      // value::rgb8 wcolor;

      // The number of children in the tree.
      // It is stored on any representatives.
      unsigned nchild;

      // Is it a representative of a leaf ?.
      // It is stored on any representatives.
      bool is_leaf;

      // The ratio own_area / area.
      // It is stored on 0-representatives ONLY.
      float own_area_ratio;

      // It is stored on 0-representatives ONLY.
      float rectangularity;

      // It is stored on 0-representatives ONLY.
      float elongation;

      // The maximum color distance between the non-0-representative
      // children and the non-0-representative parent colors.
      // It is stored on 0-representatives ONLY.
      //
      //     @
      //     |
      //     o  gp
      //     |
      //     @
      //     |
      //     o
      //     @   --> d_color_gp = max(d_color_gp, l1::distance(p.color, gp.color))
      //     |
      //     o  p
      unsigned d_color_q;

      // The maximum color distance between the non-0-representative
      // children and the non-0-representative grandparent colors.
      // It is stored on 0-representatives ONLY.
      //
      //     @
      //     |
      //     o  gp
      //     |
      //     @
      //     |
      //     o
      //     @   --> d_color_gp = max(d_color_gp, l1::distance(p.color, gp.color))
      //     |
      //     o  p
      unsigned d_color_gp;

      // Maximum distance between the non-0-representative children
      // and the non-0-representative parent laplacian values.
      // It is stored on 0-representatives ONLY.
      unsigned dlap;

      // Maximum absolute value in laplacian image for the current
      // shape.
      // It is stored on any representatives.
      unsigned max_abs_lap;

      // Set whether this shape contains a hole or is a hole.
      // It is stored on any representatives.
      Hole hole;

      // The higher is this score, the higher is the probability that
      // this shape is a background.  It is based on the hypothesis
      // that interesting objects are leaves (excepted holes).  This
      // score take into account the number of holes and leaves
      // included in that shape.
      // It is stored on non-0-representatives ONLY.
      unsigned bg_score;

      // The higher is this score, the higher is the probability that
      // this shape is foreground.
      // It is stored on non-0-representatives ONLY.
      unsigned fg_score;

      bool is_valid;
    };

    bool operator==(const attrib_t& l, const attrib_t& r)
    {
      return l.tag == r.tag
	&& l.own_area == r.own_area
	&& l.area == r.area
	&& l.bbox == r.bbox
	&& l.color == r.color
	&& l.nchild == r.nchild
	&& l.is_leaf == r.is_leaf
	&& l.own_area_ratio == r.own_area_ratio
	&& l.rectangularity == r.rectangularity
	&& l.elongation == r.elongation
	&& l.d_color_q == r.d_color_q
	&& l.d_color_gp == r.d_color_gp
	&& l.dlap == r.dlap
	&& l.max_abs_lap == r.max_abs_lap
	&& l.hole == r.hole
	&& l.bg_score == r.bg_score
	&& l.fg_score == r.fg_score
	&& l.is_valid == r.is_valid;
    }

    bool operator!=(const attrib_t& l, const attrib_t& r)
    {
      return ! (l == r);
    }

    template <typename I>
    class ztree_of_shapes
    {
    public:
      typedef mln_site(I) P;
      typedef mln_value(I) V;
      typedef mln_equiv(V) EV;
      typedef mln_ch_value(I,face_id_t) parent_t; // FIXME: could be
						  // replaced by an
						  // array (direct
						  // access from
						  // face_id_t).
      typedef I Fb_t;

      // all pixels, from root to leaf
      std::vector<face_id_t> R;
      // 0-representatives only, from root to leaf
      std::vector<face_id_t> R0;
      // representative only, from root to leaf
      std::vector<face_id_t> R01;

      ztree_of_shapes();
      ztree_of_shapes(const util::tree_of_shapes<I>& tree,
		      const I& lap_k1);

      V level(const P& p) const;

      bool is_root(const P& p) const;
      bool is_root(const face_id_t& pi) const;

      bool is_representative(const P& p) const;
      bool is_representative(const face_id_t& pi) const;

      bool is_0_representative(const P& p) const;
      bool is_0_representative(const face_id_t& p) const;

      bool is_non_0_representative(const P& p) const;
      bool is_non_0_representative(const face_id_t& p) const;

      bool is_compressed() const;
      bool is_valid() const;


      const face_id_t& parent(const face_id_t& pi) const;
      P parentp(const P& pi) const;

      const V& Fb(const face_id_t& pi) const;
      const V& Fb(const P& pi) const;
      const V& lap(const face_id_t& pi) const;
      const V& lap(const P& pi) const;

      const box2d& parent_domain() const;
      const parent_t& parent_image() const;
      parent_t& parent_image_();

      const box2d& Fb_domain() const;
      const I& Fb_image() const;
      I& Fb_image_();

      I& lap_image();
      const I& lap_image() const;

      void trim(const util::array<face_id_t>& f,
		const std::vector<face_id_t>& R0,
		const std::vector<face_id_t>& R01,
		const util::array<bool>& is_removed);

      // Debug
      bool is_01_canonicalized() const;

      face_id_t offset_of(const point2d& p) const;
      point2d face_at(const face_id_t& pi) const;

      unsigned nfaces() const;

    private:

      Fb_t Fb_;
      // compressed parent image.
      parent_t parent_;
      I lap_k1_;

      void compress_tree();
      bool compressed_;
    };


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename V>
      bool have_same_sign(const V& v1, const V& v2)
      {
	// FIXME: Remove when Fb is flat.
	mln_precondition(v1.is_degenerated() && v2.is_degenerated());
	if (!v1.is_degenerated() || !v2.is_degenerated())
	{
	  std::cout << "OUUUUPPPSSS" << std::endl;
	  abort();
	}

	return (v1.first() == 0 && v2.first() == 0) || (v1.first() > 0 && v2.first() > 0) || (v1.first() < 0 && v2.first() < 0);
      }


      template <typename I, typename P>
      bool
      is_01_representative_in_parent01(util::ztree_of_shapes<I>& t, const P& p)
      {
	typedef mln_value(I) V;
	return t.is_root(p)
	  || (t.Fb(p) == V(0) && t.Fb(t.parent(p)) != V(0))
	  || (t.Fb(p) != V(0) && t.Fb(t.parent(p)) == V(0));
      }

      template <typename I>
      struct point_to_offset_t : Function_v2v<point_to_offset_t<I> >
      {
	typedef face_id_t result;
	typedef point2d argument;

	point_to_offset_t(const I& Fb) : Fb_(Fb) {}

	result operator()(const argument& v) const
	{
	  return face_id_t(Fb_.offset_of_point(v));
	}

	I Fb_;
      };

    }

    template <typename I>
    ztree_of_shapes<I>::ztree_of_shapes()
      : compressed_(false)
    {
    }

    template <typename I>
    ztree_of_shapes<I>::ztree_of_shapes(const util::tree_of_shapes<I>& tree,
					const I& lap_k1)
    {
      lap_k1_ = duplicate(lap_k1);
      border::resize(lap_k1_, 1);

      // Make sure that underlying images have same border (site
      // offsets are therefore equivalent).
      Fb_ = duplicate(tree.Fb);
      border::resize(Fb_, 1);

      internal::point_to_offset_t<I> point_to_offset(Fb_);
      parent_ = data::transform(tree.parent, point_to_offset);
      border::resize(parent_, 1);

      // Check image sizes.
      mln_assertion(Fb_.domain() == tree.parent.domain());
      mln_assertion(Fb_.border() == parent_.border());
      mln_assertion(Fb_.domain() == parent_.domain());

# ifndef RELEASE
      if (Fb_.domain() != tree.parent.domain()
	  || Fb_.border() != parent_.border()
	  || Fb_.domain() != parent_.domain())
	abort();

      //Check
      {
	mln_piter(image2d<face_id_t>) p(parent_domain());
	for_all(p)
	  if (parent(offset_of(p)) >= nfaces())
	  {
	    std::cout << "Invalid parent id!" << std::endl;
	    abort();
	  }
      }


      // debug::println(Fb_);
      // debug::println(parent_);
# endif // ! RELEASE

      for (unsigned i = 0; i < tree.R.size(); ++i)
	R.push_back(offset_of(tree.R[i]));

      // Initialize R0, 0-representative faces and R01 representative
      // pixels in compressed tree.
      for (unsigned i = 0; i < R.size(); ++i)
      {
	face_id_t pi = R[i];
	if (is_representative(pi))
	{
	  R01.push_back(pi);
	  if (is_0_representative(pi))
	    R0.push_back(pi);
	}
      }

      compress_tree();
    }

    template <typename I>
    face_id_t
    ztree_of_shapes<I>::offset_of(const point2d& p) const
    {
      return Fb_.offset_of_point(p);
    }

    template <typename I>
    point2d
    ztree_of_shapes<I>::face_at(const face_id_t& pi) const
    {
      return Fb_.point_at_offset(pi);
    }

    template <typename I>
    unsigned
    ztree_of_shapes<I>::nfaces() const
    {
      return Fb_.nelements();
    }

    template <typename I>
    typename ztree_of_shapes<I>::V
    ztree_of_shapes<I>::level(const P& p) const
    {
      return Fb(p);
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_root(const face_id_t& pi) const
    {
      return parent(pi) == pi;
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_root(const P& p) const
    {
      return is_root(offset_of(p));
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_0_representative(const P& p) const
    {
      return is_0_representative(offset_of(p));
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_0_representative(const face_id_t& pi) const
    {
      return (is_root(pi) || Fb(parent(pi)) != Fb(pi)) && (Fb(pi) == V(0));
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_non_0_representative(const P& p) const
    {
      return is_non_0_representative(offset_of(p));
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_non_0_representative(const face_id_t& pi) const
    {
      return is_representative(pi) && ! is_0_representative(pi);
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_representative(const P& p) const
    {
      return is_representative(offset_of(p));
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_representative(const face_id_t& pi) const
    {
      return is_root(pi)
	|| (Fb(pi) == V(0) && Fb(parent(pi)) != V(0))
	|| (Fb(pi) != V(0) && Fb(parent(pi)) == V(0));
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_valid() const
    {
      return is_compressed();
    }

    template <typename I>
    bool
    ztree_of_shapes<I>::is_compressed() const
    {
      mln_precondition(is_01_canonicalized());

# ifndef RELEASE
      // Precondition
      if (!is_01_canonicalized())
	abort();
# endif // ! RELEASE

      return compressed_;
    }


    template <typename I>
    const face_id_t&
    ztree_of_shapes<I>::parent(const face_id_t& pi) const
    {
      return parent_.element(pi);
    }

    template <typename I>
    typename ztree_of_shapes<I>::P
    ztree_of_shapes<I>::parentp(const P& p) const
    {
      return face_at(parent(offset_of(p)));
    }

    template <typename I>
    const typename ztree_of_shapes<I>::V&
    ztree_of_shapes<I>::Fb(const face_id_t& pi) const
    {
      return Fb_.element(pi);
    }

    template <typename I>
    const typename ztree_of_shapes<I>::V&
    ztree_of_shapes<I>::Fb(const P& p) const
    {
      return Fb(offset_of(p));
    }

    template <typename I>
    const typename ztree_of_shapes<I>::V&
    ztree_of_shapes<I>::lap(const face_id_t& pi) const
    {
      return lap_k1_.element(pi);
    }

    template <typename I>
    const typename ztree_of_shapes<I>::V&
    ztree_of_shapes<I>::lap(const P& p) const
    {
      return lap(offset_of(p));
    }

    template <typename I>
    const box2d&
    ztree_of_shapes<I>::parent_domain() const
    {
      return parent_.domain();
    }

    template <typename I>
    const typename ztree_of_shapes<I>::parent_t&
    ztree_of_shapes<I>::parent_image() const
    {
      return parent_;
    }

    template <typename I>
    typename ztree_of_shapes<I>::parent_t&
    ztree_of_shapes<I>::parent_image_()
    {
      return parent_;
    }

    template <typename I>
    const box2d&
    ztree_of_shapes<I>::Fb_domain() const
    {
      return Fb_.domain();
    }

    template <typename I>
    const I&
    ztree_of_shapes<I>::Fb_image() const
    {
      return Fb_;
    }

    template <typename I>
    I&
    ztree_of_shapes<I>::Fb_image_()
    {
      return Fb_;
    }

    template <typename I>
    I&
    ztree_of_shapes<I>::lap_image()
    {
      return lap_k1_;
    }

    template <typename I>
    const I&
    ztree_of_shapes<I>::lap_image() const
    {
      return lap_k1_;
    }

    template <typename I>
    void
    ztree_of_shapes<I>::compress_tree()
    {
      typedef mln_site(I) P;

      unsigned N = parent_.nsites();

      // // DEBUG
      // {
      // 	image2d<bool> imap0(parent.domain());
      // 	data::fill(imap0, false);
      // 	for (unsigned i = 0; i < R0.size(); ++i)
      // 	  imap0(R0[i]) = true;

      // 	io::magick::save(imap0, "debug_imap0.pbm");
      // 	std::cout << "0-node count: " << R0.size() << std::endl;
      // }
      // {
      // 	image2d<value::rgb8> imap01(parent.domain());
      // 	data::fill(imap01, literal::black);
      // 	for (unsigned i = 0; i < R01.size(); ++i)
      // 	  if (is_0_zrepresentative(t, R01[i]))
      // 	    imap01(R01[i]) = literal::blue;
      // 	  else
      // 	    imap01(R01[i]) = literal::red;

      // 	io::magick::save(imap01, "debug_imap01.ppm");
      // 	std::cout << "representative count: " << R01.size() << std::endl;
      // }


      // Canonicalization
      typedef mln_value(I) V;
      for (unsigned i = 0; i < N; ++i)
      {
	face_id_t p = R[i];  // p goes from root to leaves
	face_id_t q = parent(p);
	if (! is_representative(q))
	  parent_.element(p) = parent(q);
      }

      compressed_ = true;

# ifndef RELEASE
      // Check
      for (unsigned i = 0; i < N; ++i)
      {
	face_id_t p = R[i];  // p goes from root to leaves
	face_id_t q = parent(p);
	if (! is_representative(q))
	{
	  std::cerr << "OOOOOOOOOOOO" << std::endl;
	  std::abort();
	}
      }


      // Check
      unsigned j = 0;
      for (unsigned i = 0; i < N; ++i)
	if (internal::is_01_representative_in_parent01(*this, R[i]))
	  if (R01[j++] != R[i])
	  {
	    std::cout << "Oups 2" << std::endl;
	    std::abort();
	  }
# endif // ! RELEASE

    }



    template <typename I>
    bool
    ztree_of_shapes<I>::is_01_canonicalized() const
    {
      mln_precondition(parent.is_valid());

      // Check if parent relationship is ok.
      mln_piter(image2d<point2d>) p(parent_domain());
      for_all(p)
      {
	face_id_t pi = offset_of(p);
	if (! is_representative(parent(pi)))
	{
	  std::cout << "Parent " << parent(pi)
		    << " is not 01-representative!" << std::endl;
# ifndef RELEASE
	  std::abort();
# endif // ! RELEASE
	  return false;
	}

	if (is_representative(pi))
	{
	  if (! is_root(pi))
	    if (internal::have_same_sign(Fb(pi), Fb(parent(pi))))
	    {
	      std::cout << "A repr and its parent have same sign!" << std::endl;
# ifndef RELEASE
	      std::abort();
# endif // ! RELEASE
	      return false;
	    }
	}
	else // p is not a repr
	  if (! internal::have_same_sign(Fb(pi), Fb(parent(pi))))
	  {
	    std::cout << "a regular p and its repr have not the same sign!" << std::endl;
# ifndef RELEASE
	    std::abort();
# endif // ! RELEASE
	    return false;
	  }
      }
      return true;
    }

    template <typename I>
    void
    ztree_of_shapes<I>::trim(const util::array<face_id_t>& f,
			     const std::vector<face_id_t>& R0_,
			     const std::vector<face_id_t>& R01_,
			     const util::array<bool>& is_removed)
    {
      // We may merge shapes and faces having a different sign value
      // in Fb. It would break the tree properties for recognizing
      // representative among all other faces.  To fix this issue, we
      // update Fb so that all faces have the same value as their
      // parent.
      //
      // Warning: we must update Fb before parent_ in order to
      // preserve the validity of is_representative() which is based
      // on the difference of levels between a pixel and its parent to
      // decide whether it is a representative or not. Faces parts of
      // 0-representative nodes may not be updated if this order of
      // updates is not preserved.
      for (int i = R.size() - 1; i >= 0; --i) // leaves to root
      {
	face_id_t p = R[i];
	if (! is_representative(p) || is_removed(p))
	  Fb_.element(p) = Fb(f(parent(p)));
      }

      // Adjust parent data.
      parent_ = data::transform(parent_, f);

      // Update representative arrays.
      R0 = R0_;
      R01 = R01_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_ZTREE_OF_SHAPES_HH
