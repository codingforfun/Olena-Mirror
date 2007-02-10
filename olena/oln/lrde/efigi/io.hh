// Copyright (C) 2006  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_LRDE_EFIGI_IO_HH
# define OLENA_LRDE_EFIGI_IO_HH

#include <cmath>

#include "fitsio.h"

#include <iostream>
#include <fstream>
#include <string>

#include <oln/basics2d.hh>



namespace oln
{

  namespace efigi
  {


    // Fwd decls.

    image2d<float> load_fits(const std::string& filename);

    image2d<float> load_pfm(const std::string& filename);
    void save_pfm(const image2d<float>& ima, const std::string& filename);

    // end of fwd decls.



    // sample code I:
    /*

    image2d<unsigned char> lena = oln::load("lena.pgm");

    image2d<float> lenaf = efigi::add_float_noise(lena);
    efigi::save_pfm(lenaf, "lenaf.pfm");

    image2d<float> lenaf_2 = efigi::load_pfm("lenaf.pfm");
    assert(not efigi::diff(lenaf, lenaf_2));

    */
    // end of sample code




    // sample code II:
    /*

    image2d<float> ima = efigi::load_fits(argv[1]);
    float min, max;
    efigi::min_max(ima, min, max);
    std::cout << "min = " << min << "  max = " << max << std::endl;

    image2d<unsigned char> lin = efigi::convert_linear_8(ima, min, max);
    save(lin, "tmp_lin.pgm");

    std::vector<unsigned> h = efigi::compute_histo(lin);
    std::ofstream file("tmp_h8.dat");
    efigi::print_histo(h, file);

    */


    void fits_exit(int status)
    {
      if (status)
	{
	  fits_report_error(stderr, status);
	  exit(status);
	}
      return;
    }


    image2d<float> load_fits(const char* filename)
    {
      fitsfile *fptr;
      int status,  nfound, anynull;
      long naxes[2];
      float nullval;

      status = 0;
      if (fits_open_file(&fptr, filename, READONLY, &status))
	fits_exit(status);

      if (fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxes, &nfound, &status))
	fits_exit(status);

      const int nrows = naxes[0], ncols = naxes[1];
      std::cout << "nrows = " << nrows << "  ncols = " << ncols << std::endl;
      image2d<float> output(nrows, ncols);

      nullval  = 0; // don't check null values

      for (int row = 0; row < nrows; ++row)
	{
	  if (fits_read_img(fptr,
			    TFLOAT,
			    1 + row * ncols,
			    ncols,
			    &nullval,
			    &(output(row, 0)),
			    &anynull,
			    &status))
	    fits_exit(status);
	}

      if (fits_close_file(fptr, &status))
	fits_exit(status);

      return output;
    }


    void save_pfm(const image2d<float>& ima,
		  const std::string& filename)
    {
      std::ofstream file(filename.c_str());
      file << ima.nrows() << ' ' << ima.ncols() << std::endl
	   << "float" << std::endl;

      image2d<float>& ima_ = const_cast< image2d<float>& >(ima);
      for (int row = 0; row < ima.nrows(); ++row)
	file.write((const char*)(&(ima_(row, 0))),
		   sizeof(float) * ima.ncols());
      file.close();
    }


    image2d<float> load_pfm(const std::string& filename)
    {
      std::ifstream file(filename.c_str());
      int nrows, ncols;
      file >> nrows;
      file >> ncols;
      if (file.get() != '\n')
	{
	  std::cerr << "pb in 'oln::efigi::load_pfm' with "
		    << filename << std::endl;
	  exit(EXIT_FAILURE);
	}
      std::string tag;
      file >> tag;
      if (tag != "float")
	{
	  std::cerr << "pb in 'oln::efigi::load_pfm' with "
		    << filename << std::endl;
	  exit(EXIT_FAILURE);
	}
      if (file.get() != '\n')
	{
	  std::cerr << "pb in 'oln::efigi::load_pfm' with "
		    << filename << std::endl;
	  exit(EXIT_FAILURE);
	}
      
      image2d<float> output(nrows, ncols);
      for (int row = 0; row < nrows; ++row)
	file.read((char*)(&(output(row, 0))),
		  sizeof(float) * ncols);
      file.close();
      return output;
    }


  } // end of namespace oln::efigi

} // end of namespace oln


#endif // ! OLENA_LRDE_EFIGI_IO_HH
