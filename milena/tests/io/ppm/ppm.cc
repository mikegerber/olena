// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/io/ppm/ppm.cc
///
/// Test on mln::io::ppm::load and mln::io::ppm::save.


#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/ppms/load.hh>

#include <mln/data/compare.hh>

#include <mln/util/array.hh>

#include <mln/literal/colors.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::rgb8;

  image2d<rgb8> lena = io::ppm::load<rgb8>(MLN_IMG_DIR "/lena.ppm");
  io::ppm::save(lena, "out.ppm");

  image2d<rgb8> lena2;
  io::ppm::load(lena2, "out.ppm");
  mln_assertion(lena2 == lena);



  lena2(point2d(0,0)) = literal::green;
  io::ppm::save(lena2, "out.ppm");

  util::array<std::string> files(2);
  files[0] = MLN_IMG_DIR "/lena.ppm";
  files[1] = "out.ppm";

  image3d<rgb8> ima3d;
  io::ppms::load(ima3d, files);

  mln_assertion(ima3d.nslices() == 2);
  mln_assertion(slice(ima3d, 0) == lena);
  mln_assertion(slice(ima3d, 1) == lena2);
}

