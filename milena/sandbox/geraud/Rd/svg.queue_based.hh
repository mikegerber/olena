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

#ifndef	MLN_MORPHO_RD_QUEUE_BASED_HH
# define MLN_MORPHO_RD_QUEUE_BASED_HH

# include <queue>
# include "utils.hh"


namespace mln
{

  namespace morpho
  {

    namespace Rd
    {

      template <typename I, typename N>
      I queue_based(const I& f, const I& g, const N& nbh,
		    bool echo = false)
      {
	if (echo) std::cout << std::endl;

	f.name_it("f");
	g.name_it("g");

	typedef mln_point(I) point;
	std::queue<point> q;
	I o;
	o.name_it("o");

	unsigned n_init_pushs = 0, n_body_pushs = 0, n_pops = 0;

	// initialisation
	{
	  o = regional_maxima(f, nbh);
	  // p in M <=> o(p) != 0
	  if (echo) debug::println(o);

	  mln_piter(I) p(f.domain());
	  mln_niter(N) n(nbh, p);

	  for_all(p) if (o(p) != 0) // p in M
	    for_all(n) if (f.has(n) and o(n) == 0) // n not in M
	      {
		q.push(p);
		++n_init_pushs;
		break;
	      }
	}

	// propagation
	{
	  point p;
	  mln_niter(N) n(nbh, p);
	  while (not q.empty())
	    {
	      p = q.front();
	      if (echo) std::cout << std::endl << "pop " << p << " :";
	      q.pop();
	      ++n_pops;
	      for_all(n) if (f.has(n))
		{
		  if (o(n) < o(p) and o(n) != g(n))
		    {
		      o(n) = min(o(p), g(n));
		      if (echo) std::cout << " push " << n;
		      q.push(n);
		      ++n_body_pushs;
		    }
		}
	    }
	  if (echo) std::cout << std::endl;
	}

	std::cout << "n_init_pushs = " << n_init_pushs << std::endl
		  << "n_body_pushs = " << n_body_pushs << std::endl
		  << "n_pops = " << n_pops << std::endl;

	print_counts();

	return o;
      }

    } // end of namespace mln::morpho::Rd

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RD_QUEUE_BASED_HH