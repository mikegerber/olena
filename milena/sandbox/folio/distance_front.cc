// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

/// \file tests/transform/distance_front.cc
///
/// Test on mln::transform::distance_front.

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/w_window2d_int.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/fill.hh>
#include <mln/debug/println.hh>
#include <mln/opt/at.hh>

# include <mln/transform/internal/distance_functor.hh>
# include "distance_front_new.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<bool> I;

  I input(9, 9);
  data::fill(input, false);
  opt::at(input, 4, 4) = true;


  int_u8 dmax = 18;
  mln_VAR(nbh, c4());

  int ws[] = { 0, 9, 0, 9, 0,
	       9, 6, 4, 6, 9,
	       0, 4, 0, 4, 0,
	       9, 6, 4, 6, 9,
	       0, 9, 0, 9, 0 };
  mln_VAR(w_win, make::w_window2d_int(ws));


  transform::internal::distance_functor<I> f;
  image2d<int_u8> ref, output;

  ref = canvas::impl::generic::distance_front(input,
					      nbh,
					      w_win,
					      dmax,
					      f);
  debug::println("ref", ref);

  output = canvas::impl::distance_front_fastest(input,
						nbh,
						w_win,
						dmax,
						f);
  debug::println("output", output);

}