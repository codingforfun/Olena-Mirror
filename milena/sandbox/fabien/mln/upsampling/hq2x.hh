//Copyright (C) 2003 MaxSt ( maxst@hiend3d.com )

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 2.1 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
//Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public
//License along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA


#ifndef MLN_UPSAMPLING_HQ2X_HH
# define MLN_UPSAMPLING_HQ2X_HH

/// \file
///
/// Produce an up-scaled image using hq2x algorithm by Maxim Stepin.
/// Reference can be found at:
/// http://web.archive.org/web/20070624082212/www.hiend3d.com/hq2x.html

# include <mln/core/alias/dpoint2d.hh>
# include <mln/extension/adjust_duplicate.hh>
# include <mln/geom/max_col.hh>
# include <mln/geom/max_row.hh>
# include <mln/geom/min_col.hh>
# include <mln/geom/min_row.hh>
# include <mln/geom/ncols.hh>
# include <mln/geom/nrows.hh>
# include <mln/value/rgb8.hh>


namespace mln
{

  namespace upsampling
  {

    template <typename I>
    inline
    mln_concrete(I)
    hq2x(const Image<I>& input);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      inline
      unsigned short
      rgb8toShort(value::rgb8 val)
      {
	unsigned short result = 0;

	result += val.red() >> 3;
	result = result << 6;
	result += val.green() >> 2;
	result = result << 5;
	result += val.blue() >> 3;

	return result;
      }

      inline
      void
      intToRgb8(int in, value::rgb8& out)
      {
	out.blue() = in % 256;
	out.green() = (in >> 8) % 256;
	out.red() = (in >> 16) % 256;
      }

      template<typename T>
      inline
      void
      Interp1(image2d<T>& output, int row, int col, int c1, int c2)
      {
	intToRgb8((c1 * 3 + c2) >> 2, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp2(image2d<T>& output, int row, int col, int c1, int c2, int c3)
      {
	intToRgb8((c1 * 2 + c2 + c3) >> 2, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp5(image2d<T>& output, int row, int col, int c1, int c2)
      {
	intToRgb8((c1 + c2) >> 1, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp6(image2d<T>& output, int row, int col, int c1, int c2, int c3)
      {
	intToRgb8((c1 * 5 + c2 * 2 + c3) / 8, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp7(image2d<T>& output, int row, int col, int c1, int c2, int c3)
      {
	intToRgb8((c1 * 6 + c2 + c3) / 8, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp9(image2d<T>& output, int row, int col, int c1, int c2, int c3)
      {
	intToRgb8((c1 * 2 + (c2 + c3) * 3) / 8, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      Interp10(image2d<T>& output, int row, int col, int c1, int c2, int c3)
      {
	intToRgb8((c1 * 14 + c2 + c3) / 16, output.at_(row, col));
      }

      template<typename T>
      inline
      void
      InterpI(image2d<T>& output, int row, int col, int c1)
      {
	intToRgb8(c1, output.at_(row, col));
      }


#define PIXEL00_0     InterpI(output, i * 2, j * 2, c[5]);
#define PIXEL00_10    Interp1(output, i * 2, j * 2, c[5], c[1]);
#define PIXEL00_11    Interp1(output, i * 2, j * 2, c[5], c[4]);
#define PIXEL00_12    Interp1(output, i * 2, j * 2, c[5], c[2]);
#define PIXEL00_20    Interp2(output, i * 2, j * 2, c[5], c[4], c[2]);
#define PIXEL00_21    Interp2(output, i * 2, j * 2, c[5], c[1], c[2]);
#define PIXEL00_22    Interp2(output, i * 2, j * 2, c[5], c[1], c[4]);
#define PIXEL00_60    Interp6(output, i * 2, j * 2, c[5], c[2], c[4]);
#define PIXEL00_61    Interp6(output, i * 2, j * 2, c[5], c[4], c[2]);
#define PIXEL00_70    Interp7(output, i * 2, j * 2, c[5], c[4], c[2]);
#define PIXEL00_90    Interp9(output, i * 2, j * 2, c[5], c[4], c[2]);
#define PIXEL00_100   Interp10(output, i * 2, j * 2, c[5], c[4], c[2]);

#define PIXEL01_0     InterpI(output, i * 2, j * 2 + 1, c[5]);
#define PIXEL01_10    Interp1(output, i * 2, j * 2 + 1, c[5], c[3]);
#define PIXEL01_11    Interp1(output, i * 2, j * 2 + 1, c[5], c[2]);
#define PIXEL01_12    Interp1(output, i * 2, j * 2 + 1, c[5], c[6]);
#define PIXEL01_20    Interp2(output, i * 2, j * 2 + 1, c[5], c[2], c[6]);
#define PIXEL01_21    Interp2(output, i * 2, j * 2 + 1, c[5], c[3], c[6]);
#define PIXEL01_22    Interp2(output, i * 2, j * 2 + 1, c[5], c[3], c[2]);
#define PIXEL01_60    Interp6(output, i * 2, j * 2 + 1, c[5], c[6], c[2]);
#define PIXEL01_61    Interp6(output, i * 2, j * 2 + 1, c[5], c[2], c[6]);
#define PIXEL01_70    Interp7(output, i * 2, j * 2 + 1, c[5], c[2], c[6]);
#define PIXEL01_90    Interp9(output, i * 2, j * 2 + 1, c[5], c[2], c[6]);
#define PIXEL01_100   Interp10(output, i * 2, j * 2 + 1, c[5], c[2], c[6]);

#define PIXEL10_0     InterpI(output, i * 2 + 1, j * 2, c[5]);
#define PIXEL10_10    Interp1(output, i * 2 + 1, j * 2, c[5], c[7]);
#define PIXEL10_11    Interp1(output, i * 2 + 1, j * 2, c[5], c[8]);
#define PIXEL10_12    Interp1(output, i * 2 + 1, j * 2, c[5], c[4]);
#define PIXEL10_20    Interp2(output, i * 2 + 1, j * 2, c[5], c[8], c[4]);
#define PIXEL10_21    Interp2(output, i * 2 + 1, j * 2, c[5], c[7], c[4]);
#define PIXEL10_22    Interp2(output, i * 2 + 1, j * 2, c[5], c[7], c[8]);
#define PIXEL10_60    Interp6(output, i * 2 + 1, j * 2, c[5], c[4], c[8]);
#define PIXEL10_61    Interp6(output, i * 2 + 1, j * 2, c[5], c[8], c[4]);
#define PIXEL10_70    Interp7(output, i * 2 + 1, j * 2, c[5], c[8], c[4]);
#define PIXEL10_90    Interp9(output, i * 2 + 1, j * 2, c[5], c[8], c[4]);
#define PIXEL10_100   Interp10(output, i * 2 + 1, j * 2, c[5], c[8], c[4]);

#define PIXEL11_0     InterpI(output, i * 2 + 1, j * 2 + 1, c[5]);
#define PIXEL11_10    Interp1(output, i * 2 + 1, j * 2 + 1, c[5], c[9]);
#define PIXEL11_11    Interp1(output, i * 2 + 1, j * 2 + 1, c[5], c[6]);
#define PIXEL11_12    Interp1(output, i * 2 + 1, j * 2 + 1, c[5], c[8]);
#define PIXEL11_20    Interp2(output, i * 2 + 1, j * 2 + 1, c[5], c[6], c[8]);
#define PIXEL11_21    Interp2(output, i * 2 + 1, j * 2 + 1, c[5], c[9], c[8]);
#define PIXEL11_22    Interp2(output, i * 2 + 1, j * 2 + 1, c[5], c[9], c[6]);
#define PIXEL11_60    Interp6(output, i * 2 + 1, j * 2 + 1, c[5], c[8], c[6]);
#define PIXEL11_61    Interp6(output, i * 2 + 1, j * 2 + 1, c[5], c[6], c[8]);
#define PIXEL11_70    Interp7(output, i * 2 + 1, j * 2 + 1, c[5], c[6], c[8]);
#define PIXEL11_90    Interp9(output, i * 2 + 1, j * 2 + 1, c[5], c[6], c[8]);
#define PIXEL11_100   Interp10(output, i * 2 + 1, j * 2 + 1, c[5], c[6], c[8]);


      inline
      bool
      Diff(int& YUV1, int& YUV2, int* RGBtoYUV,
	   const int trY, const int trU, const int trV,
	   const int Ymask, const int Umask, const int Vmask,
	   unsigned int w1, unsigned int w2)
      {
	YUV1 = RGBtoYUV[w1];
	YUV2 = RGBtoYUV[w2];
	return ((abs((YUV1 & Ymask) - (YUV2 & Ymask)) > trY) ||
		(abs((YUV1 & Umask) - (YUV2 & Umask)) > trU) ||
		(abs((YUV1 & Vmask) - (YUV2 & Vmask)) > trV));
      }

      inline
      void
      initLUTs(int LUT16to32[65536], int RGBtoYUV[65536])
      {
	int i, j, k, r, g, b, Y, u, v;

	for (i=0; i<65536; i++)
	  LUT16to32[i] = ((i & 0xF800) << 8) + ((i & 0x07E0) << 5) + ((i & 0x001F) << 3);

	for (i=0; i<32; i++)
	  for (j=0; j<64; j++)
	    for (k=0; k<32; k++)
	    {
	      r = i << 3;
	      g = j << 2;
	      b = k << 3;
	      Y = (r + g + b) >> 2;
	      u = 128 + ((r - b) >> 2);
	      v = 128 + ((-r + 2*g -b)>>3);
	      RGBtoYUV[ (i << 11) + (j << 5) + k ] = (Y<<16) + (u<<8) + v;
	    }
      }



      template <typename T>
      inline
      mln_concrete(image2d<T>)
      hq2x_(const image2d<T>& input)
      {
	trace::entering("upsampling::impl::hq2x_");

	mln_domain(image2d<T>) bbox(input.domain().pmin() * 2,
				    input.domain().pmax() * 2 + mln::down_right);
        mln_concrete(image2d<T>) output(bbox);

	int   LUT16to32[65536];
	int   RGBtoYUV[65536];
	int   YUV1, YUV2;
	const  int   Ymask = 0x00FF0000;
	const  int   Umask = 0x0000FF00;
	const  int   Vmask = 0x000000FF;
	const  int   trY   = 0x00300000;
	const  int   trU   = 0x00000700;
	const  int   trV   = 0x00000006;

	initLUTs(LUT16to32, RGBtoYUV);

	unsigned int  w[10];
	unsigned int  c[10];

	//   +----+----+----+
	//   |    |    |    |
	//   | w1 | w2 | w3 |
	//   +----+----+----+
	//   |    |    |    |
	//   | w4 | w5 | w6 |
	//   +----+----+----+
	//   |    |    |    |
	//   | w7 | w8 | w9 |
	//   +----+----+----+
	//
	// w5 is the current point.

	for (int j = geom::min_col(input); j <= geom::max_col(input); j++)
	{
	  for (int i = geom::min_row(input); i <= geom::max_row(input); i++)
	  {
	    w[1] = rgb8toShort(input.at_(i - 1, j - 1));
	    w[2] = rgb8toShort(input.at_(i - 1, j));
	    w[3] = rgb8toShort(input.at_(i - 1, j + 1));
	    w[4] = rgb8toShort(input.at_(i, j - 1));
	    w[5] = rgb8toShort(input.at_(i, j));
	    w[6] = rgb8toShort(input.at_(i, j + 1));
	    w[7] = rgb8toShort(input.at_(i + 1, j - 1));
	    w[8] = rgb8toShort(input.at_(i + 1, j));
	    w[9] = rgb8toShort(input.at_(i + 1, j + 1));

	    int pattern = 0;
	    int flag = 1;

	    YUV1 = RGBtoYUV[w[5]];

	    for (int k = 1; k <= 9; k++)
	    {
	      if (k == 5)
		continue;

	      if (w[k] != w[5])
	      {
		YUV2 = RGBtoYUV[w[k]];
		if ((abs((YUV1 & Ymask) - (YUV2 & Ymask)) > trY) ||
		    (abs((YUV1 & Umask) - (YUV2 & Umask)) > trU) ||
		    (abs((YUV1 & Vmask) - (YUV2 & Vmask)) > trV))
		  pattern |= flag;
	      }
	      flag <<= 1;
	    }

	    for (int k = 1; k <= 9; k++)
	      c[k] = LUT16to32[w[k]];

	    switch (pattern)
	    {
	      case 0:
	      case 1:
	      case 4:
	      case 32:
	      case 128:
	      case 5:
	      case 132:
	      case 160:
	      case 33:
	      case 129:
	      case 36:
	      case 133:
	      case 164:
	      case 161:
	      case 37:
	      case 165:
		{
		  PIXEL00_20
		    PIXEL01_20
		    PIXEL10_20
		    PIXEL11_20
		    break;
		}
	      case 2:
	      case 34:
	      case 130:
	      case 162:
		{
		  PIXEL00_22
		    PIXEL01_21
		    PIXEL10_20
		    PIXEL11_20
		    break;
		}
	      case 16:
	      case 17:
	      case 48:
	      case 49:
		{
		  PIXEL00_20
		    PIXEL01_22
		    PIXEL10_20
		    PIXEL11_21
		    break;
		}
	      case 64:
	      case 65:
	      case 68:
	      case 69:
		{
		  PIXEL00_20
		    PIXEL01_20
		    PIXEL10_21
		    PIXEL11_22
		    break;
		}
	      case 8:
	      case 12:
	      case 136:
	      case 140:
		{
		  PIXEL00_21
		    PIXEL01_20
		    PIXEL10_22
		    PIXEL11_20
		    break;
		}
	      case 3:
	      case 35:
	      case 131:
	      case 163:
		{
		  PIXEL00_11
		    PIXEL01_21
		    PIXEL10_20
		    PIXEL11_20
		    break;
		}
	      case 6:
	      case 38:
	      case 134:
	      case 166:
		{
		  PIXEL00_22
		    PIXEL01_12
		    PIXEL10_20
		    PIXEL11_20
		    break;
		}
	      case 20:
	      case 21:
	      case 52:
	      case 53:
		{
		  PIXEL00_20
		    PIXEL01_11
		    PIXEL10_20
		    PIXEL11_21
		    break;
		}
	      case 144:
	      case 145:
	      case 176:
	      case 177:
		{
		  PIXEL00_20
		    PIXEL01_22
		    PIXEL10_20
		    PIXEL11_12
		    break;
		}
	      case 192:
	      case 193:
	      case 196:
	      case 197:
		{
		  PIXEL00_20
		    PIXEL01_20
		    PIXEL10_21
		    PIXEL11_11
		    break;
		}
	      case 96:
	      case 97:
	      case 100:
	      case 101:
		{
		  PIXEL00_20
		    PIXEL01_20
		    PIXEL10_12
		    PIXEL11_22
		    break;
		}
	      case 40:
	      case 44:
	      case 168:
	      case 172:
		{
		  PIXEL00_21
		    PIXEL01_20
		    PIXEL10_11
		    PIXEL11_20
		    break;
		}
	      case 9:
	      case 13:
	      case 137:
	      case 141:
		{
		  PIXEL00_12
		    PIXEL01_20
		    PIXEL10_22
		    PIXEL11_20
		    break;
		}
	      case 18:
	      case 50:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_10
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_20
		    PIXEL11_21
		    break;
		}
	      case 80:
	      case 81:
		{
		  PIXEL00_20
		    PIXEL01_22
		    PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_10
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 72:
	      case 76:
		{
		  PIXEL00_21
		    PIXEL01_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_22
		    break;
		}
	      case 10:
	      case 138:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_21
		    PIXEL10_22
		    PIXEL11_20
		    break;
		}
	      case 66:
		{
		  PIXEL00_22
		    PIXEL01_21
		    PIXEL10_21
		    PIXEL11_22
		    break;
		}
	      case 24:
		{
		  PIXEL00_21
		    PIXEL01_22
		    PIXEL10_22
		    PIXEL11_21
		    break;
		}
	      case 7:
	      case 39:
	      case 135:
		{
		  PIXEL00_11
		    PIXEL01_12
		    PIXEL10_20
		    PIXEL11_20
		    break;
		}
	      case 148:
	      case 149:
	      case 180:
		{
		  PIXEL00_20
		    PIXEL01_11
		    PIXEL10_20
		    PIXEL11_12
		    break;
		}
	      case 224:
	      case 228:
	      case 225:
		{
		  PIXEL00_20
		    PIXEL01_20
		    PIXEL10_12
		    PIXEL11_11
		    break;
		}
	      case 41:
	      case 169:
	      case 45:
		{
		  PIXEL00_12
		    PIXEL01_20
		    PIXEL10_11
		    PIXEL11_20
		    break;
		}
	      case 22:
	      case 54:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_20
		    PIXEL11_21
		    break;
		}
	      case 208:
	      case 209:
		{
		  PIXEL00_20
		    PIXEL01_22
		    PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 104:
	      case 108:
		{
		  PIXEL00_21
		    PIXEL01_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_22
		    break;
		}
	      case 11:
	      case 139:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_21
		    PIXEL10_22
		    PIXEL11_20
		    break;
		}
	      case 19:
	      case 51:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL00_11
		      PIXEL01_10
		  }
		  else
		  {
		    PIXEL00_60
		      PIXEL01_90
		  }
		  PIXEL10_20
		    PIXEL11_21
		    break;
		}
	      case 146:
	      case 178:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_10
			PIXEL11_12
		    }
		    else
		    {
		      PIXEL01_90
			PIXEL11_61
		    }
		  PIXEL10_20
		    break;
		}
	      case 84:
	      case 85:
		{
		  PIXEL00_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL01_11
			PIXEL11_10
		    }
		    else
		    {
		      PIXEL01_60
			PIXEL11_90
		    }
		  PIXEL10_21
		    break;
		}
	      case 112:
	      case 113:
		{
		  PIXEL00_20
		    PIXEL01_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL10_12
			PIXEL11_10
		    }
		    else
		    {
		      PIXEL10_61
			PIXEL11_90
		    }
		  break;
		}
	      case 200:
	      case 204:
		{
		  PIXEL00_21
		    PIXEL01_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
			PIXEL11_11
		    }
		    else
		    {
		      PIXEL10_90
			PIXEL11_60
		    }
		  break;
		}
	      case 73:
	      case 77:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL00_12
		      PIXEL10_10
		  }
		  else
		  {
		    PIXEL00_61
		      PIXEL10_90
		  }
		  PIXEL01_20
		    PIXEL11_22
		    break;
		}
	      case 42:
	      case 170:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		      PIXEL10_11
		  }
		  else
		  {
		    PIXEL00_90
		      PIXEL10_60
		  }
		  PIXEL01_21
		    PIXEL11_20
		    break;
		}
	      case 14:
	      case 142:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		      PIXEL01_12
		  }
		  else
		  {
		    PIXEL00_90
		      PIXEL01_61
		  }
		  PIXEL10_22
		    PIXEL11_20
		    break;
		}
	      case 67:
		{
		  PIXEL00_11
		    PIXEL01_21
		    PIXEL10_21
		    PIXEL11_22
		    break;
		}
	      case 70:
		{
		  PIXEL00_22
		    PIXEL01_12
		    PIXEL10_21
		    PIXEL11_22
		    break;
		}
	      case 28:
		{
		  PIXEL00_21
		    PIXEL01_11
		    PIXEL10_22
		    PIXEL11_21
		    break;
		}
	      case 152:
		{
		  PIXEL00_21
		    PIXEL01_22
		    PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 194:
		{
		  PIXEL00_22
		    PIXEL01_21
		    PIXEL10_21
		    PIXEL11_11
		    break;
		}
	      case 98:
		{
		  PIXEL00_22
		    PIXEL01_21
		    PIXEL10_12
		    PIXEL11_22
		    break;
		}
	      case 56:
		{
		  PIXEL00_21
		    PIXEL01_22
		    PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 25:
		{
		  PIXEL00_12
		    PIXEL01_22
		    PIXEL10_22
		    PIXEL11_21
		    break;
		}
	      case 26:
	      case 31:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_20
		  }
		  PIXEL10_22
		    PIXEL11_21
		    break;
		}
	      case 82:
	      case 214:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 88:
	      case 248:
		{
		  PIXEL00_21
		    PIXEL01_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_20
		  }
		  break;
		}
	      case 74:
	      case 107:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_22
		    break;
		}
	      case 27:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_10
		    PIXEL10_22
		    PIXEL11_21
		    break;
		}
	      case 86:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_21
		    PIXEL11_10
		    break;
		}
	      case 216:
		{
		  PIXEL00_21
		    PIXEL01_22
		    PIXEL10_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 106:
		{
		  PIXEL00_10
		    PIXEL01_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_22
		    break;
		}
	      case 30:
		{
		  PIXEL00_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_22
		    PIXEL11_21
		    break;
		}
	      case 210:
		{
		  PIXEL00_22
		    PIXEL01_10
		    PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 120:
		{
		  PIXEL00_21
		    PIXEL01_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_10
		    break;
		}
	      case 75:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_21
		    PIXEL10_10
		    PIXEL11_22
		    break;
		}
	      case 29:
		{
		  PIXEL00_12
		    PIXEL01_11
		    PIXEL10_22
		    PIXEL11_21
		    break;
		}
	      case 198:
		{
		  PIXEL00_22
		    PIXEL01_12
		    PIXEL10_21
		    PIXEL11_11
		    break;
		}
	      case 184:
		{
		  PIXEL00_21
		    PIXEL01_22
		    PIXEL10_11
		    PIXEL11_12
		    break;
		}
	      case 99:
		{
		  PIXEL00_11
		    PIXEL01_21
		    PIXEL10_12
		    PIXEL11_22
		    break;
		}
	      case 57:
		{
		  PIXEL00_12
		    PIXEL01_22
		    PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 71:
		{
		  PIXEL00_11
		    PIXEL01_12
		    PIXEL10_21
		    PIXEL11_22
		    break;
		}
	      case 156:
		{
		  PIXEL00_21
		    PIXEL01_11
		    PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 226:
		{
		  PIXEL00_22
		    PIXEL01_21
		    PIXEL10_12
		    PIXEL11_11
		    break;
		}
	      case 60:
		{
		  PIXEL00_21
		    PIXEL01_11
		    PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 195:
		{
		  PIXEL00_11
		    PIXEL01_21
		    PIXEL10_21
		    PIXEL11_11
		    break;
		}
	      case 102:
		{
		  PIXEL00_22
		    PIXEL01_12
		    PIXEL10_12
		    PIXEL11_22
		    break;
		}
	      case 153:
		{
		  PIXEL00_12
		    PIXEL01_22
		    PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 58:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 83:
		{
		  PIXEL00_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_10
		    }
		    else
		    {
		      PIXEL01_70
		    }
		  PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_10
		    }
		    else
		    {
		      PIXEL11_70
		    }
		  break;
		}
	      case 92:
		{
		  PIXEL00_21
		    PIXEL01_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 202:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  PIXEL01_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  PIXEL11_11
		    break;
		}
	      case 78:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  PIXEL01_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  PIXEL11_22
		    break;
		}
	      case 154:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 114:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_10
		    }
		    else
		    {
		      PIXEL01_70
		    }
		  PIXEL10_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_10
		    }
		    else
		    {
		      PIXEL11_70
		    }
		  break;
		}
	      case 89:
		{
		  PIXEL00_12
		    PIXEL01_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 90:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_10
		  }
		  else
		  {
		    PIXEL10_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 55:
	      case 23:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL00_11
		      PIXEL01_0
		  }
		  else
		  {
		    PIXEL00_60
		      PIXEL01_90
		  }
		  PIXEL10_20
		    PIXEL11_21
		    break;
		}
	      case 182:
	      case 150:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
			PIXEL11_12
		    }
		    else
		    {
		      PIXEL01_90
			PIXEL11_61
		    }
		  PIXEL10_20
		    break;
		}
	      case 213:
	      case 212:
		{
		  PIXEL00_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL01_11
			PIXEL11_0
		    }
		    else
		    {
		      PIXEL01_60
			PIXEL11_90
		    }
		  PIXEL10_21
		    break;
		}
	      case 241:
	      case 240:
		{
		  PIXEL00_20
		    PIXEL01_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL10_12
			PIXEL11_0
		    }
		    else
		    {
		      PIXEL10_61
			PIXEL11_90
		    }
		  break;
		}
	      case 236:
	      case 232:
		{
		  PIXEL00_21
		    PIXEL01_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
			PIXEL11_11
		    }
		    else
		    {
		      PIXEL10_90
			PIXEL11_60
		    }
		  break;
		}
	      case 109:
	      case 105:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL00_12
		      PIXEL10_0
		  }
		  else
		  {
		    PIXEL00_61
		      PIXEL10_90
		  }
		  PIXEL01_20
		    PIXEL11_22
		    break;
		}
	      case 171:
	      case 43:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		      PIXEL10_11
		  }
		  else
		  {
		    PIXEL00_90
		      PIXEL10_60
		  }
		  PIXEL01_21
		    PIXEL11_20
		    break;
		}
	      case 143:
	      case 15:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		      PIXEL01_12
		  }
		  else
		  {
		    PIXEL00_90
		      PIXEL01_61
		  }
		  PIXEL10_22
		    PIXEL11_20
		    break;
		}
	      case 124:
		{
		  PIXEL00_21
		    PIXEL01_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_10
		    break;
		}
	      case 203:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_21
		    PIXEL10_10
		    PIXEL11_11
		    break;
		}
	      case 62:
		{
		  PIXEL00_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 211:
		{
		  PIXEL00_11
		    PIXEL01_10
		    PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 118:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_12
		    PIXEL11_10
		    break;
		}
	      case 217:
		{
		  PIXEL00_12
		    PIXEL01_22
		    PIXEL10_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 110:
		{
		  PIXEL00_10
		    PIXEL01_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_22
		    break;
		}
	      case 155:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_10
		    PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 188:
		{
		  PIXEL00_21
		    PIXEL01_11
		    PIXEL10_11
		    PIXEL11_12
		    break;
		}
	      case 185:
		{
		  PIXEL00_12
		    PIXEL01_22
		    PIXEL10_11
		    PIXEL11_12
		    break;
		}
	      case 61:
		{
		  PIXEL00_12
		    PIXEL01_11
		    PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 157:
		{
		  PIXEL00_12
		    PIXEL01_11
		    PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 103:
		{
		  PIXEL00_11
		    PIXEL01_12
		    PIXEL10_12
		    PIXEL11_22
		    break;
		}
	      case 227:
		{
		  PIXEL00_11
		    PIXEL01_21
		    PIXEL10_12
		    PIXEL11_11
		    break;
		}
	      case 230:
		{
		  PIXEL00_22
		    PIXEL01_12
		    PIXEL10_12
		    PIXEL11_11
		    break;
		}
	      case 199:
		{
		  PIXEL00_11
		    PIXEL01_12
		    PIXEL10_21
		    PIXEL11_11
		    break;
		}
	      case 220:
		{
		  PIXEL00_21
		    PIXEL01_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_20
		  }
		  break;
		}
	      case 158:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_20
		  }
		  PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 234:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  PIXEL01_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_11
		    break;
		}
	      case 242:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_10
		    }
		    else
		    {
		      PIXEL01_70
		    }
		  PIXEL10_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 59:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 121:
		{
		  PIXEL00_12
		    PIXEL01_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 87:
		{
		  PIXEL00_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_10
		    }
		    else
		    {
		      PIXEL11_70
		    }
		  break;
		}
	      case 79:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  PIXEL11_22
		    break;
		}
	      case 122:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_0
		  }
		  else
		  {
		    PIXEL10_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 94:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_10
		  }
		  else
		  {
		    PIXEL10_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 218:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_10
		  }
		  else
		  {
		    PIXEL10_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_20
		  }
		  break;
		}
	      case 91:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_10
		  }
		  else
		  {
		    PIXEL10_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 229:
		{
		  PIXEL00_20
		    PIXEL01_20
		    PIXEL10_12
		    PIXEL11_11
		    break;
		}
	      case 167:
		{
		  PIXEL00_11
		    PIXEL01_12
		    PIXEL10_20
		    PIXEL11_20
		    break;
		}
	      case 173:
		{
		  PIXEL00_12
		    PIXEL01_20
		    PIXEL10_11
		    PIXEL11_20
		    break;
		}
	      case 181:
		{
		  PIXEL00_20
		    PIXEL01_11
		    PIXEL10_20
		    PIXEL11_12
		    break;
		}
	      case 186:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_10
		  }
		  else
		  {
		    PIXEL01_70
		  }
		  PIXEL10_11
		    PIXEL11_12
		    break;
		}
	      case 115:
		{
		  PIXEL00_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_10
		    }
		    else
		    {
		      PIXEL01_70
		    }
		  PIXEL10_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_10
		    }
		    else
		    {
		      PIXEL11_70
		    }
		  break;
		}
	      case 93:
		{
		  PIXEL00_12
		    PIXEL01_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_10
		  }
		  else
		  {
		    PIXEL11_70
		  }
		  break;
		}
	      case 206:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  PIXEL01_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  PIXEL11_11
		    break;
		}
	      case 205:
	      case 201:
		{
		  PIXEL00_12
		    PIXEL01_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_10
		    }
		    else
		    {
		      PIXEL10_70
		    }
		  PIXEL11_11
		    break;
		}
	      case 174:
	      case 46:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_10
		  }
		  else
		  {
		    PIXEL00_70
		  }
		  PIXEL01_12
		    PIXEL10_11
		    PIXEL11_20
		    break;
		}
	      case 179:
	      case 147:
		{
		  PIXEL00_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_10
		    }
		    else
		    {
		      PIXEL01_70
		    }
		  PIXEL10_20
		    PIXEL11_12
		    break;
		}
	      case 117:
	      case 116:
		{
		  PIXEL00_20
		    PIXEL01_11
		    PIXEL10_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_10
		    }
		    else
		    {
		      PIXEL11_70
		    }
		  break;
		}
	      case 189:
		{
		  PIXEL00_12
		    PIXEL01_11
		    PIXEL10_11
		    PIXEL11_12
		    break;
		}
	      case 231:
		{
		  PIXEL00_11
		    PIXEL01_12
		    PIXEL10_12
		    PIXEL11_11
		    break;
		}
	      case 126:
		{
		  PIXEL00_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_0
		  }
		  else
		  {
		    PIXEL10_20
		  }
		  PIXEL11_10
		    break;
		}
	      case 219:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_10
		    PIXEL10_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 125:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL00_12
		      PIXEL10_0
		  }
		  else
		  {
		    PIXEL00_61
		      PIXEL10_90
		  }
		  PIXEL01_11
		    PIXEL11_10
		    break;
		}
	      case 221:
		{
		  PIXEL00_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL01_11
			PIXEL11_0
		    }
		    else
		    {
		      PIXEL01_60
			PIXEL11_90
		    }
		  PIXEL10_10
		    break;
		}
	      case 207:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		      PIXEL01_12
		  }
		  else
		  {
		    PIXEL00_90
		      PIXEL01_61
		  }
		  PIXEL10_10
		    PIXEL11_11
		    break;
		}
	      case 238:
		{
		  PIXEL00_10
		    PIXEL01_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
			PIXEL11_11
		    }
		    else
		    {
		      PIXEL10_90
			PIXEL11_60
		    }
		  break;
		}
	      case 190:
		{
		  PIXEL00_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
			PIXEL11_12
		    }
		    else
		    {
		      PIXEL01_90
			PIXEL11_61
		    }
		  PIXEL10_11
		    break;
		}
	      case 187:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		      PIXEL10_11
		  }
		  else
		  {
		    PIXEL00_90
		      PIXEL10_60
		  }
		  PIXEL01_10
		    PIXEL11_12
		    break;
		}
	      case 243:
		{
		  PIXEL00_11
		    PIXEL01_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL10_12
			PIXEL11_0
		    }
		    else
		    {
		      PIXEL10_61
			PIXEL11_90
		    }
		  break;
		}
	      case 119:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL00_11
		      PIXEL01_0
		  }
		  else
		  {
		    PIXEL00_60
		      PIXEL01_90
		  }
		  PIXEL10_12
		    PIXEL11_10
		    break;
		}
	      case 237:
	      case 233:
		{
		  PIXEL00_12
		    PIXEL01_20
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_100
		    }
		  PIXEL11_11
		    break;
		}
	      case 175:
	      case 47:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_100
		  }
		  PIXEL01_12
		    PIXEL10_11
		    PIXEL11_20
		    break;
		}
	      case 183:
	      case 151:
		{
		  PIXEL00_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_100
		    }
		  PIXEL10_20
		    PIXEL11_12
		    break;
		}
	      case 245:
	      case 244:
		{
		  PIXEL00_20
		    PIXEL01_11
		    PIXEL10_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_100
		    }
		  break;
		}
	      case 250:
		{
		  PIXEL00_10
		    PIXEL01_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_20
		  }
		  break;
		}
	      case 123:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_10
		    break;
		}
	      case 95:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_20
		  }
		  PIXEL10_10
		    PIXEL11_10
		    break;
		}
	      case 222:
		{
		  PIXEL00_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 252:
		{
		  PIXEL00_21
		    PIXEL01_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_100
		  }
		  break;
		}
	      case 249:
		{
		  PIXEL00_12
		    PIXEL01_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_100
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_20
		  }
		  break;
		}
	      case 235:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_100
		    }
		  PIXEL11_11
		    break;
		}
	      case 111:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_100
		  }
		  PIXEL01_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_20
		    }
		  PIXEL11_22
		    break;
		}
	      case 63:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_100
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_20
		  }
		  PIXEL10_11
		    PIXEL11_21
		    break;
		}
	      case 159:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_100
		  }
		  PIXEL10_22
		    PIXEL11_12
		    break;
		}
	      case 215:
		{
		  PIXEL00_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_100
		    }
		  PIXEL10_21
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 246:
		{
		  PIXEL00_22
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  PIXEL10_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_100
		    }
		  break;
		}
	      case 254:
		{
		  PIXEL00_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_20
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_0
		  }
		  else
		  {
		    PIXEL10_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_100
		  }
		  break;
		}
	      case 253:
		{
		  PIXEL00_12
		    PIXEL01_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_100
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_100
		  }
		  break;
		}
	      case 251:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  PIXEL01_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_100
		    }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_20
		  }
		  break;
		}
	      case 239:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_100
		  }
		  PIXEL01_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		    {
		      PIXEL10_0
		    }
		    else
		    {
		      PIXEL10_100
		    }
		  PIXEL11_11
		    break;
		}
	      case 127:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_100
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_0
		  }
		  else
		  {
		    PIXEL10_20
		  }
		  PIXEL11_10
		    break;
		}
	      case 191:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_100
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_100
		  }
		  PIXEL10_11
		    PIXEL11_12
		    break;
		}
	      case 223:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_20
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_100
		  }
		  PIXEL10_10
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_20
		    }
		  break;
		}
	      case 247:
		{
		  PIXEL00_11
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		    {
		      PIXEL01_0
		    }
		    else
		    {
		      PIXEL01_100
		    }
		  PIXEL10_12
		    if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		    {
		      PIXEL11_0
		    }
		    else
		    {
		      PIXEL11_100
		    }
		  break;
		}
	      case 255:
		{
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[4], w[2]))
		  {
		    PIXEL00_0
		  }
		  else
		  {
		    PIXEL00_100
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[2], w[6]))
		  {
		    PIXEL01_0
		  }
		  else
		  {
		    PIXEL01_100
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[8], w[4]))
		  {
		    PIXEL10_0
		  }
		  else
		  {
		    PIXEL10_100
		  }
		  if (Diff(YUV1, YUV2, RGBtoYUV, trY, trU, trV, Ymask, Umask, Vmask, w[6], w[8]))
		  {
		    PIXEL11_0
		  }
		  else
		  {
		    PIXEL11_100
		  }
		  break;
		}

	    }
	  }
	}

	trace::exiting("upsampling::impl::hq2x_");
	return output;
      }

    } // end of namespace mln::upsampling::impl


    template <typename I>
    inline
    mln_concrete(I)
    hq2x(const Image<I>& input)
    {
      trace::entering("upsampling::hq2x");
      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output;

      // Preparing extension.
      extension::adjust_duplicate(input, 1);

      output = impl::hq2x_(exact(input));

      trace::exiting("upsampling::hq2x");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::upsampling

} // end of namespace mln


#endif // ! MLN_UPSAMPLING_HQ2X_HH
