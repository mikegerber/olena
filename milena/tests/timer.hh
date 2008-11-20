// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

/*! \file tests/morpho/erosion.cc
 *
 * \brief Timer to bench tests.
 */

class timer
{
public:
  void start()
  {
    start_ = clock();
  }

  void stop()
  {
    end_ = clock();
    len = float(end_ - start_) / CLOCKS_PER_SEC;
  }

  float lenght()
  {
    return float(clock() - start_) / CLOCKS_PER_SEC;
  }

private:
  clock_t start_;
  clock_t end_;
  float len;
};


std::ostream& operator<<(std::ostream& ostr, timer t)
{
  return ostr << t.lenght() << "s";
}