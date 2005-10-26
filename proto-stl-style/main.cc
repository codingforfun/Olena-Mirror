# include "all.hh"



template <class P>
void test(const abstract::points<P>& pts)
{
  oln_pts_type_of(P, piter) p(pts.exact());
  for_all_p(p)
    std::cout << p << std::endl;
}


int main()
{
  image2d<int> ima(3,3);
  point2d p(1,1);

  image2d<bool> imab(3,3);
  fill(imab, true);
  imab(0,0) = imab(0,2) = imab(2,2) = false;

  iota(ima);


// // //   {
// // //     println(std::cout, "ima|(row=1)=", rowof(ima, 1));
// // //   }



  {
    test( mk_bbox2d(1,1, 2,2) );

    std::cout << "bbox= " << mk_bbox2d(1,1, 2,2) << std::endl;
    echoln( mask(ima, mk_bbox2d(1,1, 2,2)) );
    println_2d( std::cout, "ima|bbox=",
 		mask(ima, mk_bbox2d(1,1, 2,2)) ); // FIXME HERE bug!
  }


  {
    image2d<bool> diag(3,3);
    fill(diag, false);
    diag(0,0) = diag(1,1) = diag(2,2) = true;

    std::cout << "diag= " << mk_pvec(diag) << std::endl;
    // echoln( mask(ima, mk_pvec(diag)) );
    println_2d( std::cout, "ima|pvec=",
		mask(ima, mk_pvec(diag)) );

  }

  {
    std::cout << "rlep= " << mk_rlep(imab) << std::endl;
    // echoln( mask(ima, mk_rlep(imab)) );
    println_2d( std::cout, "ima|rlep=",
		mask(ima, mk_rlep(imab)) );
  }



  {
    println_2d(std::cout, "cos(ima)=", f_cos(ima));
  }

  {

    println_2d(std::cout, "ima=",  ima);
    println_2d(std::cout, "imab=", imab);

    println_2d(std::cout, "mean(ima+c4)=",
	       mean_over_nbh(ima + c4, true));

    println_2d(std::cout, "ima+c4|mask=",
	       mask(ima + c4, imab));

    println_2d(std::cout, "mean((ima+c4)|mask)=",
	       mean_over_nbh(mask(ima + c4, imab), true));

    println_2d(std::cout, "mean((ima|mask)+c4)=",
	       mean_over_nbh(mask(ima, imab) + c4, true));

    println_2d(std::cout, "mean((ima|subset)+c4)=",
	       mean_over_nbh(subset(ima, imab) + c4, true));

    println_2d(std::cout, "mean((ima+c4)|subset)=",
	       mean_over_nbh(subset(ima + c4, imab), true));

  }



  {
    image2d<rgb> imac(3, 3);
    iota(imac);
    f_red(imac)[p] = 12;
    println_2d(std::cout, "imac=", imac);
    println_2d(std::cout, "red(imac)=", f_red(imac));
  }

  {
    println_2d(std::cout, "ima/2=", f_::div<int>(2)(ima));
  }

  {
    f_::inc<int> inc;

    inc(ima)[p] = 12;
    println_2d(std::cout, "inc(ima)=", inc(ima));
    println_2d(std::cout, "ima=", ima);
  }

  {
    image2d<rgb> ima(3, 3);
    iota(ima);
    f_::add_less<rgb, rgb_less> f;

    println_2d( std::cout, "ero(ima)=",
	        geo_erosion(f(ima) + c4) );

    echo(f(ima));
    // echo(f(ima) + c4);

    println_2d( std::cout, "ero2(ima)=",
		geo_erosion(f(ima + c4)) );
  }


  {
    println_2d(std::cout, "mask=",
	       f_::leq<int>(4)(ima));

    println_2d( std::cout, "ima|mask=",
	        mask( ima,
		      f_::leq<int>(4)(ima) )
	       );
  }

  {
    println_2d( std::cout, "mean(ima)=",
		ima + c4 );

    println_2d( std::cout, "mean(ima|mask)=",
		mean_over_nbh( mask( ima + c4,
				     f_::leq<int>(4)(ima)
				     )
			       )
		);
  }

  {

    std::cout << (with_log(ima)[p] = 12) << std::endl;

    std::cout << "   nreads = " << log_data.nreads
	      << "  nwrites = " << log_data.nwrites << std::endl;

  }



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -   problems



  // FIXME: some morphers do not commute as they should...

  {

//     echoln( subset(ima + c4, imab) );
//     echoln( subset(ima, imab) + c4 );

//     echoln( mask(ima + c4, imab) );
//     echoln( mask(ima, imab) + c4 );


    /*

    typedef with_imask_iter<mlc::no_type, image2d<bool>> niter_type;
    niter_type niter( subset(ima, imab).info() );

    */

//     println_2d(std::cout, "mean((ima|subset)+c4)=",
// 	       mean_over_nbh(subset(ima, imab) + c4, true));

//     echo( subset(ima, imab) + c4 );
//     // here, niter_type is fwd_niter<point2d>

//     println_2d(std::cout, "mean((ima+c4)|subset)=",
// 	       mean_over_nbh(subset(ima + c4, imab), true));

//     echo( subset(ima + c4, imab) );
//     // here, niter_type is with_imask_iter<fwd_niter<point2d>, image2d<bool>>

  }



//   // FIXME: image types do not interoper...

//   {
//     image2d<int> ima2 = mean_over_nbh(ima + c4, true);
//   }



//   // FIXME: some morphers can not be applied twice...

//   {
//     println_2d(std::cout, "ima|m|m=",
// 	       mask( mask( ima, thru_pw_fun(ima, f_::leq<int>(4)) ),
// 			  thru_pw_fun(ima, f_::geq<int>(2)) )
// 	       );
//   }

  {
    typedef checker fun_type;
    typedef size2d  size_type;
    typedef function_image<size_type, fun_type> image_type;

    function_image<size_type, fun_type> ima(size_type(6, 6), fun_type());
    println_2d(std::cout, "ima=",  ima);
    vflip<size_type> fun(ima.info());
    image_with_point_function<function_image<size_type, fun_type>, vflip<size_type> > ima_(ima, fun);
    println_2d(std::cout, "ima_=",  ima_);
  }


}
