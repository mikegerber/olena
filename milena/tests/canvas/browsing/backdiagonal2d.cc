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

#include <mln/core/image/image2d.hh>
#include <mln/canvas/browsing/backdiagonal2d.hh>
#include <mln/fun/p2v/iota.hh>
#include <mln/debug/println.hh>


template <typename I_, typename F>
struct assign_browsing_functor
{
  typedef I_ I;
  typedef mln_site(I) S;
  enum { dim = S::dim };

  typedef assign_browsing_functor<I, F> self;
  typedef mln_deduce(I, psite, delta) dpsite;
  typedef void (assign_browsing_functor<I,F>::*move_fun)();

  I input;
  F f;

  assign_browsing_functor(I& input, F f = F())
    : input(input),
      f(f)
  {
  }

  mln_psite(I) p;

  void init()  { std::cout << "init" << std::endl; }
  void final() { std::cout << "final" << std::endl; }

  void next_()
  {
    input(p) = f(p);
  }
  void next()  { std::cout << "next : " << p << std::endl; next_(); }
  void init_diag()  { std::cout << "init_diag: " << p << std::endl; }
};

namespace mln
{

  template <typename I, typename F, typename B>
  void my_test(Image<I>& ima_,
	       const Function_v2v<F>& f_,
	       const Browsing<B>& browse_)
  {
    I& ima = exact(ima_);
    const F& f = exact(f_);
    const B& browse = exact(browse_);

    assign_browsing_functor<I, F> fun(ima, f);
    browse(fun);
  }

}


int main()
{
  using namespace mln;
  image2d<unsigned> ima2(10, 10);

  std::cout << ima2.bbox() << std::endl;
  my_test(ima2, fun::p2v::iota(), canvas::browsing::backdiagonal2d);
  debug::println(ima2);
}
