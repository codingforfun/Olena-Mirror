#include <string>
#include <iostream>
#include <mlc/contract.hh>
#include "tracked_ptr.hh"

namespace oln
{
  struct array2d { int i; };

  struct image2d
  {
      image2d() { // ctor
      }
      image2d(int i) { // ctor
	data_ = new array2d; // réalise : data_.ptr_ = new array2d
	data_-> i = i; // correspond à : data_.operator->().i
	// réalise      : data_.ptr_->i = i
      }
      // l'attribut :
      internal::tracked_ptr<array2d> data_;

      void echo(const std::string& name) const {
	std::cout << name << " = { data_ -> "
		  << data_.ptr_ << " }" << std::endl;
      }
  };

}

int main()
{
  typedef oln::image2d ima_t;

  ima_t ima0;       ima0.echo("ima0");
  ima_t ima1(1);    ima1.echo("ima1");
  ima_t ima2(ima1); ima2.echo("ima2");
  // donc ima2 est "normalement" une copie de ima1...

  std::cout << std::endl
	    <<ima0.data_ << std::endl
	    <<ima1.data_ << std::endl
	    <<ima2.data_ << std::endl;
}
