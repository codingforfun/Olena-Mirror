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

#ifndef MLN_CORE_SITE_SET_P_NODES_PITER_HH
# define MLN_CORE_SITE_SET_P_NODES_PITER_HH

/*! \file
 *
 * \brief Definition of point iterators on mln::p_nodes.
 */

# include <mln/core/site_set/p_nodes.hh>
# include <mln/core/internal/site_set_iterator_base.hh>


namespace mln
{

  /*! \brief Forward iterator on points of a p_nodes<T>.
   *
   */
  template <typename T>
  class p_nodes_fwd_piter_
    :
    public internal::site_set_iterator_base< p_nodes<T>,
					     p_nodes_fwd_piter_<T> >
  {
    typedef p_nodes_fwd_piter_<T> self_;
    typedef internal::site_set_iterator_base< p_nodes<T>, self_ > super_;
  public:

    /// Constructor without arguments.
    p_nodes_fwd_piter_();

    /// Coordinate associated type.
    p_nodes_fwd_piter_(const p_nodes<T>& r);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Go to the brother.
    void go_to_brother();

    /// Go to
    void go_to(const typename T::node_t& node);

  protected:
    using super_::p_;
    using super_::s_;
  };



  /*! \brief Backward iterator on points of a p_nodes<T>.
   *
   */
  template <typename T>
  class p_nodes_bkd_piter_
    :
    public internal::site_set_iterator_base< p_nodes<T>,
					     p_nodes_bkd_piter_<T> >
  {
    typedef p_nodes_bkd_piter_<T> self_;
    typedef internal::site_set_iterator_base< p_nodes<T>, self_ > super_;
  public:

    /// Constructor without arguments.
    p_nodes_bkd_piter_();

    /// Coordinate associated type.
    p_nodes_bkd_piter_(const p_nodes<T>& r);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

  protected:
    using super_::p_;
    using super_::s_;
  };



# ifndef MLN_INCLUDE_ONLY

  // p_nodes_fwd_piter_<T>

  template <typename T>
  inline
  p_nodes_fwd_piter_<T>::p_nodes_fwd_piter_()
  {
  }

  template <typename T>
  inline
  p_nodes_fwd_piter_<T>::p_nodes_fwd_piter_(const p_nodes<T>& r)
  {
    this->change_target(r);
  }

  template <typename T>
  inline
  bool
  p_nodes_fwd_piter_<T>::is_valid_() const
  {
    mln_invariant(p_.index() >= 0);
    return p_.index() < int(s_->nsites());
  }

  template <typename T>
  inline
  void
  p_nodes_fwd_piter_<T>::invalidate_()
  {
    p_.change_index(s_->nsites());
  }

  template <typename T>
  inline
  void
  p_nodes_fwd_piter_<T>::start_()
  {
    p_.change_index(0);
  }

  template <typename T>
  inline
  void
  p_nodes_fwd_piter_<T>::next_()
  {
    p_.inc_index();
  }

  template <typename T>
  inline
  void
  p_nodes_fwd_piter_<T>::go_to_brother()
  {
    unsigned l = s_->tree().length_(p_.subj_());
    p_.change_index(p_.index() + l);
  }

  template <typename T>
  inline
  void
  p_nodes_fwd_piter_<T>::go_to(const typename T::node_t& node)
  {
    p_.change_index(node.index());
  }

  // p_nodes_bkd_piter_<T>

  template <typename T>
  inline
  p_nodes_bkd_piter_<T>::p_nodes_bkd_piter_()
  {
  }

  template <typename T>
  inline
  p_nodes_bkd_piter_<T>::p_nodes_bkd_piter_(const p_nodes<T>& r)
  {
    this->change_target(r);
  }

  template <typename T>
  inline
  bool
  p_nodes_bkd_piter_<T>::is_valid_() const
  {
    mln_invariant(p_.index() < int(s_->nsites()));
    return p_.index() >= 0;
  }

  template <typename T>
  inline
  void
  p_nodes_bkd_piter_<T>::invalidate_()
  {
    p_.change_index(-1);
  }

  template <typename T>
  inline
  void
  p_nodes_bkd_piter_<T>::start_()
  {
    p_.change_index(s_->nsites() - 1);
  }

  template <typename T>
  inline
  void
  p_nodes_bkd_piter_<T>::next_()
  {
    p_.dec_index();
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_NODES_PITER_HH
