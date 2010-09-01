#ifndef UTILS_H_QUEUE_HH
# define UTILS_H_QUEUE_HH

# include <ostream>
# include <queue>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace utils
  {

    template <typename E, typename T>
    struct h_queue
    {

      h_queue()
      {
	size_ = 0;
      }

      void push(const E& e, const T& v)
      {
	q_[v].push(e);
	++size_;
      }

      E pop(const T& v)
      {
	mln_precondition(! is_empty(v));
	E e = q_[v].front();
	q_[v].pop();
	--size_;
	return e;
      }

      unsigned size() const
      {
	return size_;
      }

      bool is_empty() const
      {
	return size_ == 0;
      }

      bool is_empty(const T& v) const
      {
	return q_[v].empty();
      }

      enum {
	nbits_  = mln_nbits(T),
	sup = mlc_pow_int(2, nbits_)
      };

      void debug_println(std::ostream& ostr = std::cout) const
      {
	ostr << "[ ";
	for (unsigned v = 0; v < sup; ++v)
	  {
	    if (q_[v].empty())
	      continue;
	    ostr << v << '-' << q_[v].size() << ' ';
	  }
	ostr << "]  #" << size_ << std::endl;
      }

    private:

      std::queue<E> q_[sup];
      unsigned size_;
    };


  } // end of namespace mln::utils

} // end of namespace mln


#endif // ! UTILS_H_QUEUE_HH
