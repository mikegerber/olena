// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/core/p_priority_queue.cc
 *
 * \brief Tests on mln::p_priority_queue.
 */

#include <mln/core/point2d.hh>
#include <mln/core/site_set/p_priority_queue.hh>

int main ()
{
  using namespace mln;

  p_priority_queue<point2d, unsigned> q;
  point2d p1 (6, 9);
  point2d p2 (5, 1);
  point2d p3 (4, 2);

  mln_assertion (q.is_empty ());

  mln_assertion (q.npoints () == 0);

  q.push_force (p3);
  q.push_force (p1, 3);
  q.push_force (p2, 5);

  std::cout << q.bbox () << std::endl;
  std::cout << q << std::endl;

  mln_assertion (!q.is_empty ());

  mln_assertion (q.has (p1));
  mln_assertion (q.has (p2));
  mln_assertion (q.has (p3));

  mln_assertion (q.npoints () == 3);
  mln_assertion (q.front () == p2);
  q.pop ();

  mln_assertion (q.has (p1));
  mln_assertion (!q.has (p2));
  mln_assertion (q.has (p3));

  mln_assertion (q.npoints () == 2);
  mln_assertion (q.front () == p1);
  q.pop ();

  mln_assertion (!q.has (p1));
  mln_assertion (!q.has (p2));
  mln_assertion (q.has (p3));

  mln_assertion (q.npoints () == 1);
  mln_assertion (q.front () == p3);
  q.pop ();

  mln_assertion (!q.has (p1));
  mln_assertion (!q.has (p2));
  mln_assertion (!q.has (p3));
  mln_assertion (q.npoints () == 0);

  mln_assertion (q.is_empty ());

  q.push_force (p3);
  q.push_force (p2, 5);
  q.push_force (p1, 3);

  mln_assertion (q[2] == p3);
  mln_assertion (q[1] == p1);
  mln_assertion (q[0] == p2);
  q.clear ();
  mln_assertion (q.is_empty ());
}
