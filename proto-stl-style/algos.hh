#ifndef MINIOLN_ALGOS_HH
# define MINIOLN_ALGOS_HH

# include <iostream>
# include <string>

# include "abstract.hh"
# include "type_of.hh"
# include "mlc.hh"


template <class I>
void println(std::ostream& ostr,
	     const std::string& msg,
	     const I& input)
{
  mlc_is_a(I, abstract::image)::ensure();

  ostr << msg << std::endl;
  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    ostr << input[p] << ' ';
  ostr << std::endl << std::endl;
}


template <class I>
void println_2d(std::ostream& ostr,
		const std::string& msg,
		const I& input)
{
  mlc_is_a(I, abstract::image)::ensure();

  ostr << msg << std::endl;

  oln_type_of(I, piter) p(input.info());
  p.start();

  // FIXME: rewrite using bbox and coord_0
  // that should work with empty images...

  for (coord row = 0; row < input.info().exact().nrows(); ++row)
    {
      for (coord col = 0; col < input.info().exact().ncols(); ++col)
	{
	  oln_type_of(I, point) p_(row, col);
	  if (p.is_valid() and p_ == p)
	    {
	      ostr << input[p] << " ";
	      p.next();
	    }
	  else
	    ostr << "  ";
	}
      ostr << std::endl;
    }
  ostr << std::endl;
}


template <class I>
void iota(I& input)
{
  mlc_is_a(I, abstract::image)::ensure();

  unsigned value = 0;
  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    input[p] = ++value;
}


template <class I>
void fill(I& input,
	  const oln_type_of(I, value)& value)
{
  mlc_is_a(I, abstract::image)::ensure();

  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    input[p] = value;
}


template <class I>
oln_type_of(I, concrete)
clone(const I& input)
{
//  mlc_is_a(I, abstract::image)::ensure();

  oln_type_of(I, concrete) output(input.info());
  oln_type_of(I, piter) p(input.info());
  for_all_p(p)
    output[p] = input[p];
  return output;
}


template <class I>
oln_type_of(I, concrete)
mean_over_nbh(const I& input, bool log = false)
{
//  mlc_is_a(I, abstract::image)::ensure();

  oln_type_of(I, concrete) output(input.info());
  oln_type_of(I, piter) p(input.info());
  oln_type_of(I, niter) n(input.info());

  for_all_p(p)
    {
      oln_type_of(I, value) sum = input[p];
      if (log)  std::cout << input[p] << ": ";
      unsigned count = 1;
      for_all_n_of_p(n, p)
	if (input.has(n))
	  {
	    sum += input[n];
	    if (log)  std::cout << input[n] << " ";
	    ++count;
	  }
      output[p] = sum / count;
      if (log)  std::cout << " -> " << sum << "/" << count << "=" << output[p] << std::endl;
    }
  return output;
}




template <class I>
oln_type_of(I, concrete)
geo_erosion(const I& input)
{
//  mlc_is_a(I, abstract::image)::ensure();

  oln_type_of(I, concrete) output(input.info());
  oln_type_of(I, piter) p(input.info());
  oln_type_of(I, niter) n(input.info());

  for_all_p(p)
    {
      oln_type_of(I, value) min = input[p];
      for_all_n_of_p(n, p)
	if (input.has(n) and input[n] < min)
	  min = input[n];
      output[p] = min;
    }
  return output;
}


#endif
