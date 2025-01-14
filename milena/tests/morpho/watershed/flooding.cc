// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/morpho/watershed/flooding.hh>
#include <mln/data/transform.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/util/timer.hh>

#include "tests/data.hh"

struct f_16_to_8 : mln::Function_v2v< f_16_to_8 >
{
  typedef mln::value::int_u8 result;
  result operator()(const mln::value::int_u16& v) const
  {
    if (v == 0)
      return 0;
    return 1 + (v - 1) % 255;
  }
};


int main()
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  image2d<int_u8> input;
  io::pgm::load(input, MLN_IMG_DIR "/squares.pgm");

  typedef int_u16 L;
  L n_basins;
  {
    util::timer t;
    t.start();
    image2d<L> output =
      morpho::watershed::impl::generic::flooding(input, c4(), n_basins);
    std::cout << "gen:  " << t << std::endl;
    io::pgm::save(data::transform(output, f_16_to_8()),
		  "flooding-tmp_ref.pgm");
  }
  {
    util::timer t;
    t.start();
    image2d<L> output =
      morpho::watershed::impl::flooding_fastest(input, c4(), n_basins);
    std::cout << "fast: " << t << std::endl;
    io::pgm::save(data::transform(output, f_16_to_8()),
		  "flooding-tmp_out.pgm");
  }
}
