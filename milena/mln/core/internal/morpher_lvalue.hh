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

#ifndef MLN_CORE_INTERNAL_MORPHER_LVALUE_HH
# define MLN_CORE_INTERNAL_MORPHER_LVALUE_HH

/*! \file mln/core/internal/morpher_lvalue.hh
 *
 * \brief Definition of a base class for some images.
 */

# include <mln/core/macros.hh>


# define mln_morpher_lvalue(I) typename internal::morpher_lvalue_<I>::ret



namespace mln
{

  namespace internal
  {

    /// Return the lvalue type when an image with type \c I
    /// is morphed.
    template <typename I>
    struct morpher_lvalue_
    {
      typedef mln_lvalue(I) ret;
    };

    template <typename I>
    struct morpher_lvalue_< const I >
    {
      typedef mln_rvalue(I) ret;
    };

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_MORPHER_LVALUE_HH