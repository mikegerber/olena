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

#ifndef MLN_CONVERT_TO_VEC_P_HH
# define MLN_CONVERT_TO_VEC_P_HH

/*! \file mln/convert/to_vec_p.hh
 *
 * \brief Conversions to mln::vec_p.
 */

# include <mln/core/vec_p.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a point set \p pset into a vec_p (point set vector).
    template <typename S>
    vec_p<mln_point(S)> to_vec_p(const Point_Set<S>& pset);


    /// Convert a window \p win with a point \p p into a vec_p (point set vector).
    template <typename W>
    vec_p<mln_point(W)> to_vec_p(const Window<W>& win, const mln_point(W) p);


# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    vec_p<mln_point(S)> to_vec_p(const Point_Set<S>& pset_)
    {
      const S& pset = exact(pset_);
      vec_p<mln_point(S)> v;
      v.reserve(pset.npoints());
      mln_fwd_piter(S) p(pset);
      for_all(p)
	v.append(p);
      return v;
    }

    template <typename W>
    vec_p<mln_point(W)> to_vec_p(const Window<W>& win, const mln_point(W) p_center)
    {
      vec_p<mln_point(W)> v;
      mln_qiter(W) dp(win, p_center);

      v.reserve(exact(win).ndpoints());

      for_all(dp)
	v.append(dp);
      return v;
    }
# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_VEC_P_HH
