// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef OLENA_UTILS_TIMER_HH
# define OLENA_UTILS_TIMER_HH

# include <mlc/contract.hh>
# include <time.h>

namespace oln {

  namespace utils {

    class timer
    {
    public:

      timer()
      {
	_status = e_unknown;
	_total_time = 0;
	_start_time = clock();
      }

      void start()
      {
	assertion(_status != e_running);
	_total_time = 0;
	_status = e_running;
	_start_time = clock();
      }

      float restart()
      {
	assertion(_status != e_unknown);
	float val = value();
	start();
	return val;
      }

      void resume()
      {
	assertion(_status == e_stopped);
	_status = e_running;
	_start_time = clock();
      }

      float stop()
      {
	assertion(_status == e_running);
	_total_time += (clock() - _start_time);
	_status = e_stopped;
	return value();
      }

      float value()
      {
	assertion(_status != e_unknown);
	return
	  _status == e_stopped ?
	  float(_total_time) / CLOCKS_PER_SEC :
	  float(_total_time + (clock() - _start_time)) / CLOCKS_PER_SEC;
      }

    private:

      bool _stopped;
      enum {
	e_unknown,
	e_stopped,
	e_running
      } _status;
      clock_t _total_time;
      clock_t _start_time;
    };

  } // utils

} // oln

#endif // OLENA_UTILS_TIMER_HH
