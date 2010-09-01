#ifndef DH_QUEUE
# define DH_QUEUE

# include <queue>



namespace mln
{

  template <typename T, typename E>
  struct h_queue
  {
    enum { MAX = 256 };

    h_queue()
    {
      size_ = 0;
    }

    void push(const E& p, int v)
    {
      mln_precondition(v >= 0 && v < MAX);
      q_[v].push(p);
      ++size_;
    }

    E pop(int l)
    {
      mln_precondition(l >= 0 && l < MAX);
      mln_precondition(! is_empty(l));
      E e = q_[l].front();
      q_[l].pop();
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

    bool is_empty(int l) const
    {
      mln_precondition(l >= 0 && l < MAX);
      return q_[l].empty();
    }

    std::queue<E> q_[MAX];
    unsigned size_;
  };



  enum state_t
    {
      level_do_not_change,
      level_change,
      level_invertion
    };


  template <typename T, typename P>
  struct dh_queue
  {
    enum { MAX = 256 };

    image2d< range<T> > Kr;

    h_queue<T,P> q_up, q_down;
    bool going_up;
    int l;
    state_t state;

    unsigned size() const
    {
      return q_up.size() + q_down.size();
    }

    bool is_empty() const
    {
      return size() == 0;
    }

    bool is_level_completed() const
    {
      if (going_up)
	return q_up.is_empty(l);
      else
	return q_down.is_empty(l);
    }

    void push(const P& p)
    {
      const T& lower = Kr(p).lower, upper = Kr(p).upper;
      if (going_up)
	{
	  if (lower >= l)
	    //  l   lower .. upper
	    //  --> --------------
	    q_up.push(p, lower);
	  else
	    // lower < l
	    if (upper >= l)
	      //  lower..  l  .. upper
	      //  ------- --> --------
	      q_up.push(p, l);
	    else
	      //  lower .. upper  l
	      //  -------------- -->
	      q_down.push(p, upper);
	}
      else
	{
	  if (upper <= l)
	    q_down.push(p, upper);
	  else
	    if (lower <= l)
	      q_down.push(p, l);
	    else
	      q_up.push(p, lower);
	}
    }

    P pop()
    {
      mln_precondition(! is_empty());

      if (going_up)
	{
	  if (! q_up.is_empty(l))
	    {
	      P p = q_up.pop(l);
	      state = level_do_not_change;
	      return p;
	    }
	  state = level_change;
	  // so going up...
	  for (; l < MAX; ++l)
	    if (! q_up.is_empty(l))
	      return q_up.pop(l);
	  // not found so now going down...
	  going_up = false;
	  state = level_invertion;
	  for (l = 255; l >= 0; --l)
	    if (! q_down.is_empty(l))
	      return q_down.pop(l);
	}
      else
	{
	  if (! q_down.is_empty(l))
	    {
	      P p = q_down.pop(l);
	      state = level_do_not_change;
	      return p;
	    }
	  state = level_change;
	  // so going down...
	  for (; l >= 0; --l)
	    if (! q_down.is_empty(l))
	      return q_down.pop(l);
	  // not found so now going up...
	  going_up = true;
	  state = level_invertion;
	  for (l = 0; l < MAX; ++l)
	    if (! q_up.is_empty(l))
	      return q_up.pop(l);
	}

      std::cerr << "weird: not empty but could not find one" << std::endl;
      return P();
    }

    bool invariant_() const
    {
      if (going_up)
	{
	  for (int l_ = 0; l_ < l; ++l_)
	    if (! q_up.is_empty(l_))
	      return false;
	}
      else // going down
	{
	  for (int l_ = l; l_ >= 0; --l_)
	    if (! q_down.is_empty(l_))
	      return false;
	}
      return true;
    }

  };


} // mln


# endif // ! DH_QUEUE_HH
