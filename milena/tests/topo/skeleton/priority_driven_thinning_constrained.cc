// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Testing the computation of a skeleton of a 2D image using
/// an priority-driven thinning preserving end points.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/topo/skeleton/priority_driven_thinning.hh>

#include <mln/topo/is_simple_point2d.hh>
#include <mln/topo/detach_point.hh>
#include <mln/topo/is_not_end_point.hh>

#include <mln/logical/not.hh>
#include <mln/arith/revert.hh>
#include <mln/transform/distance_geodesic.hh>

#include <mln/io/pbm/all.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  typedef image2d<bool> I;
  typedef neighb2d N;

  // Add a border of (at least) 1 pixel, to a guarantee a meaningful
  // result of the computation of connectivity numbers (called within
  // is_simple_2d); indeed, this computation always expects each pixel
  // to have 8 neighboring sites.
  border::thickness = 1;

  I input = io::pbm::load(MLN_IMG_DIR "/small.pbm");

  // FIXME: Use a dual neighborhood instead?

  // Foreground neighborhood.
  neighb2d nbh_fg = c4();
  // Background neighborhood.
  neighb2d nbh_bg = c8();

  // Simplicity criterion functor.
  topo::is_simple_point2d<I, N> is_simple(nbh_fg, nbh_bg);
  // Simple point detach procedure.
  topo::detach_point<I> detach;
  // Constraint: do not collapse end points.
  topo::is_not_end_point<I, N> constraint(nbh_fg, input);

  // Distance type.
  typedef value::int_u8 D;
  // Distance map type.
  typedef image2d<D> M;

  // Compute a distance map on the objects (foreground) of the image,
  // instead of the (usual) background.
  M dmap = transform::distance_geodesic(logical::not_(input), nbh_fg,
					mln_max(D));
  // Create a priority function (actually, an image) using the inverse
  // of the distance map.
  M priority = arith::revert(dmap);

  I output = topo::skeleton::priority_driven_thinning(input, nbh_fg,
						      is_simple,
						      detach,
						      priority,
						      constraint);
  io::pbm::save(output, "priority_driven_thinning_constrained-small.pbm");
}
