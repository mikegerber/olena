// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/fun/v2i/index_of_value.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_s8.hh>


int main()
{
  using namespace mln;

  {
    fun::v2i::index_of_value<bool> f;
    mln_assertion(f(false) == 0);
    mln_assertion(f(true)  == 1);
  }
  {
    fun::v2i::index_of_value< value::int_u8 > f;
    mln_assertion(f(  0) ==   0);
    mln_assertion(f(255) == 255);
  }
  {
    fun::v2i::index_of_value< value::int_s8 > f;
    mln_assertion(f(-127) ==   0);
    mln_assertion(f(+127) == 254);
  }
}
