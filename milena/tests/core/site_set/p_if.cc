// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/core/site_set/p_if.hh>
#include <mln/fun/p2b/chess.hh>
#include <mln/convert/to_image.hh>
#include <mln/convert/to_p_set.hh>
#include <mln/set/card.hh>


int main()
{
  using namespace mln;

  box2d box_8x8 = make::box2d(8, 8);
  mln_assertion(set::card((box_8x8 | fun::p2b::chess())) == 32);

  {
    p_set<point2d> s = convert::to_p_set(box_8x8 | fun::p2b::chess());
    mln_precondition(s == (box_8x8 | fun::p2b::chess()));
  }
}
