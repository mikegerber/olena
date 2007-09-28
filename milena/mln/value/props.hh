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

#ifndef MLN_VALUE_PROPS_HH
# define MLN_VALUE_PROPS_HH

/*! \file mln/value/props.hh
 *
 * \brief Define properties of value types.
 *  FIXME : add interop typedef in each props
 */

# include <cstddef>
# include <climits>
# include <cfloat>

# include <mln/core/macros.hh>
# include <mln/value/kind.hh>
# include <mln/metal/bool.hh>
# include <mln/metal/vec.hh>
# include <mln/metal/mat.hh>
# include <mln/metal/binary_arith_trait.hh>


/// Get the minimum value of type \c T.
# define mln_min(T) mln::value::props< T >::min()


/// Get the maximum value of type \c T.
# define mln_max(T) mln::value::props< T >::max()


/// Get the number of values for value type \c T.
# define mln_card_(T) mln::value::props< T >::card_


/// Get the kind of value type \c T.
# define mln_kind(T) typename mln::value::props< T >::kind


/// Test is the value type \c T is low quantized.
# define mln_is_lowq(T) typename metal::bool_<( mln_card_(T) != 0 )>::type


/// Give the summation type for values of type \c T.
# define mln_sum(T) typename mln::value::props< T >::sum



namespace mln
{

  namespace value
  {


    /*! Class that defines the properties (by default) of any value
     * type \c T.
     */
    template <typename T>
    struct props
    {
      typedef data_kind kind;
      static const std::size_t card_ = 0;
    };


    namespace internal
    {

      template <typename T>
      struct convert_
      {
	static T value_at_index(std::size_t i)
	{
	  return mln_min(T) + i;
	}
	static std::size_t index_of_value(const T& v)
	{
	  return v - mln_min(T);
	}
      };

    } // end of namespace mln::value::internal


    // bool

    template <>
    struct props<bool>
    {
      static const bool min() { return false; }
      static const bool max() { return true; }
      static const std::size_t card_ = 2;
      typedef binary_kind kind;
    };


    // integers

    template <>
    struct props<unsigned char>
    {
      static const unsigned char min() { return   0; }
      static const unsigned char max() { return 255; }
      static const std::size_t  card_ = 256;
      typedef data_kind kind;
      typedef float sum;
    };

    template <>
    struct props<signed char>
    {
      static const signed char  min() { return -128; }
      static const signed char  max() { return  127; }
      static const std::size_t card_ =  256;
      typedef data_kind kind;
      typedef float sum;
    };

    template <>
    struct props<unsigned short>
    {
      static const unsigned short min() { return     0; }
      static const unsigned short max() { return 65535; }
      static const std::size_t   card_ = 65536;
      typedef data_kind kind;
      typedef float sum;
    };

    template <>
    struct props<signed short>
    {
      static const signed short min() { return -32768; }
      static const signed short max() { return  32767; }
      static const std::size_t card_ = 65536;
      typedef data_kind kind;
      typedef float sum;
    };

    template <>
    struct props<unsigned int>
    {
      static const unsigned int min() { return 0; }
      static const unsigned int max() { return UINT_MAX; }
      typedef data_kind kind;
      static const std::size_t card_ = 0;
      typedef float sum;
    };

    template <>
    struct props<signed int>
    {
      static const signed int  min() { return INT_MIN; }
      static const signed int  max() { return INT_MAX; }
      typedef data_kind kind;
      static const std::size_t card_ = 0;
      typedef float sum;
    };

    template <>
    struct props<unsigned long int>
    {
      static const unsigned long int min() { return 0; }
      static const unsigned long int max() { return ULONG_MAX; }
      typedef data_kind kind;
      static const std::size_t card_ = 0;
      typedef float sum;
    };

    template <>
    struct props<signed long int>
    {
      static const signed long int min() { return LONG_MIN; }
      static const signed long int max() { return LONG_MAX; }
      typedef data_kind kind;
      static const std::size_t card_ = 0;
      typedef float sum;
    };


    // floating

    template <>
    struct props<float>
    {
      static const float min() { return FLT_MIN; }
      static const float max() { return FLT_MAX; }
      static const float epsilon() { return 0.00001f; }
      typedef data_kind kind;
      static const std::size_t card_ = 0;
      typedef float sum;
    };

    template <>
    struct props<double>
    {
      static const double min() { return DBL_MIN; }
      static const double max() { return DBL_MAX; }
      static const double epsilon() { return 0.0000001; }
      typedef data_kind kind;
      static const std::size_t card_ = 0;
      typedef double sum;
    };

    // records

    template <unsigned n, typename T>
    struct props<metal::vec<n,T> >
    {
	static const metal::vec<n,T> min() { return make::vec<n>(mln_min(T)); }
	static const metal::vec<n,T> max() { return make::vec<n>(mln_max(T)); }
	typedef data_kind kind;
	static const std::size_t card_ = n * mln_card_(T);
	typedef mlc_bin_arith(float,T) sum;
    };

    template <unsigned n, unsigned m, typename T>
    struct props<metal::mat<n,m,T> >
    {
	static const metal::mat<n,m,T> min() { return make::mat<n,m>(mln_min(T)); }
	static const metal::mat<n,m,T> max() { return make::mat<n,m>(mln_max(T)); }
	typedef data_kind kind;
	static const std::size_t card_ = n * m * mln_card_(T);
	typedef mlc_bin_arith(float,T) sum;
    };

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROPS_HH
