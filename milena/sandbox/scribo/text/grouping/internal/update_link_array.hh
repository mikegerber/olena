// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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


#ifndef SCRIBO_TEXT_GROUPING_INTERNAL_UPDATE_LINK_ARRAY_HH
# define SCRIBO_TEXT_GROUPING_INTERNAL_UPDATE_LINK_ARRAY_HH

/// \file scribo/text/grouping/internal/update_link_array.hh
///
/// Update a lookup table if a neighbor is found on the right of
/// the current bbox.


# include <mln/core/concept/image.hh>
# include <mln/util/array.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      namespace internal
      {

	/// Update the lookup table \p link_array if a neighbor is found
	/// on the right of the current bbox.
	template <typename I>
	void
	update_link_array(const Image<I>& lbl, mln::util::array<unsigned>& link_array,
			  const mln_site(I)& p, const mln_site(I)& c,
			  unsigned i, int dmax);

# ifndef MLN_INCLUDE_ONLY

	template <typename I>
	inline
	void
	update_link_array(const Image<I>& lbl_, mln::util::array<unsigned>& link_array,
			  const mln_site(I)& p, const mln_site(I)& c,
			  unsigned i, int dmax)
	{
	  const I& lbl = exact(lbl_);

	  mlc_is_a(mln_value(I), mln::value::Symbolic)::check();
	  mln_assertion(lbl.is_valid());

	  if (lbl.domain().has(p) && lbl(p) != literal::zero && lbl(p) != i
	      && (math::abs(p.col() - c.col())) < dmax && link_array[lbl(p)] == lbl(p)
	      && link_array[i] != lbl(p))
	    link_array[lbl(p)] = i;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::text::grouping::internal

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo


#endif // ! SCRIBO_TEXT_GROUPING_INTERNAL_UPDATE_LINK_ARRAY_HH