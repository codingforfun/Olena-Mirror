// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_NODES_HH
# define MLN_CORE_SITE_SET_P_NODES_HH

/// \file
///
/// Definition of a run of nodes.
///
/// \todo Use a lazy approach (in subj) like in p_array psite.
///

# include <mln/util/ctree/node.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/pseudo_site_base.hh>

namespace mln
{

  // Forward declarations.
  template <typename T> class p_nodes;
  template <typename T> class p_nodes_psite;
  template <typename T> class p_nodes_fwd_piter_;
  template <typename T> class p_nodes_bkd_piter_;

  namespace trait
  {

    template <typename T>
    struct site_set_< p_nodes<T> >
    {
      typedef trait::site_set::nsites::known	nsites;
      typedef trait::site_set::bbox::unknown	bbox;
      typedef trait::site_set::contents::fixed	contents;
      typedef trait::site_set::arity::unique	arity;
    };

    template <typename T>
    struct set_precise_unary_< op::ord, p_nodes<T> >
    {
      typedef set_precise_unary_< op::ord, p_nodes<T> > ret; // Itself.
      bool strict(const p_nodes<T>& lhs, const p_nodes<T>& rhs) const;
    };

  } // end of namespace trait



  /// \brief Site set class in run.
  ///
  /// \ingroup modsitesetbasic
  template <typename T>
  class p_nodes : public internal::site_set_base_< util::ctree::node<T>, p_nodes<T> >
  {
  public:

    /// Element associated type.
    typedef util::ctree::node<T> element;

    /// Psite associated type.
    typedef p_nodes_psite<T> psite;

    /// Forward Site_Iterator associated type.
    typedef p_nodes_fwd_piter_<T> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_nodes_bkd_piter_<T> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without argument.
    p_nodes();

    /// Constructors.
    /// \{
    p_nodes(const Tree<T>& tree);
    /// Create the node set [\p start, \p start + \p len [
    p_nodes(const Tree<T>& tree, const element& start, unsigned len);
    /// Create the node set [\p start, \p end [
    p_nodes(const Tree<T>& tree, const element& start, const element& end);
    /// \}

    /// Init.
    /// \{
    void init(const Tree<T>& tree, const element& start, unsigned len);
    void init(const Tree<T>& tree, const element& start, const element& end);
    /// \}

    /// Test if \p p belongs to this site set.
    bool has(const psite& p) const;

    /// Test if \p p belongs to this site set.
    bool has(const element& p) const;

    /// Test if index \p i belongs to this site set.
    bool has_index(unsigned i) const;

    /// Give the number of sites.
    unsigned nsites() const;

    /// Return the \p i-th site.
    element operator[](int i) const;

    /// Return the starting site.
    const element& start() const;

    /// Return (compute) the ending site.
    element end() const;

    /// Test if this run is valid, i.e., with length >= 0.
    bool is_valid() const;

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Return the attached tree.
    const T& tree() const;

  protected:
    const T* tree_;

    /// The first site of the run.
    util::ctree::node<T> start_;

    /// The length of the run.
    unsigned len_;
  };

  template <typename T>
  class p_nodes_psite : public internal::pseudo_site_base_< const util::ctree::node<T>&, p_nodes_psite<T> >
  {
    typedef p_nodes_psite<T> self;
    typedef internal::pseudo_site_base_<const util::ctree::node<T>&, self> super;

  public:

    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_nodes<T> target;

    // As a Proxy:
    const util::ctree::node<T>& subj_();

    p_nodes_psite();
    p_nodes_psite(const p_nodes<T>& run, int i);

    /// Index materials
    /// \{
    int index() const;
    void change_index(int i);
    void inc_index();
    void dec_index();
    /// \}

    /// Run materials
    /// \{
    const p_nodes<T>* target_() const;
    const p_nodes<T>& run() const;
    void change_target(const p_nodes<T>& new_target);
    /// \}

    bool is_valid() const;

  private:
    const p_nodes<T>* run_;
    int i_;
    mutable util::ctree::node<T> p_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename T>
  inline
  p_nodes<T>::p_nodes() :
    tree_ (0), len_ (0)
  {
  }

  template <typename T>
  inline
  p_nodes<T>::p_nodes(const Tree<T>& tree)
    : tree_ (&(exact(tree))), len_ (0)
  {
    mln_precondition(tree_.is_valid());
  }

  template <typename T>
  inline
  p_nodes<T>::p_nodes(const Tree<T>& tree,
		      const util::ctree::node<T>& start,
		      unsigned len)
    : tree_ (&(exact(tree))), start_(start), len_ (len)
  {
    mln_precondition(tree_->is_valid() && start.is_valid());
    mln_postcondition(is_valid());
  }

  template <typename T>
  inline
  p_nodes<T>::p_nodes(const Tree<T>& tree,
		      const util::ctree::node<T>& start,
		      const util::ctree::node<T>& end)
    : tree_ (&(exact(tree))), start_(start)
  {
    mln_precondition(tree_.is_valid() && start.is_valid() && end.is_valid());
    mln_precondition(start.index() <= end.index());
    len_ = end.index() - start.index();
    mln_postcondition(is_valid());
  }

  template <typename T>
  inline
  void
  p_nodes<T>::init(const Tree<T>& tree, const util::ctree::node<T>& start, unsigned len)
  {
    tree_ = &(exact(tree));
    mln_precondition(start.is_valid());
    mln_precondition(tree_->is_valid());
    start_ = start;
    len_ = len;
    mln_postcondition(is_valid());
  }

  template <typename T>
  inline
  void
  p_nodes<T>::init(const Tree<T>& tree,
		   const util::ctree::node<T>& start,
		   const util::ctree::node<T>& end)
  {
    tree_ = &(exact(tree));
    mln_precondition(tree_->is_valid());
    mln_precondition(start.is_valid() && end.is_valid());
    mln_precondition(start.index() <= end.index());
    start_ = start;
    len_ = end.index() - start.index();
    mln_postcondition(is_valid());
  }

  template <typename T>
  inline
  bool
  p_nodes<T>::is_valid() const
  {
    return tree_->is_valid() && start_.is_valid();
  }

  template <typename T>
  inline
  bool
  p_nodes<T>::has(const psite& p) const
  {
    mln_precondition(p.target_() == this);
    return p.index() >= 0 && p.index() < (int)len_;
  }

  template <typename T>
  inline
  bool
  p_nodes<T>::has(const util::ctree::node<T>& node) const
  {
    mln_precondition(is_valid());
    return (this->start_.index() <= node.index()) && (node.index() < this->end().index());
  }

  template <typename T>
  inline
  bool
  p_nodes<T>::has_index(unsigned i) const
  {
    mln_precondition(is_valid());
    return i < len_;
  }

  template <typename T>
  inline
  unsigned
  p_nodes<T>::nsites() const
  {
    mln_precondition(is_valid());
    return len_;
  }

  template <typename T>
  inline
  const T&
  p_nodes<T>::tree() const
  {
      mln_precondition(is_valid());
      return *tree_;
  }

  template <typename T>
  inline
  util::ctree::node<T>
  p_nodes<T>::operator[](int i) const
  {
    mln_precondition(is_valid());
    mln_precondition(i >= 0);
    mln_precondition(i < len_);
    util::ctree::node<T> p = start_;
    p.change_index(start_.index() + i);
    return p;
  }

  template <typename T>
  inline
  const util::ctree::node<T>&
  p_nodes<T>::start() const
  {
    return start_;
  }

  template <typename T>
  inline
  util::ctree::node<T>
  p_nodes<T>::end() const
  {
    util::ctree::node<T> n = start_;
    n.change_index(start_.index() + len_);
    return n;
  }

  template <typename T>
  inline
  std::size_t
  p_nodes<T>::memory_size() const
  {
    return sizeof(*this);
  }

  template <typename T>
  std::ostream& operator<<(std::ostream& ostr, const p_nodes<T>& r)
  {
    ostr << '(' << r.start() << ", " << r.nsites() << ')';
    return ostr;
  }

  // p_nodes_psite<P>

  template <typename T>
  inline
  p_nodes_psite<T>::p_nodes_psite()
    : run_(0),
      i_(0)
  {
  }

  template <typename T>
  inline
  p_nodes_psite<T>::p_nodes_psite(const p_nodes<T>& run, int i)
    : run_(&run),
      i_(i),
      p_(run.start())
  {
    p_.change_index(p_.index() + i_);
  }

  template <typename T>
  inline
  bool
  p_nodes_psite<T>::is_valid() const
  {
    return run_ != 0 && run_->has_index(i_);
  }

  template <typename T>
  inline
  int
  p_nodes_psite<T>::index() const
  {
    return i_;
  }

  template <typename T>
  inline
  void
  p_nodes_psite<T>::change_index(int i)
  {
    p_.change_index(run_->start().index() + i);
    i_ = i;
  }

  template <typename T>
  inline
  void
  p_nodes_psite<T>::dec_index()
  {
    --i_;
    p_.dec_index();
  }

  template <typename T>
  inline
  void
  p_nodes_psite<T>::inc_index()
  {
    ++i_;
    p_.inc_index();
  }

  template <typename T>
  inline
  const p_nodes<T>*
  p_nodes_psite<T>::target_() const
  {
    return run_;
  }

  template <typename T>
  inline
  void
  p_nodes_psite<T>::change_target(const p_nodes<T>& new_target)
  {
    run_ = &new_target;
    i_ = 0;
    p_ = run_->start();
  }

  template <typename T>
  inline
  const util::ctree::node<T>&
  p_nodes_psite<T>::subj_()
  {
    return p_;
  }

  template <typename T>
  inline
  const p_nodes<T>&
  p_nodes_psite<T>::run() const
  {
    mln_precondition(run_ != 0);
    return *run_;
  }

  namespace trait
  {

    template <typename T>
    inline
    bool
    set_precise_unary_< op::ord, p_nodes<T> >::strict(const p_nodes<T>& lhs, const p_nodes<T>& rhs) const
    {
      return util::ord_strict(lhs.start().index(), rhs.start().index());
    }

  } // end of namespace trait

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/site_set/p_nodes_piter.hh>


#endif // ! MLN_CORE_SITE_SET_P_NODES_HH
