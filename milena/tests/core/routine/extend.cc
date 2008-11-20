// Copyright (C) 2008 EPITA Research and Development Laboratory
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

/*! \file tests/core/routine/extend.cc
 *
 *  \brief Tests on mln::extend.
 */

#include <mln/core/var.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/sub_image.hh>

#include <mln/core/image/extended.hh>
#include <mln/core/routine/extend.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

#include <mln/extension/fill.hh>



int main()
{
  using namespace mln;

  image2d<int> ima(3, 3, 1);
  debug::iota(ima);

  box2d bb = ima.domain().to_larger(1);
  box2d BB = ima.domain().to_larger(2);

  debug::println(ima);
  debug::println(extended_to(ima, bb));

  mln_VAR(ima_, extend(ima | make::box2d(2,2), 7));
  debug::println(ima_);

  debug::println(extended_to(ima_, BB));

  extension::fill(ima_, 8);

  debug::println(ima_);
  debug::println(extended_to(ima_, BB));

  debug::println(ima_);
}