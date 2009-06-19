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


#include <mln/literal/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include "../../mln/histo/compute_histo_3d.hh"

using namespace mln;
using namespace value;


void
init_test_image(image2d<rgb8>& ima)
{
  rgb8 red = literal::red;
  rgb8 green = literal::green;
  rgb8 black = literal::black;

  for (unsigned i = 1; i < 8; ++i)
    for (unsigned j = 0; j < 8; ++j)
    {
      point2d p(j, i);
      ima(p) = black;
    }

  for (unsigned i = 0; i < 8; ++i)
  {
    point2d p(i, 0);
    ima(p) = red;
  }

  point2d p(4, 5);
  ima(p) = green;
}


int main()
{
  // build test image
  image2d<rgb8> ima(8, 8);
  init_test_image(ima);

  // build histo
  image3d<unsigned> out = histo::compute_histo_3d(ima);

  // verify...
  mln_assertion(out(point3d(255, 0, 0)) == 8);
  mln_assertion(out(point3d(0, 255, 0)) == 1);
  mln_assertion(out(point3d(0, 0, 0)) == 55);
  mln_assertion(out(point3d(1, 0, 0)) == 0);
}
