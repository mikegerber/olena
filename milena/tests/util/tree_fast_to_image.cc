// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/util/tree_fast.hh>
#include <mln/core/contract.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/stretch.hh>
#include <mln/data/fill.hh>
#include <mln/data/compare.hh>
#include <mln/io/pgm/save.hh>
#include <vector>
#include <mln/util/tree_fast_to_image.hh>
#include <mln/debug/println.hh>


template <typename P, typename V>
struct fllt_node
{
  V	value;
  mln::p_set<P> points;
  mln::p_set<P> holes;
};


int main()
{
  using namespace mln;
  using value::int_u8;

  typedef fllt_node<point2d , int_u8>  I;

  I s1;
  I s2;
  I s3;
  I s4;
  I s5;
  I s6;
  I s7;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      s1.points.insert(point2d(i, j));
  s1.value = 60;
  for (int i = 8; i < 16; ++i)
    for (int j = 0; j < 4; ++j)
      s2.points.insert(point2d(i, j));
  s1.value = 100;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      s3.points.insert(point2d(i, j));
  s3.value = 110;
  for (int i = 10; i < 12; ++i)
    for (int j = 0; j < 2; ++j)
      s4.points.insert(point2d(i, j));
  s4.value = 170;
  for (int i = 8; i < 16; ++i)
    for (int j = 0; j < 2; ++j)
      s5.points.insert(point2d(i, j));
  s5.value = 180;
  for (int i = 8; i < 16; ++i)
    for (int j = 2; j < 4; ++j)
      s6.points.insert(point2d(i, j));
  s6.value = 210;
  for (int i = 0; i < 16; ++i)
    for (int j = 0; j < 8; ++j)
      s7.points.insert(point2d(i, j));
  s7.value = 10;

  util::tree_fast<I> tree(s1);
  tree.add_child(tree.root_, s2);
  tree.add_child(tree.root_, s3);
  tree.add_child(tree.root_, s4);
  tree.add_child(tree.root_, s5);
  tree.add_child(tree.root_, s6);
  tree.add_parent(s7);
  image2d<int_u8> output (16, 16);
  data::fill(output, 0);
  util::tree_fast_to_image(tree, output);

  int_u8 vs[16][16] = {

    {110, 110, 110, 110, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {110, 110, 110, 110, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {110, 110, 110, 110, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {110, 110, 110, 110, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    { 10,  10,  10,  10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    { 10,  10,  10,  10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    { 10,  10,  10,  10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    { 10,  10,  10,  10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0},
    {180, 180, 210, 210, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0}

  };

  debug::println(output);
  image2d<int_u8> ref (make::image(vs));
  mln_assertion(ref == output);
}
