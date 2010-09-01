#include <queue>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>

#include "k6.hh"


namespace mln
{

  const point2d nil = point2d(-1, -1);


  struct h_queue
  {
    typedef point2d E;

    h_queue()
    {
      size_ = 0;
    }

    void push(const E& p, int v)
    {
      mln_precondition(v >= 0 && v < 256);
      q_[v].push(p);
      ++size_;
    }

    E pop(int l)
    {
      mln_precondition(l >= 0 && l < 256);
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
      mln_precondition(l >= 0 && l < 256);
      return q_[l].empty();
    }

    std::queue<E> q_[256];
    unsigned size_;
  };



  enum state_t
    {
      level_do_not_change,
      level_change,
      level_invertion
    };


  template <typename T>
  struct dh_queue
  {
    image2d< range<T> > Kr;

    typedef point2d P;

    h_queue q_up, q_down;
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
	  for (; l < 256; ++l)
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
	  for (l = 0; l < 256; ++l)
	    if (! q_up.is_empty(l))
	      return q_up.pop(l);
	}

      std::cerr << "weird: not empty but could not find one" << std::endl;
      return nil;
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



  template <typename R>
  void
  doit(const image2d<R>& Kr)
  {
    typedef point2d P;
    P p;
    mln_niter(neighb2d) n(c4(), p);

    box2d b = Kr.domain();

    image2d<bool> deja_vu(b), in_queue(b);
    dh_queue<value::int_u8> q;
    q.Kr = Kr;
    
    // Initialisation.
    
    data::fill(deja_vu, false);
    data::fill(in_queue, false);

    q.l = 0;
    q.going_up = true;

    p = P(0, 0);
    q.push(p);
    in_queue(p) = true;

    // Propagation.
    while (! q.is_empty())
      {
	p = q.pop();
	mln_invariant(in_queue(p) && ! deja_vu(p));
	for_all(n)
	  if (b.has(n) && ! deja_vu(n) && ! in_queue(n))
	    {
	      q.push(n);
	      in_queue(n) = true;
	    }
	deja_vu(p) = true;
      }
  }


} // mln


int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::k6;


  {
    image2d<value::int_u8> ima, K;
    io::pgm::load(ima, argv[1]);

    ima = add_border(ima, 0);
    debug::println("fly:", ima);

    K = immerge(ima);
    // debug::println("K:", K);
    pretty_println("K:", K);

    typedef range<value::int_u8> R;
    image2d<R> Kr = to_range(K);
    // debug::println("Kr:", Kr);

    doit(Kr);

//     for (int l = 0; l < 256; ++l)
//       {
// 	std::cout << l << ' ';
// 	pretty_println("K(l):", level_lines(Kr, l));
//       }


//     image2d<bool> K_(K.domain());
//     for (int l = 0; l < 256; ++l)
//       {
// 	data::fill(K_, pw::value(Kr) == pw::cst(l));
// 	std::cout << l << ' ';
// 	pretty_println("K(l):", K_);
//       }


//     data::fill(K_, pw::value(Kr) == pw::cst(120));
//     pretty_println("K(120):", K_);

//     data::fill(K_, pw::value(Kr) == pw::cst(128));
//     pretty_println("K(128):", K_);


  }

}



//   {
//     image2d<char> ima(15, 9);
//     data::fill((ima | is_2_face).rw(), '2');
//     data::fill((ima | is_1_face).rw(), '1');
//     data::fill((ima | is_0_face).rw(), '0');
//     data::fill((ima | is_no_face).rw(), ' ');
//     debug::println("ima:", ima);
//     pretty_print("ima:", ima);
//   }

//   {
//     image2d<bool> msk(15, 9);
//     data::fill(msk, true);
//     pretty_print("test:", msk);
//   }




//   {
//     data::fill(ima, ' ');
//     data::fill((ima | is_2_face).rw(), '2');
//     debug::println("2-faces:", ima);
//   }

//   {
//     data::fill(ima, ' ');
//     data::fill((ima | is_1_face).rw(), '1');
//     debug::println("1-faces:", ima);
//   }

//   {
//     data::fill(ima, ' ');
//     data::fill((ima | is_0_face).rw(), '0');
//     debug::println("0-faces:", ima);
//   }

//   {
//     data::fill(ima, ' ');
//     data::fill((ima | is_no_face).rw(), 'x');
//     debug::println("no-face:", ima);
//   }
