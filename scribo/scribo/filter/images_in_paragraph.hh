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

#ifndef SCRIBO_FILTER_IMAGES_IN_PARAGRAPH_HH
# define SCRIBO_FILTER_IMAGES_IN_PARAGRAPH_HH

/// \file
///
/// Invalidate false positive images.
/// FIXME: Share same test canvas as text::merging.


# include <mln/core/concept/image.hh>
# include <mln/draw/box_plain.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/document.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// Invalidate false positive images.
    ///
    /// \param[in,out] doc    A document structure.
    ///
    /// \ingroup grpalgofilterelt
    //
    template <typename L>
    void
    images_in_paragraph(document<L>& doc);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    void
    images_in_paragraph(document<L>& doc)
    {
      mln_trace("scribo::filter::images_in_paragraph");

      mln_precondition(doc.is_valid());

      if (! doc.has_elements())
	return;

      mln_ch_value(L,bool) billboard;
      initialize(billboard, doc.image());
      data::fill(billboard, false);

      for_all_comps(p, doc.paragraphs())
	if (doc.paragraphs()(p).is_valid())
	  mln::draw::box_plain(billboard, doc.paragraphs()(p).bbox(), true);


      component_set<L> elts = doc.elements();
      for_all_comps(c, elts)
	if (elts(c).is_valid() && elts(c).type() == component::Image)
	{
	  const mln_box(L)& b_ = elts(c).bbox();

	  const bool tl = billboard(b_.pmin());
	  const bool tr = billboard.at_(b_.pmin().row(), b_.pmax().col());
	  const bool ml = billboard.at_(b_.pcenter().row(), b_.pmin().col());
	  const bool mc = billboard.at_(b_.pcenter().row(), b_.pcenter().col());
	  const bool mr = billboard.at_(b_.pcenter().row(), b_.pmax().col());
	  const bool bl = billboard.at_(b_.pmax().row(), b_.pmin().col());
	  const bool br = billboard(b_.pmax());

	  // This separator is included in an element (picture, drawing...)
	  // => Ignore it.
	  if (tl && tr && ml && mc && mr && bl && br)
	    elts(c).update_tag(component::Ignored);
	}

      // FIXME: warning this call may produce inconsistent data
      // Ignored components are still in the separator image...
      doc.set_elements(elts);

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_IMAGES_IN_PARAGRAPH_HH
