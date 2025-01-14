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

#include <mln/accu/math/count.hh>


struct toto {};


int main()
{
  using namespace mln;

  {
    // The code below do not compile, as expected :-)
    //     accu::math::count<int> a;
    //     a.take_as_init(toto());
  }
  {
    accu::math::count<int> a;
    mln_assertion(a.to_result() == 0);
  }
  {
    accu::math::count<int> a;
    for (int i = 0; i < 200; i++)
      a.take(i);
    mln_assertion(a.to_result() == 200);
  }
  {
    accu::math::count<int> a, a_;
    a.take_as_init(1);
    mln_assertion(a == 1u);
    a.take(2);
    mln_assertion(a == 2u);

    a_.take_as_init(a);
    mln_assertion(a_ == 2u);
  }
}
