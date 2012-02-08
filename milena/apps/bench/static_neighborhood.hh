// Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_STATIC_NEIGHBORHOOD_HH
# define MLN_CORE_STATIC_NEIGHBORHOOD_HH

// FIXME: Review and update the documentation.

/*! \file
 *
 * \brief Definition of the generic neighborhood class mln::neighborhood.
 *
 * \todo Make naming coherent: we have window (without '_') but
 * point_, neighb_, etc.
 *
 * \todo Code other comparisons (< and <=).
 *
 * \todo Add static checks in insert methods.
 *
 * \todo Rename dps_hook_ as util_set_.
 */

# include <mln/core/concept/gdpoint.hh>

# include <mln/metal/is_a.hh>
# include <mln/util/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/norm/linfty.hh>
# include <mln/literal/zero.hh>

# include "apps/bench/static_array.hh"
# include "apps/bench/static_window.hh"


namespace mln
{

  // Forward declarations.
  template <typename D, unsigned n> class static_neighborhood;
  template <typename V> class dpsites_fwd_piter;
  template <typename V> class dpsites_bkd_piter;



  /*! \brief Generic neighborhood class.
   *
   * This type of neighborhood is just like a set of delta-points.  The
   * parameter is \c D, type of delta-point.
   */
  template <typename D, unsigned n>
  class static_neighborhood
    : public Neighborhood< static_neighborhood<D, n> >
  {
  public:

    enum { length = n };

    /// Window associated type.
    typedef static_window<D, n> window;

    /// Get the corresponding window.
    window win() const;

    static_neighborhood(const mln::util::static_array<D, n>& dps);

    /*! \brief Site_Iterator type to browse the points of a basic neighborhood
     * w.r.t. the ordering of delta-points.
     */
    typedef dpsites_fwd_piter< static_neighborhood<D, n> > fwd_niter;

    /*! \brief Site_Iterator type to browse the points of a basic neighborhood
     * w.r.t. the reverse ordering of delta-points.
     */
    typedef dpsites_bkd_piter< static_neighborhood<D, n> > bkd_niter;

    /*! \brief Site_Iterator type to browse the points of a basic neighborhood
     * whatever the ordering of delta-points.
     */
    typedef fwd_niter niter;

    /// Give the neighborhood size, i.e., the number of delta-sites.
    unsigned size() const;

    /*! \brief Test if the neighborhood is empty (null size; no delta-point).
     */
    bool is_empty() const;

    // /// Clear the neighborhood.
    // void clear();

    /*! \brief Give the maximum coordinate gap between the neighborhood
      center and a neighborhood point.
    */
    unsigned delta() const;

    /// Give the \p i-th delta-point.
    const D& dp(unsigned i) const;

    /// Test if \p dp is in this neighborhood definition.
    bool has(const D& dp) const;

    // /// Insert a delta-point \p dp.
    // static_neighborhood<D, n>& insert(const D& dp);

    // /// Insert another neighborhood \p nbh.
    // template <typename W>
    // static_neighborhood<D, n>& insert(const Neighborhood<W>& nbh);

    // /// \{ Insertion of a delta-point with different numbers of
    // /// arguments (coordinates) w.r.t. the dimension.
    // static_neighborhood<D, n>& insert(const mln_coord(D)& dind); // For 1D or index access.

    // static_neighborhood<D, n>& insert(const mln_coord(D)& drow,
    // 		      const mln_coord(D)& dcol); // For 2D.

    // static_neighborhood<D, n>& insert(const mln_coord(D)& dsli,
    // 		      const mln_coord(D)& drow,
    // 		      const mln_coord(D)& dcol); // For 3D.
    /// \}


    /// Hook to the set of D.
    const mln::util::static_array<D, n>& dps_hook_() const;

    /// Return true by default.
    bool is_valid() const;

    /// Print the neighborhood definition into \p ostr.
    void print(std::ostream& ostr) const;

  private:

    mln::util::static_array<D, n> dps_;

    unsigned delta_(int i) const;                // For indices.
    unsigned delta_(const Gdpoint<D>& dp) const; // For grids delta-points.
  };



  /*! \brief Equality comparison between neighborhoods \p lhs and \p rhs.
   *
   * \relates mln::neighborhood
   */
  template <typename D, unsigned n>
  bool operator==(const static_neighborhood<D, n>& lhs, const static_neighborhood<D, n>& rhs);



# ifndef MLN_INCLUDE_ONLY

  // static_neighborhood<D, n>

  template <typename D, unsigned n>
  inline
  static_neighborhood<D, n>::static_neighborhood(const mln::util::static_array<D, n>& dps)
    : dps_(dps)
  {
    // FIXME HERE: Was: mln::metal::is_a<D, Dpoint>::check();
    // mln::metal::is_a<D, Delta_Point_Site>::check();
  }

  template <typename D, unsigned n>
  inline
  static_window<D, n>
  static_neighborhood<D, n>::win() const
  {
    return static_window<D, n>(dps_);
  }

  template <typename D, unsigned n>
  inline
  bool
  static_neighborhood<D, n>::is_empty() const
  {
    return n == 0;
  }

  // template <typename D, unsigned n>
  // inline
  // void
  // static_neighborhood<D, n>::clear()
  // {
  //   dps_.clear();
  // }

  template <typename D, unsigned n>
  inline
  unsigned
  static_neighborhood<D, n>::delta() const
  {
    unsigned d = 0;
    for (unsigned i = 0; i < n; ++i)
      {
	unsigned dd = delta_(dp(i));
	if (dd > d)
	  d = dd;
      }
    return d;
  }

  template <typename D, unsigned n>
  inline
  unsigned
  static_neighborhood<D, n>::delta_(int i) const
  {
    return i;
  }

  template <typename D, unsigned n>
  inline
  unsigned
  static_neighborhood<D, n>::delta_(const Gdpoint<D>& dp) const
  {
    return norm::linfty(exact(dp).to_vec());
  }

  template <typename D, unsigned n>
  inline
  unsigned
  static_neighborhood<D, n>::size() const
  {
    return n;
  }

  template <typename D, unsigned n>
  inline
  const D&
  static_neighborhood<D, n>::dp(unsigned i) const
  {
    mln_precondition(i < n);
    return dps_[i];
  }

  template <typename D, unsigned n>
  inline
  bool
  static_neighborhood<D, n>::has(const D& dp) const
  {
    return dps_.has(dp);
  }

  // template <typename D, unsigned n>
  // inline
  // static_neighborhood<D, n>&
  // static_neighborhood<D, n>::insert(const D& dp)
  // {
  //   dps_.insert(dp);
  //   return *this;
  // }

  // template <typename D, unsigned n>
  // template <typename W>
  // inline
  // static_neighborhood<D, n>&
  // static_neighborhood<D, n>::insert(const Neighborhood<W>& nbh_)
  // {
  //   const W& nbh = exact(nbh_);
  //   const unsigned n = nbh.size();
  //   for (unsigned i = 0; i < n; ++i)
  //     dps_.insert(nbh.dp(i));
  //   return *this;
  // }

  // template <typename D, unsigned n>
  // inline
  // static_neighborhood<D, n>&
  // static_neighborhood<D, n>::insert(const mln_coord(D)& dind)
  // {
  //   mlc_bool(D::dim == 1)::check();
  //   D dp(dind);
  //   return insert(dp);
  // }

  // template <typename D, unsigned n>
  // inline
  // static_neighborhood<D, n>&
  // static_neighborhood<D, n>::insert(const mln_coord(D)& drow,
  // 		    const mln_coord(D)& dcol)
  // {
  //   mlc_bool(D::dim == 2)::check();
  //   D dp(drow, dcol);
  //   return insert(dp);
  // }

  // template <typename D, unsigned n>
  // inline
  // static_neighborhood<D, n>&
  // static_neighborhood<D, n>::insert(const mln_coord(D)& dsli,
  // 		    const mln_coord(D)& drow,
  // 		    const mln_coord(D)& dcol)
  // {
  //   mlc_bool(D::dim == 3)::check();
  //   D dp(dsli, drow, dcol);
  //   return insert(dp);
  // }

  template <typename D, unsigned n>
  inline
  const mln::util::static_array<D, n>&
  static_neighborhood<D, n>::dps_hook_() const
  {
    return dps_;
  }

  template <typename D, unsigned n>
  inline
  bool
  static_neighborhood<D, n>::is_valid() const
  {
    return true;
  }

  template <typename D, unsigned n>
  inline
  void
  static_neighborhood<D, n>::print(std::ostream& ostr) const
  {
    ostr << dps_;
  }


  // Operators.

  template <typename D, unsigned n>
  bool
  operator==(const static_neighborhood<D, n>& lhs, const static_neighborhood<D, n>& rhs)
  {
    return lhs.dps_hook_() == rhs.dps_hook_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/dpsites_piter.hh>


#endif // ! MLN_CORE_STATIC_NEIGHBORHOOD_HH
