// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_WIN_SYM_HH
# define MLN_WIN_SYM_HH

/// \file mln/win/sym.hh
///
/// Give the symmetrical object.

# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>



namespace mln
{

  namespace win
  {

    /*! \brief Give the symmetrical window of \p win.
     */
    template <typename W>
    W sym(const Window<W>& win);

    /*! \brief Give the symmetrical weighted window of \p w_win.
     */
    template <typename W>
    W sym(const Weighted_Window<W>& w_win);


# ifndef MLN_INCLUDE_ONLY

    template <typename W>
    inline
    W sym(const Window<W>& win)
    {
      trace::entering("win::sym");
      // mln_precondition(win.is_valid());
      W tmp = exact(win);
      tmp.sym();
      trace::exiting("win::sym");
      return tmp;
    }

    template <typename W>
    inline
    W sym(const Weighted_Window<W>& w_win)
    {
      trace::entering("win::sym");
      // mln_precondition(win.is_valid());
      W tmp = exact(w_win);
      tmp.sym();
      trace::exiting("win::sym");
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_SYM_HH