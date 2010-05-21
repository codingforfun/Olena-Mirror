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

#ifndef MLN_UTIL_CTREE_NODE_HH
# define MLN_UTIL_CTREE_NODE_HH

# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/site.hh>
# include <mln/core/internal/pseudo_site_base.hh>

namespace mln
{

  /// Node category flag type.
  template <typename E> struct Node {};

  template <>
  struct Node<void>
  {
    typedef Site<void> super;
  };

  /// Fwd declaration.
  namespace util
  {
    namespace ctree
    {
       template <typename T>
       class node;
    }

  }

  namespace trait
  {
    template <typename T>
    struct set_precise_unary_< op::ord, util::ctree::node<T> >
    {
      typedef set_precise_unary_< op::ord, util::ctree::node<T> > ret; // Itself.
      bool strict(const util::ctree::node<T>& lhs, const util::ctree::node<T>& rhs) const;
    };

  } // end of namespace trait

  namespace util
  {

    namespace ctree
    {

      template <typename T>
      class node
      {
      public:
	/// Object category.
	typedef Node<void> category;

	/// The node index.
	typedef int index_t;

	/// Tree associated type.
	typedef T tree_t;

	/// FIXME:
	typedef int delta;

	/// Constructors
	/// \{
	node();
	explicit node(const Tree<T>& tree);
	node(const Tree<T>& tree, int idx);
	/// \}

	/// Misc
	/// \{
	/// Return whether the node is valid.
	bool is_valid() const;

	/// Index manipulation.
	/// \{
	int index() const;
	void inc_index();
	void dec_index();
	void change_index(int idx);
	/// \}

	/// Return a reference to the graph holding this node.
	const T& tree() const;

	/// Change target.
	void change_tree(const Tree<T>& tree);
	/// \}


      private:
	const T*	tree_;
	int		idx_;
      };

      /*
      ** Operators.
      */
      template <typename T>
      bool
      operator== (const node<T>& n1, const node<T>& n2);

      template <typename T>
      bool
      operator!= (const node<T>& n1, const node<T>& n2);

      /// Test if the node \p n1 is a descendant of \p n2
      ///
      template <typename T>
      bool
      operator< (const node<T>& n1, const node<T>& n2);

    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

 
  namespace internal
  {

    /// subject_impl specialization (Proxy)
    /// \{

    template <typename T, typename E>
    struct subject_impl< const util::ctree::node<T>, E >
    {
      int index() const;
      const T& tree() const;

    private:
      const E& exact_() const;
    };

    template <typename T, typename E>
    struct subject_impl< util::ctree::node<T>, E > :
      subject_impl< const util::ctree::node<T>, E >
    {
      void change_index(int idx);

    private:
      E& exact_();
    };

    /// \}

  } // end of namespace mln::internal

# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    namespace ctree
    {

      template <typename T>
      inline
      node<T>::node()
	: tree_ (0)
      {
      }

      template <typename T>
      inline
      node<T>::node(const Tree<T>& tree)
	: tree_ (&(exact(tree)))
      {
      }


      template <typename T>
      inline
      node<T>::node(const Tree<T>& tree, int idx)
	: tree_ (&(exact(tree))),
	  idx_ (idx)
      {
	mln_precondition(tree_->is_valid());
      }

      template <typename T>
      inline
      int
      node<T>::index() const
      {
	return idx_;
      }

      template <typename T>
      inline
      void
      node<T>::inc_index()
      {
	++idx_;
      }

      template <typename T>
      inline
      void
      node<T>::dec_index()
      {
	--idx_;
      }

      template <typename T>
      inline
      void
      node<T>::change_index(int idx)
      {
	idx_ = idx;
      }

      template <typename T>
      inline
      const T&
      node<T>::tree() const
      {
	mln_precondition(this->is_valid());
	return *tree_;
      }

      template <typename T>
      inline
      void
      node<T>::change_tree(const Tree<T>& tree)
      {
	tree_ = &(exact(tree));
      }

      template <typename T>
      inline
      bool
      node<T>::is_valid() const
      {
	return tree_ != 0 && tree_->is_valid();
      }

      /*
      ** Operators
      */
      template <typename T>
      inline
      bool
      operator== (const node<T>& n1, const node<T>& n2)
      {
	return n1.index() == n2.index();
      }

      template <typename T>
      inline
      bool
      operator!= (const node<T>& n1, const node<T>& n2)
      {
	return n1.index() != n2.index();
      }

      template <typename T>
      inline
      bool
      operator< (const node<T>& n1, const node<T>& n2)
      {
	return n1.index() > n2.index();
      }

    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

  namespace internal
  {

    /*-----------------------------------------`
    | subject_impl< const util::ctree::node<G> |
    \-----------------------------------------*/

    template <typename T, typename E>
    inline
    const E&
    subject_impl< const util::ctree::node<T>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename T, typename E>
    inline
    int
    subject_impl< const util::ctree::node<T>, E >::index() const
    {
      return exact_().get_subject().index();
    }

    template <typename T, typename E>
    inline
    const T&
    subject_impl< const util::ctree::node<T>, E >::tree() const
    {
      return exact_().get_subject().tree();
    }

   /*------------------------------------------`
    | subject_impl<       util::ctree::node<T> |
    \-----------------------------------------*/

    template <typename T, typename E>
    inline
    E&
    subject_impl< util::ctree::node<T>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

    template <typename T, typename E>
    inline
    void
    subject_impl< util::ctree::node<T>, E >::change_index(int idx)
    {
      return exact_().get_subject().change_index(idx);
    }

  } // end of namespace mln::internal

  namespace trait
  {

    template <typename T>
    inline
    bool
    set_precise_unary_< op::ord, util::ctree::node<T> >::strict(const util::ctree::node<T>& lhs, const util::ctree::node<T>& rhs) const
    {
      return util::ord_strict(lhs.index(), rhs.index());
    }

  } // end of namespace trait


# endif // !MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_UTIL_CTREE_NODE_HH
