// Copyright (C) 2006, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_TRAIT_SOLVE_UNARY_HH
# define MLN_TRAIT_SOLVE_UNARY_HH

/*!
 * \file   mln/trait/solve_unary.hh
 *
 * \brief  FIXME
 *
 *
 */

# include <mln/core/category.hh>
# include <mln/core/routine/exact.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/ret.hh>


// FIXME: Just for the record (use it...)

#  ifndef MLN_DEBUG_TRAITS 
#  endif // ! MLN_DEBUG_TRAITS



namespace mln
{

  namespace trait
  {

    namespace internal
    {


      template < template <class> class Name,
		 typename Category,
		 typename T >
      struct trait_set_unary_;

      template < template <class> class Name,
		 template <class> class Category, typename _,
		 typename T >
      struct trait_set_unary_< Name, Category<_>, T >
      {
	typedef typename mln::trait::set_unary_<Name, Category, T>::ret ret;
      };


      // Fwd decls.
      template < template <class> class Name,
		 typename Category, typename T >
      struct get_unary_;


      template < typename user_ret, /* != not_found and != undefined */
		 template <class> class Name,
		 typename Category, typename T >
      struct helper_get_unary_
      {
	typedef user_ret ret;  // The user has defined 'ret' so we return it.
      };


      template < template <class> class Name,
		 typename Category, typename T >
      struct helper_get_unary_< /* user_ret == */ not_found,
				Name, Category, T >
      {
	typedef not_found ret;  // End of search due to a blocker; 'ret' is not found. 
      };


      template < template <class> class Name,
		 typename Category, typename T >
      struct helper_get_unary_< /* user_ret == */ undefined,
				Name, Category, T >
      {
	typedef typename mln::internal::super_category_< Category, T >::ret Super_Category;
	typedef typename get_unary_<Name, Super_Category, T>::ret ret;   // No user ret definition => Recursion.
      };


      template < template <class> class Name,
		 typename Category, typename T >
      struct get_unary_
      {
	typedef typename trait_set_unary_<Name, Category, T>::ret user_ret;  // First get 'user_ret'
	typedef helper_get_unary_<user_ret, Name, Category, T> helper;       // Set the helper to make a decision.
	typedef mlc_ret(helper) ret;                                         // Return.
      };
    

      template < typename precise_ret,
		 template <class> class Name,
		 typename Category, typename T >
      struct helper_choose_unary_wrt_ /* precise_ret != undefined */
      {
	typedef precise_ret ret;                                 // -> A precise ret has been defined so it is it.
      };

      template < template <class> class Name,
		 typename Category, typename T >
      struct helper_choose_unary_wrt_< /* precise_ret == */ undefined,
				       Name, Category, T >
      {
	typedef typename get_unary_<Name, Category, T>::ret ret; // -> Go up into the category inheritance
	                                                         //    to fetch a ret from 'set_unary_'s.
      };

      template < template <class> class Name,
		 typename Category, typename T >
      struct helper_solve_unary_
      {
	typedef typename set_precise_unary_<Name, T>::ret precise_ret;
	typedef helper_choose_unary_wrt_< precise_ret, /* undefined or not (?) */
					  Name, Category, T> helper;
	typedef mlc_ret(helper) ret;
      };

    } // end of namespace mln::trait::internal


    template < template <class> class Name,
	       typename T_ >
    struct solve_unary
    {
      typedef mln_exact(T_) T;
      typedef typename mln::category<T>::ret Category;
      typedef internal::helper_solve_unary_< Name, Category, T > meta_code;
      typedef typename meta_code::ret ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_SOLVE_UNARY_HH