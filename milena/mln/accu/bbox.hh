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

#ifndef MLN_ACCU_BBOX_HH
# define MLN_ACCU_BBOX_HH

/*! \file mln/accu/bbox.hh
 *
 * \brief Define an accumulator that computes a bbox.
 */

# include <mln/core/concept/accumulator.hh>
# include <mln/core/box.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic bbox accumulator class.
     *
     * The parameter \c P is the type of points.
     */
    template <typename P>
    struct bbox : public Accumulator< bbox<P> >
    {
      typedef P value;

      bbox();
      void take(const P& p);
      void init();

      operator box_<P>() const;
      const box_<P>& to_value() const;

      bool is_valid() const;

    protected:

      bool is_valid_;
      box_<P> b_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    bbox<P>::bbox()
    {
      init();
    }

    template <typename P>
    void bbox<P>::take(const P& p)
    {
      if (! is_valid_)
	{
	  b_.pmin() = p;
	  b_.pmax() = p;
	  is_valid_ = true;
	  return;
	}
      for (unsigned i = 0; i < P::dim; ++i)
	if (p[i] < b_.pmin()[i])
	  b_.pmin()[i] = p[i];
	else if (p[i] > b_.pmax()[i])
	  b_.pmax()[i] = p[i];
    }

    template <typename P>
    void
    bbox<P>::init()
    {
      is_valid_ = false;
    }

    template <typename P>
    bbox<P>::operator box_<P>() const
    {
      mln_precondition(is_valid_);
      return b_;
    }

    template <typename P>
    const box_<P>&
    bbox<P>::to_value() const
    {
      mln_precondition(is_valid_);
      return b_;
    }

    template <typename P>
    bool
    bbox<P>::is_valid() const
    {
      return is_valid_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_BBOX_HH
