// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CONVERT_TO_HH
# define MLN_CONVERT_TO_HH

/// \file mln/convert/to.hh
///
/// General conversion procedure given the destination type.
///
/// \todo Prefer a static check that fails in the "unknown" case.

# include <mln/core/routine/exact.hh>
# include <mln/metal/equal.hh>
# include <mln/trace/all.hh>
# include <mln/convert/from_to.hxx>


namespace mln
{

  namespace convert
  {


    /// Conversion of the object \p from towards an object with type \c T.
    template <typename T, typename O>
    T
    to(const O& from);

    // This is no "Object" here to remain as general as possible.  For
    // instance, the conversion "float -> glf" should occur.


# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename O>
    inline
    T
    to(const O& from)
    {
      mlc_equal(T, mln_exact(T))::check();
      mlc_equal(O, mln_exact(O))::check();
      trace::entering("convert::to");

      T tmp;
      from_to(from, tmp);

      trace::exiting("convert::to");
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_HH