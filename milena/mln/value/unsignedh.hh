// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_VALUE_UNSIGNEDH_HH
# define MLN_VALUE_UNSIGNEDH_HH

/// \file
///
/// Define a semi-unsigned value class.

# include <cstdlib>
# include <iostream>
# include <sstream>

// FIXME: parameterize unsignedh so that we also can have
// Z/4 and so on...

namespace mln
{

  namespace value
  {


    class unsignedh
    {
    public:
      unsignedh();
      unsignedh(const unsignedh& rhs);
      unsignedh(unsigned i);
      unsignedh(int i);

      unsignedh& operator=(const unsignedh& rhs);
      unsignedh& operator=(unsigned i);
      unsignedh& operator=(int i);

      unsigned enc() const;

      /// Construct an unsignedh with value v
      static unsignedh make(unsigned v);
      /// Construct an unsignedh with value v. The given value is
      /// rounded to the closest integer or half value.
//      static unsignedh make(float v);

      bool is_integer() const;

      unsigned to_unsigned() const;

      /// Set this unsignedh to its next value and return itself.
      unsignedh& goto_succ();

      /// Set this unsignedh to its previous value and return itself.
      unsignedh& goto_pred();

      operator std::string() const;

    private:
      unsigned enc_;
    };


    extern const unsignedh half;


//  next

    unsignedh succ(unsignedh i);
    unsignedh pred(unsignedh i);

//  rounding

    unsignedh floor(unsignedh i);
    unsignedh ceil(unsignedh i);


//  comparison

    bool operator==(unsignedh l, unsignedh r);
    bool operator<=(unsignedh l, unsignedh r);
    bool operator!=(unsignedh l, unsignedh r);
    bool operator>=(unsignedh l, unsignedh r);
    bool operator>(unsignedh l, unsignedh r);
    bool operator<(unsignedh l, unsignedh r);

// arithmetics

    unsignedh operator+(unsignedh l, unsignedh r);
    void operator+=(unsignedh& l, unsignedh r);
    unsignedh operator-(unsignedh l, unsignedh r);
    void operator-=(unsignedh& l, unsignedh r);
    unsignedh operator/(unsignedh l, unsigned r);

//  other ops

    unsignedh min(unsignedh u1, unsignedh u2);
    unsignedh max(unsignedh u1, unsignedh u2);
    unsignedh mean(unsignedh u1, unsignedh u2);

// for a transparent use of both unsigned and unsignedh
// FIXME: is it useful?

    unsigned decode(unsigned u);
    void encode(unsigned u, unsigned& dst);
    unsigned decode(unsignedh u);
    void encode(unsigned u, unsignedh& dst);

//  <<

    std::ostream&
    operator<<(std::ostream& ostr, const unsignedh& i);


# ifndef MLN_INCLUDE_ONLY

    inline
    unsignedh::unsignedh()
    {
    }

    inline
    unsignedh::unsignedh(const unsignedh& rhs)
      : enc_(rhs.enc_)
    {
    }


    inline
    unsignedh::unsignedh(unsigned i)
    {
      enc_ = 2 * i;
    }

    inline
    unsignedh::unsignedh(int i)
    {
      if (i < 0)
	std::abort();
      enc_ = 2 * i;
    }

    inline
    unsignedh&
    unsignedh::operator=(const unsignedh& rhs)
    {
      enc_ = rhs.enc_;
      return *this;
    }

    inline
    unsignedh&
    unsignedh::operator=(unsigned i)
    {
      enc_ = 2 * i;
      return *this;
    }

    inline
    unsignedh&
    unsignedh::operator=(int i)
    {
      if (i < 0)
	std::abort();
      enc_ = 2 * i;
      return *this;
    }

    inline
    unsigned
    unsignedh::enc() const
    {
      return enc_;
    }

    inline
    unsignedh unsignedh::make(unsigned enc)
    {
      unsignedh i;
      i.enc_ = enc;
      return i;
    }

    inline
    bool
    unsignedh::is_integer() const
    {
      return enc_ % 2 == 0;
    }

    inline
    unsigned
    unsignedh::to_unsigned() const
    {
      if (! this->is_integer())
	std::abort();
      return enc_ / 2;
    }

    inline
    unsignedh&
    unsignedh::goto_succ()
    {
      enc_ += 1;
      return *this;
    }

    inline
    unsignedh&
    unsignedh::goto_pred()
    {
      if (enc_ == 0)
	std::abort();
      enc_ -= 1;
      return *this;
    }

    inline
    unsignedh::operator std::string() const
    {
      std::stringstream s;
      s << *this;
      return s.str();
    }



//  half

#  ifndef MLN_WO_GLOBAL_VARS

    const unsignedh half =  unsignedh::make(1u);

#  endif // ! MLN_WO_GLOBAL_VARS


//  next

    unsignedh succ(unsignedh i)
    {
      return unsignedh::make(i.enc() + 1);
    }

    unsignedh pred(unsignedh i)
    {
      if (i.enc() == 0)
	std::abort();
      return unsignedh::make(i.enc() - 1);
    }



//  rounding

    unsignedh floor(unsignedh i)
    {
      return i.is_integer() ? i : pred(i);
    }

    unsignedh ceil(unsignedh i)
    {
      return i.is_integer() ? i : succ(i);
    }



//  comparison

    bool operator==(unsignedh l, unsignedh r)
    {
      return l.enc() == r.enc();
    }

    bool operator<=(unsignedh l, unsignedh r)
    {
      return l.enc() <= r.enc();
    }

    bool operator!=(unsignedh l, unsignedh r)
    {
      return ! (l == r);
    }

    bool operator>=(unsignedh l, unsignedh r)
    {
      return r <= l;
    }

    bool operator>(unsignedh l, unsignedh r)
    {
      return ! (l <= r);
    }

    bool operator<(unsignedh l, unsignedh r)
    {
      return r > l;
    }



// arithmetics

    unsignedh operator+(unsignedh l, unsignedh r)
    {
      return unsignedh::make(l.enc() + r.enc());
    }

    void operator+=(unsignedh& l, unsignedh r)
    {
      l = l + r;
    }

    unsignedh operator-(unsignedh l, unsignedh r)
    {
      if (r > l)
	std::abort();
      return unsignedh::make(l.enc() - r.enc());
    }

    void operator-=(unsignedh& l, unsignedh r)
    {
      if (r > l)
	std::abort();
      l = l - r;
    }

    unsignedh operator/(unsignedh l, unsigned r)
    {
      if (l.enc() % r != 0)
	std::abort();
      return unsignedh::make(l.enc() / r);
    }



//  other ops

    unsignedh min(unsignedh u1, unsignedh u2)
    {
      return unsignedh::make(u1.enc() < u2.enc() ? u1.enc() : u2.enc());
    }

    unsignedh max(unsignedh u1, unsignedh u2)
    {
      return unsignedh::make(u1.enc() > u2.enc() ? u1.enc() : u2.enc());
    }

    unsignedh mean(unsignedh u1, unsignedh u2)
    {
      return (u1 + u2) / 2;
    }


// for a transparent use of both unsigned and unsignedh
// FIXME: is it useful?

    unsigned decode(unsigned u)
    {
      return u;
    }

    void encode(unsigned u, unsigned& dst)
    {
      dst = u;
    }

    unsigned decode(unsignedh u)
    {
      return u.enc();
    }

    void encode(unsigned u, unsignedh& dst)
    {
      dst = unsignedh::make(u);
    }


//  <<

    std::ostream&
    operator<<(std::ostream& ostr, const unsignedh& i)
    {
      if (i.is_integer())
	return ostr << i.to_unsigned();
      else
	return ostr << floor(i).to_unsigned() << ".5";
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln

#endif // ndef UNSIGNEDH_HH
