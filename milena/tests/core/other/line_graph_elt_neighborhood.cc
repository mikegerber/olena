// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/core/other/line_graph_elt_neighborhood.cc
///
/// Tests on mln::line_graph_elt_neighborhood.

#include <vector>

#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_edges.hh>

#include <mln/util/graph.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>


unsigned fwd_neighb[] = { 0, 2, 4 };
unsigned bkd_neighb[] = { 4, 2, 0 };


int main()
{
  using namespace mln;

  typedef point2d p_t;

  /*--------.
  | Graph.  |
  `--------*/

  /* The graph and its corresponding line graph are as follows:

            0 1 2 3 4               0 1 2 3 4
         .-----------	         .-----------
         |		         |
       0 |  0       2	       0 |  *       *
       1 |    \   / |	       1 |    0   1 |
       2 |      1   |	       2 |      *   4
       3 |       \  |	       3 |       2  |
       4 |        3-4	       4 |        *3*

  */

  // Points associated to vertices.
  typedef fun::i2v::array<p_t> F;
  F points(5);
  points(0) = point2d(0,0); // Point associated to vertex 0.
  points(1) = point2d(2,2); // Point associated to vertex 1.
  points(2) = point2d(0,4); // Point associated to vertex 2.
  points(3) = point2d(4,3); // Point associated to vertex 3.
  points(4) = point2d(4,4); // Point associated to vertex 4.

  // Edges.
  typedef mln::util::graph G;
  G g;

  // Populate the graph with vertices.
  g.add_vertices(points.size());

  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*-------------------------.
  | Graph and neighborhood.  |
  `-------------------------*/

  // Line graph psite set.
  typedef p_edges<G, F> pe_t;
  pe_t pe(g, points);

  // Line graph point site.
  mln_psite_(pe_t) p(pe, 1);

  // ``Sliding'' neighborhood of a psite of PLG.
  typedef line_graph_elt_neighborhood<G, F> nbh_t;
  nbh_t nbh;

  unsigned i = 0;
  mln_fwd_niter_(nbh_t) fq(nbh, p);
  for_all(fq)
    mln_assertion(fq.element().id() == fwd_neighb[i++]);

  i = 0;
  mln_bkd_niter_(nbh_t) bq(nbh, p);
  for_all(bq)
    mln_assertion(bq.element().id() == bkd_neighb[i++]);
}
