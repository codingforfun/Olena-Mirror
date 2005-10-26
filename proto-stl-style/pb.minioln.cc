#include <iostream>
#include <cmath>


# include "misc.hh"
# include "abstract.hh"
# include "coord.hh"


// #########################################################  SECTION gen




// ---------------------------------------------------  SECTION algos



# define for_all_p(p) \
  for(p.start(); p.is_valid(); p.next())



template <class Image>
void println_gen(std::ostream& ostr,
		 const std::string& msg,
		 const Image& input)
{
  ostr << msg << std::endl;
  typename Image::fwd_piter_type p(input.info());
  for_all_p(p)
    ostr << input[p] << ' ';
  ostr << std::endl << std::endl;
}


template <class Image>
void println_2d(std::ostream& ostr,
		const std::string& msg,
		const Image& input)
{
  ostr << msg << std::endl;

  typename Image::piter_type p(input.info());
  p.start();

  typename Image::point_type p_ = p;
  coord prev_row = p_.row();

  for_all_p(p)
    {
      p_ = p;
      if (p_.row() != prev_row)
	{
	  prev_row = p_.row();
	  ostr << std::endl;
	}
      ostr << input[p] << ' ';
    }
  ostr << std::endl << std::endl;
}


template <class Image>
void iota(Image& input)
{
  typename Image::value_type value = 0;
  typename Image::piter_type p(input.info());
  for_all_p(p)
    input[p] = ++value;
}



// #########################################################  SECTION *d


# include "1d.hh"
# include "2d.hh"

# include "f.hh"


// ###################################################  SECTION morphers




# include "fun.hh"
# include "rowof.hh"
# include "mask.hh"


// FIXME: write other morphers (subset, cylinder, etc.)




// ###################################################  main SECTION



int main()
{
  image2d<int> ima(3,3);
  iota(ima);
  println_2d(std::cout, "ima=", ima);

  {

    println_2d(std::cout, "mask=",
	       through_fun(ima, f_::leq<int>(4)));

    println_2d( std::cout, "ima|mask=",
	        with_mask( ima,
			   through_fun(ima, f_::leq<int>(4)) )
	       );


    typedef image2d<int> ima_type;
    typedef through_fun_<ima_type, f_::leq<int> > thru_type;

    with_mask_<ima_type, thru_type> ima2 = with_mask( ima,
						      through_fun(ima, f_::leq<int>(4)) );

    typedef with_mask_<ima_type, thru_type> ima2_type;
    typedef through_fun_<ima_type, f_::geq<int> > thru2_type;

    with_mask_<ima2_type, thru2_type> ima3 = with_mask( ima2,
							through_fun(ima, f_::geq<int>(2)) );

    // Neither this:
    println_2d( std::cout, "ima|mask|mask=", ima3);

// mask_piter<Piter, MaskImage>::mask_piter(const Info&)

//   // debut msg d'err
//   Info = mask_info< mask_info< size2d_<coord>, // info orig
//                                through_fun_< image2d<int>, f_::leq<int> > // type de l'image-masque
//                              >,
//                   main()::thru2_type >

// Piter = fwd_piter2d_<coord>
// MaskImage = through_fun_<image2d<int>, f_::leq<int> >

  // fin


//     // nor the following statement work!
//     println_2d( std::cout, "ima|mask|mask=",
// 		with_mask (with_mask ( ima,
// 				       through_fun(ima,
// 						   f_::leq<int>(4)) ),
// 			   through_fun( ima,
// 					f_::geq<int>(2) )
// 			   )
// 		);

  }

//   {
//     // through_fun
//     f_::div div2(2);
//     typedef through_fun_<const image2d<int>, f_::div> morpher_type;

//     morpher_type ima2(ima, div2);
//     morpher_type ima3 = ima2;

//     // ima3[point2d(1,1)] = 12; // ok: err "non lvalue in assignmt"
//     ima[point2d(1,1)] = 12; // ch ima3 since data is shared

//     println_2d(std::cout, "ima/2=", ima3);

//     // sugar proc:
//     // println_2d(std::cout, through_fun(ima, f_::div(2)));
//   }


//   {
//     println_gen(std::cout, "ima|(row=1)=", rowof(ima, 1));

//     std::cout << rowof(ima, 1).info().nindices() << std::endl;
//     // rowof(ima, 1).info().ncols(); // ok: err
//   }


}
