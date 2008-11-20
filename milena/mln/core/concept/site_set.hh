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

#ifndef MLN_CORE_CONCEPT_SITE_SET_HH
# define MLN_CORE_CONCEPT_SITE_SET_HH

/*! \file mln/core/concept/site_set.hh
 *
 * \brief Definition of the concept of mln::Site_Set.
 *
 * \todo Add a "composite" property.
 *
 * \todo Rewrite and move out the ops.
 */

# include <mln/core/concept/site_iterator.hh>
# include <mln/trait/site_sets.hh>
# include <mln/metal/not_equal.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/is_unqualif.hh>
# include <mln/util/ord.hh>

# include <mln/util/yes.hh> // Temporary include.



namespace mln
{

  // Fwd decl.
  template <typename E> struct Site_Set;


  /// Site_Set category flag type.
  template <>
  struct Site_Set<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes of site sets.
   *
   * \see mln::doc::Site_Set for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Site_Set : public Object<E>
  {
    typedef Site_Set<void> category;

    /*
      typedef element;

      typedef site;
      typedef psite;

      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
      bool is_valid() const;

      std::size_t memory_size() const;
     */

  protected:
    Site_Set();
  };


  template <typename Sl, typename Sr>
  Sl& operator+=(Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);


  /*! \brief Equality test between site sets \p lhs and \p rhs.
   *
   * \param[in] lhs A site set.
   * \param[in] rhs Another site set.
   *
   * \relates mln::Site_Set
   */
  template <typename Sl, typename Sr>
  util::yes operator==(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /*! \brief Inclusion test between site sets \p lhs and \p rhs.
   *
   * \param[in] lhs A site set (included?).
   * \param[in] rhs Another site set (includer?).
   *
   * \relates mln::Site_Set
   */
  template <typename Sl, typename Sr>
  util::yes operator<=(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /*! \brief Strict inclusion test between site sets \p lhs and \p
   *  rhs.
   *
   * \param[in] lhs A site set (strictly included?).
   * \param[in] rhs Another site set (includer?).
   *
   * \relates mln::Site_Set
   */
  template <typename Sl, typename Sr>
  util::yes operator<(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /*! \brief Print a site set \p set into the output stream \p
   *  ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] set A site set.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Site_Set
   */
  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const Site_Set<S>& set);



  namespace trait
  {

    template <typename S>
    struct set_unary_< op::ord, Site_Set, S >
    {
      // No definition here.

      // That prevents from calling the inclusion operator ("op<")
      // which is clearly not the expected ordering...

      // Consequently site sets have to define the way they are
      // ordered.
    };

  } // end of namespace mln::trait



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // nsites: known or unknown.

    template <typename trait_nsites, typename E>
    struct site_set_nsites_check
    {
      static void run() { /* No requirement. */ }
    };

    template <typename E>
    struct site_set_nsites_check< mln::trait::site_set::nsites::known, E >
    {
      static void run()
      {
	unsigned (E::*m)() const = & E::nsites;
	m = 0;
      }
    };

    // bbox: unknown, lazy, or straight.

    template <typename trait_bbox, typename E>
    struct site_set_bbox_check
    {
      static void run()
      {
	typedef typename E::q_box q_box;
	q_box (E::*m)() const = & E::bbox;
	m = 0;
      }
    };

    template <typename E>
    struct site_set_bbox_check< mln::trait::site_set::bbox::unknown, E >
    {
      static void run() { /* No requirement. */ }
    };

    // contents: fixed, growing, or free.

    template <typename trait_contents, typename E>
    struct site_set_contents_check;

    template <typename E>
    struct site_set_contents_check< mln::trait::site_set::contents::fixed, E >
    {
      static void run() { /* No requirement. */ }
    };

    template <typename E>
    struct site_set_contents_check< mln::trait::site_set::contents::growing, E >
    {
      static void run()
      {
	typedef typename E::i_element i_element;
	mlc_equal(mlc_unqualif(i_element), i_element)::check();
	void (E::*m1)(const i_element&) = & E::insert;
	m1 = 0;
	void (E::*m2)() = & E::clear;
	m2 = 0;
      }
    };

    template <typename E>
    struct site_set_contents_check< mln::trait::site_set::contents::free, E >
    {
      static void run()
      {
	typedef typename E::i_element i_element;
	mlc_equal(mlc_unqualif(i_element), i_element)::check();
	void (E::*m1)(const i_element&) = & E::insert;
	m1 = 0;
	typedef typename E::r_element r_element;
	mlc_equal(mlc_unqualif(r_element), r_element)::check();
	void (E::*m2)(const r_element&) = & E::remove;
	m2 = 0;
	void (E::*m3)() = & E::clear;
	m3 = 0;
      }
    };

  } // end of namespace mln::internal


  // fwd decl
  template <typename P> struct box_;

  template <typename E>
  inline
  Site_Set<E>::Site_Set()
  {
    // Check properties.
    mlc_not_equal( mln_trait_site_set_nsites(E),   mln::trait::undef )::check();
    mlc_not_equal( mln_trait_site_set_bbox(E),     mln::trait::undef )::check();
    mlc_not_equal( mln_trait_site_set_contents(E), mln::trait::undef )::check();
    mlc_not_equal( mln_trait_site_set_arity(E),    mln::trait::undef )::check();
    // FIXME: also test something like
    //        mlc_is_a( mln_trait_site_set_nsites(E),   mln::trait::site_set::nsites::any   )::check();

    // Check associated types.
    typedef mln_element(E) element;
    mlc_is_unqualif(element)::check();
    typedef mln_site(E)  site;
    typedef mln_psite(E) psite;
    typedef mln_piter(E) piter;
    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;

    // Check methods.
    bool (E::*m1)(const psite& p) const = & E::has;
    m1 = 0;
    bool (E::*m2)() const = & E::is_valid;
    m2 = 0;
    std::size_t (E::*m3)() const = & E::memory_size;
    m3 = 0;

    // Check methods depending upon properties.
    internal::site_set_nsites_check  < mln_trait_site_set_nsites(E),   E >::run();
    internal::site_set_bbox_check    < mln_trait_site_set_bbox(E),     E >::run();
    internal::site_set_contents_check< mln_trait_site_set_contents(E), E >::run();
  }



  // Operators.


  template <typename Sl, typename Sr>
  inline
  Sl& operator+=(Site_Set<Sl>& lhs_, const Site_Set<Sr>& rhs)
  {
    mlc_is( mln_trait_site_set_contents(Sl),
	    mln::trait::site_set::contents::dynamic )::check();
    mlc_equal(mln_site(Sr), typename Sl::i_element)::check();
    Sl& lhs = exact(lhs_);
    mln_fwd_piter(Sr) p(exact(rhs));
    for_all(p)
      lhs.insert(p);
    return lhs;
  }


  template <typename Sl, typename Sr>
  inline
  util::yes operator==(const Site_Set<Sl>&, const Site_Set<Sr>&)
  {
//     // FIXME: Same grid!
//     const Sl& lhs = exact(lhs_);
//     const Sr& rhs = exact(rhs_);

//     // exhaustive test:
//     mln_fwd_piter(Sl) pl(lhs);
//     mln_fwd_piter(Sr) pr(rhs);
//     for (pl.start(),      pr.start();
// 	 pl.is_valid() && pr.is_valid();
// 	 pl.next(),       pr.next())
//       if (pl != pr)
// 	return false; // difference found

//     // both sets are equal only if both browsings are completed
//     // at the same time:
//     return ! pl.is_valid() && ! pr.is_valid();
    return util::yes(true);
  }


  template <typename Sl, typename Sr>
  inline
  util::yes operator<=(const Site_Set<Sl>&, const Site_Set<Sr>&)
  {
//     // FIXME: Same grid!
//     const Sl& lhs = exact(lhs_);
//     const Sr& rhs = exact(rhs_);

//     // exhaustive test:
//     mln_piter(Sl) pl(lhs);
//     for_all(pl)
//       if (! rhs.has(pl))
// 	return false;

    return util::yes(true);
  }


  template <typename Sl, typename Sr>
  inline
  util::yes operator<(const Site_Set<Sl>&, const Site_Set<Sr>&)
  {
//     // FIXME: Same grid!
//     const Sl& lhs = exact(lhs_);
//     const Sr& rhs = exact(rhs_);
//     return lhs <= rhs && lhs != rhs;
    return util::yes(true);
  }


  template <typename S>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Site_Set<S>& set_)
  {
    const S& set = exact(set_);
    ostr << '{';
    mln_piter(S) p(set);
    for_all(p)
      ostr << p;
    return ostr << '}';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/routine/ops.hh>


#endif // ! MLN_CORE_CONCEPT_SITE_SET_HH