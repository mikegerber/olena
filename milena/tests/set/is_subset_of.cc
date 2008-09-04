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

/*! \file tests/set/is_subset_of.cc
 *
 * \brief Tests on mln::set::is_subset_of.
 */

#include <mln/set/is_subset_of.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/core/point2d.hh>

int main()
{
  using namespace mln;

  {
    p_set<point2d> pst1;
    p_set<point2d> pst2;
    p_set<point2d> pst3;
    p_set<point2d> pst4;
    pst1
      .insert(point2d( 2, 7))
      .insert(point2d( 2, 1))
      .insert(point2d(-4, 0))
      .insert(point2d( 0, 0))
      .insert(point2d( 1, 1))
      .insert(point2d( 6, 5));
    pst2
      .insert(point2d( 2, 7))
      .insert(point2d(-2, 1))
      .insert(point2d(-4, 0))
      .insert(point2d( 1,-1))
      .insert(point2d( 6, 5));
    pst3
      .insert(point2d( 2, 7))
      .insert(point2d( 2, 1))
      .insert(point2d(-4, 0))
      .insert(point2d( 0, 0))
      .insert(point2d( 1, 1))
      .insert(point2d( 6, 5))
      .insert(point2d(-2, 1))
      .insert(point2d( 1,-1));

    mln_assertion(set::is_subset_of(pst1, pst3));
    mln_assertion(set::is_subset_of(pst2, pst3));
    mln_assertion(!set::is_subset_of(pst3, pst1));
    mln_assertion(!set::is_subset_of(pst3, pst2));
    mln_assertion(set::is_subset_of(pst3, pst3));
    mln_assertion(set::is_subset_of(pst4, pst1));
    mln_assertion(set::is_subset_of(pst4, pst2));
    mln_assertion(set::is_subset_of(pst4, pst3));
    mln_assertion(set::is_subset_of(pst4, pst4));
  }
}
