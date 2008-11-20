// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.  
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_TOPO_INTERNAL_COMPLEX_SET_ITERATOR_BASE_HH
# define MLN_TOPO_INTERNAL_COMPLEX_SET_ITERATOR_BASE_HH

/// \file mln/topo/internal/complex_set_iterator_base.hh
/// \brief Definition of an implementation (factoring) class for
/// (set) iterators on faces of a complex.

# include <mln/topo/internal/complex_iterator_base.hh>


namespace mln
{

  namespace topo
  {

    namespace internal
    {
      /// \brief Factoring class for (set) iterators on mln::complex.
      ///
      /// \arg \p F The type of the face handle.
      /// \arg \p E The type exact type of the iterator.
      template <typename F, typename E>
      class complex_set_iterator_base : public complex_iterator_base<F, E>
      {
	typedef complex_set_iterator_base<F, E> self_;

      public:
	/// The type of the iterated faces.
	typedef F face_type;
	// FIXME: Maybe we could just get the dimension D of the face's
	// complex, an define complex_type as mln::complex<D>?
	typedef typename F::complex_type complex_type;

	/// Construction and assignment.
	/// \{
	/* FIXME: Keep this non-const?  See a (big) comment about this in
	   milena/tests/complex_image.cc.   */
	complex_set_iterator_base();
	complex_set_iterator_base(complex_type& c);
	/// \}

	/// Manipulation.
	/// \{
	/// Change the target complex.
	// FIXME: Same comment as the ctor above.
	void set_cplx(complex_type& c);

	/// Test if the iterator is valid.
	bool is_valid() const;
	/// Invalidate the iterator.
	void invalidate();
	/// \}
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename F, typename E>
      inline
      complex_set_iterator_base<F, E>::complex_set_iterator_base()
      {
	invalidate();
      }

      template <typename F, typename E>
      inline
      complex_set_iterator_base<F, E>::complex_set_iterator_base(complex_type& c)
      {
	this->f_.set_cplx(c);
	// Invalidate f_.
	invalidate();
      }

      template <typename F, typename E>
      inline
      void
      complex_set_iterator_base<F, E>::set_cplx(complex_type& c)
      {
	this->f_.set_cplx(c);
	// Invalidate f_.
	invalidate();
      }

      template <typename F, typename E>
      inline
      bool
      complex_set_iterator_base<F, E>::is_valid() const
      {
	return this->f_.is_valid();
      }

      template <typename F, typename E>
      inline
      void
      complex_set_iterator_base<F, E>::invalidate()
      {
	this->f_.invalidate();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::topo::internal

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_INTERNAL_COMPLEX_SET_ITERATOR_BASE_HH