// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_DATA_PASTE_HH
# define MLN_DATA_PASTE_HH

/*! \file mln/data/paste.hh
 *
 * \brief Paste the contents of an image into another one.
 *
 */

# include <mln/core/concept/image.hh>



// Specializations are in:
# include <mln/data/paste.spe.hh>



namespace mln
{

  namespace data
  {

    /// \brief Paste the contents of image \p input into the image \p
    /// output.
    ///
    /// \param[in] input The input image providing pixels values.
    /// \param[in,out] output The image in which values are
    /// assigned.
    ///
    /// This routine runs: \n
    /// for all p of \p input, \p output(p) = \p input(p).
    ///
    /// \warning The definition domain of \p input has to be included in
    /// the one of \p output; so using mln::safe_image does not
    /// make pasting outside the output domain work.
    ///
    /// \pre \p input.domain <= \p output.domain
    ///
    /// \{
    template <typename I, typename J>
    void paste(const Image<I>& input, Image<J>& output);
    /// \}



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I, typename J>
      inline
      void paste_tests(const Image<I>& input, Image<J>& output)
      {
	mlc_is(mln_trait_image_pw_io(J), trait::image::pw_io::read_write)::
          check();
	mlc_converts_to(mln_value(I), mln_value(J))::check();

	mln_precondition(exact(input).has_data());
	mln_precondition(exact(input).domain() <= exact(output).domain());

        // Avoid warning when compiling with NDEBUG
	(void)input;
	(void)output;
      }

    } // end of namespace mln::data::internal

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename J>
	inline
	void paste(const Image<I>& input_, Image<J>& output_)
	{
	  trace::entering("data::impl::generic::paste");

	  data::internal::paste_tests(input_, output_);

	  const I& input  = exact(input_);
	  J& output = exact(output_);


	  mln_piter(I) p(input.domain());
	  for_all(p)
	    output(p) = input(p);

	  trace::exiting("data::impl::generic::paste");
	}

      } // end of namespace mln::data::impl::generic

    } // end of namespace mln::data::impl


    // Facade.

    template <typename I, typename J>
    inline
    void paste(const Image<I>& input, Image<J>& output)
    {
      trace::entering("data::paste");


      internal::paste_tests(input, output);
      internal::paste_(input, output);

      trace::exiting("data::paste");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln



#endif // ! MLN_DATA_PASTE_HH