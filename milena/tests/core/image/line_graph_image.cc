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

/// \file tests/core/image/graph_image.cc
/// \brief Tests on mln::graph_image.

#include <vector>

#include <mln/core/alias/point2d.hh>
#include <mln/pw/all.hh>

//#include <mln/core/image/line_graph_image.hh>
#include <mln/core/image/line_graph_elt_window.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>
#include <mln/core/site_set/p_edges.hh>

#include <mln/fun/i2v/array.hh>

#include <mln/util/graph.hh>

#include <mln/core/var.hh>

template <typename S>
struct viota_t : public mln::Function_p2v< viota_t<S> >
{
  typedef unsigned result;

  viota_t(unsigned size)
  {
    v_.resize(size);
    for (unsigned i = 0; i < size; ++i)
      v_[i] = 10 + i;
  }

  unsigned
  operator()(const mln_psite(S)& p) const
  {
    return v_[p.e().id()];
  }

  protected:
    std::vector<result> v_;
};



int main()
{
  using namespace mln;

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

  // Sites associated to edges.
  typedef fun::i2v::array<point2d> fsite_t;
  fsite_t sites(5);
  sites(0) = point2d(0,0); // Point associated to edge 0.
  sites(1) = point2d(2,2); // Point associated to edge 1.
  sites(2) = point2d(0,4); // Point associated to edge 2.
  sites(3) = point2d(4,3); // Point associated to edge 3.
  sites(4) = point2d(4,4); // Point associated to edge 4.

  util::graph g;
  // Populate the graph with vertices.
  for (unsigned i = 0; i < sites.size(); ++i)
    g.add_vertex();
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*---------------------------.
  | Line graph image support.  |
  `---------------------------*/

  typedef p_edges<util::graph, fsite_t> S;
  S pe(g, sites);

  /*-------------------.
  | Line graph image.  |
  `-------------------*/

  // Graph values.
  viota_t<S> iota(5);

  // Create line graph image.
  mln_const_VAR(ima, (iota | pe));

  /*------------.
  | Iterators.  |
  `------------*/

  // Manual iteration over the domain of IMA.
  mln_piter_(ima_t) p(ima.domain());
  unsigned i = 10;
  for_all (p)
    mln_assertion(ima(p) == i++);

  {
    // Window - Forward iteration
    typedef line_graph_elt_window<util::graph, fsite_t> win_t;
    win_t win;
    mln_qiter_(win_t) q(win, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), "
		<< "including the site itself:" << std::endl;
      for_all (q)
	std::cout << "  " << q << " (level = " << ima(q) << ")" << std::endl;
    }
  }

  {
    // Window - Backward iteration
    typedef line_graph_elt_window<util::graph, fsite_t> win_t;
    win_t win;
    mln_bkd_qiter_(win_t) q(win, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), "
		<< "including the site itself:" << std::endl;
      for_all (q)
	std::cout << "  " << q << " (level = " << ima(q) << ")" << std::endl;
    }
  }
  {
    // Neighborhood - Forward iteration
    typedef line_graph_elt_neighborhood<util::graph, fsite_t> neigh_t;
    neigh_t neigh;
    mln_niter_(neigh_t) n(neigh, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), " << std::endl;
      for_all (n)
      {
	mln_assertion(n != p);
	std::cout << "  " << n << " (level = " << ima(n) << ")" << std::endl;
      }
    }
  }

  {
    // Neighborhood - Backward iteration
    typedef line_graph_elt_neighborhood<util::graph, fsite_t> neigh_t;
    neigh_t neigh;
    mln_bkd_niter_(neigh_t) n(neigh, p);
    for_all (p)
    {
      std::cout << "neighbors of " << p << " (" << ima(p) << "), " << std::endl;
      for_all (n)
      {
	mln_assertion(n != p);
	std::cout << "  " << n << " (level = " << ima(n) << ")" << std::endl;
      }
    }
  }

  std::cout << std::endl;
}