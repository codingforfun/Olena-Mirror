// current version

#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/neighb2d.hh>
#include <cstddef>
#include <oln/core/equipment.hh>

namespace oln
{
  stc_decl_associated_type( output_t );
  stc_decl_associated_type( input_t );

  //------------------------------
  //             concept
  //------------------------------

  //--Functor-------------------

  template <typename Exact>
  struct Functor : public virtual Any< Exact >
  {
      stc_typename( output_t );
      stc_typename( input_t  );

      output_t	operator()(input_t input) { return exact(this)->impl_apply(input); }

    protected:
      Functor() {}
  };

  //--Nbhfunctor---------------

  template <typename Exact>
  struct NbhFunctor : public virtual Functor< Exact >  {
      stc_using_from(Functor, input_t);
      stc_using_from(Functor, output_t);

      void	operator()(const input_t& input) { exact(this)->impl_apply_local(input); }
      output_t	value()                          { return exact(this)->impl_value();     }
    protected:
      NbhFunctor() {}
  };

  //--functor_modifier---------------

  template <typename Exact>
  struct Functor_of_Points : public virtual Functor< Exact >  {
      stc_using_from(Functor, input_t);
      stc_using_from(Functor, output_t);

      void	operator()(input_t input) { return exact(this)->impl_apply(input); }
      output_t	value()                   { return exact(this)->impl_value();      }
    protected:
      Functor_of_Points() {}
  };

  //--functor_modifier---------------

  template <typename Exact>
  struct Functor_modifier : public virtual Functor< Exact >  {
      stc_using_from(Functor, input_t);
      stc_using_from(Functor, output_t);

      output_t	operator()(input_t input) { return exact(this)->impl_apply(input); }
    protected:
      Functor_modifier() {}
  };

  //------------------------------
  //             apply
  //------------------------------

  //--local-Imagewith-nbh---------

  template<typename I, typename F>
  typename F::output_t apply_local(const Image_with_Nbh<I>&	        input,
				   box_fwd_piter_<typename I::point>&	p,
				   NbhFunctor<F>&			f)
  {
    oln_niter(I) n(p, input.nbhood());

    for_all(n)
      if (input.owns_(n))
	f( input(n) );
    return f.value();
  }

  //-apply sur nbh image--------
  template <typename I, typename F>
  void	apply(const Image_with_Nbh<I>&	input,
	      NbhFunctor<F>&		fun)
  {
    oln_piter(I) p(input.points());

    for_all(p)
      {
	apply_local(input, p, fun);
	std::cout << std::endl;
      }
  }

  //-apply on points
  template <typename I, typename F>
  typename F::output_t	apply(Mutable_Image<I>&		input,
			      Functor_of_Points<F>&	f)
  {
    oln_piter(I) p(input.points());

    for_all(p)
      f( input(p) );
  }

  //-generic--apply--------------
  template <typename I, typename F>
  typename F::output_t	apply(Image<I>&		input,
			      Functor<F>&	f)
  {
    return f(input);
  }

  //------------------------------
  //            fonctions
  //------------------------------

  //-printnbh

  template <typename T> struct printnbh;

  template <typename I>
  struct vtypes< printnbh<I> >
  {
      typedef printnbh<I>	current;
      typedef top< current >	super_type;

      typedef typename I::value 	output_t;
      typedef typename I::value 	input_t;

      typedef stc::is<NbhFunctor>	category;
  };

  template <typename I>
  struct printnbh : public top< printnbh<I> >
  {
      typedef top< printnbh<I> >    super;

      stc_using( output_t );
      stc_using( input_t  );

      printnbh() { }
      output_t	impl_value()	{ return 0; }

      void	impl_apply_local(const input_t& p) { std::cout << p;}
  };



  //-dilate

  template <typename T> struct dilate;

  template <typename I>
  struct vtypes< dilate<I> >
  {
      typedef dilate<I>		current;
      typedef top< current >	super_type;

      typedef typename I::value 	output_t;
      typedef typename I::value 	input_t;

      typedef stc::is<NbhFunctor>	category;
  };

  template <typename I>
  struct dilate : public top< printnbh<I> >
  {
      typedef top< printnbh<I> >    super;

      stc_using( output_t );
      stc_using( input_t  );

      dilate(int n = 1) : order(n), v_som()
      { }

      void	impl_apply_local(const input_t& p) { v_som += v_som + p;}

      output_t	impl_value()   {
	output_t tmp = v_som;
	v_som = 0;
	if (tmp > order)
	  return tmp;
	else
	  return 0;
      }

    private:
      int	order;
      int	v_som;
  };

  //-identity

  template <typename I> struct identity;

  template <typename I>
  struct vtypes< identity<I> >
  {
      typedef identity<I>		current;
      typedef top< current >	super_type;

      typedef Image<I>&	output_t;
      typedef Image<I>&	input_t;

      typedef stc::is<Functor>	category;
  };

  template <typename I>
  struct identity : public top< identity<I> >
  {
      typedef top< identity<I> >    super;

      stc_using( output_t );
      stc_using( input_t  );

      identity() {}
      output_t impl_apply(input_t input) { return input; }
  };



  //-plusplus------

  template <typename T> struct plusplus;

  template <typename I>
  struct vtypes< plusplus<I> >
  {
      typedef plusplus<I>	current;
      typedef top< current >	super_type;

      typedef void		 output_t;
      typedef typename I::value& input_t;

      typedef stc::is<Functor_of_Points> category;
  };

  template <typename I>
  struct plusplus : public top< plusplus<I> >
  {
      typedef top< plusplus<I> >    super;

      stc_using( output_t );
      stc_using( input_t  );

      plusplus(const typename I::value n) : val(n) {}

      output_t impl_apply(input_t p) { p += val; }
    private:
      const typename I::value val;
  };
}

#include <string>

int main()
{
  using namespace oln;

  //images
  //-----------

  image2d<bool> ima(3, 3);
  image2d<int>  ima2(2, 3);
  image2d<std::string>  ima3(5, 6);



  //fonctions
  //------------------

  //probleme une fonction par type d image
  //pourquoi pas apply<Fun>(ima)
  // ou dilate fdilate (comment ??);
  printnbh< image2d<bool> > fprintNbh1;
  printnbh< image2d<int> >  fprintNbh2;

  identity< image2d<int> > id;

  plusplus< image2d<int> > fplusplus(5);



  //applications
  //-----------------

  std::cout << "plus 5 sur chaque points de ima2" << std::endl;
  apply(ima2, fplusplus);

  std::cout << "affichage du voisinage de ima1"<< std::endl;
  apply(ima + c4, fprintNbh1);

  std::cout << "affichage du voisinage de ima2" << std::endl;
  apply(ima2 + c8, fprintNbh2);

  std::cout << "affiche le voisinage de l'identite" << std::endl;
  apply(apply(ima2, id) + c8, fprintNbh2);

}

