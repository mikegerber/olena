// Copyright (C) 2009 EPITA Research and Development Laboratory
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


#ifndef SCRIBO_DEBUG_SAVE_TEXTBBOXES_IMAGE_HH
# define SCRIBO_DEBUG_SAVE_TEXTBBOXES_IMAGE_HH

/// \file scribo/draw/bounding_boxes.hh
///
/// Draw a list of bounding boxes and their associated mass center.

# include <mln/core/concept/image.hh>
# include <mln/level/convert.hh>
# include <mln/value/rgb8.hh>
# include <mln/util/array.hh>
# include <mln/io/ppm/save.hh>

# include <scribo/draw/bounding_boxes.hh>
# include <scribo/make/debug_filename.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// Draw a list of bounding boxes and their associated mass center.
    template <typename I>
    void
    save_textbboxes_image(const Image<I>& input,
			  const mln::util::array< box<mln_site(I)> >& textbboxes,
			  const value::rgb8& value,
			  const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    void
    save_textbboxes_image(const Image<I>& input,
			  const mln::util::array< box<mln_site(I)> >& textbboxes,
			  const value::rgb8& value,
			  const std::string& filename)
    {
      trace::entering("scribo::debug::save_textbboxes_image");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I,value::rgb8) tmp = level::convert(value::rgb8(), input);
      draw::bounding_boxes(tmp, textbboxes, value);
      io::ppm::save(tmp, scribo::make::debug_filename(filename));

      trace::exiting("scribo::debug::save_textbboxes_image");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_SAVE_TEXTBBOXES_IMAGE_HH