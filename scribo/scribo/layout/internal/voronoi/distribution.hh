// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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
/// \brief Routines used in Voronoi algorithm.


#ifndef SCRIBO_LAYOUT_INTERNAL_VORONOI_DISTRIBUTION_HH
# define SCRIBO_LAYOUT_INTERNAL_VORONOI_DISTRIBUTION_HH

# include <climits>
# include <cstdlib>
# include <vector>
# include <map>
# include <mln/util/ord_pair.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace voronoi
      {
	using namespace mln;

	void compute_peaks(const std::vector<int>& distribution,
			   const float t,
			   int& td1,
			   int& td2);

	void compute_distribution(const int max_distance,
				  const std::map<util::ord_pair<unsigned>,
						 int>& distances,
				  const float t,
				  int& td1,
				  int& td2);

# ifndef MLN_INCLUDE_ONLY

	inline
	void compute_peaks(const std::vector<int>& distribution,
			   const float t,
			   int& td1,
			   int& td2)
	{
	  const int nelements = distribution.size();
	  int peak_index1 = 0;
	  int peak_index2 = 0;
	  int peak_value1 = 0;
	  int peak_value2 = 0;

	  for (int i = 0; i < nelements; ++i)
	  {
	    const int index_min = i - 1;
	    const int index_max = i + 1;

	    if (index_min < 0 || index_max >= nelements)
	      continue;

	    if (distribution[index_min] < distribution[i] &&
		distribution[index_max] < distribution[i])
	    {
	      if (distribution[i] > peak_value1)
	      {
		peak_index2 = peak_index1;
		peak_value2 = peak_value1;
		peak_index1 = i;
		peak_value1 = distribution[i];
	      }
	      else if (distribution[i] > peak_value2)
	      {
		peak_index2 = i;
		peak_value2 = distribution[i];
	      }
	    }
	  }

	  int td2_value = 0;

	  if (peak_index1 < peak_index2)
	  {
	    td1 = peak_index1;
	    td2 = peak_index2;
	    td2_value = peak_value2 * t;
	  }
	  else
	  {
	    td1 = peak_index2;
	    td2 = peak_index1;
	    td2_value = peak_value1 * t;
	  }

	  int min_delta = INT_MAX;

	  for (int i = td2 + 1; i < nelements; ++i)
	  {
	    const int delta = abs(distribution[i] - td2_value);

	    if (delta == 0)
	    {
	      td2 = i;
	      break;
	    }

	    if (delta < min_delta)
	    {
	      td2 = i;
	      min_delta = delta;
	    }
	  }
	}

	inline
	void compute_distribution(const int max_distance,
				  const std::map<util::ord_pair<unsigned>,
						 int>& distances,
				  const float t,
				  int& td1,
				  int& td2)
	{
	  const int nelements = max_distance + 1;
	  const int window = 2;
	  std::vector<int> distribution(nelements, 0);
	  std::vector<int> smoothed_distribution(nelements, 0);
	  std::map<util::ord_pair<unsigned>, int>::const_iterator it_begin =
	    distances.begin();
	  std::map<util::ord_pair<unsigned>, int>::const_iterator it_end =
	    distances.end();

	  for (; it_begin != it_end; ++it_begin)
	    ++distribution[(*it_begin).second];

	  const int factor = 2 * window + 1;

	  // Smoothed distribution
	  for (int i = 0; i < nelements; ++i)
	  {
	    int count = 0;

	    for (int j = i - window; j <= i + window; ++j)
	    {
	      if (j < 0 || j >= nelements)
		continue;

	      count += distribution[j];
	    }

	    smoothed_distribution[i] = count / factor;
	  }

	  // Compute peaks in distribution
	  compute_peaks(smoothed_distribution, t, td1, td2);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::voronoi

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_VORONOI_DISTRIBUTION_HH


