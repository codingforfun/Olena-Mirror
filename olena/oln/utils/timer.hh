// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
    /*! Timer class.
     */
    class timer
    {
    public:
      /*! Constructor.
      **
      ** \note The timer is not stopped, it is in a so-called 'unknown state'.
      ** Therefor, Restart can not be called.
      */
      timer()
      {
	status_ = e_unknown;
	total_time_ = 0;
	start_time_ = clock();
      }

      /*! Start the timer.
      **
      ** \pre The timer should not be running.
      */
      void
      start()
      {
	assertion(status_ != e_running);
	total_time_ = 0;
	stop_time_ = 0;
	status_ = e_running;
	start_time_ = clock();
      }
      /*! Restart the timer.
      **
      ** \pre The timer should have been start at least one time.
      */
      float
      restart()
      {
	assertion(status_ != e_unknown);
	float val = total_time();
	start();
	return val;
      }

      /*! Resume the timer.
      **
      ** \pre The timer should be running.
      */
      void
      resume()
      {
	assertion(status_ == e_stopped);
	status_ = e_running;
	start_time_ = clock();
      }

      /*! Stop the timer.
      **
      ** \pre The timer should be running.
      */
      float
      stop()
      {
	assertion(status_ == e_running);
	stop_time_ = clock();
	total_time_ += (stop_time_ - start_time_);
	status_ = e_stopped;
	return total_time();
      }

      /// Time since the last resume() or start().
      float
      last_time() const
      {
	assertion(status_ != e_unknown);
	return
	  status_ == e_stopped ?
	  float(stop_time_ - start_time_) / CLOCKS_PER_SEC :
	  float((clock() - start_time_)) / CLOCKS_PER_SEC;
      }

      /// Total time elapsed.
      float
      total_time() const
      {
	assertion(status_ != e_unknown);
	return
	  status_ == e_stopped ?
	  float(total_time_) / CLOCKS_PER_SEC :
	  float(total_time_ + (clock() - start_time_)) / CLOCKS_PER_SEC;
      }

    private:
      bool stopped_;
      enum {
	e_unknown,
	e_stopped,
	e_running
      } status_;
      clock_t total_time_;
      clock_t start_time_;
      clock_t stop_time_;
    };

  } // end of namespace utils

} // end of namespace oln

#endif // ! OLENA_UTILS_TIMER_HH
