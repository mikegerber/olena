// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/win/disk2d.cc
 *
 * \brief Tests on mln::win::disk2d.
 */

#include <cmath>

#include <mln/win/disk2d.hh>

#include <mln/convert/to_image.hh>

#include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  const unsigned l = 55;
  const unsigned l2 = 27 * 27;
  win::disk2d disk(l);

  mln_assertion(disk.delta() == 27);

  for (int x = -30; x <= 30; ++x)
    for (int y = -30; y <= 30; ++y)
    {
      mln_assertion(((abs(x) <= 27) && (abs(y) <= 27)) ||
		    !disk.has(dpoint2d(y, x)));
      mln_assertion(((x * x + y * y) <= l2) == (disk.has(dpoint2d(y, x))) ||
		    abs(x) > 27 ||
		    abs(y) > 27);
    }

  debug::println(convert::to_image(disk));
}

