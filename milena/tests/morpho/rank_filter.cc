// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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
#include <mln/win/rectangle2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/io/pgm/load.hh>

#include <mln/value/int_u8.hh>
#include <mln/data/fill.hh>
#include <mln/data/compare.hh>
#include <mln/data/median.hh>
#include <mln/morpho/rank_filter.hh>
#include <mln/morpho/dilation.hh>

#include <mln/pw/value.hh>
#include <mln/pw/cst.hh>
#include <mln/fun/ops.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  win::rectangle2d rec(21, 21);
  border::thickness = 66;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/small.pgm");

  {
    image2d<int_u8> out;
    out = morpho::rank_filter(lena, rec, 0);

    image2d<int_u8> ref(lena.domain());
    ref = morpho::erosion(lena, rec);

    mln_assertion(ref == out);
  }


  {
    image2d<int_u8> out;
    out = morpho::rank_filter(lena, rec, 21 * 21);

    image2d<int_u8> ref(lena.domain());
    ref = morpho::dilation(lena, rec);

    mln_assertion(ref == out);
  }

}
