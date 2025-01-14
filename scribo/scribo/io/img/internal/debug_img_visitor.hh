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

#ifndef SCRIBO_IO_IMG_INTERNAL_DEBUG_IMG_VISITOR_HH
# define SCRIBO_IO_IMG_INTERNAL_DEBUG_IMG_VISITOR_HH

/// \file
///
/// Save document image analysis to a small image.

# include <fstream>

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/draw/polygon.hh>
# include <mln/subsampling/antialiased.hh>
# include <mln/morpho/elementary/gradient_external.hh>
# include <mln/draw/box.hh>

# include <scribo/core/internal/doc_serializer.hh>
# include <scribo/core/document.hh>
# include <scribo/core/paragraph_set.hh>
# include <scribo/core/line_info.hh>

# include <scribo/util/component_precise_outline.hh>
# include <scribo/io/img/internal/draw_edges.hh>
# include <scribo/text/paragraphs_closing.hh>


namespace scribo
{

  namespace io
  {

    namespace img
    {

      namespace internal
      {


	template <typename L>
	class debug_img_visitor : public doc_serializer<debug_img_visitor<L> >
	{
	public:
	  // Constructor
	  debug_img_visitor(mln::image2d<value::rgb8>& out,
			    unsigned output_ratio);

	  // Visit overloads
	  void visit(const document<L>& doc) const;

	  void visit(const component_info<L>& info) const;

	  void visit(const paragraph_set<L>& parset) const;

	  void visit(const line_info<L>& line) const;

	private: // Attributes
	  mln::image2d<value::rgb8>& output;
	  unsigned output_ratio;
	  mutable L lbl_;

	private: // Methods
	  box2d compute_bbox(const box2d& b) const;
	};



# ifndef MLN_INCLUDE_ONLY


	template <typename L>
	box2d
	debug_img_visitor<L>::compute_bbox(const box2d& b) const
	{
	  point2d
	    pmin = b.pmin() / output_ratio,
	    pmax = b.pmax() / output_ratio;

	  return box2d(pmin, pmax);
	}


	template <typename L>
	debug_img_visitor<L>::debug_img_visitor(mln::image2d<value::rgb8>& out,
					     unsigned output_ratio)
	  : output(out), output_ratio(output_ratio)
	{
	  mln_assertion(output.is_valid());
	}


	/// Document
	//
	template <typename L>
	void
	debug_img_visitor<L>::visit(const document<L>& doc) const
	{
	  // Text
	  if (doc.has_text())
	    doc.paragraphs().accept(*this);

	  // Page elements (Pictures, ...)
	  if (doc.has_elements())
	  {
	    for_all_comps(e, doc.elements())
	    {
	      lbl_ = doc.elements().labeled_image();
	      if (doc.elements()(e).is_valid())
		doc.elements()(e).accept(*this);
	    }
	  }

	  // line seraparators
	  if (doc.has_vline_seps())
	  {
	    lbl_ = doc.vline_seps_comps().labeled_image();
	    for_all_comps(c, doc.vline_seps_comps())
	      if (doc.vline_seps_comps()(c).is_valid())
		doc.vline_seps_comps()(c).accept(*this);
	  }
	  if (doc.has_hline_seps())
	  {
	    lbl_ = doc.hline_seps_comps().labeled_image();
	    for_all_comps(c, doc.hline_seps_comps())
	      if (doc.hline_seps_comps()(c).is_valid())
		doc.hline_seps_comps()(c).accept(*this);
	  }

	}


	/// Component_info
	//
	template <typename L>
	void
	debug_img_visitor<L>::visit(const component_info<L>& info) const
	{
	  // Getting component outline
	  scribo::def::lbl_type id = (scribo::def::lbl_type)info.id().to_equiv();
	  //const L& lbl = info.holder().labeled_image();
	  p_array<point2d>
	    par = scribo::util::component_precise_outline(lbl_ | info.bbox(), id);

	  switch (info.type())
	  {
	    case component::HorizontalLineSeparator:
	    case component::VerticalLineSeparator:
	    {
	      mln::draw::polygon(output, par, literal::cyan, output_ratio);
	    }
	    break;


	    case component::DropCapital:
	    {
	      mln::draw::polygon(output, par, literal::violet, output_ratio);
	    }
	    break;

	    default:
	    case component::Image:
	    {
	      mln::draw::polygon(output, par, literal::orange, output_ratio);
	    }
	    break;
	  }
	}

	/// Paragraph Set
	//
	template <typename L>
	void
	debug_img_visitor<L>::visit(const paragraph_set<L>& parset) const
	{
	  const line_set<L>& lines = parset.lines();

	  // Prepare paragraph outlines.
	  L par_clo = text::paragraphs_closing(parset);

	  for_all_paragraphs(p, parset)
	    if (parset(p).is_valid())
	    {
	      const mln::util::array<line_id_t>& line_ids = parset(p).line_ids();

	      for_all_paragraph_lines(lid, line_ids)
	      {
	      	line_id_t l = line_ids(lid);
	      	lines(l).accept(*this);
	      }

	      // Adjust bbox to output image size.
	      box2d b = compute_bbox(parset(p).bbox());
	      b.enlarge(1);
	      b.crop_wrt(output.domain());
	      mln::draw::box(output, b, literal::blue);
	    }
	}


	template <typename L>
	void
	debug_img_visitor<L>::visit(const line_info<L>& line) const
	{
	  point2d
	    pmin = line.bbox().pmin(),
	    pmax = line.bbox().pmax();
	  pmax.row() = line.baseline();
	  pmin.row() = line.baseline();

	  pmin = pmin / output_ratio;
	  pmax = pmax / output_ratio;

	  mln::draw::line(output, pmin, pmax, literal::red);
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::img::internal

    } // end of namespace scribo::io::img

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // SCRIBO_IO_IMG_INTERNAL_DEBUG_IMG_VISITOR_HH
