// Copyright (C) 2004  EPITA Research and Development Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_OLN_UTILS_MD5_HXX
# define OLENA_OLN_UTILS_MD5_HXX

// Ctor.
inline
MD5::MD5(buffer b): buffer_(b),
		    X(16),
		    T(),
		    current_(0),
		    a(0x67452301),
		    b(0xefcdab89),
		    c(0x98badcfe),
		    d(0x10325476)
  // 		 a(0x01234567),
  // 		 b(0x89abcdef),
  // 		 c(0xfedcba98),
  // 		 d(0x76543210)
{
  precondition(buffer_.is_append_padding());
  precondition(buffer_.is_append_length());
  T.reserve(64);
  T.push_back(0xd76aa478); /* 1 */
  T.push_back(0xe8c7b756); /* 2 */
  T.push_back(0x242070db); /* 3 */
  T.push_back(0xc1bdceee); /* 4 */
  T.push_back(0xf57c0faf); /* 5 */
  T.push_back(0x4787c62a); /* 6 */
  T.push_back(0xa8304613); /* 7 */
  T.push_back(0xfd469501); /* 8 */
  T.push_back(0x698098d8); /* 9 */
  T.push_back(0x8b44f7af); /* 10 */
  T.push_back(0xffff5bb1); /* 11 */
  T.push_back(0x895cd7be); /* 12 */
  T.push_back(0x6b901122); /* 13 */
  T.push_back(0xfd987193); /* 14 */
  T.push_back(0xa679438e); /* 15 */
  T.push_back(0x49b40821); /* 16 */
  T.push_back(0xf61e2562); /* 17 */
  T.push_back(0xc040b340); /* 18 */
  T.push_back(0x265e5a51); /* 19 */
  T.push_back(0xe9b6c7aa); /* 20 */
  T.push_back(0xd62f105d); /* 21 */
  T.push_back(0x2441453); /* 22 */
  T.push_back(0xd8a1e681); /* 23 */
  T.push_back(0xe7d3fbc8); /* 24 */
  T.push_back(0x21e1cde6); /* 25 */
  T.push_back(0xc33707d6); /* 26 */
  T.push_back(0xf4d50d87); /* 27 */
  T.push_back(0x455a14ed); /* 28 */
  T.push_back(0xa9e3e905); /* 29 */
  T.push_back(0xfcefa3f8); /* 30 */
  T.push_back(0x676f02d9); /* 31 */
  T.push_back(0x8d2a4c8a); /* 32 */
  T.push_back(0xfffa3942); /* 33 */
  T.push_back(0x8771f681); /* 34 */
  T.push_back(0x6d9d6122); /* 35 */
  T.push_back(0xfde5380c); /* 36 */
  T.push_back(0xa4beea44); /* 37 */
  T.push_back(0x4bdecfa9); /* 38 */
  T.push_back(0xf6bb4b60); /* 39 */
  T.push_back(0xbebfbc70); /* 40 */
  T.push_back(0x289b7ec6); /* 41 */
  T.push_back(0xeaa127fa); /* 42 */
  T.push_back(0xd4ef3085); /* 43 */
  T.push_back(0x4881d05); /* 44 */
  T.push_back(0xd9d4d039); /* 45 */
  T.push_back(0xe6db99e5); /* 46 */
  T.push_back(0x1fa27cf8); /* 47 */
  T.push_back(0xc4ac5665); /* 48 */
  T.push_back(0xf4292244); /* 49 */
  T.push_back(0x432aff97); /* 50 */
  T.push_back(0xab9423a7); /* 51 */
  T.push_back(0xfc93a039); /* 52 */
  T.push_back(0x655b59c3); /* 53 */
  T.push_back(0x8f0ccc92); /* 54 */
  T.push_back(0xffeff47d); /* 55 */
  T.push_back(0x85845dd1); /* 56 */
  T.push_back(0x6fa87e4f); /* 57 */
  T.push_back(0xfe2ce6e0); /* 58 */
  T.push_back(0xa3014314); /* 59 */
  T.push_back(0x4e0811a1); /* 60 */
  T.push_back(0xf7537e82); /* 61 */
  T.push_back(0xbd3af235); /* 62 */
  T.push_back(0x2ad7d2bb); /* 63 */
  T.push_back(0xeb86d391); /* 64 */
};

// sav() implementation.
inline
void MD5::sav()
{
  for (unsigned i = 0; i < 16; ++i)
    X[i] = buffer_[current_ + i];
  current_ += 16;
  aa = a;
  bb = b;
  cc = c;
  dd = d;
}

// round1() implementation.
inline
void MD5::round1()
{
  sub_step(F, a, b, c, d, 0, 7, 1);
  sub_step(F, d, a, b, c, 1, 12, 2);
  sub_step(F, c, d, a, b, 2, 17, 3);
  sub_step(F, b, c, d, a, 3, 22, 4);
  sub_step(F, a, b, c, d, 4, 7, 5);
  sub_step(F, d, a, b, c, 5, 12, 6);
  sub_step(F, c, d, a, b, 6, 17, 7);
  sub_step(F, b, c, d, a, 7, 22, 8);
  sub_step(F, a, b, c, d, 8, 7, 9);
  sub_step(F, d, a, b, c, 9, 12, 10);
  sub_step(F, c, d, a, b, 10, 17, 11);
  sub_step(F, b, c, d, a, 11, 22, 12);
  sub_step(F, a, b, c, d, 12, 7, 13);
  sub_step(F, d, a, b, c, 13, 12, 14);
  sub_step(F, c, d, a, b, 14, 17, 15);
  sub_step(F, b, c, d, a, 15, 22, 16);
}

// round2() implementation.
inline
void MD5::round2()
{
  sub_step(G, a, b, c, d, 1, 5, 17);
  sub_step(G, d, a, b, c, 6, 9, 18);
  sub_step(G, c, d, a, b, 11, 14, 19);
  sub_step(G, b, c, d, a, 0, 20, 20);
  sub_step(G, a, b, c, d, 5, 5, 21);
  sub_step(G, d, a, b, c, 10, 9, 22);
  sub_step(G, c, d, a, b, 15, 14, 23);
  sub_step(G, b, c, d, a, 4, 20, 24);
  sub_step(G, a, b, c, d, 9, 5, 25);
  sub_step(G, d, a, b, c, 14, 9, 26);
  sub_step(G, c, d, a, b, 3, 14, 27);
  sub_step(G, b, c, d, a, 8, 20, 28);
  sub_step(G, a, b, c, d, 13, 5, 29);
  sub_step(G, d, a, b, c, 2, 9, 30);
  sub_step(G, c, d, a, b, 7, 14, 31);
  sub_step(G, b, c, d, a, 12, 20, 32);
}

// round3() implementation.
inline
void MD5::round3()
{
  sub_step(H, a, b, c, d,  5,  4, 33);
  sub_step(H, d, a, b, c,  8, 11, 34);
  sub_step(H, c, d, a, b, 11, 16, 35);
  sub_step(H, b, c, d, a, 14, 23, 36);
  sub_step(H, a, b, c, d,  1,  4, 37);
  sub_step(H, d, a, b, c,  4, 11, 38);
  sub_step(H, c, d, a, b,  7, 16, 39);
  sub_step(H, b, c, d, a, 10, 23, 40);
  sub_step(H, a, b, c, d, 13,  4, 41);
  sub_step(H, d, a, b, c,  0, 11, 42);
  sub_step(H, c, d, a, b,  3, 16, 43);
  sub_step(H, b, c, d, a,  6, 23, 44);
  sub_step(H, a, b, c, d,  9,  4, 45);
  sub_step(H, d, a, b, c, 12, 11, 46);
  sub_step(H, c, d, a, b, 15, 16, 47);
  sub_step(H, b, c, d, a,  2, 23, 48);
}

// round 4() implementation.
inline
void MD5::round4()
{
  sub_step(I, a, b, c, d,  0,  6, 49);
  sub_step(I, d, a, b, c,  7, 10, 50);
  sub_step(I, c, d, a, b, 14, 15, 51);
  sub_step(I, b, c, d, a,  5, 21, 52);
  sub_step(I, a, b, c, d, 12,  6, 53);
  sub_step(I, d, a, b, c,  3, 10, 54);
  sub_step(I, c, d, a, b, 10, 15, 55);
  sub_step(I, b, c, d, a,  1, 21, 56);
  sub_step(I, a, b, c, d,  8,  6, 57);
  sub_step(I, d, a, b, c, 15, 10, 58);
  sub_step(I, c, d, a, b,  6, 15, 59);
  sub_step(I, b, c, d, a, 13, 21, 60);
  sub_step(I, a, b, c, d,  4,  6, 61);
  sub_step(I, d, a, b, c, 11, 10, 62);
  sub_step(I, c, d, a, b,  2, 15, 63);
  sub_step(I, b, c, d, a,  9, 21, 64);
}

// step  implementation.
inline
void MD5::step()
{
  sav();
  round1();
  round2();
  round3();
  round4();
  a += aa;
  b += bb;
  c += cc;
  d += dd;
};

// sub step implementation.
template <class Fun>
inline
void MD5::sub_step(const Fun &fun,
		   storage_type &a,
		   const storage_type &b,
		   const storage_type &c,
		   const storage_type &d,
		   unsigned k,
		   unsigned s,
		   unsigned i)
{
  a += fun(b, c, d) + (X[k] + T[i - 1]);
  a = a<<s | a>>(32-s);
  a += b;
  //a = b + ((a + fun(b, c, d) + X[k] + T[i - 1]) << s);
}

// gen_sub_key implementation.
inline
void MD5::gen_sub_key(std::vector<key::value_type> &v,
		      storage_type x,
		      unsigned base)
{
  base *= sizeof(value_type);

  for (unsigned i = 0; i < sizeof(value_type); ++i)
    v[base + i] = (x & (0xFF << (i * 8))) >> (i * 8);
}

// gen_key implementation.

inline
key MD5::gen_key()
{
  std::vector<key::value_type>	k;

  k.reserve(16);
  gen_sub_key(k, a, 0);
  gen_sub_key(k, b, 1);
  gen_sub_key(k, c, 2);
  gen_sub_key(k, d, 3);
  return key(k);
}

// execute implementation.
inline
key MD5::execute()
{
  while (buffer_.size() > current_)
    step();
  return gen_key();
}


// md5 implementation.
template <class I>
inline
key md5(const abstract::non_vectorial_image<I> &im)
{
  oln_iter_type(I)	p(im);
  buffer	b;

  for_all(p)
    b.add(im[p]);
  b.append_padding();
  b.append_length();

  oln::utils::MD5	md5(b);
  return md5.execute();
}

// md5 implementation.
template <class I>
inline
key md5(const abstract::vectorial_image<I> &im)
{
  oln_iter_type(I)	p(im);
  buffer	b;
  //  unsigned nb =

  for_all(p)
    {
      for (unsigned i = 0; i < ntg_nb_comp(oln_value_type(I)); ++i)
	b.add(im[p][i]);
    }
  b.append_padding();
  b.append_length();

  oln::utils::MD5	md5(b);
  return md5.execute();
}

#endif // !OLENA_OLN_UTILS_MD5_HXX
