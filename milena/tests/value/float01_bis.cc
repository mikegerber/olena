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

#include <iostream>
#include <mln/value/float01_8.hh>
#include <mln/value/float01_16.hh>


int main()
{
  /* FIXME: The contents of this test seens to have nothing to do with
     floats.  I (Roland) suspect a copy-and-paste error here.  Either
     rename the test or turn it into an actual test on floats or get
     rid of it. */
#if 0
  using namespace mln::value;

  float01_8 a;

  gl8 a = white;
  gl16 b = white;
  assert((a == b) == true);
  gl8 c = (a + b) / 2;
  assert(c == white);
  c = a;
  assert(c == white);

  c = (a * 2) / 2;
  assert(c == white);
#endif
}
