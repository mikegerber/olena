// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef METALIC_ARRAY_3D_HH
# define METALIC_ARRAY_3D_HH

# include <mlc/array/objs.hh>
# include <mlc/contract.hh>
# include <mlc/cmp.hh>
# include <ntg/basics.hh>

# include <iostream>

// impl
# include <mlc/array/3d.hxx>

namespace mlc
{

  class not_implemented_yet{}; // FIXME

  template<class Info_, class T_>
  struct array3d
  {
    typedef array3d self;
    typedef T_ T;
    typedef Info_ Info;

    //
    // Constructors
    //

    array3d()
    {
    }

    array3d(T* ptr)
    {
      less< 0, Info_::nplanes >::ensure();
      less< 0, Info_::nrows >::ensure();
      less< 0, Info_::ncols >::ensure();
      less< Info_::card, internal::max_card_ >::ensure();
      for (unsigned i = 0; i < Info_::card; ++i)
	buffer_[i] = *ptr++;
    }

    // Copy

    array3d(const self& rhs)
    {
      for (unsigned i = 0; i < Info_::card; ++i)
	buffer_[i] = rhs[i];
    }
    self& operator=(const self& rhs)
    {
      for (unsigned i = 0; i < Info_::card; ++i)
	buffer_[i] = rhs[i];
      return *this;
    }


    // Name

    static std::string
    name()
    {
      return std::string("array3d< Info, ") + ntg_name(T) + " >";
    }



    //
    // Operations on array
    //

    // Normalize (absolute values -> relative values)

    array3d<Info_, ntg::float_s>
    normalize()
    {
      array3d<Info_, ntg::float_s> tmp;
      ntg::float_s sum = 0.f;
      const ntg::float_s epsilon = 0.01f; // FIXME : epsilon should be global
      unsigned i;
      for (i = 0; i < Info_::card; ++i)
	sum += this->buffer_[i];
      for (i = 0; i < Info_::card; ++i)
	tmp[i] = this->buffer_[i] / sum;
      // security
      sum = 0.f;
      for (i = 0; i < Info_::card; ++i)
	sum += tmp[i];
      postcondition(std::abs(sum - 1) <= epsilon);
      return tmp;
    }

    // Central symmetry

    array3d<array3d_info<Info_::nplanes,
			 Info_::nrows,
			 Info_::ncols,
			 Info_::card - Info_::center - 1,
			 Info_::i>, T>
    operator-() const
    {
      enum { new_center =  Info_::card - Info_::center - 1 };
      array3d<array3d_info< Info_::nplanes, Info_::nrows, Info_::ncols, new_center, Info_::i>,T> tmp;

      for (unsigned i = 0; i < Info_::card; ++i)
	tmp[Info_::card - i - 1] = this->operator[](i);
      return tmp;
    }


    // Transpose

    array3d<Info, T> transpose() const // FIXME
    {
      std::cerr << "[31m===> 3D transposition not implemented yet. <===[0m" << std::endl;
      throw not_implemented_yet();
    }


    // Operators

    // FIXME: This code should be factorized between 1d, 2d and 3d.
    // Think of a mechanism similar to apply() and apply2().

    template <class U>
    array3d< Info, ntg_return_type(times, T, U) >
    operator*(U w)
    {
      array3d< Info, ntg_return_type(times, T, U) > tmp;
      for (unsigned i = 0; i < Info::card; ++i)
	tmp[i] = this->buffer_[i] * w;
      return tmp;
    }

    template <class U>
    array3d< Info, ntg_return_type(div, T, U) >
    operator/(U w)
    {
      array3d< Info, ntg_return_type(div, T, U) > tmp;
      for (unsigned i = 0; i < Info::card; ++i)
	tmp[i] = this->buffer_[i] / w;
      return tmp;
    }

    self operator+(const self& rhs) const
    {
      self tmp;
      for (unsigned i = 0; i < Info::card; ++i)
	tmp[i] = this->buffer_[i] + rhs.buffer_[i];
      return tmp;
    }
    self& operator+=(const self& rhs)
    {
      for (unsigned i = 0; i < Info::card; ++i)
	this->buffer_[i] += rhs.buffer_[i];
      return *this;
    }

    self operator-(const self& rhs) const
    {
      self tmp;
      for (unsigned i = 0; i < Info::card; ++i)
	tmp[i] = this->buffer_[i] - rhs.buffer_[i];
      return tmp;
    }
    self& operator-=(const self& rhs)
    {
      for (unsigned i = 0; i < Info::card; ++i)
	this->buffer_[i] -= rhs.buffer_[i];
      return *this;
    }



    //      template<class U> int operator,(U); // FIXME: why this?

    //
    // Accessors
    //

    unsigned size() const
    {
      return Info_::card;
    }

    const T* buffer() const
    {
      return buffer_;
    }

    // dynamic accessors:

    T operator[](unsigned i) const	// Absolute position
    {
      precondition(i < Info_::card);
      return *(buffer_ + i);
    }
    T& operator[](unsigned i)
    {
      precondition(i < Info_::card);
      return *(buffer_ + i);
    }


    T operator()(int plane, int row, int col) const		// Relative position
    {
      precondition(-Info_::center_plane <= plane);
      precondition(plane <= Info::nplanes - Info_::center_plane - 1);
      precondition(-Info_::center_row <= row);
      precondition(row <= Info_::nrows - Info_::center_row - 1);
      precondition(-Info_::center_col <= col);
      precondition(col <= Info_::ncols - Info_::center_col - 1);

      return *(buffer_ + Info_::center + (plane * Info::nrows * Info::ncols) + (row * Info::ncols) + col);
    }

    T& operator()(int plane, int row, int col)
    {
      precondition(-Info_::center_plane <= plane);
      precondition(plane <= Info::nplanes - Info_::center_plane - 1);
      precondition(-Info_::center_row <= row);
      precondition(row <= Info_::nrows - Info_::center_row - 1);
      precondition(-Info_::center_col <= col);
      precondition(col <= Info_::ncols - Info_::center_col - 1);

      return *(buffer_ + Info_::center + (plane * Info::nrows * Info::ncols) + (row * Info::ncols) + col);
    }


    // do not use these methods...

    template<unsigned i>
    T get_at_() const {
      lesseq<i, Info_::card>::ensure();
      return *(buffer_ + i);
    }

    template<int nplane, int nrow, int ncol>
    T get_() const {
      lesseq< -Info_::center_plane, nplane >::ensure();
      lesseq< nplane, Info::nplanes - Info_::center_plane - 1 >::ensure();
      lesseq< -Info_::center_row, nrow >::ensure();
      lesseq< nrow, Info_::nrows - Info_::center_row - 1 >::ensure();
      lesseq< -Info_::center_col, ncol >::ensure();
      lesseq< ncol, Info_::ncols - Info_::center_col - 1 >::ensure();
      return *(buffer_ + Info_::center + (nplane * Info::nrows * Info::ncols) + (nrow * Info::ncols) + ncol);
    }


  protected:

    T buffer_[internal::max_card_];
  };


  // ...but these static accessors:

  template<unsigned i, class Info, class T> inline
  T get_at(const array3d<Info, T>& arr)
  {
    return arr.template get_at_<i>();
  }

  template<int plane, int row, int col, class Info, class T> inline
  T get(const array3d<Info, T>& arr)
  {
    return arr.template get_<plane, row, col>();
  }

  // starter objects

  // FIXME: what about other types? Replace this by a function
  // returning a starter.

# define array3d_starter(T) \
  static internal::array3d_start_<T >   T##s_3d   = internal::array3d_start_<T >()

  array3d_starter(int);		// ints_3d
  array3d_starter(float);	// floats_3d


  // print

  template<class Info, class T>
  std::ostream& operator<<(std::ostream& ostr, const array3d<Info, T>& rhs)
  {
    for (int i = 0; i < Info::card; ++i)
      {
	if (i == Info::center)
	  ostr << "<" << rhs[i] << "> ";
	else
	  ostr << rhs[i] << " ";

	ostr << ((i + 1) % Info::ncols == 0 ? "\n" : "\t");
	ostr << ((i + 1) % (Info::ncols * Info::nrows) == 0 ? "\n" : "");
      }
    ostr << std::flush;

    return ostr;
  }


} // end of mlc

#endif // ! METALIC_ARRAY_3D_HH