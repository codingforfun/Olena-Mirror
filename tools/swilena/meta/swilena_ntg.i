%module swilena_ntg
%include swilena_exception.i


/***** Generic class declaration for scalars ******/
%define decl_scalar_class(Ns, Name, ValueType)
template<unsigned nbits, typename behaviour>
class Name
{
 public:
  Name();
  Name(ValueType);

  %extend {
    ValueType value() const
      { return (*self); }
    void value(ValueType x) 
      { *self = Ns::Name<nbits, behaviour>(x); }

#if defined(SWIGPYTHON)
    Ns::Name<nbits, behaviour> operator+ (const Ns::Name<nbits, behaviour>& other) const { return (*self) + other; }
    Ns::Name<nbits, behaviour> operator- (const Ns::Name<nbits, behaviour>& other) const { return (*self) - other; }
    Ns::Name<nbits, behaviour> operator* (const Ns::Name<nbits, behaviour>& other) const { return (*self) * other; }
    Ns::Name<nbits, behaviour> operator/ (const Ns::Name<nbits, behaviour>& other) const { return (*self) / other; }
    Ns::Name<nbits, behaviour> operator+ (ValueType other) const { return (*self) + other; }
    Ns::Name<nbits, behaviour> operator- (ValueType other) const { return (*self) - other; }
    Ns::Name<nbits, behaviour> operator* (ValueType other) const { return (*self) * other; }
    Ns::Name<nbits, behaviour> operator/ (ValueType other) const { return (*self) / other; }
    Ns::Name<nbits, behaviour>& operator+= (const Ns::Name<nbits, behaviour>& other) { return (*self) += other; }
    Ns::Name<nbits, behaviour>& operator-= (const Ns::Name<nbits, behaviour>& other) { return (*self) -= other; }
    Ns::Name<nbits, behaviour>& operator*= (const Ns::Name<nbits, behaviour>& other) { return (*self) *= other; }
    Ns::Name<nbits, behaviour>& operator/= (const Ns::Name<nbits, behaviour>& other) { return (*self) /= other; }
    Ns::Name<nbits, behaviour>& operator+= (ValueType other) { return (*self) += other; }
    Ns::Name<nbits, behaviour>& operator-= (ValueType other) { return (*self) -= other; }
    Ns::Name<nbits, behaviour>& operator*= (ValueType other) { return (*self) *= other; }
    Ns::Name<nbits, behaviour>& operator/= (ValueType other) { return (*self) /= other; }
    bool operator< (const Ns::Name<nbits, behaviour>&  other) const { return (*self) < other; }
    bool operator> (const Ns::Name<nbits, behaviour>&  other) const { return (*self) > other; }
    bool operator<= (const Ns::Name<nbits, behaviour>&  other) const { return (*self) <= other; }
    bool operator>= (const Ns::Name<nbits, behaviour>&  other) const { return (*self) >= other; }
    bool operator== (const Ns::Name<nbits, behaviour>&  other) const { return (*self) == other; }
    bool operator!= (const Ns::Name<nbits, behaviour>&  other) const { return (*self) != other; }
    bool operator< (ValueType other) const { return (*self) < other; }
    bool operator> (ValueType other) const { return (*self) > other; }
    bool operator<= (ValueType other) const { return (*self) <= other; }
    bool operator>= (ValueType other) const { return (*self) >= other; }
    bool operator== (ValueType other) const { return (*self) == other; }
    bool operator!= (ValueType other) const { return (*self) != other; }
#else
    Ns::Name<nbits, behaviour> add (const Ns::Name<nbits, behaviour>& other) const { return (*self) + other; }
    Ns::Name<nbits, behaviour> sub (const Ns::Name<nbits, behaviour>& other) const { return (*self) - other; }
    Ns::Name<nbits, behaviour> mul (const Ns::Name<nbits, behaviour>& other) const { return (*self) * other; }
    Ns::Name<nbits, behaviour> div (const Ns::Name<nbits, behaviour>& other) const { return (*self) / other; }
    Ns::Name<nbits, behaviour> add (ValueType other) const { return (*self) + other; }
    Ns::Name<nbits, behaviour> sub (ValueType other) const { return (*self) - other; }
    Ns::Name<nbits, behaviour> mul (ValueType other) const { return (*self) * other; }
    Ns::Name<nbits, behaviour> div (ValueType other) const { return (*self) / other; }
    Ns::Name<nbits, behaviour>& iadd (const Ns::Name<nbits, behaviour>& other) const { return (*self) += other; }
    Ns::Name<nbits, behaviour>& isub (const Ns::Name<nbits, behaviour>& other) const { return (*self) -= other; }
    Ns::Name<nbits, behaviour>& imul (const Ns::Name<nbits, behaviour>& other) const { return (*self) *= other; }
    Ns::Name<nbits, behaviour>& idiv (const Ns::Name<nbits, behaviour>& other) const { return (*self) /= other; }
    Ns::Name<nbits, behaviour>& iadd (ValueType other) const { return (*self) += other; }
    Ns::Name<nbits, behaviour>& isub (ValueType other) const { return (*self) -= other; }
    Ns::Name<nbits, behaviour>& imul (ValueType other) const { return (*self) *= other; }
    Ns::Name<nbits, behaviour>& idiv (ValueType other) const { return (*self) /= other; }
    bool lt (const Ns::Name<nbits, behaviour>&  other) const { return (*self) < other; }
    bool gt (const Ns::Name<nbits, behaviour>&  other) const { return (*self) > other; }
    bool le (const Ns::Name<nbits, behaviour>&  other) const { return (*self) <= other; }
    bool ge (const Ns::Name<nbits, behaviour>&  other) const { return (*self) >= other; }
    bool eq (const Ns::Name<nbits, behaviour>&  other) const { return (*self) == other; }
    bool ne (const Ns::Name<nbits, behaviour>&  other) const { return (*self) != other; }
    bool lt (ValueType other) const { return (*self) < other; }
    bool gt (ValueType other) const { return (*self) > other; }
    bool le (ValueType other) const { return (*self) <= other; }
    bool ge (ValueType other) const { return (*self) >= other; }
    bool eq (ValueType other) const { return (*self) == other; }
    bool ne (ValueType other) const { return (*self) != other; }
#endif    
  }

  EXTEND_DESCRIBE2(Ns::Name<nbits, behaviour>);
};  
%enddef

/****** Generic class declaration for booleans *****/
%define decl_bin_class(Ns, Name, ValueType)
class Name
{
 public:
  Name();
  Name(ValueType);

  %extend {
    ValueType value() const
      { return (*self); }
    void value(ValueType x) 
      { *self = Ns::Name(x); }

#if defined(SWIGPYTHON)
    Ns::Name operator| (const Ns::Name& other) const { return (*self) | other; }
    Ns::Name operator| (ValueType other) const { return (*self) | other; }
    Ns::Name operator& (const Ns::Name& other) const { return (*self) & other; }
    Ns::Name operator& (ValueType other) const { return (*self) & other; }
    Ns::Name operator^ (const Ns::Name& other) const { return (*self) ^ other; }
    Ns::Name operator^ (ValueType other) const { return (*self) ^ other; }
    Ns::Name& operator|= (const Ns::Name& other) { return (*self) |= other; }
    Ns::Name& operator|= (ValueType other) { return (*self) |= other; }
    Ns::Name& operator&= (const Ns::Name& other) { return (*self) &= other; }
    Ns::Name& operator&= (ValueType other) { return (*self) &= other; }
    Ns::Name& operator^= (const Ns::Name& other) { return (*self) ^= other; }
    Ns::Name& operator^= (ValueType other) { return (*self) ^= other; }
    bool operator< (const Ns::Name&  other) const { return (*self) < other; }
    bool operator== (const Ns::Name&  other) const { return (*self) == other; }
    bool operator!= (const Ns::Name&  other) const { return (*self) != other; }
    bool operator< (ValueType other) const { return (*self) < other; }
    bool operator== (ValueType other) const { return (*self) == other; }
    bool operator!= (ValueType other) const { return (*self) != other; }
#else
    Ns::Name lor (const Ns::Name& other) const { return (*self) | other; }
    Ns::Name lor (ValueType other) const { return (*self) | other; }
    Ns::Name land (const Ns::Name& other) const { return (*self) & other; }
    Ns::Name land (ValueType other) const { return (*self) & other; }
    Ns::Name lxor (const Ns::Name& other) const { return (*self) ^ other; }
    Ns::Name lxor (ValueType other) const { return (*self) ^ other; }
    Ns::Name& ilor (const Ns::Name& other) { return (*self) |= other; }
    Ns::Name& ilor (ValueType other) { return (*self) |= other; }
    Ns::Name& iland (const Ns::Name& other) { return (*self) &= other; }
    Ns::Name& iland (ValueType other) { return (*self) &= other; }
    Ns::Name& ilxor (const Ns::Name& other) { return (*self) ^= other; }
    Ns::Name& iixor (ValueType other) { return (*self) ^= other; }
    bool lt (const Ns::Name&  other) const { return (*self) < other; }
    bool eq (const Ns::Name&  other) const { return (*self) == other; }
    bool ne (const Ns::Name&  other) const { return (*self) != other; }
    bool lt (ValueType other) const { return (*self) < other; }
    bool eq (ValueType other) const { return (*self) == other; }
    bool ne (ValueType other) const { return (*self) != other; }
#endif

  }

  EXTEND_DESCRIBE(Ns::Name);
};  
%enddef
    
%define decl_complex()
template<typename repr, typename T>
class cplx
{
 public:
  cplx(const T& p1 = 0, const T& p2 = 0);
  cplx(const ntg::cplx<repr, T>& other);

  const float_d real() const;
  const float_d imag() const;
  const T magn() const;
  const T angle() const;
  const T first() const;
  const T second() const;
  
  ntg::cplx<repr, T> conj() const;

  %extend {
    ntg::cplx<repr, T>& set(const T& p1, const T& p2)
      { return (*self) = ntg::cplx<repr, T>(p1, p2); }
    ntg::cplx<repr, T>& set(const ntg::cplx<repr, T>& other)
      { return (*self) = ntg::cplx<repr, T>(other); }
  }

  EXTEND_DESCRIBE2(ntg::cplx<repr, T>)
};
%enddef

%define decl_ntg()
%include <ntg/vect/cplx_representation.hh>
%include <ntg/core/predecls.hh>
%{
#include <ntg/all.hh>
%}

%include swilena_describe.i

namespace ntg
{

  decl_scalar_class(ntg, int_u, unsigned long)
  decl_scalar_class(ntg, int_s, long)
  decl_bin_class(ntg, bin, bool)

  decl_complex()

}
%enddef

decl_ntg()
