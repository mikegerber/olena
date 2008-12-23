// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_EROSION_HH
# define MLN_MORPHO_EROSION_HH

/// \file mln/morpho/erosion.hh
///
/// Morphological erosion.
///
/// \todo The overloads are hidden and I don't know why!

# include <mln/morpho/general.hh>
# include <mln/morpho/includes.hh>
# include <mln/accu/land.hh>
# include <mln/accu/land_basic.hh>
# include <mln/accu/min.hh>
# include <mln/accu/min_h.hh>


namespace mln
{

  namespace morpho
  {

    /// Morphological erosion.
    template <typename I, typename W>
    mln_concrete(I)
    erosion(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    struct erosion_op
    {

      template <typename I>
      mln_morpho_select_accu(I, land_basic, min)
      accu(const Image<I>&) const
      {
	mln_morpho_select_accu(I, land_basic, min) tmp;
	return tmp;
      }

      template <typename I>
      mln_morpho_select_accu(I, land, min_h)
      accu_incr(const Image<I>&) const
      {
	mln_morpho_select_accu(I, land, min_h) tmp;
	return tmp;
      }

      template <typename I>
      mln_value(I) neutral(const Image<I>&) const
      {
	return internal::neutral<I>::supremum();
      }

    };


    namespace impl
    {


      // On set with centered window (overloads).

      template <typename I, typename W>
      mln_concrete(I)
      general_on_set_centered(const erosion_op&,
			      const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::general_on_set_centered__erosion");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);

	extension::adjust_fill(input, win, true);

	O output;
	output = duplicate(input);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  if (input(p) == true)
	    for_all(q) if (input.has(q))
	      if (input(q) == false)
	      {
		output(p) = false;
		break;
	      }

	trace::exiting("morpho::impl::general_on_set_centered__erosion");
	return output;
      }


      template <typename I, typename W>
      mln_concrete(I)
      general_on_set_centered_fastest(const erosion_op&,
				      const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::general_on_set_centered_fastest__erosion");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);

	extension::adjust_fill(input, win, true);

	O output;
	output = duplicate(input);

	mln_pixter(const I) p(input);
	mln_qixter(const I, W) q(p, win);
	mln_pixter(O) p_out(output);
	for_all_2(p, p_out)
	  if (p.val() == true)
	    for_all(q)
	      if (q.val() == false)
	      {
		p_out.val() = false;
		break;
	      }

	trace::exiting("morpho::impl::general_on_set_centered_fastest__erosion");
	return output;
      }

    } // end of namespace morpho::impl



    template <typename I, typename W>
    inline
    mln_concrete(I)
    erosion(const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::erosion");
      mln_precondition(exact(input).has_data());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = general(erosion_op(), input, win);

      if (exact(win).is_centered())
	mln_postcondition(output <= input);

      trace::exiting("morpho::erosion");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_EROSION_HH
