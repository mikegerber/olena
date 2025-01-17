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
#include <mln/data/stretch.hh>
#include <mln/debug/iota.hh>
#include <mln/value/int_u8.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  int vs[3][3] = {
    { 1000, 2000, 3000 },
    { 1000, 2000, 3000 },
    { 1000, 2000, 3000 }
  };
  image2d<int> ima = make::image(vs);
  image2d<int_u8> out = data::stretch(int_u8(), ima);

  int_u8 ws[3][3] = {
    { 0, 127, 255 },
    { 0, 127, 255 },
    { 0, 127, 255 }
  };
  image2d<int_u8> ref = make::image(ws);

  box_fwd_piter_<point2d> p(out.domain());
  for_all(p)
    mln_assertion(out(p) == ref(p));
}
