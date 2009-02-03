// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_CLOUD_SAVE_HH
# define MLN_IO_CLOUD_SAVE_HH

/// \file mln/io/cloud/save.hh
///
/// Save an image as a cloud of points.

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_array.hh>

namespace mln
{

  namespace io
  {

    namespace cloud
    {

      /// Load a cloud of points
      ///
      /// \param[in] arr the cloud of points to save.
      /// \param[in] filename the destination.
      template <typename P>
      void save(const p_array<P>& arr, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename P>
        inline
        void save_data(const p_array<P>& arr, std::ofstream& file)
	{
	  std::ostringstream sline;
	  mln_piter(p_array<P>) p(arr);
	  for_all(p)
          {
	    algebra::vec<P::dim,float> v = p.to_site().to_vec();
	    sline << v[0];
	    for (unsigned i = 1; i < P::dim; ++i)
	      sline << ' ' << v[i];
	    sline << std::endl;
	    file << sline.str();
	  }
	}

      } // end of namespace mln::io::cloud::internal



      template <typename P>
      void save(const p_array<P>& arr, const std::string& filename)
      {
	trace::entering("mln::io::cloud::save");

	std::ofstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	internal::save_data(arr, file);

	trace::exiting("mln::io::cloud::save");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::cloud

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_CLOUD_SAVE_HH