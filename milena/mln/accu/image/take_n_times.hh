// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ACCU_IMAGE_TAKE_N_TIMES_HH
# define MLN_ACCU_IMAGE_TAKE_N_TIMES_HH

/// \file mln/accu/image/take_n_times.hh
///
/// Update an image of accumulators by taking the contents of another
/// image with a multiplicity.
///
/// \todo Add "take(input, n, arg_value)".

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/border/resize.hh>


namespace mln
{

  namespace accu
  {

    namespace image
    {

      template <typename I, typename J, typename K>
      void
      take_n_times(Image<I>& input, const Image<J>& n_times, const Image<K>& arg);



# ifndef MLN_INCLUDE_ONLY


      // Tests.

      namespace internal
      {

	template <typename I, typename J, typename K>
	void
	take_n_times_tests(Image<I>& input_, const Image<J>& n_times_, const Image<K>& arg_)
	{
	  I& input = exact(input_);
	  const J& n_times = exact(n_times_);
	  const K& arg = exact(arg_);
	  
	  mln_precondition(input.is_valid());
	  mln_precondition(n_times.is_valid());
	  mln_precondition(arg.is_valid());

	  mln_precondition(arg.domain() <= input.domain());
	  mln_precondition(arg.domain() <= n_times.domain());

	  (void) input;
	  (void) n_times;
	  (void) arg;
	}

      } // end of namespace mln::accu::image::internal


      // Implementations.

      namespace impl
      {

	// Generic version.

	namespace generic
	{

	  template <typename I, typename J, typename K>
	  void
	  take_n_times(Image<I>& input_, const Image<J>& n_times_, const Image<K>& arg_)
	  {
	    trace::entering("accu::impl::image::generic::take_n_times");

	    mlc_is_a(mln_value(I), Accumulator)::check();
	    mlc_converts_to(mln_value(J), unsigned)::check();
	    mlc_converts_to(mln_value(K), mln_deduce(I, value, argument))::check();

	    I& input = exact(input_);
	    const J& n_times = exact(n_times_);
	    const K& arg = exact(arg_);

	    internal::take_n_times_tests(input, n_times, arg);

	    mln_piter(J) p(arg.domain());
	    for_all(p)
	      input(p).take_n_times(n_times(p), arg(p));

	    trace::exiting("accu::impl::image::generic::take_n_times");
	  }

	} // end of namespace mln::accu::image::impl::generic


	// Fastest version.

	template <typename I, typename J, typename K>
	void
	take_n_times_fastest(Image<I>& input_, const Image<J>& n_times_, const Image<K>& arg_)
	{
	  trace::entering("accu::impl::image::take_n_times_fastest");

	  mlc_is_a(mln_value(I), Accumulator)::check();
	  mlc_converts_to(mln_value(J), unsigned)::check();
	  mlc_converts_to(mln_value(K), mln_deduce(I, value, argument))::check();

	  I& input = exact(input_);
	  const J& n_times = exact(n_times_);
	  const K& arg = exact(arg_);

	  internal::take_n_times_tests(input, n_times, arg);

	  // Extra (stronger) tests.
	  mln_precondition(arg.domain() == input.domain());
	  mln_precondition(arg.domain() == n_times.domain());

	  border::resize(n_times, input.border());
	  border::resize(arg,     input.border());
	  
	  mln_pixter(I)       p_in(input);
	  mln_pixter(const J) p_ntm(n_times);
	  mln_pixter(const K) p_arg(arg);
	  for_all_3(p_in, p_ntm, p_arg)
	    p_in.val().take_n_times( p_ntm.val(), p_arg.val() );

	  trace::exiting("accu::impl::image::take_n_times_fastest");
	}

      } // end of namespace mln::accu::image::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename J, typename K>
	void
	take_n_times_dispatch(trait::image::speed::any,
			      trait::image::speed::any,
			      trait::image::speed::any,
			      Image<I>& input, const Image<J>& n_times, const Image<K>& arg)
	{
	  impl::generic::take_n_times(input, n_times, arg);
	}

	template <typename I, typename J, typename K>
	void
	take_n_times_dispatch(trait::image::speed::fastest,
			      trait::image::speed::fastest,
			      trait::image::speed::fastest,
			      Image<I>& input, const Image<J>& n_times, const Image<K>& arg)
	{
	  if (exact(n_times).domain() == exact(input).domain() &&
	      exact(arg)    .domain() == exact(input).domain())
	    impl::take_n_times_fastest(input, n_times, arg);
	  else
	    impl::generic::take_n_times(input, n_times, arg);
	}

	template <typename I, typename J, typename K>
	void
	take_n_times_dispatch(Image<I>& input, const Image<J>& n_times, const Image<K>& arg)
	{
	  take_n_times_dispatch(mln_trait_image_speed(I)(),
				mln_trait_image_speed(J)(),
				mln_trait_image_speed(K)(),
				input, n_times, arg);
	}

      } // end of namespace mln::accu::image::internal


      // Facade.

      template <typename I, typename J, typename K>
      void
      take_n_times(Image<I>& input_, const Image<J>& n_times_, const Image<K>& arg_)
      {
	trace::entering("accu::image::take_n_times");

	mlc_is_a(mln_value(I), Accumulator)::check();
	mlc_converts_to(mln_value(J), unsigned)::check();
	mlc_converts_to(mln_value(K), mln_deduce(I, value, argument))::check();

	I& input = exact(input_);
	const J& n_times = exact(n_times_);
	const K& arg = exact(arg_);

	internal::take_n_times_tests(input, n_times, arg);
	internal::take_n_times_dispatch(input, n_times, arg);

	trace::exiting("accu::image::take_n_times");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::image

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_IMAGE_TAKE_N_TIMES_HH