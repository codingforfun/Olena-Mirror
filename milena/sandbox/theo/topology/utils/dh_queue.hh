
#ifndef UTILS_DH_QUEUE_HH
# define UTILS_DH_QUEUE_HH

# include "h_queue.hh"
# include "range.hh"


namespace mln
{

  namespace utils
  {



    template <typename E, typename T>
    struct dh_queue
    {

      void init(const T& l, bool going_up)
      {
	l_ = l;
	going_up_ = going_up;
      }

      unsigned size() const
      {
	return q_up_.size() + q_down_.size();
      }

      bool is_empty() const
      {
	return size() == 0;
      }

      bool is_level_completed() const
      {
	if (going_up_)
	  return q_up_.is_empty(l_);
	else
	  return q_down_.is_empty(l_);
      }

      int level() const
      {
	return l_;
      }

      void push_element_at_current_level(const E& e)
      {
	if (going_up_)
	  q_up_.push(e, l_);
	else
	  q_down_.push(e, l_);
      }

      void push(const E& e, const range<T>& v)
      {
	const T& lower = v.lower, upper = v.upper;
	if (going_up_)
	  {
	    if (lower >= l_)
	      //  l   lower .. upper
	      //  --> --------------
	      q_up_.push(e, lower);
	    else
	      // lower < l
	      if (upper >= l_)
		//  lower..  l  .. upper
		//  ------- --> --------
		q_up_.push(e, l_);
	      else
		//  lower .. upper  l
		//  -------------- -->
		q_down_.push(e, upper);
	  }
	else
	  {
	    if (upper <= l_)
	      q_down_.push(e, upper);
	    else
	      if (lower <= l_)
		q_down_.push(e, l_);
	      else
		q_up_.push(e, lower);
	  }
      }

      E pop()
      {
	mln_precondition(! is_empty());

	if (going_up_)
	  {
	    if (! q_up_.is_empty(l_))
	      {
		state_ = level_do_not_change;
		return q_up_.pop(l_);
	      }
	    state_ = level_change;
	    // so going up...
	    for (; l_ < l_sup; ++l_)
	      if (! q_up_.is_empty(l_))
		return q_up_.pop(l_);
	    // not found so now going down...
	    going_up_ = false;
	    state_ = level_invertion;
	    for (l_ = l_sup - 1; l_ >= 0; --l_)
	      if (! q_down_.is_empty(l_))
		return q_down_.pop(l_);
	  }
	else
	  {
	    if (! q_down_.is_empty(l_))
	      {
		state_ = level_do_not_change;
		return q_down_.pop(l_);
	      }
	    state_ = level_change;
	    // so going down...
	    for (; l_ >= 0; --l_)
	      if (! q_down_.is_empty(l_))
		return q_down_.pop(l_);
	    // not found so now going up...
	    going_up_ = true;
	    state_ = level_invertion;
	    for (l_ = 0; l_ < l_sup; ++l_)
	      if (! q_up_.is_empty(l_))
		return q_up_.pop(l_);
	  }

	std::cerr << "weird: not empty but could not find one" << std::endl;
	std::abort();

	return E();
      }

      void debug_println(std::ostream& ostr = std::cout) const
      {
	ostr << "l = " << l_ << " going " << (going_up_ ? "up" : "down") << ":" << std::endl;
	ostr << "  q_up = ";
	q_up_.debug_println(ostr);
	ostr << "  q_down = ";
	q_down_.debug_println(ostr);
      }

    private:

      enum state_t
	{
	  level_do_not_change,
	  level_change,
	  level_invertion
	};

      h_queue<E,T> q_up_, q_down_;
      bool going_up_;
      int l_;
      state_t state_;

      enum {
	l_sup = h_queue<E,T>::sup
      };

      bool invariant_() const
      {
	if (going_up_)
	  {
	    for (int l = 0; l < l_; ++l)
	      if (! q_up_.is_empty(l))
		return false;
	  }
	else // going down
	  {
	    for (int l = l_; l >= 0; --l)
	      if (! q_down_.is_empty(l))
		return false;
	  }
	return true;
      }

    };



  } // end of namespace mln::utils

} // end of namespace mln


#endif // ! UTILS_DH_QUEUE_HH
