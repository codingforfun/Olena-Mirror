// -*- C++ -*-
%{
#include <ntg/all.hh>
#include <sstream>
%}
%include std_string.i

%define CommonOperators(Name)
  Name();
%extend {
  bool operator==(const Name& other) const
    { return (*self) == other; }
  bool equals(const Name& other) const
    { return (*self) == other; }
  std::string describe() const
    {
      std::ostringstream o;
      o << (*self);
      return o.str();
    }
};
%enddef

%define ScalarOperators(Name, Interface)
  Name(Interface);
%extend { 
  Interface value() const { return (*self); }
  void value(Interface x) { *self = oln::Name(x); }
  
  Name& operator+=(const Name& other)
    { return (*self) += other; }
  Name& operator+=(Interface other)
    { return (*self) += oln:: Name(other); }
  Name operator+(const Name& other) const
    { return (*self) + other; }
  Name operator+(Interface other) const
    { return (*self) + oln:: Name(other); }
  Name add(const Name& other) const
    { return (*self) + other; }
  Name add(Interface other) const
    { return (*self) + oln:: Name(other); }
       
  Name& operator-=(const Name& other)
    { return (*self) -= other; }
  Name& operator-=(Interface other)
    { return (*self) -= oln:: Name(other); }
  Name operator-(const Name& other) const
    { return (*self) - other; }
  Name operator-(Interface other) const
    { return (*self) - oln:: Name(other); }
  Name sub(const Name& other) const
    { return (*self) - other; }
  Name sub(Interface other) const
    { return (*self) - oln:: Name(other); }

  Name& operator*=(const Name& other)
    { return (*self) *= other; }
  Name& operator*=(Interface other)
    { return (*self) *= oln:: Name(other); }
  Name operator*(const Name& other) const
    { return (*self) * other; }
  Name operator*(Interface other) const
    { return (*self) * oln:: Name(other); }
  Name mul(const Name& other) const
    { return (*self) * other; }
  Name mul(Interface other) const
    { return (*self) * oln:: Name(other); }

  Name& operator/=(const Name& other)
    { return (*self) /= other; }
  Name& operator/=(Interface other)
    { return (*self) /= oln:: Name(other); }
  Name operator/(const Name& other) const
    { return (*self) / other; }
  Name operator/(Interface other) const
    { return (*self) / oln:: Name(other); }
  Name div(const Name& other) const
    { return (*self) / other; }
  Name div(Interface other) const
    { return (*self) / oln:: Name(other); }

  bool operator<=(const Name& other) const
    { return (*self) <= other; }
  bool operator>=(const Name& other) const
    { return (*self) >= other; }
  bool operator<(const Name& other) const
    { return (*self) < other; }
  bool operator>(const Name& other) const
    { return (*self) > other; }
  bool operator<=(Interface other) const
    { return (*self) <= oln:: Name(other); }
  bool operator>=(Interface other) const
    { return (*self) >= oln:: Name(other); }
  bool operator<(Interface other) const
    { return (*self) < oln:: Name(other); }
  bool operator>(Interface other) const
    { return (*self) > oln:: Name(other); }

  bool operator==(Interface other) const
    { return (*self) == oln:: Name(other); }
  bool equals(Interface other) const
    { return (*self) == oln:: Name(other); }

};
%enddef

%define MakeValue(Name)
  namespace oln
  {
    struct Name 
    {
      CommonOperators(Name);
      ScalarOperators(Name, int);
    };

  }  
%enddef

%define MakeFloatValue(Name)
  namespace oln
  {
    struct Name 
    {
      CommonOperators(Name);
      ScalarOperators(Name, double);
    };

  }  
%enddef

%define MakeRgbValue(Name)
  namespace oln
  {
    struct Name
    {
      CommonOperators(Name);
      %extend {
	float r() const { return (*self)[0]; }
	float g() const { return (*self)[1]; }
	float b() const { return (*self)[2]; }
	void r(float v) { (*self)[0] = v; }
	void g(float v) { (*self)[1] = v; }
	void b(float v) { (*self)[2] = v; }
	float color(int comp) const { return (*self)[comp]; }
	void color(int comp, float v) { (*self)[comp] = v; }
      };

    };
  }
%enddef

namespace oln
{
  struct bin
  {
    CommonOperators(bin);
    bin(bool);
    
    %extend {
      bool value() { return (*self); }
      void value(bool v) { (*self) = v; }

      bin operator&(const bin& other) const
	{ return (*self) & other; }
      bin operator^(const bin& other) const
	{ return (*self) ^ other; }
      bin operator|(const bin& other) const
	{ return (*self) | other; }
      bin& operator&=(const bin& other)
	{ return (*self) &= other; }
      bin& operator^=(const bin& other)
	{ return (*self) ^= other; }
      bin& operator|=(const bin& other)
	{ return (*self) |= other; }

      bin operator&(bool other) const
	{ return (*self) & other; }
      bin operator^(bool other) const
	{ return (*self) ^ other; }
      bin operator|(bool other) const
	{ return (*self) | other; }
      bin& operator&=(bool other)
	{ return (*self) &= other; }
      bin& operator^=(bool other)
	{ return (*self) ^= other; }
      bin& operator|=(bool other)
	{ return (*self) |= other; }

      bool operator==(bool other) const
	{ return (*self) == other; }
      bool equals(bool other) const
	{ return (*self) == other; }
    };
  };
}

	
