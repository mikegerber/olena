// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_DEBUG_LINKS_IMAGE_HH
# define SCRIBO_DEBUG_LINKS_IMAGE_HH

/// \file
///
/// Save a color image showing components links.

# include <mln/core/concept/image.hh>
# include <mln/accu/center.hh>
# include <mln/data/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/draw/line.hh>

# include <scribo/core/object_links.hh>
# include <scribo/draw/bounding_boxes.hh>
# include <scribo/primitive/link/internal/compute_anchor.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /// FIXME: DOC!
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    links_image(const Image<I>& input_,
		const object_links<L>& links,
		anchor::Type anchor);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    links_image(const Image<I>& input_,
		const object_links<L>& links,
		anchor::Type anchor)
    {
      trace::entering("scribo::debug::links_image");
      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mln_precondition(links.is_valid());

      const component_set<L>& comps = links.components();

      image2d<value::rgb8>
	links_image = data::convert(value::rgb8(), input);

      scribo::draw::bounding_boxes(links_image, comps, literal::blue);

      for_all_links(l, links)
	if (links(l) != l)
	  mln::draw::line(links_image,
			  primitive::link::internal::compute_anchor(comps,
								    l,
								    anchor),
			  primitive::link::internal::compute_anchor(comps,
								    links(l),
								    anchor),
			  literal::green);

      trace::exiting("scribo::debug::links_image");
      return links_image;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_LINKS_IMAGE_HH