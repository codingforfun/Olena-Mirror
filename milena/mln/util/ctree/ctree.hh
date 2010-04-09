// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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


// \todo definition of site, psite is ambigous, replace with content_t
// or other.
// \todo node operator ?
//
// \todo handle forest in case of non contiguous domain pictures.



#ifndef MLN_UTIL_CTREE_CTREE_HH
# define MLN_UTIL_CTREE_CTREE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/site_set.hh>
# include <mln/util/ctree/internal/tree_base.hh>
# include <mln/util/array.hh>
# include <mln/core/image/attribute_image.hh>
# include <algorithm>

# define mln_node(T) typename T::node_t

namespace mln
{

  /// Forward declaration.
  namespace util { namespace ctree { template <typename I> class ctree; } }

  namespace morpho
  {

    namespace tree
    {

      namespace impl
      {

	namespace generic
	{

	  template <typename T, typename I, typename N>
	  util::ctree::ctree<I>
	  union_find_fast(const tag::tree::tree_t<T>& tree_tag,
			  const Image<I>& f,
			  const Neighborhood<N>& nbh);

	} // end of namespace morpho::tree::impl::generic

	template <typename T, typename I, typename N>
	util::ctree::ctree<I>
	hqueue_fast(const tag::tree::tree_t<T>&,
		    const Image<I>& f,
		    const Neighborhood<N>& nbh);

      } // end of namespace morpho::tree::impl

    } // end of namespace morpho::tree

  } // end of namespace morpho


  namespace internal
  {
    /// Data structure for component tree.
    template <>
    template <typename I>
    struct data< util::ctree::ctree<I> >
    {
      typedef util::ctree::ctree<I> T;
      typedef typename T::node_t::index_t index_t;

      data(const I&);
      ~data();

      mln_ch_value(I, index_t)	map_;
      std::vector<index_t>	parent_;
      std::vector<unsigned>	length_;
      std::vector<mln_value(I)> values_;

      const mln_domain(I)&	domain_;
      int			nnodes;
    };

  } // end of namespace mln::internal

  namespace util
  {

    namespace ctree
    {

      template <typename I>
      struct ctree : public internal::tree_base< I, ctree<I> >
      {
	/// The super class.
	typedef internal::tree_base<I, ctree<I> > super;

	/// Self class
	typedef ctree<I> self;

	/// Site related type definitions.
	/// \{
	/// The type of site.
	typedef mln_site(I) site;
	/// The type of pseudo-site.
	typedef mln_psite(I) psite;
	/// The type of domain.
	typedef mln_domain(I) domain_t;
	/// \}

	/// Node related type definions.
	/// \{
	/// The type of node.
	typedef util::ctree::node<self> node_t;
	/// The type of node set.
	typedef p_nodes<self> nodes_t;
	/// \}

	/// Node/Site value related type definitions.
	/// \{
	/// The type of node value
	typedef mln_value(I) value;
	/// The type of node rvalue.
	typedef mln_rvalue(I) rvalue;
	/// The type of node lvalue.
	typedef mln_lvalue(I) lvalue;
	/// \}

	typedef mln_ch_value(I, typename node_t::index_t) map_t;

	/// Constructors
	/// \{
	ctree();
	/// \}

	/// Reserve space for tree.
	void reserve(const Image<I>& ima, unsigned nnodes);


	/// Constructor from union-find results.
	/// \param[in] f The input image.
	/// \param[in] parent The (not necessary canonized) parent image.
	/// \param[in] area The number of nodes in each sub-tree.
	/// \param[in] s The sorted site set.
	/// \param[in] nb_nodes The total number of nodes.
	template <typename S>
	ctree(const Image<I>& f_,
	      const S& s,
	      mln_ch_value(I, unsigned)& parent,
	      mln_ch_value(I, unsigned)& area,
	      unsigned nb_nodes);

	// ctree(const Image<I>& f,
	//       const Site_Set<S>& s,
	//       mln_ch_value(I, psite)& parent,
	//       mln_ch_value(I, unsigned)& area,
	//       unsigned nb_nodes);

	/// \}

	ctree(const Image<I>& f,
	      const mln_ch_value(I, unsigned)& map,
	      const std::vector<unsigned>& nodes,
	      const std::vector<unsigned>& parent,
	      const std::vector<unsigned>& area);

	/// Misc.
	/// \{
	const domain_t& domain() const;
	const map_t& map() const;
	nodes_t nodes() const;
	bool has(const psite&) const;
	bool has(const node_t&) const;

	int n_nodes() const;
	/// \}

	/// Value access  material.
	/// \{
	rvalue f(const psite&) const;
	lvalue f(const psite&);
	rvalue f(const node_t&) const;
	lvalue f(const node_t&);
	/// \}

	/// Node relationship material.
	/// \{
	node_t node(const psite&) const;
	void set_node(const psite&, const node_t&);
	node_t parent(const node_t&) const;
	void set_parent(const node_t& n, const node_t& parent);

	/// Return the node set of the sub-tree of \p n.
	/// \post Non empty set: n belongs to desc(n).
	nodes_t desc(const node_t& n) const;

	/// Return the node set of ancestors of \p n.
	nodes_t anc(const node_t& n) const;

	/// Return the number of descendants of \p n.
	/// Equivalent to desc(n).nsites() - 1.
	unsigned length_(const node_t& n) const;
	/// \}

	/// Node tests material.
	/// \{
	bool is_a_leaf(const node_t&) const;
	bool is_a_root(const node_t&) const;
	/// \}

	/// Fast access material.
	/// \{

	/// The type of index.
	typedef typename node_t::index_t index_t;

	bool has_index(index_t i) const;

	/// Return the number of descendants of the node at index \p i.
	/// Equivalent to desc(tree.node_at(i)).nsites().
	unsigned length_at_(index_t i) const;
	unsigned& length_at_(index_t i);
	rvalue f_at_(index_t i) const;
	lvalue f_at_(index_t i);
	index_t parent_at_(index_t i) const;
	index_t& parent_at_(index_t i);
	index_t node_at_(const psite& p) const;
	index_t& node_at_(const psite& p);
	/// \}

	/// To attribute image
	attribute_image<ctree<I>, value> to_image() const;
	void to_image(attribute_image<ctree<I>, value>& a) const;


	/// Friend functions.
	/// \{
	template <typename T, typename J, typename N>
	friend util::ctree::ctree<J>
	morpho::tree::impl::generic::union_find_fast(const tag::tree::tree_t<T>&, const Image<J>&, const Neighborhood<N>&);

	template <typename T, typename J, typename N>
	friend util::ctree::ctree<J>
	morpho::tree::impl::hqueue_fast(const tag::tree::tree_t<T>&, const Image<J>&, const Neighborhood<N>&);
	/// \}

      };

    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename I>
    inline
    data< util::ctree::ctree<I> >::data(const I& f)
      : domain_ (f.domain())
    {
    }

    template <typename I>
    inline
    data< util::ctree::ctree<I> >::~data()
    {
    }

  } // end of namespace mln::internal

  namespace util
  {

    namespace ctree
    {
      namespace internal
      {
	template <typename T>
	struct create_node : Function_v2v< create_node<T> >
	{
	  typedef typename T::index_t param;
	  typedef typename T::node_t result;

	  create_node(const T& tree)
	    : tree_ (tree)
	  {
	  }

	  result
	  operator()(const param& idx) const
	  {
	    result tmp(tree_, idx);
	    return tmp;
	  }

	private:
	  const T& tree_;
	};
      }


      template <typename I>
      inline
      ctree<I>::ctree()
      {
      }

      template <typename I>
      inline
      void
      ctree<I>::reserve(const Image<I>& f_, unsigned nnodes)
      {
	const I& f = exact(f_);

	mln_precondition(f.is_valid());
	this->data_ = new mln::internal::data< ctree<I> >(f);

	initialize(this->data_->map_, f);
	this->data_->parent_.resize(nnodes);
	this->data_->length_.resize(nnodes);
	this->data_->values_.resize(nnodes);
        this->data_->nnodes = nnodes;
# ifndef DNDEBUG
	data::fill(this->data_->map_, 0);
        std::fill(this->data_->parent_.begin(), this->data_->parent_.end(), 0);
        std::fill(this->data_->length_.begin(), this->data_->length_.end(), 0);
# endif
      }


      template <typename I>
      template <typename S>
      inline
      ctree<I>::ctree(const Image<I>& f_,
		      const S& s,
		      mln_ch_value(I, unsigned)& parent,
		      mln_ch_value(I, unsigned)& area,
		      unsigned nb_nodes)
      {
	const I& f = exact(f_);

	mln_precondition(f.is_valid());
	mln_precondition(parent.is_valid());
	mln_precondition(area.is_valid());
	mln_precondition(f.domain() == parent.domain());
	mln_precondition(f.domain() == area.domain());

	this->data_ = new mln::internal::data< ctree<I> >(f);

	initialize(this->data_->map_, f);
	this->data_->parent_.resize(nb_nodes);
	this->data_->length_.resize(nb_nodes);
	this->data_->values_.resize(nb_nodes);
	this->data_->nnodes = nb_nodes;

	//debug::println("Parent: ", parent);
	//debug::println("Area: ", area);

	unsigned root_offset = 0;

	const unsigned nvalues = mln_card(mln_value(I));

	// FIXME: max tree only
	for (unsigned v = 0; v < nvalues; ++v)
	  {
	    const util::array<unsigned>& s_v = s[v];
	    unsigned n_p = s_v.nelements();

	    for (int i = n_p - 1; i >= 0; --i)
	      {
		unsigned p = s_v[i];
		unsigned q = parent.element(p);

		if (f.element(parent.element(q)) == f.element(q)) // Canonization
		  q = (parent.element(p) = parent.element(q));

		if (f.element(p) == f.element(q) && p != q) // Not a node.
		  {
		    mln_assertion(q == parent.element(p));
		    this->data_->map_.element(p) =
		      this->data_->map_.element(q);
		  }
		else
		  {
		    unsigned& offset = (p == q) ? root_offset : area.element(q);

		    // Insert Node.
		    mln_assertion(offset < nb_nodes);
		    this->data_->map_.element(p) = offset;
		    this->data_->parent_[offset] = this->data_->map_.element(q);
		    this->data_->values_[offset] = f.element(p);
		    this->data_->length_[offset] = area.element(p);
		    area.element(p) = offset + 1;
		    offset += this->data_->length_[offset] + 1;
		  }
	      }
	  }
      }

// template <typename T>
// std::ostream&
// operator<< (std::ostream& os, const std::vector<T>& v)
// {
//   for (unsigned i = 0; i < v.size(); i++)
//     os << v[i] << " ";
//   return os;
// }

      template <typename I>
      inline
      ctree<I>::ctree(const Image<I>& f_,
		      const mln_ch_value(I, unsigned)& map,
		      const std::vector<unsigned>& location,
		      const std::vector<unsigned>& parent,
		      const std::vector<unsigned>& area)
      {
	const I& f = exact(f_);
	unsigned n_nodes = location.size();

	mln_precondition(f.is_valid());
	mln_precondition(map.is_valid());
	mln_precondition(parent.size() == n_nodes);
	mln_precondition(area.size() == n_nodes);

	this->data_ = new mln::internal::data< ctree<I> >(f);

	initialize(this->data_->map_, f);
	this->data_->parent_.resize(n_nodes);
	this->data_->length_.resize(n_nodes);
	this->data_->values_.resize(n_nodes);
	this->data_->nnodes = n_nodes;


	// debug::println("F:", f);
	// debug::println("map:", map);
	// std::cout << "Location: " << location << std::endl;
	// std::cout << "Parent: " << parent << std::endl;
	// std::cout << "Area: " << area << std::endl;


	std::vector<char> deja_vu(n_nodes, 0);
	mln_pixter(const I) pix(f);
	for_all(pix)
	{
	  unsigned idx = map.element(pix.offset());
	  unsigned loc = n_nodes - location[idx] - 1;
	  this->data_->map_.element(pix.offset()) = loc;

	  if (!deja_vu[idx])
	    {
	      this->data_->values_[loc] = pix.val();
	      this->data_->parent_[loc] = n_nodes - location[parent[idx]] - 1;
	      this->data_->length_[loc] = area[idx];
	      deja_vu[idx] = true;
	    }
	}

	// debug::println("map:", this->data_->map_);
	// std::cout << "Parent: " << this->data_->parent_ << std::endl;
	// std::cout << "Area: " << this->data_->length_ << std::endl;
	// std::cout << "Values: " << this->data_->values_ << std::endl;

      }

      template <typename I>
      inline
      const mln_domain(I)&
      ctree<I>::domain() const
      {
	mln_precondition(this->is_valid());
	return this->data_->domain_;
      }

      template <typename I>
      inline
      typename ctree<I>::nodes_t
      ctree<I>::nodes() const
      {
	mln_precondition(this->is_valid());
	nodes_t nodes_(*this, node_t(*this, 0), this->data_->nnodes);
	return nodes_;
      }

      template <typename I>
      inline
      const typename ctree<I>::map_t&
      ctree<I>::map() const
      {
	mln_precondition(this->is_valid());
	return this->data_->map_;
      }

      template <typename I>
      inline
      int
      ctree<I>::n_nodes() const
      {
	mln_precondition(this->is_valid());
	return this->data_->nnodes;
      }

      template <typename I>
      inline
      bool
      ctree<I>::has(const mln_psite(I)& p) const
      {
	mln_precondition(this->is_valid());
	return this->data_->domain_.has(p);
      }

      template <typename I>
      inline
      bool
      ctree<I>::has(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(n.is_valid());
	mln_invariant(n.tree().data_.ptr_ == this->data_.ptr_);
	return n.index() < this->data_->nnodes;
      }

      template <typename I>
      inline
      mln_rvalue(I)
      ctree<I>::f (const mln_psite(I)& p) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	return this->data_->values_[this->data_->map_(p)];
      }

      template <typename I>
      inline
      mln_lvalue(I)
      ctree<I>::f (const mln_psite(I)& p)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	return this->data_->values_[this->data_->map_(p)];
      }

      template <typename I>
      inline
      mln_rvalue(I)
      ctree<I>::f (const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return this->data_->values_[n.index()];
      }

      template <typename I>
      inline
      mln_lvalue(I)
      ctree<I>::f (const node_t& n)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return this->data_->values_[n.index()];
      }

      template <typename I>
      inline
      typename ctree<I>::node_t
      ctree<I>::node(const mln_psite(I)& p) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	node_t tmp(*this, this->data_->map_(p));
	mln_postcondition(this->has(tmp));
	return tmp;
      }

      template <typename I>
      inline
      void
      ctree<I>::set_node(const mln_psite(I)& p, const node_t& n)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	mln_precondition(this->has(n));
	this->data_->map_(p) = n.index();
      }


      template <typename I>
      inline
      typename ctree<I>::node_t
      ctree<I>::parent(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	node_t tmp(*this, this->data_->parent_[n.index()]);
	mln_postcondition(tmp.index() <= n.index());
	mln_postcondition(this->has(tmp));
	return tmp;
      }

      template <typename I>
      inline
      void
      ctree<I>::set_parent(const node_t& n, const node_t& parent)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	mln_precondition(this->has(parent));
	this->data_->parent_[n.index()] = parent.index();
      }

      template <typename I>
      inline
      typename ctree<I>::nodes_t
      ctree<I>::desc(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	nodes_t tmp(*this, n, this->data_->length_[n.index()] + 1);
	mln_postcondition(tmp.nsites() >= 1);
	return tmp;
      }

      template <typename I>
      inline
      bool
      ctree<I>::is_a_root(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return (this->data_->parent_[n.index()] == n.index());
      }

      template <typename I>
      inline
      bool
      ctree<I>::is_a_leaf(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return (this->data_->length_[n.index()] == 0);
      }

      template <typename I>
      inline
      bool
      ctree<I>::has_index(index_t i) const
      {
	mln_precondition(this->is_valid());
	return 0 <= i && i <= this->n_nodes();
      }


      template <typename I>
      inline
      unsigned
      ctree<I>::length_(const node_t& node) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(node));
	return this->data_->length_[node.index()];
      }

      template <typename I>
      inline
      unsigned
      ctree<I>::length_at_(index_t i) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	return this->data_->length_[i];
      }

      template <typename I>
      inline
      unsigned&
      ctree<I>::length_at_(index_t i)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	return this->data_->length_[i];
      }



      template <typename I>
      inline
      mln_rvalue(I)
      ctree<I>::f_at_(index_t i) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	return this->data_->values_[i];
      }

      template <typename I>
      inline
      mln_lvalue(I)
      ctree<I>::f_at_(index_t i)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	return this->data_->values_[i];
      }

      template <typename I>
      inline
      typename ctree<I>::index_t
      ctree<I>::parent_at_(index_t i) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	mln_postcondition(this->has_index(this->data_->parent_[i]));
	return this->data_->parent_[i];
      }

      template <typename I>
      inline
      typename ctree<I>::index_t&
      ctree<I>::parent_at_(index_t i)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	mln_postcondition(this->has_index(this->data_->parent_[i]));
	return this->data_->parent_[i];
      }

      template <typename I>
      inline
      typename ctree<I>::index_t
      ctree<I>::node_at_(const psite& p) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	mln_postcondition(this->has_index(this->data_->map_(p)));
	return this->data_->map_(p);
      }

      template <typename I>
      inline
      typename ctree<I>::index_t&
      ctree<I>::node_at_(const psite& p)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	mln_postcondition(this->has_index(this->data_->map_(p)));
	return this->data_->map_(p);
      }

      template <typename I>
      inline
      attribute_image<ctree<I>, mln_value(I)>
      ctree<I>::to_image() const
      {
	mln_precondition(this->is_valid());

	attribute_image<ctree<I>, value> to(*this);
	this->to_image(to);
	return to;
      }

      template <typename I>
      inline
      void
      ctree<I>::to_image(attribute_image<ctree<I>, value>& a) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(a.is_valid());
	mln_precondition(a.tree().data_.ptr_ == this->data_.ptr_);

	std::copy(this->data_->values_.begin(), this->data_->values_.end(), a.buffer());
      }

    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_UTIL_CTREE_CTREE_HH
