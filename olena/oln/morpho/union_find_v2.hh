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
// You should have receiv a copy of the GNU General Public License
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

#ifndef	OLN_MORPHO_UNION_FIND_HH
# define OLN_MORPHO_UNION_FIND_HH

# include <oln/core/concept/image.hh>

# include <oln/canvas/two_pass.hh>
# include <oln/level/fill.hh>

namespace oln
{

  namespace morpho
  {

    template <typename I>
    oln_plain_value(I, unsigned)
    union_find(const Image_with_Nbh<I>& input);

# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      struct union_find_
      {
	oln_plain_value(I, unsigned)  output;

	oln_plain(I) is_processed;
	oln_plain_value(I, oln_point(I)) parent;

	union_find_(I in)
	{
	  prepare(is_processed, with, in);
	  prepare(output, with, in);
	  prepare(parent, with, in);
	}

	oln_point(I)
	find_root(const I& ima,
		  const oln_point(I)& x,
		  oln_plain_value(I, oln_point(I))& parent)
	{
	  if (parent(x) != x)
	  {
	    parent(x) = find_root(ima, parent(x), parent);
	    return parent(x);
	  }
	  return x;
	}

	void
	do_union(const I& ima,
		 const oln_point(I)& n,
		 const oln_point(I)& p,
		 oln_plain_value(I, oln_point(I))& parent)
	{
	  oln_point(I) r = find_root(ima, n, parent);
	  if (r != p)
	    parent(r) = p;
	}

	void init()
	{
	  level::fill(inplace(is_processed), false);
	}

	void first_pass_body(const oln_point(I)& p, I input)
	{
	  parent(p) = p;
	  if ( input(p) )
	  {
	    oln_niter(I) n(input, p);
	    for_all(n)
	      {
		if ( input(n) == true and is_processed(n) )
		  do_union(input, n, p, parent);
	      }
	    is_processed(p) = true;
	  }

	}

	void second_pass_body(const oln_point(I)& p, I input)
	{
	  unsigned current_label = 0;
	  if ( input(p) == true and parent(p) == p )
	    output(p) = ++current_label;
	  else
	    output(p) = output(parent(p));
	}

	void final()
	{

      };

    } // end of namespace oln::morpho::impl

// Facades.

    template <typename I>
    oln_plain_value(I, unsigned)
    union_find(const Image_with_Nbh<I>& input)
    {
      impl::union_find_<I> f(exact(input));
      canvas::v2::two_pass(f, input);
      return f.output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_UNION_FIND_HH
