// Copyright (C) 2007, 2008, 2009, 2010, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_OBJECT_HH
# define MLN_CORE_CONCEPT_OBJECT_HH

/// \file
///
/// Definition of the top Milena class mln::Object.

# include <cassert>
# include <iostream>

# include <mln/core/macros.hh>
# include <mln/core/category.hh>
# include <mln/core/contract.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/debug/trace.hh>

// metal
# include <mln/metal/abort.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/is_not_a.hh>
# include <mln/metal/is.hh>
# include <mln/metal/is_not.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/not_equal.hh>
# include <mln/metal/converts_to.hh>
# include <mln/metal/ret.hh>
# include <mln/metal/unqualif.hh>
# include <mln/metal/math/all.hh>


/** \namespace mln
   \brief The namespace mln corresponds to the Milena (mini-Olena) project.

   FIXME: Re-write this description.

   The contents of mln mimics the contents of the olena project but
   in a simplified way.  Some classes have the same name in both
   projects and roughly have the same behavior.

   \warning The Milena project is independent from the Olena
   project; the user has to choose between both the project she
   wants to work with.
 */
namespace mln
{
  /// Implementation namespace of mln namespace.
  namespace impl {}

  /// Internal namespace of mln namespace.
  namespace internal
  {
    /// FIXME : doc
    namespace check {}
  }

  // Fwd decl.
  template <typename E> struct Object;

  /// \cond INTERNAL_API
  /// Object category flag type.
  template <>
  struct Object<void>
  {
    typedef Unknown<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for almost every class defined in Milena.

    The parameter \a E is the exact type.

    \ingroup modconcepts
  */
  template <typename E>
  struct Object
  {
    typedef E exact_t;
    typedef Object<void> category; // Default.
  protected:
    Object();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Object<E>::Object()
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/routine/exact.hh>
# include <mln/core/routine/ops.hh>


#endif // ! MLN_CORE_CONCEPT_OBJECT_HH
