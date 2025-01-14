// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/value/builtin/integers.hh>

#include <mln/accu/tuple.hh>

#include <mln/accu/math/count.hh>
#include <mln/accu/stat/max.hh>
#include <mln/accu/stat/min.hh>
#include <mln/accu/stat/mean.hh>


int main()
{
  using namespace mln;

  typedef accu::tuple<int, 4, accu::math::count<int>, accu::stat::max<int>, accu::stat::min<int>, accu::stat::mean<int> > teratuple;

  teratuple tuple1;
  teratuple tuple2;

  int i1 = 7;
  int i2 = 1;
  int i3 = 12;
  int i4 = -5;
  int i5 = -7;
  int i6 = 14;

  tuple1.take_as_init(i1);
  tuple1.take(i2);
  tuple1.take(i3);
  tuple1.take(i4);

  tuple2.take_as_init(i5);
  tuple2.take(i6);

  tuple1.take(tuple2);
  boost::tuple<unsigned, int, int, float> res1 = tuple1.to_result();

  mln_assertion(6 == res1.get<0>());
  mln_assertion(14 == res1.get<1>());
  mln_assertion(-7 == res1.get<2>());
  mln_assertion(res1.get<3>() - 3.66 < 0.1);
}
