// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/morpho/tree/max.cc
///
/// Tests on mln::morpho::tree::max.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/pw/image.hh>

#include <mln/debug/println.hh>
#include <mln/debug/iota.hh>
#include <mln/morpho/elementary/dilation.hh>

#include <mln/morpho/tree/utils.hh>
#include <mln/morpho/tree/max.hh>


int main()
{
  using namespace mln;

  image2d<unsigned> ima(3, 3);
  debug::iota(ima);

  ima = morpho::elementary::dilation(ima, c8());
  debug::println(ima);

  image2d<point2d> par = morpho::tree::max(ima, c4());
  debug::println(par);


//   using morpho::tree::is_root;
//   using morpho::tree::is_a_node;

//   mln_piter_(box2d) p(ima.domain());
//   for_all(p)
//     if (is_root(par, p, ima))
//       std::cout << "R ";
//     else if (is_a_node(par, p, ima))
//       std::cout << "n ";
//     else
//       std::cout << ". ";
//   std::cout << std::endl;

  p_array<point2d> s = level::sort_psites_increasing(ima);
  std::cout << morpho::tree::nodes(par, ima, s) << std::endl;
}