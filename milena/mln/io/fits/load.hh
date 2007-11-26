// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#ifndef MLN_IO_FITS_LOAD_HH
# define MLN_IO_FITS_LOAD_HH

/*!
 * \file   mln/io/fits/load.hh
 *
 * \brief Define a function which loads an image of kind fits with
 * given path.
 *
 */

# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image2d.hh>
# include <mln/value/int_u8.hh>
# include <fitsio.h>

namespace mln
{

  namespace io
  {

    namespace fits
    {
      void fits_exit(int status)
      {
	if (status)
	{
	  fits_report_error(stderr, status);
	  exit(status);
	}
	return;
      }

      image2d<float> load(const std::string& filename)
      {
	fitsfile *fptr;
	int status,  nfound, anynull;
	long naxes[2];
	float nullval;

	status = 0;
	if (fits_open_file(&fptr, filename.c_str(), READONLY, &status))
	  fits_exit(status);

	if (fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxes, &nfound, &status))
	  fits_exit(status);

	const int ncols = naxes[0], nrows = naxes[1];

	image2d<float> output(nrows, ncols);

	nullval  = 0; // don't check null values

	point2d p(make::point2d(0, 0));

	for (p.row() = 0; p.row() < nrows; ++p.row())
	{
	  if (fits_read_img(fptr,
			    TFLOAT,
			    1 + p.row() * ncols,
			    ncols,
			    &nullval,
			    &(output(p)),
			    &anynull,
			    &status))
	    fits_exit(status);
	}

	if (fits_close_file(fptr, &status))
	  fits_exit(status);

	return output;
      }

    } // end of namespace mln::io::fits

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_FITS_LOAD_HH
