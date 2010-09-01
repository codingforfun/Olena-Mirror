#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>
#include <mln/labeling/blobs.hh>

#include "kk2d.hh"


int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::kk2d;

  {
    typedef value::int_u8 T;
    typedef range<T> R;

    image2d<T> f;
    io::pgm::load(f, argv[1]);
    debug::println("f", f);

    image2d<R> K = immerge_as_range(f); // FIXME: 2- and 1-faces only...
    // debug::println("K", K | is_not_0_face);

    add_0_faces_range(K);
    // debug::println("K", K);


    // >= 130

    debug::println(f | (pw::value(f) >= pw::cst(130)));
    kk2d::println("K >= 130", upper_set(K, 130) );


    // < 130
    kk2d::println("K < 130", lower_set_strict(K, 130) );


    // = 130
    kk2d::println("K = 130", level_set(K, 130) );

    // > 130
    kk2d::println("K > 130", upper_set_strict(K, 130) );


    std::abort(); // ****************************************************************


    debug::println(f | (pw::value(f) >= pw::cst(130)));

    unsigned nlabels;
    image2d<bool> Kge130, CKge130, Kl130, CKl130;


    // >= 130

    Kge130 = upper_set_v2(K, 130);
    kk2d::println("K >= 130  Version 2", Kge130 );
    debug::println(labeling::blobs(Kge130, c4(), nlabels) | pw::value(Kge130));

    CKge130 = contour(Kge130);
    kk2d::println("C( K >= 130 )  Version 2", CKge130 );
    debug::println(labeling::blobs(CKge130, c4(), nlabels) | pw::value(CKge130));
    

    // < 130
    Kl130 = lower_set_strict_v2(K, 130);
    kk2d::println("K < 130  Version 2", Kl130);
    debug::println(labeling::blobs(Kl130, c4(), nlabels) | pw::value(Kl130));

    CKl130 = contour(Kl130);
    kk2d::println("C( K < 130 )  Version 2", CKl130 );
    debug::println(labeling::blobs(CKl130, c4(), nlabels) | pw::value(CKl130));


  }


//   {
//     int vals[] = { 0, 1,
// 		   1, 0 };
//     image2d<int> f = make::image2d(vals);
//     debug::println("f", f);

//     image2d< range<int> > K = immerge_as_range(f);
//     add_0_faces_range(K);

//     kk2d::println("K = 0", level_set(K, 0) );
//     kk2d::println("K = 1", level_set(K, 1) );

//   }


//   {
//     int vals[] = { 0, 2,
// 		   2, 0 };
//     image2d<int> f = make::image2d(vals);
//     debug::println("f", f);

//     image2d< range<int> > K = immerge_as_range(f);
//     add_0_faces_range(K);

//     kk2d::println("K = 0", level_set(K, 0) );
//     kk2d::println("K = 1", level_set(K, 1) );
//     kk2d::println("K = 2", level_set(K, 2) );

//   }


}
