#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>

#include <mln/literal/colors.hh>

#include <mln/core/alias/box2d.hh>
#include <mln/accu/shape/bbox.hh>
#include <mln/value/label_8.hh>
#include <mln/labeling/compute.hh>

#include <mln/pw/value.hh>
#include <mln/draw/box.hh>

#include <mln/util/timer.hh>

namespace mln
{
  namespace inpainting
  {
    /*
     * Interface
     */

    template <template <typename> class I, typename T, typename F>
    I<T> inpaint(const Image<I<T> >& src_,
		 const Image<I<bool> >& mask_,
		 F inpaint_method,
		 unsigned inpaint_radius);

    /*
     * Implem
     */

    template <template <typename> class I, typename T, typename F>
    I<T> inpaint(const Image<I<T> >& inpaint_src_,
		 const Image<I<bool> >& inpaint_mask_,
		 F inpaint_method,
		 unsigned inpaint_radius)
    {
      const I<T>& inpaint_src = exact(inpaint_src_);
      const I<bool>& inpaint_mask = exact(inpaint_mask_);

      box2d roi;
      { // Compute the region of interest
	I<value::label_8> lbl(inpaint_mask.domain());

	data::fill(lbl, 1);
	data::fill((lbl | pw::value(inpaint_mask)).rw(), 0);

	value::label_8 nlabels = 2;
      
	util::array<box2d> roi_bboxes =
	  labeling::compute(accu::meta::shape::bbox(),
			    lbl,
			    nlabels);

	roi = roi_bboxes[0];
	roi.enlarge(inpaint_radius);
	roi.crop_wrt(inpaint_src.domain());
      }

      I<mln_sum(T)> new_src(roi);
      { // paste inpaint_src to new_src
	mln_piter(I<T>) p(roi);
	mln_pixter(I<mln_sum(T)>) p_new(new_src);
	
	for_all_2(p, p_new)
	  p_new.val() = inpaint_src(p);
      }

      I<bool> new_mask(roi);
      data::paste(inpaint_mask | roi, new_mask);

      mln::util::timer t;
      t.start();

      inpaint_method(new_src, new_mask);

      t.stop();
      std::cout << "inpaint time = " << t << std::endl;

      I<T> output(inpaint_src);
      { // paste new_src to output
	mln_piter(I<T>) p(roi);
	mln_pixter(const I<mln_sum(T)>) p_new(new_src);
	
	for_all_2(p, p_new)
	  output(p) = p_new.val();
      }

      return output;
    }

  } /* mln::inpainting */

} /* mln*/
