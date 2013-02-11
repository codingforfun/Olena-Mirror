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

#ifndef MLN_UTIL_MAP_HH
# define MLN_UTIL_MAP_HH

/// \file
///
/// Definition of mln::util::map.


# include <map>
# include <iostream>
# include <algorithm>

# include <mln/core/alias/box1d.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/iterator.hh>

# include <mln/fun/internal/selector.hh>

# include <mln/util/ord.hh>


namespace mln
{

  // Forward declarations.
  namespace fun {
    namespace i2v {
      template <typename K, typename V> class map;
    }
  }
  template <typename V> struct image1d;
  // End of forward declarations


  namespace util
  {

    // Forward declarations.
    template <typename K, typename V> class map_fwd_iter;
    template <typename K, typename V> class map_bkd_iter;


    /// \brief A dynamic map class.
    ///
    /// Elements are stored by copy.  Implementation is lazy.
    ///
    /// The parameter \c T is the element type, which shall not be
    /// const-qualified.
    ///
    /// \ingroup modutil
    //
    template <typename K, typename V>
    class map
      : public mln::fun::internal::selector_from_result_<V, map<K,V> >::ret
    {
    public:

      /// Element associated type.
      typedef K element;

      /// Returned value types.
      /// Related to the Function_v2v concept.
      /// @{
      typedef V result;
      typedef const V& ro_result;
      typedef V& mutable_result;
      /// @}


      /// Iterator types
      /// @{
      /// Forward iterator associated type.
      typedef map_fwd_iter<K,V> fwd_eiter;

      /// Backward iterator associated type.
      typedef map_bkd_iter<K,V> bkd_eiter;

      /// Iterator associated type.
      typedef fwd_eiter eiter;
      /// @}


      /// Constructors
      /// @{
      /// Constructor without arguments.
      map();

      /// Remove element mapped to key \p key.
      void erase(const K& key);

      /// Return the number of elements of the map.
      unsigned nelements() const;

      /// Return the number of elements of the map.
      /// \sa nelements
      unsigned size() const;

      /// Test if the map is empty.
      bool is_empty() const;


      /// \brief Return the value mapped to the key \p key.
      /// \pre i < nelements()
      ro_result operator()(const K& key) const;

      /// \brief Return the value mapped to the key \p key.
      /// \pre i < nelements()
      mutable_result operator()(const K& key);

      /// \brief Return the value mapped to the key \p key.
      /// \pre i < nelements()
      ro_result operator[](const K& key) const;

      /// \brief Return the value mapped to the key \p key.
      /// \pre i < nelements()
      mutable_result operator[](const K& key);

      /// \brief Return true if \p key is in this map.
      bool has(const K& key) const;

      /// Empty the map.  All elements contained in the map are
      /// destroyed.  \post is_empty() == true
      void clear();

      /// Return the corresponding std::map of elements.
      const std::map<K,V,util::ord<K> >& std_map() const;

      /// Hook to the mutable std::map of elements.
      std::map<K,V,util::ord<K> >& hook_std_map_();

    private:
      mutable std::map<K,V,util::ord<K> > m_;
    };


    /// Operator<<.
    template <typename K, typename V>
    std::ostream& operator<<(std::ostream& ostr,
			     const map<K,V>& a);

    /// Operator==
    template <typename K, typename V>
    bool operator==(const map<K,V>& lhs,
		    const map<K,V>& rhs);


    // map_fwd_iter<K,V>

    template <typename K, typename V>
    class map_fwd_iter : public Proxy< map_fwd_iter<K,V> >,
			 public mln::internal::proxy_impl< const std::pair<K,V>,
							   map_fwd_iter<K,V> >
    {
    public:
      typedef const std::pair<K,V> subj_t;

      /// Constructors
      /// @{
      /// Constructor without argument.
      map_fwd_iter();

      /// Constructor from an map \p a.
      map_fwd_iter(const map<K,V>& a);
      /// @}

      /// Change the map it iterates on to \p a.
      void change_target(const map<K,V>& a);

      /// Start an iteration.
      void start();

      /// Go to the next element.
      void next();

      /// Returns true if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Give the element the iterator designates.
      subj_t element() const;

      // As a Proxy.
      subj_t subj_();

      /// Give the current index.
      unsigned index_() const;

    protected:
      typename std::map<K,V,util::ord<K> >::const_iterator i_;
      const std::map<K,V,util::ord<K> >* a_;
    };




    // map_bkd_iter<K,V>

    template <typename K, typename V>
    class map_bkd_iter : public Proxy< map_bkd_iter<K,V> >,
			 public mln::internal::proxy_impl< const std::pair<K,V>,
							   map_bkd_iter<K,V> >
    {
    public:
      typedef const std::pair<K,V> subj_t;

      /// Constructors
      /// @{
      /// Constructor without argument.
      map_bkd_iter();

      /// Constructor from an map \p a.
      map_bkd_iter(const map<K,V>& a);
      /// @}

      /// Change the map it iterates on to \p a.
      void change_target(const map<K,V>& a);

      /// Start an iteration.
      void start();

      /// Go to the next element.
      void next();

      /// Returns true if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Give the element the iterator designates.
      subj_t element() const;

      // As a Proxy.
      subj_t subj_();

      /// Give the current index.
      unsigned index_() const;

    protected:
      typename std::map<K,V,util::ord<K> >::const_reverse_iterator i_;
      const std::map<K,V,util::ord<K> >* a_;
    };

  } // end of namespace mln::util


  namespace internal
  {

    template <typename K, typename V, typename E>
    struct subject_impl<const util::map<K,V>, E>
    {
      typedef typename util::map<K,V>::ro_result ro_result;

      ro_result operator()(const K& key) const;
      ro_result operator[](const K& key) const;

      unsigned nelements() const;
      unsigned size() const;
      bool is_empty() const;
      const std::map<K,V,util::ord<K> > std_map() const;

    private:
      const E& exact_() const;
    };


    template <typename K, typename V, typename E>
    struct subject_impl<util::map<K,V>, E>
      : subject_impl<const util::map<K,V>, E>
    {
      typedef typename util::map<K,V>::mutable_result mutable_result;

      mutable_result operator()(const K& key);
      mutable_result operator[](const K& key);

      void clear();

      std::map<K,V,util::ord<K> > hook_std_map_();

    private:
      E& exact_();
    };


  } // end of namespace mln::internal


# ifndef MLN_INCLUDE_ONLY


  namespace util
  {

    // util::map<K,V>


    template <typename K, typename V>
    inline
    map<K,V>::map()
    {
    }

    template <typename K, typename V>
    inline
    void
    map<K,V>::clear()
    {
      m_.clear();
      mln_postcondition(is_empty());
    }

    template <typename K, typename V>
    inline
    unsigned
    map<K,V>::size() const
    {
      return nelements();
    }

    template <typename K, typename V>
    inline
    unsigned
    map<K,V>::nelements() const
    {
      return m_.size();
    }

    template <typename K, typename V>
    inline
    typename map<K,V>::ro_result
    map<K,V>::operator()(const K& key) const
    {
//      mln_precondition(has(key));
      return (*this)[key];
    }

    template <typename K, typename V>
    inline
    typename map<K,V>::mutable_result
    map<K,V>::operator()(const K& key)
    {
//      mln_precondition(has(key));
      return (*this)[key];
    }

    template <typename K, typename V>
    inline
    typename map<K,V>::ro_result
    map<K,V>::operator[](const K& key) const
    {
//      mln_precondition(has(key));
      return m_[key];
    }

    template <typename K, typename V>
    inline
    typename map<K,V>::mutable_result
    map<K,V>::operator[](const K& key)
    {
//      mln_precondition(has(key));
      return m_[key];
    }

    template <typename K, typename V>
    inline
    void
    map<K,V>::erase(const K& key)
    {
      mln_precondition(has(key));
      m_.erase(key);
    }

    template <typename K, typename V>
    inline
    bool
    map<K,V>::has(const K& key) const
    {
      return m_.find(key) != m_.end();
    }

    template <typename K, typename V>
    inline
    bool
    map<K,V>::is_empty() const
    {
      return nelements() == 0;
    }

    template <typename K, typename V>
    inline
    const std::map<K,V,util::ord<K> >&
    map<K,V>::std_map() const
    {
      return m_;
    }

    template <typename K, typename V>
    inline
    std::map<K,V,util::ord<K> >&
    map<K,V>::hook_std_map_()
    {
      return m_;
    }


    // util::map_fwd_iter<K,V>


    template <typename K, typename V>
    inline
    map_fwd_iter<K,V>::map_fwd_iter()
    {
      a_ = 0;
    }

    template <typename K, typename V>
    inline
    map_fwd_iter<K,V>::map_fwd_iter(const map<K,V>& a)
    {
      change_target(a);
    }

    template <typename K, typename V>
    inline
    void
    map_fwd_iter<K,V>::change_target(const map<K,V>& a)
    {
      a_ = &a.std_map();
      invalidate();
    }

    template <typename K, typename V>
    inline
    void
    map_fwd_iter<K,V>::start()
    {
      mln_precondition(a_ != 0);
      i_ = a_->begin();
    }

    template <typename K, typename V>
    inline
    void
    map_fwd_iter<K,V>::next()
    {
      mln_precondition(is_valid());
      ++i_;
    }

    template <typename K, typename V>
    inline
    bool
    map_fwd_iter<K,V>::is_valid() const
    {
      return i_ != a_->end();
    }

    template <typename K, typename V>
    inline
    void
    map_fwd_iter<K,V>::invalidate()
    {
      if (a_ != 0)
	i_ = a_->end();
      mln_postcondition(! is_valid());
    }

    template <typename K, typename V>
    inline
    typename map_fwd_iter<K,V>::subj_t
    map_fwd_iter<K,V>::element() const
    {
      mln_precondition(is_valid());
      return *i_;
    }

    template <typename K, typename V>
    inline
    typename map_fwd_iter<K,V>::subj_t
    map_fwd_iter<K,V>::subj_()
    {
      mln_precondition(is_valid());
      return *i_;
    }



    // util::map_bkd_iter<K,V>


    template <typename K, typename V>
    inline
    map_bkd_iter<K,V>::map_bkd_iter()
    {
      a_ = 0;
    }

    template <typename K, typename V>
    inline
    map_bkd_iter<K,V>::map_bkd_iter(const map<K,V>& a)
    {
      change_target(a);
    }

    template <typename K, typename V>
    inline
    void
    map_bkd_iter<K,V>::change_target(const map<K,V>& a)
    {
      a_ = &a.std_map();
      invalidate();
    }

    template <typename K, typename V>
    inline
    void
    map_bkd_iter<K,V>::start()
    {
      mln_precondition(a_ != 0);
      if (! a_->empty())
	i_ = a_->rbegin();
    }

    template <typename K, typename V>
    inline
    void
    map_bkd_iter<K,V>::next()
    {
      mln_precondition(is_valid());
      ++i_;
    }

    template <typename K, typename V>
    inline
    bool
    map_bkd_iter<K,V>::is_valid() const
    {
      return i_ != a_->rend();
    }

    template <typename K, typename V>
    inline
    void
    map_bkd_iter<K,V>::invalidate()
    {
      if (a_ != 0)
	i_ = a_->rend();
      mln_postcondition(! is_valid());
    }

    template <typename K, typename V>
    inline
    typename map_bkd_iter<K,V>::subj_t
    map_bkd_iter<K,V>::element() const
    {
      mln_precondition(is_valid());
      return *i_;
    }

    template <typename K, typename V>
    inline
    typename map_bkd_iter<K,V>::subj_t
    map_bkd_iter<K,V>::subj_()
    {
      mln_precondition(is_valid());
      return *i_;
    }


    // Operator <<.

    template <typename K, typename V>
    std::ostream& operator<<(std::ostream& ostr,
			     const map<K,V>& a)
    {
      ostr << '{';
      typedef map<K,V> M;
      mln_eiter(M) e(a);
      unsigned i = 0;
      for_all(e)
      {
	ostr << "(" << e.first << "," << e.second << ")";
	if (i != e.nelements())
	  ostr << ", ";
	++i;
      }
      ostr << '}';
      return ostr;
    }


    // Operator <<.

    template <typename K, typename V>
    bool operator==(const map<K,V>& lhs,
		    const map<K,V>& rhs)
    {
      return lhs.std_map() == rhs.std_map();
    }

  } // end of namespace mln::util


  namespace internal
  {

    template <typename K, typename V, typename E>
    inline
    typename subject_impl<util::map<K,V>, E>::mutable_result
    subject_impl<util::map<K,V>, E>::operator()(const K& key)
    {
      return exact_().get_subject()(key);
    }

    template <typename K, typename V, typename E>
    inline
    typename subject_impl<util::map<K,V>, E>::mutable_result
    subject_impl<util::map<K,V>, E>::operator[](const K& key)
    {
      return exact_().get_subject()[key];
    }

    template <typename K, typename V, typename E>
    inline
    void
    subject_impl<util::map<K,V>, E>::clear()
    {
      exact_().get_subject().clear();
    }

    template <typename K, typename V, typename E>
    inline
    std::map<K,V,util::ord<K> >
    subject_impl<util::map<K,V>, E>::hook_std_map_()
    {
      return exact_().get_subject().hook_std_map_();
    }

    template <typename K, typename V, typename E>
    inline
    E&
    subject_impl<util::map<K,V>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }


    template <typename K, typename V, typename E>
    inline
    unsigned
    subject_impl<const util::map<K,V>, E>::size() const
    {
      return exact_().get_subject().size();
    }

    template <typename K, typename V, typename E>
    inline
    unsigned
    subject_impl<const util::map<K,V>, E>::nelements() const
    {
      return exact_().get_subject().nelements();
    }

    template <typename K, typename V, typename E>
    inline
    bool
    subject_impl<const util::map<K,V>, E>::is_empty() const
    {
      return exact_().get_subject().is_empty();
    }

    template <typename K, typename V, typename E>
    inline
    typename subject_impl<const util::map<K,V>, E>::ro_result
    subject_impl<const util::map<K,V>, E>::operator()(const K& key) const
    {
      return exact_().get_subject()(key);
    }

    template <typename K, typename V, typename E>
    inline
    typename subject_impl<const util::map<K,V>, E>::ro_result
    subject_impl<const util::map<K,V>, E>::operator[](const K& key) const
    {
      return exact_().get_subject()[key];
    }

    template <typename K, typename V, typename E>
    inline
    const std::map<K,V,util::ord<K> >
    subject_impl<const util::map<K,V>, E>::std_map() const
    {
      return exact_().get_subject().std_map();
    }

    template <typename K, typename V, typename E>
    inline
    const E&
    subject_impl<const util::map<K,V>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }


  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_UTIL_MAP_HH
