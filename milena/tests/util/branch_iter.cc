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

#include <mln/core/image/image2d.hh>
#include <mln/util/tree.hh>
#include <mln/util/branch_iter.hh>

int main()
{
  using namespace mln;

  util::tree_node<int> n(11);
  util::tree<int> t(&n);
  util::tree_node<int>* f = n.add_child(21);
  util::tree_node<int>* g = f->add_child(31);

  f->add_child(32);
  g->add_child(41)->add_child(51)->add_child(61)->add_child(71)
    ->add_child(81)->add_child(91);
  g->add_child(42);
  f->add_child(33);
  f->add_child(34);
  n.add_child(22);

  util::branch<int> b(t, n);

  std::vector< util::tree_node<int>* >::iterator it;
  util::branch_iter<int> p(b);

  int prev;
  int current;
  for(p.start(), prev = util::tree_node<int>(p).elt(), p.next();
      p.is_valid();
      prev = util::tree_node<int>(p).elt(), p.next())
    {
      current = util::tree_node<int>(p).elt ();

      // children
      if (prev + 10 == current)
	continue;

      // brother
      if (prev + 1 == current)
	continue;

      // parent
      if (prev > current)
	continue;

      mln_assertion (false);
    }

}
