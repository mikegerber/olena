// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file tests/core/site_set/p_set.cc
 *
 * \brief Tests on mln::p_set.
 */

#include <iterator>

#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/make/box2d.hh>
#include <mln/geom/bbox.hh>



int main()
{
  using namespace mln;

  p_set<point2d> ps;
  ps.insert(point2d(6, 9));
  ps.insert(point2d(4, 2));
  ps.insert(point2d(5, 1));
  mln_assertion(ps.nsites() == 3);

  mln_assertion(geom::bbox(ps) == make::box2d(4,1, 6, 9));

  ps.remove(point2d(4, 2));
  mln_assertion(ps.nsites() == 2);

  mln_assertion(geom::bbox(ps) == make::box2d(5,1, 6, 9));
}