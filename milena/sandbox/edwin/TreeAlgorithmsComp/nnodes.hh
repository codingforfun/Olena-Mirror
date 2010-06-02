#ifndef NNODES_HH
# define NNODES_HH

namespace mln
{

  template <typename I, typename J>
  unsigned nnodes(const I& f, const J& parent)
  {
    mlc_equal(mln_value(J), unsigned)::check();
    unsigned n = 0;
    mln_piter(I) p(parent.domain());
    for_all(p)
    {
      unsigned o = parent.index_of_point(p);
      if (parent(p) == o || f.element(parent(p)) != f(p))
	++n;
    }
    return n;
  }

}

#endif // !NNODES_HH
