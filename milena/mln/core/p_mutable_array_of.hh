// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_P_MUTABLE_ARRAY_OF_HH
# define MLN_CORE_P_MUTABLE_ARRAY_OF_HH

/*! \file mln/core/p_mutable_array_of.hh
 *
 * \brief Definition of a mutable array of site sets.
 *
 * \todo Zed: Add another version (p_array_of) of this class that can
 * inherit, when possible, nsites and bbox (just like in p_vaccess).
 * It is a different class since such a feature is incompatible with
 * the "op[] mutable".
 */

# include <mln/core/p_double.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/util/array.hh>



namespace mln
{

  // Forward declaration.
  template <typename S> class  p_mutable_array_of;


  namespace trait
  {

    template <typename S>
    struct site_set_< p_mutable_array_of<S> >
    {
      typedef trait::site_set::nsites::unknown   nsites;
      typedef trait::site_set::bbox::unknown     bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait



  /*! \brief p_mutable_array_of is a mutable array of site sets.
   *
   * Parameter \c S is the type of the contained site sets.
   */
  template <typename S>
  class p_mutable_array_of : public internal::site_set_base_< mln_element(S),
						      p_mutable_array_of<S> >,
		     private mlc_is_a(S, Site_Set)::check_t
  {
    typedef p_mutable_array_of<S>  self_;
    typedef util::array<S> array_;
  public:

    /// Element associated type.
    typedef S element;


    /// Psite associated type.
    typedef p_double_psite<self_, element> psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_fwd_iter(array_),
			   mln_fwd_piter(S)> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_bkd_iter(array_),
			   mln_bkd_piter(S)> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without arguments.
    p_mutable_array_of();


    /// Test if \p p belongs to this point set.
    bool has(const psite&) const;

    /// Test if this set of runs is valid.
    bool is_valid() const;


    /// Insertion element associated type.
    typedef S i_element;

    /// Insert a site set \p s.
    void insert(const S& s);


    /// Return the \p i-th site set (const version).
    const S& operator[](unsigned i) const;

    /// Return the \p i-th site set (mutable version).
    S& operator[](unsigned i);


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Hook to the set of runs.
    const util::array<S>& array_hook_() const;


    // Required by p_double-related classes.
    const util::array<S>& set_1_() const;
    template <typename I>
    const S& set_2_(const I& it) const;

  protected:

    /// Array of site sets.
    util::array<S> arr_;
  };



  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const p_mutable_array_of<S>& r);




# ifndef MLN_INCLUDE_ONLY

  template <typename S>
  inline
  p_mutable_array_of<S>::p_mutable_array_of()
  {
  }

  template <typename S>
  inline
  bool
  p_mutable_array_of<S>::has(const psite&) const
  {
    // FIXME
    return true;
  }

  template <typename S>
  inline
  bool
  p_mutable_array_of<S>::is_valid() const
  {
    return true;
  }

  template <typename S>
  inline
  void
  p_mutable_array_of<S>::insert(const S& s)
  {
    arr_.append(s);
  }

  template <typename S>
  inline
  const S&
  p_mutable_array_of<S>::operator[](unsigned i) const
  {
    mln_precondition(i < arr_.nelements());
    return arr_[i];
  }

  template <typename S>
  inline
  S&
  p_mutable_array_of<S>::operator[](unsigned i)
  {
    mln_precondition(i < arr_.nelements());
    return arr_[i];
  }

  template <typename S>
  inline
  std::size_t
  p_mutable_array_of<S>::memory_size() const
  {
    return arr_.memory_size();
  }

  template <typename S>
  inline
  const util::array<S>&
  p_mutable_array_of<S>::array_hook_() const
  {
    return arr_;
  }

  template <typename S>
  inline
  const util::array<S>&
  p_mutable_array_of<S>::set_1_() const
  {
    return arr_;
  }

  template <typename S>
  template <typename I>
  inline
  const S&
  p_mutable_array_of<S>::set_2_(const I& it) const
  {
    return it.element();
  }


  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const p_mutable_array_of<S>& a)
  {
    return ostr << a.array_hook_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_MUTABLE_ARRAY_OF_HH
