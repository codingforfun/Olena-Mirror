
#ifndef UTILS_H_MEDIAN_HH
# define UTILS_H_MEDIAN_HH

# include <mln/trait/value_.hh>


namespace mln
{

  namespace utils
  {

    template <typename T>
    struct h_median
    {

      enum {
	nbits_ = mln_nbits(T),
	sup = mlc_pow_int(2, nbits_)
      };

      h_median()
      {
	clear();
      }

      void clear()
      {
	for (unsigned v = 0; v < sup; ++v)
	  h_[v] = 0;
	sum_ = 0;
	sum_minus_ = 0;
	sum_plus_ = 0;
	t_ = sup / 2;
	valid_ = true;
      }

      void take(const T& t)
      {
	++h_[t];
	++sum_;
	if (t < t_)
	  ++sum_minus_;
	else if (t > t_)
	  ++sum_plus_;
	if (valid_)
	  valid_ = false;
      }

      const T& value() const
      {
	if (! valid_)
	  update_();
	return t_;
      }

      void println() const
      {
	if (! valid_)
	  update_();
	
	std::cout << " h = ";
	for (unsigned v = 0; v < sup; ++v)
	  if (h_[v])
	    std::cout << v << ':' << h_[v] << ' ';
	std::cout << std::endl;
	std::cout << " v = " << t_ << std::endl;
	std::cout << " sum = " << sum_minus_ << " (-) " << h_[t_] << " (+) " << sum_plus_ << std::endl;
	std::cout << " bal = " << (2 * sum_minus_) << " (-) " << sum_ << " (+) " << (2 * sum_plus_) << std::endl;
      }

      unsigned sum()       const { return sum_;       }
      unsigned sum_plus()  const { return sum_plus_;  }
      unsigned sum_minus() const { return sum_minus_; }

      bool goes_up() const
      {
	return sum_plus_ > sum_minus_;
      }

    private:

      mutable unsigned h_[sup];
      mutable unsigned sum_;

      mutable unsigned sum_minus_, sum_plus_;
      mutable bool valid_;
      mutable T t_; // the median current value


      void update_() const
      {
	valid_ = true;

	if (sum_ == 0)
	  return;

	if (2 * sum_minus_ > sum_)
	  go_minus_();
	else
	  if (2 * sum_plus_ > sum_)
	    go_plus_();
	  else
	    if (h_[t_] == 0)
	      {
		// go to the heaviest side
		if (sum_plus_ > sum_minus_)
		  go_plus_();
		else
		  go_minus_(); // default when both sides are balanced
	      }
      }

      void go_minus_() const
      {
	do
	  {
	    sum_plus_ += h_[t_];
	    do
	      --t_;
	    while (h_[t_] == 0);
	    sum_minus_ -= h_[t_];
	  }
	while (2 * sum_minus_ > sum_);
      }

      void go_plus_() const
      {
	do
	  {
	    sum_minus_ += h_[t_];
	    do
	      ++t_;
	    while (h_[t_] == 0);
	    sum_plus_ -= h_[t_];
	  }
	while (2 * sum_plus_ > sum_);
      }
    };


  } // end of namespace mln::utils

} // end of namespace mln


#endif // ! UTILS_H_MEDIAN_HH
